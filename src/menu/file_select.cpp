#include <ultra64.h>

#include "sm64.h"
#include "audio/external.h"
#include "game/game.h"
#include "game/ingame_menu.h"
#include "game/object_helpers.h"
#include "game/area.h"
#include "game/save_file.h"
#include "game/spawn_object.h"
#include "game/object_list_processor.h"
#include "game/segment2.h"
#include "game/segment7.h"
#include "game/print.h"
#include "engine/behavior_script.h"
#include "engine/graph_node.h"
#include "engine/math_util.h"
#include "behavior_data.h"
#include "text_strings.h"
#include "file_select.h"
#include "dialog_ids.h"
#include "game/motor.h"

/**
 * @file file_select.c
 * This file implements how the file select and it's menus render and function.
 * That includes button IDs rendered as object models, strings, hand cursor,
 * special menu messages and phases, button states and button clicked checks.
 */

// The current sound mode is automatically centered on US due to
// the large length difference between options.
static s16 sSoundTextX;

// Amount of main menu buttons defined in the code called by s_makeobj_relative.
// See file_select.h for the names in MenuButtonTypes.
static struct Object* sMainMenuButtons[32];

// Used to defined yes/no fade colors after a file is selected in the erase menu.
// sYesNoColor[0]: YES | sYesNoColor[1]: NO
static u8 sYesNoColor[2];

// The button that is selected when it is clicked.
static s8 selectNo = MENU_BUTTON_NONE;

// Whether we are on the main menu or one of the submenus.
static s8 Floor = MENU_LAYER_MAIN;

// Used for text opacifying. If it is below 250, it is constantly incremented.
static u8 alpha_value = 0;

// 2D position of the cursor on the screen.
// cursol_pos[0]: X | cursol_pos[1]: Y
static f32 cursol_pos[] = {0, 0};

// Determines which graphic to use for the cursor.
static s16 cursol_flag = 0;

// Equal to cursol_pos if the cursor gets clicked, {-10000, -10000} otherwise.
static s16 cursol_ptr[] = {-10000, -10000};

// Used for determining which file has been selected during copying and erasing.
static s8 click_file = -1;

// Whether to fade out text or not.
static s8 click_flag = FALSE;

// The message currently being displayed at the top of a menu.
static s8 click_mess = 0;

// Used for text fading. The alpha value of text is calculated as
// alpha_value - click_alpha.
static u8 click_alpha = 0;

// File select timer that keeps counting until it reaches 1000.
// Used to prevent buttons from being clickable as soon as a menu loads.
// Gets reset when you click an empty save, existing saves in copy and erase menus
// and when you click yes/no in the erase confirmation prompt.
static s16 click_counter = 0;

// Sound mode menu buttonID, has different values compared to gSoundMode in audio.
// 0: gSoundMode = 0 (Stereo) | 1: gSoundMode = 3 (Mono) | 2: gSoundMode = 1 (Headset)
static s8 sound_flag = 0;

// Tracks which button will be pressed in the erase confirmation prompt (yes/no).
static s8 yesNo_flag = MENU_ERASE_HOVER_NONE;

// Used for the copy menu, defines if the game as all 4 save slots with data.
// if TRUE, it doesn't allow copying more files.
static s8 sAllFilesExist = FALSE;

// Defines the value of the save slot selected in the menu.
// Mario A: 1 | Mario B: 2 | Mario C: 3 | Mario D: 4
s8 goTogame = 0;

// Which coin score mode to use when scoring files. 0 for local
// coin high score, 1 for high score across all files.
static s8 sScoreFileCoinScoreMode = 0;

static unsigned char textReturn[]		 = {TEXT_RETURN};
static unsigned char textViewScore[]		 = {TEXT_CHECK_SCORE};
static unsigned char textCopyFileButton[]	 = {TEXT_COPY_FILE_BUTTON};
static unsigned char textEraseFileButton[]	 = {TEXT_ERASE_FILE_BUTTON};
static unsigned char textSoundModes[][8]	 = {{TEXT_STEREO}, {TEXT_MONO}, {TEXT_HEADSET}};
static unsigned char textMarioA[]		 = {TEXT_FILE_MARIO_A};
static unsigned char textMarioB[]		 = {TEXT_FILE_MARIO_B};
static unsigned char textMarioC[]		 = {TEXT_FILE_MARIO_C};
static unsigned char textMarioD[]		 = {TEXT_FILE_MARIO_D};
static unsigned char textNew[]			 = {TEXT_NEW};
static unsigned char starIcon[]			 = {GLYPH_STAR, GLYPH_SPACE};
static unsigned char xIcon[]			 = {GLYPH_MULTIPLY, GLYPH_SPACE};
static unsigned char textSelectFile[]		 = {TEXT_SELECT_FILE};
static unsigned char textScore[]		 = {TEXT_SCORE};
static unsigned char textCopy[]			 = {TEXT_COPY};
static unsigned char textErase[]		 = {TEXT_ERASE};
static unsigned char textCheckFile[]		 = {TEXT_CHECK_FILE};
static unsigned char textNoSavedDataExists[]	 = {TEXT_NO_SAVED_DATA_EXISTS};
static unsigned char textCopyFile[]		 = {TEXT_COPY_FILE};
static unsigned char textCopyItToWhere[]	 = {TEXT_COPY_IT_TO_WHERE};
static unsigned char textNoSavedDataExistsCopy[] = {TEXT_NO_SAVED_DATA_EXISTS};
static unsigned char textCopyCompleted[]	 = {TEXT_COPYING_COMPLETED};
static unsigned char textSavedDataExists[]	 = {TEXT_SAVED_DATA_EXISTS};
static unsigned char textNoFileToCopyFrom[]	 = {TEXT_NO_FILE_TO_COPY_FROM};
static unsigned char textYes[]			 = {TEXT_YES};
static unsigned char textNo[]			 = {TEXT_NO};

/**
 * Yellow Background Menu Initial Action
 * Rotates the background at 180 grades and it's scale.
 * Although the scale is properly applied in the loop function.
 */
void beh_yellow_background_menu_init()
{
	gCurrentObject->oFaceAngleYaw	 = 0x8000;
	gCurrentObject->oMenuButtonScale = 9.0f;
}

/**
 * Yellow Background Menu Loop Action
 * Properly scales the background in the main menu.
 */
void beh_yellow_background_menu_loop()
{
	s_set_scale(9.0f);
}

/**
 * Check if a button was clicked.
 * depth = 200.0 for main menu, 22.0 for submenus.
 */
static s32 ButtonClickCheck(s16 x, s16 y, f32 depth)
{
	f32 a	 = 52.4213;
	f32 newX = ((f32)x * 160.0) / (a * depth);
	f32 newY = ((f32)y * 120.0) / (a * 3.0f / 4.0f * depth);
	s16 maxX = newX + 25.0f;
	s16 minX = newX - 25.0f;
	s16 maxY = newY + 21.0f;
	s16 minY = newY - 21.0f;

	if(cursol_ptr[0] < maxX && minX < cursol_ptr[0] && cursol_ptr[1] < maxY && minY < cursol_ptr[1])
	{
		return TRUE;
	}
	return FALSE;
}

/**
 * Grow from main menu, used by selecting files and menus.
 */
static void bhv_menu_button_growing_from_main_menu(struct Object* button)
{
	if(button->oMenuButtonTimer < 16 * FRAME_RATE_SCALER_INV)
	{
		button->oFaceAngleYaw += 0x800 / FRAME_RATE_SCALER_INV;
	}
	if(button->oMenuButtonTimer < 8 * FRAME_RATE_SCALER_INV)
	{
		button->oFaceAnglePitch += 0x800 / FRAME_RATE_SCALER_INV;
	}
	if(button->oMenuButtonTimer >= 8 * FRAME_RATE_SCALER_INV && button->oMenuButtonTimer < 16 * FRAME_RATE_SCALER_INV)
	{
		button->oFaceAnglePitch -= 0x800 / FRAME_RATE_SCALER_INV;
	}
	button->oParentRelativePosX -= button->oMenuButtonOrigPosX / 16.0 * FRAME_RATE_SCALER;
	button->oParentRelativePosY -= button->oMenuButtonOrigPosY / 16.0 * FRAME_RATE_SCALER;

	if(button->oPosZ < button->oMenuButtonOrigPosZ + 17800.0)
	{
		button->oParentRelativePosZ += 1112.5 * FRAME_RATE_SCALER;
	}

	button->oMenuButtonTimer++;
	if(button->oMenuButtonTimer == 16 * FRAME_RATE_SCALER_INV)
	{
		button->oParentRelativePosX = 0.0f;
		button->oParentRelativePosY = 0.0f;
		button->oMenuButtonState    = MENU_BUTTON_STATE_FULLSCREEN;
		button->oMenuButtonTimer    = 0;
	}
}

/**
 * Shrink back to main menu, used to return back while inside menus.
 */
static void bhv_menu_button_shrinking_to_main_menu(struct Object* button)
{
	if(button->oMenuButtonTimer < 16 * FRAME_RATE_SCALER_INV)
	{
		button->oFaceAngleYaw -= 0x800 / FRAME_RATE_SCALER_INV;
	}

	if(button->oMenuButtonTimer < 8 * FRAME_RATE_SCALER_INV)
	{
		button->oFaceAnglePitch -= 0x800 / FRAME_RATE_SCALER_INV;
	}

	if(button->oMenuButtonTimer >= 8 * FRAME_RATE_SCALER_INV && button->oMenuButtonTimer < 16 * FRAME_RATE_SCALER_INV)
	{
		button->oFaceAnglePitch += 0x800 / FRAME_RATE_SCALER_INV;
	}

	button->oParentRelativePosX += button->oMenuButtonOrigPosX / 16.0 * FRAME_RATE_SCALER;
	button->oParentRelativePosY += button->oMenuButtonOrigPosY / 16.0 * FRAME_RATE_SCALER;

	if(button->oPosZ > button->oMenuButtonOrigPosZ)
	{
		button->oParentRelativePosZ -= 1112.5 * FRAME_RATE_SCALER;
	}

	button->oMenuButtonTimer++;
	if(button->oMenuButtonTimer == 16 * FRAME_RATE_SCALER_INV)
	{
		button->oParentRelativePosX = button->oMenuButtonOrigPosX;
		button->oParentRelativePosY = button->oMenuButtonOrigPosY;
		button->oMenuButtonState    = MENU_BUTTON_STATE_DEFAULT;
		button->oMenuButtonTimer    = 0;
	}
}

