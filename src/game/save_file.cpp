#include <ultra64.h>

#include "sm64.h"
#include "game.h"
#include "main.h"
#include "engine/math_util.h"
#include "area.h"
#include "level_update.h"
#include "save_file.h"
#include "sound_init.h"
#include "level_table.h"

#define MENU_DATA_MAGIC 0x4849
#define SAVE_FILE_MAGIC 0x4441

static_assert(sizeof(struct SaveBuffer) == EEPROM_SIZE, "eeprom buffer size must match");

extern struct SaveBuffer gSaveBuffer;

struct WarpCheckpoint gWarpCheckpoint;

s8 gMainMenuDataModified;
s8 gSaveFileModified;

u8 gLastCompletedCourseNum     = 0;
u8 gLastCompletedStarNum       = 0;
s8 sUnusedGotGlobalCoinHiScore = 0;
u8 gGotFileCoinHiScore	       = 0;
u8 gCurrCourseStarFlags	       = 0;

u8 buYosshiJump = 0;

#define STUB_LEVEL(_0, _1, courseenum, _3, _4, _5, _6, _7, _8) courseenum,
#define DEFINE_LEVEL(_0, _1, courseenum, _3, _4, _5, _6, _7, _8, _9, _10) courseenum,

s8 gLevelToCourseNumTable[] = {
#include "levels/level_defines.h"
};
#undef STUB_LEVEL
#undef DEFINE_LEVEL

static_assert(ARRAY_COUNT(gLevelToCourseNumTable) == LEVEL_COUNT - 1, "change this array if you are adding levels");

// This was probably used to set progress to 100% for debugging, but
// it was removed from the release ROM.
static void no_op(void)
{
	UNUSED s32 pad;
}

/**
 * Read from EEPROM to a given address.
 * The EEPROM address is computed using the offset of the destination address from gSaveBuffer.
 * Try at most 4 times, and return 0 on success. On failure, return the status returned from
 * osEepromLongRead. It also returns 0 if EEPROM isn't loaded correctly in the system.
 */
static s32 read_eeprom_data(void* buffer, s32 size)
{
	s32 status = 0;

	s32 triesLeft = 4;
	u32 offset    = (u32)((u8*)buffer - (u8*)&gSaveBuffer) / 8;

	do
	{
		triesLeft--;
		status = osEepromLongRead(&gSIEventMesgQueue, offset, (u8*)buffer, size);
	} while(triesLeft > 0 && status != 0);

	return status;
}

/**
 * Write data to EEPROM.
 * The EEPROM address is computed using the offset of the source address from gSaveBuffer.
 * Try at most 4 times, and return 0 on success. On failure, return the status returned from
 * osEepromLongWrite. Unlike read_eeprom_data, return 1 if EEPROM isn't loaded.
 */
static s32 write_eeprom_data(void* buffer, s32 size)
{
	s32 status = 1;

	s32 triesLeft = 4;
	u32 offset    = (u32)((u8*)buffer - (u8*)&gSaveBuffer) >> 3;

	do
	{
		triesLeft--;
		status = osEepromLongWrite(&gSIEventMesgQueue, offset, (u8*)buffer, size);
	} while(triesLeft > 0 && status != 0);

	return status;
}

/**
 * Sum the bytes in data to data + size - 2. The last two bytes are ignored
 * because that is where the checksum is stored.
 */
static s32 calc_checksum(u8* data, s32 size)
{
	u16 chksum = 0;

	while(size-- > 2)
	{
		chksum += *data++;
	}
	return chksum;
}

/**
 * Verify the signature at the end of the block to check if the data is valid.
 */
static s32 verify_save_block_signature(void* buffer, s32 size, u16 magic)
{
	struct SaveBlockSignature* sig = (struct SaveBlockSignature*)((size - 4) + (u8*)buffer);

	if(sig->magic != magic)
	{
		return FALSE;
	}
	if(sig->chksum != calc_checksum((u8*)buffer, size))
	{
		return FALSE;
	}
	return TRUE;
}

