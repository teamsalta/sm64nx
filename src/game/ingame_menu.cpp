#include <ultra64.h>

#include "sm64.h"
#include "gfx_dimensions.h"
#include "memory.h"
#include "types.h"
#include "audio/external.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "game.h"
#include "save_file.h"
#include "level_update.h"
#include "camera.h"
#include "text_strings.h"
#include "segment2.h"
#include "segment7.h"
#include "eu_translation.h"
#include "ingame_menu.h"
#include "print.h"
#include "engine/math_util.h"
#include "actors/common1.h"
#include "levels/header.h"
#include "game/options.h"
#include <functional>
#include <string>
#include "player/players.h"
#include <algorithm>

extern Gfx* gDisplayListHead;
extern u32 frameCounter;
extern s16 activeCourseNo;
extern s16 activePlayerNo;

extern u8 main_menu_seg7_table_0700ABD0[];

namespace sm64::gfx
{
	void set_fullscreen(bool value);
}

u16 gDialogColorFadeTimer;
s8 gLastDialogLineNum;
s32 gDialogVariable;
u16 font_alpha;
s16 gCutsceneMsgXOffset;
s16 gCutsceneMsgYOffset;
s8 gRedCoinsCollected;

extern u8 gLastCompletedCourseNum;
extern u8 gLastCompletedStarNum;

void DrawPauseScore();

enum DialogBoxState
{
	DIALOG_STATE_OPENING,
	DIALOG_STATE_VERTICAL,
	DIALOG_STATE_HORIZONTAL,
	DIALOG_STATE_CLOSING
};

enum DialogBoxPageState
{
	DIALOG_PAGE_STATE_NONE,
	DIALOG_PAGE_STATE_SCROLL,
	DIALOG_PAGE_STATE_END
};

enum DialogBoxType
{
	DIALOG_TYPE_ROTATE, // used in NPCs and level messages
	DIALOG_TYPE_ZOOM    // used in signposts and wall signs and etc
};

enum DialogMark
{
	DIALOG_MARK_NONE       = 0,
	DIALOG_MARK_DAKUTEN    = 1,
	DIALOG_MARK_HANDAKUTEN = 2
};

#define DEFAULT_DIALOG_BOX_ANGLE 90.0f
#define DEFAULT_DIALOG_BOX_SCALE 19.0f

u8 gDialogCharWidths[256] = { // TODO: Is there a way to auto generate this?
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 5, 6, 6, 5, 8, 8, 6, 6, 6, 6, 6, 5, 6, 6, 8, 7, 6,  6, 6, 5, 5, 6, 5, 5, 6, 5, 4, 5, 5, 3, 7, 5, 5, 5, 6, 5, 5, 5, 5, 5, 7,  7, 5, 5,	 4, 4,
    8, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 7, 7, 6, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0,	 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0,	 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 5, 10, 5, 9, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 7, 7, 6, 6, 8, 0, 8, 10, 6, 4, 10, 0, 0};

s8 gDialogBoxState	  = DIALOG_STATE_OPENING;
f32 gDialogBoxOpenTimer	  = DEFAULT_DIALOG_BOX_ANGLE;
f32 gDialogBoxScale	  = DEFAULT_DIALOG_BOX_SCALE;
s16 gDialogScrollOffsetY  = 0;
s8 gDialogBoxType	  = DIALOG_TYPE_ROTATE;
s16 gDialogID		  = -1;
s16 gLastDialogPageStrPos = 0;
s16 gDialogTextPos	  = 0; // EU: D_802FD64C
s8 gDialogLineNum	  = 1;
s8 gCheatsLineNum	  = 1;
s8 gLastDialogResponse	  = 0;
u8 gMenuHoldKeyIndex	  = 0;
u8 gMenuHoldKeyTimer	  = 0;
s32 gDialogResponse	  = 0;

#include "drawfont.hpp"

