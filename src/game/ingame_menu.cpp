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
extern u32 gGlobalTimer;
extern s16 gCurrCourseNum;
extern s16 gCurrSaveFileNum;

extern u8 main_menu_seg7_table_0700ABD0[];

namespace sm64::gfx
{
	void set_fullscreen(bool value);
}

u16 gDialogColorFadeTimer;
s8 gLastDialogLineNum;
s32 gDialogVariable;
u16 gDialogTextAlpha;
s16 gCutsceneMsgXOffset;
s16 gCutsceneMsgYOffset;
s8 gRedCoinsCollected;

extern u8 gLastCompletedCourseNum;
extern u8 gLastCompletedStarNum;

void draw_pause_nav();

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

#ifndef VERSION_JP
u8 gDialogCharWidths[256] = { // TODO: Is there a way to auto generate this?
    7, 7, 7,  7, 7, 7, 7, 7, 7, 7, 6,  6, 6, 6,	 6, 6, 6, 6, 5, 6, 6, 5, 8, 8, 6, 6, 6, 6, 6, 5, 6, 6, 8, 7, 6, 6, 6, 5, 5, 6, 5, 5, 6, 5, 4, 5, 5, 3, 7, 5, 5, 5, 6, 5, 5, 5, 5, 5, 7, 7, 5, 5, 4, 4, 8, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    8, 8, 8,  8, 7, 7, 6, 7, 7, 0, 0,  0, 0, 0,	 0, 0,
#ifdef VERSION_EU
    6, 6, 6,  0, 6, 6, 6, 0, 0, 0, 0,  0, 0, 0,	 0, 4, 5, 5, 5, 5, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 5, 6, 0, 4, 4, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#else
    0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0,	 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
    0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0,	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#ifdef VERSION_EU
    7, 5, 10, 5, 9, 8, 4, 0, 0, 0, 0,  5, 5, 6,	 5, 0,
#else
    7, 5, 10, 5, 9, 8, 4, 0, 0, 0, 0,  0, 0, 0,	 0, 0,
#endif
    0, 0, 5,  7, 7, 6, 6, 8, 0, 8, 10, 6, 4, 10, 0, 0};
#endif

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

void create_dl_identity_matrix(void)
{
	Mtx* matrix = (Mtx*)alloc_display_list(sizeof(Mtx));

	if(matrix == NULL)
	{
		return;
	}

	guMtxIdent(matrix);

	gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
	gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
}

void create_dl_translation_matrix(s8 pushOp, f32 x, f32 y, f32 z)
{
	Mtx* matrix = (Mtx*)alloc_display_list(sizeof(Mtx));

	if(matrix == NULL)
	{
		return;
	}

	guTranslate(matrix, x, y, z);

	if (pushOp == MENU_MTX_PUSH)
	{
		gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
	}
	else if (pushOp == MENU_MTX_NOPUSH)
	{
		gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
	}
}

void create_dl_rotation_matrix(s8 pushOp, f32 a, f32 x, f32 y, f32 z)
{
	Mtx* matrix = (Mtx*)alloc_display_list(sizeof(Mtx));

	if(matrix == NULL)
	{
		return;
	}

	guRotate(matrix, a, x, y, z);

	if(pushOp == MENU_MTX_PUSH)
		gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);

	if(pushOp == MENU_MTX_NOPUSH)
		gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
}

void create_dl_scale_matrix(s8 pushOp, f32 x, f32 y, f32 z)
{
	Mtx* matrix = (Mtx*)alloc_display_list(sizeof(Mtx));

	if(matrix == NULL)
	{
		return;
	}

	guScale(matrix, x, y, z);

	if(pushOp == MENU_MTX_PUSH)
		gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);

	if(pushOp == MENU_MTX_NOPUSH)
		gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
}

void create_dl_ortho_matrix(void)
{
	Mtx* matrix = (Mtx*)alloc_display_list(sizeof(Mtx));

	if(matrix == NULL)
	{
		return;
	}

	create_dl_identity_matrix();

	guOrtho(matrix, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, -10.0f, 10.0f, 1.0f);

	// Should produce G_RDPHALF_1 in Fast3D
	gSPPerspNormalize((Gfx*)(gDisplayListHead++), 0xFFFF);

	gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH)
}

static u8* alloc_ia8_text_from_i1(u16* in, s16 width, s16 height)
{
	s32 inPos;
	u16 bitMask;
	u8* out;
	s16 outPos;

	outPos = 0;
	out    = (u8*)alloc_display_list(width * height);

	if(out == NULL)
	{
		return NULL;
	}

	for(inPos = 0; inPos < (width * height) / 16; inPos++)
	{
		bitMask = 0x8000;

		while(bitMask != 0)
		{
			if(in[inPos] & bitMask)
			{
				out[outPos] = 0xFF;
			}
			else
			{
				out[outPos] = 0x00;
			}

			bitMask /= 2;
			outPos++;
		}
	}

	return out;
}

void render_generic_char(u8 c)
{
	void** fontLUT;
	void* packedTexture;
#ifdef VERSION_JP
	void* unpackedTexture;
#endif

	fontLUT	      = (void**)segmented_to_virtual(main_font_lut);
	packedTexture = segmented_to_virtual(fontLUT[c]);

#ifdef VERSION_JP
	unpackedTexture = alloc_ia8_text_from_i1(packedTexture, 8, 16);

	gDPPipeSync(gDisplayListHead++);
	gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_8b, 1, VIRTUAL_TO_PHYSICAL(unpackedTexture));
#else
#ifdef VERSION_US
	gDPPipeSync(gDisplayListHead++);
#endif
	gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, VIRTUAL_TO_PHYSICAL(packedTexture));
#endif
	gSPDisplayList(gDisplayListHead++, dl_ia_text_tex_settings);
#ifdef VERSION_EU
	gSPTextureRectangleFlip(gDisplayListHead++, gDialogX << 2, (gDialogY - 16) << 2, (gDialogX + 8) << 2, gDialogY << 2, G_TX_RENDERTILE, 16 << 5, 8 << 5, 1 << 10, 1 << 10);
#endif
}

#ifdef VERSION_EU
u8* alloc_ia4_tex_from_i1(u8* in, s16 width, s16 height)
{
	u32 size = (u32)width * (u32)height;
	u8* out;
	s32 inPos;
	s16 outPos;
	u8 bitMask;

	outPos = 0;
	out    = (u8*)alloc_display_list(size);

	if(out == NULL)
	{
		return NULL;
	}

	for(inPos = 0; inPos < (width * height) / 4; inPos++)
	{
		bitMask = 0x80;
		while(bitMask != 0)
		{
			out[outPos] = (in[inPos] & bitMask) ? 0xF0 : 0x00;
			bitMask /= 2;
			out[outPos] = (in[inPos] & bitMask) ? out[outPos] + 0x0F : out[outPos];
			bitMask /= 2;
			outPos++;
		}
	}

	return out;
}

void render_generic_char_at_pos(s16 xPos, s16 yPos, u8 c)
{
	void** fontLUT;
	void* packedTexture;
	void* unpackedTexture;

	fontLUT		= segmented_to_virtual(main_font_lut);
	packedTexture	= segmented_to_virtual(fontLUT[c]);
	unpackedTexture = alloc_ia4_tex_from_i1(packedTexture, 8, 8);

	gDPPipeSync(gDisplayListHead++);
	gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, VIRTUAL_TO_PHYSICAL(unpackedTexture));
	gSPDisplayList(gDisplayListHead++, dl_ia_text_tex_settings);
	gSPTextureRectangleFlip(gDisplayListHead++, xPos << 2, (yPos - 16) << 2, (xPos + 8) << 2, yPos << 2, G_TX_RENDERTILE, 16 << 5, 8 << 5, 1 << 10, 1 << 10);
}

void render_lowercase_diacritic(s16* xPos, s16* yPos, u8 letter, u8 diacritic)
{
	render_generic_char_at_pos(*xPos, *yPos, letter);
	render_generic_char_at_pos(*xPos, *yPos, diacritic + 0xE7);
	*xPos += gDialogCharWidths[letter];
}

void render_uppercase_diacritic(s16* xPos, s16* yPos, u8 letter, u8 diacritic)
{
	render_generic_char_at_pos(*xPos, *yPos, letter);
	render_generic_char_at_pos(*xPos, *yPos - 4, diacritic + 0xE3);
	*xPos += gDialogCharWidths[letter];
}
#endif // VERSION_EU

#ifndef VERSION_JP
struct MultiTextEntry
{
	u8 length;
	u8 str[4];
};

#define TEXT_THE_RAW ASCII_TO_DIALOG('t'), ASCII_TO_DIALOG('h'), ASCII_TO_DIALOG('e'), 0x00
#define TEXT_YOU_RAW ASCII_TO_DIALOG('y'), ASCII_TO_DIALOG('o'), ASCII_TO_DIALOG('u'), 0x00

enum MultiStringIDs
{
	STRING_THE,
	STRING_YOU
};

/*
 * Place the multi-text string according to the ID passed. (US, EU)
 * 0: 'the'
 * 1: 'you'
 */
#ifdef VERSION_US
void render_multi_text_string(s8 multiTextID) // US: 802D76C8
#elif defined(VERSION_EU)
void render_multi_text_string(s16* xPos, s16* yPos, s8 multiTextID) // EU: 802AD650
#endif
{
	s8 i;
	struct MultiTextEntry textLengths[2] = {
	    {3, {TEXT_THE_RAW}},
	    {3, {TEXT_YOU_RAW}},
	};

	for(i = 0; i < textLengths[multiTextID].length; i++)
	{
#ifdef VERSION_US
		render_generic_char(textLengths[multiTextID].str[i]);
		create_dl_translation_matrix(MENU_MTX_NOPUSH, (f32)(gDialogCharWidths[textLengths[multiTextID].str[i]]), 0.0f, 0.0f);
#elif defined(VERSION_EU)
		render_generic_char_at_pos(*xPos, *yPos, textLengths[multiTextID].str[i]);
		*xPos += gDialogCharWidths[textLengths[multiTextID].str[i]];
#endif
	}
}
#endif

#ifdef VERSION_JP
#define MAX_STRING_WIDTH 18
#else
#define MAX_STRING_WIDTH 16
#endif

/**
 * Prints a generic white string.
 * In JP/EU a IA1 texture is used but in US a IA4 texture is used.
 */
void print_generic_string(s16 x, s16 y, const u8* str)
{
	UNUSED s8 mark = DIALOG_MARK_NONE; // unused in EU
	s32 strPos     = 0;
	u8 lineNum     = 1;
#ifdef VERSION_EU
	s16 xCoord = x;
	s16 yCoord = 240 - y;
#endif

#ifndef VERSION_EU
	create_dl_translation_matrix(MENU_MTX_PUSH, x, y, 0.0f);
#endif

	while(str[strPos] != DIALOG_CHAR_TERMINATOR)
	{
		switch(str[strPos])
		{
#ifdef VERSION_EU
			case DIALOG_CHAR_SPACE:
				xCoord += 5;
				break;
			case DIALOG_CHAR_NEWLINE:
				yCoord += 16;
				xCoord = x;
				lineNum++;
				break;
			case DIALOG_CHAR_LOWER_A_GRAVE:
			case DIALOG_CHAR_LOWER_A_CIRCUMFLEX:
			case DIALOG_CHAR_LOWER_A_UMLAUT:
				render_lowercase_diacritic(&xCoord, &yCoord, ASCII_TO_DIALOG('a'), str[strPos] & 0xF);
				break;
			case DIALOG_CHAR_UPPER_A_UMLAUT: // @bug grave and circumflux (0x64-0x65) are absent here
				render_uppercase_diacritic(&xCoord, &yCoord, ASCII_TO_DIALOG('A'), str[strPos] & 0xF);
				break;
			case DIALOG_CHAR_LOWER_E_GRAVE:
			case DIALOG_CHAR_LOWER_E_CIRCUMFLEX:
			case DIALOG_CHAR_LOWER_E_UMLAUT:
			case DIALOG_CHAR_LOWER_E_ACUTE:
				render_lowercase_diacritic(&xCoord, &yCoord, ASCII_TO_DIALOG('e'), str[strPos] & 0xF);
				break;
			case DIALOG_CHAR_UPPER_E_GRAVE:
			case DIALOG_CHAR_UPPER_E_CIRCUMFLEX:
			case DIALOG_CHAR_UPPER_E_UMLAUT:
			case DIALOG_CHAR_UPPER_E_ACUTE:
				render_uppercase_diacritic(&xCoord, &yCoord, ASCII_TO_DIALOG('E'), str[strPos] & 0xF);
				break;
			case DIALOG_CHAR_LOWER_U_GRAVE:
			case DIALOG_CHAR_LOWER_U_CIRCUMFLEX:
			case DIALOG_CHAR_LOWER_U_UMLAUT:
				render_lowercase_diacritic(&xCoord, &yCoord, ASCII_TO_DIALOG('u'), str[strPos] & 0xF);
				break;
			case DIALOG_CHAR_UPPER_U_UMLAUT: // @bug grave and circumflex (0x84-0x85) are absent here
				render_uppercase_diacritic(&xCoord, &yCoord, ASCII_TO_DIALOG('U'), str[strPos] & 0xF);
				break;
			case DIALOG_CHAR_LOWER_O_CIRCUMFLEX:
			case DIALOG_CHAR_LOWER_O_UMLAUT:
				render_lowercase_diacritic(&xCoord, &yCoord, ASCII_TO_DIALOG('o'), str[strPos] & 0xF);
				break;
			case DIALOG_CHAR_UPPER_O_UMLAUT: // @bug circumflex (0x95) is absent here
				render_uppercase_diacritic(&xCoord, &yCoord, ASCII_TO_DIALOG('O'), str[strPos] & 0xF);
				break;
			case DIALOG_CHAR_LOWER_I_CIRCUMFLEX:
			case DIALOG_CHAR_LOWER_I_UMLAUT:
				render_lowercase_diacritic(&xCoord, &yCoord, DIALOG_CHAR_I_NO_DIA, str[strPos] & 0xF);
				break;
#else // i.e. not EU
			case DIALOG_CHAR_DAKUTEN:
				mark = DIALOG_MARK_DAKUTEN;
				break;
			case DIALOG_CHAR_PERIOD_OR_HANDAKUTEN:
				mark = DIALOG_MARK_HANDAKUTEN;
				break;
			case DIALOG_CHAR_NEWLINE:
				gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
				create_dl_translation_matrix(MENU_MTX_PUSH, x, y - (lineNum * MAX_STRING_WIDTH), 0.0f);
				lineNum++;
				break;
			case DIALOG_CHAR_PERIOD:
				create_dl_translation_matrix(MENU_MTX_PUSH, -2.0f, -5.0f, 0.0f);
				render_generic_char(DIALOG_CHAR_PERIOD_OR_HANDAKUTEN);
				gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
				break;
#endif
#ifndef VERSION_JP
			case DIALOG_CHAR_SLASH:
#ifdef VERSION_US
				create_dl_translation_matrix(MENU_MTX_NOPUSH, (f32)(gDialogCharWidths[DIALOG_CHAR_SPACE] * 2), 0.0f, 0.0f);
#elif defined(VERSION_EU)
				xCoord += gDialogCharWidths[DIALOG_CHAR_SPACE] * 2;
#endif
				break;
			case DIALOG_CHAR_MULTI_THE:
#ifdef VERSION_EU
				render_multi_text_string(&xCoord, &yCoord, STRING_THE);
#else
				render_multi_text_string(STRING_THE);
#endif
				break;
			case DIALOG_CHAR_MULTI_YOU:
#ifdef VERSION_EU
				render_multi_text_string(&xCoord, &yCoord, STRING_YOU);
#else
				render_multi_text_string(STRING_YOU);
#endif
				break;
#endif
#ifndef VERSION_EU
			case DIALOG_CHAR_SPACE:
#ifdef VERSION_JP
				create_dl_translation_matrix(MENU_MTX_NOPUSH, 5.0f, 0.0f, 0.0f);
				break;
#else
				create_dl_translation_matrix(MENU_MTX_NOPUSH, (f32)(gDialogCharWidths[DIALOG_CHAR_SPACE]), 0.0f, 0.0f);
#endif
#endif
				break; // ? needed to match
			default:
#ifdef VERSION_EU
				render_generic_char_at_pos(xCoord, yCoord, str[strPos]);
				xCoord += gDialogCharWidths[str[strPos]];
				break;
#else
				render_generic_char(str[strPos]);
				if(mark != DIALOG_MARK_NONE)
				{
					create_dl_translation_matrix(MENU_MTX_PUSH, 5.0f, 5.0f, 0.0f);
					render_generic_char(mark + 0xEF);
					gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
					mark = DIALOG_MARK_NONE;
				}

#ifdef VERSION_JP
				create_dl_translation_matrix(MENU_MTX_NOPUSH, 10.0f, 0.0f, 0.0f);
#else
				create_dl_translation_matrix(MENU_MTX_NOPUSH, (f32)(gDialogCharWidths[str[strPos]]), 0.0f, 0.0f);
				break; // what an odd difference. US added a useless break here.
#endif
#endif
		}

		strPos++;
	}

#ifndef VERSION_EU
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
#endif
}