/**
 * Write a signature at the end of the block to make sure the data is valid
 */
static void add_save_block_signature(void* buffer, s32 size, u16 magic)
{
	struct SaveBlockSignature* sig = (struct SaveBlockSignature*)((size - 4) + (u8*)buffer);

	sig->magic  = magic;
	sig->chksum = calc_checksum((u8*)buffer, size);
}

/**
 * Copy main menu data from one backup slot to the other slot.
 */
static void restore_main_menu_data(s32 srcSlot)
{
	s32 destSlot = srcSlot ^ 1;

	// Compute checksum on source data
	add_save_block_signature(&gSaveBuffer.menuData[srcSlot], sizeof(gSaveBuffer.menuData[srcSlot]), MENU_DATA_MAGIC);

	// Copy source data to destination
	bcopy(&gSaveBuffer.menuData[srcSlot], &gSaveBuffer.menuData[destSlot], sizeof(gSaveBuffer.menuData[destSlot]));

	// Write destination data to EEPROM
	write_eeprom_data(&gSaveBuffer.menuData[destSlot], sizeof(gSaveBuffer.menuData[destSlot]));
}

static void save_main_menu_data(void)
{
	if(gMainMenuDataModified)
	{
		// Compute checksum
		add_save_block_signature(&gSaveBuffer.menuData[0], sizeof(gSaveBuffer.menuData[0]), MENU_DATA_MAGIC);

		// Back up data
		bcopy(&gSaveBuffer.menuData[0], &gSaveBuffer.menuData[1], sizeof(gSaveBuffer.menuData[1]));

		// Write to EEPROM
		write_eeprom_data(gSaveBuffer.menuData, sizeof(gSaveBuffer.menuData));

		gMainMenuDataModified = FALSE;
	}
}

static void wipe_main_menu_data(void)
{
	bzero(&gSaveBuffer.menuData[0], sizeof(gSaveBuffer.menuData[0]));

	// Set score ages for all courses to 3, 2, 1, and 0, respectively.
	gSaveBuffer.menuData[0].coinScoreAges[0] = 0x3FFFFFFF;
	gSaveBuffer.menuData[0].coinScoreAges[1] = 0x2AAAAAAA;
	gSaveBuffer.menuData[0].coinScoreAges[2] = 0x15555555;

	gMainMenuDataModified = TRUE;
	save_main_menu_data();
}

static s32 get_coin_score_age(s32 fileIndex, s32 courseIndex)
{
	return (gSaveBuffer.menuData[0].coinScoreAges[fileIndex] >> (2 * courseIndex)) & 0x3;
}

static void set_coin_score_age(s32 fileIndex, s32 courseIndex, s32 age)
{
	s32 mask = 0x3 << (2 * courseIndex);

	gSaveBuffer.menuData[0].coinScoreAges[fileIndex] &= ~mask;
	gSaveBuffer.menuData[0].coinScoreAges[fileIndex] |= age << (2 * courseIndex);
}

/**
 * Mark a coin score for a save file as the newest out of all save files.
 */
static void touch_coin_score_age(s32 fileIndex, s32 courseIndex)
{
	s32 i;
	u32 age;
	u32 currentAge = get_coin_score_age(fileIndex, courseIndex);

	if(currentAge != 0)
	{
		for(i = 0; i < NUM_SAVE_FILES; i++)
		{
			age = get_coin_score_age(i, courseIndex);
			if(age < currentAge)
			{
				set_coin_score_age(i, courseIndex, age + 1);
			}
		}

		set_coin_score_age(fileIndex, courseIndex, 0);
		gMainMenuDataModified = TRUE;
	}
}

/**
 * Mark all coin scores for a save file as new.
 */
static void touch_high_score_ages(s32 fileIndex)
{
	s32 i;

	for(i = 0; i < 15; i++)
	{
		touch_coin_score_age(fileIndex, i);
	}
}

/**
 * Copy save file data from one backup slot to the other slot.
 */
