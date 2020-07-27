#ifndef INGAME_MENU_H
#define INGAME_MENU_H

#include "types.h"

#define ASCII_TO_DIALOG(asc) (((asc) >= '0' && (asc) <= '9') ? ((asc) - '0') : ((asc) >= 'A' && (asc) <= 'Z') ? ((asc) - 'A' + 0x0A) : ((asc) >= 'a' && (asc) <= 'z') ? ((asc) - 'a' + 0x24) : 0x00)

#define MENU_MTX_PUSH 1
#define MENU_MTX_NOPUSH 2

#define MENU_SCROLL_VERTICAL 1
#define MENU_SCROLL_HORIZONTAL 2

// Japanese File Select uses an unique table
// to print specific Japanese HUD chars
#define HUD_LUT_JPMENU 1
#define HUD_LUT_GLOBAL 2

// For file select JP HUD difference
#define HUD_LUT_DIFF HUD_LUT_GLOBAL

#define RENDER_PAUSE_SCREEN 1
#define RENDER_COURSE_DONE_SCREEN 2

extern s8 gDialogCourseActNum;
extern s8 getStar_flag;

struct DialogEntry
{
	/*0x00*/ u32 unused;
	/*0x04*/ s8 linesPerBox;
	/*0x06*/ s16 leftOffset;
	/*0x08*/ s16 width;
	/*0x0C*/ const u8* str;
};

// EU only
enum HudSpecialHUDChars
{
	HUD_CHAR_A_UMLAUT = 0x3A,
	HUD_CHAR_O_UMLAUT = 0x3B,
	HUD_CHAR_U_UMLAUT = 0x3C
};

enum SpecialFontChars
{
	GLOBAL_CHAR_SPACE      = 0x9E,
	GLOBAR_CHAR_TERMINATOR = 0xFF
};

// definitions for some of the special characters defined in charmap.txt
enum DialogSpecialChars
{
	DIALOG_CHAR_SLASH		 = 0xD0,
	DIALOG_CHAR_MULTI_THE		 = 0xD1, // 'the'
	DIALOG_CHAR_MULTI_YOU		 = 0xD2, // 'you'
	DIALOG_CHAR_PERIOD		 = 0x6E,
	DIALOG_CHAR_COMMA		 = 0x6F,
	DIALOG_CHAR_SPACE		 = 0x9E,
	DIALOG_CHAR_STAR_COUNT		 = 0xE0, // number of stars
	DIALOG_CHAR_UMLAUT		 = 0xE9,
	DIALOG_CHAR_DAKUTEN		 = 0xF0,
	DIALOG_CHAR_PERIOD_OR_HANDAKUTEN = 0xF1,
	DIALOG_CHAR_STAR_FILLED		 = 0xFA,
	DIALOG_CHAR_STAR_OPEN		 = 0xFD,
	DIALOG_CHAR_NEWLINE		 = 0xFE,
	DIALOG_CHAR_TERMINATOR		 = 0xFF
};

extern s32 gDialogResponse;
extern u16 gDialogColorFadeTimer;
extern s8 gLastDialogLineNum;
extern s32 gDialogVariable;
extern u16 font_alpha;
extern s16 gCutsceneMsgXOffset;
extern s16 gCutsceneMsgYOffset;
extern s8 gRedCoinsCollected;

extern void iwa_LoadImatrix(void);
extern void iTranslate(s8 pushOp, f32 x, f32 y, f32 z);
extern void MakeSelectProjection(void);
extern void Draw8bitFont(s16 x, s16 y, const u8* str);
extern void Draw16bitFont(s8 fontLut, s16 x, s16 y, const u8* str);
extern void print_menu_generic_string(s16 x, s16 y, const u8* str);
extern void ContCursorEvent(s8 scrollDirection, s8* currentIndex, s8 minIndex, s8 maxIndex);
extern s16 CharCentering(s16 centerPos, u8* str, f32 scale);
extern void DrawMyScore(s32 useCourseCoinScore, s8 fileNum, s8 courseNum, s16 x, s16 y);
extern void I_itochar(s32 num, u8* dst);
extern s16 GetMessageNo(void);
extern void CallMessage(s16 dialog);
extern void CallMessageInt(s16 dialog, s32 dialogVar);
extern void CallMessageFuda(s16 dialog);
extern void CallMessageYesNo(s16 dialog);
extern void MessageEnforcedDestroy(void);
extern void CallSelectMessage(s16 mode);
extern void ClearEndingMessage(void);
extern void BeginEndingMessage(void);
extern void EndEndingMessage(void);
extern void EndingMessage(s16 x, s16 y, const char* str);
extern void CallEndingDemoMessage(s16 xOffset, s16 yOffset, s16 msgIndex, Duration msgDuration);
extern void EndingDemoMessageEvent(void);
extern void CannonSightEvent(void);
extern void iwaStageInit(void);
extern s16 MessageEvent(void);

#endif /* INGAME_MENU_H */