/**
 * Grow from submenu, used by selecting a file in the score menu.
 */
static void bhv_menu_button_growing_from_submenu(struct Object* button)
{
	if(button->oMenuButtonTimer < 16 * FRAME_RATE_SCALER_INV)
	{
		button->oFaceAngleYaw += 0x800 / FRAME_RATE_SCALER_INV;
	}

	if(button->oMenuButtonTimer < 8 * FRAME_RATE_SCALER_INV)
	{
		button->oFaceAnglePitch += 0x800 / FRAME_RATE_SCALER_INV;
	}

	if(button->oMenuButtonTimer >= 8 * FRAME_RATE_SCALER_INV && button->oMenuButtonTimer < 16 * FRAME_RATE_SCALER_INV)
	{
		button->oFaceAnglePitch -= 0x800 / FRAME_RATE_SCALER_INV;
	}

	button->oParentRelativePosX -= button->oMenuButtonOrigPosX / 16.0 * FRAME_RATE_SCALER;
	button->oParentRelativePosY -= button->oMenuButtonOrigPosY / 16.0 * FRAME_RATE_SCALER;
	button->oParentRelativePosZ -= 116.25 * FRAME_RATE_SCALER;
	button->oMenuButtonTimer++;
	if(button->oMenuButtonTimer == 16 * FRAME_RATE_SCALER_INV)
	{
		button->oParentRelativePosX = 0.0f;
		button->oParentRelativePosY = 0.0f;
		button->oMenuButtonState    = MENU_BUTTON_STATE_FULLSCREEN;
		button->oMenuButtonTimer    = 0;
	}
}

/**
 * Shrink back to submenu, used to return back while inside a score save menu.
 */
static void bhv_menu_button_shrinking_to_submenu(struct Object* button)
{
	if(button->oMenuButtonTimer < 16 * FRAME_RATE_SCALER_INV)
	{
		button->oFaceAngleYaw -= 0x800 / FRAME_RATE_SCALER_INV;
	}
	if(button->oMenuButtonTimer < 8 * FRAME_RATE_SCALER_INV)
	{
		button->oFaceAnglePitch -= 0x800 / FRAME_RATE_SCALER_INV;
	}
	if(button->oMenuButtonTimer >= 8 * FRAME_RATE_SCALER_INV && button->oMenuButtonTimer < 16 * FRAME_RATE_SCALER_INV)
	{
		button->oFaceAnglePitch += 0x800 / FRAME_RATE_SCALER_INV;
	}
	button->oParentRelativePosX += button->oMenuButtonOrigPosX / 16.0 * FRAME_RATE_SCALER;
	button->oParentRelativePosY += button->oMenuButtonOrigPosY / 16.0 * FRAME_RATE_SCALER;
	if(button->oPosZ > button->oMenuButtonOrigPosZ)
	{
		button->oParentRelativePosZ += 116.25 * FRAME_RATE_SCALER;
	}
	button->oMenuButtonTimer++;
	if(button->oMenuButtonTimer == 16 * FRAME_RATE_SCALER_INV)
	{
		button->oParentRelativePosX = button->oMenuButtonOrigPosX;
		button->oParentRelativePosY = button->oMenuButtonOrigPosY;
		button->oMenuButtonState    = MENU_BUTTON_STATE_DEFAULT;
		button->oMenuButtonTimer    = 0;
	}
}

/**
 * A small increase and decrease in size.
 * Used by failed copy/erase/score operations and sound mode select.
 */
static void bhv_menu_button_zoom_in_out(struct Object* button)
{
	if(Floor == MENU_LAYER_MAIN)
	{
		if(button->oMenuButtonTimer < 4 * FRAME_RATE_SCALER_INV)
		{
			button->oParentRelativePosZ -= 20.0f * FRAME_RATE_SCALER;
		}

		if(button->oMenuButtonTimer >= 4 * FRAME_RATE_SCALER_INV)
		{
			button->oParentRelativePosZ += 20.0f * FRAME_RATE_SCALER;
		}
	}
	else
	{
		if(button->oMenuButtonTimer < 4 * FRAME_RATE_SCALER_INV)
		{
			button->oParentRelativePosZ += 20.0f * FRAME_RATE_SCALER;
		}

		if(button->oMenuButtonTimer >= 4 * FRAME_RATE_SCALER_INV)
		{
			button->oParentRelativePosZ -= 20.0f * FRAME_RATE_SCALER;
		}
	}

	button->oMenuButtonTimer++;

	if(button->oMenuButtonTimer == 8 * FRAME_RATE_SCALER_INV)
	{
		button->oMenuButtonState = MENU_BUTTON_STATE_DEFAULT;
		button->oMenuButtonTimer = 0;
	}
}

/**
 * A small temporary increase in size.
 * Used while selecting a target copy/erase file or yes/no erase confirmation prompt.
 */
static void bhv_menu_button_zoom_in(struct Object* button)
{
	button->oMenuButtonScale += 0.0022 * FRAME_RATE_SCALER;
	button->oMenuButtonTimer++;

	if(button->oMenuButtonTimer == 10 * FRAME_RATE_SCALER_INV)
	{
		button->oMenuButtonState = MENU_BUTTON_STATE_DEFAULT;
		button->oMenuButtonTimer = 0;
	}
}

/**
 * A small temporary decrease in size.
 * Used after selecting a target copy/erase file or
 * yes/no erase confirmation prompt to undo the zoom in.
 */
static void bhv_menu_button_zoom_out(struct Object* button)
{
	button->oMenuButtonScale -= 0.0022 * FRAME_RATE_SCALER;
	button->oMenuButtonTimer++;
	if(button->oMenuButtonTimer == 10 * FRAME_RATE_SCALER_INV)
	{
		button->oMenuButtonState = MENU_BUTTON_STATE_DEFAULT;
		button->oMenuButtonTimer = 0;
	}
}

/**
 * Menu Buttons Menu Initial Action
 * Aligns menu buttons so they can stay in their original
 * positions when you choose a button.
 */
void bhv_menu_button_init()
{
	gCurrentObject->oMenuButtonOrigPosX = gCurrentObject->oParentRelativePosX;
	gCurrentObject->oMenuButtonOrigPosY = gCurrentObject->oParentRelativePosY;
}

/**
 * Menu Buttons Menu Loop Action
 * Handles the functions of the button states and
 * object scale for each button.
 */
void bhv_menu_button_loop()
{
	switch(gCurrentObject->oMenuButtonState)
	{
		case MENU_BUTTON_STATE_DEFAULT: // Button state
			gCurrentObject->oMenuButtonOrigPosZ = gCurrentObject->oPosZ;
			break;
		case MENU_BUTTON_STATE_GROWING: // Switching from button to menu state
			if(Floor == MENU_LAYER_MAIN)
			{
				bhv_menu_button_growing_from_main_menu(gCurrentObject);
			}
			if(Floor == MENU_LAYER_SUBMENU)
			{
				bhv_menu_button_growing_from_submenu(gCurrentObject); // Only used for score files
			}
			alpha_value = 0;
			cursol_flag = 4;
			break;
		case MENU_BUTTON_STATE_FULLSCREEN: // Menu state
			break;
		case MENU_BUTTON_STATE_SHRINKING: // Switching from menu to button state
			if(Floor == MENU_LAYER_MAIN)
			{
				bhv_menu_button_shrinking_to_main_menu(gCurrentObject);
			}
			if(Floor == MENU_LAYER_SUBMENU)
			{
				bhv_menu_button_shrinking_to_submenu(gCurrentObject); // Only used for score files
			}
			alpha_value = 0;
			cursol_flag = 4;
			break;
		case MENU_BUTTON_STATE_ZOOM_IN_OUT:
			bhv_menu_button_zoom_in_out(gCurrentObject);
			cursol_flag = 4;
			break;
		case MENU_BUTTON_STATE_ZOOM_IN:
			bhv_menu_button_zoom_in(gCurrentObject);
			cursol_flag = 4;
			break;
		case MENU_BUTTON_STATE_ZOOM_OUT:
			bhv_menu_button_zoom_out(gCurrentObject);
			cursol_flag = 4;
			break;
	}
	s_set_scale(gCurrentObject->oMenuButtonScale);
}

/**
 * Handles how to exit the score file menu using button states.
 */
static void WatchFileScore(struct Object* scoreFileButton, s8 scoreButtonID)
{
	// Begin exit
	if(scoreFileButton->oMenuButtonState == MENU_BUTTON_STATE_FULLSCREEN && cursol_flag == 2)
	{
		AudStartSound(SOUND_MENU_CAMERA_ZOOM_OUT, gDefaultSoundArgs);
		SendMotorEvent(5, 80);
		scoreFileButton->oMenuButtonState = MENU_BUTTON_STATE_SHRINKING;
	}
	// End exit
	if(scoreFileButton->oMenuButtonState == MENU_BUTTON_STATE_DEFAULT)
	{
		selectNo = scoreButtonID;
		if(Floor == MENU_LAYER_SUBMENU)
		{
			Floor = MENU_LAYER_MAIN;
		}
	}
}

