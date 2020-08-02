#ifndef _MARIO_STEP_H
#define _MARIO_STEP_H

#include "types.h"

extern struct Surface gWaterSurfacePseudoFloor;

extern f32 PL_GetTrampolinePower(void);
// extern void PL_CheckTrampolineJump(struct PlayerRecord *);
extern void DoTrampoline(void);
/*
extern void playerRefrection(struct PlayerRecord *, u32);
extern u32 checkPlayerSinking(struct PlayerRecord *, f32);
extern u32 fallFromHeavySlope(struct PlayerRecord *, u32, u32);
extern u32 quicksandProcess(struct PlayerRecord *);
extern u32 gustProcess(struct PlayerRecord *);
extern void stopProcess(struct PlayerRecord *);
extern s32 waitProcess(struct PlayerRecord *);
extern s32 walkProcess(struct PlayerRecord *);
extern s32 jumpProcess(struct PlayerRecord *, u32);
*/

#endif /* _MARIO_STEP_H */