void ContCursorEvent(s8 scrollDirection, s8* currentIndex, s8 minIndex, s8 maxIndex)
{
	u8 index = 0;

	if(scrollDirection == MENU_SCROLL_VERTICAL)
	{
		if(sm64::player(0).controller().rawStickY > 60)
		{
			index++;
		}

		if(sm64::player(0).controller().rawStickY < -60)
		{
			index += 2;
		}
	}
	else if(scrollDirection == MENU_SCROLL_HORIZONTAL)
	{
		if(sm64::player(0).controller().rawStickX > 60)
		{
			index += 2;
		}

		if(sm64::player(0).controller().rawStickX < -60)
		{
			index++;
		}
	}

	if(((index ^ gMenuHoldKeyIndex) & index) == 2)
	{
		if(currentIndex[0] == maxIndex)
		{
			//! Probably originally a >=, but later replaced with an == and an else statement.
			currentIndex[0] = maxIndex;
		}
		else
		{
			AudStartSound(SOUND_MENU_CHANGE_SELECT, gDefaultSoundArgs);
			currentIndex[0]++;
		}
	}

	if(((index ^ gMenuHoldKeyIndex) & index) == 1)
	{
		if(currentIndex[0] == minIndex)
		{
			// Same applies to here as above
		}
		else
		{
			AudStartSound(SOUND_MENU_CHANGE_SELECT, gDefaultSoundArgs);
			currentIndex[0]--;
		}
	}

	if(gMenuHoldKeyTimer == 10 * FRAME_RATE_SCALER_INV)
	{
		gMenuHoldKeyTimer = 8 * FRAME_RATE_SCALER_INV;
		gMenuHoldKeyIndex = 0;
	}
	else
	{
		gMenuHoldKeyTimer++;
		gMenuHoldKeyIndex = index;
	}

	if((index & 3) == 0)
	{
		gMenuHoldKeyTimer = 0;
	}
}

s16 CharCentering(s16 centerPos, u8* str, UNUSED f32 scale)
{
	s16 strPos	= 0;
	f32 spacesWidth = 0.0f;

	while(str[strPos] != DIALOG_CHAR_TERMINATOR)
	{
		spacesWidth += gDialogCharWidths[str[strPos]];
		strPos++;
	}
	// return the x position of where the string starts as half the string's
	// length from the position of the provided center.
	return (s16)(centerPos - (s16)(spacesWidth / 2.0));
}

s16 MessageLength(u8* str)
{
	s16 strPos = 0;
	s16 width  = 0;

	while(str[strPos] != DIALOG_CHAR_TERMINATOR)
	{
		width += gDialogCharWidths[str[strPos]];
		strPos++;
	}
	return width;
}

u8 gHudSymCoin[] = {GLYPH_COIN, GLYPH_SPACE};
u8 gHudSymX[]	 = {GLYPH_MULTIPLY, GLYPH_SPACE};

void DrawMyScore(s32 useCourseCoinScore, s8 fileNum, s8 courseNum, s16 x, s16 y)
{
	u8 strNumCoins[4];
	s16 numCoins;

	if(!useCourseCoinScore)
	{
		numCoins = (u16)(BuGetWinScore(courseNum) & 0xFFFF);
	}
	else
	{
		numCoins = BuGetNumCoins(fileNum, courseNum);
	}

	if(numCoins != 0)
	{
		Draw16bitFont(HUD_LUT_GLOBAL, x, y, gHudSymCoin);
		Draw16bitFont(HUD_LUT_GLOBAL, x + 16, y, gHudSymX);
		I_itochar(numCoins, strNumCoins);
		Draw16bitFont(HUD_LUT_GLOBAL, x + 32, y, strNumCoins);
	}
}

void DrawCourseStar(s8 fileNum, s8 courseNum, s16 x, s16 y)
{
	u8 strStarCount[4];
	s16 starCount;
	u8 textSymStar[] = {GLYPH_STAR, GLYPH_SPACE};
	UNUSED u16 unused;
	u8 textSymX[] = {GLYPH_MULTIPLY, GLYPH_SPACE};

	starCount = BuGetNumStars(fileNum, courseNum);

	if(starCount != 0)
	{
		Draw16bitFont(HUD_LUT_GLOBAL, x, y, textSymStar);
		Draw16bitFont(HUD_LUT_GLOBAL, x + 16, y, textSymX);
		I_itochar(starCount, strStarCount);
		Draw16bitFont(HUD_LUT_GLOBAL, x + 32, y, strStarCount);
	}
}

