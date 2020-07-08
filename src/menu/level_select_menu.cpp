#include <ultra64.h>

#include "sm64.h"
#include "audio/external.h"
#include "game/game.h"
#include "game/main.h"
#include "game/memory.h"
#include "game/area.h"
#include "game/save_file.h"
#include "game/level_update.h"
#include "game/sound_init.h"
#include "game/print.h"
#include "game/display.h"
#include "seq_ids.h"
#include "engine/math_util.h"
#include "level_table.h"

#define PRESS_START_DEMO_TIMER 800

#define STUB_LEVEL(textname, _1, _2, _3, _4, _5, _6, _7, _8) textname,
#define DEFINE_LEVEL(textname, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) textname,

static char gLevelSelect_StageNamesText[64][16] = {
#include "levels/level_defines.h"
};
#undef STUB_LEVEL
#undef DEFINE_LEVEL

static u16 gDemoCountdown = 0;
#ifndef VERSION_JP
static s16 D_U_801A7C34	     = 1;
static s16 gameOverNotPlayed = 1;
#endif

// run the demo timer on the PRESS START screen.
// this function will return a non-0 timer once
// the demo starts, signaling to the subsystem that
// the demo needs to be ran.

// don't shift this function from being the first function in the segment.
// the level scripts assume this function is the first, so it cant be moved.
int run_press_start_demo_timer(s32 timer)
{
	gCurrDemoInput = NULL;

	if(timer == 0)
	{
		if(!sm64::player(0).controller().buttonDown && !sm64::player(0).controller().stickMag)
		{
			if((++gDemoCountdown) == PRESS_START_DEMO_TIMER * FRAME_RATE_SCALER_INV)
			{
				// start the demo. 800 frames has passed while
				// player is idle on PRESS START screen.

				// start the mario demo animation for the demo list.
				func_80278AD4(&gDemo, gDemoInputListID);

				// if the next demo sequence ID is the count limit, reset it back to
				// the first sequence.
				if(++gDemoInputListID == gDemo.animDmaTable->count)
				{
					gDemoInputListID = 0;
				}

				// add 1 (+4) to the pointer to skip the demoID.
				gCurrDemoInput	 = ((struct DemoInput*)gDemo.targetAnim) + 1;
				timer		 = (s8)((struct DemoInput*)gDemo.targetAnim)->timer * FRAME_RATE_SCALER_INV;
				gCurrSaveFileNum = 1;
				gCurrActNum	 = 1;
			}
		}
		else
		{
			// activity was detected, so reset the demo countdown.
			gDemoCountdown = 0;
		}
	}
	return timer;
}

extern int gDemoInputListID_2;
extern int gPressedStart;

int start_demo(int timer)
{
	gCurrDemoInput = NULL;
	gPressedStart  = 0;
	// start the mario demo animation for the demo list.
	func_80278AD4(&gDemo, gDemoInputListID_2);

	// if the next demo sequence ID is the count limit, reset it back to
	// the first sequence.

	if((++gDemoInputListID_2) == gDemo.animDmaTable->count)
	{
		gDemoInputListID_2 = 0;
	}

	gCurrDemoInput = ((struct DemoInput*)gDemo.targetAnim) + 1; // add 1 (+4) to the pointer to skip the demoID.
	gCurrDemoInput->timer *= FRAME_RATE_SCALER_INV;

	timer		 = (s8)((struct DemoInput*)gDemo.targetAnim)->timer * FRAME_RATE_SCALER_INV; // TODO: see if making timer s8 matches
	gCurrSaveFileNum = 1;
	gCurrActNum	 = 6;
	return timer;
}

// input loop for the level select menu. updates the selected stage
// count if an input was received. signals the stage to be started
// or the level select to be exited if start or the quit combo is
// pressed.

