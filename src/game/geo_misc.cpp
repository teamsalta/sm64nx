#include <ultra64.h>

#include "sm64.h"
#include "geo_misc.h"

#include "area.h"
#include "engine/math_util.h"
#include "level_update.h"
#include "mario_actions_cutscene.h"
#include "mario.h"
#include "memory.h"
#include "rendering_graph_node.h"
#include "save_file.h"
#include "segment2.h"
#include "levels/header.h"

/**
 * @file geo_misc.c
 * This file contains miscellaneous geo_asm scripts.
 *
 * In particular, it builds:
 *   - the light that shows the player where to look for Tower of the Wing Cap,
 *   - the flying carpets seen in Rainbow Ride, and
 *   - the end screen displaying Peach's delicious cake.
 */

#define NUM_FLYING_CARPET_VERTICES 21

static s16 sCurAreaTimer	    = 1;
static s16 sPrevAreaTimer	    = 0;
static s16 sFlyingCarpetRippleTimer = 0;

s8 gFlyingCarpetState;

/**
 * Create a vertex with the given parameters and insert it into `vtx` at
 * position `n`.
 *
 * Texture coordinates are s10.5 fixed-point, which means you should left-shift the actual coordinates by 5.
 */
void Tani_SetOneVtxData(Vtx* vtx, s32 n, f32 x, f32 y, f32 z, s16 tx, s16 ty, u8 r, u8 g, u8 b, u8 a)
{
	vtx[n].v.ob[0] = x;
	vtx[n].v.ob[1] = y;
	vtx[n].v.ob[2] = z;

	vtx[n].v.flag = 0;

	vtx[n].v.tc[0] = tx;
	vtx[n].v.tc[1] = ty;

	vtx[n].v.cn[0] = r;
	vtx[n].v.cn[1] = g;
	vtx[n].v.cn[2] = b;
	vtx[n].v.cn[3] = a;
}

/**
 * Round `num` to the nearest `s16`.
 */
s16 Tani_RoundOff(f32 num)
{
	// Note that double literals are used here, rather than float literals.
	if(num >= 0.0)
	{
		return num + 0.5;
	}
	else
	{
		return num - 0.5;
	}
}

/**
 * Create a display list for the light in the castle lobby that shows the
 * player where to look to enter Tower of the Wing Cap.
 */
Gfx* SelRoomEffect(s32 callContext, struct GraphNode* node, UNUSED f32 mtx[4][4])
{
	s32 flags;
	struct GraphNodeGenerated* generatedNode;
	Gfx* displayListHead = NULL;
	Gfx* displayList     = NULL;

	if(callContext == GEO_CONTEXT_RENDER)
	{
		flags = BuGetItemFlag();
		if(playerMeter.stars >= 10 && (flags & SAVE_FLAG_HAVE_WING_CAP) == 0)
		{
			displayList = (Gfx*)AllocDynamic(2 * sizeof(*displayList));

			if(displayList == NULL)
			{
				return NULL;
			}
			else
			{
				displayListHead = displayList;
			}

			generatedNode			 = (struct GraphNodeGenerated*)node;
			generatedNode->fnNode.node.flags = (generatedNode->fnNode.node.flags & 0xFF) | 0x500;

			gSPDisplayList(displayListHead++, dl_castle_lobby_wing_cap_light);
			gSPEndDisplayList(displayListHead);
		}
	}

	return displayList;
}

/**
 * Update static timer variables that control the flying carpets' ripple effect.
 */
Gfx* ArajinInit(s32 callContext, UNUSED struct GraphNode* node, UNUSED f32 mtx[4][4])
{
	if(callContext != GEO_CONTEXT_RENDER)
	{
		sFlyingCarpetRippleTimer = 0;
		sPrevAreaTimer		 = animationCounter - 1;
		sCurAreaTimer		 = animationCounter;
		gFlyingCarpetState	 = FLYING_CARPET_IDLE;
	}
	else
	{
		sPrevAreaTimer = sCurAreaTimer;
		sCurAreaTimer  = animationCounter;
		if(sPrevAreaTimer != sCurAreaTimer)
		{
			sFlyingCarpetRippleTimer += 0x400 / FRAME_RATE_SCALER_INV;
		}
	}

	return NULL;
}

