void ClearEndingMessage()
{
	gCutsceneMsgFade = 0;
}

void BeginEndingMessage()
{
	gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gCutsceneMsgFade);
}

void EndEndingMessage()
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

u8 ConvAscii(u8 c)
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

void EndingMessage(s16 x, s16 y, const char* str)
{
	s32 pos = 0;
	u8 c	= str[pos];
	u8 creditStr[100];

	while(c != 0)
	{
		creditStr[pos++] = ConvAscii(c);
		c		 = str[pos];
	}

	creditStr[pos] = GLOBAR_CHAR_TERMINATOR;

	print_credits_string(x, y, creditStr);
}

void CallEndingDemoMessage(s16 xOffset, s16 yOffset, s16 msgIndex, Duration msgDuration)
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

void EndingDemoMessageEvent()
{
	s16 x;

	// is a cutscene playing? do not perform this handler's actions if so.
	if(gCutsceneMsgIndex == -1)
	{
		return;
	}

	MakeSelectProjection();

	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gCutsceneMsgFade);

	// get the x coordinate of where the cutscene string starts.
	x = CharCentering(gCutsceneMsgXOffset, gEndCutsceneStringsEn[gCutsceneMsgIndex], 10.0f);

	Draw8bitFont(x, 240 - gCutsceneMsgYOffset, gEndCutsceneStringsEn[gCutsceneMsgIndex]);

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
void OpeningMessageEvent()
{
	void** dialogTable;
	struct DialogEntry* dialog;
	u8* str;
	dialogTable = (void**)segmented_to_virtual(seg2_dialog_table);
	dialog	    = (DialogEntry*)segmented_to_virtual(dialogTable[gDialogID]);

	str = (u8*)segmented_to_virtual(dialog->str);

	iTranslate(MENU_MTX_PUSH, 97.0f, 118.0f, 0);

	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gCutsceneMsgFade);
	gSPDisplayList(gDisplayListHead++, castle_grounds_seg7_dl_0700EA58);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	gDPSetEnvColor(gDisplayListHead++, 20, 20, 20, gCutsceneMsgFade);

	Draw8bitFont(STR_X, STR_Y, str);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
	gDPSetEnvColor(gDisplayListHead++, 200, 80, 120, gCutsceneMsgFade);
	gSPDisplayList(gDisplayListHead++, castle_grounds_seg7_us_dl_0700F2E8);

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
void CannonSightEvent()
{
	iTranslate(MENU_MTX_PUSH, 160.0f, 120.0f, 0);

	gDPSetEnvColor(gDisplayListHead++, 50, 50, 50, 180);
	iTranslate(MENU_MTX_PUSH, -20.0f, -8.0f, 0);
	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

	iTranslate(MENU_MTX_PUSH, 20.0f, 8.0f, 0);
	iRotate(MENU_MTX_NOPUSH, 180.0f, 0, 0, 1.0f);
	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

	iTranslate(MENU_MTX_PUSH, 8.0f, -20.0f, 0);
	iRotate(MENU_MTX_NOPUSH, DEFAULT_DIALOG_BOX_ANGLE, 0, 0, 1.0f);
	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

	iTranslate(MENU_MTX_PUSH, -8.0f, 20.0f, 0);
	iRotate(MENU_MTX_NOPUSH, -DEFAULT_DIALOG_BOX_ANGLE, 0, 0, 1.0f);
	gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}