#ifdef VERSION_EU
void print_hud_char_umlaut(s16 x, s16 y, u8 chr)
{
	void** fontLUT = segmented_to_virtual(main_hud_lut);

	gDPPipeSync(gDisplayListHead++);
	gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, fontLUT[chr]);
	gSPDisplayList(gDisplayListHead++, dl_rgba16_load_tex_block);
	gSPTextureRectangle(gDisplayListHead++, x << 2, y << 2, (x + 16) << 2, (y + 16) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);

	gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, fontLUT[GLYPH_UMLAUT]);
	gSPDisplayList(gDisplayListHead++, dl_rgba16_load_tex_block);
	gSPTextureRectangle(gDisplayListHead++, x << 2, (y - 4) << 2, (x + 16) << 2, (y + 12) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
}
#endif

/**
 * Prints a hud string depending of the hud table list defined.
 */
void print_hud_lut_string(s8 hudLUT, s16 x, s16 y, const u8* str)
{
	s32 strPos     = 0;
	void** hudLUT1 = (void**)segmented_to_virtual(menu_hud_lut); // Japanese Menu HUD Color font
	void** hudLUT2 = (void**)segmented_to_virtual(main_hud_lut); // 0-9 A-Z HUD Color Font
	u32 curX       = x;
	u32 curY       = y;

	u32 xStride; // X separation

	if(hudLUT == HUD_LUT_JPMENU)
	{
		xStride = 16;
	}
	else
	{ // HUD_LUT_GLOBAL
#ifdef VERSION_JP
		xStride = 14;
#else
		xStride = 12;
#endif
	}

	while(str[strPos] != GLOBAR_CHAR_TERMINATOR)
	{
#ifdef VERSION_EU
		switch(str[strPos])
		{
			case GLOBAL_CHAR_SPACE:
				curX += xStride / 2;
				break;
			case HUD_CHAR_A_UMLAUT:
				print_hud_char_umlaut(curX, curY, ASCII_TO_DIALOG('A'));
				curX += xStride;
				break;
			case HUD_CHAR_O_UMLAUT:
				print_hud_char_umlaut(curX, curY, ASCII_TO_DIALOG('O'));
				curX += xStride;
				break;
			case HUD_CHAR_U_UMLAUT:
				print_hud_char_umlaut(curX, curY, ASCII_TO_DIALOG('U'));
				curX += xStride;
				break;
			default:
#endif
#ifdef VERSION_US
				if(str[strPos] == GLOBAL_CHAR_SPACE)
				{
					if(0) //! dead code
					{
					}
					curX += 8;
				}
				else
				{
#endif
					gDPPipeSync(gDisplayListHead++);

					if(hudLUT == HUD_LUT_JPMENU)
						gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, hudLUT1[str[strPos]]);

					if(hudLUT == HUD_LUT_GLOBAL)
						gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, hudLUT2[str[strPos]]);

					gSPDisplayList(gDisplayListHead++, dl_rgba16_load_tex_block);
					gSPTextureRectangle(gDisplayListHead++, curX << 2, curY << 2, (curX + 16) << 2, (curY + 16) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);

					curX += xStride;
#ifdef VERSION_EU
					break;
				}
#endif
#ifdef VERSION_US
		}
#endif
		strPos++;
	}
}

#ifdef VERSION_EU
void print_menu_char_umlaut(s16 x, s16 y, u8 chr)
{
	void** fontLUT = segmented_to_virtual(menu_font_lut);

	gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_8b, 1, fontLUT[chr]);
	gDPLoadSync(gDisplayListHead++);
	gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 8 * 8 - 1, CALC_DXT(8, G_IM_SIZ_8b_BYTES));
	gSPTextureRectangle(gDisplayListHead++, x << 2, y << 2, (x + 8) << 2, (y + 8) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);

	gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_8b, 1, fontLUT[DIALOG_CHAR_UMLAUT]);
	gDPLoadSync(gDisplayListHead++);
	gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 8 * 8 - 1, CALC_DXT(8, G_IM_SIZ_8b_BYTES));
	gSPTextureRectangle(gDisplayListHead++, x << 2, (y - 4) << 2, (x + 8) << 2, (y + 4) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
}
#endif

void print_menu_generic_string(s16 x, s16 y, const u8* str)
{
	UNUSED s8 mark = DIALOG_MARK_NONE; // unused in EU
	s32 strPos     = 0;
	s32 curX       = x;
	s32 curY       = y;
	void** fontLUT = (void**)segmented_to_virtual(menu_font_lut);

	while(str[strPos] != DIALOG_CHAR_TERMINATOR)
	{
		switch(str[strPos])
		{
#ifdef VERSION_EU
			case DIALOG_CHAR_UPPER_A_UMLAUT:
				print_menu_char_umlaut(curX, curY, ASCII_TO_DIALOG('A'));
				curX += gDialogCharWidths[str[strPos]];
				break;
			case DIALOG_CHAR_UPPER_U_UMLAUT:
				print_menu_char_umlaut(curX, curY, ASCII_TO_DIALOG('U'));
				curX += gDialogCharWidths[str[strPos]];
				break;
			case DIALOG_CHAR_UPPER_O_UMLAUT:
				print_menu_char_umlaut(curX, curY, ASCII_TO_DIALOG('O'));
				curX += gDialogCharWidths[str[strPos]];
				break;
#else
			case DIALOG_CHAR_DAKUTEN:
				mark = DIALOG_MARK_DAKUTEN;
				break;
			case DIALOG_CHAR_PERIOD_OR_HANDAKUTEN:
				mark = DIALOG_MARK_HANDAKUTEN;
				break;
#endif
			case DIALOG_CHAR_SPACE:
				curX += 4;
				break;
			default:
				gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_8b, 1, fontLUT[str[strPos]]);
				gDPLoadSync(gDisplayListHead++);
				gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 8 * 8 - 1, CALC_DXT(8, G_IM_SIZ_8b_BYTES));
				gSPTextureRectangle(gDisplayListHead++, curX << 2, curY << 2, (curX + 8) << 2, (curY + 8) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);

#ifndef VERSION_EU
				if(mark != DIALOG_MARK_NONE)
				{
					gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_8b, 1, fontLUT[mark + 0xEF]);
					gDPLoadSync(gDisplayListHead++);
					gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 8 * 8 - 1, CALC_DXT(8, G_IM_SIZ_8b_BYTES));
					gSPTextureRectangle(gDisplayListHead++, (curX + 6) << 2, (curY - 7) << 2, (curX + 14) << 2, (curY + 1) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);

					mark = DIALOG_MARK_NONE;
				}
#endif
#ifdef VERSION_JP
				curX += 9;
#else
				curX += gDialogCharWidths[str[strPos]];
#endif
		}
		strPos++;
	}
}

void print_credits_string(s16 x, s16 y, const u8* str)
{
	s32 strPos     = 0;
	void** fontLUT = (void**)segmented_to_virtual(main_credits_font_lut);
	u32 curX       = x;
	u32 curY       = y;

	gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD);
	gDPTileSync(gDisplayListHead++);
	gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 2, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 3, G_TX_NOLOD, G_TX_CLAMP, 3, G_TX_NOLOD);
	gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, (8 - 1) << G_TEXTURE_IMAGE_FRAC, (8 - 1) << G_TEXTURE_IMAGE_FRAC);

	while(str[strPos] != GLOBAR_CHAR_TERMINATOR)
	{
		switch(str[strPos])
		{
			case GLOBAL_CHAR_SPACE:
				curX += 4;
				break;
			default:
				gDPPipeSync(gDisplayListHead++);
				gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, fontLUT[str[strPos]]);
				gDPLoadSync(gDisplayListHead++);
				gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 8 * 8 - 1, CALC_DXT(8, G_IM_SIZ_16b_BYTES));
				gSPTextureRectangle(gDisplayListHead++, curX << 2, curY << 2, (curX + 8) << 2, (curY + 8) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
				curX += 7;
				break;
		}
		strPos++;
	}
}

void handle_menu_scrolling(s8 scrollDirection, s8* currentIndex, s8 minIndex, s8 maxIndex)
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
			play_sound(SOUND_MENU_CHANGE_SELECT, gDefaultSoundArgs);
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
			play_sound(SOUND_MENU_CHANGE_SELECT, gDefaultSoundArgs);
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

// EU has both get_str_x_pos_from_center and get_str_x_pos_from_center_scale
// US and JP only implement one or the other
#if defined(VERSION_US) || defined(VERSION_EU)
s16 get_str_x_pos_from_center(s16 centerPos, u8* str, UNUSED f32 scale)
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
#endif

#if defined(VERSION_JP) || defined(VERSION_EU)
s16 get_str_x_pos_from_center_scale(s16 centerPos, u8* str, f32 scale)
{
	s16 strPos	= 0;
	f32 charsWidth	= 0.0f;
	f32 spacesWidth = 0.0f;

	while(str[strPos] != DIALOG_CHAR_TERMINATOR)
	{
		//! EU checks for dakuten and handakuten despite dialog code unable to handle it
		if(str[strPos] == DIALOG_CHAR_SPACE)
		{
			spacesWidth += 1.0;
		}
		else if(str[strPos] != DIALOG_CHAR_DAKUTEN && str[strPos] != DIALOG_CHAR_PERIOD_OR_HANDAKUTEN)
		{
			charsWidth += 1.0;
		}
		strPos++;
	}
	// return the x position of where the string starts as half the string's
	// length from the position of the provided center.
	return (f32)centerPos - (scale * (charsWidth / 2.0)) - ((scale / 2.0) * (spacesWidth / 2.0));
}
#endif

#ifndef VERSION_JP
s16 get_string_width(u8* str)
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
#endif

u8 gHudSymCoin[] = {GLYPH_COIN, GLYPH_SPACE};
u8 gHudSymX[]	 = {GLYPH_MULTIPLY, GLYPH_SPACE};

void print_hud_my_score_coins(s32 useCourseCoinScore, s8 fileNum, s8 courseNum, s16 x, s16 y)
{
	u8 strNumCoins[4];
	s16 numCoins;

	if(!useCourseCoinScore)
	{
		numCoins = (u16)(save_file_get_max_coin_score(courseNum) & 0xFFFF);
	}
	else
	{
		numCoins = save_file_get_course_coin_score(fileNum, courseNum);
	}

	if(numCoins != 0)
	{
		print_hud_lut_string(HUD_LUT_GLOBAL, x, y, gHudSymCoin);
		print_hud_lut_string(HUD_LUT_GLOBAL, x + 16, y, gHudSymX);
		int_to_str(numCoins, strNumCoins);
		print_hud_lut_string(HUD_LUT_GLOBAL, x + 32, y, strNumCoins);
	}
}

void print_hud_my_score_stars(s8 fileNum, s8 courseNum, s16 x, s16 y)
{
	u8 strStarCount[4];
	s16 starCount;
	u8 textSymStar[] = {GLYPH_STAR, GLYPH_SPACE};
	UNUSED u16 unused;
	u8 textSymX[] = {GLYPH_MULTIPLY, GLYPH_SPACE};

	starCount = save_file_get_course_star_count(fileNum, courseNum);

	if(starCount != 0)
	{
		print_hud_lut_string(HUD_LUT_GLOBAL, x, y, textSymStar);
		print_hud_lut_string(HUD_LUT_GLOBAL, x + 16, y, textSymX);
		int_to_str(starCount, strStarCount);
		print_hud_lut_string(HUD_LUT_GLOBAL, x + 32, y, strStarCount);
	}
}

void int_to_str(s32 num, u8* dst)
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

s16 get_dialog_id(void)
{
	return gDialogID;
}

void create_dialog_box(s16 dialog)
{
	if(gDialogID == -1)
	{
		gDialogID      = dialog;
		gDialogBoxType = DIALOG_TYPE_ROTATE;
	}
}

void create_dialog_box_with_var(s16 dialog, s32 dialogVar)
{
	if(gDialogID == -1)
	{
		gDialogID	= dialog;
		gDialogVariable = dialogVar;
		gDialogBoxType	= DIALOG_TYPE_ROTATE;
	}
}

