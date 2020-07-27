void iwa_LoadImatrix()
{
	Mtx* matrix = (Mtx*)AllocDynamic(sizeof(Mtx));

	if(matrix == NULL)
	{
		return;
	}

	guMtxIdent(matrix);

	gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
	gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
}

void iTranslate(s8 pushOp, f32 x, f32 y, f32 z)
{
	Mtx* matrix = (Mtx*)AllocDynamic(sizeof(Mtx));

	if(matrix == NULL)
	{
		return;
	}

	guTranslate(matrix, x, y, z);

	if(pushOp == MENU_MTX_PUSH)
	{
		gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
	}
	else if(pushOp == MENU_MTX_NOPUSH)
	{
		gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
	}
}

void iRotate(s8 pushOp, f32 a, f32 x, f32 y, f32 z)
{
	Mtx* matrix = (Mtx*)AllocDynamic(sizeof(Mtx));

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

void iScale(s8 pushOp, f32 x, f32 y, f32 z)
{
	Mtx* matrix = (Mtx*)AllocDynamic(sizeof(Mtx));

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

void MakeSelectProjection()
{
	Mtx* matrix = (Mtx*)AllocDynamic(sizeof(Mtx));

	if(matrix == NULL)
	{
		return;
	}

	iwa_LoadImatrix();

	guOrtho(matrix, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, -10.0f, 10.0f, 1.0f);

	// Should produce G_RDPHALF_1 in Fast3D
	gSPPerspNormalize((Gfx*)(gDisplayListHead++), 0xFFFF);

	gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH)
}

static u8* ImageDefrost(u16* in, s16 width, s16 height)
{
	s32 inPos;
	u16 bitMask;
	u8* out;
	s16 outPos;

	outPos = 0;
	out    = (u8*)AllocDynamic(width * height);

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

void DrawMessageTexture(u8 c)
{
	void** fontLUT;
	void* packedTexture;

	fontLUT	      = (void**)segmented_to_virtual(main_font_lut);
	packedTexture = segmented_to_virtual(fontLUT[c]);

	gDPPipeSync(gDisplayListHead++);
	gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, VIRTUAL_TO_PHYSICAL(packedTexture));
	gSPDisplayList(gDisplayListHead++, dl_ia_text_tex_settings);
}

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

void render_multi_text_string(s8 multiTextID) // US: 802D76C8
{
	s8 i;
	struct MultiTextEntry textLengths[2] = {
	    {3, {TEXT_THE_RAW}},
	    {3, {TEXT_YOU_RAW}},
	};

	for(i = 0; i < textLengths[multiTextID].length; i++)
	{
		DrawMessageTexture(textLengths[multiTextID].str[i]);
		iTranslate(MENU_MTX_NOPUSH, (f32)(gDialogCharWidths[textLengths[multiTextID].str[i]]), 0.0f, 0.0f);
	}
}

#define MAX_STRING_WIDTH 16

/**
 * Prints a generic white string.
 * In JP/EU a IA1 texture is used but in US a IA4 texture is used.
 */
void Draw8bitFont(s16 x, s16 y, const u8* str)
{
	UNUSED s8 mark = DIALOG_MARK_NONE; // size in EU
	s32 strPos     = 0;
	u8 lineNum     = 1;

	iTranslate(MENU_MTX_PUSH, x, y, 0.0f);

	while(str[strPos] != DIALOG_CHAR_TERMINATOR)
	{
		switch(str[strPos])
		{
			case DIALOG_CHAR_DAKUTEN:
				mark = DIALOG_MARK_DAKUTEN;
				break;
			case DIALOG_CHAR_PERIOD_OR_HANDAKUTEN:
				mark = DIALOG_MARK_HANDAKUTEN;
				break;
			case DIALOG_CHAR_NEWLINE:
				gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
				iTranslate(MENU_MTX_PUSH, x, y - (lineNum * MAX_STRING_WIDTH), 0.0f);
				lineNum++;
				break;
			case DIALOG_CHAR_PERIOD:
				iTranslate(MENU_MTX_PUSH, -2.0f, -5.0f, 0.0f);
				DrawMessageTexture(DIALOG_CHAR_PERIOD_OR_HANDAKUTEN);
				gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
				break;

			case DIALOG_CHAR_SLASH:
				iTranslate(MENU_MTX_NOPUSH, (f32)(gDialogCharWidths[DIALOG_CHAR_SPACE] * 2), 0.0f, 0.0f);
				break;
			case DIALOG_CHAR_MULTI_THE:
				render_multi_text_string(STRING_THE);
				break;
			case DIALOG_CHAR_MULTI_YOU:
				render_multi_text_string(STRING_YOU);
				break;
			case DIALOG_CHAR_SPACE:
				iTranslate(MENU_MTX_NOPUSH, (f32)(gDialogCharWidths[DIALOG_CHAR_SPACE]), 0.0f, 0.0f);
				break; // ? needed to match
			default:
				DrawMessageTexture(str[strPos]);
				if(mark != DIALOG_MARK_NONE)
				{
					iTranslate(MENU_MTX_PUSH, 5.0f, 5.0f, 0.0f);
					DrawMessageTexture(mark + 0xEF);
					gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
					mark = DIALOG_MARK_NONE;
				}

				iTranslate(MENU_MTX_NOPUSH, (f32)(gDialogCharWidths[str[strPos]]), 0.0f, 0.0f);
				break; // what an odd difference. US added a useless break here.
		}

		strPos++;
	}

	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

/**
 * Prints a hud string depending of the hud table list defined.
 */
void Draw16bitFont(s8 hudLUT, s16 x, s16 y, const u8* str)
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
		xStride = 12;
	}

	while(str[strPos] != GLOBAR_CHAR_TERMINATOR)
	{
		if(str[strPos] == GLOBAL_CHAR_SPACE)
		{
			if(0) //! dead code
			{
			}
			curX += 8;
		}
		else
		{
			gDPPipeSync(gDisplayListHead++);

			if(hudLUT == HUD_LUT_JPMENU)
				gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, hudLUT1[str[strPos]]);

			if(hudLUT == HUD_LUT_GLOBAL)
				gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, hudLUT2[str[strPos]]);

			gSPDisplayList(gDisplayListHead++, dl_rgba16_load_tex_block);
			gSPTextureRectangle(gDisplayListHead++, curX << 2, curY << 2, (curX + 16) << 2, (curY + 16) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);

			curX += xStride;
		}
		strPos++;
	}
}

