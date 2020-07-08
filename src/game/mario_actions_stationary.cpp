#include <ultra64.h>

#include "sm64.h"
#include "engine/math_util.h"
#include "mario.h"
#include "interaction.h"
#include "mario_step.h"
#include "mario_actions_stationary.h"
#include "area.h"
#include "audio/external.h"
#include "memory.h"
#include "behavior_data.h"
#include "sound_init.h"
#include "level_update.h"
#include "save_file.h"
#include "camera.h"

s32 MarioState::func_802604E0()
{
	mario_drop_held_object(this);
	if(this->floor->normal.y < 0.29237169f)
	{
		return mario_push_off_steep_floor(ACT_FREEFALL, 0);
	}

	if(this->input & INPUT_UNKNOWN_10)
	{
		return set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & INPUT_A_PRESSED)
	{
		return set_jumping_action(ACT_JUMP, 0);
	}

	if(this->input & INPUT_OFF_FLOOR)
	{
		return set_mario_action(ACT_FREEFALL, 0);
	}

	if(this->input & INPUT_ABOVE_SLIDE)
	{
		return set_mario_action(ACT_BEGIN_SLIDING, 0);
	}

	if(this->input & INPUT_FIRST_PERSON)
	{
		return set_mario_action(ACT_FIRST_PERSON, 0);
	}

	if(this->input & INPUT_NONZERO_ANALOG)
	{
		this->faceAngle[1] = (s16)this->intendedYaw;
		return set_mario_action(ACT_WALKING, 0);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_PUNCHING, 0);
	}

	if(this->input & INPUT_Z_DOWN)
	{
		return set_mario_action(ACT_START_CROUCHING, 0);
	}

	return 0;
}

s32 MarioState::func_802606DC()
{
	if(this->floor->normal.y < 0.29237169f)
	{
		return mario_push_off_steep_floor(ACT_HOLD_FREEFALL, 0);
	}

	if(this->heldObj->oInteractionSubtype & INT_SUBTYPE_DROP_IMMEDIATELY)
	{
		this->heldObj->oInteractionSubtype = (s32)(this->heldObj->oInteractionSubtype & ~INT_SUBTYPE_DROP_IMMEDIATELY);
		return set_mario_action(ACT_PLACING_DOWN, 0);
	}

	if(this->input & INPUT_UNKNOWN_10)
	{
		return drop_and_set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & INPUT_A_PRESSED)
	{
		return set_jumping_action(ACT_HOLD_JUMP, 0);
	}

	if(this->input & INPUT_OFF_FLOOR)
	{
		return set_mario_action(ACT_HOLD_FREEFALL, 0);
	}

	if(this->input & INPUT_ABOVE_SLIDE)
	{
		return set_mario_action(ACT_HOLD_BEGIN_SLIDING, 0);
	}

	if(this->input & INPUT_NONZERO_ANALOG)
	{
		this->faceAngle[1] = (s16)this->intendedYaw;
		return set_mario_action(ACT_HOLD_WALKING, 0);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_THROWING, 0);
	}

	if(this->input & INPUT_Z_DOWN)
	{
		return drop_and_set_mario_action(ACT_START_CROUCHING, 0);
	}

	return 0;
}

s32 MarioState::act_idle()
{
	if(this->quicksandDepth > 30.0f)
	{
		return set_mario_action(ACT_IN_QUICKSAND, 0);
	}

	if(this->input & INPUT_IN_POISON_GAS)
	{
		return set_mario_action(ACT_COUGHING, 0);
	}

	if(!(this->actionArg & 1) && this->health < 0x300)
	{
		return set_mario_action(ACT_PANTING, 0);
	}

	if(func_802604E0())
	{
		return 1;
	}

	if(this->actionState == 3)
	{
		if((this->area->terrainType & TERRAIN_MASK) == TERRAIN_SNOW)
		{
			return set_mario_action(ACT_SHIVERING, 0);
		}
		else
		{
			return set_mario_action(ACT_START_SLEEPING, 0);
		}
	}

	if(this->actionArg & 1)
	{
		set_mario_animation(MARIO_ANIM_STAND_AGAINST_WALL);
	}
	else
	{
		switch(this->actionState)
		{
			case 0:
				set_mario_animation(MARIO_ANIM_IDLE_HEAD_LEFT);
				break;

			case 1:
				set_mario_animation(MARIO_ANIM_IDLE_HEAD_RIGHT);
				break;

			case 2:
				set_mario_animation(MARIO_ANIM_IDLE_HEAD_CENTER);
				break;
		}

		if(is_anim_at_end())
		{
			// Fall asleep after 10 head turning cycles.
			// act_start_sleeping is triggered earlier in the function
			// when actionState == 3. This happens when Mario's done
			// turning his head back and forth. However, we do some checks
			// here to make sure that Mario would be able to sleep here,
			// and that he's gone through 10 cycles before sleeping.
			// actionTimer is used to track how many cycles have passed.
			if(++this->actionState == 3)
			{
				f32 sp24 = this->pos[1] - find_floor_height_relative_polar(-0x8000, 60.0f);

				if(sp24 < -24.0f || 24.0f < sp24 || this->floor->flags & 1)
				{
					this->actionState = 0;
				}
				else
				{
					// If Mario hasn't turned his head 10 times yet, stay idle instead of going to
					// sleep.
					this->actionTimer++;
					if(this->actionTimer < 10)
					{
						this->actionState = 0;
					}
				}
			}
		}
	}

	stationary_ground_step();

	return 0;
}

