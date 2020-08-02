#ifndef _TRANSPARENT_TEXTURE_H
#define _TRANSPARENT_TEXTURE_H

#include "types.h"

enum FlyingCarpetState
{
	FLYING_CARPET_IDLE		   = 0,
	FLYING_CARPET_MOVING_WITHOUT_MARIO = 1,
	FLYING_CARPET_MOVING_WITH_MARIO	   = 2
};

extern s8 gFlyingCarpetState;

extern void Tani_SetOneVtxData(Vtx* vtx, s32 n, f32 x, f32 y, f32 z, s16 tx, s16 ty, u8 r, u8 g, u8 b, u8 a);
extern s16 Tani_RoundOff(f32);
extern Gfx* SelRoomEffect(s32 callContext, struct GraphNode* node, f32 mtx[4][4]);
extern Gfx* ArajinInit(s32 callContext, struct GraphNode* node, f32 mtx[4][4]);
extern Gfx* ArajinWave(s32 callContext, struct GraphNode* node, f32 mtx[4][4]);
extern Gfx* EndingBGDraw(s32 callContext, struct GraphNode* node, f32 mtx[4][4]);

#endif /* _TRANSPARENT_TEXTURE_H */