void print_menu_generic_string(s16 x, s16 y, const u8* str)
{
	UNUSED s8 mark = DIALOG_MARK_NONE; // size in EU
	s32 strPos     = 0;
	s32 curX       = x;
	s32 curY       = y;
	void** fontLUT = (void**)segmented_to_virtual(menu_font_lut);

	while(str[strPos] != DIALOG_CHAR_TERMINATOR)
	{
		switch(str[strPos])
		{
			case DIALOG_CHAR_DAKUTEN:
				mark = DIALOG_MARK_DAKUTEN;
				break;
			case DIALOG_CHAR_PERIOD_OR_HANDAKUTEN:
				mark = DIALOG_MARK_HANDAKUTEN;
				break;
			case DIALOG_CHAR_SPACE:
				curX += 4;
				break;
			default:
				gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_8b, 1, fontLUT[str[strPos]]);
				gDPLoadSync(gDisplayListHead++);
				gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 8 * 8 - 1, CALC_DXT(8, G_IM_SIZ_8b_BYTES));
				gSPTextureRectangle(gDisplayListHead++, curX << 2, curY << 2, (curX + 8) << 2, (curY + 8) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);

				if(mark != DIALOG_MARK_NONE)
				{
					gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_8b, 1, fontLUT[mark + 0xEF]);
					gDPLoadSync(gDisplayListHead++);
					gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 8 * 8 - 1, CALC_DXT(8, G_IM_SIZ_8b_BYTES));
					gSPTextureRectangle(gDisplayListHead++, (curX + 6) << 2, (curY - 7) << 2, (curX + 14) << 2, (curY + 1) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);

					mark = DIALOG_MARK_NONE;
				}
				curX += gDialogCharWidths[str[strPos]];
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