static void restore_save_file_data(s32 fileIndex, s32 srcSlot)
{
	s32 destSlot = srcSlot ^ 1;

	// Compute checksum on source data
	add_save_block_signature(&gSaveBuffer.files[fileIndex][srcSlot], sizeof(gSaveBuffer.files[fileIndex][srcSlot]), SAVE_FILE_MAGIC);

	// Copy source data to destination slot
	bcopy(&gSaveBuffer.files[fileIndex][srcSlot], &gSaveBuffer.files[fileIndex][destSlot], sizeof(gSaveBuffer.files[fileIndex][destSlot]));

	// Write destination data to EEPROM
	write_eeprom_data(&gSaveBuffer.files[fileIndex][destSlot], sizeof(gSaveBuffer.files[fileIndex][destSlot]));
}

void BuWriteStorage(s32 fileIndex)
{
	if(gSaveFileModified)
	{
		// Compute checksum
		add_save_block_signature(&gSaveBuffer.files[fileIndex][0], sizeof(gSaveBuffer.files[fileIndex][0]), SAVE_FILE_MAGIC);

		// Copy to backup slot
		bcopy(&gSaveBuffer.files[fileIndex][0], &gSaveBuffer.files[fileIndex][1], sizeof(gSaveBuffer.files[fileIndex][1]));

		// Write to EEPROM
		write_eeprom_data(gSaveBuffer.files[fileIndex], sizeof(gSaveBuffer.files[fileIndex]));

		gSaveFileModified = FALSE;
	}

	save_main_menu_data();
}

void BuClearStorage(s32 fileIndex)
{
	touch_high_score_ages(fileIndex);
	bzero(&gSaveBuffer.files[fileIndex][0], sizeof(gSaveBuffer.files[fileIndex][0]));

	gSaveFileModified = TRUE;
	BuWriteStorage(fileIndex);
}

void BuCopyStorage(s32 srcFileIndex, s32 destFileIndex)
{
	UNUSED s32 pad;

	touch_high_score_ages(destFileIndex);
	bcopy(&gSaveBuffer.files[srcFileIndex][0], &gSaveBuffer.files[destFileIndex][0], sizeof(gSaveBuffer.files[destFileIndex][0]));

	gSaveFileModified = TRUE;
	BuWriteStorage(destFileIndex);
}

void BuInitBackUp(void)
{
	s32 file;
	s32 validSlots;

	gMainMenuDataModified = FALSE;
	gSaveFileModified     = FALSE;

	bzero(&gSaveBuffer, sizeof(gSaveBuffer));
	read_eeprom_data(&gSaveBuffer, sizeof(gSaveBuffer));

	// Verify the main menu data and create a backup copy if only one of the slots is valid.
	validSlots = verify_save_block_signature(&gSaveBuffer.menuData[0], sizeof(gSaveBuffer.menuData[0]), MENU_DATA_MAGIC);
	validSlots |= verify_save_block_signature(&gSaveBuffer.menuData[1], sizeof(gSaveBuffer.menuData[1]), MENU_DATA_MAGIC) << 1;
	switch(validSlots)
	{
		case 0: // Neither copy is correct
			wipe_main_menu_data();
			break;
		case 1: // Slot 0 is correct and slot 1 is incorrect
			restore_main_menu_data(0);
			break;
		case 2: // Slot 1 is correct and slot 0 is incorrect
			restore_main_menu_data(1);
			break;
	}

	for(file = 0; file < NUM_SAVE_FILES; file++)
	{
		// Verify the save file and create a backup copy if only one of the slots is valid.
		validSlots = verify_save_block_signature(&gSaveBuffer.files[file][0], sizeof(gSaveBuffer.files[file][0]), SAVE_FILE_MAGIC);
		validSlots |= verify_save_block_signature(&gSaveBuffer.files[file][1], sizeof(gSaveBuffer.files[file][1]), SAVE_FILE_MAGIC) << 1;
		switch(validSlots)
		{
			case 0: // Neither copy is correct
				BuClearStorage(file);
				break;
			case 1: // Slot 0 is correct and slot 1 is incorrect
				restore_save_file_data(file, 0);
				break;
			case 2: // Slot 1 is correct and slot 0 is incorrect
				restore_save_file_data(file, 1);
				break;
		}
	}

	no_op();
}