/**
 * Render buttons for the score menu.
 * Also check if the save file exists to render a different Mario button.
 */
static void ScoreScene_init(struct Object* scoreButton)
{
	// File A
	if(BuIsActive(SAVE_FILE_A) == TRUE)
	{
		sMainMenuButtons[MENU_BUTTON_SCORE_FILE_A] = s_makeobj_relative(scoreButton, MODEL_MAIN_MENU_MARIO_SAVE_BUTTON, sm64::bhv::bhvMenuButton(), 711, 311, -100, 0, -0x8000, 0);
	}
	else
	{
		sMainMenuButtons[MENU_BUTTON_SCORE_FILE_A] = s_makeobj_relative(scoreButton, MODEL_MAIN_MENU_MARIO_NEW_BUTTON, sm64::bhv::bhvMenuButton(), 711, 311, -100, 0, -0x8000, 0);
	}
	sMainMenuButtons[MENU_BUTTON_SCORE_FILE_A]->oMenuButtonScale = 0.11111111f;
	// File B
	if(BuIsActive(SAVE_FILE_B) == TRUE)
	{
		sMainMenuButtons[MENU_BUTTON_SCORE_FILE_B] = s_makeobj_relative(scoreButton, MODEL_MAIN_MENU_MARIO_SAVE_BUTTON, sm64::bhv::bhvMenuButton(), -166, 311, -100, 0, -0x8000, 0);
	}
	else
	{
		sMainMenuButtons[MENU_BUTTON_SCORE_FILE_B] = s_makeobj_relative(scoreButton, MODEL_MAIN_MENU_MARIO_NEW_BUTTON, sm64::bhv::bhvMenuButton(), -166, 311, -100, 0, -0x8000, 0);
	}
	sMainMenuButtons[MENU_BUTTON_SCORE_FILE_B]->oMenuButtonScale = 0.11111111f;
	// File C
	if(BuIsActive(SAVE_FILE_C) == TRUE)
	{
		sMainMenuButtons[MENU_BUTTON_SCORE_FILE_C] = s_makeobj_relative(scoreButton, MODEL_MAIN_MENU_MARIO_SAVE_BUTTON, sm64::bhv::bhvMenuButton(), 711, 0, -100, 0, -0x8000, 0);
	}
	else
	{
		sMainMenuButtons[MENU_BUTTON_SCORE_FILE_C] = s_makeobj_relative(scoreButton, MODEL_MAIN_MENU_MARIO_NEW_BUTTON, sm64::bhv::bhvMenuButton(), 711, 0, -100, 0, -0x8000, 0);
	}
	sMainMenuButtons[MENU_BUTTON_SCORE_FILE_C]->oMenuButtonScale = 0.11111111f;
	// File D
	if(BuIsActive(SAVE_FILE_D) == TRUE)
	{
		sMainMenuButtons[MENU_BUTTON_SCORE_FILE_D] = s_makeobj_relative(scoreButton, MODEL_MAIN_MENU_MARIO_SAVE_BUTTON, sm64::bhv::bhvMenuButton(), -166, 0, -100, 0, -0x8000, 0);
	}
	else
	{
		sMainMenuButtons[MENU_BUTTON_SCORE_FILE_D] = s_makeobj_relative(scoreButton, MODEL_MAIN_MENU_MARIO_NEW_BUTTON, sm64::bhv::bhvMenuButton(), -166, 0, -100, 0, -0x8000, 0);
	}
	sMainMenuButtons[MENU_BUTTON_SCORE_FILE_D]->oMenuButtonScale = 0.11111111f;
	// Return to main menu button
	sMainMenuButtons[MENU_BUTTON_SCORE_RETURN]		     = s_makeobj_relative(scoreButton, MODEL_MAIN_MENU_YELLOW_FILE_BUTTON, sm64::bhv::bhvMenuButton(), 711, -388, -100, 0, -0x8000, 0);
	sMainMenuButtons[MENU_BUTTON_SCORE_RETURN]->oMenuButtonScale = 0.11111111f;
	// Switch to copy menu button
	sMainMenuButtons[MENU_BUTTON_SCORE_COPY_FILE]			= s_makeobj_relative(scoreButton, MODEL_MAIN_MENU_BLUE_COPY_BUTTON, sm64::bhv::bhvMenuButton(), 0, -388, -100, 0, -0x8000, 0);
	sMainMenuButtons[MENU_BUTTON_SCORE_COPY_FILE]->oMenuButtonScale = 0.11111111f;
	// Switch to erase menu button
	sMainMenuButtons[MENU_BUTTON_SCORE_ERASE_FILE]			 = s_makeobj_relative(scoreButton, MODEL_MAIN_MENU_RED_ERASE_BUTTON, sm64::bhv::bhvMenuButton(), -711, -388, -100, 0, -0x8000, 0);
	sMainMenuButtons[MENU_BUTTON_SCORE_ERASE_FILE]->oMenuButtonScale = 0.11111111f;
}

/**
 * In the score menu, checks if a button was clicked to play a sound, button state and other functions.
 */
static void FileScoreScene(struct Object* scoreButton)
{
	if(scoreButton->oMenuButtonState == MENU_BUTTON_STATE_FULLSCREEN)
	{
		s32 buttonID;
		// Configure score menu button group
		for(buttonID = MENU_BUTTON_SCORE_MIN; buttonID < MENU_BUTTON_SCORE_MAX; buttonID++)
		{
			s16 buttonX = sMainMenuButtons[buttonID]->oPosX;
			s16 buttonY = sMainMenuButtons[buttonID]->oPosY;

			if(ButtonClickCheck(buttonX, buttonY, 22.0f) == TRUE && click_counter >= 31 * FRAME_RATE_SCALER_INV)
			{
				// If menu button clicked, select it
				if(buttonID == MENU_BUTTON_SCORE_RETURN || buttonID == MENU_BUTTON_SCORE_COPY_FILE || buttonID == MENU_BUTTON_SCORE_ERASE_FILE)
				{
					AudStartSound(SOUND_MENU_CLICK_FILE_SELECT, gDefaultSoundArgs);
					SendMotorEvent(5, 80);

					sMainMenuButtons[buttonID]->oMenuButtonState = MENU_BUTTON_STATE_ZOOM_IN_OUT;
					selectNo				     = buttonID;
				}
				else
				{ // Check if a save file is clicked
					if(click_counter >= 31 * FRAME_RATE_SCALER_INV)
					{
						// If clicked in a existing save file, select it too see it's score
						if(BuIsActive(buttonID - MENU_BUTTON_SCORE_MIN) == TRUE)
						{
							AudStartSound(SOUND_MENU_CAMERA_ZOOM_IN, gDefaultSoundArgs);
							SendMotorEvent(5, 80);
							sMainMenuButtons[buttonID]->oMenuButtonState = MENU_BUTTON_STATE_GROWING;
							selectNo				     = buttonID;
						}
						else
						{
							// If clicked in a non-existing save file, play buzz sound
							AudStartSound(SOUND_MENU_CAMERA_BUZZ, gDefaultSoundArgs);
							SendMotorEvent(5, 80);
							sMainMenuButtons[buttonID]->oMenuButtonState = MENU_BUTTON_STATE_ZOOM_IN_OUT;
							if(click_counter >= 31 * FRAME_RATE_SCALER_INV)
							{
								click_flag    = TRUE;
								click_counter = 0;
							}
						}
					}
				}
				Floor = MENU_LAYER_SUBMENU;
				break;
			}
		}
	}
}

/**
 * Render buttons for the copy menu.
 * Also check if the save file exists to render a different Mario button.
 */
