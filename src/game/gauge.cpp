#include <ultra64.h>

#include "sm64.h"
#include "gfx_dimensions.h"
#include "display.h"
#include "game.h"
#include "level_update.h"
#include "camera.h"
#include "print.h"
#include "ingame_menu.h"
#include "hud.h"
#include "segment2.h"
#include "area.h"
#include "save_file.h"
#include "print.h"
#include "hud.h"

/* @file hud.c
 * This file implements HUD rendering and power meter animations.
 * That includes stars, lives, coins, camera status, power meter, timer
 * cannon reticle, and the size keys.
 **/

struct PowerMeterHUD
{
	s8 flag;
	s16 x;
	s16 y;
	f32 size;
};

struct UnusedStruct803314F0
{
	u32 unused1;
	u16 unused2;
	u16 unused3;
};

struct CameraHUD
{
	s16 status;
};

// Stores health segmented value defined by numHealthWedges
// When the HUD is rendered this value is 8, full health.
static s16 be_dameter;

static PowerMeterHUD meter = {
    POWER_METER_HIDDEN,
    140,
    166,
    1.0,
};

// Power Meter timer that keeps counting when it's visible.
// Gets reset when the health is filled and stops counting
// when the power meter is hidden.
s32 count = 0;

static struct UnusedStruct803314F0 unused803314F0 = {0x00000000, 0x000A, 0x0000};

static struct CameraHUD sCameraHUD = {CAM_STATUS_NONE};

/**
 * Renders a rgba16 16x16 glyph texture from a table list.
 */
void Copy16BitTexture(s32 x, s32 y, u16* texture)
{
	gDPPipeSync(gDisplayListHead++);
	gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture);
	gSPDisplayList(gDisplayListHead++, &RCP_tfont_main);
	gSPTextureRectangle(gDisplayListHead++, x << 2, y << 2, (x + 15) << 2, (y + 15) << 2, G_TX_RENDERTILE, 0, 0, 4 << 10, 1 << 10);
}

/**
 * Renders a rgba16 8x8 glyph texture from a table list.
 */
void Copy88Texture(s32 x, s32 y, u16* texture)
{
	gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD);
	gDPTileSync(gDisplayListHead++);
	gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 2, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 3, G_TX_NOLOD, G_TX_CLAMP, 3, G_TX_NOLOD);
	gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, (8 - 1) << G_TEXTURE_IMAGE_FRAC, (8 - 1) << G_TEXTURE_IMAGE_FRAC);
	gDPPipeSync(gDisplayListHead++);
	gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture);
	gDPLoadSync(gDisplayListHead++);
	gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 8 * 8 - 1, CALC_DXT(8, G_IM_SIZ_16b_BYTES));
	gSPTextureRectangle(gDisplayListHead++, x << 2, y << 2, (x + 7) << 2, (y + 7) << 2, G_TX_RENDERTILE, 0, 0, 4 << 10, 1 << 10);
}

/**
 * Renders power meter health segment texture using a table list.
 */
static void LoadGaugeTexture(s16 numHealthWedges)
{
	u16** texture = (u16**)&power_meter_health_segments_lut;

	gDPPipeSync(gDisplayListHead++);
	gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture[numHealthWedges - 1]);
	gDPLoadSync(gDisplayListHead++);
	gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 32 * 32 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES));
	gSP1Triangle(gDisplayListHead++, 0, 1, 2, 0);
	gSP1Triangle(gDisplayListHead++, 0, 2, 3, 0);
}

/**
 * Renders power meter display lists.
 * That includes the "POWER" base and the colored health segment textures.
 */