/**
 * Reload the current save file from its backup copy, which is effectively a
 * a cached copy of what has been written to EEPROM.
 * This is used after getting a game over.
 */
void BuRestoreBackUp(void)
{
	// Copy save file data from backup
	bcopy(&gSaveBuffer.files[activePlayerNo - 1][1], &gSaveBuffer.files[activePlayerNo - 1][0], sizeof(gSaveBuffer.files[activePlayerNo - 1][0]));

	// Copy main menu data from backup
	bcopy(&gSaveBuffer.menuData[1], &gSaveBuffer.menuData[0], sizeof(gSaveBuffer.menuData[0]));

	gMainMenuDataModified = FALSE;
	gSaveFileModified     = FALSE;
}

/**
 * Update the current save file after collecting a star or a key.
 * If coin score is greater than the current high score, update it.
 */
void BuStoreBackUp(s16 coinScore, s16 starIndex)
{
	s32 fileIndex	= activePlayerNo - 1;
	s32 courseIndex = activeCourseNo - 1;

	s32 starFlag	 = 1 << starIndex;
	UNUSED s32 flags = BuGetItemFlag();

	gLastCompletedCourseNum	    = courseIndex + 1;
	gLastCompletedStarNum	    = starIndex + 1;
	sUnusedGotGlobalCoinHiScore = 0;
	gGotFileCoinHiScore	    = 0;

	if(courseIndex >= 0 && courseIndex < COURSE_STAGES_COUNT)
	{
		//! Compares the coin score as a 16 bit value, but only writes the 8 bit
		// truncation. This can allow a high score to decrease.

		if(coinScore > ((u16)BuGetWinScore(courseIndex) & 0xFFFF))
		{
			sUnusedGotGlobalCoinHiScore = 1;
		}

		if(coinScore > BuGetNumCoins(fileIndex, courseIndex))
		{
			gSaveBuffer.files[fileIndex][0].courseCoinScores[courseIndex] = coinScore;
			touch_coin_score_age(fileIndex, courseIndex);

			gGotFileCoinHiScore = 1;
			gSaveFileModified   = TRUE;
		}
	}

	switch(activeStageNo)
	{
		case LEVEL_BOWSER_1:
			if(!(BuGetItemFlag() & (SAVE_FLAG_HAVE_KEY_1 | SAVE_FLAG_UNLOCKED_BASEMENT_DOOR)))
			{
				BuSetItemFlag(SAVE_FLAG_HAVE_KEY_1);
			}
			break;

		case LEVEL_BOWSER_2:
			if(!(BuGetItemFlag() & (SAVE_FLAG_HAVE_KEY_2 | SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR)))
			{
				BuSetItemFlag(SAVE_FLAG_HAVE_KEY_2);
			}
			break;

		case LEVEL_BOWSER_3:
			break;

		default:
			if(!(BuGetStarFlag(fileIndex, courseIndex) & starFlag))
			{
				BuSetStarFlag(fileIndex, courseIndex, starFlag);
			}
			break;
	}
}

s32 BuIsActive(s32 fileIndex)
{
	return (gSaveBuffer.files[fileIndex][0].flags & SAVE_FLAG_FILE_EXISTS) != 0;
}

/**
 * Get the maximum coin score across all files for a course. The lower 16 bits
 * of the returned value are the score, and the upper 16 bits are the file number
 * of the save file with this score.
 */