/**
 * Create a display list for a flying carpet with dynamic ripples.
 */
Gfx* ArajinWave(s32 callContext, struct GraphNode* node, UNUSED f32 mtx[4][4])
{
	s16 n, row, col, x, y, z, tx, ty;
	Vtx* verts;
	struct GraphNodeGenerated* generatedNode = (struct GraphNodeGenerated*)node;

	s16* sp64	     = (s16*)segmented_to_virtual(&arajin_vtx);
	Gfx* displayList     = NULL;
	Gfx* displayListHead = NULL;
	struct Object* curGraphNodeObject;

	if(callContext == GEO_CONTEXT_RENDER)
	{
		verts		= (Vtx*)AllocDynamic(NUM_FLYING_CARPET_VERTICES * sizeof(*verts));
		displayList	= (Gfx*)AllocDynamic(7 * sizeof(*displayList));
		displayListHead = displayList;

		if(verts == NULL || displayList == NULL)
		{
			return NULL;
		}

		generatedNode->fnNode.node.flags = (generatedNode->fnNode.node.flags & 0xFF) | 0x100;

		for(n = 0; n <= 20; n++)
		{
			row = n / 3;
			col = n % 3;

			x  = sp64[n * 4 + 0];
			y  = Tani_RoundOff(sins(sFlyingCarpetRippleTimer + (row << 12) + (col << 14)) * 20.0);
			z  = sp64[n * 4 + 1];
			tx = sp64[n * 4 + 2];
			ty = sp64[n * 4 + 3];

			Tani_SetOneVtxData(verts, n, x, y, z, tx, ty, 0, 127, 0, 255);
		}

		gSPDisplayList(displayListHead++, dl_flying_carpet_begin);

		// The forward half.
		gSPVertex(displayListHead++, verts, 12, 0);
		gSPDisplayList(displayListHead++, dl_flying_carpet_model_half);

		// The back half.
		gSPVertex(displayListHead++, verts + 9, 12, 0);
		gSPDisplayList(displayListHead++, dl_flying_carpet_model_half);

		gSPDisplayList(displayListHead++, dl_flying_carpet_end);
		gSPEndDisplayList(displayListHead);

		curGraphNodeObject = (struct Object*)gCurGraphNodeObject;
		if(gMarioObject->platform == curGraphNodeObject)
		{
			gFlyingCarpetState = FLYING_CARPET_MOVING_WITH_MARIO;
		}
		else if(curGraphNodeObject->oForwardVel != 0.0)
		{
			gFlyingCarpetState = FLYING_CARPET_MOVING_WITHOUT_MARIO;
		}
		else
		{
			gFlyingCarpetState = FLYING_CARPET_IDLE;
		}
	}

	return displayList;
}

/**
 * Create a display list for the end screen with Peach's delicious cake.
 */
Gfx* EndingBGDraw(s32 callContext, struct GraphNode* node, UNUSED f32 mtx[4][4])
{
	struct GraphNodeGenerated* generatedNode = (struct GraphNodeGenerated*)node;
	Gfx* displayList			 = NULL;
	Gfx* displayListHead			 = NULL;

	if(callContext == GEO_CONTEXT_RENDER)
	{
		displayList	= (Gfx*)AllocDynamic(3 * sizeof(*displayList));
		displayListHead = displayList;

		generatedNode->fnNode.node.flags = (generatedNode->fnNode.node.flags & 0xFF) | 0x100;

		gSPDisplayList(displayListHead++, wipe_gfx_init);
		gSPDisplayList(displayListHead++, gfx_ending);
		gSPEndDisplayList(displayListHead);
	}

	return displayList;
}
