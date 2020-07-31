#include <ultra64.h>

#include "sm64.h"
#include "options.h"
#include "area.h"
#include "level_update.h"
#include "engine/math_util.h"
#include "interaction.h"
#include "mario.h"
#include "mario_step.h"
#include "display.h"
#include "camera.h"
#include "save_file.h"
#include "audio/external.h"
#include "engine/graph_node.h"
#include "controller/controller.h"
#include "game/motor.h"

void PlayerRecord::play_flip_sounds(s16 frame1, s16 frame2, s16 frame3)
{
	s32 animFrame = this->marioObj->header.gfx.unk38.frame();
	if(animFrame == frame1 || animFrame == frame2 || animFrame == frame3)
	{
		AudStartSound(SOUND_ACTION_SPIN, this->marioObj->header.gfx.cameraToObject);
	}
}

void PlayerRecord::play_far_fall_sound()
{
	u32 action = this->status;
	if(!(action & ACT_FLAG_INVULNERABLE) && action != ACT_TWIRLING && action != ACT_FLYING && !(this->flags & MARIO_UNKNOWN_18))
	{
		if(this->fallpos - this->pos[1] > 1150.0f)
		{
			AudStartSound(SOUND_MARIO_WAAAOOOW, this->marioObj->header.gfx.cameraToObject);
			this->flags |= MARIO_UNKNOWN_18;
		}
	}
}

void PlayerRecord::func_u_8026A090()
{
	if(this->actionArg == 0 && (this->forwardVel <= -28.0f || this->forwardVel >= 28.0f))
	{
		this->play_sound_if_no_flag(SOUND_MARIO_DOH, MARIO_MARIO_SOUND_PLAYED);
	}
	else
	{
		this->play_sound_if_no_flag(SOUND_MARIO_UH, MARIO_MARIO_SOUND_PLAYED);
	}
}

s32 PlayerRecord::lava_boost_on_wall()
{
	this->faceAngle[1] = atan2s(this->wall->normal.z, this->wall->normal.x);

	if(this->forwardVel < 24.0f)
	{
		this->forwardVel = 24.0f;
	}

	if(!(this->flags & MARIO_METAL_CAP))
	{
		this->hurtCounter += (this->flags & MARIO_CAP_ON_HEAD) ? 12 : 18;
	}

	AudStartSound(SOUND_MARIO_ON_FIRE, this->marioObj->header.gfx.cameraToObject);
	this->update_mario_sound_and_camera();
	return this->drop_and_set_mario_action(ACT_LAVA_BOOST, 1);
}

s32 PlayerRecord::check_fall_damage(u32 hardFallAction)
{
	f32 fallHeight;
	f32 damageHeight;

	fallHeight = this->fallpos - this->pos[1];

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"

	//! Never true
	if(this->actionState == ACT_GROUND_POUND)
	{
		damageHeight = 600.0f;
	}
	else
	{
		damageHeight = 1150.0f;
	}

#pragma GCC diagnostic pop

	if(this->status != ACT_TWIRLING && this->floor->type != SURFACE_BURNING)
	{
		if(this->vel[1] < -55.0f && !sm64::config().cheats().moonJump() && !sm64::config().cheats().superJump())
		{
			if(fallHeight > 3000.0f)
			{
				this->hurtCounter += (this->flags & MARIO_CAP_ON_HEAD) ? 16 : 24;
				Vieweffect(SHAKE_FALL_DAMAGE);
				AudStartSound(SOUND_MARIO_ATTACKED, this->marioObj->header.gfx.cameraToObject);
				SendMotorEvent(5, 80);
				return this->drop_and_set_mario_action(hardFallAction, 4);
			}
			else if(fallHeight > damageHeight && !this->mario_floor_is_slippery())
			{
				this->hurtCounter += (this->flags & MARIO_CAP_ON_HEAD) ? 8 : 12;
				this->squishTimer = 30 * FRAME_RATE_SCALER_INV;
				Vieweffect(SHAKE_FALL_DAMAGE);
				AudStartSound(SOUND_MARIO_ATTACKED, this->marioObj->header.gfx.cameraToObject);
				SendMotorEvent(5, 80);
			}
		}
	}

	return FALSE;
}

s32 PlayerRecord::check_kick_or_dive_in_air()
{
	if(this->input & INPUT_B_PRESSED)
	{
		return this->ChangePlayerStatus(this->forwardVel > 28.0f ? ACT_DIVE : ACT_JUMP_KICK, 0);
	}
	return FALSE;
}