u32 BuGetWinScore(s32 courseIndex)
{
	s32 fileIndex;
	s32 maxCoinScore    = -1;
	s32 maxScoreAge	    = -1;
	s32 maxScoreFileNum = 0;

	for(fileIndex = 0; fileIndex < NUM_SAVE_FILES; fileIndex++)
	{
		if(BuGetStarFlag(fileIndex, courseIndex) != 0)
		{
			s32 coinScore = BuGetNumCoins(fileIndex, courseIndex);
			s32 scoreAge  = get_coin_score_age(fileIndex, courseIndex);

			if(coinScore > maxCoinScore || (coinScore == maxCoinScore && scoreAge > maxScoreAge))
			{
				maxCoinScore	= coinScore;
				maxScoreAge	= scoreAge;
				maxScoreFileNum = fileIndex + 1;
			}
		}
	}
	return (maxScoreFileNum << 16) + MAX(maxCoinScore, 0);
}

s32 BuGetNumStars(s32 fileIndex, s32 courseIndex)
{
	s32 i;
	s32 count    = 0;
	u8 flag	     = 1;
	u8 starFlags = BuGetStarFlag(fileIndex, courseIndex);

	for(i = 0; i < 7; i++, flag <<= 1)
	{
		if(starFlags & flag)
		{
			count++;
		}
	}
	return count;
}

s32 BuGetSumStars(s32 fileIndex, s32 minCourse, s32 maxCourse)
{
	s32 count = 0;

	// Get standard course star count.
	for(; minCourse <= maxCourse; minCourse++)
	{
		count += BuGetNumStars(fileIndex, minCourse);
	}

	// Add castle secret star count.
	return BuGetNumStars(fileIndex, -1) + count;
}

void BuSetItemFlag(s32 flags)
{
	gSaveBuffer.files[activePlayerNo - 1][0].flags |= (flags | SAVE_FLAG_FILE_EXISTS);
	gSaveFileModified = TRUE;
}

void BuClrItemFlag(s32 flags)
{
	gSaveBuffer.files[activePlayerNo - 1][0].flags &= ~flags;
	gSaveBuffer.files[activePlayerNo - 1][0].flags |= SAVE_FLAG_FILE_EXISTS;
	gSaveFileModified = TRUE;
}

s32 BuGetItemFlag(void)
{
	if(snEndingScene != 0 || autoDemoPtr != NULL)
	{
		return 0;
	}
	return gSaveBuffer.files[activePlayerNo - 1][0].flags;
}

/**
 * Return the bitset of obtained stars in the specified course.
 * If course is -1, return the bitset of obtained castle secret stars.
 */
s32 BuGetStarFlag(s32 fileIndex, s32 courseIndex)
{
	s32 starFlags;

	if(courseIndex == -1)
	{
		starFlags = (gSaveBuffer.files[fileIndex][0].flags >> 24) & 0x7F;
	}
	else
	{
		starFlags = gSaveBuffer.files[fileIndex][0].courseStars[courseIndex] & 0x7F;
	}

	return starFlags;
}

/**
 * Add to the bitset of obtained stars in the specified course.
 * If course is -1, add ot the bitset of obtained castle secret stars.
 */
void BuSetStarFlag(s32 fileIndex, s32 courseIndex, s32 starFlags)
{
	if(courseIndex == -1)
	{
		gSaveBuffer.files[fileIndex][0].flags |= starFlags << 24;
	}
	else
	{
		gSaveBuffer.files[fileIndex][0].courseStars[courseIndex] |= starFlags;
	}

	gSaveBuffer.files[fileIndex][0].flags |= SAVE_FLAG_FILE_EXISTS;
	gSaveFileModified = TRUE;
}

s32 BuGetNumCoins(s32 fileIndex, s32 courseIndex)
{
	return gSaveBuffer.files[fileIndex][0].courseCoinScores[courseIndex];
}

/**
 * Return TRUE if the cannon is unlocked in the current course.
 */
s32 BuGetCannonFlag(void)
{
	return (gSaveBuffer.files[activePlayerNo - 1][0].courseStars[activeCourseNo] & 0x80) != 0;
}

/**
 * Sets the cannon status to unlocked in the current course.
 */
void BuSetCannonFlag(void)
{
	gSaveBuffer.files[activePlayerNo - 1][0].courseStars[activeCourseNo] |= 0x80;
	gSaveBuffer.files[activePlayerNo - 1][0].flags |= SAVE_FLAG_FILE_EXISTS;
	gSaveFileModified = TRUE;
}