void MarioState::func_80260BC4(u32 actionState, s32 animFrame, u32 sound)
{
	if(this->actionState == actionState && this->marioObj->header.gfx.unk38.frame() == animFrame)
	{
		play_sound(sound, this->marioObj->header.gfx.cameraToObject);
	}
}

s32 MarioState::act_start_sleeping()
{
#ifndef VERSION_JP
	s32 sp24;
#endif

	if(func_802604E0())
	{
		return 1;
	}

	if(this->quicksandDepth > 30.0f)
	{
		return set_mario_action(ACT_IN_QUICKSAND, 0);
	}

	if(this->actionState == 4)
	{
		return set_mario_action(ACT_SLEEPING, 0);
	}

	switch(this->actionState)
	{
		case 0:
#ifndef VERSION_JP
			sp24 = set_mario_animation(MARIO_ANIM_START_SLEEP_IDLE);
#else
			set_mario_animation(MARIO_ANIM_START_SLEEP_IDLE);
#endif
			break;

		case 1:
#ifndef VERSION_JP
			sp24 = set_mario_animation(MARIO_ANIM_START_SLEEP_SCRATCH);
#else
			set_mario_animation(MARIO_ANIM_START_SLEEP_SCRATCH);
#endif
			break;

		case 2:
#ifndef VERSION_JP
			sp24 = set_mario_animation(MARIO_ANIM_START_SLEEP_YAWN);
#else
			set_mario_animation(MARIO_ANIM_START_SLEEP_YAWN);
#endif
			this->marioBodyState->eyeState = MARIO_EYES_HALF_CLOSED;
			break;

		case 3:
#ifndef VERSION_JP
			sp24 = set_mario_animation(MARIO_ANIM_START_SLEEP_SITTING);
#else
			set_mario_animation(MARIO_ANIM_START_SLEEP_SITTING);
#endif
			this->marioBodyState->eyeState = MARIO_EYES_HALF_CLOSED;
			break;
	}

	func_80260BC4(1, 41, SOUND_ACTION_PAT_BACK);
	func_80260BC4(1, 49, SOUND_ACTION_PAT_BACK);
	func_80260BC4(3, 15, this->terrainSoundAddend + SOUND_ACTION_TERRAIN_BODY_HIT_GROUND);

	if(is_anim_at_end())
	{
		this->actionState++;
	}

#ifndef VERSION_JP
	if(this->actionState == 2)
	{
		if(sp24 == -1)
		{
			play_sound(SOUND_MARIO_YAWNING, this->marioObj->header.gfx.cameraToObject);
		}
	}

	if(this->actionState == 1)
	{
		if(sp24 == -1)
		{
			play_sound(SOUND_MARIO_IMA_TIRED, this->marioObj->header.gfx.cameraToObject);
		}
	}
#else
	if(this->actionState == 2)
	{
		play_sound_if_no_flag(SOUND_MARIO_YAWNING, MARIO_MARIO_SOUND_PLAYED);
	}
#endif

	stationary_ground_step();
	return 0;
}

