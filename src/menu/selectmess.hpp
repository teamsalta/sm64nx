#include "game/motor.h"

/**
 * Cursor function that handles button inputs.
 * If the cursor is clicked, cursol_ptr uses the same value as cursol_pos.
 */
static void ContButton()
{
	// If scoring a file, pressing A just changes the coin score mode.
	if(selectNo == MENU_BUTTON_SCORE_FILE_A || selectNo == MENU_BUTTON_SCORE_FILE_B || selectNo == MENU_BUTTON_SCORE_FILE_C || selectNo == MENU_BUTTON_SCORE_FILE_D)
	{
		if(sm64::player(0).controller().buttonPressed & (sm64::hid::B_BUTTON | sm64::hid::START_BUTTON))
		{
			cursol_ptr[0] = cursol_pos[0];
			cursol_ptr[1] = cursol_pos[1];
			cursol_flag   = 1;
		}
		else if(sm64::player(0).controller().buttonPressed & sm64::hid::A_BUTTON)
		{
			sScoreFileCoinScoreMode = 1 - sScoreFileCoinScoreMode;
			AudStartSound(SOUND_MENU_CLICK_FILE_SELECT, gDefaultSoundArgs);
		}
	}
	else
	{ // If cursor is clicked
		if(sm64::player(0).controller().buttonPressed & (sm64::hid::A_BUTTON | sm64::hid::B_BUTTON | sm64::hid::START_BUTTON))
		{
			cursol_ptr[0] = cursol_pos[0];
			cursol_ptr[1] = cursol_pos[1];
			cursol_flag   = 1;
		}
	}
}

/**
 * Cursor function that handles analog stick input and button presses with a function near the end.
 */
static void CursolMove()
{
	s16 rawStickX = sm64::player(0).controller().rawStickX;
	s16 rawStickY = sm64::player(0).controller().rawStickY;

	// Handle deadzone
	if(rawStickY > -2 && rawStickY < 2)
	{
		rawStickY = 0;
	}
	if(rawStickX > -2 && rawStickX < 2)
	{
		rawStickX = 0;
	}

	// Move cursor
	cursol_pos[0] += rawStickX / (8 * FRAME_RATE_SCALER_INV);
	cursol_pos[1] += rawStickY / (8 * FRAME_RATE_SCALER_INV);

	// Stop cursor from going offscreen
	if(cursol_pos[0] > 132.0f)
	{
		cursol_pos[0] = 132.0f;
	}
	if(cursol_pos[0] < -132.0f)
	{
		cursol_pos[0] = -132.0f;
	}

	if(cursol_pos[1] > 90.0f)
	{
		cursol_pos[1] = 90.0f;
	}
	if(cursol_pos[1] < -90.0f)
	{
		cursol_pos[1] = -90.0f;
	}

	if(cursol_flag == 0)
	{
		ContButton();
	}
}

/**
 * Prints the cursor (Mario Hand, different to the one in the Mario screen)
 * and loads it's controller inputs in CursolMove
 * to be usable on the file select.
 */
static void print_menu_cursor()
{
	CursolMove();
	iTranslate(MENU_MTX_PUSH, cursol_pos[0] + 160.0f - 5.0, cursol_pos[1] + 120.0f - 25.0, 0.0f);
	// Get the right graphic to use for the cursor.
	if(cursol_flag == 0)
		// Idle
		gSPDisplayList(gDisplayListHead++, dl_menu_idle_hand);
	if(cursol_flag != 0)
		// Grabbing
		gSPDisplayList(gDisplayListHead++, dl_menu_grabbing_hand);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
	if(cursol_flag != 0)
	{
		cursol_flag++; // This is a very strange way to implement a timer? It counts up and
			       // then resets to 0 instead of just counting down to 0.
		if(cursol_flag == 5 * FRAME_RATE_SCALER_INV)
		{
			cursol_flag = 0;
		}
	}
}

/**
 * Prints a hud string depending of the hud table list defined with text fade properties.
 */
static void print_hud_lut_string_fade(s8 hudLUT, s16 x, s16 y, const unsigned char* text)
{
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value - click_alpha);
	Draw16bitFont(hudLUT, x, y, text);
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
}

/**
 * Prints a generic white string with text fade properties.
 */