static void CopyScene_init(struct Object* copyButton)
{
	// File A
	if(BuIsActive(SAVE_FILE_A) == TRUE)
	{
		sMainMenuButtons[MENU_BUTTON_COPY_FILE_A] = s_makeobj_relative(copyButton, MODEL_MAIN_MENU_MARIO_SAVE_BUTTON, sm64::bhv::bhvMenuButton(), 711, 311, -100, 0, -0x8000, 0);
	}
	else
	{
		sMainMenuButtons[MENU_BUTTON_COPY_FILE_A] = s_makeobj_relative(copyButton, MODEL_MAIN_MENU_MARIO_NEW_BUTTON, sm64::bhv::bhvMenuButton(), 711, 311, -100, 0, -0x8000, 0);
	}
	sMainMenuButtons[MENU_BUTTON_COPY_FILE_A]->oMenuButtonScale = 0.11111111f;
	// File B
	if(BuIsActive(SAVE_FILE_B) == TRUE)
	{
		sMainMenuButtons[MENU_BUTTON_COPY_FILE_B] = s_makeobj_relative(copyButton, MODEL_MAIN_MENU_MARIO_SAVE_BUTTON, sm64::bhv::bhvMenuButton(), -166, 311, -100, 0, -0x8000, 0);
	}
	else
	{
		sMainMenuButtons[MENU_BUTTON_COPY_FILE_B] = s_makeobj_relative(copyButton, MODEL_MAIN_MENU_MARIO_NEW_BUTTON, sm64::bhv::bhvMenuButton(), -166, 311, -100, 0, -0x8000, 0);
	}
	sMainMenuButtons[MENU_BUTTON_COPY_FILE_B]->oMenuButtonScale = 0.11111111f;
	// File C
	if(BuIsActive(SAVE_FILE_C) == TRUE)
	{
		sMainMenuButtons[MENU_BUTTON_COPY_FILE_C] = s_makeobj_relative(copyButton, MODEL_MAIN_MENU_MARIO_SAVE_BUTTON, sm64::bhv::bhvMenuButton(), 711, 0, -100, 0, -0x8000, 0);
	}
	else
	{
		sMainMenuButtons[MENU_BUTTON_COPY_FILE_C] = s_makeobj_relative(copyButton, MODEL_MAIN_MENU_MARIO_NEW_BUTTON, sm64::bhv::bhvMenuButton(), 711, 0, -100, 0, -0x8000, 0);
	}
	sMainMenuButtons[MENU_BUTTON_COPY_FILE_C]->oMenuButtonScale = 0.11111111f;
	// File D
	if(BuIsActive(SAVE_FILE_D) == TRUE)
	{
		sMainMenuButtons[MENU_BUTTON_COPY_FILE_D] = s_makeobj_relative(copyButton, MODEL_MAIN_MENU_MARIO_SAVE_BUTTON, sm64::bhv::bhvMenuButton(), -166, 0, -100, 0, -0x8000, 0);
	}
	else
	{
		sMainMenuButtons[MENU_BUTTON_COPY_FILE_D] = s_makeobj_relative(copyButton, MODEL_MAIN_MENU_MARIO_NEW_BUTTON, sm64::bhv::bhvMenuButton(), -166, 0, -100, 0, -0x8000, 0);
	}
	sMainMenuButtons[MENU_BUTTON_COPY_FILE_D]->oMenuButtonScale = 0.11111111f;
	// Return to main menu button
	sMainMenuButtons[MENU_BUTTON_COPY_RETURN]		    = s_makeobj_relative(copyButton, MODEL_MAIN_MENU_YELLOW_FILE_BUTTON, sm64::bhv::bhvMenuButton(), 711, -388, -100, 0, -0x8000, 0);
	sMainMenuButtons[MENU_BUTTON_COPY_RETURN]->oMenuButtonScale = 0.11111111f;
	// Switch to scire menu button
	sMainMenuButtons[MENU_BUTTON_COPY_CHECK_SCORE]			 = s_makeobj_relative(copyButton, MODEL_MAIN_MENU_GREEN_SCORE_BUTTON, sm64::bhv::bhvMenuButton(), 0, -388, -100, 0, -0x8000, 0);
	sMainMenuButtons[MENU_BUTTON_COPY_CHECK_SCORE]->oMenuButtonScale = 0.11111111f;
	// Switch to erase menu button
	sMainMenuButtons[MENU_BUTTON_COPY_ERASE_FILE]			= s_makeobj_relative(copyButton, MODEL_MAIN_MENU_RED_ERASE_BUTTON, sm64::bhv::bhvMenuButton(), -711, -388, -100, 0, -0x8000, 0);
	sMainMenuButtons[MENU_BUTTON_COPY_ERASE_FILE]->oMenuButtonScale = 0.11111111f;
}

/**
 * Copy Menu phase actions that handles what to do when a file button is clicked.
 */
static void FileCopyButtonEvent(struct Object* copyButton, s32 copyFileButtonID)
{
	switch(copyButton->oMenuButtonActionPhase)
	{
		case COPY_PHASE_MAIN: // Copy Menu Main Phase
			if(sAllFilesExist == TRUE)
			{ // Don't enable copy if all save files exists
				return;
			}
			if(BuIsActive(copyFileButtonID - MENU_BUTTON_COPY_MIN) == TRUE)
			{
				// If clicked in a existing save file, ask where it wants to copy
				AudStartSound(SOUND_MENU_CLICK_FILE_SELECT, gDefaultSoundArgs);
				SendMotorEvent(5, 80);
				sMainMenuButtons[copyFileButtonID]->oMenuButtonState = MENU_BUTTON_STATE_ZOOM_IN;
				click_file					     = copyFileButtonID - MENU_BUTTON_COPY_MIN;
				copyButton->oMenuButtonActionPhase		     = COPY_PHASE_COPY_WHERE;
				click_flag					     = TRUE;
				click_counter					     = 0;
			}
			else
			{
				// If clicked in a non-existing save file, play buzz sound
				AudStartSound(SOUND_MENU_CAMERA_BUZZ, gDefaultSoundArgs);
				SendMotorEvent(5, 80);
				sMainMenuButtons[copyFileButtonID]->oMenuButtonState = MENU_BUTTON_STATE_ZOOM_IN_OUT;
				if(click_counter >= 21 * FRAME_RATE_SCALER_INV)
				{
					click_flag    = TRUE;
					click_counter = 0;
				}
			}
			break;
		case COPY_PHASE_COPY_WHERE: // Copy Menu "COPY IT TO WHERE?" Phase (after a file is selected)
			sMainMenuButtons[copyFileButtonID]->oMenuButtonState = MENU_BUTTON_STATE_ZOOM_IN_OUT;
			if(BuIsActive(copyFileButtonID - MENU_BUTTON_COPY_MIN) == FALSE)
			{
				// If clicked in a non-existing save file, copy the file
				AudStartSound(SOUND_MENU_STAR_SOUND, gDefaultSoundArgs);
				SendMotorEvent(5, 80);
				copyButton->oMenuButtonActionPhase = COPY_PHASE_COPY_COMPLETE;
				click_flag			   = TRUE;
				click_counter			   = 0;
				BuCopyStorage(click_file, copyFileButtonID - MENU_BUTTON_COPY_MIN);
				sMainMenuButtons[copyFileButtonID]->header.gfx.sharedChild			  = stageShapes[MODEL_MAIN_MENU_MARIO_SAVE_BUTTON_FADE];
				sMainMenuButtons[copyFileButtonID - MENU_BUTTON_COPY_MIN]->header.gfx.sharedChild = stageShapes[MODEL_MAIN_MENU_MARIO_SAVE_BUTTON_FADE];
			}
			else
			{
				// If clicked in a existing save file, play buzz sound
				if(MENU_BUTTON_COPY_FILE_A + click_file == copyFileButtonID)
				{
					AudStartSound(SOUND_MENU_CAMERA_BUZZ, gDefaultSoundArgs);
					SendMotorEvent(5, 80);
					sMainMenuButtons[MENU_BUTTON_COPY_FILE_A + click_file]->oMenuButtonState = MENU_BUTTON_STATE_ZOOM_OUT;
					copyButton->oMenuButtonActionPhase					 = COPY_PHASE_MAIN;
					click_flag								 = TRUE;
					return;
				}
				if(click_counter >= 21 * FRAME_RATE_SCALER_INV)
				{
					click_flag    = TRUE;
					click_counter = 0;
				}
			}
			break;
	}
}

/**
 * In the copy menu, checks if a button was clicked to play a sound, button state and other functions.
 */
static void FileCopyScene(struct Object* copyButton)
{
	if(copyButton->oMenuButtonState == MENU_BUTTON_STATE_FULLSCREEN)
	{
		s32 buttonID;
		// Configure copy menu button group
		for(buttonID = MENU_BUTTON_COPY_MIN; buttonID < MENU_BUTTON_COPY_MAX; buttonID++)
		{
			s16 buttonX = sMainMenuButtons[buttonID]->oPosX;
			s16 buttonY = sMainMenuButtons[buttonID]->oPosY;

			if(ButtonClickCheck(buttonX, buttonY, 22.0f) == TRUE)
			{
				// If menu button clicked, select it
				if(buttonID == MENU_BUTTON_COPY_RETURN || buttonID == MENU_BUTTON_COPY_CHECK_SCORE || buttonID == MENU_BUTTON_COPY_ERASE_FILE)
				{
					if(copyButton->oMenuButtonActionPhase == COPY_PHASE_MAIN)
					{
						AudStartSound(SOUND_MENU_CLICK_FILE_SELECT, gDefaultSoundArgs);
						SendMotorEvent(5, 80);
						sMainMenuButtons[buttonID]->oMenuButtonState = MENU_BUTTON_STATE_ZOOM_IN_OUT;
						selectNo				     = buttonID;
					}
				}
				else
				{
					// Check if a file button is clicked to play a copy action
					if(sMainMenuButtons[buttonID]->oMenuButtonState == MENU_BUTTON_STATE_DEFAULT && click_counter >= 31 * FRAME_RATE_SCALER_INV)
					{
						FileCopyButtonEvent(copyButton, buttonID);
					}
				}
				Floor = MENU_LAYER_SUBMENU;
				break;
			}
		}
		// After copy is complete, return to main copy phase
		if(copyButton->oMenuButtonActionPhase == COPY_PHASE_COPY_COMPLETE && click_counter >= 31 * FRAME_RATE_SCALER_INV)
		{
			copyButton->oMenuButtonActionPhase				      = COPY_PHASE_MAIN;
			sMainMenuButtons[MENU_BUTTON_COPY_MIN + click_file]->oMenuButtonState = MENU_BUTTON_STATE_ZOOM_OUT;
		}
	}
}

/**
 * Render buttons for the erase menu.
 * Also check if the save file exists to render a different Mario button.
 */