void I_itochar(s32 num, u8* dst)
{
	s32 digit1;
	s32 digit2;
	s32 digit3;

	s8 pos = 0;

	if(num > 999)
	{
		dst[0] = 0x00;
		dst[1] = DIALOG_CHAR_TERMINATOR;
		return;
	}

	digit1 = num / 100;
	digit2 = (num - digit1 * 100) / 10;
	digit3 = (num - digit1 * 100) - (digit2 * 10);

	if(digit1 != 0)
	{
		dst[pos++] = digit1;
	}

	if(digit2 != 0 || digit1 != 0)
	{
		dst[pos++] = digit2;
	}

	dst[pos++] = digit3;
	dst[pos]   = DIALOG_CHAR_TERMINATOR;
}

s16 GetMessageNo()
{
	return gDialogID;
}

void CallMessage(s16 dialog)
{
	if(gDialogID == -1)
	{
		gDialogID      = dialog;
		gDialogBoxType = DIALOG_TYPE_ROTATE;
	}
}

void CallMessageInt(s16 dialog, s32 dialogVar)
{
	if(gDialogID == -1)
	{
		gDialogID	= dialog;
		gDialogVariable = dialogVar;
		gDialogBoxType	= DIALOG_TYPE_ROTATE;
	}
}

void CallMessageFuda(s16 dialog)
{
	if(gDialogID == -1)
	{
		gDialogID      = dialog;
		gDialogBoxType = DIALOG_TYPE_ZOOM;
	}
}

void CallMessageYesNo(s16 dialog)
{
	if(gDialogID == -1)
	{
		gDialogID	    = dialog;
		gDialogBoxType	    = DIALOG_TYPE_ROTATE;
		gLastDialogResponse = 1;
	}
}

void MessageEnforcedDestroy()
{
	FreezeGame(0, 0);

	if(gDialogBoxType == DIALOG_TYPE_ZOOM)
	{
		ViewingCam(2);
	}

	gDialogBoxScale	      = 19.0f;
	gDialogBoxOpenTimer   = 90.0f;
	gDialogBoxState	      = DIALOG_STATE_OPENING;
	gDialogID	      = -1;
	gDialogTextPos	      = 0;
	gLastDialogResponse   = 0;
	gLastDialogPageStrPos = 0;
	gDialogResponse	      = 0;
}

#define X_VAL1 -7.0f
#define Y_VAL1 5.0
#define Y_VAL2 5.0f

void DrawMessageWindow(struct DialogEntry* dialog, s8 linesPerBox)
{
	iTranslate(MENU_MTX_NOPUSH, dialog->leftOffset, dialog->width, 0);

	switch(gDialogBoxType)
	{
		case DIALOG_TYPE_ROTATE: // Renders a dialog black box with zoom and rotation
			if(gDialogBoxState == DIALOG_STATE_OPENING || gDialogBoxState == DIALOG_STATE_CLOSING)
			{
				iScale(MENU_MTX_NOPUSH, 1.0 / gDialogBoxScale, 1.0 / gDialogBoxScale, 1.0f);
				// convert the speed into angle
				iRotate(MENU_MTX_NOPUSH, gDialogBoxOpenTimer * 4.0f, 0, 0, 1.0f);
			}
			gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 150);
			break;
		case DIALOG_TYPE_ZOOM: // Renders a dialog white box with zoom
			if(gDialogBoxState == DIALOG_STATE_OPENING || gDialogBoxState == DIALOG_STATE_CLOSING)
			{
				iTranslate(MENU_MTX_NOPUSH, 65.0 - (65.0 / gDialogBoxScale), (40.0 / gDialogBoxScale) - 40, 0);
				iScale(MENU_MTX_NOPUSH, 1.0 / gDialogBoxScale, 1.0 / gDialogBoxScale, 1.0f);
			}
			gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 150);
			break;
	}

	iTranslate(MENU_MTX_PUSH, X_VAL1, Y_VAL1, 0);
	iScale(MENU_MTX_NOPUSH, 1.1f, ((f32)linesPerBox / Y_VAL2) + 0.1, 1.0f);

	gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