static void print_generic_string_fade(s16 x, s16 y, const unsigned char* text)
{
	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value - click_alpha);
	Draw8bitFont(x, y, text);
	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

/**
 * Updates text fade at the top of a menu.
 */
static s32 update_text_fade_out()
{
	if(click_flag == TRUE)
	{
		click_alpha += 50 / FRAME_RATE_SCALER_INV;
		if(click_alpha == 250)
		{
			click_flag = FALSE;
			return TRUE;
		}
	}
	else
	{
		if(click_alpha > 0)
		{
			click_alpha -= 50;
		}
	}
	return FALSE;
}

/**
 * Prints the amount of stars of a save file.
 * If a save doesn't exist, print "NEW" instead.
 */
static void print_save_file_star_count(s8 fileIndex, s16 x, s16 y)
{
	u8 starCountText[4];
	s8 offset = 0;
	s16 starCount;

	if(BuIsActive(fileIndex) == TRUE)
	{
		starCount = BuGetSumStars(fileIndex, 0, 24);
		// Print star icon
		Draw16bitFont(HUD_LUT_GLOBAL, x, y, starIcon);
		// If star count is less than 100, print x icon and move
		// the star count text one digit to the right.
		if(starCount < 100)
		{
			Draw16bitFont(HUD_LUT_GLOBAL, x + 16, y, xIcon);
			offset = 16;
		}
		// Print star count
		I_itochar(starCount, starCountText);
		Draw16bitFont(HUD_LUT_GLOBAL, x + offset + 16, y, starCountText);
	}
	else
	{
		// Print "new" text
		Draw16bitFont(HUD_LUT_GLOBAL, x, y, textNew);
	}
}

#define SELECT_FILE_X 93
#define SCORE_X 52
#define COPY_X 117
#define ERASE_X 177
#define SOUNDMODE_X1 sSoundTextX

/**
 * Prints main menu strings that shows on the yellow background menu screen.
 */
static void PrintCourseSelectScene()
{
	// Print "SELECT FILE" text
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value);
	Draw16bitFont(HUD_LUT_DIFF, SELECT_FILE_X, 35, textSelectFile);
	// Print file star counts
	print_save_file_star_count(SAVE_FILE_A, 92, 78);
	print_save_file_star_count(SAVE_FILE_B, 209, 78);
	print_save_file_star_count(SAVE_FILE_C, 92, 118);
	print_save_file_star_count(SAVE_FILE_D, 209, 118);
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
	// Print menu names
	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value);
	Draw8bitFont(SCORE_X, 39, textScore);
	Draw8bitFont(COPY_X, 39, textCopy);
	Draw8bitFont(ERASE_X, 39, textErase);
	sSoundTextX = CharCentering(254, textSoundModes[sound_flag], 10.0f);
	Draw8bitFont(SOUNDMODE_X1, 39, textSoundModes[sound_flag]);
	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
	// Print file names
	gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value);
	print_menu_generic_string(92, 65, textMarioA);
	print_menu_generic_string(207, 65, textMarioB);
	print_menu_generic_string(92, 105, textMarioC);
	print_menu_generic_string(207, 105, textMarioD);
	gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_end);
}

#define CHECK_FILE_X 95
#define NOSAVE_DATA_X1 99

/**
 * Defines IDs for the top message of the score menu and displays it if the ID is called in messageID.
 */
static void score_menu_display_message(s8 messageID)
{
	switch(messageID)
	{
		case SCORE_MSG_CHECK_FILE:
			print_hud_lut_string_fade(HUD_LUT_DIFF, CHECK_FILE_X, 35, textCheckFile);
			break;
		case SCORE_MSG_NOSAVE_DATA:
			print_generic_string_fade(NOSAVE_DATA_X1, 190, textNoSavedDataExists);
			break;
	}
}

#define RETURN_X 44
#define COPYFILE_X1 135
#define ERASEFILE_X1 231

/**
 * Prints score menu strings that shows on the green background menu screen.
 */