void create_dialog_inverted_box(s16 dialog)
{
	if(gDialogID == -1)
	{
		gDialogID      = dialog;
		gDialogBoxType = DIALOG_TYPE_ZOOM;
	}
}

void create_dialog_box_with_response(s16 dialog)
{
	if(gDialogID == -1)
	{
		gDialogID	    = dialog;
		gDialogBoxType	    = DIALOG_TYPE_ROTATE;
		gLastDialogResponse = 1;
	}
}

void reset_dialog_render_state(void)
{
	level_set_transition(0, 0);

	if(gDialogBoxType == DIALOG_TYPE_ZOOM)
	{
		trigger_cutscene_dialog(2);
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

#ifdef VERSION_JP
#define X_VAL1 -5.0f
#define Y_VAL1 2.0
#define Y_VAL2 4.0f
#else
#define X_VAL1 -7.0f
#define Y_VAL1 5.0
#define Y_VAL2 5.0f
#endif

void render_dialog_box_type(struct DialogEntry* dialog, s8 linesPerBox)
{
	UNUSED s32 unused;

	create_dl_translation_matrix(MENU_MTX_NOPUSH, dialog->leftOffset, dialog->width, 0);

	switch(gDialogBoxType)
	{
		case DIALOG_TYPE_ROTATE: // Renders a dialog black box with zoom and rotation
			if(gDialogBoxState == DIALOG_STATE_OPENING || gDialogBoxState == DIALOG_STATE_CLOSING)
			{
				create_dl_scale_matrix(MENU_MTX_NOPUSH, 1.0 / gDialogBoxScale, 1.0 / gDialogBoxScale, 1.0f);
				// convert the speed into angle
				create_dl_rotation_matrix(MENU_MTX_NOPUSH, gDialogBoxOpenTimer * 4.0f, 0, 0, 1.0f);
			}
			gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 150);
			break;
		case DIALOG_TYPE_ZOOM: // Renders a dialog white box with zoom
			if(gDialogBoxState == DIALOG_STATE_OPENING || gDialogBoxState == DIALOG_STATE_CLOSING)
			{
				create_dl_translation_matrix(MENU_MTX_NOPUSH, 65.0 - (65.0 / gDialogBoxScale), (40.0 / gDialogBoxScale) - 40, 0);
				create_dl_scale_matrix(MENU_MTX_NOPUSH, 1.0 / gDialogBoxScale, 1.0 / gDialogBoxScale, 1.0f);
			}
			gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 150);
			break;
	}

	create_dl_translation_matrix(MENU_MTX_PUSH, X_VAL1, Y_VAL1, 0);
	create_dl_scale_matrix(MENU_MTX_NOPUSH, 1.1f, ((f32)linesPerBox / Y_VAL2) + 0.1, 1.0f);

	gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

void change_and_flash_dialog_text_color_lines(s8 colorMode, s8 lineNum)
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

#ifdef VERSION_EU
void render_generic_dialog_char_at_pos(struct DialogEntry* dialog, s16 x, s16 y, u8 c)
{
	s16 width;  // sp26
	s16 height; // sp24
	s16 tmpX;
	s16 tmpY;
	s16 xCoord; // sp1E
	s16 yCoord; // sp1C
	void** fontLUT;
	void* packedTexture;
	void* unpackedTexture;

	width  = (8.0 - (gDialogBoxScale * 0.8));
	height = (16.0 - (gDialogBoxScale * 0.8));
	tmpX   = (dialog->leftOffset + (65.0 - (65.0 / gDialogBoxScale)));
	tmpY   = ((240 - dialog->width) - ((40.0 / gDialogBoxScale) - 40));
	xCoord = (tmpX + (x / gDialogBoxScale));
	yCoord = (tmpY + (y / gDialogBoxScale));

	fontLUT		= segmented_to_virtual(main_font_lut);
	packedTexture	= segmented_to_virtual(fontLUT[c]);
	unpackedTexture = alloc_ia4_tex_from_i1(packedTexture, 8, 8);

	gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, VIRTUAL_TO_PHYSICAL(unpackedTexture));
	gSPDisplayList(gDisplayListHead++, dl_ia_text_tex_settings);
	gSPTextureRectangleFlip(gDisplayListHead++, xCoord << 2, (yCoord - height) << 2, (xCoord + width) << 2, yCoord << 2, G_TX_RENDERTILE, 16 << 5, 8 << 5, 1 << 10, 1 << 10);
}
#endif

#ifdef VERSION_JP
#define X_VAL3 5.0f
#define Y_VAL3 20
#else
#define X_VAL3 0.0f
#define Y_VAL3 16
#endif

#ifdef VERSION_EU
void handle_dialog_scroll_page_state(s8 lineNum, s8 totalLines, s8* pageState, s8* xMatrix)
#else
void handle_dialog_scroll_page_state(s8 lineNum, s8 totalLines, s8* pageState, s8* xMatrix, s16* linePos)
#endif
{
#ifndef VERSION_EU
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
#endif

	if(lineNum == totalLines)
	{
		pageState[0] = DIALOG_PAGE_STATE_SCROLL;
		return;
	}
#ifdef VERSION_EU
	gDialogY += 16;
#else
	create_dl_translation_matrix(MENU_MTX_PUSH, X_VAL3, 2 - (lineNum * Y_VAL3), 0);

	linePos[0] = 0;
#endif
	xMatrix[0] = 1;
}

#ifdef VERSION_JP
void adjust_pos_and_print_period_char(s8* xMatrix, s16* linePos)
{
	if(linePos[0] != 0)
	{
		create_dl_translation_matrix(MENU_MTX_NOPUSH, xMatrix[0] * 10, 0, 0);
	}

	create_dl_translation_matrix(MENU_MTX_PUSH, -2.0f, -5.0f, 0);
	render_generic_char(DIALOG_CHAR_PERIOD_OR_HANDAKUTEN);

	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

	linePos[0]++;
	xMatrix[0] = 1;
}
#endif

#ifdef VERSION_EU
void render_star_count_dialog_text(struct DialogEntry* dialog, s8* linePos)
#else
void render_star_count_dialog_text(s8* xMatrix, s16* linePos)
#endif
{
	s8 tensDigit = gDialogVariable / 10;
	s8 onesDigit = gDialogVariable - (tensDigit * 10); // remainder

	if(tensDigit != 0)
	{
#ifdef VERSION_JP
		create_dl_translation_matrix(MENU_MTX_NOPUSH, xMatrix[0] * 10, 0, 0);
		render_generic_char(tensDigit);
#elif defined(VERSION_EU)
		render_generic_dialog_char_at_pos(dialog, gDialogX, gDialogY, tensDigit);
		gDialogX += gDialogCharWidths[tensDigit];
		linePos[0] = 1;
#else
		if(xMatrix[0] != 1)
		{
			create_dl_translation_matrix(MENU_MTX_NOPUSH, (f32)(gDialogCharWidths[DIALOG_CHAR_SPACE] * xMatrix[0]), 0, 0);
		}

		render_generic_char(tensDigit);
		create_dl_translation_matrix(MENU_MTX_NOPUSH, (f32)gDialogCharWidths[tensDigit], 0, 0);
		xMatrix[0] = 1;
		linePos[0]++;
#endif
	}
#ifndef VERSION_EU
	else
	{
#ifdef VERSION_JP
		xMatrix[0]++;
#endif
	}
#endif

#ifdef VERSION_EU
	render_generic_dialog_char_at_pos(dialog, gDialogX, gDialogY, onesDigit);
	gDialogX += gDialogCharWidths[onesDigit];
	linePos[0] = 1;
#else

#ifdef VERSION_JP
	create_dl_translation_matrix(MENU_MTX_NOPUSH, xMatrix[0] * 10, 0, 0);
	render_generic_char(onesDigit);
#else
	if(xMatrix[0] != 1)
	{
		create_dl_translation_matrix(MENU_MTX_NOPUSH, (f32)(gDialogCharWidths[DIALOG_CHAR_SPACE] * (xMatrix[0] - 1)), 0, 0);
	}

	render_generic_char(onesDigit);
	create_dl_translation_matrix(MENU_MTX_NOPUSH, (f32)gDialogCharWidths[onesDigit], 0, 0);
#endif

	linePos[0]++;
	xMatrix[0] = 1;
#endif
}

#ifndef VERSION_JP
#ifdef VERSION_EU
void render_multi_text_string_lines(s8 multiTextId, s8 lineNum, s8 linesPerBox, UNUSED s16 linePos, s8 lowerBound, struct DialogEntry* dialog)
#else
void render_multi_text_string_lines(s8 multiTextId, s8 lineNum, s16* linePos, s8 linesPerBox, s8 xMatrix, s8 lowerBound)
#endif
{
	s8 i;
	struct MultiTextEntry textLengths[2] = {
	    {3, {TEXT_THE_RAW}},
	    {3, {TEXT_YOU_RAW}},
	};

	if(lineNum >= lowerBound && lineNum <= (lowerBound + linesPerBox))
	{
#ifdef VERSION_US
		if(linePos[0] != 0 || (xMatrix != 1))
		{
			create_dl_translation_matrix(MENU_MTX_NOPUSH, (gDialogCharWidths[DIALOG_CHAR_SPACE] * (xMatrix - 1)), 0, 0);
		}
#endif
		for(i = 0; i < textLengths[multiTextId].length; i++)
		{
#ifdef VERSION_EU
			render_generic_dialog_char_at_pos(dialog, gDialogX, gDialogY, textLengths[multiTextId].str[i]);
			gDialogX += gDialogCharWidths[textLengths[multiTextId].str[i]];
#else
			render_generic_char(textLengths[multiTextId].str[i]);
			create_dl_translation_matrix(MENU_MTX_NOPUSH, (gDialogCharWidths[textLengths[multiTextId].str[i]]), 0, 0);
#endif
		}
	}
#ifdef VERSION_US
	linePos += textLengths[multiTextId].length;
#endif
}
#endif

#ifdef VERSION_EU
void render_dialog_lowercase_diacritic(struct DialogEntry* dialog, u8 chr, u8 diacritic)
{
	render_generic_dialog_char_at_pos(dialog, gDialogX, gDialogY, chr);
	render_generic_dialog_char_at_pos(dialog, gDialogX, gDialogY, diacritic + 0xE7);
	gDialogX += gDialogCharWidths[chr];
}

void render_dialog_uppercase_diacritic(struct DialogEntry* dialog, u8 chr, u8 diacritic)
{
	render_generic_dialog_char_at_pos(dialog, gDialogX, gDialogY, chr);
	render_generic_dialog_char_at_pos(dialog, gDialogX, gDialogY - 4, diacritic + 0xE3);
	gDialogX += gDialogCharWidths[chr];
}
#endif

u32 ensure_nonnegative(s16 value)
{
	if(value < 0)
	{
		value = 0;
	}

	return value;
}