static void RemoveScene_init(struct Object* eraseButton)
{
	// File A
	if(BuIsActive(SAVE_FILE_A) == TRUE)
	{
		sMainMenuButtons[MENU_BUTTON_ERASE_FILE_A] = s_makeobj_relative(eraseButton, MODEL_MAIN_MENU_MARIO_SAVE_BUTTON, sm64::bhv::bhvMenuButton(), 711, 311, -100, 0, -0x8000, 0);
	}
	else
	{
		sMainMenuButtons[MENU_BUTTON_ERASE_FILE_A] = s_makeobj_relative(eraseButton, MODEL_MAIN_MENU_MARIO_NEW_BUTTON, sm64::bhv::bhvMenuButton(), 711, 311, -100, 0, -0x8000, 0);
	}
	sMainMenuButtons[MENU_BUTTON_ERASE_FILE_A]->oMenuButtonScale = 0.11111111f;
	// File B
	if(BuIsActive(SAVE_FILE_B) == TRUE)
	{
		sMainMenuButtons[MENU_BUTTON_ERASE_FILE_B] = s_makeobj_relative(eraseButton, MODEL_MAIN_MENU_MARIO_SAVE_BUTTON, sm64::bhv::bhvMenuButton(), -166, 311, -100, 0, -0x8000, 0);
	}
	else
	{
		sMainMenuButtons[MENU_BUTTON_ERASE_FILE_B] = s_makeobj_relative(eraseButton, MODEL_MAIN_MENU_MARIO_NEW_BUTTON, sm64::bhv::bhvMenuButton(), -166, 311, -100, 0, -0x8000, 0);
	}
	sMainMenuButtons[MENU_BUTTON_ERASE_FILE_B]->oMenuButtonScale = 0.11111111f;
	// File C
	if(BuIsActive(SAVE_FILE_C) == TRUE)
	{
		sMainMenuButtons[MENU_BUTTON_ERASE_FILE_C] = s_makeobj_relative(eraseButton, MODEL_MAIN_MENU_MARIO_SAVE_BUTTON, sm64::bhv::bhvMenuButton(), 711, 0, -100, 0, -0x8000, 0);
	}
	else
	{
		sMainMenuButtons[MENU_BUTTON_ERASE_FILE_C] = s_makeobj_relative(eraseButton, MODEL_MAIN_MENU_MARIO_NEW_BUTTON, sm64::bhv::bhvMenuButton(), 711, 0, -100, 0, -0x8000, 0);
	}
	sMainMenuButtons[MENU_BUTTON_ERASE_FILE_C]->oMenuButtonScale = 0.11111111f;
	// File D
	if(BuIsActive(SAVE_FILE_D) == TRUE)
	{
		sMainMenuButtons[MENU_BUTTON_ERASE_FILE_D] = s_makeobj_relative(eraseButton, MODEL_MAIN_MENU_MARIO_SAVE_BUTTON, sm64::bhv::bhvMenuButton(), -166, 0, -100, 0, -0x8000, 0);
	}
	else
	{
		sMainMenuButtons[MENU_BUTTON_ERASE_FILE_D] = s_makeobj_relative(eraseButton, MODEL_MAIN_MENU_MARIO_NEW_BUTTON, sm64::bhv::bhvMenuButton(), -166, 0, -100, 0, -0x8000, 0);
	}
	sMainMenuButtons[MENU_BUTTON_ERASE_FILE_D]->oMenuButtonScale = 0.11111111f;
	// Return to main menu button
	sMainMenuButtons[MENU_BUTTON_ERASE_RETURN]		     = s_makeobj_relative(eraseButton, MODEL_MAIN_MENU_YELLOW_FILE_BUTTON, sm64::bhv::bhvMenuButton(), 711, -388, -100, 0, -0x8000, 0);
	sMainMenuButtons[MENU_BUTTON_ERASE_RETURN]->oMenuButtonScale = 0.11111111f;
	// Switch to score menu button
	sMainMenuButtons[MENU_BUTTON_ERASE_CHECK_SCORE]			  = s_makeobj_relative(eraseButton, MODEL_MAIN_MENU_GREEN_SCORE_BUTTON, sm64::bhv::bhvMenuButton(), 0, -388, -100, 0, -0x8000, 0);
	sMainMenuButtons[MENU_BUTTON_ERASE_CHECK_SCORE]->oMenuButtonScale = 0.11111111f;
	// Switch to copy menu button
	sMainMenuButtons[MENU_BUTTON_ERASE_COPY_FILE]			= s_makeobj_relative(eraseButton, MODEL_MAIN_MENU_BLUE_COPY_BUTTON, sm64::bhv::bhvMenuButton(), -711, -388, -100, 0, -0x8000, 0);
	sMainMenuButtons[MENU_BUTTON_ERASE_COPY_FILE]->oMenuButtonScale = 0.11111111f;
}

/**
 * Erase Menu phase actions that handles what to do when a file button is clicked.
 */
static void FileRemoveButtonEvent(struct Object* eraseButton, s32 eraseFileButtonID)
{
	switch(eraseButton->oMenuButtonActionPhase)
	{
		case ERASE_PHASE_MAIN: // Erase Menu Main Phase
			if(BuIsActive(eraseFileButtonID - MENU_BUTTON_ERASE_MIN) == TRUE)
			{
				// If clicked in a existing save file, ask if it wants to delete it
				AudStartSound(SOUND_MENU_CLICK_FILE_SELECT, gDefaultSoundArgs);
				SendMotorEvent(5, 80);
				sMainMenuButtons[eraseFileButtonID]->oMenuButtonState = MENU_BUTTON_STATE_ZOOM_IN;
				click_file					      = eraseFileButtonID - MENU_BUTTON_ERASE_MIN;
				eraseButton->oMenuButtonActionPhase		      = ERASE_PHASE_PROMPT;
				click_flag					      = TRUE;
				click_counter					      = 0;
			}
			else
			{
				// If clicked in a non-existing save file, play buzz sound
				AudStartSound(SOUND_MENU_CAMERA_BUZZ, gDefaultSoundArgs);
				SendMotorEvent(5, 80);
				sMainMenuButtons[eraseFileButtonID]->oMenuButtonState = MENU_BUTTON_STATE_ZOOM_IN_OUT;
				if(click_counter >= 21 * FRAME_RATE_SCALER_INV)
				{
					click_flag    = TRUE;
					click_counter = 0;
				}
			}
			break;
		case ERASE_PHASE_PROMPT: // Erase Menu "SURE? YES NO" Phase (after a file is selected)
			if(MENU_BUTTON_ERASE_MIN + click_file == eraseFileButtonID)
			{
				// If clicked in a existing save file, play click sound and zoom out button
				// Note: The prompt functions are actually called when the ERASE_MSG_PROMPT
				// message is displayed with DrawYesNo
				AudStartSound(SOUND_MENU_CLICK_FILE_SELECT, gDefaultSoundArgs);
				SendMotorEvent(5, 80);
				sMainMenuButtons[MENU_BUTTON_ERASE_MIN + click_file]->oMenuButtonState = MENU_BUTTON_STATE_ZOOM_OUT;
				eraseButton->oMenuButtonActionPhase				       = ERASE_PHASE_MAIN;
				click_flag							       = TRUE;
			}
			break;
	}
}

/**
 * In the erase menu, checks if a button was clicked to play a sound, button state and other functions.
 */
static void FileRemoveScene(struct Object* eraseButton)
{
	if(eraseButton->oMenuButtonState == MENU_BUTTON_STATE_FULLSCREEN)
	{
		s32 buttonID;
		// Configure erase menu button group
		for(buttonID = MENU_BUTTON_ERASE_MIN; buttonID < MENU_BUTTON_ERASE_MAX; buttonID++)
		{
			s16 buttonX = sMainMenuButtons[buttonID]->oPosX;
			s16 buttonY = sMainMenuButtons[buttonID]->oPosY;

			if(ButtonClickCheck(buttonX, buttonY, 22.0f) == TRUE)
			{
				// If menu button clicked, select it
				if(buttonID == MENU_BUTTON_ERASE_RETURN || buttonID == MENU_BUTTON_ERASE_CHECK_SCORE || buttonID == MENU_BUTTON_ERASE_COPY_FILE)
				{
					if(eraseButton->oMenuButtonActionPhase == ERASE_PHASE_MAIN)
					{
						AudStartSound(SOUND_MENU_CLICK_FILE_SELECT, gDefaultSoundArgs);
						SendMotorEvent(5, 80);
						sMainMenuButtons[buttonID]->oMenuButtonState = MENU_BUTTON_STATE_ZOOM_IN_OUT;
						selectNo				     = buttonID;
					}
				}
				else
				{
					// Check if a file button is clicked to play an erase action
					if(click_counter >= 31 * FRAME_RATE_SCALER_INV)
					{
						FileRemoveButtonEvent(eraseButton, buttonID);
					}
				}
				Floor = MENU_LAYER_SUBMENU;
				break;
			}
		}
		// After erase is complete, return to main erase phase
		if(eraseButton->oMenuButtonActionPhase == ERASE_PHASE_MARIO_ERASED && click_counter >= 31 * FRAME_RATE_SCALER_INV)
		{
			eraseButton->oMenuButtonActionPhase				       = ERASE_PHASE_MAIN;
			sMainMenuButtons[MENU_BUTTON_ERASE_MIN + click_file]->oMenuButtonState = MENU_BUTTON_STATE_ZOOM_OUT;
		}
	}
}

/**
 * Render buttons for the sound mode menu.
 */
static void SoundScene_init(struct Object* soundModeButton)
{
	// Stereo option button
	sMainMenuButtons[MENU_BUTTON_STEREO]		       = s_makeobj_relative(soundModeButton, MODEL_MAIN_MENU_GENERIC_BUTTON, sm64::bhv::bhvMenuButton(), 533, 0, -100, 0, -0x8000, 0);
	sMainMenuButtons[MENU_BUTTON_STEREO]->oMenuButtonScale = 0.11111111f;
	// Mono option button
	sMainMenuButtons[MENU_BUTTON_MONO]		     = s_makeobj_relative(soundModeButton, MODEL_MAIN_MENU_GENERIC_BUTTON, sm64::bhv::bhvMenuButton(), 0, 0, -100, 0, -0x8000, 0);
	sMainMenuButtons[MENU_BUTTON_MONO]->oMenuButtonScale = 0.11111111f;
	// Headset option button
	sMainMenuButtons[MENU_BUTTON_HEADSET]			= s_makeobj_relative(soundModeButton, MODEL_MAIN_MENU_GENERIC_BUTTON, sm64::bhv::bhvMenuButton(), -533, 0, -100, 0, -0x8000, 0);
	sMainMenuButtons[MENU_BUTTON_HEADSET]->oMenuButtonScale = 0.11111111f;
	// Zoom in current selection
	sMainMenuButtons[MENU_BUTTON_SOUND_MIN + sound_flag]->oMenuButtonState = MENU_BUTTON_STATE_ZOOM_IN;
}