void SetFontColor(s8 colorMode, s8 lineNum)
{
	u8 colorFade;

	if(colorMode == 1)
	{
		if(lineNum == 1)
		{
			gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
		}
		else
		{
			if(lineNum == gDialogLineNum)
			{
				colorFade = (gSineTable[(gDialogColorFadeTimer / FRAME_RATE_SCALER_INV) >> 4] * 50.0f) + 200.0f;
				gDPSetEnvColor(gDisplayListHead++, colorFade, colorFade, colorFade, 255);
			}
			else
			{
				gDPSetEnvColor(gDisplayListHead++, 200, 200, 200, 255);
			}
		}
	}
	else
	{
		switch(gDialogBoxType)
		{
			case DIALOG_TYPE_ROTATE:
				break;
			case DIALOG_TYPE_ZOOM:
				gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 255);
				break;
		}
	}
}

#define X_VAL3 0.0f
#define Y_VAL3 16

void KaigyouEvent(s8 lineNum, s8 totalLines, s8* pageState, s8* xMatrix, s16* linePos)
{
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

	if(lineNum == totalLines)
	{
		pageState[0] = DIALOG_PAGE_STATE_SCROLL;
		return;
	}

	iTranslate(MENU_MTX_PUSH, X_VAL3, 2 - (lineNum * Y_VAL3), 0);

	linePos[0] = 0;
	xMatrix[0] = 1;
}

void VariableEvent(s8* xMatrix, s16* linePos)
{
	s8 tensDigit = gDialogVariable / 10;
	s8 onesDigit = gDialogVariable - (tensDigit * 10); // remainder

	if(tensDigit != 0)
	{
		if(xMatrix[0] != 1)
		{
			iTranslate(MENU_MTX_NOPUSH, (f32)(gDialogCharWidths[DIALOG_CHAR_SPACE] * xMatrix[0]), 0, 0);
		}

		DrawMessageTexture(tensDigit);
		iTranslate(MENU_MTX_NOPUSH, (f32)gDialogCharWidths[tensDigit], 0, 0);
		xMatrix[0] = 1;
		linePos[0]++;
	}
	else
	{
	}

	if(xMatrix[0] != 1)
	{
		iTranslate(MENU_MTX_NOPUSH, (f32)(gDialogCharWidths[DIALOG_CHAR_SPACE] * (xMatrix[0] - 1)), 0, 0);
	}

	DrawMessageTexture(onesDigit);
	iTranslate(MENU_MTX_NOPUSH, (f32)gDialogCharWidths[onesDigit], 0, 0);

	linePos[0]++;
	xMatrix[0] = 1;
}

void DrawFontStrings(s8 multiTextId, s8 lineNum, s16* linePos, s8 linesPerBox, s8 xMatrix, s8 lowerBound)
{
	s8 i;
	struct MultiTextEntry textLengths[2] = {
	    {3, {TEXT_THE_RAW}},
	    {3, {TEXT_YOU_RAW}},
	};

	if(lineNum >= lowerBound && lineNum <= (lowerBound + linesPerBox))
	{
		if(linePos[0] != 0 || (xMatrix != 1))
		{
			iTranslate(MENU_MTX_NOPUSH, (gDialogCharWidths[DIALOG_CHAR_SPACE] * (xMatrix - 1)), 0, 0);
		}

		for(i = 0; i < textLengths[multiTextId].length; i++)
		{
			DrawMessageTexture(textLengths[multiTextId].str[i]);
			iTranslate(MENU_MTX_NOPUSH, (gDialogCharWidths[textLengths[multiTextId].str[i]]), 0, 0);
		}
	}

	linePos += textLengths[multiTextId].length;
}