static void PrintCourseScoreScene()
{
	// Update and print the message at the top of the menu.
	if(click_counter == 20 * FRAME_RATE_SCALER_INV)
	{
		click_flag = TRUE;
	}
	if(update_text_fade_out() == TRUE)
	{
		if(click_mess == SCORE_MSG_CHECK_FILE)
		{
			click_mess = SCORE_MSG_NOSAVE_DATA;
		}
		else
		{
			click_mess = SCORE_MSG_CHECK_FILE;
		}
	}
	// Print messageID called above
	score_menu_display_message(click_mess);
	// Print file star counts
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value);
	print_save_file_star_count(SAVE_FILE_A, 90, 76);
	print_save_file_star_count(SAVE_FILE_B, 211, 76);
	print_save_file_star_count(SAVE_FILE_C, 90, 119);
	print_save_file_star_count(SAVE_FILE_D, 211, 119);
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
	// Print menu names
	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value);
	Draw8bitFont(RETURN_X, 35, textReturn);
	Draw8bitFont(COPYFILE_X1, 35, textCopyFileButton);
	Draw8bitFont(ERASEFILE_X1, 35, textEraseFileButton);
	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
	// Print file names
	gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value);
	print_menu_generic_string(89, 62, textMarioA);
	print_menu_generic_string(211, 62, textMarioB);
	print_menu_generic_string(89, 105, textMarioC);
	print_menu_generic_string(211, 105, textMarioD);
	gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_end);
}

#define NOFILE_COPY_X 119
#define COPY_FILE_X 104
#define COPYIT_WHERE_X 109
#define NOSAVE_DATA_X2 101
#define COPYCOMPLETE_X 110
#define SAVE_EXISTS_X1 110

/**
 * Defines IDs for the top message of the copy menu and displays it if the ID is called in messageID.
 */
static void copy_menu_display_message(s8 messageID)
{
	switch(messageID)
	{
		case COPY_MSG_MAIN_TEXT:
			if(sAllFilesExist == TRUE)
			{
				print_generic_string_fade(NOFILE_COPY_X, 190, textNoFileToCopyFrom);
			}
			else
			{
				print_hud_lut_string_fade(HUD_LUT_DIFF, COPY_FILE_X, 35, textCopyFile);
			}
			break;
		case COPY_MSG_COPY_WHERE:
			print_generic_string_fade(COPYIT_WHERE_X, 190, textCopyItToWhere);
			break;
		case COPY_MSG_NOSAVE_EXISTS:
			print_generic_string_fade(NOSAVE_DATA_X2, 190, textNoSavedDataExistsCopy);
			break;
		case COPY_MSG_COPY_COMPLETE:
			print_generic_string_fade(COPYCOMPLETE_X, 190, textCopyCompleted);
			break;
		case COPY_MSG_SAVE_EXISTS:
			print_generic_string_fade(SAVE_EXISTS_X1, 190, textSavedDataExists);
			break;
	}
}

/**
 * Updates messageIDs of the copy menu depending of the copy phase value defined.
 */
static void copy_menu_update_message()
{
	switch(sMainMenuButtons[MENU_BUTTON_COPY]->oMenuButtonActionPhase)
	{
		case COPY_PHASE_MAIN:
			if(click_counter == 20 * FRAME_RATE_SCALER_INV)
			{
				click_flag = TRUE;
			}
			if(update_text_fade_out() == TRUE)
			{
				if(click_mess == COPY_MSG_MAIN_TEXT)
				{
					click_mess = COPY_MSG_NOSAVE_EXISTS;
				}
				else
				{
					click_mess = COPY_MSG_MAIN_TEXT;
				}
			}
			break;
		case COPY_PHASE_COPY_WHERE:
			if(click_counter == 20 * FRAME_RATE_SCALER_INV && click_mess == COPY_MSG_SAVE_EXISTS)
			{
				click_flag = TRUE;
			}
			if(update_text_fade_out() == TRUE)
			{
				if(click_mess != COPY_MSG_COPY_WHERE)
				{
					click_mess = COPY_MSG_COPY_WHERE;
				}
				else
				{
					click_mess = COPY_MSG_SAVE_EXISTS;
				}
			}
			break;
		case COPY_PHASE_COPY_COMPLETE:
			if(click_counter == 20 * FRAME_RATE_SCALER_INV)
			{
				click_flag = TRUE;
			}
			if(update_text_fade_out() == TRUE)
			{
				if(click_mess != COPY_MSG_COPY_COMPLETE)
				{
					click_mess = COPY_MSG_COPY_COMPLETE;
				}
				else
				{
					click_mess = COPY_MSG_MAIN_TEXT;
				}
			}
			break;
	}
}