s32 MarioState::act_sleeping()
{
	s32 sp24;
	if(this->input & INPUT_UNKNOWN_A41F /* ? */)
	{
		return set_mario_action(ACT_WAKING_UP, this->actionState);
	}

	if(this->quicksandDepth > 30.0f)
	{
		return set_mario_action(ACT_WAKING_UP, this->actionState);
	}

	if(this->pos[1] - find_floor_height_relative_polar(-0x8000, 60.0f) > 24.0f)
	{
		return set_mario_action(ACT_WAKING_UP, this->actionState);
	}

	this->marioBodyState->eyeState = MARIO_EYES_CLOSED;
	stationary_ground_step();
	switch(this->actionState)
	{
		case 0:
		{
			sp24 = set_mario_animation(MARIO_ANIM_SLEEP_IDLE);

			if(sp24 == -1 && !this->actionTimer)
			{
				func_80248C28(2);
			}

			if(sp24 == 2)
			{
				play_sound(SOUND_MARIO_SNORING1, this->marioObj->header.gfx.cameraToObject);
			}

			if(sp24 == 20)
			{
				play_sound(SOUND_MARIO_SNORING2, this->marioObj->header.gfx.cameraToObject);
			}

			if(is_anim_at_end())
			{
				this->actionTimer++;
				if(this->actionTimer > 45 * FRAME_RATE_SCALER_INV)
				{
					this->actionState++;
				}
			}
			break;
		}
		case 1:
		{
			if(set_mario_animation(MARIO_ANIM_SLEEP_START_LYING) == 18)
			{
				play_mario_heavy_landing_sound(SOUND_ACTION_TERRAIN_BODY_HIT_GROUND);
			}

			if(is_anim_at_end())
			{
				this->actionState++;
			}
			break;
		}
		case 2:
		{
			sp24 = set_mario_animation(MARIO_ANIM_SLEEP_LYING);
#ifndef VERSION_JP
			play_sound_if_no_flag(SOUND_MARIO_SNORING3, MARIO_ACTION_SOUND_PLAYED);
#else
			if(sp24 == 2)
			{
				play_sound(SOUND_MARIO_SNORING2, this->marioObj->header.gfx.cameraToObject);
			}

			if(sp24 == 25)
			{
				play_sound(SOUND_MARIO_SNORING1, this->marioObj->header.gfx.cameraToObject);
			}
#endif
			break;
		}
	}
	return 0;
}

s32 MarioState::act_waking_up()
{
	if(!this->actionTimer)
	{
		func_803205E8(SOUND_MARIO_SNORING1, this->marioObj->header.gfx.cameraToObject);
		func_803205E8(SOUND_MARIO_SNORING2, this->marioObj->header.gfx.cameraToObject);
#ifndef VERSION_JP
		func_803205E8(SOUND_MARIO_SNORING3, this->marioObj->header.gfx.cameraToObject);
#endif
		func_80248CB8(2);
	}

	if(this->input & INPUT_UNKNOWN_10)
	{
		return set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & INPUT_OFF_FLOOR)
	{
		return set_mario_action(ACT_FREEFALL, 0);
	}

	if(this->input & INPUT_ABOVE_SLIDE)
	{
		return set_mario_action(ACT_BEGIN_SLIDING, 0);
	}

	this->actionTimer++;

	if(this->actionTimer > 20 * FRAME_RATE_SCALER_INV)
	{
		return set_mario_action(ACT_IDLE, 0);
	}

	stationary_ground_step();

	set_mario_animation(!this->actionArg ? MARIO_ANIM_WAKE_FROM_SLEEP : MARIO_ANIM_WAKE_FROM_LYING);

	return 0;
}

s32 MarioState::act_shivering()
{
	s32 sp24;

	if(this->input & INPUT_UNKNOWN_10)
	{
		return set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & INPUT_OFF_FLOOR)
	{
		return set_mario_action(ACT_FREEFALL, 0);
	}

	if(this->input & INPUT_ABOVE_SLIDE)
	{
		return set_mario_action(ACT_BEGIN_SLIDING, 0);
	}

	if(this->input & INPUT_UNKNOWN_A41F)
	{
		this->actionState = 2;
	}

	stationary_ground_step();
	switch(this->actionState)
	{
		case 0:
		{
			sp24 = set_mario_animation(MARIO_ANIM_SHIVERING_WARMING_HAND);
			if(sp24 == 0x31)
			{
				this->particleFlags |= PARTICLE_WATER_MIST;
				play_sound(SOUND_MARIO_PANTING_COLD, this->marioObj->header.gfx.cameraToObject);
			}
			if(sp24 == 7 || sp24 == 0x51)
			{
				play_sound(SOUND_ACTION_CLAP_HANDS_COLD, this->marioObj->header.gfx.cameraToObject);
			}
			if(is_anim_past_end())
			{
				this->actionState = 1;
			}
			break;
		}
		case 1:
		{
			sp24 = set_mario_animation(MARIO_ANIM_SHIVERING);
			if(sp24 == 9 || sp24 == 0x19 || sp24 == 0x2C)
			{
				play_sound(SOUND_ACTION_CLAP_HANDS_COLD, this->marioObj->header.gfx.cameraToObject);
			}
			break;
		}
		case 2:
		{
			set_mario_animation(MARIO_ANIM_SHIVERING_RETURN_TO_IDLE);
			if(is_anim_past_end())
			{
				set_mario_action(ACT_IDLE, 0);
			}
			break;
		}
	}
	return 0;
}