u32 ZeroRe(s16 value)
{
	if(value < 0)
	{
		value = 0;
	}

	return value;
}

void DrawMessageFont(s8 colorMode, struct DialogEntry* dialog, s8 lowerBound)
{
	u8 strChar;

	u8* str	   = (u8*)segmented_to_virtual(dialog->str);
	s8 lineNum = 1;

	s8 totalLines;

	s8 pageState   = DIALOG_PAGE_STATE_NONE;
	UNUSED s8 mark = DIALOG_MARK_NONE; // size in US, EU
	s8 xMatrix     = 1;

	s8 linesPerBox = dialog->linesPerBox;

	s16 strIdx;
	s16 linePos = 0;

	if(gDialogBoxState == DIALOG_STATE_HORIZONTAL)
	{
		// If scrolling, consider the number of lines for both
		// the current page and the page being scrolled to.
		totalLines = linesPerBox * 2 + 1;
	}
	else
	{
		totalLines = linesPerBox + 1;
	}

	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	strIdx = gDialogTextPos;

	if(gDialogBoxState == DIALOG_STATE_HORIZONTAL)
	{
		iTranslate(MENU_MTX_NOPUSH, 0, (f32)gDialogScrollOffsetY, 0);
	}

	iTranslate(MENU_MTX_PUSH, X_VAL3, 2 - lineNum * Y_VAL3, 0);

	while(pageState == DIALOG_PAGE_STATE_NONE)
	{
		SetFontColor(colorMode, lineNum);
		strChar = str[strIdx];

		switch(strChar)
		{
			case DIALOG_CHAR_TERMINATOR:
				pageState = DIALOG_PAGE_STATE_END;
				gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
				break;
			case DIALOG_CHAR_NEWLINE:
				lineNum++;
				KaigyouEvent(lineNum, totalLines, &pageState, &xMatrix, &linePos);
				break;

			case DIALOG_CHAR_DAKUTEN:
				mark = DIALOG_MARK_DAKUTEN;
				break;
			case DIALOG_CHAR_PERIOD_OR_HANDAKUTEN:
				mark = DIALOG_MARK_HANDAKUTEN;
				break;
			case DIALOG_CHAR_SPACE:

				xMatrix++;

				linePos++;
				break;

			case DIALOG_CHAR_SLASH:
				xMatrix += 2;
				linePos += 2;
				break;
			case DIALOG_CHAR_MULTI_THE:
				DrawFontStrings(STRING_THE, lineNum, &linePos, linesPerBox, xMatrix, lowerBound);
				xMatrix = 1;
				break;
			case DIALOG_CHAR_MULTI_YOU:
				DrawFontStrings(STRING_YOU, lineNum, &linePos, linesPerBox, xMatrix, lowerBound);
				xMatrix = 1;
				break;
			case DIALOG_CHAR_STAR_COUNT:
				VariableEvent(&xMatrix, &linePos);
				break;
			default: // any other character

				if(lineNum >= lowerBound && lineNum <= lowerBound + linesPerBox)
				{
					if(linePos || xMatrix != 1)
					{
						iTranslate(MENU_MTX_NOPUSH, (f32)(gDialogCharWidths[DIALOG_CHAR_SPACE] * (xMatrix - 1)), 0, 0);
					}

					DrawMessageTexture(strChar);
					iTranslate(MENU_MTX_NOPUSH, (f32)(gDialogCharWidths[strChar]), 0, 0);
					xMatrix = 1;
					linePos++;
				}
		}

		strIdx++;
	}
	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);

	if(gDialogBoxState == DIALOG_STATE_VERTICAL)
	{
		if(pageState == DIALOG_PAGE_STATE_END)
		{
			gLastDialogPageStrPos = -1;
		}
		else
		{
			gLastDialogPageStrPos = strIdx;
		}
	}

	gLastDialogLineNum = lineNum;
}

#define X_VAL4_1 56
#define X_VAL4_2 47
#define Y_VAL4_1 2
#define Y_VAL4_2 16

