#include <ultra64.h>

#include "sm64.h"
#include "options.h"
#include "mario.h"
#include "area.h"
#include "audio/external.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "mario_misc.h"
#include "display.h"
#include "game.h"
#include "engine/graph_node.h"
#include "interaction.h"
#include "level_update.h"
#include "memory.h"
#include "main.h"
#include "mario_actions_object.h"
#include "mario_actions_automatic.h"
#include "mario_actions_cutscene.h"
#include "mario_actions_submerged.h"
#include "mario_actions_airborne.h"
#include "mario_actions_moving.h"
#include "mario_actions_stationary.h"
#include "mario_step.h"
#include "engine/math_util.h"
#include "object_fields.h"
#include "object_helpers.h"
#include "print.h"
#include "save_file.h"
#include "sound_init.h"
#include "engine/surface_collision.h"
#include "level_table.h"
#include "game/motor.h"

/**************************************************
 *                    ANIMATIONS                  *
 **************************************************/

/**
 * Rotates the animation's translation into the global coordinate system
 * and returns the animation's flags.
 */
s16 find_mario_anim_flags_and_translation(struct Object* obj, s32 yaw, Vec3s translation)
{
	f32 dx;
	f32 dz;

	struct Animation* curAnim = obj->header.gfx.unk38.curAnim;
	s16 animFrame		  = geo_update_animation_frame(&obj->header.gfx.unk38, NULL);
	u16* animIndex		  = (u16*)segmented_to_virtual((void*)curAnim->index);
	s16* animValues		  = (s16*)segmented_to_virtual((void*)curAnim->values);

	f32 s = (f32)sins(yaw);
	f32 c = (f32)coss(yaw);

	dx	       = *(animValues + (retrieve_animation_index(animFrame, &animIndex))) / 4.0f;
	translation[1] = *(animValues + (retrieve_animation_index(animFrame, &animIndex))) / 4.0f;
	dz	       = *(animValues + (retrieve_animation_index(animFrame, &animIndex))) / 4.0f;

	translation[0] = (dx * c) + (dz * s);
	translation[2] = (-dx * s) + (dz * c);

	return curAnim->flags;
}

/**
 * Collides with walls and returns the most recent wall.
 */
struct Surface* resolve_and_return_wall_collisions(Vec3f pos, f32 offset, f32 radius)
{
	struct WallCollisionData collisionData;
	struct Surface* wall = NULL;

	collisionData.x	      = pos[0];
	collisionData.y	      = pos[1];
	collisionData.z	      = pos[2];
	collisionData.radius  = radius;
	collisionData.offsetY = offset;

	if(find_wall_collisions(&collisionData))
	{
		wall = collisionData.walls[collisionData.numWalls - 1];
	}

	pos[0] = collisionData.x;
	pos[1] = collisionData.y;
	pos[2] = collisionData.z;

	// This only returns the most recent wall and can also return NULL
	// there are no wall collisions.
	return wall;
}

/**
 * Finds the ceiling from a vec3f horizontally and a height (with 80 vertical buffer).
 */
f32 PL_CheckRoofPlane(Vec3f pos, f32 height, struct Surface** ceil)
{
	UNUSED f32 unused;

	return find_ceil(pos[0], height + 80.0f, pos[2], ceil);
}

static void func_sh_8025574C(void)
{
	if(marioWorks->particleFlags & PARTICLE_POUND_TINY_STAR)
	{
		SendMotorEvent(5, 80);
	}
	else if(marioWorks->particleFlags & PARTICLE_WALL_TINY_STAR)
	{
		SendMotorEvent(5, 80);
	}
	else if(marioWorks->particleFlags & PARTICLE_PUNCH_TINY_TRIANGLES)
	{
		SendMotorEvent(5, 80);
	}

	if(marioWorks->heldObj && marioWorks->heldObj->behavior == sm64::bhv::bhvBobomb())
	{
		ResetMotorPack();
	}
}

/**
 * Main function for executing Mario's behavior.
 */
s32 execute_mario_action(struct Object* o)
{
	s32 inLoop = TRUE;

	if(marioWorks->status)
	{
		marioWorks->marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
		marioWorks->mario_reset_bodystate();
		marioWorks->update_mario_inputs();
		PL_CheckGroundCondition(marioWorks);
		PL_CollisionCheck(marioWorks);

		// If Mario is OOB, stop executing actions.
		if(marioWorks->floor == NULL)
		{
			return 0;
		}

		// The function can loop through many action shifts in one frame,
		// which can lead to unexpected sub-frame behavior. Could potentially hang
		// if a loop of actions were found, but there has not been a situation found.
		while(inLoop)
		{
			switch(marioWorks->status & ACT_GROUP_MASK)
			{
				case ACT_GROUP_STATIONARY:
					inLoop = marioWorks->playerWaitMain();
					break;

				case ACT_GROUP_MOVING:
					inLoop = marioWorks->playerMoveMain();
					break;

				case ACT_GROUP_AIRBORNE:
					inLoop = marioWorks->playerJumpMain();
					break;

				case ACT_GROUP_SUBMERGED:
					inLoop = marioWorks->PL_PlayerSwimMain();
					break;

				case ACT_GROUP_CUTSCENE:
					inLoop = marioWorks->playerDemoMain();
					break;

				case ACT_GROUP_AUTOMATIC:
					inLoop = marioWorks->playerSpecMain();
					break;

				case ACT_GROUP_OBJECT:
					inLoop = marioWorks->playerActnMain();
					break;
			}
		}

		marioWorks->sink_mario_in_quicksand();
		marioWorks->squish_mario_model();
		marioWorks->set_submerged_cam_preset_and_spawn_bubbles();
		marioWorks->update_mario_health();
		marioWorks->update_mario_info_for_cam();
		marioWorks->mario_update_hitbox_and_cap_model();

		// Both of the wind handling portions play wind audio only in
		// non-Japanese releases.
		if(marioWorks->floor->type == SURFACE_HORIZONTAL_WIND)
		{
			s_make_effectwind(0, (marioWorks->floor->force << 8));
			AudStartSound(SOUND_ENV_WIND2, marioWorks->marioObj->header.gfx.cameraToObject);
		}

		if(marioWorks->floor->type == SURFACE_VERTICAL_WIND)
		{
			s_make_effectwind(1, 0);
			AudStartSound(SOUND_ENV_WIND2, marioWorks->marioObj->header.gfx.cameraToObject);
		}

		AudUnlimitedMusic();
		marioWorks->marioObj->oInteractStatus = 0;
		func_sh_8025574C();

		return marioWorks->particleFlags;
	}

	return 0;
}

/**************************************************
 *                  INITIALIZATION                *
 **************************************************/