void BuSetHatPosition(s16 x, s16 y, s16 z)
{
	struct SaveFile* saveFile = &gSaveBuffer.files[activePlayerNo - 1][0];

	saveFile->capLevel = activeStageNo;
	saveFile->capArea  = activeSceneNo;
	SetSVector(saveFile->capPos, x, y, z);
	BuSetItemFlag(SAVE_FLAG_CAP_ON_GROUND);
}

s32 BuGetHatPosition(Vec3s& capPos)
{
	struct SaveFile* saveFile = &gSaveBuffer.files[activePlayerNo - 1][0];
	s32 flags		  = BuGetItemFlag();

	if(saveFile->capLevel == activeStageNo && saveFile->capArea == activeSceneNo && (flags & SAVE_FLAG_CAP_ON_GROUND))
	{
		vec3s_copy(capPos, saveFile->capPos);
		return TRUE;
	}
	return FALSE;
}

void BuSetSoundMode(u16 mode)
{
	set_sound_mode(mode);
	gSaveBuffer.menuData[0].soundMode = mode;

	gMainMenuDataModified = TRUE;
	save_main_menu_data();
}

u16 BuGetSoundMode(void)
{
	return gSaveBuffer.menuData[0].soundMode;
}

void BuRestoreHat(void)
{
	if(BuGetItemFlag() & SAVE_FLAG_CAP_ON_GROUND)
	{
		switch(gSaveBuffer.files[activePlayerNo - 1][0].capLevel)
		{
			case LEVEL_SSL:
				BuSetItemFlag(SAVE_FLAG_CAP_ON_KLEPTO);
				break;
			case LEVEL_SL:
				BuSetItemFlag(SAVE_FLAG_CAP_ON_MR_BLIZZARD);
				break;
			case LEVEL_TTM:
				BuSetItemFlag(SAVE_FLAG_CAP_ON_UKIKI);
				break;
		}
		BuClrItemFlag(SAVE_FLAG_CAP_ON_GROUND);
	}
}

void disable_warp_checkpoint(void)
{
	// BuGetMidPoint() checks to see if gWarpCheckpoint.courseNum != 0
	gWarpCheckpoint.courseNum = 0;
}

/**
 * Checks the upper bit of the WarpNode->destLevel byte to see if the
 * game should set a warp checkpoint.
 */
void BuSetMidPoint(struct WarpNode* warpNode)
{
	if(warpNode->destLevel & 0x80)
	{
		// Overwrite the warp checkpoint variables.
		gWarpCheckpoint.actNum	  = activeLevelNo;
		gWarpCheckpoint.courseNum = activeCourseNo;
		gWarpCheckpoint.levelID	  = warpNode->destLevel & 0x7F;
		gWarpCheckpoint.areaNum	  = warpNode->destArea;
		gWarpCheckpoint.warpNode  = warpNode->destNode;
	}
}

/**
 * Checks to see if a checkpoint is properly active or not. This will
 * also update the level, area, and destination node of the input WarpNode.
 * returns TRUE if input WarpNode was updated, and FALSE if not.
 */
s32 BuGetMidPoint(struct WarpNode* warpNode)
{
	s16 isWarpCheckpointActive = FALSE;
	s16 currCourseNum	   = gLevelToCourseNumTable[(warpNode->destLevel & 0x7F) - 1];

	// ramSaveCourse is only used in this function.
	if(gWarpCheckpoint.courseNum != 0 && ramSaveCourse == currCourseNum && gWarpCheckpoint.actNum == activeLevelNo)
	{
		warpNode->destLevel    = gWarpCheckpoint.levelID;
		warpNode->destArea     = gWarpCheckpoint.areaNum;
		warpNode->destNode     = gWarpCheckpoint.warpNode;
		isWarpCheckpointActive = TRUE;
	}
	else
	{
		// Disable the warp checkpoint just incase the other 2 conditions failed?
		gWarpCheckpoint.courseNum = 0;
	}

	return isWarpCheckpointActive;
}