void DrawCursor()
{
	if(gDialogBoxState == DIALOG_STATE_VERTICAL)
	{
		ContCursorEvent(MENU_SCROLL_HORIZONTAL, &gDialogLineNum, 1, 2);
	}

	iTranslate(MENU_MTX_NOPUSH, (gDialogLineNum * X_VAL4_1) - X_VAL4_2, Y_VAL4_1 - (gLastDialogLineNum * Y_VAL4_2), 0);

	if(gDialogBoxType == DIALOG_TYPE_ROTATE)
	{
		gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
	}
	else
	{
		gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 255);
	}

	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
}

#define X_VAL5 118.0f
#define Y_VAL5_1 -16
#define Y_VAL5_2 5
#define X_Y_VAL6 0.8f

void DrawOkuri(s8 linesPerBox)
{
	s32 timer = frameCounter;

	if(timer & (0x08 * FRAME_RATE_SCALER_INV))
	{
		return;
	}

	iTranslate(MENU_MTX_PUSH, X_VAL5, (linesPerBox * Y_VAL5_1) + Y_VAL5_2, 0);
	iScale(MENU_MTX_NOPUSH, X_Y_VAL6, X_Y_VAL6, 1.0f);
	iRotate(MENU_MTX_NOPUSH, -DEFAULT_DIALOG_BOX_ANGLE, 0, 0, 1.0f);

	if(gDialogBoxType == DIALOG_TYPE_ROTATE)
	{ // White Text
		gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
	}
	else
	{ // Black Text
		gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 255);
	}

	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

void MessageSpecialSound(s16 dialogID)
{ // dialog ID tables, in order
	// King Bob-omb (Start), Whomp (Start), King Bob-omb (throw him out), Eyerock (Start), Wiggler (Start)
	s16 dialogBossStart[] = {17, 114, 128, 117, 150};
	// Koopa the Quick (BOB), Koopa the Quick (THI), Penguin Race, Fat Penguin Race (120 stars)
	s16 dialogRaceSound[] = {5, 9, 55, 164};
	// Red Switch, Green Switch, Blue Switch, 100 coins star, Bowser Red Coin Star
	s16 dialogStarSound[] = {10, 11, 12, 13, 14};
	// King Bob-omb (Start), Whomp (Defeated), King Bob-omb (Defeated, missing in JP), Eyerock (Defeated), Wiggler (Defeated)
#if BUGFIX_KING_BOB_OMB_FADE_MUSIC
	s16 dialogBossStop[] = {17, 115, 116, 118, 152};
#else
	//! @bug JP misses King Bob-omb defeated dialog "116", meaning that the boss music will still
	//! play after King Bob-omb is defeated until BOB loads it's music after the star cutscene
	s16 dialogBossStop[] = {17, 115, 118, 152};
#endif
	s16 i;

	for(i = 0; i < (s16)ARRAY_COUNT(dialogBossStart); i++)
	{
		if(dialogBossStart[i] == dialogID)
		{
			Na_SeqVolRecover(0, 60);
			Na_MusicStart(0, SEQUENCE_ARGS(4, SEQ_EVENT_BOSS), 0);
			return;
		}
	}

	for(i = 0; i < (s16)ARRAY_COUNT(dialogRaceSound); i++)
	{
		if(dialogRaceSound[i] == dialogID && gDialogLineNum == 1)
		{
			Na_RaceFanfareBgm();
			return;
		}
	}

	for(i = 0; i < (s16)ARRAY_COUNT(dialogStarSound); i++)
	{
		if(dialogStarSound[i] == dialogID && gDialogLineNum == 1)
		{
			AudStartSound(SOUND_MENU_STAR_SOUND, gDefaultSoundArgs);
			return;
		}
	}

	for(i = 0; i < (s16)ARRAY_COUNT(dialogBossStop); i++)
	{
		if(dialogBossStop[i] == dialogID)
		{
			Na_BossBgmStop(0, 1);
			return;
		}
	}
}

s16 gMenuMode = -1;