#define VIEWSCORE_X1 128
#define ERASEFILE_X2 230

/**
 * Prints copy menu strings that shows on the blue background menu screen.
 */
static void PrintFileCopyScene()
{
	// Update and print the message at the top of the menu.
	copy_menu_update_message();
	// Print messageID called inside a copy_menu_update_message case
	copy_menu_display_message(click_mess);
	// Print file star counts
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value);
	print_save_file_star_count(SAVE_FILE_A, 90, 76);
	print_save_file_star_count(SAVE_FILE_B, 211, 76);
	print_save_file_star_count(SAVE_FILE_C, 90, 119);
	print_save_file_star_count(SAVE_FILE_D, 211, 119);
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
	// Print menu names
	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value);
	Draw8bitFont(RETURN_X, 35, textReturn);
	Draw8bitFont(VIEWSCORE_X1, 35, textViewScore);
	Draw8bitFont(ERASEFILE_X2, 35, textEraseFileButton);
	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
	// Print file names
	gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value);
	print_menu_generic_string(89, 62, textMarioA);
	print_menu_generic_string(211, 62, textMarioB);
	print_menu_generic_string(89, 105, textMarioC);
	print_menu_generic_string(211, 105, textMarioD);
	gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_end);
}

#define CURSOR_X (x + 0x46)
#define MENU_ERASE_YES_MIN_X 0x8C
#define MENU_ERASE_YES_MAX_X 0xA9

#define MENU_ERASE_YES_NO_MIN_Y 0xBF
#define MENU_ERASE_YES_NO_MAX_Y 0xD2
#define MENU_ERASE_NO_MIN_X 0xBD
#define MENU_ERASE_NO_MAX_X 0xDA

/**
 * Prints the "YES NO" prompt and checks if one of the prompts are hovered to do it's functions.
 */
static void DrawYesNo(s16 x, s16 y)
{
	s16 colorFade = (frameCounter / FRAME_RATE_SCALER_INV) << 12;

	s16 cursorX = cursol_pos[0] + CURSOR_X;
	s16 cursorY = cursol_pos[1] + 120.0f;

	if(cursorX < MENU_ERASE_YES_MAX_X && cursorX >= MENU_ERASE_YES_MIN_X && cursorY < MENU_ERASE_YES_NO_MAX_Y && cursorY >= MENU_ERASE_YES_NO_MIN_Y)
	{
		// Fade "YES" string color but keep "NO" gray
		sYesNoColor[0] = sins(colorFade) * 50.0f + 205.0f;
		sYesNoColor[1] = 150;
		yesNo_flag     = MENU_ERASE_HOVER_YES;
	}
	else if(cursorX < MENU_ERASE_NO_MAX_X && cursorX >= MENU_ERASE_NO_MIN_X && cursorY < MENU_ERASE_YES_NO_MAX_Y && cursorY >= MENU_ERASE_YES_NO_MIN_Y)
	{
		// Fade "NO" string color but keep "YES" gray
		sYesNoColor[0] = 150;
		sYesNoColor[1] = sins(colorFade) * 50.0f + 205.0f;
		yesNo_flag     = MENU_ERASE_HOVER_NO;
	}
	else
	{
		// Don't fade both strings and keep them gray
		sYesNoColor[0] = 150;
		sYesNoColor[1] = 150;
		yesNo_flag     = MENU_ERASE_HOVER_NONE;
	}
	// If the cursor is clicked...
	if(cursol_flag == 2 * FRAME_RATE_SCALER_INV)
	{
		// ..and is hovering "YES", delete file
		if(yesNo_flag == MENU_ERASE_HOVER_YES)
		{
			AudStartSound(SOUND_MARIO_WAAAOOOW, gDefaultSoundArgs);
			SendMotorEvent(5, 80);
			sMainMenuButtons[MENU_BUTTON_ERASE]->oMenuButtonActionPhase = ERASE_PHASE_MARIO_ERASED;
			click_flag						    = TRUE;
			click_counter						    = 0;
			BuClearStorage(click_file);
			sMainMenuButtons[MENU_BUTTON_ERASE_MIN + click_file]->header.gfx.sharedChild = stageShapes[MODEL_MAIN_MENU_MARIO_NEW_BUTTON_FADE];
			sMainMenuButtons[click_file]->header.gfx.sharedChild			     = stageShapes[MODEL_MAIN_MENU_MARIO_NEW_BUTTON_FADE];
			yesNo_flag								     = MENU_ERASE_HOVER_NONE;
			// ..and is hovering "NO", return back to main phase
		}
		else if(yesNo_flag == MENU_ERASE_HOVER_NO)
		{
			AudStartSound(SOUND_MENU_CLICK_FILE_SELECT, gDefaultSoundArgs);
			SendMotorEvent(5, 80);
			sMainMenuButtons[MENU_BUTTON_ERASE_MIN + click_file]->oMenuButtonState = MENU_BUTTON_STATE_ZOOM_OUT;
			sMainMenuButtons[MENU_BUTTON_ERASE]->oMenuButtonActionPhase	       = ERASE_PHASE_MAIN;
			click_flag							       = TRUE;
			click_counter							       = 0;
			yesNo_flag							       = MENU_ERASE_HOVER_NONE;
		}
	}
	// Print "YES NO" strings
	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, sYesNoColor[0], sYesNoColor[0], sYesNoColor[0], alpha_value);
	Draw8bitFont(x + 56, y, textYes);
	gDPSetEnvColor(gDisplayListHead++, sYesNoColor[1], sYesNoColor[1], sYesNoColor[1], alpha_value);
	Draw8bitFont(x + 98, y, textNo);
	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