#if defined(VERSION_EU) && !defined(NON_MATCHING)
// TODO: EU is not quite matching
void handle_dialog_text_and_pages(s8 colorMode, struct DialogEntry* dialog, s8 lowerBound);
GLOBAL_ASM("asm/non_matchings/handle_dialog_text_and_pages_eu.s")
#else
#ifdef VERSION_JP
void handle_dialog_text_and_pages(s8 colorMode, struct DialogEntry* dialog)
#else
void handle_dialog_text_and_pages(s8 colorMode, struct DialogEntry* dialog, s8 lowerBound)
#endif
{
	UNUSED s32 pad[2];

	u8 strChar;

	u8* str = (u8*)segmented_to_virtual(dialog->str);
	s8 lineNum = 1;

	s8 totalLines;

	s8 pageState = DIALOG_PAGE_STATE_NONE;
	UNUSED s8 mark = DIALOG_MARK_NONE; // unused in US, EU
	s8 xMatrix = 1;

	s8 linesPerBox = dialog->linesPerBox;

	s16 strIdx;
#ifndef VERSION_EU
	s16 linePos = 0;
#endif

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
#ifdef VERSION_EU
	gDialogX = 0;

	// If this is turned into "gDialogY2 = 14;" with a symbol gDialogY2 that
	// alises gDialogY the code matches...
	gDialogY = 14;
#endif

	if(gDialogBoxState == DIALOG_STATE_HORIZONTAL)
	{
#ifdef VERSION_EU
		gDialogY -= gDialogScrollOffsetY;
#else
		create_dl_translation_matrix(MENU_MTX_NOPUSH, 0, (f32)gDialogScrollOffsetY, 0);
#endif
	}

#ifndef VERSION_EU
	create_dl_translation_matrix(MENU_MTX_PUSH, X_VAL3, 2 - lineNum * Y_VAL3, 0);
#endif

	while(pageState == DIALOG_PAGE_STATE_NONE)
	{
		change_and_flash_dialog_text_color_lines(colorMode, lineNum);
		strChar = str[strIdx];

		switch(strChar)
		{
			case DIALOG_CHAR_TERMINATOR:
				pageState = DIALOG_PAGE_STATE_END;
#ifndef VERSION_EU
				gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
#endif
				break;
			case DIALOG_CHAR_NEWLINE:
				lineNum++;
#ifdef VERSION_EU
				handle_dialog_scroll_page_state(lineNum, totalLines, &pageState, &xMatrix);
				gDialogX = 0;
#else
				handle_dialog_scroll_page_state(lineNum, totalLines, &pageState, &xMatrix, &linePos);
#endif
				break;
#ifdef VERSION_EU
			case DIALOG_CHAR_LOWER_A_GRAVE:
			case DIALOG_CHAR_LOWER_A_CIRCUMFLEX:
			case DIALOG_CHAR_LOWER_A_UMLAUT:
				render_dialog_lowercase_diacritic(dialog, ASCII_TO_DIALOG('a'), strChar & 0xF);
				break;
			case DIALOG_CHAR_UPPER_A_GRAVE:
			case DIALOG_CHAR_UPPER_A_CIRCUMFLEX:
			case DIALOG_CHAR_UPPER_A_UMLAUT:
				render_dialog_uppercase_diacritic(dialog, ASCII_TO_DIALOG('A'), strChar & 0xF);
				break;
			case DIALOG_CHAR_LOWER_E_GRAVE:
			case DIALOG_CHAR_LOWER_E_CIRCUMFLEX:
			case DIALOG_CHAR_LOWER_E_UMLAUT:
			case DIALOG_CHAR_LOWER_E_ACUTE:
				render_dialog_lowercase_diacritic(dialog, ASCII_TO_DIALOG('e'), strChar & 0xF);
				break;
			case DIALOG_CHAR_UPPER_E_GRAVE:
			case DIALOG_CHAR_UPPER_E_CIRCUMFLEX:
			case DIALOG_CHAR_UPPER_E_UMLAUT:
			case DIALOG_CHAR_UPPER_E_ACUTE:
				render_dialog_uppercase_diacritic(dialog, ASCII_TO_DIALOG('E'), strChar & 0xF);
				break;
			case DIALOG_CHAR_LOWER_U_GRAVE:
			case DIALOG_CHAR_LOWER_U_CIRCUMFLEX:
			case DIALOG_CHAR_LOWER_U_UMLAUT:
				render_dialog_lowercase_diacritic(dialog, ASCII_TO_DIALOG('u'), strChar & 0xF);
				break;
			case DIALOG_CHAR_UPPER_U_GRAVE:
			case DIALOG_CHAR_UPPER_U_CIRCUMFLEX:
			case DIALOG_CHAR_UPPER_U_UMLAUT:
				render_dialog_uppercase_diacritic(dialog, ASCII_TO_DIALOG('U'), strChar & 0xF);
				break;
			case DIALOG_CHAR_LOWER_O_CIRCUMFLEX:
			case DIALOG_CHAR_LOWER_O_UMLAUT:
				render_dialog_lowercase_diacritic(dialog, ASCII_TO_DIALOG('o'), strChar & 0xF);
				break;
			case DIALOG_CHAR_UPPER_O_CIRCUMFLEX:
			case DIALOG_CHAR_UPPER_O_UMLAUT:
				render_dialog_uppercase_diacritic(dialog, ASCII_TO_DIALOG('O'), strChar & 0xF);
				break;
			case DIALOG_CHAR_LOWER_I_CIRCUMFLEX:
			case DIALOG_CHAR_LOWER_I_UMLAUT:
				render_dialog_lowercase_diacritic(dialog, DIALOG_CHAR_I_NO_DIA, strChar & 0xF);
				break;
#else
			case DIALOG_CHAR_DAKUTEN:
				mark = DIALOG_MARK_DAKUTEN;
				break;
			case DIALOG_CHAR_PERIOD_OR_HANDAKUTEN:
				mark = DIALOG_MARK_HANDAKUTEN;
				break;
#endif
			case DIALOG_CHAR_SPACE:
#ifdef VERSION_EU
				gDialogX += gDialogCharWidths[DIALOG_CHAR_SPACE];
#else
#ifdef VERSION_JP
				if(linePos != 0)
				{
#endif
					xMatrix++;
#ifdef VERSION_JP
				}
#endif
				linePos++;

#endif
				break;
#ifdef VERSION_JP
			case DIALOG_CHAR_PERIOD:
				adjust_pos_and_print_period_char(&xMatrix, &linePos);
				break;
#else
			case DIALOG_CHAR_SLASH:
#ifdef VERSION_EU
				gDialogX += gDialogCharWidths[DIALOG_CHAR_SPACE] * 2;
#else
				xMatrix += 2;
				linePos += 2;
#endif
				break;
			case DIALOG_CHAR_MULTI_THE:
#ifdef VERSION_EU
				render_multi_text_string_lines(STRING_THE, lineNum, linesPerBox, xMatrix, lowerBound, dialog);
#else
				render_multi_text_string_lines(STRING_THE, lineNum, &linePos, linesPerBox, xMatrix, lowerBound);
#endif
				xMatrix = 1;
				break;
			case DIALOG_CHAR_MULTI_YOU:
#ifdef VERSION_EU
				render_multi_text_string_lines(STRING_YOU, lineNum, linesPerBox, xMatrix, lowerBound, dialog);
#else
				render_multi_text_string_lines(STRING_YOU, lineNum, &linePos, linesPerBox, xMatrix, lowerBound);
#endif
				xMatrix = 1;
				break;
#endif
			case DIALOG_CHAR_STAR_COUNT:
#ifdef VERSION_EU
				render_star_count_dialog_text(dialog, &xMatrix);
#else
				render_star_count_dialog_text(&xMatrix, &linePos);
#endif
				break;
#ifdef VERSION_EU
			case DIALOG_CHAR_DOUBLE_LOW_QUOTE:
				render_generic_dialog_char_at_pos(dialog, gDialogX, gDialogY + 8, 0xF6);
				gDialogX += gDialogCharWidths[0xF6];
				break;
#endif
			default: // any other character
#ifdef VERSION_JP
				if(linePos != 0)
				{
					create_dl_translation_matrix(MENU_MTX_NOPUSH, xMatrix * 10, 0, 0);
				}

				render_generic_char(strChar);
				xMatrix = 1;
				linePos++;

				if(mark != 0)
				{
					create_dl_translation_matrix(MENU_MTX_PUSH, 5.0f, 7.0f, 0);
					render_generic_char(mark + 0xEF);
					gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
					mark = 0;
				}
#elif defined(VERSION_US)
				if(lineNum >= lowerBound && lineNum <= lowerBound + linesPerBox)
				{
					if(linePos || xMatrix != 1)
					{
						create_dl_translation_matrix(MENU_MTX_NOPUSH, (f32)(gDialogCharWidths[DIALOG_CHAR_SPACE] * (xMatrix - 1)), 0, 0);
					}

					render_generic_char(strChar);
					create_dl_translation_matrix(MENU_MTX_NOPUSH, (f32)(gDialogCharWidths[strChar]), 0, 0);
					xMatrix = 1;
					linePos++;
				}
#else // VERSION_EU
				if(lineNum >= lowerBound && lineNum <= lowerBound + linesPerBox)
				{
					render_generic_dialog_char_at_pos(dialog, gDialogX, gDialogY, strChar);
				}
				gDialogX += gDialogCharWidths[strChar];
#endif
		}

#ifdef VERSION_JP
		if(linePos == 12)
		{
			if(str[strIdx + 1] == DIALOG_CHAR_PERIOD)
			{
				adjust_pos_and_print_period_char(&xMatrix, &linePos);
				strIdx++;
			}

			if(str[strIdx + 1] == DIALOG_CHAR_COMMA)
			{
				create_dl_translation_matrix(MENU_MTX_NOPUSH, xMatrix * 10, 0, 0);
				render_generic_char(DIALOG_CHAR_COMMA);
				strIdx++;
			}

			if(str[strIdx + 1] == DIALOG_CHAR_NEWLINE)
			{
				strIdx++;
			}

			if(str[strIdx + 1] == DIALOG_CHAR_TERMINATOR)
			{
				pageState = DIALOG_PAGE_STATE_END;
				gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
				break; // exit loop
			}
			else
			{
				lineNum++;
				handle_dialog_scroll_page_state(lineNum, totalLines, &pageState, &xMatrix, &linePos);
			}
		}
#endif

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
#endif

#ifdef VERSION_JP
#define X_VAL4_1 50
#define X_VAL4_2 25
#define Y_VAL4_1 1
#define Y_VAL4_2 20
#else
#define X_VAL4_1 56
#define X_VAL4_2 47
#define Y_VAL4_1 2
#define Y_VAL4_2 16
#endif

void render_dialog_triangle_choice(void)
{
	if(gDialogBoxState == DIALOG_STATE_VERTICAL)
	{
		handle_menu_scrolling(MENU_SCROLL_HORIZONTAL, &gDialogLineNum, 1, 2);
	}

	create_dl_translation_matrix(MENU_MTX_NOPUSH, (gDialogLineNum * X_VAL4_1) - X_VAL4_2, Y_VAL4_1 - (gLastDialogLineNum * Y_VAL4_2), 0);

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

#ifdef VERSION_EU
#define X_VAL5 122.0f
#define Y_VAL5_1 -16
#define Y_VAL5_2 3
#define X_Y_VAL6 0.5f
#elif defined(VERSION_US)
#define X_VAL5 118.0f
#define Y_VAL5_1 -16
#define Y_VAL5_2 5
#define X_Y_VAL6 0.8f
#elif defined(VERSION_JP)
#define X_VAL5 123.0f
#define Y_VAL5_1 -20
#define Y_VAL5_2 2
#define X_Y_VAL6 0.8f
#endif

void render_dialog_string_color(s8 linesPerBox)
{
	s32 timer = gGlobalTimer;

	if(timer & (0x08 * FRAME_RATE_SCALER_INV))
	{
		return;
	}

	create_dl_translation_matrix(MENU_MTX_PUSH, X_VAL5, (linesPerBox * Y_VAL5_1) + Y_VAL5_2, 0);
	create_dl_scale_matrix(MENU_MTX_NOPUSH, X_Y_VAL6, X_Y_VAL6, 1.0f);
	create_dl_rotation_matrix(MENU_MTX_NOPUSH, -DEFAULT_DIALOG_BOX_ANGLE, 0, 0, 1.0f);

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

void handle_special_dialog_text(s16 dialogID)
{	// dialog ID tables, in order
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
			sequence_player_unlower(0, 60);
			play_music(0, SEQUENCE_ARGS(4, SEQ_EVENT_BOSS), 0);
			return;
		}
	}

	for(i = 0; i < (s16)ARRAY_COUNT(dialogRaceSound); i++)
	{
		if(dialogRaceSound[i] == dialogID && gDialogLineNum == 1)
		{
			play_race_fanfare();
			return;
		}
	}

	for(i = 0; i < (s16)ARRAY_COUNT(dialogStarSound); i++)
	{
		if(dialogStarSound[i] == dialogID && gDialogLineNum == 1)
		{
			play_sound(SOUND_MENU_STAR_SOUND, gDefaultSoundArgs);
			return;
		}
	}

	for(i = 0; i < (s16)ARRAY_COUNT(dialogBossStop); i++)
	{
		if(dialogBossStop[i] == dialogID)
		{
			sequence_player_fade_out(0, 1);
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
    // This [8] string is actually unused. In the cutscene handler, the developers do not
    // set the 8th one, but use the first string again at the very end, so Peach ends up
    // saying "Mario!" twice. It is likely that she was originally meant to say "Mario?" at
    // the end but the developers changed their mind, possibly because the line recorded
    // sounded more like an exclamation than a question.
    gEndCutsceneStrEn8, NULL};

#ifdef VERSION_EU
u8 gEndCutsceneStrFr0[] = {TEXT_FILE_MARIO_EXCLAMATION};
u8 gEndCutsceneStrFr1[] = {TEXT_POWER_STARS_RESTORED_FR};
u8 gEndCutsceneStrFr2[] = {TEXT_THANKS_TO_YOU_FR};
u8 gEndCutsceneStrFr3[] = {TEXT_THANK_YOU_MARIO_FR};
u8 gEndCutsceneStrFr4[] = {TEXT_SOMETHING_SPECIAL_FR};
u8 gEndCutsceneStrFr5[] = {TEXT_COME_ON_EVERYBODY_FR};
u8 gEndCutsceneStrFr6[] = {TEXT_LETS_HAVE_CAKE_FR};
u8 gEndCutsceneStrFr7[] = {TEXT_FOR_MARIO_FR};
u8 gEndCutsceneStrFr8[] = {TEXT_FILE_MARIO_QUESTION};

u8* gEndCutsceneStringsFr[] = {gEndCutsceneStrFr0, gEndCutsceneStrFr1, gEndCutsceneStrFr2, gEndCutsceneStrFr3, gEndCutsceneStrFr4, gEndCutsceneStrFr5, gEndCutsceneStrFr6, gEndCutsceneStrFr7, gEndCutsceneStrFr8, NULL};

u8 gEndCutsceneStrDe0[] = {TEXT_FILE_MARIO_EXCLAMATION};
u8 gEndCutsceneStrDe1[] = {TEXT_POWER_STARS_RESTORED_DE};
u8 gEndCutsceneStrDe2[] = {TEXT_THANKS_TO_YOU_DE};
u8 gEndCutsceneStrDe3[] = {TEXT_THANK_YOU_MARIO_DE};
u8 gEndCutsceneStrDe4[] = {TEXT_SOMETHING_SPECIAL_DE};
u8 gEndCutsceneStrDe5[] = {TEXT_COME_ON_EVERYBODY_DE};
u8 gEndCutsceneStrDe6[] = {TEXT_LETS_HAVE_CAKE_DE};
u8 gEndCutsceneStrDe7[] = {TEXT_FOR_MARIO_DE};
u8 gEndCutsceneStrDe8[] = {TEXT_FILE_MARIO_QUESTION};

u8* gEndCutsceneStringsDe[] = {gEndCutsceneStrDe0, gEndCutsceneStrDe1, gEndCutsceneStrDe2, gEndCutsceneStrDe3, gEndCutsceneStrDe4, gEndCutsceneStrDe5, gEndCutsceneStrDe6, gEndCutsceneStrDe7, gEndCutsceneStrDe8, NULL};
#endif

u16 gCutsceneMsgFade	   = 0;
s16 gCutsceneMsgIndex	   = -1;
s16 gCutsceneMsgDuration   = -1;
s16 gCutsceneMsgTimer	   = 0;
s8 gDialogCameraAngleIndex = CAM_SELECTION_MARIO;
s8 gDialogCourseActNum	   = 1;

#ifdef VERSION_JP
#define DIAG_VAL1 20
#define DIAG_VAL3 130
#define DIAG_VAL4 4
#else
#define DIAG_VAL1 16
#define DIAG_VAL3 132 // US & EU
#define DIAG_VAL4 5
#endif
#ifdef VERSION_EU
#define DIAG_VAL2 238
#else
#define DIAG_VAL2 240 // JP & US
#endif

void render_dialog_entries(void)
{
#ifdef VERSION_EU
	s8 lowerBound;
#endif
	void** dialogTable;
	struct DialogEntry* dialog;
#ifdef VERSION_US
	s8 lowerBound;
#endif
#ifdef VERSION_EU
	gInGameLanguage = eu_get_language();
	switch(gInGameLanguage)
	{
		case LANGUAGE_ENGLISH:
			dialogTable = segmented_to_virtual(dialog_table_eu_en);
			break;
		case LANGUAGE_FRENCH:
			dialogTable = segmented_to_virtual(dialog_table_eu_fr);
			break;
		case LANGUAGE_GERMAN:
			dialogTable = segmented_to_virtual(dialog_table_eu_de);
			break;
	}
#else
	dialogTable = (void**)segmented_to_virtual(seg2_dialog_table);
#endif
	dialog = (DialogEntry*)segmented_to_virtual(dialogTable[gDialogID]);

	// if the dialog entry is invalid, set the ID to -1.
	if(segmented_to_virtual(NULL) == dialog)
	{
		gDialogID = -1;
		return;
	}

#ifdef VERSION_EU
	gDialogX = 0;
	gDialogY = 0;
#endif

	switch(gDialogBoxState)
	{
		case DIALOG_STATE_OPENING:
			if(gDialogBoxOpenTimer == DEFAULT_DIALOG_BOX_ANGLE)
			{
				play_dialog_sound(gDialogID);
				play_sound(SOUND_MENU_MESSAGE_APPEAR, gDefaultSoundArgs);
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
#ifndef VERSION_JP
			lowerBound = 1;
#endif
			break;
		case DIALOG_STATE_VERTICAL:
			gDialogBoxOpenTimer = 0.0f;

			if((sm64::player(0).controller().buttonPressed & sm64::hid::A_BUTTON) || (sm64::player(0).controller().buttonPressed & sm64::hid::B_BUTTON))
			{
				if(gLastDialogPageStrPos == -1)
				{
					handle_special_dialog_text(gDialogID);
					gDialogBoxState = DIALOG_STATE_CLOSING;
				}
				else
				{
					gDialogBoxState = DIALOG_STATE_HORIZONTAL;
					play_sound(SOUND_MENU_MESSAGE_NEXT_PAGE, gDefaultSoundArgs);
				}
			}
#ifndef VERSION_JP
			lowerBound = 1;
#endif
			break;
		case DIALOG_STATE_HORIZONTAL:
			gDialogScrollOffsetY += dialog->linesPerBox * 2 / FRAME_RATE_SCALER_INV;

			if(gDialogScrollOffsetY >= dialog->linesPerBox * DIAG_VAL1)
			{
				gDialogTextPos	     = gLastDialogPageStrPos;
				gDialogBoxState	     = DIALOG_STATE_VERTICAL;
				gDialogScrollOffsetY = 0;
			}
#ifndef VERSION_JP
			lowerBound = (gDialogScrollOffsetY / 16) + 1;
#endif
			break;
		case DIALOG_STATE_CLOSING:
			if(gDialogBoxOpenTimer == 20.0f)
			{
				level_set_transition(0, 0);
				play_sound(SOUND_MENU_MESSAGE_DISAPPEAR, gDefaultSoundArgs);

				if(gDialogBoxType == DIALOG_TYPE_ZOOM)
				{
					trigger_cutscene_dialog(2);
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
#ifndef VERSION_JP
			lowerBound = 1;
#endif
			break;
	}

	render_dialog_box_type(dialog, dialog->linesPerBox);

	gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, ensure_nonnegative(DIAG_VAL2 - dialog->width), SCREEN_WIDTH, ensure_nonnegative(240 + ((dialog->linesPerBox * 80) / DIAG_VAL4) - dialog->width));

	handle_dialog_text_and_pages(0, dialog, lowerBound);

	if(gLastDialogPageStrPos == -1 && gLastDialogResponse == 1)
	{
		render_dialog_triangle_choice();
	}

	gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 2, 2, SCREEN_WIDTH - BORDER_HEIGHT / 2, SCREEN_HEIGHT - BORDER_HEIGHT / 2);

	if(gLastDialogPageStrPos != -1 && gDialogBoxState == DIALOG_STATE_VERTICAL)
	{
		render_dialog_string_color(dialog->linesPerBox);
	}
}

// Calls a gMenuMode value defined by render_menus_and_dialogs cases
void set_menu_mode(s16 mode)
{
	if(gMenuMode == -1)
	{
		gMenuMode = mode;
	}
}

void reset_cutscene_msg_fade(void)
{
	gCutsceneMsgFade = 0;
}

void dl_rgba16_begin_cutscene_msg_fade(void)
{
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gCutsceneMsgFade);
}

void dl_rgba16_stop_cutscene_msg_fade(void)
{
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);

	if(gCutsceneMsgFade < 250)
	{
		gCutsceneMsgFade += 25 / FRAME_RATE_SCALER_INV;
	}
	else
	{
		gCutsceneMsgFade = 255;
	}
}

u8 ascii_to_credits_char(u8 c)
{
	if(c >= 'A' && c <= 'Z')
	{
		return (c - ('A' - 0xA));
	}

	if(c >= 'a' && c <= 'z')
	{ // remap lower to upper case
		return (c - ('a' - 0xA));
	}

	if(c == ' ')
	{
		return GLOBAL_CHAR_SPACE;
	}
	if(c == '.')
	{
		return 0x24;
	}
	if(c == '3')
	{
		return ASCII_TO_DIALOG('3');
	}
	if(c == '4')
	{
		return ASCII_TO_DIALOG('4');
	}
	if(c == '6')
	{
		return ASCII_TO_DIALOG('6');
	}

	return GLOBAL_CHAR_SPACE;
}

void print_credits_str_ascii(s16 x, s16 y, const char* str)
{
	s32 pos = 0;
	u8 c	= str[pos];
	u8 creditStr[100];

	while(c != 0)
	{
		creditStr[pos++] = ascii_to_credits_char(c);
		c		 = str[pos];
	}

	creditStr[pos] = GLOBAR_CHAR_TERMINATOR;

	print_credits_string(x, y, creditStr);
}

void set_cutscene_message(s16 xOffset, s16 yOffset, s16 msgIndex, Duration msgDuration)
{
	// is message done printing?
	if(gCutsceneMsgIndex == -1)
	{
		gCutsceneMsgIndex    = msgIndex;
		gCutsceneMsgDuration = msgDuration;
		gCutsceneMsgTimer    = 0;
		gCutsceneMsgXOffset  = xOffset;
		gCutsceneMsgYOffset  = yOffset;
		gCutsceneMsgFade     = 0;
	}
}

void do_cutscene_handler(void)
{
	s16 x;

	// is a cutscene playing? do not perform this handler's actions if so.
	if(gCutsceneMsgIndex == -1)
	{
		return;
	}

	create_dl_ortho_matrix();

	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gCutsceneMsgFade);

#ifdef VERSION_EU
	switch(eu_get_language())
	{
		case LANGUAGE_ENGLISH:
			x = get_str_x_pos_from_center(gCutsceneMsgXOffset, gEndCutsceneStringsEn[gCutsceneMsgIndex], 10.0f);
			print_generic_string(x, 240 - gCutsceneMsgYOffset, gEndCutsceneStringsEn[gCutsceneMsgIndex]);
			break;
		case LANGUAGE_FRENCH:
			x = get_str_x_pos_from_center(gCutsceneMsgXOffset, gEndCutsceneStringsFr[gCutsceneMsgIndex], 10.0f);
			print_generic_string(x, 240 - gCutsceneMsgYOffset, gEndCutsceneStringsFr[gCutsceneMsgIndex + 8]);
			break;
		case LANGUAGE_GERMAN:
			x = get_str_x_pos_from_center(gCutsceneMsgXOffset, gEndCutsceneStringsDe[gCutsceneMsgIndex], 10.0f);
			print_generic_string(x, 240 - gCutsceneMsgYOffset, gEndCutsceneStringsDe[gCutsceneMsgIndex + 16]);
			break;
	}
#else
	// get the x coordinate of where the cutscene string starts.
	x = get_str_x_pos_from_center(gCutsceneMsgXOffset, gEndCutsceneStringsEn[gCutsceneMsgIndex], 10.0f);

	print_generic_string(x, 240 - gCutsceneMsgYOffset, gEndCutsceneStringsEn[gCutsceneMsgIndex]);
#endif

	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);

	// if the timing variable is less than 5, increment
	// the fade until we are at full opacity.
	if(gCutsceneMsgTimer < 5 * FRAME_RATE_SCALER_INV)
	{
		gCutsceneMsgFade += 50 / FRAME_RATE_SCALER_INV;
	}

	// if the cutscene frame length + the fade-in counter is
	// less than the timer, it means we have exceeded the
	// time that the message is supposed to remain on
	// screen. if (message_duration = 50) and (msg_timer = 55)
	// then after the first 5 frames, the message will remain
	// on screen for another 50 frames until it starts fading.
	if(gCutsceneMsgDuration + 5 < gCutsceneMsgTimer)
	{
		gCutsceneMsgFade -= 50 / FRAME_RATE_SCALER_INV;
	}

	// like the first check, it takes 5 frames to fade out, so
	// perform a + 10 to account for the earlier check (10-5=5).
	if(gCutsceneMsgDuration + 10 < gCutsceneMsgTimer)
	{
		gCutsceneMsgIndex = -1;
		gCutsceneMsgFade  = 0;
		gCutsceneMsgTimer = 0;
		return;
	}

	gCutsceneMsgTimer++;
}

#define PEACH_MESSAGE_TIMER 250
#define STR_X 38
#define STR_Y 142

// "Dear Mario" message handler
void print_peach_letter_message(void)
{
	void** dialogTable;
	struct DialogEntry* dialog;
	u8* str;
#ifdef VERSION_EU
	gInGameLanguage = eu_get_language();
	switch(gInGameLanguage)
	{
		case LANGUAGE_ENGLISH:
			dialogTable = segmented_to_virtual(dialog_table_eu_en);
			break;
		case LANGUAGE_FRENCH:
			dialogTable = segmented_to_virtual(dialog_table_eu_fr);
			break;
		case LANGUAGE_GERMAN:
			dialogTable = segmented_to_virtual(dialog_table_eu_de);
			break;
	}
#else
	dialogTable = (void**)segmented_to_virtual(seg2_dialog_table);
#endif
	dialog = (DialogEntry*)segmented_to_virtual(dialogTable[gDialogID]);

	str = (u8*)segmented_to_virtual(dialog->str);

	create_dl_translation_matrix(MENU_MTX_PUSH, 97.0f, 118.0f, 0);

	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gCutsceneMsgFade);
	gSPDisplayList(gDisplayListHead++, castle_grounds_seg7_dl_0700EA58);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 20, 20, 20, gCutsceneMsgFade);

	print_generic_string(STR_X, STR_Y, str);