s32 MarioState::act_coughing()
{
	s32 sp1C;

	if(func_802604E0())
	{
		return 1;
	}

	stationary_ground_step();
	sp1C = set_mario_animation(MARIO_ANIM_COUGHING);
	if(sp1C == 0x19 || sp1C == 0x23)
	{
		play_sound(SOUND_MARIO_COUGHING3, this->marioObj->header.gfx.cameraToObject);
	}

	if(sp1C == 0x32 || sp1C == 0x3A)
	{
		play_sound(SOUND_MARIO_COUGHING2, this->marioObj->header.gfx.cameraToObject);
	}

	if(sp1C == 0x47 || sp1C == 0x50)
	{
		play_sound(SOUND_MARIO_COUGHING1, this->marioObj->header.gfx.cameraToObject);
	}

	return 0;
}

s32 MarioState::act_hold_idle()
{
	if(segmented_to_virtual(sm64::bhv::bhvJumpingBox()) == this->heldObj->behavior)
	{
		return set_mario_action(ACT_CRAZY_BOX_BOUNCE, 0);
	}

	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_IDLE, 0);
	}

	if(this->quicksandDepth > 30.0f)
	{
		return drop_and_set_mario_action(ACT_IN_QUICKSAND, 0);
	}

	if(func_802606DC())
	{
		return 1;
	}

	stationary_ground_step();
	set_mario_animation(MARIO_ANIM_IDLE_WITH_LIGHT_OBJ);
	return 0;
}

s32 MarioState::act_hold_heavy_idle()
{
	if(this->input & INPUT_UNKNOWN_10)
	{
		return drop_and_set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & INPUT_OFF_FLOOR)
	{
		return drop_and_set_mario_action(ACT_FREEFALL, 0);
	}

	if(this->input & INPUT_ABOVE_SLIDE)
	{
		return drop_and_set_mario_action(ACT_BEGIN_SLIDING, 0);
	}

	if(this->input & INPUT_NONZERO_ANALOG)
	{
		return set_mario_action(ACT_HOLD_HEAVY_WALKING, 0);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_HEAVY_THROW, 0);
	}

	stationary_ground_step();
	set_mario_animation(MARIO_ANIM_IDLE_HEAVY_OBJ);
	return 0;
}

s32 MarioState::act_standing_against_wall()
{
	if(this->input & INPUT_UNKNOWN_10)
	{
		return set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE))
	{
		return check_common_action_exits();
	}

	if(this->input & INPUT_FIRST_PERSON)
	{
		return set_mario_action(ACT_FIRST_PERSON, 0);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_PUNCHING, 0);
	}

	set_mario_animation(MARIO_ANIM_STAND_AGAINST_WALL);
	stationary_ground_step();
	return 0;
}

s32 MarioState::act_in_quicksand()
{
	if(this->quicksandDepth < 30.0f)
	{
		return set_mario_action(ACT_IDLE, 0);
	}

	if(func_802604E0() != 0)
	{
		return 1;
	}

	if(this->quicksandDepth > 70.0f)
	{
		set_mario_animation(MARIO_ANIM_DYING_IN_QUICKSAND);
	}
	else
	{
		set_mario_animation(MARIO_ANIM_IDLE_IN_QUICKSAND);
	}

	stationary_ground_step();
	return 0;
}

s32 MarioState::act_crouching()
{
	if(this->input & INPUT_UNKNOWN_10)
	{
		return set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & INPUT_A_PRESSED)
	{
		return set_jumping_action(ACT_BACKFLIP, 0);
	}

	if(this->input & INPUT_OFF_FLOOR)
	{
		return set_mario_action(ACT_FREEFALL, 0);
	}

	if(this->input & INPUT_ABOVE_SLIDE)
	{
		return set_mario_action(ACT_BEGIN_SLIDING, 0);
	}

	if(this->input & INPUT_FIRST_PERSON)
	{
		return set_mario_action(ACT_UNKNOWN_022, 0);
	}

	if(!(this->input & INPUT_Z_DOWN))
	{
		return set_mario_action(ACT_UNKNOWN_022, 0);
	}

	if(this->input & INPUT_NONZERO_ANALOG)
	{
		return set_mario_action(ACT_START_CRAWLING, 0);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_PUNCHING, 9);
	}

	stationary_ground_step();
	set_mario_animation(MARIO_ANIM_CROUCHING);
	return 0;
}

s32 MarioState::act_panting()
{
	if(this->input & INPUT_UNKNOWN_10)
	{
		return set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->health >= 0x500)
	{
		return set_mario_action(ACT_IDLE, 0);
	}

	if(func_802604E0())
	{
		return 1;
	}

	if(set_mario_animation(MARIO_ANIM_WALK_PANTING) == 1)
	{
		play_sound(SOUND_MARIO_PANTING + ((gAudioRandom % 3U) << 0x10), this->marioObj->header.gfx.cameraToObject);
	}

	stationary_ground_step();
	this->marioBodyState->eyeState = MARIO_EYES_HALF_CLOSED;
	return 0;
}