#define ERASE_FILE_X 98
#define NOSAVE_DATA_X3 100
#define MARIO_ERASED_VAR 6
#define MARIO_ERASED_X 100
#define SAVE_EXISTS_X2 100

/**
 * Defines IDs for the top message of the erase menu and displays it if the ID is called in messageID.
 */
static void erase_menu_display_message(s8 messageID)
{
	unsigned char textEraseFile[]	      = {TEXT_ERASE_FILE};
	unsigned char textSure[]	      = {TEXT_SURE};
	unsigned char textNoSavedDataExists[] = {TEXT_NO_SAVED_DATA_EXISTS};
	unsigned char textMarioAJustErased[]  = {TEXT_FILE_MARIO_A_JUST_ERASED};
	unsigned char textSavedDataExists[]   = {TEXT_SAVED_DATA_EXISTS};

	switch(messageID)
	{
		case ERASE_MSG_MAIN_TEXT:
			print_hud_lut_string_fade(HUD_LUT_DIFF, ERASE_FILE_X, 35, textEraseFile);
			break;
		case ERASE_MSG_PROMPT:
			print_generic_string_fade(90, 190, textSure);
			DrawYesNo(90, 190); // YES NO, has functions for it too
			break;
		case ERASE_MSG_NOSAVE_EXISTS:
			print_generic_string_fade(NOSAVE_DATA_X3, 190, textNoSavedDataExists);
			break;
		case ERASE_MSG_MARIO_ERASED:
			textMarioAJustErased[MARIO_ERASED_VAR] = click_file + 10;
			print_generic_string_fade(MARIO_ERASED_X, 190, textMarioAJustErased);
			break;
		case ERASE_MSG_SAVE_EXISTS: // size
			print_generic_string_fade(SAVE_EXISTS_X2, 190, textSavedDataExists);
			break;
	}
}

/**
 * Updates messageIDs of the erase menu depending of the erase phase value defined.
 */