#ifdef VERSION_JP
	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
#endif
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
#ifndef VERSION_JP
	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
	gDPSetEnvColor(gDisplayListHead++, 200, 80, 120, gCutsceneMsgFade);
	gSPDisplayList(gDisplayListHead++, castle_grounds_seg7_us_dl_0700F2E8);
#endif

	// at the start/end of message, reset the fade.
	if(gCutsceneMsgTimer == 0)
	{
		gCutsceneMsgFade = 0;
	}

	// we're less than 20 increments, so increase the fade.
	if(gCutsceneMsgTimer < 20 * FRAME_RATE_SCALER_INV)
	{
		gCutsceneMsgFade += 10 / FRAME_RATE_SCALER_INV;
	}

	// we're after PEACH_MESSAGE_TIMER increments, so decrease the fade.
	if(gCutsceneMsgTimer > PEACH_MESSAGE_TIMER * FRAME_RATE_SCALER_INV)
	{
		gCutsceneMsgFade -= 10 / FRAME_RATE_SCALER_INV;
	}

	// 20 increments after the start of the decrease, we're
	// back where we are, so reset everything at the end.
	if(gCutsceneMsgTimer > (PEACH_MESSAGE_TIMER + 20) * FRAME_RATE_SCALER_INV)
	{
		gCutsceneMsgIndex = -1;
		gCutsceneMsgFade  = 0; //! uselessly reset since the next execution will just set it to 0 again.
		gDialogID	  = -1;
		gCutsceneMsgTimer = 0;
		return; // return to avoid incrementing the timer
	}

	gCutsceneMsgTimer++;
}

/**
 * Renders the cannon reticle when Mario is inside a cannon.
 * Formed by four triangles.
 */
