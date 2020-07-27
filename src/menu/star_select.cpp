#include <ultra64.h>

#include "sm64.h"
#include "audio/external.h"
#include "game/game.h"
#include "game/memory.h"
#include "game/area.h"
#include "game/save_file.h"
#include "game/object_helpers.h"
#include "game/ingame_menu.h"
#include "game/level_update.h"
#include "game/segment2.h"
#include "game/segment7.h"
#include "game/object_list_processor.h"
#include "engine/behavior_script.h"
#include "engine/graph_node.h"
#include "behavior_data.h"
#include "text_strings.h"
#include "star_select.h"
#include "game/motor.h"

/**
 * @file star_select.c
 * This file implements how the star select screen (act selector) function.
 * That includes handles what stars can be selected, star selector types,
 * strings, act values, and star selector model rendering if a star is collected or not.
 */

// Star Selector count models printed in the act selector menu.
static struct Object* sStarSelectorModels[8];

// The act the course is loaded as, affects whether some objects spawn.
s8 sLoadedActNum;

// Number of obtained stars, excluding the coin star.
static u8 sObtainedStars;

// Total number of stars that appear in the act selector menu.
static s8 sVisibleStars;

// Act selected when the act menu is first opened.
static u8 sInitSelectedActNum;

// Index value of the act selected in the act menu.
static s8 sSelectedActIndex = 0;

// Index value of the star that is selectable in the act menu.
// Excluding the next star, it doesn't count other transparent stars.
static s8 sSelectableStarIndex = 0;

// Act Selector menu timer that keeps counting until you choose an act.
s32 sActSelectorMenuTimer = 0;

/**
 * Act Selector Star Type Loop Action
 * Defines a select type for a star in the act selector.
 */
void bhv_act_selector_star_type_loop(void)
{
	switch(gCurrentObject->oStarSelectorType)
	{
		// If a star is not selected, don't rotate or change size
		case STAR_SELECTOR_NOT_SELECTED:
			gCurrentObject->oStarSelectorSize -= 0.1;
			if(gCurrentObject->oStarSelectorSize < 1.0)
			{
				gCurrentObject->oStarSelectorSize = 1.0;
			}
			gCurrentObject->oFaceAngleYaw = 0;
			break;
			// If a star is selected, rotate and slightly increase size
		case STAR_SELECTOR_SELECTED:
			gCurrentObject->oStarSelectorSize += 0.1 * FRAME_RATE_SCALER;
			if(gCurrentObject->oStarSelectorSize > 1.3)
			{
				gCurrentObject->oStarSelectorSize = 1.3;
			}
			gCurrentObject->oFaceAngleYaw += 0x800 / FRAME_RATE_SCALER_INV;
			break;
			// If the 100 coin star is selected, rotate
		case STAR_SELECTOR_100_COINS:
			gCurrentObject->oFaceAngleYaw += 0x800 / FRAME_RATE_SCALER_INV;
			break;
	}
	// Scale act selector stars depending of the type selected
	s_set_scale(gCurrentObject->oStarSelectorSize);
	// Unused timer, only referenced here. Probably replaced by sActSelectorMenuTimer
	gCurrentObject->oStarSelectorTimer++;
}

/**
 * Renders the 100 coin star with an special star selector type.
 */
void render_100_coin_star(u8 stars)
{
	if(stars & (1 << 6))
	{
		// If the 100 coin star has been collected, create a new star selector next to the coin score.
		sStarSelectorModels[6]			  = s_makeobj_absolute(gCurrentObject, 0, MODEL_STAR, sm64::bhv::bhvActSelectorStarType(), 370, 24, -300, 0, 0, 0);
		sStarSelectorModels[6]->oStarSelectorSize = 0.8;
		sStarSelectorModels[6]->oStarSelectorType = STAR_SELECTOR_100_COINS;
	}
}

/**
 * Act Selector Init Action
 * Checks how many stars has been obtained in a course, to render
 * the correct star models, the 100 coin star and also handles
 * checks of what star should be next in sInitSelectedActNum.
 */