static void erase_menu_update_message()
{
	switch(sMainMenuButtons[MENU_BUTTON_ERASE]->oMenuButtonActionPhase)
	{
		case ERASE_PHASE_MAIN:
			if(click_counter == 20 * FRAME_RATE_SCALER_INV && click_mess == ERASE_MSG_NOSAVE_EXISTS)
			{
				click_flag = TRUE;
			}
			if(update_text_fade_out() == TRUE)
			{
				if(click_mess == ERASE_MSG_MAIN_TEXT)
				{
					click_mess = ERASE_MSG_NOSAVE_EXISTS;
				}
				else
				{
					click_mess = ERASE_MSG_MAIN_TEXT;
				}
			}
			break;
		case ERASE_PHASE_PROMPT:
			if(update_text_fade_out() == TRUE)
			{
				if(click_mess != ERASE_MSG_PROMPT)
				{
					click_mess = ERASE_MSG_PROMPT;
				}
				cursol_pos[0] = 43.0f;
				cursol_pos[1] = 80.0f;
			}
			break;
		case ERASE_PHASE_MARIO_ERASED:
			if(click_counter == 20 * FRAME_RATE_SCALER_INV)
			{
				click_flag = TRUE;
			}
			if(update_text_fade_out() == TRUE)
			{
				if(click_mess != ERASE_MSG_MARIO_ERASED)
				{
					click_mess = ERASE_MSG_MARIO_ERASED;
				}
				else
				{
					click_mess = ERASE_MSG_MAIN_TEXT;
				}
			}
			break;
	}
}

#define VIEWSCORE_X2 127
#define COPYFILE_X2 233

/**
 * Prints erase menu strings that shows on the red background menu screen.
 */
static void PrintFileRemoveScene()
{
	// Update and print the message at the top of the menu.
	erase_menu_update_message();
	// Print messageID called inside a erase_menu_update_message case
	erase_menu_display_message(click_mess);
	// Print file star counts
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value);
	print_save_file_star_count(SAVE_FILE_A, 90, 76);
	print_save_file_star_count(SAVE_FILE_B, 211, 76);
	print_save_file_star_count(SAVE_FILE_C, 90, 119);
	print_save_file_star_count(SAVE_FILE_D, 211, 119);
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
	// Print menu names
	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value);
	Draw8bitFont(RETURN_X, 35, textReturn);
	Draw8bitFont(VIEWSCORE_X2, 35, textViewScore);
	Draw8bitFont(COPYFILE_X2, 35, textCopyFileButton);
	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
	// Print file names
	gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value);
	print_menu_generic_string(89, 62, textMarioA);
	print_menu_generic_string(211, 62, textMarioB);
	print_menu_generic_string(89, 105, textMarioC);
	print_menu_generic_string(211, 105, textMarioD);
	gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_end);
}

#define SOUND_SELECT_X 88
#define SOUNDMODE_X2 textX

/**
 * Prints sound mode menu strings that shows on the purple background menu screen.
 */
static void PrintSoundSelect()
{
	s32 mode;
	s16 textX;
	unsigned char textSoundSelect[] = {TEXT_SOUND_SELECT};
	// Print "SOUND SELECT" text
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value);
	Draw16bitFont(HUD_LUT_DIFF, SOUND_SELECT_X, 35, textSoundSelect);
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
	// Print mode names
	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	for(mode = 0; mode < 3; mode++)
	{
		if(mode == sound_flag)
		{
			gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value);
		}
		else
		{
			gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, alpha_value);
		}
		// Mode names are centered correctly on US
		textX = CharCentering(mode * 74 + 87, textSoundModes[mode], 10.0f);
		Draw8bitFont(SOUNDMODE_X2, 87, textSoundModes[mode]);
	}
	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

static unsigned char textStarX[] = {TEXT_STAR_X};

/**
 * Prints castle secret stars collected in a score menu save file.
 */
static void print_score_file_castle_secret_stars(s8 fileIndex, s16 x, s16 y)
{
	unsigned char secretStarsText[20];
	// Print "[star] x"
	print_menu_generic_string(x, y, textStarX);
	// Print number of castle secret stars
	I_itochar(BuGetSumStars(fileIndex, 15, 24), secretStarsText);
	print_menu_generic_string(x + 16, y, secretStarsText);
}

/**
 * Prints course coins collected in a score menu save file.
 */