u8 gEndCutsceneStrEn0[] = {TEXT_FILE_MARIO_EXCLAMATION};
u8 gEndCutsceneStrEn1[] = {TEXT_POWER_STARS_RESTORED};
u8 gEndCutsceneStrEn2[] = {TEXT_THANKS_TO_YOU};
u8 gEndCutsceneStrEn3[] = {TEXT_THANK_YOU_MARIO};
u8 gEndCutsceneStrEn4[] = {TEXT_SOMETHING_SPECIAL};
u8 gEndCutsceneStrEn5[] = {TEXT_LISTEN_EVERYBODY};
u8 gEndCutsceneStrEn6[] = {TEXT_LETS_HAVE_CAKE};
u8 gEndCutsceneStrEn7[] = {TEXT_FOR_MARIO};
u8 gEndCutsceneStrEn8[] = {TEXT_FILE_MARIO_QUESTION};

u8* gEndCutsceneStringsEn[] = {
    gEndCutsceneStrEn0, gEndCutsceneStrEn1, gEndCutsceneStrEn2, gEndCutsceneStrEn3, gEndCutsceneStrEn4, gEndCutsceneStrEn5, gEndCutsceneStrEn6, gEndCutsceneStrEn7,
    // This [8] string is actually size. In the cutscene handler, the developers do not
    // set the 8th one, but use the first string again at the very end, so Peach ends up
    // saying "Mario!" twice. It is likely that she was originally meant to say "Mario?" at
    // the end but the developers changed their mind, possibly because the line recorded
    // sounded more like an exclamation than a question.
    gEndCutsceneStrEn8, NULL};

u16 gCutsceneMsgFade	   = 0;
s16 gCutsceneMsgIndex	   = -1;
s16 gCutsceneMsgDuration   = -1;
s16 gCutsceneMsgTimer	   = 0;
s8 gDialogCameraAngleIndex = CAM_SELECTION_MARIO;
s8 gDialogCourseActNum	   = 1;

#define DIAG_VAL1 16
#define DIAG_VAL3 132 // US & EU
#define DIAG_VAL4 5
#define DIAG_VAL2 240 // JP & US

