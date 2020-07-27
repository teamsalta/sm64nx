#ifndef _MARIO_STEP_H
#define _MARIO_STEP_H

#include "types.h"

extern struct Surface gWaterSurfacePseudoFloor;

extern f32 get_additive_y_vel_for_jumps(void);
// extern void nop_80254E3C(struct PlayerRecord *);
extern void nop_80254E50(void);
/*
extern void mario_bonk_reflection(struct PlayerRecord *, u32);
extern u32 mario_update_quicksand(struct PlayerRecord *, f32);
extern u32 mario_push_off_steep_floor(struct PlayerRecord *, u32, u32);
extern u32 mario_update_moving_sand(struct PlayerRecord *);
extern u32 mario_update_windy_ground(struct PlayerRecord *);
extern void stop_and_set_height_to_floor(struct PlayerRecord *);
extern s32 stationary_ground_step(struct PlayerRecord *);
extern s32 perform_ground_step(struct PlayerRecord *);
extern s32 perform_air_step(struct PlayerRecord *, u32);
*/

#endif /* _MARIO_STEP_H */