void render_hud_cannon_reticle(void)
{
	create_dl_translation_matrix(MENU_MTX_PUSH, 160.0f, 120.0f, 0);

	gDPSetEnvColor(gDisplayListHead++, 50, 50, 50, 180);
	create_dl_translation_matrix(MENU_MTX_PUSH, -20.0f, -8.0f, 0);
	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

	create_dl_translation_matrix(MENU_MTX_PUSH, 20.0f, 8.0f, 0);
	create_dl_rotation_matrix(MENU_MTX_NOPUSH, 180.0f, 0, 0, 1.0f);
	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

	create_dl_translation_matrix(MENU_MTX_PUSH, 8.0f, -20.0f, 0);
	create_dl_rotation_matrix(MENU_MTX_NOPUSH, DEFAULT_DIALOG_BOX_ANGLE, 0, 0, 1.0f);
	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

	create_dl_translation_matrix(MENU_MTX_PUSH, -8.0f, 20.0f, 0);
	create_dl_rotation_matrix(MENU_MTX_NOPUSH, -DEFAULT_DIALOG_BOX_ANGLE, 0, 0, 1.0f);
	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

void reset_red_coins_collected(void)
{
	gRedCoinsCollected = 0;
}

void change_dialog_camera_angle(void)
{
	if(cam_select_alt_mode(0) == CAM_SELECTION_MARIO)
	{
		gDialogCameraAngleIndex = CAM_SELECTION_MARIO;
	}
	else
	{
		gDialogCameraAngleIndex = CAM_SELECTION_FIXED;
	}
}

void shade_screen(void)
{
	create_dl_translation_matrix(MENU_MTX_PUSH, GFX_DIMENSIONS_FROM_LEFT_EDGE(0), 240.0f, 0);

	// This is a bit weird. It reuses the dialog text box (width 130, height -80),
	// so scale to at least fit the screen.
#ifdef TARGET_N64
	create_dl_scale_matrix(MENU_MTX_NOPUSH, 2.6f, 3.4f, 1.0f);
#else
	create_dl_scale_matrix(MENU_MTX_NOPUSH, GFX_DIMENSIONS_ASPECT_RATIO * SCREEN_HEIGHT / 130.0f, 3.0f, 1.0f);
#endif

	gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 110);
	gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

void print_animated_red_coin(s16 x, s16 y)
{
	s32 timer = gGlobalTimer;

	create_dl_translation_matrix(MENU_MTX_PUSH, x, y, 0);
	create_dl_scale_matrix(MENU_MTX_NOPUSH, 0.2f, 0.2f, 1.0f);
	gDPSetRenderMode(gDisplayListHead++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);

	switch((timer / FRAME_RATE_SCALER_INV) & 6)
	{
		case 0:
			gSPDisplayList(gDisplayListHead++, coin_seg3_dl_03007940);
			break;
		case 2:
			gSPDisplayList(gDisplayListHead++, coin_seg3_dl_03007968);
			break;
		case 4:
			gSPDisplayList(gDisplayListHead++, coin_seg3_dl_03007990);
			break;
		case 6:
			gSPDisplayList(gDisplayListHead++, coin_seg3_dl_030079B8);
			break;
	}

	gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

void render_pause_red_coins(void)
{
	s8 x;

	for(x = 0; x < gRedCoinsCollected; x++)
	{
		print_animated_red_coin(GFX_DIMENSIONS_FROM_RIGHT_EDGE(30) - x * 20, 16);
	}
}

#ifdef VERSION_EU
u8 gTextCourseArr[][7] = { // D_802FDA10
    {TEXT_COURSE},
    {TEXT_COURSE_FR},
    {TEXT_COURSE_DE}};
#endif

#ifdef VERSION_JP
#define CRS_NUM_X1 93
#else
#define CRS_NUM_X1 100
#endif
#ifdef VERSION_EU
#define TXT_STAR_X 89
#define ACT_NAME_X 107
#define LVL_NAME_X 108
#define MYSCORE_X 48
#else
#define TXT_STAR_X 98
#define ACT_NAME_X 116
#define LVL_NAME_X 117
#define MYSCORE_X 62
#endif

void render_pause_my_score_coins(void)
{
#ifdef VERSION_EU
	u8 textMyScore[][10] = {{TEXT_MY_SCORE}, {TEXT_MY_SCORE_FR}, {TEXT_MY_SCORE_DE}};
#define textMyScore textMyScore[gInGameLanguage]
#else
	u8 textCourse[] = {TEXT_COURSE};
	u8 textMyScore[] = {TEXT_MY_SCORE};
#endif
	u8 textStar[]	      = {TEXT_STAR};
	u8 textUnfilledStar[] = {TEXT_UNFILLED_STAR};

	u8 strCourseNum[4];
	void** courseNameTbl;
	u8* courseName;
	void** actNameTbl;
	u8* actName;
	u8 courseIndex;
	u8 starFlags;

	courseNameTbl = (void**)segmented_to_virtual(seg2_course_name_table);
	actNameTbl    = (void**)segmented_to_virtual(seg2_act_name_table);

	courseIndex = gCurrCourseNum - 1;
	starFlags   = save_file_get_star_flags(gCurrSaveFileNum - 1, gCurrCourseNum - 1);

#ifdef VERSION_EU
	switch(gInGameLanguage)
	{
		case LANGUAGE_ENGLISH:
			actNameTbl    = segmented_to_virtual(act_name_table_eu_en);
			courseNameTbl = segmented_to_virtual(course_name_table_eu_en);
			break;
		case LANGUAGE_FRENCH:
			actNameTbl    = segmented_to_virtual(act_name_table_eu_fr);
			courseNameTbl = segmented_to_virtual(course_name_table_eu_fr);
			break;
		case LANGUAGE_GERMAN:
			actNameTbl    = segmented_to_virtual(act_name_table_eu_de);
			courseNameTbl = segmented_to_virtual(course_name_table_eu_de);
			break;
	}
#endif

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);

	if(courseIndex < COURSE_STAGES_COUNT)
	{
		print_hud_my_score_coins(1, gCurrSaveFileNum - 1, courseIndex, 178, 103);
		print_hud_my_score_stars(gCurrSaveFileNum - 1, courseIndex, 118, 103);
	}

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);

	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);

	if(courseIndex < COURSE_STAGES_COUNT && save_file_get_course_star_count(gCurrSaveFileNum - 1, courseIndex) != 0)
	{
		print_generic_string(MYSCORE_X, 121, textMyScore);
	}

	courseName = (u8*)segmented_to_virtual(courseNameTbl[courseIndex]);

	if(courseIndex < COURSE_STAGES_COUNT)
	{
#ifdef VERSION_EU
		print_generic_string(48, 157, gTextCourseArr[gInGameLanguage]);
#else
		print_generic_string(63, 157, textCourse);
#endif
		int_to_str(gCurrCourseNum, strCourseNum);
#ifdef VERSION_EU
		print_generic_string(get_string_width(gTextCourseArr[gInGameLanguage]) + 51, 157, strCourseNum);
#else
		print_generic_string(CRS_NUM_X1, 157, strCourseNum);
#endif

		actName = (u8*)segmented_to_virtual(actNameTbl[(gCurrCourseNum - 1) * 6 + gDialogCourseActNum - 1]);

		if(starFlags & (1 << (gDialogCourseActNum - 1)))
		{
			print_generic_string(TXT_STAR_X, 140, textStar);
		}
		else
		{
			print_generic_string(TXT_STAR_X, 140, textUnfilledStar);
		}
		print_generic_string(ACT_NAME_X, 140, actName);
#ifndef VERSION_JP
		print_generic_string(LVL_NAME_X, 157, &courseName[3]);
#endif
	}
#ifndef VERSION_JP
	else
	{
#ifdef VERSION_US
		print_generic_string(94, 157, &courseName[3]);
#elif defined(VERSION_EU)
		print_generic_string(get_str_x_pos_from_center(159, &courseName[3], 10.0f), 157, &courseName[3]);
#endif
	}
#else
	print_generic_string(117, 157, &courseName[3]);
#endif
	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

#ifdef VERSION_JP
#define TXT1_X 4
#define TXT2_X 116
#define Y_VAL7 0
#else
#define TXT1_X 3
#define TXT2_X 119
#define Y_VAL7 2
#endif

void render_pause_camera_options(s16 x, s16 y, s8* index, s16 xIndex)
{
	u8 textLakituMario[] = {TEXT_LAKITU_MARIO};
	u8 textLakituStop[]  = {TEXT_LAKITU_STOP};
#ifdef VERSION_EU
	u8 textNormalUpClose[][20] = {{TEXT_NORMAL_UPCLOSE}, {TEXT_NORMAL_UPCLOSE_FR}, {TEXT_NORMAL_UPCLOSE_DE}};
	u8 textNormalFixed[][17]   = {
	      {TEXT_NORMAL_FIXED},
	      {TEXT_NORMAL_FIXED_FR},
	      {TEXT_NORMAL_FIXED_DE},
	  };
#define textNormalUpClose textNormalUpClose[gInGameLanguage]
#define textNormalFixed textNormalFixed[gInGameLanguage]
#else
	u8 textNormalUpClose[] = {TEXT_NORMAL_UPCLOSE};
	u8 textNormalFixed[] = {TEXT_NORMAL_FIXED};
#endif

	handle_menu_scrolling(MENU_SCROLL_HORIZONTAL, index, 1, 2);

	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);

	print_generic_string(x + 14, y + 2, textLakituMario);
	print_generic_string(x + TXT1_X, y - 13, textNormalUpClose);
	print_generic_string(x + 124, y + 2, textLakituStop);
	print_generic_string(x + TXT2_X, y - 13, textNormalFixed);

	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
	create_dl_translation_matrix(MENU_MTX_PUSH, ((index[0] - 1) * xIndex) + x, y + Y_VAL7, 0);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);
	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

	switch(index[0])
	{
		case 1:
			cam_select_alt_mode(1);
			break;
		case 2:
			cam_select_alt_mode(2);
			break;
	}
}

#ifdef VERSION_JP
#define X_VAL8 0
#define Y_VAL8 4
#else
#define X_VAL8 4
#define Y_VAL8 2
#endif

void render_pause_course_options(s16 x, s16 y, s8* index, s16 yIndex)
{
	u8 textContinue[]     = {TEXT_CONTINUE};
	u8 textExitCourse[]   = {TEXT_EXIT_COURSE};
	u8 textCameraAngleR[] = {TEXT_CAMERA_ANGLE_R};

	handle_menu_scrolling(MENU_SCROLL_VERTICAL, index, 1, 3);

	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);

	print_generic_string(x + 10, y - 2, textContinue);
	print_generic_string(x + 10, y - 17, textExitCourse);

	draw_pause_nav();

	if(index[0] != 3)
	{
		print_generic_string(x + 10, y - 33, textCameraAngleR);
		gSPDisplayList(gDisplayListHead++, dl_ia_text_end);

		create_dl_translation_matrix(MENU_MTX_PUSH, x - X_VAL8, (y - ((index[0] - 1) * yIndex)) - Y_VAL8, 0);

		gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);
		gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
		gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
	}

	if(index[0] == 3)
	{
		render_pause_camera_options(x - 42, y - 42, &gDialogCameraAngleIndex, 110);
	}
}

namespace sm64::hud
{
	class String
	{
		public:
		/*String()
		{
		}*/

		String(const std::string& str)
		{
			for(auto& c : str)
			{
				if(c >= 'A' && c <= 'Z')
				{
					m_buffer.push_back(c + 0x0A - 'A');
				}
				else if(c >= 'a' && c <= 'z')
				{
					m_buffer.push_back(c + 0x0A - 'a');
				}
				else if(c >= '0' && c <= '9')
				{
					m_buffer.push_back(c - '0');
				}
				else if(c == ' ')
				{
					m_buffer.push_back(0x9E);
				}
				else if(c == '.')
				{
					m_buffer.push_back(0x24);
				}
				else
				{
					m_buffer.push_back(0x00);
				}
			}

			m_buffer.push_back(0xFF);
		}

		const u8* buffer() const
		{
			return m_buffer.data();
		}

		protected:
		std::vector<u8> m_buffer;
	};
} // namespace sm64::hud

void print_hud_colorful_str(const sm64::hud::String& str, s16 x, s16 y)
{
	u8 textPause[] = {TEXT_PAUSE};

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);

	print_hud_lut_string(HUD_LUT_GLOBAL, x, y, str.buffer());

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
}

sm64::hud::String R_NEXT = std::string("R");
sm64::hud::String R_BACK = std::string("BACK");

namespace sm64::menu::item
{
	class Base
	{
		public:
		Base(const std::string& title, const std::function<void(s64)>& onChange) : m_title(title), m_titleEncoded(title), m_onChange(onChange)
		{
		}

		virtual ~Base()
		{
		}

		virtual void draw(const s16 x, const s16 y)
		{
			print_generic_string(x, y, m_titleEncoded.buffer());
		}

		virtual void click()
		{
		}

		const char* title() const
		{
			return m_title.c_str();
		}
		const u8* titleEncoded() const
		{
			return m_titleEncoded.buffer();
		}

		protected:
		std::string m_title;
		hud::String m_titleEncoded;
		std::function<void(s64)> m_onChange;
	};

	class EnumFloat : public Base
	{
		public:
		EnumFloat(const std::string& title, const std::vector<float>& enums, float* value) : Base(title, nullptr), m_value(value), m_index(0)
		{
			char buffer[32] = {};
			for(auto f : enums)
			{
				m_enums.push_back(f);
				sprintf(buffer, "%2.2f", f);
				m_labels.push_back(hud::String(buffer));
			}
		}

		void draw(const s16 x, const s16 y) override
		{
			Base::draw(x, y);

			if(m_value)
			{
				u32 i = 0;
				for(auto v : m_enums)
				{
					if(v == *m_value)
					{
						print_generic_string(x + 110, y, m_labels[i].buffer());
						break;
					}
					i++;
				}
			}
		}

		void click() override
		{
			m_index = (m_index + 1) % m_enums.size();

			if(m_value && m_index < m_enums.size())
			{
				*m_value = m_enums[m_index];
			}
		}

		protected:
		std::vector<float> m_enums;
		std::vector<hud::String> m_labels;
		float* m_value;
		u32 m_index;
	};

	class Bool : public Base
	{
		public:
		Bool(const std::string& title, bool* value, const std::function<void(bool)>& onChange = nullptr) : Base(title, nullptr), m_value(value), m_trueString("ON"), m_falseString("OFF"), m_onBoolChange(onChange)
		{
			m_onChange = [this](s64 v) {
				if(m_value)
				{
					*m_value = v != 0;

					if (m_onBoolChange)
					{
						m_onBoolChange(*m_value);
					}
				}
			};
		}

		void draw(const s16 x, const s16 y) override
		{
			Base::draw(x, y);

			if(m_value)
			{
				if(*m_value)
				{
					print_generic_string(x + 110, y, m_trueString.buffer());
				}
				else
				{
					print_generic_string(x + 110, y, m_falseString.buffer());
				}
			}
		}

		void click() override
		{
			if(m_value)
			{
				*m_value = !*m_value;

				if (m_onBoolChange)
				{
					m_onBoolChange(*m_value);
				}
			}
		}

		protected:
		bool* m_value;
		hud::String m_trueString;
		hud::String m_falseString;
		std::function<void(bool)> m_onBoolChange;
	};
} // namespace sm64::menu::item

namespace sm64::menu
{
#define DIALOG_TITLE_X 123
#define DIALOG_TITLE_Y 61

	class Dialog
	{
		public:
		Dialog(const char* title = "") : m_index(1), title(title)
		{
		}

		virtual ~Dialog()
		{
		}

		virtual void render(const s16 x, const s16 y, const s16 yIndex = 15)
		{
			handle_menu_scrolling(MENU_SCROLL_VERTICAL, &m_index, 1, m_items.size());

			if(sm64::player(0).controller().buttonPressed & sm64::hid::A_BUTTON)
			{
				if(m_index && m_index - 1 < m_items.size())
				{
					m_items[m_index - 1]->click();
				}

				sm64::config().save();
			}

			gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
			gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);

			s32 yOffset = 2;
			for(u32 i = 0; i < m_items.size(); i++)
			{
				auto& item = m_items[i];
				item->draw(x + 10, y - yOffset);
				yOffset += 15;
			}

			draw_pause_nav();

			// print_generic_string(x + 10, y - 33, textCameraAngleR);
			gSPDisplayList(gDisplayListHead++, dl_ia_text_end);

			create_dl_translation_matrix(MENU_MTX_PUSH, x - X_VAL8, (y - ((m_index - 1) * yIndex)) - Y_VAL8, 0);

			gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);
			gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
			gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
		}

		hud::String title;

		protected:
		std::vector<item::Base*> m_items;
		s8 m_index;
	};

	class Cheats : public Dialog
	{
		public:
		Cheats() : Dialog("CHEATS")
		{
			m_items.push_back(new sm64::menu::item::Bool("UNLIMITED LIVES", &sm64::config().cheats().unlimitedLives()));
			m_items.push_back(new sm64::menu::item::Bool("INVINCIBLE", &sm64::config().cheats().invincible()));
			m_items.push_back(new sm64::menu::item::Bool("MOON JUMP", &sm64::config().cheats().moonJump()));
			m_items.push_back(new sm64::menu::item::Bool("SUPER JUMP", &sm64::config().cheats().superJump()));
			m_items.push_back(new sm64::menu::item::Bool("QUADRUPLE JUMP", &sm64::config().cheats().quadrupleJump()));
			m_items.push_back(new sm64::menu::item::EnumFloat("BOWSER AIM ASSIST", {0.0f, 12.5f, 25.0f, 45.0f, 60.0f}, &sm64::config().cheats().bowserAimAssist()));
		}

		void render(const s16 x, const s16 y, const s16 yIndex = 15) override
		{
			print_hud_colorful_str(std::string("CHEATS"), DIALOG_TITLE_X, DIALOG_TITLE_Y);
			Dialog::render(x, y, yIndex);
		}
	};

	class Mods : public Dialog
	{
	public:
		Mods() : Dialog("MODIFICATIONS")
		{
			for(auto& dir : config().mods().dirs())
			{
				std::string name = dir.first.c_str() + ((dir.first[0] < '0' || dir.first[0] == '~') ? 1 : 0);
				std::transform(name.begin(), name.end(), name.begin(), ::toupper);
				m_items.push_back(new sm64::menu::item::Bool(name, &dir.second));
			}
		}

		void render(const s16 x, const s16 y, const s16 yIndex = 15) override
		{
			print_hud_colorful_str(std::string("MODS"), DIALOG_TITLE_X, DIALOG_TITLE_Y);
			Dialog::render(x, y, yIndex);
		}
	};

	class Camera : public Dialog
	{
		public:
		Camera() : Dialog("CAMERA")
		{
			// m_items.push_back(new sm64::menu::item::EnumFloat("DISTANCE SCALER", {1.0f, 1.25f, 1.5f, 2.0f, 3.0f}, &sm64::config().camera().distanceScaler()));

			m_items.push_back(new sm64::menu::item::Bool("DISABLE DIST CLIP", &sm64::config().camera().setDisableDistanceClip()));
			m_items.push_back(new sm64::menu::item::Bool("CLASSIC CAMERA", &sm64::config().camera().useClassicCamera()));
			m_items.push_back(new sm64::menu::item::EnumFloat("RETURN SCALER", {1.0f, 0.75f, 0.5f, 0.25f, 0.10f, 0.0f}, &sm64::config().camera().yawReturnScaler()));

#ifdef ENABLE_MOUSE
			m_items.push_back(new sm64::menu::item::EnumFloat("MOUSE X SCALER", { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f }, &sm64::config().camera().mousexScaler()));
			m_items.push_back(new sm64::menu::item::EnumFloat("MOUSE Y SCALER", { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f }, &sm64::config().camera().mouseyScaler()));
			m_items.push_back(new sm64::menu::item::Bool("MOUSE X INVERT", &sm64::config().camera().mousexInvert()));
			m_items.push_back(new sm64::menu::item::Bool("MOUSE Y INVERT", &sm64::config().camera().mouseyInvert()));
#endif

		}

		void render(const s16 x, const s16 y, const s16 yIndex = 15) override
		{
			print_hud_colorful_str(std::string("CAMERA"), DIALOG_TITLE_X, DIALOG_TITLE_Y);
			Dialog::render(x, y, yIndex);
		}
	};

	class Game : public Dialog
	{
	public:
		Game() : Dialog("GAME")
		{
#ifndef __SWITCH__
			m_items.push_back(new sm64::menu::item::Bool("FULL SCREEN", &sm64::config().game().fullscreen(), [](bool value) {
				sm64::gfx::set_fullscreen(value);
			}));
#endif
			m_items.push_back(new sm64::menu::item::Bool("MIRROR MODE", &sm64::config().game().setMirror()));
			m_items.push_back(new sm64::menu::item::Bool("DISABLE SOUND", &sm64::config().game().disableSound()));
		}

		void render(const s16 x, const s16 y, const s16 yIndex = 15) override
		{
			print_hud_colorful_str(std::string("GAME"), DIALOG_TITLE_X, DIALOG_TITLE_Y);
			Dialog::render(x, y, yIndex);
		}
	};

	class Credits : public Dialog
	{
		public:
		Credits() : Dialog("CREDITS")
		{
		}

		void render(const s16 x, const s16 y, const s16 yIndex = 15) override
		{
			static std::vector<hud::String> credits = {std::string("SM64 DECOMP TEAM"), std::string("ARREDONDO 3D MODELS"), std::string("SGI HD MODELS"), std::string("PIG REXXER MODELS"), std::string("HYPATIA TEXTURE PACK"), std::string("CLEANER AESTHETICS TEXTURES"), std::string("MANY MORE") };
			print_hud_colorful_str(std::string("CREDITS"), DIALOG_TITLE_X, DIALOG_TITLE_Y);

			gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
			gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);

			u32 yOffset = 2;
			for(u32 i = 0; i < credits.size(); i++)
			{
				print_generic_string(x + 10, y - yOffset, credits[i].buffer());
				yOffset += 15;
			}

			draw_pause_nav();

			// print_generic_string(x + 10, y - 33, textCameraAngleR);
			gSPDisplayList(gDisplayListHead++, dl_ia_text_end);

			create_dl_translation_matrix(MENU_MTX_PUSH, x - X_VAL8, (y - ((m_index - 1) * yIndex)) - Y_VAL8, 0);

			gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);
			gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
			gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
		}
	};
} // namespace sm64::menu

void render_pause_castle_menu_box(s16 x, s16 y)
{
	const s16 xOff = 78;
	const s16 yOff = 32;

	create_dl_translation_matrix(MENU_MTX_PUSH, x - xOff, y - yOff, 0);
	create_dl_scale_matrix(MENU_MTX_NOPUSH, 1.2f, 0.8f, 1.0f);
	gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 105);
	gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

	create_dl_translation_matrix(MENU_MTX_PUSH, x + 6, y - 28, 0);
	create_dl_rotation_matrix(MENU_MTX_NOPUSH, DEFAULT_DIALOG_BOX_ANGLE, 0, 0, 1.0f);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);
	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

	create_dl_translation_matrix(MENU_MTX_PUSH, x - 9, y - 101, 0);
	create_dl_rotation_matrix(MENU_MTX_NOPUSH, 270.0f, 0, 0, 1.0f);
	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

void highlight_last_course_complete_stars(void)
{
	u8 courseDone;

	if(gLastCompletedCourseNum == 0)
	{
		courseDone = 0;
	}
	else
	{
		courseDone = gLastCompletedCourseNum - 1;

		if(courseDone >= COURSE_STAGES_COUNT)
		{
			courseDone = COURSE_STAGES_COUNT;
		}
	}

	gDialogLineNum = courseDone;
}

void print_hud_pause_colorful_str(void)
{
	u8 textPause[] = {TEXT_PAUSE};

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);

	print_hud_lut_string(HUD_LUT_GLOBAL, 123, 81, textPause);

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
}

void render_pause_castle_course_stars(s16 x, s16 y, s16 fileNum, s16 courseNum)
{
	s16 hasStar = 0;

	u8 str[COURSE_STAGES_COUNT * 2];

	u8 textStar[] = {TEXT_STAR};

	u8 starFlags  = save_file_get_star_flags(fileNum, courseNum);
	u16 starCount = save_file_get_course_star_count(fileNum, courseNum);

	u16 nextStar = 0;

	if(starFlags & 0x40)
	{
		starCount--;
		print_generic_string(x + 89, y - 5, textStar);
	}

	while(hasStar != starCount)
	{
		if(starFlags & (1 << nextStar))
		{
			str[nextStar * 2] = DIALOG_CHAR_STAR_FILLED;
			hasStar++;
		}
		else
		{
			str[nextStar * 2] = DIALOG_CHAR_STAR_OPEN;
		}

		str[nextStar * 2 + 1] = DIALOG_CHAR_SPACE;
		nextStar++;
	}

	if(starCount == nextStar && starCount != 6)
	{
		str[nextStar * 2]     = DIALOG_CHAR_STAR_OPEN;
		str[nextStar * 2 + 1] = DIALOG_CHAR_SPACE;
		nextStar++;
	}

	str[nextStar * 2] = DIALOG_CHAR_TERMINATOR;

	print_generic_string(x + 14, y + 13, str);
}

void render_pause_castle_main_strings(s16 x, s16 y)
{
#ifdef VERSION_EU
	void** courseNameTbl;
#else
	void** courseNameTbl = (void**)segmented_to_virtual(seg2_course_name_table);
#endif

#ifdef VERSION_EU
	u8 textCoin[] = {TEXT_COIN};
	u8 textX[]    = {TEXT_VARIABLE_X};
#else
	u8 textCoin[] = {TEXT_COIN_X};
#endif

	void* courseName;

	u8 strVal[8];
	s16 starNum = gDialogLineNum;

#ifdef VERSION_EU
	switch(gInGameLanguage)
	{
		case LANGUAGE_ENGLISH:
			courseNameTbl = segmented_to_virtual(course_name_table_eu_en);
			break;
		case LANGUAGE_FRENCH:
			courseNameTbl = segmented_to_virtual(course_name_table_eu_fr);
			break;
		case LANGUAGE_GERMAN:
			courseNameTbl = segmented_to_virtual(course_name_table_eu_de);
			break;
	}
#endif

	handle_menu_scrolling(MENU_SCROLL_VERTICAL, &gDialogLineNum, -1, COURSE_STAGES_COUNT + 1);

	if(gDialogLineNum == COURSE_STAGES_COUNT + 1)
	{
		gDialogLineNum = 0;
	}

	if(gDialogLineNum == -1)
	{
		gDialogLineNum = COURSE_STAGES_COUNT;
	}

	if(gDialogLineNum != COURSE_STAGES_COUNT)
	{
		while(save_file_get_course_star_count(gCurrSaveFileNum - 1, gDialogLineNum) == 0)
		{
			if(gDialogLineNum >= starNum)
			{
				gDialogLineNum++;
			}
			else
			{
				gDialogLineNum--;
			}

			if(gDialogLineNum == COURSE_STAGES_COUNT || gDialogLineNum == -1)
			{
				gDialogLineNum = COURSE_STAGES_COUNT;
				break;
			}
		}
	}

	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);

	if(gDialogLineNum < COURSE_STAGES_COUNT)
	{
		courseName = segmented_to_virtual(courseNameTbl[gDialogLineNum]);
		render_pause_castle_course_stars(x, y, gCurrSaveFileNum - 1, gDialogLineNum);
		print_generic_string(x + 34, y - 5, textCoin);
#ifdef VERSION_EU
		print_generic_string(x + 44, y - 5, textX);
#endif
		int_to_str(save_file_get_course_coin_score(gCurrSaveFileNum - 1, gDialogLineNum), strVal);
		print_generic_string(x + 54, y - 5, strVal);
#ifdef VERSION_EU
		print_generic_string(x - 17, y + 30, courseName);
#endif
	}
	else
	{
		u8 textStarX[] = {TEXT_STAR_X};
		courseName     = segmented_to_virtual(courseNameTbl[COURSE_MAX]);
		print_generic_string(x + 40, y + 13, textStarX);
		int_to_str(save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_BONUS_STAGES - 1, COURSE_MAX - 1), strVal);
		print_generic_string(x + 60, y + 13, strVal);
#ifdef VERSION_EU
		print_generic_string(get_str_x_pos_from_center(x + 51, courseName, 10.0f), y + 30, courseName);
#endif
	}

#ifndef VERSION_EU
	print_generic_string(x - 9, y + 30, (const u8*)courseName);
#endif

	draw_pause_nav();

	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

s8 gCourseCompleteCoinsEqual = 0;
s32 gCourseDoneMenuTimer     = 0;
s32 gCourseCompleteCoins     = 0;
s8 gHudFlash		     = 0;
s8 subMenu		     = 0;

static std::vector<sm64::menu::Dialog*>& menus()
{
	static std::vector<sm64::menu::Dialog*> sMenus = { new sm64::menu::Game(), new sm64::menu::Camera(), new sm64::menu::Mods(), new sm64::menu::Cheats(), new sm64::menu::Credits() };
	return sMenus;
}



void draw_pause_nav()
{
	s16 x	 = 0;
	s16 y	 = 10;

	print_generic_string(x, y, R_NEXT.buffer());

	if(subMenu >= menus().size())
	{
		print_generic_string(x + 15, y, R_BACK.buffer());
	}
	else
	{
		print_generic_string(x + 15, y, menus()[subMenu]->title.buffer());
	}
}

s16 render_pause_courses_and_castle(void)
{
	s16 num;

	if(sm64::player(0).controller().buttonPressed & sm64::hid::R_TRIG)
	{
		subMenu++;
		subMenu = subMenu % (menus().size() + 1);
	}
	else if(sm64::player(0).controller().buttonPressed & sm64::hid::L_TRIG)
	{
		subMenu--;
		subMenu = subMenu % (menus().size() + 1);
	}

	switch(gDialogBoxState)
	{
		case DIALOG_STATE_OPENING:
			gDialogLineNum	 = 1;
			gDialogTextAlpha = 0;
			level_set_transition(-1, 0);

			play_sound(SOUND_MENU_PAUSE_HIGHPRIO, gDefaultSoundArgs);

			if(gCurrCourseNum >= COURSE_MIN && gCurrCourseNum <= COURSE_MAX)
			{
				change_dialog_camera_angle();
				gDialogBoxState = DIALOG_STATE_VERTICAL;
			}
			else
			{
				highlight_last_course_complete_stars();
				gDialogBoxState = DIALOG_STATE_HORIZONTAL;
			}
			break;
		case DIALOG_STATE_VERTICAL:
			shade_screen();

			if(gMarioStates[0].action & ACT_FLAG_PAUSE_EXIT)
			{
				if(!subMenu)
				{
					render_pause_my_score_coins();
					render_pause_red_coins();
					render_pause_course_options(99, 93, &gDialogLineNum, 15);
				}
				else if(subMenu - 1 < menus().size())
				{
					menus()[subMenu - 1]->render(99, 93 + 40);
				}
			}
			else if(subMenu && subMenu - 1 < menus().size())
			{
				menus()[subMenu - 1]->render(99, 93 + 40);
			}
			else
			{
				render_pause_my_score_coins();
				render_pause_red_coins();
			}

			if((sm64::player(0).controller().buttonPressed & sm64::hid::A_BUTTON && subMenu == 0) || sm64::player(0).controller().buttonPressed & sm64::hid::START_BUTTON)
			{
				level_set_transition(0, 0);
				play_sound(SOUND_MENU_PAUSE_2, gDefaultSoundArgs);
				gDialogBoxState = DIALOG_STATE_OPENING;
				gMenuMode	= -1;

				if(gDialogLineNum == 2)
				{
					num = gDialogLineNum;
				}
				else
				{
					num = 1;
				}

				return num;
			}
			break;
		case DIALOG_STATE_HORIZONTAL:
			shade_screen();

			if(!subMenu)
			{
				print_hud_pause_colorful_str();
				render_pause_castle_menu_box(160, 143);
				render_pause_castle_main_strings(104, 60);
			}
			else if(subMenu - 1 < menus().size())
			{
				menus()[subMenu - 1]->render(99, 93+40);
			}

			if((sm64::player(0).controller().buttonPressed & sm64::hid::A_BUTTON && subMenu == 0) || sm64::player(0).controller().buttonPressed & sm64::hid::START_BUTTON)
			{
				level_set_transition(0, 0);
				play_sound(SOUND_MENU_PAUSE_2, gDefaultSoundArgs);
				gMenuMode	= -1;
				gDialogBoxState = DIALOG_STATE_OPENING;

				return 1;
			}
			break;
	}

	if(gDialogTextAlpha < 250)
	{
		gDialogTextAlpha += 25 / FRAME_RATE_SCALER_INV;
	}

	return 0;
}