s32 MarioState::act_hold_panting_unused()
{
	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_PANTING, 0);
	}

	if(this->input & INPUT_UNKNOWN_10)
	{
		return drop_and_set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->health >= 0x500)
	{
		return set_mario_action(ACT_HOLD_IDLE, 0);
	}

	if(func_802606DC() != 0)
	{
		return 1;
	}

	set_mario_animation(MARIO_ANIM_WALK_PANTING);
	stationary_ground_step();
	this->marioBodyState->eyeState = MARIO_EYES_HALF_CLOSED;
	return 0;
}

void MarioState::func_80261D70(s32 animID, u32 action)
{
	stationary_ground_step();
	set_mario_animation(animID);
	if(is_anim_at_end() != 0)
	{
		set_mario_action(action, 0);
	}
}

s32 MarioState::act_braking_stop()
{
	if(this->input & INPUT_UNKNOWN_10)
	{
		return set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & INPUT_OFF_FLOOR)
	{
		return set_mario_action(ACT_FREEFALL, 0);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_PUNCHING, 0);
	}

	if(!(this->input & INPUT_FIRST_PERSON) && this->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE))
	{
		return check_common_action_exits();
	}

	func_80261D70(MARIO_ANIM_STOP_SKID, ACT_IDLE);
	return 0;
}

s32 MarioState::act_butt_slide_stop()
{
	if(this->input & INPUT_UNKNOWN_10)
	{
		return set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE))
	{
		return check_common_action_exits();
	}

	func_80261D70(MARIO_ANIM_STOP_SLIDE, ACT_IDLE);
	if(this->marioObj->header.gfx.unk38.frame() == 6)
	{
		play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
	}

	return 0;
}

s32 MarioState::func_80261F8C()
{
	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_IDLE, 0);
	}

	if(this->input & INPUT_UNKNOWN_10)
	{
		return drop_and_set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE))
	{
		return check_common_hold_action_exits();
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_THROWING, 0);
	}

	func_80261D70(MARIO_ANIM_STAND_UP_FROM_SLIDING_WITH_LIGHT_OBJ, ACT_HOLD_IDLE);
	return 0;
}

s32 MarioState::act_slide_kick_slide_stop()
{
	if(this->input & INPUT_UNKNOWN_10)
	{
		return drop_and_set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & INPUT_OFF_FLOOR)
	{
		return drop_and_set_mario_action(ACT_FREEFALL, 0);
	}

	func_80261D70(MARIO_ANIM_CROUCH_FROM_SLIDE_KICK, ACT_CROUCHING);
	return 0;
}

s32 MarioState::act_start_crouching()
{
	if(this->input & INPUT_UNKNOWN_10)
	{
		return set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & INPUT_OFF_FLOOR)
	{
		return set_mario_action(ACT_FREEFALL, 0);
	}

	if(this->input & INPUT_A_PRESSED)
	{
		return set_jumping_action(ACT_BACKFLIP, 0);
	}

	if(this->input & INPUT_ABOVE_SLIDE)
	{
		return set_mario_action(ACT_BEGIN_SLIDING, 0);
	}

	stationary_ground_step();
	set_mario_animation(MARIO_ANIM_START_CROUCHING);
	if(is_anim_past_end())
	{
		set_mario_action(ACT_CROUCHING, 0);
	}
	return 0;
}

s32 MarioState::act_stop_crouching()
{
	if(this->input & INPUT_UNKNOWN_10)
	{
		return set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & INPUT_OFF_FLOOR)
	{
		return set_mario_action(ACT_FREEFALL, 0);
	}

	if(this->input & INPUT_A_PRESSED)
	{
		return set_jumping_action(ACT_BACKFLIP, 0);
	}

	if(this->input & INPUT_ABOVE_SLIDE)
	{
		return set_mario_action(ACT_BEGIN_SLIDING, 0);
	}

	stationary_ground_step();
	set_mario_animation(MARIO_ANIM_STOP_CROUCHING);
	if(is_anim_past_end())
	{
		set_mario_action(ACT_IDLE, 0);
	}
	return 0;
}

s32 MarioState::act_start_crawling()
{
	if(this->input & INPUT_FIRST_PERSON)
	{
		return set_mario_action(ACT_UNKNOWN_022, 0);
	}

	if(this->input & INPUT_OFF_FLOOR)
	{
		return set_mario_action(ACT_FREEFALL, 0);
	}

	if(this->input & INPUT_UNKNOWN_10)
	{
		return set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & INPUT_ABOVE_SLIDE)
	{
		return set_mario_action(ACT_BEGIN_SLIDING, 0);
	}

	stationary_ground_step();
	set_mario_animation(MARIO_ANIM_START_CRAWLING);
	if(is_anim_past_end())
	{
		set_mario_action(ACT_CRAWLING, 0);
	}

	return 0;
}