/**
 * In the sound mode menu, checks if a button was clicked to change sound mode & button state.
 */
static void SoundSelectScene(struct Object* soundModeButton)
{
	if(soundModeButton->oMenuButtonState == MENU_BUTTON_STATE_FULLSCREEN)
	{
		s32 buttonID;
		// Configure sound mode menu button group
		for(buttonID = MENU_BUTTON_SOUND_MIN; buttonID < MENU_BUTTON_SOUND_MAX; buttonID++)
		{
			s16 buttonX = sMainMenuButtons[buttonID]->oPosX;
			s16 buttonY = sMainMenuButtons[buttonID]->oPosY;

			if(ButtonClickCheck(buttonX, buttonY, 22.0f) == TRUE)
			{
				// If sound mode button clicked, select it
				// The check will always be true because of the group configured above
				if(buttonID == MENU_BUTTON_STEREO || buttonID == MENU_BUTTON_MONO || buttonID == MENU_BUTTON_HEADSET)
				{
					if(soundModeButton->oMenuButtonActionPhase == SOUND_MODE_PHASE_MAIN)
					{
						AudStartSound(SOUND_MENU_CLICK_FILE_SELECT, gDefaultSoundArgs);
						SendMotorEvent(5, 80);
						sMainMenuButtons[buttonID]->oMenuButtonState = MENU_BUTTON_STATE_ZOOM_IN_OUT;
						selectNo				     = buttonID;
						sound_flag				     = buttonID - MENU_BUTTON_SOUND_MIN;
						BuSetSoundMode(sound_flag);
					}
				}
				Floor = MENU_LAYER_SUBMENU;
				break;
			}
		}
	}
}

/**
 * Loads a save file selected after it goes into a full screen state
 * retuning goTogame to a save value defined in fileNum.
 */
static void GoToGame(struct Object* fileButton, s32 fileNum)
{
	if(fileButton->oMenuButtonState == MENU_BUTTON_STATE_FULLSCREEN)
	{
		goTogame = fileNum;
	}
}

/**
 * Returns from the previous menu back to the main menu using
 * the return button (or sound mode) as source button.
 */
static void GoToFileSelect(s16 prevMenuButtonID, struct Object* sourceButton)
{
	s32 buttonID;
	// If the source button is in default state and the previous menu in full screen,
	// play zoom out sound and shrink previous menu
	if(sourceButton->oMenuButtonState == MENU_BUTTON_STATE_DEFAULT && sMainMenuButtons[prevMenuButtonID]->oMenuButtonState == MENU_BUTTON_STATE_FULLSCREEN)
	{
		AudStartSound(SOUND_MENU_CAMERA_ZOOM_OUT, gDefaultSoundArgs);
		sMainMenuButtons[prevMenuButtonID]->oMenuButtonState = MENU_BUTTON_STATE_SHRINKING;
		Floor						     = MENU_LAYER_MAIN;
	}
	// If the previous button is in default state, return back to the main menu
	if(sMainMenuButtons[prevMenuButtonID]->oMenuButtonState == MENU_BUTTON_STATE_DEFAULT)
	{
		selectNo = MENU_BUTTON_NONE;
		// Hide buttons of corresponding button menu groups
		if(prevMenuButtonID == MENU_BUTTON_SCORE)
		{
			for(buttonID = MENU_BUTTON_SCORE_MIN; buttonID < MENU_BUTTON_SCORE_MAX; buttonID++)
			{
				RemoveShape(sMainMenuButtons[buttonID]);
			}
		}
		if(prevMenuButtonID == MENU_BUTTON_COPY)
		{
			for(buttonID = MENU_BUTTON_COPY_MIN; buttonID < MENU_BUTTON_COPY_MAX; buttonID++)
			{
				RemoveShape(sMainMenuButtons[buttonID]);
			}
		}
		if(prevMenuButtonID == MENU_BUTTON_ERASE)
		{
			for(buttonID = MENU_BUTTON_ERASE_MIN; buttonID < MENU_BUTTON_ERASE_MAX; buttonID++)
			{
				RemoveShape(sMainMenuButtons[buttonID]);
			}
		}
		if(prevMenuButtonID == MENU_BUTTON_SOUND_MODE)
		{
			for(buttonID = MENU_BUTTON_SOUND_MIN; buttonID < MENU_BUTTON_SOUND_MAX; buttonID++)
			{
				RemoveShape(sMainMenuButtons[buttonID]);
			}
		}
	}
}

/**
 * Loads score menu from the previous menu using "CHECK SCORE" as source button.
 */
static void GoToFileScore(s16 prevMenuButtonID, struct Object* sourceButton)
{
	s32 buttonID;
	// If the source button is in default state and the previous menu in full screen,
	// play zoom out sound and shrink previous menu
	if(sourceButton->oMenuButtonState == MENU_BUTTON_STATE_DEFAULT && sMainMenuButtons[prevMenuButtonID]->oMenuButtonState == MENU_BUTTON_STATE_FULLSCREEN)
	{
		AudStartSound(SOUND_MENU_CAMERA_ZOOM_OUT, gDefaultSoundArgs);
		sMainMenuButtons[prevMenuButtonID]->oMenuButtonState = MENU_BUTTON_STATE_SHRINKING;
		Floor						     = MENU_LAYER_MAIN;
	}
	// If the previous button is in default state
	if(sMainMenuButtons[prevMenuButtonID]->oMenuButtonState == MENU_BUTTON_STATE_DEFAULT)
	{
		// Hide buttons of corresponding button menu groups
		if(prevMenuButtonID == MENU_BUTTON_SCORE) //! Not possible, this is checking if the score menu
							  //! was opened from the score menu!
		{
			for(buttonID = MENU_BUTTON_SCORE_MIN; buttonID < MENU_BUTTON_SCORE_MAX; buttonID++)
			{
				RemoveShape(sMainMenuButtons[buttonID]);
			}
		}
		if(prevMenuButtonID == MENU_BUTTON_COPY)
		{
			for(buttonID = MENU_BUTTON_COPY_MIN; buttonID < MENU_BUTTON_COPY_MAX; buttonID++)
			{
				RemoveShape(sMainMenuButtons[buttonID]);
			}
		}
		if(prevMenuButtonID == MENU_BUTTON_ERASE)
		{
			for(buttonID = MENU_BUTTON_ERASE_MIN; buttonID < MENU_BUTTON_ERASE_MAX; buttonID++)
			{
				RemoveShape(sMainMenuButtons[buttonID]);
			}
		}
		// Play zoom in sound, select score menu and render it's buttons
		selectNo = MENU_BUTTON_SCORE;
		AudStartSound(SOUND_MENU_CAMERA_ZOOM_IN, gDefaultSoundArgs);
		sMainMenuButtons[MENU_BUTTON_SCORE]->oMenuButtonState = MENU_BUTTON_STATE_GROWING;
		ScoreScene_init(sMainMenuButtons[MENU_BUTTON_SCORE]);
	}
}

/**
 * Loads copy menu from the previous menu using "COPY FILE" as source button.
 */
static void GoToFileCopy(s16 prevMenuButtonID, struct Object* sourceButton)
{
	s32 buttonID;
	// If the source button is in default state and the previous menu in full screen,
	// play zoom out sound and shrink previous menu
	if(sourceButton->oMenuButtonState == MENU_BUTTON_STATE_DEFAULT && sMainMenuButtons[prevMenuButtonID]->oMenuButtonState == MENU_BUTTON_STATE_FULLSCREEN)
	{
		AudStartSound(SOUND_MENU_CAMERA_ZOOM_OUT, gDefaultSoundArgs);
		sMainMenuButtons[prevMenuButtonID]->oMenuButtonState = MENU_BUTTON_STATE_SHRINKING;
		Floor						     = MENU_LAYER_MAIN;
	}
	// If the previous button is in default state
	if(sMainMenuButtons[prevMenuButtonID]->oMenuButtonState == MENU_BUTTON_STATE_DEFAULT)
	{
		// Hide buttons of corresponding button menu groups
		if(prevMenuButtonID == MENU_BUTTON_SCORE)
		{
			for(buttonID = MENU_BUTTON_SCORE_MIN; buttonID < MENU_BUTTON_SCORE_MAX; buttonID++)
			{
				RemoveShape(sMainMenuButtons[buttonID]);
			}
		}
		if(prevMenuButtonID == MENU_BUTTON_COPY) //! Not possible, this is checking if the copy menu
							 //! was opened from the copy menu!
		{
			for(buttonID = MENU_BUTTON_COPY_MIN; buttonID < MENU_BUTTON_COPY_MAX; buttonID++)
			{
				RemoveShape(sMainMenuButtons[buttonID]);
			}
		}
		if(prevMenuButtonID == MENU_BUTTON_ERASE)
		{
			for(buttonID = MENU_BUTTON_ERASE_MIN; buttonID < MENU_BUTTON_ERASE_MAX; buttonID++)
			{
				RemoveShape(sMainMenuButtons[buttonID]);
			}
		}
		// Play zoom in sound, select copy menu and render it's buttons
		selectNo = MENU_BUTTON_COPY;
		AudStartSound(SOUND_MENU_CAMERA_ZOOM_IN, gDefaultSoundArgs);
		sMainMenuButtons[MENU_BUTTON_COPY]->oMenuButtonState = MENU_BUTTON_STATE_GROWING;
		CopyScene_init(sMainMenuButtons[MENU_BUTTON_COPY]);
	}
}