s16 level_select_input_loop(void)
{
	s32 stageChanged = FALSE;

	// perform the ID updates per each button press.
	if(sm64::player(0).controller().buttonPressed & sm64::hid::A_BUTTON)
	{
		++gCurrLevelNum, stageChanged = TRUE;
	}

	if(sm64::player(0).controller().buttonPressed & sm64::hid::B_BUTTON)
	{
		--gCurrLevelNum, stageChanged = TRUE;
	}

	if(sm64::player(0).controller().buttonPressed & sm64::hid::U_JPAD)
	{
		--gCurrLevelNum, stageChanged = TRUE;
	}

	if(sm64::player(0).controller().buttonPressed & sm64::hid::D_JPAD)
	{
		++gCurrLevelNum, stageChanged = TRUE;
	}

	if(sm64::player(0).controller().buttonPressed & sm64::hid::L_JPAD)
	{
		gCurrLevelNum -= 10, stageChanged = TRUE;
	}

	if(sm64::player(0).controller().buttonPressed & sm64::hid::R_JPAD)
	{
		gCurrLevelNum += 10, stageChanged = TRUE;
	}

	// if the stage was changed, play the sound for changing a stage.
	if(stageChanged)
	{
		play_sound(SOUND_GENERAL_LEVEL_SELECT_CHANGE, gDefaultSoundArgs);
	}

	// TODO: enum counts for the stage lists
	if(gCurrLevelNum > LEVEL_MAX)
	{
		gCurrLevelNum = LEVEL_MIN; // exceeded max. set to min.
	}

	if(gCurrLevelNum < LEVEL_MIN)
	{
		gCurrLevelNum = LEVEL_MAX; // exceeded min. set to max.
	}

	gCurrSaveFileNum = 4; // file 4 is used for level select tests
	gCurrActNum	 = 6;
	print_text_centered(160, 80, "SELECT STAGE");
	print_text_centered(160, 30, "PRESS START BUTTON");
	print_text_fmt_int(40, 60, "%2d", gCurrLevelNum);
	print_text(80, 60, gLevelSelect_StageNamesText[gCurrLevelNum - 1]); // print stage name

#define QUIT_LEVEL_SELECT_COMBO (sm64::hid::Z_TRIG | sm64::hid::START_BUTTON | sm64::hid::L_CBUTTONS | sm64::hid::R_CBUTTONS)

	// start being pressed signals the stage to be started. that is, unless...
	if(sm64::player(0).controller().buttonPressed & sm64::hid::START_BUTTON)
	{
		// ... the level select quit combo is being pressed, which uses START. If this
		// is the case, quit the menu instead.
		if(sm64::player(0).controller().buttonDown == QUIT_LEVEL_SELECT_COMBO)
		{
			return -1;
		}
		play_sound(SOUND_MENU_STAR_SOUND, gDefaultSoundArgs);
		return gCurrLevelNum;
	}
	return 0;
}

int func_8016F444(void)
{
	s32 sp1C = 0;

	if(gameOverNotPlayed == 1)
	{
		play_sound(SOUND_MARIO_GAME_OVER, gDefaultSoundArgs);
		gameOverNotPlayed = 0;
	}

	print_intro_text();

	if(sm64::player(0).controller().buttonPressed & sm64::hid::START_BUTTON)
	{
		play_sound(SOUND_MENU_STAR_SOUND, gDefaultSoundArgs);
		sp1C = 100;

		gameOverNotPlayed = 1;
	}
	return run_press_start_demo_timer(sp1C);
}

int func_8016F4BC(void)
{
	set_background_music(0, SEQ_SOUND_PLAYER, 0);
	play_sound(SOUND_MENU_COIN_ITS_A_ME_MARIO, gDefaultSoundArgs);
	return 1;
}

s32 LevelProc_8016F508(s16 arg1, s32 arg2)
{
	s32 retVar;

	switch(arg1)
	{
		case 0: // play into "its me mario" sound
			retVar = func_8016F4BC();
			break;
		case 2:
			retVar = func_8016F444();
			break;
		//case 1:
		case 3:
			retVar = level_select_input_loop();
			break; // useless break needed to match
	}
	return retVar;
}