s32 MarioState::act_stop_crawling()
{
	if(this->input & INPUT_UNKNOWN_10)
	{
		return set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & INPUT_OFF_FLOOR)
	{
		return set_mario_action(ACT_FREEFALL, 0);
	}

	if(this->input & INPUT_ABOVE_SLIDE)
	{
		return set_mario_action(ACT_BEGIN_SLIDING, 0);
	}

	stationary_ground_step();
	set_mario_animation(MARIO_ANIM_STOP_CRAWLING);
	if(is_anim_past_end())
	{
		set_mario_action(ACT_CROUCHING, 0);
	}
	return 0;
}

s32 MarioState::func_80262574()
{
	s16 sp1E;
	f32 sp18;

	if(this->marioObj->oInteractStatus & 0x10)
	{
		return hurt_and_set_mario_action(ACT_SHOCKED, 0, 4);
	}

	if(this->actionTimer == 0)
	{
		if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_UNK1)
		{
			return hurt_and_set_mario_action(ACT_BACKWARD_GROUND_KB, 0, 0xc);
		}
	}

	if(++this->actionTimer == 0x30 * FRAME_RATE_SCALER_INV)
	{
		return set_mario_action(ACT_IDLE, 0);
	}

	sp1E = ((this->actionTimer / FRAME_RATE_SCALER_INV) % 0x10) << 0xC;
	sp18 = (f32)(((f32)(6 - (this->actionTimer / FRAME_RATE_SCALER_INV) / 8) * 8.0f) + 4.0f);

	mario_set_forward_vel(0);
	vec3f_set(this->vel, 0.0f, 0.0f, 0.0f);
	if(sins(sp1E) >= 0.0f)
	{
		this->pos[1] = sins(sp1E) * sp18 + this->floorHeight;
	}
	else
	{
		this->pos[1] = this->floorHeight - sins(sp1E) * sp18;
	}

	vec3f_copy(this->marioObj->header.gfx.pos, this->pos);
	vec3s_set(this->marioObj->header.gfx.angle, 0, this->faceAngle[1], 0);
	set_mario_animation(MARIO_ANIM_A_POSE);
	return 0;
}

s32 MarioState::func_802627B8(s32 arg1, u32 action)
{
	stationary_ground_step();
	set_mario_animation(arg1);
	if(is_anim_at_end())
	{
		return set_mario_action(action, 0);
	}
	return 0;
}

s32 MarioState::func_80262828(u32 action)
{
	if(this->input & INPUT_UNKNOWN_10)
	{
		return set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & INPUT_FIRST_PERSON)
	{
		return set_mario_action(ACT_IDLE, 0);
	}

	if(this->input & INPUT_A_PRESSED)
	{
		if(!action)
		{
			return set_jump_from_landing();
		}
		else
		{
			return set_jumping_action(action, 0);
		}
	}

	if(this->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE))
	{
		return check_common_action_exits();
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_PUNCHING, 0);
	}

	return 0;
}

s32 MarioState::act_jump_land_stop()
{
	if(func_80262828(0))
	{
		return 1;
	}

	func_802627B8(MARIO_ANIM_LAND_FROM_SINGLE_JUMP, ACT_IDLE);
	return 0;
}

s32 MarioState::act_double_jump_land_stop()
{
	if(func_80262828(0))
	{
		return 1;
	}

	func_802627B8(MARIO_ANIM_LAND_FROM_DOUBLE_JUMP, ACT_IDLE);
	return 0;
}

s32 MarioState::act_side_flip_land_stop()
{
	if(func_80262828(0))
	{
		return 1;
	}

	func_802627B8(MARIO_ANIM_SLIDEFLIP_LAND, ACT_IDLE);
	this->marioObj->header.gfx.angle[1] += 0x8000;
	return 0;
}

s32 MarioState::act_freefall_land_stop()
{
	if(func_80262828(0))
	{
		return 1;
	}

	func_802627B8(MARIO_ANIM_GENERAL_LAND, ACT_IDLE);
	return 0;
}

s32 MarioState::act_triple_jump_land_stop()
{
	if(func_80262828(ACT_JUMP))
	{
		return 1;
	}

	func_802627B8(MARIO_ANIM_TRIPLE_JUMP_LAND, ACT_IDLE);
	return 0;
}

s32 MarioState::act_backflip_land_stop()
{
	if(!(this->input & INPUT_Z_DOWN) || this->marioObj->header.gfx.unk38.frame() >= 6)
	{
		this->input &= -3;
	}

	if(func_80262828(ACT_BACKFLIP))
	{
		return 1;
	}

	func_802627B8(MARIO_ANIM_TRIPLE_JUMP_LAND, ACT_IDLE);
	return 0;
}