/**
 * Loads erase menu from the previous menu using "ERASE FILE" as source button.
 */
static void GoToFileRemove(s16 prevMenuButtonID, struct Object* sourceButton)
{
	s32 buttonID;
	// If the source button is in default state and the previous menu in full screen,
	// play zoom out sound and shrink previous menu
	if(sourceButton->oMenuButtonState == MENU_BUTTON_STATE_DEFAULT && sMainMenuButtons[prevMenuButtonID]->oMenuButtonState == MENU_BUTTON_STATE_FULLSCREEN)
	{
		AudStartSound(SOUND_MENU_CAMERA_ZOOM_OUT, gDefaultSoundArgs);
		sMainMenuButtons[prevMenuButtonID]->oMenuButtonState = MENU_BUTTON_STATE_SHRINKING;
		Floor						     = MENU_LAYER_MAIN;
	}
	// If the previous button is in default state
	if(sMainMenuButtons[prevMenuButtonID]->oMenuButtonState == MENU_BUTTON_STATE_DEFAULT)
	{
		// Hide buttons of corresponding button menu groups
		if(prevMenuButtonID == MENU_BUTTON_SCORE)
		{
			for(buttonID = MENU_BUTTON_SCORE_MIN; buttonID < MENU_BUTTON_SCORE_MAX; buttonID++)
			{
				RemoveShape(sMainMenuButtons[buttonID]);
			}
		}
		if(prevMenuButtonID == MENU_BUTTON_COPY)
		{
			for(buttonID = MENU_BUTTON_COPY_MIN; buttonID < MENU_BUTTON_COPY_MAX; buttonID++)
			{
				RemoveShape(sMainMenuButtons[buttonID]);
			}
		}
		if(prevMenuButtonID == MENU_BUTTON_ERASE) //! Not possible, this is checking if the erase menu
							  //! was opened from the erase menu!
		{
			for(buttonID = MENU_BUTTON_ERASE_MIN; buttonID < MENU_BUTTON_ERASE_MAX; buttonID++)
			{
				RemoveShape(sMainMenuButtons[buttonID]);
			}
		}
		// Play zoom in sound, select erase menu and render it's buttons
		selectNo = MENU_BUTTON_ERASE;
		AudStartSound(SOUND_MENU_CAMERA_ZOOM_IN, gDefaultSoundArgs);
		sMainMenuButtons[MENU_BUTTON_ERASE]->oMenuButtonState = MENU_BUTTON_STATE_GROWING;
		RemoveScene_init(sMainMenuButtons[MENU_BUTTON_ERASE]);
	}
}

/**
 * Menu Buttons Menu Manager Initial Action
 * Creates models of the buttons in the menu. For the Mario buttons it
 * checks if a save file exists to render an specific button model for it.
 * Unlike buttons on submenus, these are never hidden or recreated.
 */
void bhv_menu_button_manager_init()
{
	// File A
	if(BuIsActive(SAVE_FILE_A) == TRUE)
	{
		sMainMenuButtons[MENU_BUTTON_PLAY_FILE_A] = s_makeobj_relative(gCurrentObject, MODEL_MAIN_MENU_MARIO_SAVE_BUTTON_FADE, sm64::bhv::bhvMenuButton(), -6400, 2800, 0, 0, 0, 0);
	}
	else
	{
		sMainMenuButtons[MENU_BUTTON_PLAY_FILE_A] = s_makeobj_relative(gCurrentObject, MODEL_MAIN_MENU_MARIO_NEW_BUTTON_FADE, sm64::bhv::bhvMenuButton(), -6400, 2800, 0, 0, 0, 0);
	}
	sMainMenuButtons[MENU_BUTTON_PLAY_FILE_A]->oMenuButtonScale = 1.0f;
	// File B
	if(BuIsActive(SAVE_FILE_B) == TRUE)
	{
		sMainMenuButtons[MENU_BUTTON_PLAY_FILE_B] = s_makeobj_relative(gCurrentObject, MODEL_MAIN_MENU_MARIO_SAVE_BUTTON_FADE, sm64::bhv::bhvMenuButton(), 1500, 2800, 0, 0, 0, 0);
	}
	else
	{
		sMainMenuButtons[MENU_BUTTON_PLAY_FILE_B] = s_makeobj_relative(gCurrentObject, MODEL_MAIN_MENU_MARIO_NEW_BUTTON_FADE, sm64::bhv::bhvMenuButton(), 1500, 2800, 0, 0, 0, 0);
	}
	sMainMenuButtons[MENU_BUTTON_PLAY_FILE_B]->oMenuButtonScale = 1.0f;
	// File C
	if(BuIsActive(SAVE_FILE_C) == TRUE)
	{
		sMainMenuButtons[MENU_BUTTON_PLAY_FILE_C] = s_makeobj_relative(gCurrentObject, MODEL_MAIN_MENU_MARIO_SAVE_BUTTON_FADE, sm64::bhv::bhvMenuButton(), -6400, 0, 0, 0, 0, 0);
	}
	else
	{
		sMainMenuButtons[MENU_BUTTON_PLAY_FILE_C] = s_makeobj_relative(gCurrentObject, MODEL_MAIN_MENU_MARIO_NEW_BUTTON_FADE, sm64::bhv::bhvMenuButton(), -6400, 0, 0, 0, 0, 0);
	}
	sMainMenuButtons[MENU_BUTTON_PLAY_FILE_C]->oMenuButtonScale = 1.0f;
	// File D
	if(BuIsActive(SAVE_FILE_D) == TRUE)
	{
		sMainMenuButtons[MENU_BUTTON_PLAY_FILE_D] = s_makeobj_relative(gCurrentObject, MODEL_MAIN_MENU_MARIO_SAVE_BUTTON_FADE, sm64::bhv::bhvMenuButton(), 1500, 0, 0, 0, 0, 0);
	}
	else
	{
		sMainMenuButtons[MENU_BUTTON_PLAY_FILE_D] = s_makeobj_relative(gCurrentObject, MODEL_MAIN_MENU_MARIO_NEW_BUTTON_FADE, sm64::bhv::bhvMenuButton(), 1500, 0, 0, 0, 0, 0);
	}
	sMainMenuButtons[MENU_BUTTON_PLAY_FILE_D]->oMenuButtonScale = 1.0f;
	// Score menu button
	sMainMenuButtons[MENU_BUTTON_SCORE]		      = s_makeobj_relative(gCurrentObject, MODEL_MAIN_MENU_GREEN_SCORE_BUTTON, sm64::bhv::bhvMenuButton(), -6400, -3500, 0, 0, 0, 0);
	sMainMenuButtons[MENU_BUTTON_SCORE]->oMenuButtonScale = 1.0f;
	// Copy menu button
	sMainMenuButtons[MENU_BUTTON_COPY]		     = s_makeobj_relative(gCurrentObject, MODEL_MAIN_MENU_BLUE_COPY_BUTTON, sm64::bhv::bhvMenuButton(), -2134, -3500, 0, 0, 0, 0);
	sMainMenuButtons[MENU_BUTTON_COPY]->oMenuButtonScale = 1.0f;
	// Erase menu button
	sMainMenuButtons[MENU_BUTTON_ERASE]		      = s_makeobj_relative(gCurrentObject, MODEL_MAIN_MENU_RED_ERASE_BUTTON, sm64::bhv::bhvMenuButton(), 2134, -3500, 0, 0, 0, 0);
	sMainMenuButtons[MENU_BUTTON_ERASE]->oMenuButtonScale = 1.0f;
	// Sound mode menu button
	sMainMenuButtons[MENU_BUTTON_SOUND_MODE]		   = s_makeobj_relative(gCurrentObject, MODEL_MAIN_MENU_PURPLE_SOUND_BUTTON, sm64::bhv::bhvMenuButton(), 6400, -3500, 0, 0, 0, 0);
	sMainMenuButtons[MENU_BUTTON_SOUND_MODE]->oMenuButtonScale = 1.0f;

	alpha_value = 0;
}

#define SAVE_FILE_SOUND SOUND_MENU_STAR_SOUND_OKEY_DOKEY

/**
 * In the main menu, check if a button was clicked to play it's button growing state.
 * Also play a sound and/or render buttons depending of the button ID selected.
 */
