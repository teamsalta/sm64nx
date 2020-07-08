#ifndef _MARIO_H
#define _MARIO_H

#include "types.h"

extern u32 gAudioRandom;

extern struct Object *gMarioObject;
extern struct Object *gLuigiObject;

s16 find_mario_anim_flags_and_translation(struct Object *o, s32 yaw, Vec3s translation);
s32 execute_mario_action(struct Object *o);

struct Surface *resolve_and_return_wall_collisions(Vec3f pos, f32 offset, f32 radius);
f32 vec3f_find_ceil(Vec3f pos, f32 height, struct Surface **ceil);

#endif /* _MARIO_H */