s32 MarioState::act_lava_boost_land()
{
	this->input &= -0x2011;

	if(func_80262828(0))
	{
		return 1;
	}

	func_802627B8(MARIO_ANIM_STAND_UP_FROM_LAVA_BOOST, ACT_IDLE);
	return 0;
}

s32 MarioState::act_long_jump_land_stop()
{
	this->input &= -0x2001;
	if(func_80262828(ACT_JUMP))
	{
		return 1;
	}

	func_802627B8(!this->marioObj->oMarioLongJumpIsSlow ? MARIO_ANIM_CROUCH_FROM_FAST_LONGJUMP : MARIO_ANIM_CROUCH_FROM_SLOW_LONGJUMP, ACT_CROUCHING);
	return 0;
}

s32 MarioState::func_80262CEC()
{
	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_IDLE, 0);
	}

	if(this->input & INPUT_UNKNOWN_10)
	{
		return drop_and_set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE))
	{
		return check_common_hold_action_exits();
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_THROWING, 0);
	}

	func_802627B8(MARIO_ANIM_JUMP_LAND_WITH_LIGHT_OBJ, ACT_HOLD_IDLE);
	return 0;
}

s32 MarioState::func_80262DE4()
{
	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_IDLE, 0);
	}

	if(this->input & INPUT_UNKNOWN_10)
	{
		return drop_and_set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE))
	{
		return check_common_hold_action_exits();
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_THROWING, 0);
	}
	func_802627B8(MARIO_ANIM_FALL_LAND_WITH_LIGHT_OBJ, ACT_HOLD_IDLE);
	return 0;
}

s32 MarioState::act_air_throw_land()
{
	if(this->input & INPUT_UNKNOWN_10)
	{
		return set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & INPUT_OFF_FLOOR)
	{
		return set_mario_action(ACT_FREEFALL, 0);
	}

	if(++this->actionTimer == 4 * FRAME_RATE_SCALER_INV)
	{
		mario_throw_held_object(this);
	}

	func_802627B8(MARIO_ANIM_THROW_LIGHT_OBJECT, ACT_IDLE);
	return 0;
}

s32 MarioState::act_twirl_land()
{
	this->actionState = 1;
	if(this->input & INPUT_UNKNOWN_10)
	{
		return set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & INPUT_OFF_FLOOR)
	{
		return set_mario_action(ACT_FREEFALL, 0);
	}

	stationary_ground_step();
	set_mario_animation(MARIO_ANIM_TWIRL_LAND);
	if(this->angleVel[1] > 0)
	{
		this->angleVel[1] -= 0x400;
		if(this->angleVel[1] < 0)
		{
			this->angleVel[1] = 0;
		}

		this->twirlYaw += this->angleVel[1];
	}

	this->marioObj->header.gfx.angle[1] += this->twirlYaw;
	if(is_anim_at_end())
	{
		if(this->angleVel[1] == 0)
		{
			this->faceAngle[1] += this->twirlYaw;
			set_mario_action(ACT_IDLE, 0);
		}
	}

	return 0;
}