static void print_score_file_course_coin_score(s8 fileIndex, s16 courseIndex, s16 x, s16 y)
{
	unsigned char coinScoreText[20];
	u8 stars		  = BuGetStarFlag(fileIndex, courseIndex);
	unsigned char textCoinX[] = {TEXT_COIN_X};
	unsigned char textStar[]  = {TEXT_STAR};
#define LENGTH 8
	unsigned char fileNames[][LENGTH] = {
	    {TEXT_4DASHES}, // huh?
	    {TEXT_SCORE_MARIO_A}, {TEXT_SCORE_MARIO_B}, {TEXT_SCORE_MARIO_C}, {TEXT_SCORE_MARIO_D},
	};
#undef LENGTH
	// MYSCORE
	if(sScoreFileCoinScoreMode == 0)
	{
		// Print "[coin] x"
		print_menu_generic_string(x + 25, y, textCoinX);
		// Print coin score
		I_itochar(BuGetNumCoins(fileIndex, courseIndex), coinScoreText);
		print_menu_generic_string(x + 41, y, coinScoreText);
		// If collected, print 100 coin star
		if(stars & (1 << 6))
		{
			print_menu_generic_string(x + 70, y, textStar);
		}
	}
	// HISCORE
	else
	{
		// Print "[coin] x"
		print_menu_generic_string(x + 18, y, textCoinX);
		// Print coin highscore
		I_itochar((u16)BuGetWinScore(courseIndex) & 0xFFFF, coinScoreText);
		print_menu_generic_string(x + 34, y, coinScoreText);
		// Print coin highscore file
		print_menu_generic_string(x + 60, y, fileNames[(BuGetWinScore(courseIndex) >> 16) & 0xFFFF]);
	}
}

/**
 * Prints stars collected in a score menu save file.
 */
static void print_score_file_star_score(s8 fileIndex, s16 courseIndex, s16 x, s16 y)
{
	s16 i = 0;
	unsigned char starScoreText[20]; // unknown length
	u8 stars     = BuGetStarFlag(fileIndex, courseIndex);
	s8 starCount = BuGetNumStars(fileIndex, courseIndex);
	// Don't count 100 coin star
	if(stars & (1 << 6))
	{
		starCount--;
	}
	// Add 1 star character for every star collected
	for(i = 0; i < starCount; i++)
	{
		starScoreText[i] = DIALOG_CHAR_STAR_FILLED;
	}
	// Terminating byte
	starScoreText[i] = DIALOG_CHAR_TERMINATOR;
	print_menu_generic_string(x, y, starScoreText);
}

#define MARIO_X 25
#define FILE_LETTER_X 95
#define LEVEL_NAME_X 29
#define SECRET_STARS_X 171
#define MYSCORE_X 238
#define HISCORE_X 231

/**
 * Prints save file score strings that shows when a save file is chosen inside the score menu.
 */
static void PrintFileScore(s8 fileIndex)
{
	// TODO: EU relocates level name table to translation segment 0x19
	unsigned char textMario[]      = {TEXT_MARIO};
	unsigned char textHiScore[]    = {TEXT_HI_SCORE};
	unsigned char textMyScore[]    = {TEXT_MY_SCORE};
	unsigned char textFileLetter[] = {TEXT_ZERO};
	void** levelNameTable	       = (void**)segmented_to_virtual(seg2_course_name_table);

	textFileLetter[0] = fileIndex + ASCII_TO_DIALOG('A'); // get letter of file selected
	// Print file name at top
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value);
	Draw16bitFont(HUD_LUT_DIFF, MARIO_X, 15, textMario);
	Draw16bitFont(HUD_LUT_GLOBAL, FILE_LETTER_X, 15, textFileLetter);
	// Print save file star count at top
	print_save_file_star_count(fileIndex, 124, 15);
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
	// Print course scores
	gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, alpha_value);

	//! Huge print list, for loops exist for a reason!
	//  PADCHAR is used to difference an x position value between
	//  JP and US when the course number is only one digit.
