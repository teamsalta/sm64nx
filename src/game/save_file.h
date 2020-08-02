#ifndef _SAVE_FILE_H_
#define _SAVE_FILE_H_

#include "types.h"
#include "area.h"

#include "course_table.h"

#define EEPROM_SIZE 0x200
#define NUM_SAVE_FILES 4

struct SaveBlockSignature
{
	u16 magic;
	u16 chksum;
};

struct SaveFile
{
	// Location of lost cap.
	// Note: the coordinates get set, but are never actually used, since the
	// cap can always be found in a fixed spot within the course
	u8 capLevel;
	u8 capArea;
	Vec3s capPos;

	u32 flags;

	// Star flags for each course.
	// The most significant bit of the byte *following* each course is set if the
	// cannon is open.
	u8 courseStars[COURSE_COUNT];

	u8 courseCoinScores[COURSE_STAGES_COUNT];

	struct SaveBlockSignature signature;
};

enum SaveFileIndex
{
	SAVE_FILE_A,
	SAVE_FILE_B,
	SAVE_FILE_C,
	SAVE_FILE_D
};

struct MainMenuSaveData
{
	// Each save file has a 2 bit "age" for each course. The higher this value,
	// the older the high score is. This is used for tie-breaking when displaying
	// on the high score screen.
	u32 coinScoreAges[NUM_SAVE_FILES];
	u16 soundMode;

#define SUBTRAHEND 6

	// Pad to match the EEPROM size of 0x200 (10 bytes on JP/US, 8 bytes on EU)
	u8 filler[EEPROM_SIZE / 2 - SUBTRAHEND - NUM_SAVE_FILES * (4 + sizeof(struct SaveFile))];

	struct SaveBlockSignature signature;
};

struct SaveBuffer
{
	// Each of the four save files has two copies. If one is bad, the other is used as a backup.
	struct SaveFile files[NUM_SAVE_FILES][2];
	// The main menu data has two copies. If one is bad, the other is used as a backup.
	struct MainMenuSaveData menuData[2];
};

struct WarpNode;

extern u8 gLastCompletedCourseNum;
extern u8 gLastCompletedStarNum;
extern s8 sUnusedGotGlobalCoinHiScore;
extern u8 gGotFileCoinHiScore;
extern u8 gCurrCourseStarFlags;
extern u8 buYosshiJump;
extern s8 gLevelToCourseNumTable[];

// game progress flags
#define SAVE_FLAG_FILE_EXISTS /* 0x000001 */ (1 << 0)
#define SAVE_FLAG_HAVE_WING_CAP /* 0x000002 */ (1 << 1)
#define SAVE_FLAG_HAVE_METAL_CAP /* 0x000004 */ (1 << 2)
#define SAVE_FLAG_HAVE_VANISH_CAP /* 0x000008 */ (1 << 3)
#define SAVE_FLAG_HAVE_KEY_1 /* 0x000010 */ (1 << 4)
#define SAVE_FLAG_HAVE_KEY_2 /* 0x000020 */ (1 << 5)
#define SAVE_FLAG_UNLOCKED_BASEMENT_DOOR /* 0x000040 */ (1 << 6)
#define SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR /* 0x000080 */ (1 << 7)
#define SAVE_FLAG_DDD_MOVED_BACK /* 0x000100 */ (1 << 8)
#define SAVE_FLAG_MOAT_DRAINED /* 0x000200 */ (1 << 9)
#define SAVE_FLAG_UNLOCKED_PSS_DOOR /* 0x000400 */ (1 << 10)
#define SAVE_FLAG_UNLOCKED_WF_DOOR /* 0x000800 */ (1 << 11)
#define SAVE_FLAG_UNLOCKED_CCM_DOOR /* 0x001000 */ (1 << 12)
#define SAVE_FLAG_UNLOCKED_JRB_DOOR /* 0x002000 */ (1 << 13)
#define SAVE_FLAG_UNLOCKED_BITDW_DOOR /* 0x004000 */ (1 << 14)
#define SAVE_FLAG_UNLOCKED_BITFS_DOOR /* 0x008000 */ (1 << 15)
#define SAVE_FLAG_CAP_ON_GROUND /* 0x010000 */ (1 << 16)
#define SAVE_FLAG_CAP_ON_KLEPTO /* 0x020000 */ (1 << 17)
#define SAVE_FLAG_CAP_ON_UKIKI /* 0x040000 */ (1 << 18)
#define SAVE_FLAG_CAP_ON_MR_BLIZZARD /* 0x080000 */ (1 << 19)
#define SAVE_FLAG_UNLOCKED_50_STAR_DOOR /* 0x100000 */ (1 << 20)

// Variable for setting a warp checkpoint.

// possibly a WarpDest struct where arg is a union. TODO: Check?
struct WarpCheckpoint
{
	/*0x00*/ u8 actNum;
	/*0x01*/ u8 courseNum;
	/*0x02*/ u8 levelID;
	/*0x03*/ u8 areaNum;
	/*0x04*/ u8 warpNode;
};

extern struct WarpCheckpoint gWarpCheckpoint;

extern s8 gMainMenuDataModified;
extern s8 gSaveFileModified;

void BuWriteStorage(s32 fileIndex);
void BuClearStorage(s32 fileIndex);
void BuCopyStorage(s32 srcFileIndex, s32 destFileIndex);

void BuInitBackUp(void);
void BuRestoreBackUp(void);
void BuStoreBackUp(s16 coinScore, s16 starIndex);

s32 BuIsActive(s32 fileIndex);
u32 BuGetWinScore(s32 courseIndex);
s32 BuGetNumStars(s32 fileIndex, s32 courseIndex);
s32 BuGetSumStars(s32 fileIndex, s32 minCourse, s32 maxCourse);
void BuSetItemFlag(s32 flags);
void BuClrItemFlag(s32 flags);
s32 BuGetItemFlag(void);
s32 BuGetStarFlag(s32 fileIndex, s32 courseIndex);
void BuSetStarFlag(s32 fileIndex, s32 courseIndex, s32 starFlags);
s32 BuGetNumCoins(s32 fileIndex, s32 courseIndex);
s32 BuGetCannonFlag(void);
void BuSetCannonFlag(void);
void BuSetHatPosition(s16 x, s16 y, s16 z);
s32 BuGetHatPosition(Vec3s& capPos);
void BuSetSoundMode(u16 mode);
u16 BuGetSoundMode(void);
void BuRestoreHat(void);

void disable_warp_checkpoint(void);
void BuSetMidPoint(struct WarpNode* a);
s32 BuGetMidPoint(struct WarpNode* a);

#endif