void bhv_act_selector_init(void)
{
	s16 i = 0;
	s32 selectorModelIDs[10];
	u8 stars = BuGetStarFlag(activePlayerNo - 1, activeCourseNo - 1);

	sVisibleStars = 0;
	while(i != sObtainedStars)
	{
		if(stars & (1 << sVisibleStars))
		{ // Star has been collected
			selectorModelIDs[sVisibleStars] = MODEL_STAR;
			i++;
		}
		else
		{ // Star has not been collected
			selectorModelIDs[sVisibleStars] = MODEL_TRANSPARENT_STAR;
			// If this is the first star that has not been collected, set
			// the default selection to this star.
			if(sInitSelectedActNum == 0)
			{
				sInitSelectedActNum  = sVisibleStars + 1;
				sSelectableStarIndex = sVisibleStars;
			}
		}
		sVisibleStars++;
	}

	// If the stars have been collected in order so far, show the next star.
	if(sVisibleStars == sObtainedStars && sVisibleStars != 6)
	{
		selectorModelIDs[sVisibleStars] = MODEL_TRANSPARENT_STAR;
		sInitSelectedActNum		= sVisibleStars + 1;
		sSelectableStarIndex		= sVisibleStars;
		sVisibleStars++;
	}

	// If all stars have been collected, set the default selection to the last star.
	if(sObtainedStars == 6)
	{
		sInitSelectedActNum = sVisibleStars;
	}

	//! Useless, since sInitSelectedActNum has already been set in this
	//! scenario by the code that shows the next uncollected star.
	if(sObtainedStars == 0)
	{
		sInitSelectedActNum = 1;
	}

	// Render star selector objects
	for(i = 0; i < sVisibleStars; i++)
	{
		sStarSelectorModels[i]			  = s_makeobj_absolute(gCurrentObject, 0, selectorModelIDs[i], sm64::bhv::bhvActSelectorStarType(), 75 + sVisibleStars * -75 + i * 152, 248, -300, 0, 0, 0);
		sStarSelectorModels[i]->oStarSelectorSize = 1.0f;
	}

	render_100_coin_star(stars);
}

/**
 * Act Selector Loop Action
 * Handles star selector scrolling depending of what stars are
 * selectable, whenever all 6 stars are obtained or not.
 * Also handles 2 star selector types whenever the star is selected
 * or not, the types are defined in bhv_act_selector_star_type_loop.
 */
void bhv_act_selector_loop()
{
	s8 i;
	u8 starIndexCounter;
	u8 stars = BuGetStarFlag(activePlayerNo - 1, activeCourseNo - 1);

	if(sObtainedStars != 6)
	{
		// Sometimes, stars are not selectable even if they appear on the screen.
		// This code filters selectable and non-selectable stars.
		sSelectedActIndex = 0;
		ContCursorEvent(MENU_SCROLL_HORIZONTAL, &sSelectableStarIndex, 0, sObtainedStars);
		starIndexCounter = sSelectableStarIndex;
		for(i = 0; i < sVisibleStars; i++)
		{
			// Can the star be selected (is it either already completed or the first non-completed mission)
			if((stars & (1 << i)) || i + 1 == sInitSelectedActNum)
			{
				if(starIndexCounter == 0)
				{ // We have reached the sSelectableStarIndex-th selectable star.
					sSelectedActIndex = i;
					break;
				}
				starIndexCounter--;
			}
		}
	}
	else
	{
		// If all stars are collected then they are all selectable.
		ContCursorEvent(MENU_SCROLL_HORIZONTAL, &sSelectableStarIndex, 0, sVisibleStars - 1);
		sSelectedActIndex = sSelectableStarIndex;
	}

	// Star selector type handler
	for(i = 0; i < sVisibleStars; i++)
	{
		if(sSelectedActIndex == i)
		{
			sStarSelectorModels[i]->oStarSelectorType = STAR_SELECTOR_SELECTED;
		}
		else
		{
			sStarSelectorModels[i]->oStarSelectorType = STAR_SELECTOR_NOT_SELECTED;
		}
	}
}

/**
 * Print the course number selected with the wood rgba16 course texture.
 */
static void print_course_number()
{
	u8 courseNum[4];

	iTranslate(MENU_MTX_PUSH, 158.0f, 81.0f, 0.0f);

	gSPDisplayList(gDisplayListHead++, dl_menu_rgba16_wood_course);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);

	I_itochar(activeCourseNo, courseNum);

	if(activeCourseNo < 10)
	{ // 1 digit number
		Draw16bitFont(HUD_LUT_GLOBAL, 152, 158, courseNum);
	}
	else
	{ // 2 digit number
		Draw16bitFont(HUD_LUT_GLOBAL, 143, 158, courseNum);
	}

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
}

#define ACT_NAME_X 163

/**
 * Print act selector strings, some with special checks.
 */