#ifdef VERSION_JP
#define TXT_HISCORE_X 112
#define TXT_HISCORE_Y 48
#define TXT_CONGRATS_X 60
#else
#define TXT_HISCORE_X 109
#define TXT_HISCORE_Y 36
#define TXT_CONGRATS_X 70
#endif

#define HUD_PRINT_HISCORE 0
#define HUD_PRINT_CONGRATULATIONS 1

void print_hud_course_complete_string(s8 str)
{
#ifdef VERSION_EU
	u8 textHiScore[][15]	     = {{TEXT_HUD_HI_SCORE}, {TEXT_HUD_HI_SCORE_FR}, {TEXT_HUD_HI_SCORE_DE}};
	u8 textCongratulations[][16] = {{TEXT_HUD_CONGRATULATIONS}, {TEXT_HUD_CONGRATULATIONS_FR}, {TEXT_HUD_CONGRATULATIONS_DE}};
#else
	u8 textHiScore[] = {TEXT_HUD_HI_SCORE};
	u8 textCongratulations[] = {TEXT_HUD_CONGRATULATIONS};
#endif

	u8 colorFade = sins(gDialogColorFadeTimer) * 50.0f + 200.0f;

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, colorFade, colorFade, colorFade, 255);

	if(str == HUD_PRINT_HISCORE)
	{
#ifdef VERSION_EU
		print_hud_lut_string(HUD_LUT_GLOBAL, get_str_x_pos_from_center_scale(160, textHiScore[gInGameLanguage], 12.0f), 36, textHiScore[gInGameLanguage]);
#else
		print_hud_lut_string(HUD_LUT_GLOBAL, TXT_HISCORE_X, TXT_HISCORE_Y, textHiScore);
#endif
	}
	else
	{ // HUD_PRINT_CONGRATULATIONS
#ifdef VERSION_EU
		print_hud_lut_string(HUD_LUT_GLOBAL, get_str_x_pos_from_center_scale(160, textCongratulations[gInGameLanguage], 12.0f), 67, textCongratulations[gInGameLanguage]);
#else
		print_hud_lut_string(HUD_LUT_GLOBAL, TXT_CONGRATS_X, 67, textCongratulations);
#endif
	}

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
}

void print_hud_course_complete_coins(s16 x, s16 y)
{
	u8 courseCompleteCoinsStr[4];
	u8 hudTextSymCoin[] = {GLYPH_COIN, GLYPH_SPACE};
	u8 hudTextSymX[]    = {GLYPH_MULTIPLY, GLYPH_SPACE};

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);

	print_hud_lut_string(HUD_LUT_GLOBAL, x, y, hudTextSymCoin);
	print_hud_lut_string(HUD_LUT_GLOBAL, x + 16, y, hudTextSymX);

	int_to_str(gCourseCompleteCoins, courseCompleteCoinsStr);
	print_hud_lut_string(HUD_LUT_GLOBAL, x + 32, y, courseCompleteCoinsStr);

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);

	if(gCourseCompleteCoins >= gHudDisplay.coins)
	{
		gCourseCompleteCoinsEqual = 1;
		gCourseCompleteCoins	  = gHudDisplay.coins;

		if(gGotFileCoinHiScore != 0)
		{
			print_hud_course_complete_string(HUD_PRINT_HISCORE);
		}
	}
	else
	{
		if((gCourseDoneMenuTimer & 1) || gHudDisplay.coins > 70)
		{
			gCourseCompleteCoins++;
			play_sound(SOUND_MENU_YOSHI_GAIN_LIVES, gDefaultSoundArgs);

			if(gCourseCompleteCoins == 50 || gCourseCompleteCoins == 100 || gCourseCompleteCoins == 150)
			{
				play_sound(SOUND_GENERAL_COLLECT_1UP, gDefaultSoundArgs);
				if(!sm64::config().cheats().unlimitedLives())
				{
					gMarioState[0].numLives++;
				}
			}
		}

		if(gHudDisplay.coins == gCourseCompleteCoins && gGotFileCoinHiScore != 0)
		{
			play_sound(SOUND_MENU_MARIO_CASTLE_WARP2, gDefaultSoundArgs);
		}
	}
}

void play_star_fanfare_and_flash_hud(s32 arg, u8 starNum)
{
	if(gHudDisplay.coins == gCourseCompleteCoins && (gCurrCourseStarFlags & starNum) == 0 && gHudFlash == 0)
	{
		play_star_fanfare();
		gHudFlash = arg;
	}
}

#ifdef VERSION_EU
#define TXT_NAME_X1 centerX
#define TXT_NAME_X2 centerX - 1
#else
#define TXT_NAME_X1 71
#define TXT_NAME_X2 69
#endif
#ifdef VERSION_JP
#define CRS_NUM_X2 95
#define CRS_NUM_X3 93
#define TXT_CLEAR_X1 205
#define TXT_CLEAR_X2 203
#else
#define CRS_NUM_X2 104
#define CRS_NUM_X3 102
#define TXT_CLEAR_X1 get_string_width(name) + 81
#define TXT_CLEAR_X2 get_string_width(name) + 79
#endif

void render_course_complete_lvl_info_and_hud_str(void)
{
#ifdef VERSION_JP
	u8 textSymStar[] = {GLYPH_STAR, GLYPH_SPACE};
	u8 textCourse[]	 = {TEXT_COURSE};
	u8 textCatch[]	 = {TEXT_CATCH};
	u8 textClear[]	 = {TEXT_CLEAR};
#elif defined(VERSION_EU)
	UNUSED u8 textClear[] = {TEXT_CLEAR}; // unused in EU
	u8 textSymStar[] = {GLYPH_STAR, GLYPH_SPACE};
#define textCourse gTextCourseArr[gInGameLanguage]
#else
	u8 textCourse[] = {TEXT_COURSE};
	UNUSED u8 textCatch[] = {TEXT_CATCH}; // unused in US
	UNUSED u8 textClear[] = {TEXT_CLEAR};
	u8 textSymStar[] = {GLYPH_STAR, GLYPH_SPACE};
#endif

	void** actNameTbl;
	void** courseNameTbl;
	u8* name;

	u8 strCourseNum[4];

#ifdef VERSION_EU
	s16 centerX;
	switch(gInGameLanguage)
	{
		case LANGUAGE_ENGLISH:
			actNameTbl    = segmented_to_virtual(act_name_table_eu_en);
			courseNameTbl = segmented_to_virtual(course_name_table_eu_en);
			break;
		case LANGUAGE_FRENCH:
			actNameTbl    = segmented_to_virtual(act_name_table_eu_fr);
			courseNameTbl = segmented_to_virtual(course_name_table_eu_fr);
			break;
		case LANGUAGE_GERMAN:
			actNameTbl    = segmented_to_virtual(act_name_table_eu_de);
			courseNameTbl = segmented_to_virtual(course_name_table_eu_de);
			break;
	}
#else
	actNameTbl = (void**)segmented_to_virtual(seg2_act_name_table);
	courseNameTbl = (void**)segmented_to_virtual(seg2_course_name_table);
#endif

	if(gLastCompletedCourseNum <= COURSE_STAGES_MAX)
	{
		print_hud_course_complete_coins(118, 103);
		play_star_fanfare_and_flash_hud(1, 1 << (gLastCompletedStarNum - 1));

		if(gLastCompletedStarNum == 7)
		{
			name = (u8*)segmented_to_virtual(actNameTbl[COURSE_STAGES_MAX * 6 + 1]);
		}
		else
		{
			name = (u8*)segmented_to_virtual(actNameTbl[(gLastCompletedCourseNum - 1) * 6 + gLastCompletedStarNum - 1]);
		}

		gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
		int_to_str(gLastCompletedCourseNum, strCourseNum);
		gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, gDialogTextAlpha);
		print_generic_string(65, 165, textCourse);
		print_generic_string(CRS_NUM_X2, 165, strCourseNum);
		gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);
		print_generic_string(63, 167, textCourse);
		print_generic_string(CRS_NUM_X3, 167, strCourseNum);
		gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
	}
	else if(gLastCompletedCourseNum == COURSE_BITDW || gLastCompletedCourseNum == COURSE_BITFS)
	{
		name = (u8*)segmented_to_virtual(courseNameTbl[gLastCompletedCourseNum - 1]);
		gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
		gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, gDialogTextAlpha);
#ifdef VERSION_EU
		centerX = get_str_x_pos_from_center(153, name, 12.0f);
#endif
		print_generic_string(TXT_NAME_X1, 130, name);
#ifndef VERSION_EU
		print_generic_string(TXT_CLEAR_X1, 130, textClear);
#endif
		gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);
		print_generic_string(TXT_NAME_X2, 132, name);
#ifndef VERSION_EU
		print_generic_string(TXT_CLEAR_X2, 132, textClear);
#endif
		gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
		print_hud_course_complete_string(HUD_PRINT_CONGRATULATIONS);
		print_hud_course_complete_coins(118, 111);
		play_star_fanfare_and_flash_hud(2, 0); //! 2 isn't defined, originally for key hud?
		return;
	}
	else
	{
		name = (u8*)segmented_to_virtual(actNameTbl[COURSE_STAGES_MAX * 6]);
		print_hud_course_complete_coins(118, 103);
		play_star_fanfare_and_flash_hud(1, 1 << (gLastCompletedStarNum - 1));
	}

	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);
	print_hud_lut_string(HUD_LUT_GLOBAL, 55, 77, textSymStar);
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, gDialogTextAlpha);
	print_generic_string(76, 145, name);
#ifdef VERSION_JP
	print_generic_string(220, 145, textCatch);
#endif
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);
	print_generic_string(74, 147, name);
#ifdef VERSION_JP
	print_generic_string(218, 147, textCatch);
#endif
	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

#define TXT_SAVEOPTIONS_X x + 12
#define TXT_SAVECONT_Y 0
#define TXT_SAVEQUIT_Y 20
#define TXT_CONTNOSAVE_Y 40

#define X_VAL9 x
void render_save_confirmation(s16 x, s16 y, s8* index, s16 sp6e)

{
	u8 textSaveAndContinue[]     = {TEXT_SAVE_AND_CONTINUE};
	u8 textSaveAndQuit[]	     = {TEXT_SAVE_AND_QUIT};
	u8 textContinueWithoutSave[] = {TEXT_CONTINUE_WITHOUT_SAVING};

	handle_menu_scrolling(MENU_SCROLL_VERTICAL, index, 1, 3);

	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);

	print_generic_string(TXT_SAVEOPTIONS_X, y + TXT_SAVECONT_Y, textSaveAndContinue);
	print_generic_string(TXT_SAVEOPTIONS_X, y - TXT_SAVEQUIT_Y, textSaveAndQuit);
	print_generic_string(TXT_SAVEOPTIONS_X, y - TXT_CONTNOSAVE_Y, textContinueWithoutSave);

	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);

	create_dl_translation_matrix(MENU_MTX_PUSH, X_VAL9, y - ((index[0] - 1) * sp6e), 0);

	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);
	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);

	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

s16 render_course_complete_screen(void)
{
	s16 num;
#ifdef VERSION_EU
	gInGameLanguage = eu_get_language();
#endif

	switch(gDialogBoxState)
	{
		case DIALOG_STATE_OPENING:
			render_course_complete_lvl_info_and_hud_str();
			if(gCourseDoneMenuTimer > 100 && gCourseCompleteCoinsEqual == 1)
			{
				gDialogBoxState = DIALOG_STATE_VERTICAL;
				level_set_transition(-1, 0);
				gDialogTextAlpha = 0;
				gDialogLineNum	 = 1;
			}
			break;
		case DIALOG_STATE_VERTICAL:
			shade_screen();
			render_course_complete_lvl_info_and_hud_str();

			render_save_confirmation(100, 86, &gDialogLineNum, 20);

			if(gCourseDoneMenuTimer > 110 * FRAME_RATE_SCALER_INV && (sm64::player(0).controller().buttonPressed & sm64::hid::A_BUTTON || sm64::player(0).controller().buttonPressed & sm64::hid::START_BUTTON))
			{
				level_set_transition(0, 0);
				play_sound(SOUND_MENU_STAR_SOUND, gDefaultSoundArgs);
				gDialogBoxState		  = DIALOG_STATE_OPENING;
				gMenuMode		  = -1;
				num			  = gDialogLineNum;
				gCourseDoneMenuTimer	  = 0;
				gCourseCompleteCoins	  = 0;
				gCourseCompleteCoinsEqual = 0;
				gHudFlash		  = 0;

				return num;
			}
			break;
	}

	if(gDialogTextAlpha < 250)
	{
		gDialogTextAlpha += 25 / FRAME_RATE_SCALER_INV;
	}

	gCourseDoneMenuTimer++;

	return 0;
}

// Only case 1 and 2 are used
s16 render_menus_and_dialogs()
{
	s16 mode = 0;

	create_dl_ortho_matrix();

	if(gMenuMode != -1)
	{
		switch(gMenuMode)
		{
			case 0:
				mode = render_pause_courses_and_castle();
				break;
			case 1:
				mode = render_pause_courses_and_castle();
				break;
			case 2:
				mode = render_course_complete_screen();
				break;
			case 3:
				mode = render_course_complete_screen();
				break;
		}

		gDialogColorFadeTimer += 0x1000 * FRAME_RATE_SCALER_INV;
	}
	else if(gDialogID != -1)
	{
		// Peach message "Dear Mario" new game dialog
		if(gDialogID == 20)
		{
			print_peach_letter_message(); // the peach message needs to be
						      // repositioned seperately
			return mode;
		}

		render_dialog_entries();
		gDialogColorFadeTimer += 0x1000 * FRAME_RATE_SCALER_INV;
	}
	return mode;
}