static void FileSelectScene()
{
	// Sound mode menu is handled separately because the button ID for it
	// is not grouped with the IDs of the other submenus.
	if(ButtonClickCheck(sMainMenuButtons[MENU_BUTTON_SOUND_MODE]->oPosX, sMainMenuButtons[MENU_BUTTON_SOUND_MODE]->oPosY, 200.0f) == TRUE)
	{
		sMainMenuButtons[MENU_BUTTON_SOUND_MODE]->oMenuButtonState = MENU_BUTTON_STATE_GROWING;
		selectNo						   = MENU_BUTTON_SOUND_MODE;
	}
	else
	{
		// Main Menu buttons
		s8 buttonID;
		// Configure Main Menu button group
		for(buttonID = MENU_BUTTON_MAIN_MIN; buttonID < MENU_BUTTON_MAIN_MAX; buttonID++)
		{
			s16 buttonX = sMainMenuButtons[buttonID]->oPosX;
			s16 buttonY = sMainMenuButtons[buttonID]->oPosY;

			if(ButtonClickCheck(buttonX, buttonY, 200.0f) == TRUE)
			{
				// If menu button clicked, select it
				sMainMenuButtons[buttonID]->oMenuButtonState = MENU_BUTTON_STATE_GROWING;
				selectNo				     = buttonID;
				break;
			}
		}
	}

	// Play sound of the save file clicked
	switch(selectNo)
	{
		case MENU_BUTTON_PLAY_FILE_A:
			AudStartSound(SAVE_FILE_SOUND, gDefaultSoundArgs);
			SendMotorEvent(60, 70);
			SendMotorDecay(1);
			break;
		case MENU_BUTTON_PLAY_FILE_B:
			AudStartSound(SAVE_FILE_SOUND, gDefaultSoundArgs);
			SendMotorEvent(60, 70);
			SendMotorDecay(1);
			break;
		case MENU_BUTTON_PLAY_FILE_C:
			AudStartSound(SAVE_FILE_SOUND, gDefaultSoundArgs);
			SendMotorEvent(60, 70);
			SendMotorDecay(1);
			break;
		case MENU_BUTTON_PLAY_FILE_D:
			AudStartSound(SAVE_FILE_SOUND, gDefaultSoundArgs);
			SendMotorEvent(60, 70);
			SendMotorDecay(1);
			break;
			// Play sound of the button clicked and render buttons of that menu.
		case MENU_BUTTON_SCORE:
			AudStartSound(SOUND_MENU_CAMERA_ZOOM_IN, gDefaultSoundArgs);
			SendMotorEvent(5, 80);
			ScoreScene_init(sMainMenuButtons[MENU_BUTTON_SCORE]);
			break;
		case MENU_BUTTON_COPY:
			AudStartSound(SOUND_MENU_CAMERA_ZOOM_IN, gDefaultSoundArgs);
			SendMotorEvent(5, 80);
			CopyScene_init(sMainMenuButtons[MENU_BUTTON_COPY]);
			break;
		case MENU_BUTTON_ERASE:
			AudStartSound(SOUND_MENU_CAMERA_ZOOM_IN, gDefaultSoundArgs);
			SendMotorEvent(5, 80);
			RemoveScene_init(sMainMenuButtons[MENU_BUTTON_ERASE]);
			break;
		case MENU_BUTTON_SOUND_MODE:
			AudStartSound(SOUND_MENU_CAMERA_ZOOM_IN, gDefaultSoundArgs);
			SendMotorEvent(5, 80);
			SoundScene_init(sMainMenuButtons[MENU_BUTTON_SOUND_MODE]);
			break;
	}
}
#undef SAVE_FILE_SOUND

/**
 * Menu Buttons Menu Manager Loop Action
 * Calls a menu function depending of the button chosen.
 * selectNo is MENU_BUTTON_NONE when the file select
 * is loaded, and that checks what buttonID is clicked in the main menu.
 */
void bhv_menu_button_manager_loop()
{
	switch(selectNo)
	{
		case MENU_BUTTON_NONE:
			FileSelectScene();
			break;
		case MENU_BUTTON_PLAY_FILE_A:
			GoToGame(sMainMenuButtons[MENU_BUTTON_PLAY_FILE_A], 1);
			break;
		case MENU_BUTTON_PLAY_FILE_B:
			GoToGame(sMainMenuButtons[MENU_BUTTON_PLAY_FILE_B], 2);
			break;
		case MENU_BUTTON_PLAY_FILE_C:
			GoToGame(sMainMenuButtons[MENU_BUTTON_PLAY_FILE_C], 3);
			break;
		case MENU_BUTTON_PLAY_FILE_D:
			GoToGame(sMainMenuButtons[MENU_BUTTON_PLAY_FILE_D], 4);
			break;
		case MENU_BUTTON_SCORE:
			FileScoreScene(sMainMenuButtons[MENU_BUTTON_SCORE]);
			break;
		case MENU_BUTTON_COPY:
			FileCopyScene(sMainMenuButtons[MENU_BUTTON_COPY]);
			break;
		case MENU_BUTTON_ERASE:
			FileRemoveScene(sMainMenuButtons[MENU_BUTTON_ERASE]);
			break;

		case MENU_BUTTON_SCORE_FILE_A:
			WatchFileScore(sMainMenuButtons[MENU_BUTTON_SCORE_FILE_A], MENU_BUTTON_SCORE);
			break;
		case MENU_BUTTON_SCORE_FILE_B:
			WatchFileScore(sMainMenuButtons[MENU_BUTTON_SCORE_FILE_B], MENU_BUTTON_SCORE);
			break;
		case MENU_BUTTON_SCORE_FILE_C:
			WatchFileScore(sMainMenuButtons[MENU_BUTTON_SCORE_FILE_C], MENU_BUTTON_SCORE);
			break;
		case MENU_BUTTON_SCORE_FILE_D:
			WatchFileScore(sMainMenuButtons[MENU_BUTTON_SCORE_FILE_D], MENU_BUTTON_SCORE);
			break;
		case MENU_BUTTON_SCORE_RETURN:
			GoToFileSelect(MENU_BUTTON_SCORE, sMainMenuButtons[MENU_BUTTON_SCORE_RETURN]);
			break;
		case MENU_BUTTON_SCORE_COPY_FILE:
			GoToFileCopy(MENU_BUTTON_SCORE, sMainMenuButtons[MENU_BUTTON_SCORE_COPY_FILE]);
			break;
		case MENU_BUTTON_SCORE_ERASE_FILE:
			GoToFileRemove(MENU_BUTTON_SCORE, sMainMenuButtons[MENU_BUTTON_SCORE_ERASE_FILE]);
			break;

		case MENU_BUTTON_COPY_FILE_A:
			break;
		case MENU_BUTTON_COPY_FILE_B:
			break;
		case MENU_BUTTON_COPY_FILE_C:
			break;
		case MENU_BUTTON_COPY_FILE_D:
			break;
		case MENU_BUTTON_COPY_RETURN:
			GoToFileSelect(MENU_BUTTON_COPY, sMainMenuButtons[MENU_BUTTON_COPY_RETURN]);
			break;
		case MENU_BUTTON_COPY_CHECK_SCORE:
			GoToFileScore(MENU_BUTTON_COPY, sMainMenuButtons[MENU_BUTTON_COPY_CHECK_SCORE]);
			break;
		case MENU_BUTTON_COPY_ERASE_FILE:
			GoToFileRemove(MENU_BUTTON_COPY, sMainMenuButtons[MENU_BUTTON_COPY_ERASE_FILE]);
			break;

		case MENU_BUTTON_ERASE_FILE_A:
			break;
		case MENU_BUTTON_ERASE_FILE_B:
			break;
		case MENU_BUTTON_ERASE_FILE_C:
			break;
		case MENU_BUTTON_ERASE_FILE_D:
			break;
		case MENU_BUTTON_ERASE_RETURN:
			GoToFileSelect(MENU_BUTTON_ERASE, sMainMenuButtons[MENU_BUTTON_ERASE_RETURN]);
			break;
		case MENU_BUTTON_ERASE_CHECK_SCORE:
			GoToFileScore(MENU_BUTTON_ERASE, sMainMenuButtons[MENU_BUTTON_ERASE_CHECK_SCORE]);
			break;
		case MENU_BUTTON_ERASE_COPY_FILE:
			GoToFileCopy(MENU_BUTTON_ERASE, sMainMenuButtons[MENU_BUTTON_ERASE_COPY_FILE]);
			break;

		case MENU_BUTTON_SOUND_MODE:
			SoundSelectScene(sMainMenuButtons[MENU_BUTTON_SOUND_MODE]);
			break;
		case MENU_BUTTON_STEREO:
			GoToFileSelect(MENU_BUTTON_SOUND_MODE, sMainMenuButtons[MENU_BUTTON_STEREO]);
			break;
		case MENU_BUTTON_MONO:
			GoToFileSelect(MENU_BUTTON_SOUND_MODE, sMainMenuButtons[MENU_BUTTON_MONO]);
			break;
		case MENU_BUTTON_HEADSET:
			GoToFileSelect(MENU_BUTTON_SOUND_MODE, sMainMenuButtons[MENU_BUTTON_HEADSET]);
			break;
	}

	cursol_ptr[0] = -10000;
	cursol_ptr[1] = -10000;
}

#include "selectmess.hpp"

/**
 * Initiates file select values after Mario Screen.
 * Relocates cursor position of the last save if the game goes back to the Mario Screen
 * either completing a course choosing "SAVE & QUIT" or having a game over.
 */
s32 SeSelectInitProcess(s32 arg, s32 unused)
{
	selectNo    = MENU_BUTTON_NONE;
	Floor	    = MENU_LAYER_MAIN;
	alpha_value = 0;
	// Place the cursor over the save file that was being played.
	// activePlayerNo is 1 by default when the game boots, as such
	// the cursor will point on Mario A save file.
	switch(activePlayerNo)
	{
		case 1: // File A
			cursol_pos[0] = -94.0f;
			cursol_pos[1] = 46.0f;
			break;
		case 2: // File B
			cursol_pos[0] = 24.0f;
			cursol_pos[1] = 46.0f;
			break;
		case 3: // File C
			cursol_pos[0] = -94.0f;
			cursol_pos[1] = 5.0f;
			break;
		case 4: // File D
			cursol_pos[0] = 24.0f;
			cursol_pos[1] = 5.0f;
			break;
	}

	cursol_ptr[0] = -10000;
	cursol_ptr[1] = -10000;

	cursol_flag = 0;
	goTogame    = 0;

	click_file    = MENU_BUTTON_NONE;
	click_flag    = 0;
	click_mess    = 0;
	click_alpha   = 0;
	click_counter = 0;
	yesNo_flag    = MENU_ERASE_HOVER_NONE;
	sound_flag    = BuGetSoundMode();

	return 0;
}

/**
 * Updates file select menu button objects so they can be interacted.
 * When a save file is selected, it returns fileNum value
 * defined in GoToGame.
 */
s32 SeSelectProcess(s32 code, s32 param)
{
	SnExecuteStrategy();
	return goTogame;
}