void MessageOnlyEvent()
{
	void** dialogTable;
	struct DialogEntry* dialog;
	s8 lowerBound;

	dialogTable = (void**)segmented_to_virtual(seg2_dialog_table);
	dialog	    = (DialogEntry*)segmented_to_virtual(dialogTable[gDialogID]);

	// if the dialog entry is invalid, set the ID to -1.
	if(segmented_to_virtual(NULL) == dialog)
	{
		gDialogID = -1;
		return;
	}

	switch(gDialogBoxState)
	{
		case DIALOG_STATE_OPENING:
			if(gDialogBoxOpenTimer == DEFAULT_DIALOG_BOX_ANGLE)
			{
				Na_MessageSound(gDialogID);
				AudStartSound(SOUND_MENU_MESSAGE_APPEAR, gDefaultSoundArgs);
			}

			if(gDialogBoxType == DIALOG_TYPE_ROTATE)
			{
				gDialogBoxOpenTimer -= 7.5 * FRAME_RATE_SCALER;
				gDialogBoxScale -= 1.5 * FRAME_RATE_SCALER;
			}
			else
			{
				gDialogBoxOpenTimer -= 10.0 * FRAME_RATE_SCALER;
				gDialogBoxScale -= 2.0 * FRAME_RATE_SCALER;
			}

			if(gDialogBoxOpenTimer == 0.0f)
			{
				gDialogBoxState = DIALOG_STATE_VERTICAL;
				gDialogLineNum	= 1;
			}

			lowerBound = 1;
			break;
		case DIALOG_STATE_VERTICAL:
			gDialogBoxOpenTimer = 0.0f;

			if((sm64::player(0).controller().buttonPressed & sm64::hid::A_BUTTON) || (sm64::player(0).controller().buttonPressed & sm64::hid::B_BUTTON))
			{
				if(gLastDialogPageStrPos == -1)
				{
					MessageSpecialSound(gDialogID);
					gDialogBoxState = DIALOG_STATE_CLOSING;
				}
				else
				{
					gDialogBoxState = DIALOG_STATE_HORIZONTAL;
					AudStartSound(SOUND_MENU_MESSAGE_NEXT_PAGE, gDefaultSoundArgs);
				}
			}
			lowerBound = 1;

			break;
		case DIALOG_STATE_HORIZONTAL:
			gDialogScrollOffsetY += dialog->linesPerBox * 2 / FRAME_RATE_SCALER_INV;

			if(gDialogScrollOffsetY >= dialog->linesPerBox * DIAG_VAL1)
			{
				gDialogTextPos	     = gLastDialogPageStrPos;
				gDialogBoxState	     = DIALOG_STATE_VERTICAL;
				gDialogScrollOffsetY = 0;
			}

			lowerBound = (gDialogScrollOffsetY / 16) + 1;

			break;
		case DIALOG_STATE_CLOSING:
			if(gDialogBoxOpenTimer == 20.0f)
			{
				FreezeGame(0, 0);
				AudStartSound(SOUND_MENU_MESSAGE_DISAPPEAR, gDefaultSoundArgs);

				if(gDialogBoxType == DIALOG_TYPE_ZOOM)
				{
					ViewingCam(2);
				}

				gDialogResponse = gDialogLineNum;
			}

			gDialogBoxOpenTimer += +10.0f * FRAME_RATE_SCALER;
			gDialogBoxScale += 2.0f * FRAME_RATE_SCALER;

			if(gDialogBoxOpenTimer == DEFAULT_DIALOG_BOX_ANGLE)
			{
				gDialogBoxState	      = DIALOG_STATE_OPENING;
				gDialogID	      = -1;
				gDialogTextPos	      = 0;
				gLastDialogResponse   = 0;
				gLastDialogPageStrPos = 0;
				gDialogResponse	      = 0;
			}

			lowerBound = 1;
			break;
	}

	DrawMessageWindow(dialog, dialog->linesPerBox);

	gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, ZeroRe(DIAG_VAL2 - dialog->width), SCREEN_WIDTH, ZeroRe(240 + ((dialog->linesPerBox * 80) / DIAG_VAL4) - dialog->width));

	DrawMessageFont(0, dialog, lowerBound);

	if(gLastDialogPageStrPos == -1 && gLastDialogResponse == 1)
	{
		DrawCursor();
	}

	gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 2, 2, SCREEN_WIDTH - BORDER_HEIGHT / 2, SCREEN_HEIGHT - BORDER_HEIGHT / 2);

	if(gLastDialogPageStrPos != -1 && gDialogBoxState == DIALOG_STATE_VERTICAL)
	{
		DrawOkuri(dialog->linesPerBox);
	}
}

// Calls a gMenuMode value defined by MessageEvent cases
void CallSelectMessage(s16 mode)
{
	if(gMenuMode == -1)
	{
		gMenuMode = mode;
	}
}

#include "endingmess.hpp"
#include "pausemess.hpp"

// Only case 1 and 2 are used
s16 MessageEvent()
{
	s16 mode = 0;

	MakeSelectProjection();

	if(gMenuMode != -1)
	{
		switch(gMenuMode)
		{
			case 0:
				mode = PauseSceneEvent();
				break;
			case 1:
				mode = PauseSceneEvent();
				break;
			case 2:
				mode = SaveSceneEvent();
				break;
			case 3:
				mode = SaveSceneEvent();
				break;
		}

		gDialogColorFadeTimer += 0x1000 * FRAME_RATE_SCALER_INV;
	}
	else if(gDialogID != -1)
	{
		// Peach message "Dear Mario" new game dialog
		if(gDialogID == 20)
		{
			OpeningMessageEvent(); // the peach message needs to be
					       // repositioned seperately
			return mode;
		}

		MessageOnlyEvent();
		gDialogColorFadeTimer += 0x1000 * FRAME_RATE_SCALER_INV;
	}
	return mode;
}