s32 PlayerRecord::should_get_stuck_in_ground()
{
	u32 terrainType	      = this->area->terrainType & TERRAIN_MASK;
	struct Surface* floor = this->floor;
	s32 flags	      = floor->flags;
	s32 type	      = floor->type;

	if(floor != NULL && (terrainType == TERRAIN_SNOW || terrainType == TERRAIN_SAND) && type != SURFACE_BURNING && SURFACE_IS_NOT_HARD(type))
	{
		if(!(flags & 0x01) && this->fallpos - this->pos[1] > 1000.0f && floor->normal.y >= 0.8660254f)
		{
			if(!sm64::config().cheats().moonJump())
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

s32 PlayerRecord::check_fall_damage_or_get_stuck(u32 hardFallAction)
{
	if(should_get_stuck_in_ground())
	{
		AudStartSound(SOUND_MARIO_OOOF2, this->marioObj->header.gfx.cameraToObject);
		SendMotorEvent(5, 80);

		this->particleFlags |= PARTICLE_POUND_WHITE_PUFF;
		this->drop_and_set_mario_action(ACT_FEET_STUCK_IN_GROUND, 0);
		return TRUE;
	}

	return check_fall_damage(hardFallAction);
}

s32 PlayerRecord::check_horizontal_wind()
{
	struct Surface* floor;
	f32 speed;
	s16 pushAngle;

	floor = this->floor;

	if(floor->type == SURFACE_HORIZONTAL_WIND)
	{
		pushAngle = floor->force << 8;

		this->slideVelX += 1.2f * sins(pushAngle) * FRAME_RATE_SCALER;
		this->slideVelZ += 1.2f * coss(pushAngle) * FRAME_RATE_SCALER;

		speed = sqrtf(this->slideVelX * this->slideVelX + this->slideVelZ * this->slideVelZ);

		if(speed > 48.0f)
		{
			this->slideVelX = this->slideVelX * 48.0f / speed;
			this->slideVelZ = this->slideVelZ * 48.0f / speed;
			speed		= 32.0f; //! This was meant to be 48?
		}
		else if(speed > 32.0f)
		{
			speed = 32.0f;
		}

		this->vel[0]	 = this->slideVelX;
		this->vel[2]	 = this->slideVelZ;
		this->slideYaw	 = atan2s(this->slideVelZ, this->slideVelX);
		this->forwardVel = speed * coss(this->faceAngle[1] - this->slideYaw);

		return TRUE;
	}

	return FALSE;
}

void PlayerRecord::update_air_with_turn()
{
	f32 dragThreshold;
	s16 intendedDYaw;
	f32 intendedMag;

	if(!check_horizontal_wind())
	{
		dragThreshold	 = this->status == ACT_LONG_JUMP ? 48.0f : 32.0f;
		this->forwardVel = approach_f32(this->forwardVel, 0.0f, 0.35f * FRAME_RATE_SCALER, 0.35f * FRAME_RATE_SCALER);

		if(this->input & INPUT_NONZERO_ANALOG)
		{
			intendedDYaw = this->intendedYaw - this->faceAngle[1];
			intendedMag  = this->intendedMag / 32.0f;

			this->forwardVel += 1.5f * coss(intendedDYaw) * intendedMag * FRAME_RATE_SCALER;
			this->faceAngle[1] += 512.0f * sins(intendedDYaw) * intendedMag * FRAME_RATE_SCALER;
		}

		//! Uncapped air speed. Net positive when moving forward.
		if(this->forwardVel > dragThreshold)
		{
			this->forwardVel -= 1.0f * FRAME_RATE_SCALER;
		}
		if(this->forwardVel < -16.0f)
		{
			this->forwardVel += 2.0f * FRAME_RATE_SCALER;
		}

		this->vel[0] = this->slideVelX = this->forwardVel * sins(this->faceAngle[1]);
		this->vel[2] = this->slideVelZ = this->forwardVel * coss(this->faceAngle[1]);
	}
}

void PlayerRecord::update_air_without_turn()
{
	f32 sidewaysSpeed = 0.0f;
	f32 dragThreshold;
	s16 intendedDYaw;
	f32 intendedMag;

	if(!check_horizontal_wind())
	{
		dragThreshold	 = this->status == ACT_LONG_JUMP ? 48.0f : 32.0f;
		this->forwardVel = approach_f32(this->forwardVel, 0.0f, 0.35f * FRAME_RATE_SCALER, 0.35f * FRAME_RATE_SCALER);

		if(this->input & INPUT_NONZERO_ANALOG)
		{
			intendedDYaw = this->intendedYaw - this->faceAngle[1];
			intendedMag  = this->intendedMag / 32.0f;

			this->forwardVel += intendedMag * coss(intendedDYaw) * 1.5f * FRAME_RATE_SCALER;
			sidewaysSpeed = intendedMag * sins(intendedDYaw) * 10.0f;
		}

		//! Uncapped air speed. Net positive when moving forward.
		if(this->forwardVel > dragThreshold)
		{
			this->forwardVel -= 1.0f * FRAME_RATE_SCALER;
		}
		if(this->forwardVel < -16.0f)
		{
			this->forwardVel += 2.0f * FRAME_RATE_SCALER;
		}

		this->slideVelX = this->forwardVel * sins(this->faceAngle[1]);
		this->slideVelZ = this->forwardVel * coss(this->faceAngle[1]);

		this->slideVelX += sidewaysSpeed * sins(this->faceAngle[1] + 0x4000) * FRAME_RATE_SCALER;
		this->slideVelZ += sidewaysSpeed * coss(this->faceAngle[1] + 0x4000) * FRAME_RATE_SCALER;

		this->vel[0] = this->slideVelX;
		this->vel[2] = this->slideVelZ;
	}
}

void PlayerRecord::update_lava_boost_or_twirling()
{
	s16 intendedDYaw;
	f32 intendedMag;

	if(this->input & INPUT_NONZERO_ANALOG)
	{
		intendedDYaw = this->intendedYaw - this->faceAngle[1];
		intendedMag  = this->intendedMag / 32.0f;

		this->forwardVel += coss(intendedDYaw) * intendedMag * FRAME_RATE_SCALER;
		this->faceAngle[1] += sins(intendedDYaw) * intendedMag * 1024.0f * FRAME_RATE_SCALER;

		if(this->forwardVel < 0.0f)
		{
			this->faceAngle[1] += 0x8000;
			this->forwardVel *= -1.0f;
		}

		if(this->forwardVel > 32.0f)
		{
			this->forwardVel -= 2.0f * FRAME_RATE_SCALER;
		}
	}

	this->vel[0] = this->slideVelX = this->forwardVel * sins(this->faceAngle[1]);
	this->vel[2] = this->slideVelZ = this->forwardVel * coss(this->faceAngle[1]);
}

void PlayerRecord::update_flying_yaw()
{
	s16 targetYawVel = -(s16)(this->controller->stickX * (this->forwardVel / 4.0f));

	if(targetYawVel > 0)
	{
		if(this->angleVel[1] < 0)
		{
			this->angleVel[1] += 0x40 / FRAME_RATE_SCALER;

			if(this->angleVel[1] > 0x10)
			{
				this->angleVel[1] = 0x10;
			}
		}
		else
		{
			this->angleVel[1] = approach_s32(this->angleVel[1], targetYawVel, 0x10 / FRAME_RATE_SCALER_INV, 0x20 / FRAME_RATE_SCALER_INV);
		}
	}
	else if(targetYawVel < 0)
	{
		if(this->angleVel[1] > 0)
		{
			this->angleVel[1] -= 0x40 / FRAME_RATE_SCALER_INV;
			if(this->angleVel[1] < -0x10)
			{
				this->angleVel[1] = -0x10;
			}
		}
		else
		{
			this->angleVel[1] = approach_s32(this->angleVel[1], targetYawVel, 0x20 / FRAME_RATE_SCALER_INV, 0x10 / FRAME_RATE_SCALER_INV);
		}
	}
	else
	{
		this->angleVel[1] = approach_s32(this->angleVel[1], 0, 0x40 / FRAME_RATE_SCALER_INV, 0x40 / FRAME_RATE_SCALER_INV);
	}

	this->faceAngle[1] += this->angleVel[1] / FRAME_RATE_SCALER_INV;
	this->faceAngle[2] = 20 * -this->angleVel[1];
}

void PlayerRecord::update_flying_pitch()
{
	s16 targetPitchVel = -(s16)(this->controller->stickY * (this->forwardVel / 5.0f));

	if(targetPitchVel > 0)
	{
		if(this->angleVel[0] < 0)
		{
			this->angleVel[0] += 0x40 / FRAME_RATE_SCALER_INV;
			if(this->angleVel[0] > 0x20)
			{
				this->angleVel[0] = 0x20;
			}
		}
		else
		{
			this->angleVel[0] = approach_s32(this->angleVel[0], targetPitchVel, 0x20 / FRAME_RATE_SCALER_INV, 0x40 / FRAME_RATE_SCALER_INV);
		}
	}
	else if(targetPitchVel < 0)
	{
		if(this->angleVel[0] > 0)
		{
			this->angleVel[0] -= 0x40 / FRAME_RATE_SCALER_INV;
			if(this->angleVel[0] < -0x20)
			{
				this->angleVel[0] = -0x20;
			}
		}
		else
		{
			this->angleVel[0] = approach_s32(this->angleVel[0], targetPitchVel, 0x40 / FRAME_RATE_SCALER_INV, 0x20 / FRAME_RATE_SCALER_INV);
		}
	}
	else
	{
		this->angleVel[0] = approach_s32(this->angleVel[0], 0, 0x40 / FRAME_RATE_SCALER_INV, 0x40 / FRAME_RATE_SCALER_INV);
	}
}

void PlayerRecord::update_flying()
{
	update_flying_pitch();
	update_flying_yaw();

	this->forwardVel -= (2.0f * ((f32)this->faceAngle[0] / 0x4000) + 0.1f) * FRAME_RATE_SCALER;
	this->forwardVel -= (0.5f * (1.0f - coss(this->angleVel[1]))) * FRAME_RATE_SCALER;

	if(this->forwardVel < 0.0f)
	{
		this->forwardVel = 0.0f;
	}

	if(this->forwardVel > 16.0f)
	{
		this->faceAngle[0] += (this->forwardVel - 32.0f) * 6.0f * FRAME_RATE_SCALER;
	}
	else if(this->forwardVel > 4.0f)
	{
		this->faceAngle[0] += (this->forwardVel - 32.0f) * 10.0f * FRAME_RATE_SCALER;
	}
	else
	{
		this->faceAngle[0] -= 0x400 / FRAME_RATE_SCALER_INV;
	}

	this->faceAngle[0] += this->angleVel[0] / FRAME_RATE_SCALER_INV;

	if(this->faceAngle[0] > 0x2AAA)
	{
		this->faceAngle[0] = 0x2AAA;
	}
	if(this->faceAngle[0] < -0x2AAA)
	{
		this->faceAngle[0] = -0x2AAA;
	}

	this->vel[0] = this->forwardVel * coss(this->faceAngle[0]) * sins(this->faceAngle[1]);
	this->vel[1] = this->forwardVel * sins(this->faceAngle[0]);
	this->vel[2] = this->forwardVel * coss(this->faceAngle[0]) * coss(this->faceAngle[1]);

	this->slideVelX = this->vel[0];
	this->slideVelZ = this->vel[2];
}

u32 PlayerRecord::common_air_action_step(u32 landAction, s32 animation, u32 stepArg)
{
	u32 stepResult;

	update_air_without_turn();

	stepResult = perform_air_step(stepArg);
	switch(stepResult)
	{
		case AIR_STEP_NONE:
			this->set_mario_animation(animation);
			break;

		case AIR_STEP_LANDED:
			if(!check_fall_damage_or_get_stuck(ACT_HARD_BACKWARD_GROUND_KB))
			{
				this->ChangePlayerStatus(landAction, 0);
			}
			break;

		case AIR_STEP_HIT_WALL:
			this->set_mario_animation(animation);

			if(this->forwardVel > 16.0f)
			{
				mario_bonk_reflection(FALSE);
				SendMotorEvent(5, 40);

				this->faceAngle[1] += 0x8000;

				if(this->wall != NULL)
				{
					this->ChangePlayerStatus(ACT_AIR_HIT_WALL, 0);
				}
				else
				{
					if(this->vel[1] > 0.0f)
					{
						this->vel[1] = 0.0f;
					}

					//! Hands-free holding. Bonking while no wall is referenced
					// sets Mario's action to a non-holding action without
					// dropping the object, causing the hands-free holding
					// glitch. This can be achieved using an exposed ceiling,
					// out of bounds, grazing the bottom of a wall while
					// falling such that the final quarter step does not find a
					// wall collision, or by rising into the top of a wall such
					// that the final quarter step detects a ledge, but you are
					// not able to ledge grab it.
					if(this->forwardVel >= 38.0f)
					{
						this->particleFlags |= PARTICLE_WALL_TINY_STAR;
						this->ChangePlayerStatus(ACT_BACKWARD_AIR_KB, 0);
					}
					else
					{
						if(this->forwardVel > 8.0f)
						{
							this->mario_set_forward_vel(-8.0f);
						}
						return this->ChangePlayerStatus(ACT_SOFT_BONK, 0);
					}
				}
			}
			else
			{
				this->mario_set_forward_vel(0.0f);
			}
			break;

		case AIR_STEP_GRABBED_LEDGE:
			this->set_mario_animation(MARIO_ANIM_IDLE_ON_LEDGE);
			this->drop_and_set_mario_action(ACT_LEDGE_GRAB, 0);
			break;

		case AIR_STEP_GRABBED_CEILING:
			this->ChangePlayerStatus(ACT_START_HANGING, 0);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	return stepResult;
}

s32 PlayerRecord::act_jump()
{
	if(check_kick_or_dive_in_air())
	{
		return TRUE;
	}

	if(this->input & INPUT_Z_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_GROUND_POUND, 0);
	}

	this->play_mario_sound(SOUND_ACTION_TERRAIN_JUMP, 0);
	common_air_action_step(ACT_JUMP_LAND, MARIO_ANIM_SINGLE_JUMP, AIR_STEP_CHECK_LEDGE_GRAB | AIR_STEP_CHECK_HANG);
	return FALSE;
}

s32 PlayerRecord::act_double_jump()
{
	s32 animation;

	if(this->vel[1] >= 0.0f)
	{
		animation = MARIO_ANIM_DOUBLE_JUMP_RISE;
	}
	else
	{
		animation = MARIO_ANIM_DOUBLE_JUMP_FALL;
	}

	if(check_kick_or_dive_in_air())
	{
		return TRUE;
	}

	if(this->input & INPUT_Z_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_GROUND_POUND, 0);
	}

	this->play_mario_sound(SOUND_ACTION_TERRAIN_JUMP, SOUND_MARIO_HOOHOO);
	common_air_action_step(ACT_DOUBLE_JUMP_LAND, animation, AIR_STEP_CHECK_LEDGE_GRAB | AIR_STEP_CHECK_HANG);
	return FALSE;
}

s32 PlayerRecord::act_triple_jump()
{
	if(gSpecialTripleJump)
	{
		return this->ChangePlayerStatus(ACT_SPECIAL_TRIPLE_JUMP, 0);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_DIVE, 0);
	}

	if(this->input & INPUT_Z_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_GROUND_POUND, 0);
	}

	this->play_mario_sound(SOUND_ACTION_TERRAIN_JUMP, 0);

	common_air_action_step(ACT_TRIPLE_JUMP_LAND, MARIO_ANIM_TRIPLE_JUMP, 0);
	play_flip_sounds(2, 8, 20);

	if(status == ACT_TRIPLE_JUMP_LAND)
	{
		SendMotorEvent(5, 40);
	}

	return FALSE;
}