#define PADCHAR 1
#define PRINT_COURSE_SCORES(courseIndex, pad)                                                                                                                                                                                                                  \
	print_menu_generic_string(23 + (pad * 3), 23 + 12 * courseIndex, (const u8*)segmented_to_virtual(levelNameTable[courseIndex - 1]));                                                                                                                    \
	print_score_file_star_score(fileIndex, courseIndex - 1, 171, 23 + 12 * courseIndex);                                                                                                                                                                   \
	print_score_file_course_coin_score(fileIndex, courseIndex - 1, 213, 23 + 12 * courseIndex);

	// Course values are indexed, from Bob-omb Battlefield to Rainbow Ride
	PRINT_COURSE_SCORES(COURSE_BOB, PADCHAR) // BOB
	PRINT_COURSE_SCORES(COURSE_WF, PADCHAR)	 // WF
	PRINT_COURSE_SCORES(COURSE_JRB, PADCHAR) // JRB
	PRINT_COURSE_SCORES(COURSE_CCM, PADCHAR) // CCM
	PRINT_COURSE_SCORES(COURSE_BBH, PADCHAR) // BBH
	PRINT_COURSE_SCORES(COURSE_HMC, PADCHAR) // HMC
	PRINT_COURSE_SCORES(COURSE_LLL, PADCHAR) // LLL
	PRINT_COURSE_SCORES(COURSE_SSL, PADCHAR) // SSL
	PRINT_COURSE_SCORES(COURSE_DDD, PADCHAR) // DDD
	PRINT_COURSE_SCORES(COURSE_SL, 0)	 // SL
	PRINT_COURSE_SCORES(COURSE_WDW, 0)	 // WDW
	PRINT_COURSE_SCORES(COURSE_TTM, 0)	 // TTM
	PRINT_COURSE_SCORES(COURSE_THI, 0)	 // THI
	PRINT_COURSE_SCORES(COURSE_TTC, 0)	 // TTC
	PRINT_COURSE_SCORES(COURSE_RR, 0)	 // RR

#undef PRINT_COURSE_SCORES
#undef PADCHAR

	// Print level name
	print_menu_generic_string(LEVEL_NAME_X, 215, (const u8*)segmented_to_virtual(levelNameTable[25]));
	// Print castle secret stars
	print_score_file_castle_secret_stars(fileIndex, SECRET_STARS_X, 215);
	// Print current coin score mode
	if(sScoreFileCoinScoreMode == 0)
	{
		print_menu_generic_string(MYSCORE_X, 24, textMyScore);
	}
	else
	{
		print_menu_generic_string(HISCORE_X, 24, textHiScore);
	}

	gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_end);
}

/**
 * Prints file select strings depending on the menu selected.
 * Also checks if all saves exists and defines text and main menu timers.
 */
static void print_file_select_strings()
{
	UNUSED s32 unused1;
	UNUSED s32 unused2;

	MakeSelectProjection();
	switch(selectNo)
	{
		case MENU_BUTTON_NONE:
			PrintCourseSelectScene();
			break;
		case MENU_BUTTON_SCORE:
			PrintCourseScoreScene();
			sScoreFileCoinScoreMode = 0;
			break;
		case MENU_BUTTON_COPY:
			PrintFileCopyScene();
			break;
		case MENU_BUTTON_ERASE:
			PrintFileRemoveScene();
			break;
		case MENU_BUTTON_SCORE_FILE_A:
			PrintFileScore(SAVE_FILE_A);
			break;
		case MENU_BUTTON_SCORE_FILE_B:
			PrintFileScore(SAVE_FILE_B);
			break;
		case MENU_BUTTON_SCORE_FILE_C:
			PrintFileScore(SAVE_FILE_C);
			break;
		case MENU_BUTTON_SCORE_FILE_D:
			PrintFileScore(SAVE_FILE_D);
			break;
		case MENU_BUTTON_SOUND_MODE:
			PrintSoundSelect();
			break;
	}
	// If all 4 save file exists, define true to sAllFilesExist to prevent more copies in copy menu
	if(BuIsActive(SAVE_FILE_A) == TRUE && BuIsActive(SAVE_FILE_B) == TRUE && BuIsActive(SAVE_FILE_C) == TRUE && BuIsActive(SAVE_FILE_D) == TRUE)
	{
		sAllFilesExist = TRUE;
	}
	else
	{
		sAllFilesExist = FALSE;
	}
	// Timers for menu alpha text and the main menu itself
	if(alpha_value < 250)
	{
		alpha_value += 10 / FRAME_RATE_SCALER_INV;
	}
	if(click_counter < 1000 * FRAME_RATE_SCALER_INV)
	{
		click_counter += 1;
	}
}

/**
 * Geo function that prints file select strings and the cursor.
 */
Gfx* geo_file_select_strings_and_menu_cursor(s32 callContext, struct GraphNode* node, f32 mtx[4][4])
{
	if(callContext == GEO_CONTEXT_RENDER)
	{
		print_file_select_strings();
		print_menu_cursor();
	}
	return NULL;
}