static void print_act_selector_strings()
{
	// TODO: EU relocates level and act name tables to translation segment 0x19
	unsigned char myScore[]	    = {TEXT_MYSCORE};
	unsigned char starNumbers[] = {TEXT_ZERO};
	u8** levelNameTbl	    = (u8**)segmented_to_virtual(seg2_course_name_table);
	u8* currLevelName	    = (u8*)segmented_to_virtual(levelNameTbl[activeCourseNo - 1]);
	u8** actNameTbl		    = (u8**)segmented_to_virtual(seg2_act_name_table);
	u8* selectedActName;
	s16 lvlNameX;
	s16 actNameX;
	s8 i;

	MakeSelectProjection();

	// Print the coin highscore.
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
	DrawMyScore(1, activePlayerNo - 1, activeCourseNo - 1, 155, 106);
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);

	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 255);
	// Print the "MY SCORE" text if the coin score is more than 0
	if(BuGetNumCoins(activePlayerNo - 1, activeCourseNo - 1) != 0)
	{
		Draw8bitFont(102, 118, myScore);
	}
	// Print the level name; add 3 to skip the number and spacing to get to the actual string to center.
	lvlNameX = CharCentering(160, currLevelName + 3, 10.0f);
	Draw8bitFont(lvlNameX, 33, currLevelName + 3);
	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);

	print_course_number();

	gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 255);
	// Print the name of the selected act.
	if(sVisibleStars != 0)
	{
		selectedActName = (u8*)segmented_to_virtual(actNameTbl[(activeCourseNo - 1) * 6 + sSelectedActIndex]);
		actNameX	= CharCentering(ACT_NAME_X, selectedActName, 8.0f);
		print_menu_generic_string(actNameX, 81, selectedActName);
	}

	// Print the numbers above each star.
	for(i = 1; i <= sVisibleStars; i++)
	{
		starNumbers[0] = i;
		print_menu_generic_string(i * 34 - sVisibleStars * 17 + 139, 38, starNumbers);
	}

	gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_end);
}

/**
 * Geo function that Print act selector strings.
 *!@bug: This geo function is missing the third param. Harmless in practice due to o32 convention.
 */
#ifdef AVOID_UB
Gfx* geo_act_selector_strings(s16 callContext, UNUSED struct GraphNode* node, UNUSED void* context)
{
#else
Gfx* geo_act_selector_strings(s16 callContext, UNUSED struct GraphNode* node)
{
#endif
	if(callContext == GEO_CONTEXT_RENDER)
	{
		print_act_selector_strings();
	}
	return NULL;
}

/**
 * Initiates act selector values before entering a main course.
 * Also load how much stars a course has, without counting the 100 coin star.
 */
s32 SeStarSelectInitProc(s32 arg, s32 unused)
{
	u8 stars = BuGetStarFlag(activePlayerNo - 1, activeCourseNo - 1);

	sLoadedActNum	      = 0;
	sInitSelectedActNum   = 0;
	sVisibleStars	      = 0;
	sActSelectorMenuTimer = 0;
	sObtainedStars	      = BuGetNumStars(activePlayerNo - 1, activeCourseNo - 1);

	// Don't count 100 coin star
	if(stars & (1 << 6))
	{
		sObtainedStars--;
	}

	//! no return value
#ifdef AVOID_UB
	return 0;
#endif
}

/**
 * Loads act selector button actions with selected act value checks.
 * Also updates objects and returns act number selected after is choosen.
 */
s32 SeStarSelectProcess(s32 arg, s32 unused)
{
	if(sActSelectorMenuTimer >= 11 * FRAME_RATE_SCALER_INV)
	{
		// If any of these buttons are pressed, play sound and go to course act
		if((sm64::player(0).controller().buttonPressed & sm64::hid::A_BUTTON) || (sm64::player(0).controller().buttonPressed & sm64::hid::START_BUTTON) || (sm64::player(0).controller().buttonPressed & sm64::hid::B_BUTTON))
		{
			AudStartSound(SOUND_MENU_STAR_SOUND_LETS_A_GO, gDefaultSoundArgs);
			SendMotorEvent(60, 70);
			SendMotorDecay(1);

			if(sInitSelectedActNum > sSelectedActIndex)
			{
				sLoadedActNum = sSelectedActIndex + 1;
			}
			else
			{
				sLoadedActNum = sInitSelectedActNum;
			}
			gDialogCourseActNum   = sSelectedActIndex + 1;
			sActSelectorMenuTimer = -1;
		}
	}

	SnExecuteStrategy();
	sActSelectorMenuTimer++;
	return sLoadedActNum;
}