s32 PlayerRecord::act_backflip()
{
	if(this->input & INPUT_Z_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_GROUND_POUND, 0);
	}

	this->play_mario_sound(SOUND_ACTION_TERRAIN_JUMP, SOUND_MARIO_YAH_WAH_HOO);
	common_air_action_step(ACT_BACKFLIP_LAND, MARIO_ANIM_BACKFLIP, 0);
	play_flip_sounds(2, 3, 17);

	if(status == ACT_BACKFLIP_LAND)
	{
		SendMotorEvent(5, 40);
	}
	return FALSE;
}

s32 PlayerRecord::act_freefall()
{
	s32 animation;

	if(this->input & INPUT_B_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_DIVE, 0);
	}

	if(this->input & INPUT_Z_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_GROUND_POUND, 0);
	}

	switch(this->actionArg)
	{
		case 0:
			animation = MARIO_ANIM_GENERAL_FALL;
			break;
		case 1:
			animation = MARIO_ANIM_FALL_FROM_SLIDE;
			break;
		case 2:
			animation = MARIO_ANIM_FALL_FROM_SLIDE_KICK;
			break;
	}

	common_air_action_step(ACT_FREEFALL_LAND, animation, AIR_STEP_CHECK_LEDGE_GRAB);
	return FALSE;
}

s32 PlayerRecord::act_hold_jump()
{
	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return this->drop_and_set_mario_action(ACT_FREEFALL, 0);
	}

	if((this->input & INPUT_B_PRESSED) && !(this->heldObj->oInteractionSubtype & INT_SUBTYPE_HOLDABLE_NPC))
	{
		return this->ChangePlayerStatus(ACT_AIR_THROW, 0);
	}

	if(this->input & INPUT_Z_PRESSED)
	{
		return this->drop_and_set_mario_action(ACT_GROUND_POUND, 0);
	}

	this->play_mario_sound(SOUND_ACTION_TERRAIN_JUMP, 0);
	common_air_action_step(ACT_HOLD_JUMP_LAND, MARIO_ANIM_JUMP_WITH_LIGHT_OBJ, AIR_STEP_CHECK_LEDGE_GRAB);
	return FALSE;
}

s32 PlayerRecord::act_hold_freefall()
{
	s32 animation;
	if(this->actionArg == 0)
	{
		animation = MARIO_ANIM_FALL_WITH_LIGHT_OBJ;
	}
	else
	{
		animation = MARIO_ANIM_FALL_FROM_SLIDING_WITH_LIGHT_OBJ;
	}

	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return this->drop_and_set_mario_action(ACT_FREEFALL, 0);
	}

	if((this->input & INPUT_B_PRESSED) && !(this->heldObj->oInteractionSubtype & INT_SUBTYPE_HOLDABLE_NPC))
	{
		return this->ChangePlayerStatus(ACT_AIR_THROW, 0);
	}

	if(this->input & INPUT_Z_PRESSED)
	{
		return this->drop_and_set_mario_action(ACT_GROUND_POUND, 0);
	}

	common_air_action_step(ACT_HOLD_FREEFALL_LAND, animation, AIR_STEP_CHECK_LEDGE_GRAB);
	return FALSE;
}

s32 PlayerRecord::act_side_flip()
{
	if(this->input & INPUT_B_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_DIVE, 0);
	}

	if(this->input & INPUT_Z_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_GROUND_POUND, 0);
	}

	this->play_mario_sound(SOUND_ACTION_TERRAIN_JUMP, 0);

	if(common_air_action_step(ACT_SIDE_FLIP_LAND, MARIO_ANIM_SLIDEFLIP, AIR_STEP_CHECK_LEDGE_GRAB) != AIR_STEP_GRABBED_LEDGE)
	{
		this->marioObj->header.gfx.angle[1] += 0x8000;
	}

	// (this need to be on one line to match on PAL)
	// clang-format off
	if(this->marioObj->header.gfx.unk38.frame() == 6) AudStartSound(SOUND_ACTION_SIDE_FLIP_UNK, this->marioObj->header.gfx.cameraToObject);
	// clang-format on
	return FALSE;
}

s32 PlayerRecord::act_wall_kick_air()
{
	if(this->input & INPUT_B_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_DIVE, 0);
	}

	if(this->input & INPUT_Z_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_GROUND_POUND, 0);
	}

	this->play_mario_jump_sound();
	common_air_action_step(ACT_JUMP_LAND, MARIO_ANIM_SLIDEJUMP, AIR_STEP_CHECK_LEDGE_GRAB);
	return FALSE;
}

s32 PlayerRecord::act_long_jump()
{
	s32 animation;
	if(!this->marioObj->oMarioLongJumpIsSlow)
	{
		animation = MARIO_ANIM_FAST_LONGJUMP;
	}
	else
	{
		animation = MARIO_ANIM_SLOW_LONGJUMP;
	}

	this->play_mario_sound(SOUND_ACTION_TERRAIN_JUMP, SOUND_MARIO_YAHOO);

	if(this->floor->type == SURFACE_VERTICAL_WIND && this->actionState == 0)
	{
		AudStartSound(SOUND_MARIO_HERE_WE_GO, this->marioObj->header.gfx.cameraToObject);
		this->actionState = 1;
	}

	common_air_action_step(ACT_LONG_JUMP_LAND, animation, AIR_STEP_CHECK_LEDGE_GRAB);

	if(status == ACT_LONG_JUMP_LAND)
	{
		SendMotorEvent(5, 40);
	}

	return FALSE;
}