static void DrawDamegeGage(s16 numHealthWedges)
{
	Mtx* mtx = (Mtx*)AllocDynamic(sizeof(Mtx));

	if(mtx == NULL)
	{
		return;
	}

	guTranslate(mtx, (f32)meter.x, (f32)meter.y, 0);

	gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(mtx++), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
	gSPDisplayList(gDisplayListHead++, &dl_power_meter_base);

	if(numHealthWedges != 0)
	{
		gSPDisplayList(gDisplayListHead++, &dl_power_meter_health_segments_begin);
		LoadGaugeTexture(numHealthWedges);
		gSPDisplayList(gDisplayListHead++, &dl_power_meter_health_segments_end);
	}

	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

/**
 * Power meter animation called when there's less than 8 health segments
 * Checks its timer to later change into deemphasizing mode.
 */
static void GaugeOpen()
{
	s16 hudDisplayFlags;
	hudDisplayFlags = playerMeter.flags;

	if(!(hudDisplayFlags & HUD_DISPLAY_FLAG_EMPHASIZE_POWER))
	{
		if(count == 45.0)
		{
			meter.flag = POWER_METER_DEEMPHASIZING;
		}
	}
	else
	{
		count = 0;
	}
}

/**
 * Power meter animation called after emphasized mode.
 * Moves power meter y pos speed until it's at 200 to be visible.
 */
static void GaugeUp()
{
	s16 speed = 5;

	if(meter.y >= 181)
	{
		speed = 3;
	}

	if(meter.y >= 191)
	{
		speed = 2;
	}

	if(meter.y >= 196)
	{
		speed = 1;
	}

	meter.y += speed;

	if(meter.y >= 201)
	{
		meter.y	   = 200;
		meter.flag = POWER_METER_VISIBLE;
	}
}

/**
 * Power meter animation called when there's 8 health segments.
 * Moves power meter y pos quickly until it's at 301 to be hidden.
 */
static void GaugeClose()
{
	meter.y += 20;
	if(meter.y >= 301)
	{
		meter.flag = POWER_METER_HIDDEN;
		count	   = 0;
	}
}

/**
 * Handles power meter actions depending of the health segments values.
 */
static void DamegeStateCheck(s16 numHealthWedges)
{
	// Show power meter if health is not full, less than 8
	if(numHealthWedges < 8 && be_dameter == 8 && meter.flag == POWER_METER_HIDDEN)
	{
		meter.flag = POWER_METER_EMPHASIZED;
		meter.y	   = 166;
	}

	// Show power meter if health is full, has 8
	if(numHealthWedges == 8 && be_dameter == 7)
	{
		count = 0;
	}

	// After health is full, hide power meter
	if(numHealthWedges == 8 && count > 45.0)
	{
		meter.flag = POWER_METER_HIDING;
	}

	// Update to match health value
	be_dameter = numHealthWedges;

	// If mario is swimming, keep showing power meter
	if(camPlayerInfo->action & ACT_FLAG_SWIMMING)
	{
		if(meter.flag == POWER_METER_HIDDEN || meter.flag == POWER_METER_EMPHASIZED)
		{
			meter.flag = POWER_METER_DEEMPHASIZING;
			meter.y	   = 166;
		}
		count = 0;
	}
}

/**
 * Renders the power meter that shows when Mario is in underwater
 * or has taken damage and has less than 8 health segments.
 * And calls a power meter animation function depending of the value defined.
 */
static void DamegeGaugeEvent()
{
	s16 shownHealthWedges = playerMeter.wedges;

	if(meter.flag != POWER_METER_HIDING)
	{
		DamegeStateCheck(shownHealthWedges);
	}

	if(meter.flag == POWER_METER_HIDDEN)
	{
		return;
	}

	switch(meter.flag)
	{
		case POWER_METER_EMPHASIZED:
			GaugeOpen();
			break;
		case POWER_METER_DEEMPHASIZING:
			GaugeUp();
			break;
		case POWER_METER_HIDING:
			GaugeClose();
			break;
		default:
			break;
	}

	DrawDamegeGage(shownHealthWedges);

	count += 1;
}

#define HUD_TOP_Y 209

/**
 * Renders the amount of lives Mario has.
 */
static void PlayerLifeEvent()
{
	dmprintf(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(22), HUD_TOP_Y, ","); // 'Mario Head' glyph
	dmprintf(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(38), HUD_TOP_Y, "*"); // 'X' glyph
	dprintf(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(54), HUD_TOP_Y, "%d", playerMeter.lives);
}

/**
 * Renders the amount of coins collected.
 */
static void CoinNumBerEvent()
{
	dmprintf(168, HUD_TOP_Y, "+"); // 'Coin' glyph
	dmprintf(184, HUD_TOP_Y, "*"); // 'X' glyph
	dprintf(198, HUD_TOP_Y, "%d", playerMeter.coins);
}

#define HUD_STARS_X 78

/**
 * Renders the amount of stars collected.
 * Disables "X" glyph when Mario has 100 stars or more.
 */
static void StarNumBerEvent()
{
	s8 showX = 0;

	if(getStar_flag == 1 && frameCounter & (0x08 * FRAME_RATE_SCALER_INV))
	{
		return;
	}

	if(playerMeter.stars < 100)
	{
		showX = 1;
	}

	dmprintf(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(HUD_STARS_X), HUD_TOP_Y, "-"); // 'Star' glyph
	if(showX == 1)
	{
		dmprintf(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(HUD_STARS_X) + 16, HUD_TOP_Y, "*"); // 'X' glyph
	}
	dprintf((showX * 14) + GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(HUD_STARS_X - 16), HUD_TOP_Y, "%d", playerMeter.stars);
}

/**
 * Unused function that renders the amount of keys collected.
 * Leftover function from the beta version of the game.
 */
static void KeyNumBerEvent()
{
	s16 i;

	for(i = 0; i < playerMeter.keys; i++)
	{
		dmprintf((i * 16) + 220, 142, "/"); // size glyph - beta key
	}
}

/**
 * Renders the timer when Mario start sliding in PSS.
 */
static void TimerEvent()
{
	u16** texture	   = (u16**)&main_hud_lut;
	u16 timerValFrames = playerMeter.timer;

	u16 timerMins = timerValFrames / (30 * 60);
	u16 timerSecs = (timerValFrames - (timerMins * 1800)) / 30;

	u16 timerFracSecs = ((timerValFrames - (timerMins * 1800) - (timerSecs * 30)) & 0xFFFF) / 3;

	dmprintf(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(150), 185, "TIME");
	dprintf(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(91), 185, "%0d", timerMins);
	dprintf(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(71), 185, "%02d", timerSecs);
	dprintf(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(37), 185, "%d", timerFracSecs);
	gSPDisplayList(gDisplayListHead++, RCP_tfont_on);
	Copy16BitTexture(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(81), 32, texture[GLYPH_APOSTROPHE]);
	Copy16BitTexture(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(46), 32, texture[GLYPH_DOUBLE_QUOTE]);
	gSPDisplayList(gDisplayListHead++, RCP_tfont_off);
}

/**
 * Sets HUD status camera value depending of the actions
 * defined in update_camera_status.
 */
void CameraIconSet(s16 flag)
{
	sCameraHUD.status = flag;
}

enum CameraHUDLut
{
	GLYPH_CAM_CAMERA,
	GLYPH_CAM_MARIO_HEAD,
	GLYPH_CAM_LAKITU_HEAD,
	GLYPH_CAM_FIXED,
	GLYPH_CAM_ARROW_UP,
	GLYPH_CAM_ARROW_DOWN
};

/**
 * Renders camera HUD glyphs using a table list, depending of
 * the camera status called, a defined glyph is rendered.
 */
void CameraIconEvent()
{
	u16** texture = (u16**)&main_hud_camera_lut;
	s32 x	      = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(54);
	s32 y	      = 205;

	if(sCameraHUD.status == CAM_STATUS_NONE)
	{
		return;
	}

	gSPDisplayList(gDisplayListHead++, RCP_tfont_on);
	Copy16BitTexture(x, y, texture[GLYPH_CAM_CAMERA]);

	switch(sCameraHUD.status & CAM_STATUS_MODE_GROUP)
	{
		case CAM_STATUS_MARIO:
			Copy16BitTexture(x + 16, y, texture[GLYPH_CAM_MARIO_HEAD]);
			break;
		case CAM_STATUS_LAKITU:
			Copy16BitTexture(x + 16, y, texture[GLYPH_CAM_LAKITU_HEAD]);
			break;
		case CAM_STATUS_FIXED:
			Copy16BitTexture(x + 16, y, texture[GLYPH_CAM_FIXED]);
			break;
	}

	switch(sCameraHUD.status & CAM_STATUS_C_MODE_GROUP)
	{
		case CAM_STATUS_C_DOWN:
			Copy88Texture(x + 4, y + 16, texture[GLYPH_CAM_ARROW_DOWN]);
			break;
		case CAM_STATUS_C_UP:
			Copy88Texture(x + 4, y - 8, texture[GLYPH_CAM_ARROW_UP]);
			break;
	}

	gSPDisplayList(gDisplayListHead++, RCP_tfont_off);
}

/**
 * Render HUD strings using hudDisplayFlags with it's render functions,
 * excluding the cannon reticle which detects a camera preset for it.
 */
void MeterGaugeEvent()
{
	s16 flags = playerMeter.flags;

	if(flags == HUD_DISPLAY_NONE)
	{
		meter.flag = POWER_METER_HIDDEN;
		be_dameter = 8;
		count	   = 0;
	}
	else
	{
		MakeSelectProjection();

		if(snSceneInfo != NULL && snSceneInfo->camera->mode == CAMERA_MODE_INSIDE_CANNON)
		{
			CannonSightEvent();
		}

		if(flags & HUD_DISPLAY_FLAG_LIVES)
		{
			PlayerLifeEvent();
		}

		if(flags & HUD_DISPLAY_FLAG_COIN_COUNT)
		{
			CoinNumBerEvent();
		}

		if(flags & HUD_DISPLAY_FLAG_STAR_COUNT)
		{
			StarNumBerEvent();
		}

		if(flags & HUD_DISPLAY_FLAG_KEYS)
		{
			KeyNumBerEvent();
		}

		if(flags & HUD_DISPLAY_FLAG_CAMERA_AND_POWER)
		{
			DamegeGaugeEvent();
			CameraIconEvent();
		}

		if(flags & HUD_DISPLAY_FLAG_TIMER)
		{
			TimerEvent();
		}
	}
}