s32 MarioState::act_ground_pound_land()
{
	this->actionState = 1;
	if(this->input & INPUT_UNKNOWN_10)
	{
		return drop_and_set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(this->input & INPUT_OFF_FLOOR)
	{
		return set_mario_action(ACT_FREEFALL, 0);
	}

	if(this->input & INPUT_ABOVE_SLIDE)
	{
		return set_mario_action(ACT_BUTT_SLIDE, 0);
	}

	func_802627B8(MARIO_ANIM_GROUND_POUND_LANDING, ACT_BUTT_SLIDE_STOP);
	return 0;
}

s32 MarioState::act_first_person()
{
	s32 sp1C;
	s16 sp1A;
	s16 sp18;

	sp1C = 0U < (this->input & (INPUT_UNKNOWN_10 | 0xC));
	if(this->actionState == 0)
	{
		func_80248C28(2);
		set_camera_mode(this->area->camera, CAMERA_MODE_C_UP, 0x10);
		this->actionState = 1;
	}
	else
	{
		if(!(this->input & INPUT_FIRST_PERSON) || sp1C)
		{
			func_80248CB8(2);
			// Go back to the last camera mode
			set_camera_mode(this->area->camera, -1, 1);
			return set_mario_action(ACT_IDLE, 0);
		}
	}

	if(this->floor->type == SURFACE_LOOK_UP_WARP)
	{
		if(save_file_get_total_star_count(gCurrSaveFileNum - 1, 0, 0x18) >= 10)
		{
			sp1A = this->statusForCamera->headRotation[0];
			sp18 = ((this->statusForCamera->headRotation[1] * 4) / 3) + this->faceAngle[1];
			if(sp1A == -0x1800)
			{
				if(sp18 < -0x6FFF || sp18 >= 0x7000)
				{
					level_trigger_warp(this, 1);
				}
			}
		}
	}

	stationary_ground_step();
	set_mario_animation(MARIO_ANIM_FIRST_PERSON);
	return 0;
}

s32 MarioState::func_80263378()
{
	if(this->pos[1] < this->waterLevel - 100)
	{
		if(this->action == ACT_SPAWN_SPIN_LANDING)
		{
			func_8024980C(0);
		}
		update_mario_sound_and_camera();
		return set_water_plunge_action();
	}

	if(this->input & INPUT_SQUISHED)
	{
		update_mario_sound_and_camera();
		return drop_and_set_mario_action(ACT_SQUISHED, 0);
	}

	if(this->action != 0x0002020E)
	{
		if(this->health < 0x100)
		{
			update_mario_sound_and_camera();
			return drop_and_set_mario_action(ACT_STANDING_DEATH, 0);
		}
	}
	return 0;
}

s32 MarioState::mario_execute_stationary_action()
{
	s32 sp24;

	if(func_80263378())
	{
		return 1;
	}

	if(mario_update_quicksand(0.5f))
	{
		return 1;
	}

	/* clang-format off */
	switch(this->action)
	{
		case ACT_IDLE:                  sp24 = act_idle();                             break;
		case ACT_START_SLEEPING:        sp24 = act_start_sleeping();                   break;
		case ACT_SLEEPING:              sp24 = act_sleeping();                         break;
		case ACT_WAKING_UP:             sp24 = act_waking_up();                        break;
		case ACT_PANTING:               sp24 = act_panting();                          break;
		case ACT_HOLD_PANTING_UNUSED:   sp24 = act_hold_panting_unused();              break;
		case ACT_HOLD_IDLE:             sp24 = act_hold_idle();                        break;
		case ACT_HOLD_HEAVY_IDLE:       sp24 = act_hold_heavy_idle();                  break;
		case ACT_IN_QUICKSAND:          sp24 = act_in_quicksand();                     break;
		case ACT_STANDING_AGAINST_WALL: sp24 = act_standing_against_wall();            break;
		case ACT_COUGHING:              sp24 = act_coughing();                         break;
		case ACT_SHIVERING:             sp24 = act_shivering();                        break;
		case ACT_CROUCHING:             sp24 = act_crouching();                        break;
		case ACT_START_CROUCHING:       sp24 = act_start_crouching();                  break;
		case ACT_UNKNOWN_022:           sp24 = act_stop_crouching();                   break;
		case ACT_START_CRAWLING:        sp24 = act_start_crawling();                   break;
		case ACT_UNKNOWN_024:           sp24 = act_stop_crawling();                    break;
		case ACT_SLIDE_KICK_SLIDE_STOP: sp24 = act_slide_kick_slide_stop();            break;
		case ACT_UNKNOWN_026:           sp24 = func_80262574();                        break;
		case ACT_FIRST_PERSON:          sp24 = act_first_person();                     break;
		case ACT_JUMP_LAND_STOP:        sp24 = act_jump_land_stop();                   break;
		case ACT_DOUBLE_JUMP_LAND_STOP: sp24 = act_double_jump_land_stop();            break;
		case ACT_FREEFALL_LAND_STOP:    sp24 = act_freefall_land_stop();               break;
		case ACT_SIDE_FLIP_LAND_STOP:   sp24 = act_side_flip_land_stop();              break;
		case ACT_UNKNOWN_034:           sp24 = func_80262CEC();                        break;
		case ACT_UNKNOWN_035:           sp24 = func_80262DE4();                        break;
		case ACT_AIR_THROW_LAND:        sp24 = act_air_throw_land();                   break;
		case ACT_LAVA_BOOST_LAND:       sp24 = act_lava_boost_land();                  break;
		case ACT_TWIRL_LAND:            sp24 = act_twirl_land();                       break;
		case ACT_TRIPLE_JUMP_LAND_STOP: sp24 = act_triple_jump_land_stop();            break;
		case ACT_BACKFLIP_LAND_STOP:    sp24 = act_backflip_land_stop();               break;
		case ACT_LONG_JUMP_LAND_STOP:   sp24 = act_long_jump_land_stop();              break;
		case ACT_GROUND_POUND_LAND:     sp24 = act_ground_pound_land();                break;
		case ACT_BRAKING_STOP:          sp24 = act_braking_stop();                     break;
		case ACT_BUTT_SLIDE_STOP:       sp24 = act_butt_slide_stop();                  break;
		case ACT_UNKNOWN_03F:           sp24 = func_80261F8C();                        break;
	}
	/* clang-format on */

	if(!sp24)
	{
		if(this->input & INPUT_IN_WATER)
		{
			this->particleFlags |= PARTICLE_SURFACE_WAVES;
		}
	}

	return sp24;
}