s32 PlayerRecord::act_riding_shell_air()
{
	this->play_mario_sound(SOUND_ACTION_TERRAIN_JUMP, 0);
	this->set_mario_animation(MARIO_ANIM_JUMP_RIDING_SHELL);

	update_air_without_turn();

	switch(perform_air_step(0))
	{
		case AIR_STEP_LANDED:
			this->ChangePlayerStatus(ACT_RIDING_SHELL_GROUND, 1);
			break;

		case AIR_STEP_HIT_WALL:
			this->mario_set_forward_vel(0.0f);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	this->marioObj->header.gfx.pos[1] += 42.0f * FRAME_RATE_SCALER;
	return FALSE;
}

s32 PlayerRecord::act_twirling()
{
	s16 startTwirlYaw = this->twirlYaw;
	s16 yawVelTarget;

	if(this->input & INPUT_A_DOWN)
	{
		yawVelTarget = 0x2000;
	}
	else
	{
		yawVelTarget = 0x1800;
	}

	this->angleVel[1] = approach_s32(this->angleVel[1], yawVelTarget, 0x200 / FRAME_RATE_SCALER_INV, 0x200 / FRAME_RATE_SCALER_INV);
	this->twirlYaw += this->angleVel[1] / FRAME_RATE_SCALER_INV;

	this->set_mario_animation(this->actionArg == 0 ? MARIO_ANIM_START_TWIRL : MARIO_ANIM_TWIRL);
	if(this->is_anim_past_end())
	{
		this->actionArg = 1;
	}

	if(startTwirlYaw > this->twirlYaw)
	{
		AudStartSound(SOUND_ACTION_TWIRL, this->marioObj->header.gfx.cameraToObject);
	}

	update_lava_boost_or_twirling();

	switch(perform_air_step(0))
	{
		case AIR_STEP_LANDED:
			this->ChangePlayerStatus(ACT_TWIRL_LAND, 0);
			break;

		case AIR_STEP_HIT_WALL:
			mario_bonk_reflection(FALSE);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	this->marioObj->header.gfx.angle[1] += this->twirlYaw / FRAME_RATE_SCALER_INV;
	return FALSE;
}

s32 PlayerRecord::act_dive()
{
	if(this->actionArg == 0)
	{
		this->play_mario_sound(SOUND_ACTION_THROW, SOUND_MARIO_HOOHOO);
	}
	else
	{
		this->play_mario_sound(SOUND_ACTION_TERRAIN_JUMP, 0);
	}

	this->set_mario_animation(MARIO_ANIM_DIVE);
	if(mario_check_object_grab(this))
	{
		mario_grab_used_object(this);
		this->marioBodyState->grabPos = GRAB_POS_LIGHT_OBJ;
		if(this->status != ACT_DIVE)
		{
			return TRUE;
		}
	}

	update_air_without_turn();

	switch(perform_air_step(0))
	{
		case AIR_STEP_NONE:
			if(this->vel[1] < 0.0f && this->faceAngle[0] > -0x2AAA)
			{
				this->faceAngle[0] -= 0x200 / FRAME_RATE_SCALER_INV;
				if(this->faceAngle[0] < -0x2AAA)
				{
					this->faceAngle[0] = -0x2AAA;
				}
			}
			this->marioObj->header.gfx.angle[0] = -this->faceAngle[0];
			break;

		case AIR_STEP_LANDED:
			if(should_get_stuck_in_ground() && this->faceAngle[0] == -0x2AAA)
			{
				AudStartSound(SOUND_MARIO_OOOF2, this->marioObj->header.gfx.cameraToObject);
				SendMotorEvent(5, 80);

				this->particleFlags |= PARTICLE_POUND_WHITE_PUFF;
				this->drop_and_set_mario_action(ACT_HEAD_STUCK_IN_GROUND, 0);
			}
			else if(!check_fall_damage(ACT_HARD_FORWARD_GROUND_KB))
			{
				if(this->heldObj == NULL)
				{
					this->ChangePlayerStatus(ACT_DIVE_SLIDE, 0);
				}
				else
				{
					this->ChangePlayerStatus(ACT_DIVE_PICKING_UP, 0);
				}
			}
			this->faceAngle[0] = 0;
			break;

		case AIR_STEP_HIT_WALL:
			mario_bonk_reflection(TRUE);
			this->faceAngle[0] = 0;

			if(this->vel[1] > 0.0f)
			{
				this->vel[1] = 0.0f;
			}

			this->particleFlags |= PARTICLE_WALL_TINY_STAR;
			this->drop_and_set_mario_action(ACT_BACKWARD_AIR_KB, 0);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	return FALSE;
}

s32 PlayerRecord::act_air_throw()
{
	if(++(this->actionTimer) == 4 * FRAME_RATE_SCALER_INV)
	{
		mario_throw_held_object(this);
	}

	this->play_sound_if_no_flag(SOUND_MARIO_WAH2, MARIO_MARIO_SOUND_PLAYED);
	this->set_mario_animation(MARIO_ANIM_THROW_LIGHT_OBJECT);
	update_air_without_turn();

	switch(perform_air_step(0))
	{
		case AIR_STEP_LANDED:
			if(!check_fall_damage_or_get_stuck(ACT_HARD_BACKWARD_GROUND_KB))
			{
				this->status = ACT_AIR_THROW_LAND;
			}
			break;

		case AIR_STEP_HIT_WALL:
			this->mario_set_forward_vel(0.0f);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	return FALSE;
}

s32 PlayerRecord::act_water_jump()
{
	if(this->forwardVel < 15.0f)
	{
		this->mario_set_forward_vel(15.0f);
	}

	this->play_mario_sound(SOUND_ACTION_UNKNOWN432, 0);
	this->set_mario_animation(MARIO_ANIM_SINGLE_JUMP);

	switch(perform_air_step(AIR_STEP_CHECK_LEDGE_GRAB))
	{
		case AIR_STEP_LANDED:
			this->ChangePlayerStatus(ACT_JUMP_LAND, 0);
			set_camera_mode(this->area->camera, this->area->camera->defMode, 1);
			break;

		case AIR_STEP_HIT_WALL:
			this->mario_set_forward_vel(15.0f);
			break;

		case AIR_STEP_GRABBED_LEDGE:
			this->set_mario_animation(MARIO_ANIM_IDLE_ON_LEDGE);
			this->ChangePlayerStatus(ACT_LEDGE_GRAB, 0);
			set_camera_mode(this->area->camera, this->area->camera->defMode, 1);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	return FALSE;
}

s32 PlayerRecord::act_hold_water_jump()
{
	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return this->drop_and_set_mario_action(ACT_FREEFALL, 0);
	}

	if(this->forwardVel < 15.0f)
	{
		this->mario_set_forward_vel(15.0f);
	}

	this->play_mario_sound(SOUND_ACTION_UNKNOWN432, 0);
	this->set_mario_animation(MARIO_ANIM_JUMP_WITH_LIGHT_OBJ);

	switch(perform_air_step(0))
	{
		case AIR_STEP_LANDED:
			this->ChangePlayerStatus(ACT_HOLD_JUMP_LAND, 0);
			set_camera_mode(this->area->camera, this->area->camera->defMode, 1);
			break;

		case AIR_STEP_HIT_WALL:
			this->mario_set_forward_vel(15.0f);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	return FALSE;
}

s32 PlayerRecord::act_steep_jump()
{
	if(this->input & INPUT_B_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_DIVE, 0);
	}

	this->play_mario_sound(SOUND_ACTION_TERRAIN_JUMP, 0);
	this->mario_set_forward_vel(0.98f * this->forwardVel);

	switch(perform_air_step(0))
	{
		case AIR_STEP_LANDED:
			if(!check_fall_damage_or_get_stuck(ACT_HARD_BACKWARD_GROUND_KB))
			{
				this->faceAngle[0] = 0;
				this->ChangePlayerStatus(this->forwardVel < 0.0f ? ACT_BEGIN_SLIDING : ACT_JUMP_LAND, 0);
			}
			break;

		case AIR_STEP_HIT_WALL:
			this->mario_set_forward_vel(0.0f);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	this->set_mario_animation(MARIO_ANIM_SINGLE_JUMP);
	this->marioObj->header.gfx.angle[1] = this->marioObj->oMarioSteepJumpYaw;
	return FALSE;
}

s32 PlayerRecord::act_ground_pound()
{
	u32 stepResult;
	f32 yOffset;

	this->play_sound_if_no_flag(SOUND_ACTION_THROW, MARIO_ACTION_SOUND_PLAYED);

	if(this->actionState == 0)
	{
		if(this->actionTimer < 10 /** FRAME_RATE_SCALER_INV*/)
		{
			yOffset = 20 - 2 * (this->actionTimer /*/ FRAME_RATE_SCALER_INV*/);
			if(this->pos[1] + yOffset + 160.0f < this->ceilHeight)
			{
				this->pos[1] += yOffset;
				this->fallpos = this->pos[1];
				vec3f_copy(this->marioObj->header.gfx.pos, this->pos);
			}
		}

		this->vel[1] = -50.0f;
		this->mario_set_forward_vel(0.0f);

		this->set_mario_animation(this->actionArg == 0 ? MARIO_ANIM_START_GROUND_POUND : MARIO_ANIM_TRIPLE_JUMP_GROUND_POUND);
		if(this->actionTimer == 0)
		{
			AudStartSound(SOUND_ACTION_SPIN, this->marioObj->header.gfx.cameraToObject);
		}

		this->actionTimer++;

		if(this->actionTimer >= (this->marioObj->header.gfx.unk38.curAnim->lastFrame + 4) * 2)
		{
			AudStartSound(SOUND_MARIO_GROUND_POUND_WAH, this->marioObj->header.gfx.cameraToObject);
			this->actionState = 1;
		}
	}
	else
	{
		this->set_mario_animation(MARIO_ANIM_GROUND_POUND);

		stepResult = perform_air_step(0);
		if(stepResult == AIR_STEP_LANDED)
		{
			if(should_get_stuck_in_ground())
			{
				AudStartSound(SOUND_MARIO_OOOF2, this->marioObj->header.gfx.cameraToObject);
				SendMotorEvent(5, 80);
				this->particleFlags |= PARTICLE_POUND_WHITE_PUFF;
				this->ChangePlayerStatus(ACT_BUTT_STUCK_IN_GROUND, 0);
			}
			else
			{
				this->play_mario_heavy_landing_sound(SOUND_ACTION_TERRAIN_HEAVY_LANDING);
				if(!check_fall_damage(ACT_HARD_BACKWARD_GROUND_KB))
				{
					this->particleFlags |= PARTICLE_POUND_WHITE_PUFF | PARTICLE_POUND_TINY_STAR;
					this->ChangePlayerStatus(ACT_GROUND_POUND_LAND, 0);
				}
			}
			Vieweffect(SHAKE_GROUND_POUND);
		}
		else if(stepResult == AIR_STEP_HIT_WALL)
		{
			this->mario_set_forward_vel(-16.0f);
			if(this->vel[1] > 0.0f)
			{
				this->vel[1] = 0.0f;
			}

			this->particleFlags |= PARTICLE_WALL_TINY_STAR;
			this->ChangePlayerStatus(ACT_BACKWARD_AIR_KB, 0);
		}
	}

	return FALSE;
}

s32 PlayerRecord::act_burning_jump()
{
	this->play_mario_sound(SOUND_ACTION_TERRAIN_JUMP, this->actionArg == 0 ? 0 : -1);
	this->mario_set_forward_vel(this->forwardVel);

	if(perform_air_step(0) == AIR_STEP_LANDED)
	{
		this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
		this->ChangePlayerStatus(ACT_BURNING_GROUND, 0);
	}

	this->set_mario_animation(this->actionArg == 0 ? MARIO_ANIM_SINGLE_JUMP : MARIO_ANIM_FIRE_LAVA_BURN);
	this->particleFlags |= PARTICLE_FLAME;
	AudStartSound(SOUND_MOVING_LAVA_BURN, this->marioObj->header.gfx.cameraToObject);

	this->marioObj->oMarioBurnTimer += 3 * FRAME_RATE_SCALER_INV;

	if(!sm64::config().cheats().invincible())
	{
		this->health -= 10;
	}

	if(this->health < 0x100)
	{
		this->health = 0xFF;
	}
	return FALSE;
}

s32 PlayerRecord::act_burning_fall()
{
	this->mario_set_forward_vel(this->forwardVel);

	if(perform_air_step(0) == AIR_STEP_LANDED)
	{
		this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
		this->ChangePlayerStatus(ACT_BURNING_GROUND, 0);
	}

	this->set_mario_animation(MARIO_ANIM_GENERAL_FALL);
	this->particleFlags |= PARTICLE_FLAME;
	this->marioObj->oMarioBurnTimer += 3 * FRAME_RATE_SCALER_INV;

	if(!sm64::config().cheats().invincible())
	{
		this->health -= 10;
	}

	if(this->health < 0x100)
	{
		this->health = 0xFF;
	}
	return FALSE;
}

s32 PlayerRecord::act_crazy_box_bounce()
{
	f32 minSpeed;

	if(this->actionTimer == 0)
	{
		switch(this->actionArg)
		{
			case 0:
				this->vel[1] = 45.0f;
				minSpeed     = 32.0f;
				break;

			case 1:
				this->vel[1] = 60.0f;
				minSpeed     = 36.0f;
				break;

			case 2:
				this->vel[1] = 100.0f;
				minSpeed     = 48.0f;
				break;
		}

		AudStartSound(minSpeed < 40.0f ? SOUND_GENERAL_BOING1 : SOUND_GENERAL_BOING2, this->marioObj->header.gfx.cameraToObject);

		if(this->forwardVel < minSpeed)
		{
			this->mario_set_forward_vel(minSpeed);
		}

		this->actionTimer = 1 * FRAME_RATE_SCALER_INV;
	}

	this->play_mario_sound(SOUND_ACTION_TERRAIN_JUMP, 0);
	this->set_mario_animation(MARIO_ANIM_DIVE);

	update_air_without_turn();

	switch(perform_air_step(0))
	{
		case AIR_STEP_LANDED:
			if(this->actionArg < 2)
			{
				this->ChangePlayerStatus(ACT_CRAZY_BOX_BOUNCE, this->actionArg + 1);
			}
			else
			{
				this->heldObj->oInteractStatus = INT_STATUS_STOP_RIDING;
				this->heldObj		       = NULL;
				this->ChangePlayerStatus(ACT_STOMACH_SLIDE, 0);
			}
			SendMotorEvent(5, 80);
			this->particleFlags |= PARTICLE_POUND_WHITE_PUFF;
			break;

		case AIR_STEP_HIT_WALL:
			mario_bonk_reflection(FALSE);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	this->marioObj->header.gfx.angle[0] = atan2s(this->forwardVel, -this->vel[1]);
	return FALSE;
}

u32 PlayerRecord::common_air_knockback_step(u32 landAction, u32 hardFallAction, s32 animation, f32 speed)
{
	u32 stepResult;

	this->mario_set_forward_vel(speed);

	stepResult = perform_air_step(0);
	switch(stepResult)
	{
		case AIR_STEP_NONE:
			this->set_mario_animation(animation);
			break;

		case AIR_STEP_LANDED:
			if(status == ACT_SOFT_BONK)
			{
				SendMotorEvent(5, 80);
			}
			if(!check_fall_damage_or_get_stuck(hardFallAction))
			{
				if(this->status == ACT_THROWN_FORWARD || this->status == ACT_THROWN_BACKWARD)
				{
					this->ChangePlayerStatus(landAction, this->hurtCounter);
				}
				else
				{
					this->ChangePlayerStatus(landAction, this->actionArg);
				}
			}
			break;

		case AIR_STEP_HIT_WALL:
			this->set_mario_animation(MARIO_ANIM_BACKWARD_AIR_KB);
			mario_bonk_reflection(FALSE);

			if(this->vel[1] > 0.0f)
			{
				this->vel[1] = 0.0f;
			}

			this->mario_set_forward_vel(-speed);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	return stepResult;
}

s32 PlayerRecord::func_8026CDFC()
{
	if((this->input & INPUT_A_PRESSED) && this->wallKickTimer != 0 && this->oldstatus == ACT_AIR_HIT_WALL)
	{
		this->faceAngle[1] += 0x8000;
		return this->ChangePlayerStatus(ACT_WALL_KICK_AIR, 0);
	}

	return FALSE;
}

s32 PlayerRecord::act_backward_air_kb()
{
	if(func_8026CDFC())
	{
		return 1;
	}

	func_u_8026A090();
	common_air_knockback_step(ACT_BACKWARD_GROUND_KB, ACT_HARD_BACKWARD_GROUND_KB, 0x0002, -16.0f);
	return FALSE;
}

s32 PlayerRecord::act_forward_air_kb()
{
	if(func_8026CDFC())
	{
		return 1;
	}

	func_u_8026A090();
	common_air_knockback_step(ACT_FORWARD_GROUND_KB, ACT_HARD_FORWARD_GROUND_KB, 0x002D, 16.0f);
	return FALSE;
}

s32 PlayerRecord::act_hard_backward_air_kb()
{
	func_u_8026A090();
	common_air_knockback_step(ACT_HARD_BACKWARD_GROUND_KB, ACT_HARD_BACKWARD_GROUND_KB, 0x0002, -16.0f);
	return FALSE;
}

s32 PlayerRecord::act_hard_forward_air_kb()
{
	func_u_8026A090();
	common_air_knockback_step(ACT_HARD_FORWARD_GROUND_KB, ACT_HARD_FORWARD_GROUND_KB, 0x002D, 16.0f);
	return FALSE;
}

s32 PlayerRecord::act_thrown_backward()
{
	u32 landAction;
	if(this->actionArg != 0)
	{
		landAction = ACT_HARD_BACKWARD_GROUND_KB;
	}
	else
	{
		landAction = ACT_BACKWARD_GROUND_KB;
	}

	this->play_sound_if_no_flag(SOUND_MARIO_WAAAOOOW, MARIO_MARIO_SOUND_PLAYED);

	common_air_knockback_step(landAction, ACT_HARD_BACKWARD_GROUND_KB, 0x0002, this->forwardVel);

	this->forwardVel *= 0.98f;
	return FALSE;
}

s32 PlayerRecord::act_thrown_forward()
{
	s16 pitch;

	u32 landAction;
	if(this->actionArg != 0)
	{
		landAction = ACT_HARD_FORWARD_GROUND_KB;
	}
	else
	{
		landAction = ACT_FORWARD_GROUND_KB;
	}

	this->play_sound_if_no_flag(SOUND_MARIO_WAAAOOOW, MARIO_MARIO_SOUND_PLAYED);

	if(common_air_knockback_step(landAction, ACT_HARD_FORWARD_GROUND_KB, 0x002D, this->forwardVel) == AIR_STEP_NONE)
	{
		pitch = atan2s(this->forwardVel, -this->vel[1]);
		if(pitch > 0x1800)
		{
			pitch = 0x1800;
		}

		this->marioObj->header.gfx.angle[0] = pitch + 0x1800;
	}

	this->forwardVel *= 0.98f;
	return FALSE;
}

s32 PlayerRecord::act_soft_bonk()
{
	if(func_8026CDFC())
	{
		return 1;
	}

	func_u_8026A090();

	common_air_knockback_step(ACT_FREEFALL_LAND, ACT_HARD_BACKWARD_GROUND_KB, 0x0056, this->forwardVel);
	return FALSE;
}

s32 PlayerRecord::act_getting_blown()
{
	if(this->actionState == 0)
	{
		if(this->forwardVel > -60.0f)
		{
			this->forwardVel -= 6.0f * FRAME_RATE_SCALER;
		}
		else
		{
			this->actionState = 1;
		}
	}
	else
	{
		if(this->forwardVel < -16.0f)
		{
			this->forwardVel += 0.8f * FRAME_RATE_SCALER;
		}

		if(this->vel[1] < 0.0f && this->gravity < 4.0f)
		{
			this->gravity += 0.05f * FRAME_RATE_SCALER;
		}
	}

	if(++(this->actionTimer) == 20 * FRAME_RATE_SCALER_INV)
	{
		mario_blow_off_cap(this, 50.0f);
	}

	this->mario_set_forward_vel(this->forwardVel);
	this->set_mario_animation(MARIO_ANIM_BACKWARD_AIR_KB);

	switch(perform_air_step(0))
	{
		case AIR_STEP_LANDED:
			this->ChangePlayerStatus(ACT_HARD_BACKWARD_AIR_KB, 0);
			break;

		case AIR_STEP_HIT_WALL:
			this->set_mario_animation(MARIO_ANIM_AIR_FORWARD_KB);
			mario_bonk_reflection(FALSE);

			if(this->vel[1] > 0.0f)
			{
				this->vel[1] = 0.0f;
			}

			this->mario_set_forward_vel(-this->forwardVel);
			break;
	}

	return FALSE;
}

s32 PlayerRecord::act_air_hit_wall()
{
	if(this->heldObj != NULL)
	{
		mario_drop_held_object(this);
	}

	if(++(this->actionTimer) <= 3 * FRAME_RATE_SCALER_INV)
	{
		if(this->input & INPUT_A_PRESSED)
		{
			this->vel[1] = 52.0f;
			this->faceAngle[1] += 0x8000;
			return this->ChangePlayerStatus(ACT_WALL_KICK_AIR, 0);
		}
	}
	else if(this->forwardVel >= 38.0f)
	{
		this->wallKickTimer = 5 * FRAME_RATE_SCALER_INV;

		if(this->vel[1] > 0.0f)
		{
			this->vel[1] = 0.0f;
		}

		this->particleFlags |= PARTICLE_WALL_TINY_STAR;
		return this->ChangePlayerStatus(ACT_BACKWARD_AIR_KB, 0);
	}
	else
	{
		this->wallKickTimer = 5 * FRAME_RATE_SCALER_INV;

		if(this->vel[1] > 0.0f)
		{
			this->vel[1] = 0.0f;
		}

		if(this->forwardVel > 8.0f)
		{
			this->mario_set_forward_vel(-8.0f);
		}
		return this->ChangePlayerStatus(ACT_SOFT_BONK, 0);
	}

#ifdef AVOID_UB
	// return
#endif
	this->set_mario_animation(MARIO_ANIM_START_WALLKICK);

	//! Missing return statement. The returned value is the result of the call
	// to set_mario_animation. In practice, this value is nonzero.
	// This results in this action "cancelling" into itself. It is supposed to
	// execute three times, each on a separate frame, but instead it executes
	// three times on the same frame.
	// This results in firsties only being possible for a single frame, instead
	// of three.
	return 0;
}

s32 PlayerRecord::act_forward_rollout()
{
	if(this->actionState == 0)
	{
		this->vel[1]	  = 30.0f;
		this->actionState = 1;
	}

	this->play_mario_sound(SOUND_ACTION_TERRAIN_JUMP, 0);

	update_air_without_turn();

	switch(perform_air_step(0))
	{
		case AIR_STEP_NONE:
			if(this->actionState == 1)
			{
				if(this->set_mario_animation(MARIO_ANIM_FORWARD_SPINNING) == 4)
				{
					AudStartSound(SOUND_ACTION_SPIN, this->marioObj->header.gfx.cameraToObject);
				}
			}
			else
			{
				this->set_mario_animation(MARIO_ANIM_GENERAL_FALL);
			}
			break;

		case AIR_STEP_LANDED:
			this->ChangePlayerStatus(ACT_FREEFALL_LAND_STOP, 0);
			this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
			break;

		case AIR_STEP_HIT_WALL:
			this->mario_set_forward_vel(0.0f);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	if(this->actionState == 1 && this->is_anim_past_end())
	{
		this->actionState = 2;
	}
	return FALSE;
}

s32 PlayerRecord::act_backward_rollout()
{
	if(this->actionState == 0)
	{
		this->vel[1]	  = 30.0f;
		this->actionState = 1;
	}

	this->play_mario_sound(SOUND_ACTION_TERRAIN_JUMP, 0);

	update_air_without_turn();

	switch(perform_air_step(0))
	{
		case AIR_STEP_NONE:
			if(this->actionState == 1)
			{
				if(this->set_mario_animation(MARIO_ANIM_BACKWARD_SPINNING) == 4)
				{
					AudStartSound(SOUND_ACTION_SPIN, this->marioObj->header.gfx.cameraToObject);
				}
			}
			else
			{
				this->set_mario_animation(MARIO_ANIM_GENERAL_FALL);
			}
			break;

		case AIR_STEP_LANDED:
			this->ChangePlayerStatus(ACT_FREEFALL_LAND_STOP, 0);
			this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
			break;

		case AIR_STEP_HIT_WALL:
			this->mario_set_forward_vel(0.0f);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	if(this->actionState == 1 && this->marioObj->header.gfx.unk38.frame() == 2)
	{
		this->actionState = 2;
	}
	return FALSE;
}

s32 PlayerRecord::act_butt_slide_air()
{
	if(++(this->actionTimer) > 30 * FRAME_RATE_SCALER_INV && this->pos[1] - this->floorHeight > 500.0f)
	{
		return this->ChangePlayerStatus(ACT_FREEFALL, 1);
	}

	update_air_with_turn();

	switch(perform_air_step(0))
	{
		case AIR_STEP_LANDED:
			if(this->actionState == 0 && this->vel[1] < 0.0f && this->floor->normal.y >= 0.9848077f)
			{
				this->vel[1]	  = -this->vel[1] / 2.0f;
				this->actionState = 1;
			}
			else
			{
				this->ChangePlayerStatus(ACT_BUTT_SLIDE, 0);
			}
			this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
			break;

		case AIR_STEP_HIT_WALL:
			if(this->vel[1] > 0.0f)
			{
				this->vel[1] = 0.0f;
			}
			this->particleFlags |= PARTICLE_WALL_TINY_STAR;
			this->ChangePlayerStatus(ACT_BACKWARD_AIR_KB, 0);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	this->set_mario_animation(MARIO_ANIM_SLIDE);
	return FALSE;
}

s32 PlayerRecord::act_hold_butt_slide_air()
{
	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return this->drop_and_set_mario_action(ACT_HOLD_FREEFALL, 1);
	}

	if(++this->actionTimer > 30 * FRAME_RATE_SCALER_INV && this->pos[1] - this->floorHeight > 500.0f)
	{
		return this->ChangePlayerStatus(ACT_HOLD_FREEFALL, 1);
	}

	update_air_with_turn();

	switch(perform_air_step(0))
	{
		case AIR_STEP_LANDED:
			if(this->actionState == 0 && this->vel[1] < 0.0f && this->floor->normal.y >= 0.9848077f)
			{
				this->vel[1]	  = -this->vel[1] / 2.0f;
				this->actionState = 1;
			}
			else
			{
				this->ChangePlayerStatus(ACT_HOLD_BUTT_SLIDE, 0);
			}
			this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
			break;

		case AIR_STEP_HIT_WALL:
			if(this->vel[1] > 0.0f)
			{
				this->vel[1] = 0.0f;
			}

			mario_drop_held_object(this);
			this->particleFlags |= PARTICLE_WALL_TINY_STAR;
			this->ChangePlayerStatus(ACT_BACKWARD_AIR_KB, 0);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	this->set_mario_animation(MARIO_ANIM_SLIDING_ON_BOTTOM_WITH_LIGHT_OBJ);
	return FALSE;
}

s32 PlayerRecord::act_lava_boost()
{
	if(!(flags & MARIO_MARIO_SOUND_PLAYED))
	{
		this->play_sound_if_no_flag(SOUND_MARIO_ON_FIRE, MARIO_MARIO_SOUND_PLAYED);
		SendMotorEvent(5, 80);
	}

	if(!(this->input & INPUT_NONZERO_ANALOG))
	{
		this->forwardVel = approach_f32(this->forwardVel, 0.0f, 0.35f * FRAME_RATE_SCALER, 0.35f * FRAME_RATE_SCALER);
	}

	update_lava_boost_or_twirling();

	switch(perform_air_step(0))
	{
		case AIR_STEP_LANDED:
			if(this->floor->type == SURFACE_BURNING)
			{
				this->actionState = 0;
				if(!(this->flags & MARIO_METAL_CAP))
				{
					this->hurtCounter += (this->flags & MARIO_CAP_ON_HEAD) ? 12 : 18;
				}
				this->vel[1] = 84.0f;
				AudStartSound(SOUND_MARIO_ON_FIRE, this->marioObj->header.gfx.cameraToObject);
				SendMotorEvent(5, 80);
			}
			else
			{
				this->play_mario_heavy_landing_sound(SOUND_ACTION_TERRAIN_BODY_HIT_GROUND);
				if(this->actionState < 2 && this->vel[1] < 0.0f)
				{
					this->vel[1] = -this->vel[1] * 0.4f;
					this->mario_set_forward_vel(this->forwardVel * 0.5f);
					this->actionState += 1;
				}
				else
				{
					this->ChangePlayerStatus(ACT_LAVA_BOOST_LAND, 0);
				}
			}
			break;

		case AIR_STEP_HIT_WALL:
			mario_bonk_reflection(FALSE);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	this->set_mario_animation(MARIO_ANIM_FIRE_LAVA_BURN);
	if((this->area->terrainType & TERRAIN_MASK) != TERRAIN_SNOW && !(this->flags & MARIO_METAL_CAP) && this->vel[1] > 0.0f)
	{
		this->particleFlags |= PARTICLE_FLAME;
		if(this->actionState == 0)
		{
			AudStartSound(SOUND_MOVING_LAVA_BURN, this->marioObj->header.gfx.cameraToObject);
		}
	}

	if(this->health < 0x100)
	{
		PL_StartFadeout(this, PL_FADE_LOSING);
	}

	this->marioBodyState->eyeState = MARIO_EYES_DEAD;
	return FALSE;
}

s32 PlayerRecord::act_slide_kick()
{
	if(this->actionState == 0 && this->actionTimer == 0)
	{
		this->play_mario_sound(SOUND_ACTION_TERRAIN_JUMP, SOUND_MARIO_HOOHOO);
		this->set_mario_animation(MARIO_ANIM_SLIDE_KICK);
	}

	if(++(this->actionTimer) > 30 * FRAME_RATE_SCALER_INV && this->pos[1] - this->floorHeight > 500.0f)
	{
		return this->ChangePlayerStatus(ACT_FREEFALL, 2);
	}

	update_air_without_turn();

	switch(perform_air_step(0))
	{
		case AIR_STEP_NONE:
			if(this->actionState == 0)
			{
				this->marioObj->header.gfx.angle[0] = atan2s(this->forwardVel, -this->vel[1]);
				if(this->marioObj->header.gfx.angle[0] > 0x1800)
				{
					this->marioObj->header.gfx.angle[0] = 0x1800;
				}
			}
			break;

		case AIR_STEP_LANDED:
			if(this->actionState == 0 && this->vel[1] < 0.0f)
			{
				this->vel[1]	  = -this->vel[1] / 2.0f;
				this->actionState = 1;
				this->actionTimer = 0;
			}
			else
			{
				this->ChangePlayerStatus(ACT_SLIDE_KICK_SLIDE, 0);
			}
			this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
			break;

		case AIR_STEP_HIT_WALL:
			if(this->vel[1] > 0.0f)
			{
				this->vel[1] = 0.0f;
			}

			this->particleFlags |= PARTICLE_WALL_TINY_STAR;

			this->ChangePlayerStatus(ACT_BACKWARD_AIR_KB, 0);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	return FALSE;
}

s32 PlayerRecord::act_jump_kick()
{
	s32 animFrame;

	if(this->actionState == 0)
	{
		this->play_sound_if_no_flag(SOUND_MARIO_PUNCH_HOO, MARIO_ACTION_SOUND_PLAYED);
		this->marioObj->header.gfx.unk38.animID = -1;
		this->set_mario_animation(MARIO_ANIM_AIR_KICK);
		this->actionState = 1;
	}

	animFrame = this->marioObj->header.gfx.unk38.frame();
	if(animFrame == 0)
	{
		this->marioBodyState->punchState = 0x86;
	}
	if(animFrame >= 0 && animFrame < 8)
	{
		this->flags |= MARIO_KICKING;
	}

	update_air_without_turn();

	switch(perform_air_step(0))
	{
		case AIR_STEP_LANDED:
			if(!check_fall_damage_or_get_stuck(ACT_HARD_BACKWARD_GROUND_KB))
			{
				this->ChangePlayerStatus(ACT_FREEFALL_LAND, 0);
			}
			break;

		case AIR_STEP_HIT_WALL:
			this->mario_set_forward_vel(0.0f);
			break;
	}

	return FALSE;
}

s32 PlayerRecord::act_shot_from_cannon()
{
	if(this->area->camera->mode != CAMERA_MODE_BEHIND_MARIO)
	{
		this->statusForCamera->cameraEvent = CAM_EVENT_SHOT_FROM_CANNON;
	}

	this->mario_set_forward_vel(this->forwardVel);

	this->play_sound_if_no_flag(SOUND_MARIO_YAHOO, MARIO_MARIO_SOUND_PLAYED);

	switch(perform_air_step(0))
	{
		case AIR_STEP_NONE:
			this->set_mario_animation(MARIO_ANIM_AIRBORNE_ON_STOMACH);
			this->faceAngle[0]		    = atan2s(this->forwardVel, this->vel[1]);
			this->marioObj->header.gfx.angle[0] = -this->faceAngle[0];
			break;

		case AIR_STEP_LANDED:
			this->ChangePlayerStatus(ACT_DIVE_SLIDE, 0);
			this->faceAngle[0] = 0;
			set_camera_mode(this->area->camera, this->area->camera->defMode, 1);
			SendMotorEvent(5, 80);
			break;

		case AIR_STEP_HIT_WALL:
			this->mario_set_forward_vel(-16.0f);

			this->faceAngle[0] = 0;
			if(this->vel[1] > 0.0f)
			{
				this->vel[1] = 0.0f;
			}

			this->particleFlags |= PARTICLE_WALL_TINY_STAR;
			this->ChangePlayerStatus(ACT_BACKWARD_AIR_KB, 0);
			set_camera_mode(this->area->camera, this->area->camera->defMode, 1);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	if((this->flags & MARIO_WING_CAP) && this->vel[1] < 0.0f)
	{
		this->ChangePlayerStatus(ACT_FLYING, 0);
	}

	if((this->forwardVel -= 0.05 * FRAME_RATE_SCALER) < 10.0f)
	{
		this->mario_set_forward_vel(10.0f);
	}

	if(this->vel[1] > 0.0f)
	{
		this->particleFlags |= PARTICLE_DUST;
	}
	return FALSE;
}

s32 PlayerRecord::act_flying()
{
	s16 startPitch = this->faceAngle[0];

	if(this->input & INPUT_Z_PRESSED)
	{
		if(this->area->camera->mode == CAMERA_MODE_BEHIND_MARIO)
		{
			set_camera_mode(this->area->camera, this->area->camera->defMode, 1);
		}
		return this->ChangePlayerStatus(ACT_GROUND_POUND, 1);
	}

	if(!(this->flags & MARIO_WING_CAP))
	{
		if(this->area->camera->mode == CAMERA_MODE_BEHIND_MARIO)
		{
			set_camera_mode(this->area->camera, this->area->camera->defMode, 1);
		}
		return this->ChangePlayerStatus(ACT_FREEFALL, 0);
	}

	if(this->area->camera->mode != CAMERA_MODE_BEHIND_MARIO)
	{
		set_camera_mode(this->area->camera, CAMERA_MODE_BEHIND_MARIO, 1);
	}

	if(this->actionState == 0)
	{
		if(this->actionArg == 0)
		{
			this->set_mario_animation(MARIO_ANIM_FLY_FROM_CANNON);
		}
		else
		{
			this->set_mario_animation(MARIO_ANIM_FORWARD_SPINNING_FLIP);
			if(this->marioObj->header.gfx.unk38.frame() == 1)
			{
				AudStartSound(SOUND_ACTION_SPIN, this->marioObj->header.gfx.cameraToObject);
			}
		}

		if(this->is_anim_at_end())
		{
			if(this->actionArg == 2)
			{
				PL_DisplayMessage(0);
				this->actionArg = 1;
			}

			this->set_mario_animation(MARIO_ANIM_WING_CAP_FLY);
			this->actionState = 1;
		}
	}

	update_flying();

	switch(perform_air_step(0))
	{
		case AIR_STEP_NONE:
			this->marioObj->header.gfx.angle[0] = -this->faceAngle[0];
			this->marioObj->header.gfx.angle[2] = this->faceAngle[2];
			this->actionTimer		    = 0;
			break;

		case AIR_STEP_LANDED:
			this->ChangePlayerStatus(ACT_DIVE_SLIDE, 0);

			this->set_mario_animation(MARIO_ANIM_DIVE);
			this->set_anim_to_frame(7);

			this->faceAngle[0] = 0;
			set_camera_mode(this->area->camera, this->area->camera->defMode, 1);
			SendMotorEvent(5, 80);
			break;

		case AIR_STEP_HIT_WALL:
			if(this->wall != NULL)
			{
				this->mario_set_forward_vel(-16.0f);
				this->faceAngle[0] = 0;

				if(this->vel[1] > 0.0f)
				{
					this->vel[1] = 0.0f;
				}

				AudStartSound((this->flags & MARIO_METAL_CAP) ? SOUND_ACTION_METAL_BONK : SOUND_ACTION_BONK, this->marioObj->header.gfx.cameraToObject);

				this->particleFlags |= PARTICLE_WALL_TINY_STAR;
				this->ChangePlayerStatus(ACT_BACKWARD_AIR_KB, 0);
				set_camera_mode(this->area->camera, this->area->camera->defMode, 1);
			}
			else
			{
				if(this->actionTimer++ == 0)
				{
					AudStartSound(SOUND_ACTION_HIT, this->marioObj->header.gfx.cameraToObject);
				}

				if(this->actionTimer == 30 * FRAME_RATE_SCALER_INV)
				{
					this->actionTimer = 0;
				}

				this->faceAngle[0] -= 0x200 / FRAME_RATE_SCALER_INV;
				if(this->faceAngle[0] < -0x2AAA)
				{
					this->faceAngle[0] = -0x2AAA;
				}

				this->marioObj->header.gfx.angle[0] = -this->faceAngle[0];
				this->marioObj->header.gfx.angle[2] = this->faceAngle[2];
			}
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	if(this->faceAngle[0] > 0x800 && this->forwardVel >= 48.0f)
	{
		this->particleFlags |= PARTICLE_DUST;
	}

	if(startPitch <= 0 && this->faceAngle[0] > 0 && this->forwardVel >= 48.0f)
	{
		AudStartSound(SOUND_ACTION_FLYING_FAST, this->marioObj->header.gfx.cameraToObject);
		AudStartSound(SOUND_MARIO_YAHOO_WAHA_YIPPEE + ((gAudioRandom % 5) << 16), this->marioObj->header.gfx.cameraToObject);
		SendMotorEvent(50, 40);
	}

	AudStartSound(SOUND_MOVING_FLYING, this->marioObj->header.gfx.cameraToObject);
	this->adjust_sound_for_speed();
	return FALSE;
}

s32 PlayerRecord::act_riding_hoot()
{
	if(!(this->input & INPUT_A_DOWN) || (this->marioObj->oInteractStatus & INT_STATUS_MARIO_UNK7))
	{
		this->usedObj->oInteractStatus	     = 0;
		this->usedObj->oHootMarioReleaseTime = frameCounter;

		this->play_sound_if_no_flag(SOUND_MARIO_UH, MARIO_MARIO_SOUND_PLAYED);
		SendMotorEvent(4, 40);
		return this->ChangePlayerStatus(ACT_FREEFALL, 0);
	}

	this->pos[0] = this->usedObj->oPosX;
	this->pos[1] = this->usedObj->oPosY - 92.5f;
	this->pos[2] = this->usedObj->oPosZ;

	this->faceAngle[1] = 0x4000 - this->usedObj->oMoveAngleYaw;

	if(this->actionState == 0)
	{
		this->set_mario_animation(MARIO_ANIM_HANG_ON_CEILING);
		if(this->is_anim_at_end())
		{
			this->set_mario_animation(MARIO_ANIM_HANG_ON_OWL);
			this->actionState = 1;
		}
	}

	vec3f_set(this->vel, 0.0f, 0.0f, 0.0f);
	vec3f_set(this->marioObj->header.gfx.pos, this->pos[0], this->pos[1], this->pos[2]);
	vec3s_set(this->marioObj->header.gfx.angle, 0, 0x4000 - this->faceAngle[1], 0);
	return FALSE;
}

s32 PlayerRecord::act_flying_triple_jump()
{
	if(this->input & (INPUT_B_PRESSED | INPUT_Z_PRESSED))
	{
		if(this->area->camera->mode == CAMERA_MODE_BEHIND_MARIO)
		{
			set_camera_mode(this->area->camera, this->area->camera->defMode, 1);
		}
		if(this->input & INPUT_B_PRESSED)
		{
			return this->ChangePlayerStatus(ACT_DIVE, 0);
		}
		else
		{
			return this->ChangePlayerStatus(ACT_GROUND_POUND, 0);
		}
	}

	this->play_mario_sound(SOUND_ACTION_TERRAIN_JUMP, SOUND_MARIO_YAHOO);
	if(this->actionState == 0)
	{
		this->set_mario_animation(MARIO_ANIM_TRIPLE_JUMP_FLY);

		if(this->marioObj->header.gfx.unk38.frame() == 7)
		{
			AudStartSound(SOUND_ACTION_SPIN, this->marioObj->header.gfx.cameraToObject);
		}

		if(this->is_anim_past_end())
		{
			this->set_mario_animation(MARIO_ANIM_FORWARD_SPINNING);
			SendMotorEvent(8, 80);
			this->actionState = 1;
		}
	}

	if(this->actionState == 1 && this->marioObj->header.gfx.unk38.frame() == 1)
	{
		AudStartSound(SOUND_ACTION_SPIN, this->marioObj->header.gfx.cameraToObject);
	}

	if(this->vel[1] < 4.0f)
	{
		if(this->area->camera->mode != CAMERA_MODE_BEHIND_MARIO)
		{
			set_camera_mode(this->area->camera, CAMERA_MODE_BEHIND_MARIO, 1);
		}

		if(this->forwardVel < 32.0f)
		{
			this->mario_set_forward_vel(32.0f);
		}

		this->ChangePlayerStatus(ACT_FLYING, 1);
	}

	if(this->actionTimer++ == 10 * FRAME_RATE_SCALER_INV && this->area->camera->mode != CAMERA_MODE_BEHIND_MARIO)
	{
		set_camera_mode(this->area->camera, CAMERA_MODE_BEHIND_MARIO, 1);
	}

	update_air_without_turn();

	switch(perform_air_step(0))
	{
		case AIR_STEP_LANDED:
			if(!check_fall_damage_or_get_stuck(ACT_HARD_BACKWARD_GROUND_KB))
			{
				this->ChangePlayerStatus(ACT_DOUBLE_JUMP_LAND, 0);
			}
			break;

		case AIR_STEP_HIT_WALL:
			mario_bonk_reflection(FALSE);
			break;

		case AIR_STEP_HIT_LAVA_WALL:
			lava_boost_on_wall();
			break;
	}

	return FALSE;
}

s32 PlayerRecord::act_top_of_pole_jump()
{
	this->play_mario_jump_sound();
	common_air_action_step(ACT_FREEFALL_LAND, MARIO_ANIM_HANDSTAND_JUMP, AIR_STEP_CHECK_LEDGE_GRAB);
	return FALSE;
}

s32 PlayerRecord::act_vertical_wind()
{
	s16 intendedDYaw = this->intendedYaw - this->faceAngle[1];
	f32 intendedMag	 = this->intendedMag / 32.0f;

	this->play_sound_if_no_flag(SOUND_MARIO_HERE_WE_GO, MARIO_MARIO_SOUND_PLAYED);
	if(this->actionState == 0)
	{
		this->set_mario_animation(MARIO_ANIM_FORWARD_SPINNING_FLIP);
		if(this->marioObj->header.gfx.unk38.frame() == 1)
		{
			AudStartSound(SOUND_ACTION_SPIN, this->marioObj->header.gfx.cameraToObject);
			SendMotorEvent(8, 80);
		}

		if(this->is_anim_past_end())
		{
			this->actionState = 1;
		}
	}
	else
	{
		this->set_mario_animation(MARIO_ANIM_AIRBORNE_ON_STOMACH);
	}

	update_air_without_turn();

	switch(perform_air_step(0))
	{
		case AIR_STEP_LANDED:
			this->ChangePlayerStatus(ACT_DIVE_SLIDE, 0);
			break;

		case AIR_STEP_HIT_WALL:
			this->mario_set_forward_vel(-16.0f);
			break;
	}

	this->marioObj->header.gfx.angle[0] = (s16)(6144.0f * intendedMag * coss(intendedDYaw));
	this->marioObj->header.gfx.angle[2] = (s16)(-4096.0f * intendedMag * sins(intendedDYaw));
	return FALSE;
}

s32 PlayerRecord::act_special_triple_jump()
{
	if(this->input & INPUT_B_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_DIVE, 0);
	}

	if(this->input & INPUT_Z_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_GROUND_POUND, 0);
	}

	this->play_mario_sound(SOUND_ACTION_TERRAIN_JUMP, SOUND_MARIO_YAHOO);

	update_air_without_turn();

	switch(perform_air_step(0))
	{
		case AIR_STEP_LANDED:
			if(this->actionState++ == 0)
			{
				if(sm64::config().cheats().quadrupleJump())
				{
					this->vel[1] = 42.0f * 2.00f * (sm64::config().cheats().superJump() ? 1.5f : 1.0f);
				}
				else
				{
					this->vel[1] = 42.0f;
				}
			}
			else
			{
				this->ChangePlayerStatus(ACT_FREEFALL_LAND_STOP, 0);
			}
			this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
			break;

		case AIR_STEP_HIT_WALL:
			mario_bonk_reflection(TRUE);
			break;
	}

	if(this->actionState == 0 || this->vel[1] > 0.0f)
	{
		if(this->set_mario_animation(MARIO_ANIM_FORWARD_SPINNING) == 0)
		{
			AudStartSound(SOUND_ACTION_SPIN, this->marioObj->header.gfx.cameraToObject);
		}
	}
	else
	{
		this->set_mario_animation(MARIO_ANIM_GENERAL_FALL);
	}

	this->particleFlags |= PARTICLE_SPARKLES;
	return FALSE;
}

s32 PlayerRecord::check_common_airborne_cancels()
{
	if(this->pos[1] < this->waterLevel - 100)
	{
		return this->set_water_plunge_action();
	}

	if(this->input & INPUT_SQUISHED)
	{
		return this->drop_and_set_mario_action(ACT_SQUISHED, 0);
	}

	if(this->floor->type == SURFACE_VERTICAL_WIND && (this->status & ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION))
	{
		return this->drop_and_set_mario_action(ACT_VERTICAL_WIND, 0);
	}

	this->sinking = 0.0f;
	return FALSE;
}

s32 PlayerRecord::mario_execute_airborne_action()
{
	u32 cancel;

	if(check_common_airborne_cancels())
	{
		return TRUE;
	}

	play_far_fall_sound();

	/* clang-format off */
	switch(this->status)
	{
		case ACT_JUMP:                 cancel = act_jump();                 break;
		case ACT_DOUBLE_JUMP:          cancel = act_double_jump();          break;
		case ACT_FREEFALL:             cancel = act_freefall();             break;
		case ACT_HOLD_JUMP:            cancel = act_hold_jump();            break;
		case ACT_HOLD_FREEFALL:        cancel = act_hold_freefall();        break;
		case ACT_SIDE_FLIP:            cancel = act_side_flip();            break;
		case ACT_WALL_KICK_AIR:        cancel = act_wall_kick_air();        break;
		case ACT_TWIRLING:             cancel = act_twirling();             break;
		case ACT_WATER_JUMP:           cancel = act_water_jump();           break;
		case ACT_HOLD_WATER_JUMP:      cancel = act_hold_water_jump();      break;
		case ACT_STEEP_JUMP:           cancel = act_steep_jump();           break;
		case ACT_BURNING_JUMP:         cancel = act_burning_jump();         break;
		case ACT_BURNING_FALL:         cancel = act_burning_fall();         break;
		case ACT_TRIPLE_JUMP:          cancel = act_triple_jump();          break;
		case ACT_BACKFLIP:             cancel = act_backflip();             break;
		case ACT_LONG_JUMP:            cancel = act_long_jump();            break;
		case ACT_RIDING_SHELL_JUMP:
		case ACT_RIDING_SHELL_FALL:    cancel = act_riding_shell_air();     break;
		case ACT_DIVE:                 cancel = act_dive();                 break;
		case ACT_AIR_THROW:            cancel = act_air_throw();            break;
		case ACT_BACKWARD_AIR_KB:      cancel = act_backward_air_kb();      break;
		case ACT_FORWARD_AIR_KB:       cancel = act_forward_air_kb();       break;
		case ACT_HARD_FORWARD_AIR_KB:  cancel = act_hard_forward_air_kb();  break;
		case ACT_HARD_BACKWARD_AIR_KB: cancel = act_hard_backward_air_kb(); break;
		case ACT_SOFT_BONK:            cancel = act_soft_bonk();            break;
		case ACT_AIR_HIT_WALL:         cancel = act_air_hit_wall();         break;
		case ACT_FORWARD_ROLLOUT:      cancel = act_forward_rollout();      break;
		case ACT_SHOT_FROM_CANNON:     cancel = act_shot_from_cannon();     break;
		case ACT_BUTT_SLIDE_AIR:       cancel = act_butt_slide_air();       break;
		case ACT_HOLD_BUTT_SLIDE_AIR:  cancel = act_hold_butt_slide_air();  break;
		case ACT_LAVA_BOOST:           cancel = act_lava_boost();           break;
		case ACT_GETTING_BLOWN:        cancel = act_getting_blown();        break;
		case ACT_BACKWARD_ROLLOUT:     cancel = act_backward_rollout();     break;
		case ACT_CRAZY_BOX_BOUNCE:     cancel = act_crazy_box_bounce();     break;
		case ACT_SPECIAL_TRIPLE_JUMP:  cancel = act_special_triple_jump();  break;
		case ACT_GROUND_POUND:         cancel = act_ground_pound();         break;
		case ACT_THROWN_FORWARD:       cancel = act_thrown_forward();       break;
		case ACT_THROWN_BACKWARD:      cancel = act_thrown_backward();      break;
		case ACT_FLYING_TRIPLE_JUMP:   cancel = act_flying_triple_jump();   break;
		case ACT_SLIDE_KICK:           cancel = act_slide_kick();           break;
		case ACT_JUMP_KICK:            cancel = act_jump_kick();            break;
		case ACT_FLYING:               cancel = act_flying();               break;
		case ACT_RIDING_HOOT:          cancel = act_riding_hoot();          break;
		case ACT_TOP_OF_POLE_JUMP:     cancel = act_top_of_pole_jump();     break;
		case ACT_VERTICAL_WIND:        cancel = act_vertical_wind();        break;
	}
	/* clang-format on */

	return cancel;
}
