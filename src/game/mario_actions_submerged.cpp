#include <ultra64.h>

#include "sm64.h"
#include "level_update.h"
#include "memory.h"
#include "engine/math_util.h"
#include "area.h"
#include "save_file.h"
#include "sound_init.h"
#include "engine/surface_collision.h"
#include "interaction.h"
#include "mario.h"
#include "mario_step.h"
#include "camera.h"
#include "audio/external.h"
#include "behavior_data.h"
#include "level_table.h"

#define MIN_SWIM_STRENGTH 160
#define MIN_SWIM_SPEED 16.0f

s16 sWasAtSurface	  = FALSE;
s16 sSwimStrength	  = MIN_SWIM_STRENGTH;
s16 sWaterCurrentSpeeds[] = {28, 12, 8, 4};

s16 D_80339FD0;
s16 D_80339FD2;
f32 D_80339FD4;

void MarioState::set_swimming_at_surface_particles(u32 particleFlag)
{
	s16 atSurface = this->pos[1] >= this->waterLevel - 130;

	if(atSurface)
	{
		this->particleFlags |= particleFlag;
		if(atSurface ^ sWasAtSurface)
		{
			play_sound(SOUND_ACTION_UNKNOWN431, this->marioObj->header.gfx.cameraToObject);
		}
	}

	sWasAtSurface = atSurface;
}

s32 MarioState::swimming_near_surface()
{
	if(this->flags & MARIO_METAL_CAP)
	{
		return FALSE;
	}

	return (this->waterLevel - 80) - this->pos[1] < 400.0f;
}

f32 MarioState::get_buoyancy()
{
	f32 buoyancy = 0.0f;

	if(this->flags & MARIO_METAL_CAP)
	{
		if(this->action & ACT_FLAG_INVULNERABLE)
		{
			buoyancy = -2.0f;
		}
		else
		{
			buoyancy = -18.0f;
		}
	}
	else if(swimming_near_surface())
	{
		buoyancy = 1.25f;
	}
	else if(!(this->action & ACT_FLAG_MOVING))
	{
		buoyancy = -2.0f;
	}

	return buoyancy;
}

u32 MarioState::perform_water_full_step(Vec3f nextPos)
{
	struct Surface* wall;
	struct Surface* ceil;
	struct Surface* floor;
	f32 ceilHeight;
	f32 floorHeight;

	wall	    = resolve_and_return_wall_collisions(nextPos, 10.0f, 110.0f);
	floorHeight = find_floor(nextPos[0], nextPos[1], nextPos[2], &floor);
	ceilHeight  = vec3f_find_ceil(nextPos, floorHeight, &ceil);

	if(floor == NULL)
	{
		return WATER_STEP_CANCELLED;
	}

	if(nextPos[1] >= floorHeight)
	{
		if(ceilHeight - nextPos[1] >= 160.0f)
		{
			vec3f_copy(this->pos, nextPos);
			this->floor	  = floor;
			this->floorHeight = floorHeight;

			if(wall != NULL)
			{
				return WATER_STEP_HIT_WALL;
			}
			else
			{
				return WATER_STEP_NONE;
			}
		}

		if(ceilHeight - floorHeight < 160.0f)
		{
			return WATER_STEP_CANCELLED;
		}

		//! Water ceiling downwarp
		vec3f_set(this->pos, nextPos[0], ceilHeight - 160.0f, nextPos[2]);
		this->floor	  = floor;
		this->floorHeight = floorHeight;
		return WATER_STEP_HIT_CEILING;
	}
	else
	{
		if(ceilHeight - floorHeight < 160.0f)
		{
			return WATER_STEP_CANCELLED;
		}

		vec3f_set(this->pos, nextPos[0], floorHeight, nextPos[2]);
		this->floor	  = floor;
		this->floorHeight = floorHeight;
		return WATER_STEP_HIT_FLOOR;
	}
}

void MarioState::apply_water_current(Vec3f velocity)
{
	s32 i;
	f32 whirlpoolRadius = 2000.0f;

	if(this->floor->type == SURFACE_FLOWING_WATER)
	{
		s16 currentAngle = this->floor->force << 8;
		f32 currentSpeed = sWaterCurrentSpeeds[this->floor->force >> 8];

		velocity[0] += currentSpeed * FRAME_RATE_SCALER * sins(currentAngle);
		velocity[2] += currentSpeed * FRAME_RATE_SCALER * coss(currentAngle);
	}

	for(i = 0; i < 2; i++)
	{
		struct Whirlpool* whirlpool = gCurrentArea->whirlpools[i];
		if(whirlpool != NULL)
		{
			f32 strength = 0.0f;

			f32 dx = whirlpool->pos[0] - this->pos[0];
			f32 dy = whirlpool->pos[1] - this->pos[1];
			f32 dz = whirlpool->pos[2] - this->pos[2];

			f32 lateralDist = sqrtf(dx * dx + dz * dz);
			f32 distance	= sqrtf(lateralDist * lateralDist + dy * dy);

			s16 pitchToWhirlpool = atan2s(lateralDist, dy);
			s16 yawToWhirlpool   = atan2s(dz, dx);

			yawToWhirlpool -= (s16)(0x2000 * 1000.0f / (distance + 1000.0f)) * FRAME_RATE_SCALER;

			if(whirlpool->strength >= 0)
			{
				if(gCurrLevelNum == LEVEL_DDD && gCurrAreaIndex == 2)
				{
					whirlpoolRadius = 4000.0f;
				}

				if(distance >= 26.0f && distance < whirlpoolRadius)
				{
					strength = whirlpool->strength * (1.0f - distance / whirlpoolRadius);
				}
			}
			else if(distance < 2000.0f)
			{
				strength = whirlpool->strength * (1.0f - distance / 2000.0f);
			}

			velocity[0] += strength /** FRAME_RATE_SCALER*/ * coss(pitchToWhirlpool) * sins(yawToWhirlpool);
			velocity[1] += strength /** FRAME_RATE_SCALER*/ * sins(pitchToWhirlpool);
			velocity[2] += strength /** FRAME_RATE_SCALER*/ * coss(pitchToWhirlpool) * coss(yawToWhirlpool);
		}
	}
}

u32 MarioState::perform_water_step()
{
	UNUSED u32 unused;
	u32 stepResult;
	Vec3f nextPos;
	Vec3f step;
	struct Object* marioObj = this->marioObj;

	vec3f_copy(step, this->vel);

	if(this->action & ACT_FLAG_SWIMMING)
	{
		apply_water_current(step);
	}

	nextPos[0] = this->pos[0] + step[0] / FRAME_RATE_SCALER_INV;
	nextPos[1] = this->pos[1] + step[1] / FRAME_RATE_SCALER_INV;
	nextPos[2] = this->pos[2] + step[2] / FRAME_RATE_SCALER_INV;

	if(nextPos[1] > this->waterLevel - 80)
	{
		nextPos[1]   = this->waterLevel - 80;
		this->vel[1] = 0.0f;
	}

	stepResult = perform_water_full_step(nextPos);

	vec3f_copy(marioObj->header.gfx.pos, this->pos);
	vec3s_set(marioObj->header.gfx.angle, -this->faceAngle[0], this->faceAngle[1], this->faceAngle[2]);

	return stepResult;
}

void MarioState::func_80270504()
{
	struct Object* marioObj = this->marioObj;

	if(marioObj->header.gfx.angle[0] > 0)
	{
		marioObj->header.gfx.pos[1] += (60.0f * sins(marioObj->header.gfx.angle[0]) * sins(marioObj->header.gfx.angle[0])) * FRAME_RATE_SCALER;
	}

	if(marioObj->header.gfx.angle[0] < 0)
	{
		marioObj->header.gfx.angle[0] = marioObj->header.gfx.angle[0] * 6 / 10;
	}

	if(marioObj->header.gfx.angle[0] > 0)
	{
		marioObj->header.gfx.angle[0] = marioObj->header.gfx.angle[0] * 10 / 8;
	}
}

void MarioState::stationary_slow_down()
{
	f32 buoyancy = get_buoyancy();

	this->angleVel[0] = 0;
	this->angleVel[1] = 0;

	this->forwardVel = approach_f32(this->forwardVel, 0.0f, 1.0f * FRAME_RATE_SCALER, 1.0f * FRAME_RATE_SCALER);
	this->vel[1]	 = approach_f32(this->vel[1], buoyancy, 2.0f * FRAME_RATE_SCALER, 1.0f * FRAME_RATE_SCALER);

	this->faceAngle[0] = approach_s32(this->faceAngle[0], 0, 0x200 / FRAME_RATE_SCALER_INV, 0x200 / FRAME_RATE_SCALER_INV);
	this->faceAngle[2] = approach_s32(this->faceAngle[2], 0, 0x100 / FRAME_RATE_SCALER_INV, 0x100 / FRAME_RATE_SCALER_INV);

	this->vel[0] = this->forwardVel * coss(this->faceAngle[0]) * sins(this->faceAngle[1]);
	this->vel[2] = this->forwardVel * coss(this->faceAngle[0]) * coss(this->faceAngle[1]);
}

void MarioState::update_swimming_speed(f32 decelThreshold)
{
	f32 buoyancy = get_buoyancy();
	f32 maxSpeed = 28.0f;

	if(this->action & ACT_FLAG_STATIONARY)
	{
		this->forwardVel -= 2.0f * FRAME_RATE_SCALER;
	}

	if(this->forwardVel < 0.0f)
	{
		this->forwardVel = 0.0f;
	}

	if(this->forwardVel > maxSpeed)
	{
		this->forwardVel = maxSpeed;
	}

	if(this->forwardVel > decelThreshold)
	{
		this->forwardVel -= 0.5f * FRAME_RATE_SCALER;
	}

	this->vel[0] = this->forwardVel * coss(this->faceAngle[0]) * sins(this->faceAngle[1]);
	this->vel[1] = this->forwardVel * sins(this->faceAngle[0]) + buoyancy;
	this->vel[2] = this->forwardVel * coss(this->faceAngle[0]) * coss(this->faceAngle[1]);
}

void MarioState::update_swimming_yaw()
{
	s16 targetYawVel = -(s16)(10.0f * this->controller->stickX);

	if(targetYawVel > 0)
	{
		if(this->angleVel[1] < 0)
		{
			this->angleVel[1] += 0x40 / FRAME_RATE_SCALER_INV;
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
	this->faceAngle[2] = -this->angleVel[1] * 8;
}

void MarioState::update_swimming_pitch()
{
	s16 targetPitch = -(s16)(252.0f * this->controller->stickY);

	s16 pitchVel;
	if(this->faceAngle[0] < 0)
	{
		pitchVel = 0x100;
	}
	else
	{
		pitchVel = 0x200;
	}

	if(this->faceAngle[0] < targetPitch)
	{
		if((this->faceAngle[0] += pitchVel / FRAME_RATE_SCALER_INV) > targetPitch)
		{
			this->faceAngle[0] = targetPitch;
		}
	}
	else if(this->faceAngle[0] > targetPitch)
	{
		if((this->faceAngle[0] -= pitchVel / FRAME_RATE_SCALER_INV) < targetPitch)
		{
			this->faceAngle[0] = targetPitch;
		}
	}
}

void MarioState::common_idle_step(s32 animation, s32 arg)
{
	s16* val = &this->marioBodyState->headAngle[0];

	update_swimming_yaw();
	update_swimming_pitch();
	update_swimming_speed(MIN_SWIM_SPEED);
	perform_water_step();
	func_80270504();

	if(this->faceAngle[0] > 0)
	{
		*val = approach_s32(*val, this->faceAngle[0] / 2, 0x80 / FRAME_RATE_SCALER_INV, 0x200 / FRAME_RATE_SCALER_INV);
	}
	else
	{
		*val = approach_s32(*val, 0, 0x200 / FRAME_RATE_SCALER_INV, 0x200 / FRAME_RATE_SCALER_INV);
	}

	if(arg == 0)
	{
		set_mario_animation(animation);
	}
	else
	{
		set_mario_anim_with_accel(animation, arg);
	}

	set_swimming_at_surface_particles(PARTICLE_SURFACE_WAVES);
}

s32 MarioState::act_water_idle()
{
	u32 val = 0x10000;

	if(this->flags & MARIO_METAL_CAP)
	{
		return set_mario_action(ACT_METAL_WATER_FALLING, 1);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_WATER_PUNCH, 0);
	}

	if(this->input & INPUT_A_PRESSED)
	{
		return set_mario_action(ACT_BREASTSTROKE, 0);
	}

	if(this->faceAngle[0] < -0x1000)
	{
		val = 0x30000;
	}

	common_idle_step(MARIO_ANIM_WATER_IDLE, val);
	return FALSE;
}

s32 MarioState::act_hold_water_idle()
{
	if(this->flags & MARIO_METAL_CAP)
	{
		return set_mario_action(ACT_HOLD_METAL_WATER_FALLING, 0);
	}

	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_WATER_IDLE, 0);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_WATER_THROW, 0);
	}

	if(this->input & INPUT_A_PRESSED)
	{
		return set_mario_action(ACT_HOLD_BREASTSTROKE, 0);
	}

	common_idle_step(MARIO_ANIM_WATER_IDLE_WITH_OBJ, 0);
	return FALSE;
}

s32 MarioState::act_water_action_end()
{
	if(this->flags & MARIO_METAL_CAP)
	{
		return set_mario_action(ACT_METAL_WATER_FALLING, 1);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_WATER_PUNCH, 0);
	}

	if(this->input & INPUT_A_PRESSED)
	{
		return set_mario_action(ACT_BREASTSTROKE, 0);
	}

	common_idle_step(MARIO_ANIM_WATER_ACTION_END, 0);
	if(is_anim_at_end())
	{
		set_mario_action(ACT_WATER_IDLE, 0);
	}
	return FALSE;
}

s32 MarioState::act_hold_water_action_end()
{
	if(this->flags & MARIO_METAL_CAP)
	{
		return set_mario_action(ACT_HOLD_METAL_WATER_FALLING, 0);
	}

	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_WATER_IDLE, 0);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_WATER_THROW, 0);
	}

	if(this->input & INPUT_A_PRESSED)
	{
		return set_mario_action(ACT_HOLD_BREASTSTROKE, 0);
	}

	common_idle_step(this->actionArg == 0 ? MARIO_ANIM_WATER_ACTION_END_WITH_OBJ : MARIO_ANIM_STOP_GRAB_OBJ_WATER, 0);
	if(is_anim_at_end())
	{
		set_mario_action(ACT_HOLD_WATER_IDLE, 0);
	}
	return FALSE;
}

void MarioState::func_8027107C()
{
	D_80339FD0 = 0;
	D_80339FD2 = 0x800;
	D_80339FD4 = this->faceAngle[0] / 256.0f + 20.0f;
}

void MarioState::func_802710CC()
{
	if(D_80339FD2 != 0 && this->pos[1] > this->waterLevel - 85 && this->faceAngle[0] >= 0)
	{
		if((D_80339FD0 += D_80339FD2 / FRAME_RATE_SCALER_INV) >= 0)
		{
			this->marioObj->header.gfx.pos[1] += D_80339FD4 * FRAME_RATE_SCALER * sins(D_80339FD0);
			return;
		}
	}

	D_80339FD2 = 0;
}

void MarioState::common_swimming_step(s16 swimStrength)
{
	s16 floorPitch;
	UNUSED struct Object* marioObj = this->marioObj;

	update_swimming_yaw();
	update_swimming_pitch();
	update_swimming_speed(swimStrength / 10.0f);

	switch(perform_water_step())
	{
		case WATER_STEP_HIT_FLOOR:
			floorPitch = -find_floor_slope(-0x8000);
			if(this->faceAngle[0] < floorPitch)
			{
				this->faceAngle[0] = floorPitch;
			}
			break;

		case WATER_STEP_HIT_CEILING:
			if(this->faceAngle[0] > -0x3000)
			{
				this->faceAngle[0] -= 0x100 / FRAME_RATE_SCALER_INV;
			}
			break;

		case WATER_STEP_HIT_WALL:
			if(this->controller->stickY == 0.0f)
			{
				if(this->faceAngle[0] > 0.0f)
				{
					this->faceAngle[0] += 0x200 / FRAME_RATE_SCALER_INV;
					if(this->faceAngle[0] > 0x3F00)
					{
						this->faceAngle[0] = 0x3F00;
					}
				}
				else
				{
					this->faceAngle[0] -= 0x200 / FRAME_RATE_SCALER_INV;
					if(this->faceAngle[0] < -0x3F00)
					{
						this->faceAngle[0] = -0x3F00;
					}
				}
			}
			break;
	}

	func_80270504();
	this->marioBodyState->headAngle[0] = approach_s32(this->marioBodyState->headAngle[0], 0, 0x200 / FRAME_RATE_SCALER_INV, 0x200 / FRAME_RATE_SCALER_INV);

	func_802710CC();
	set_swimming_at_surface_particles(PARTICLE_WATER_WAKE);
}

void MarioState::func_802713A8()
{
	s16 animFrame = this->marioObj->header.gfx.unk38.frame();

	// (this need to be on one line to match on PAL)
	if(animFrame == 0 || animFrame == 12)
	{
		play_sound(SOUND_ACTION_UNKNOWN434, this->marioObj->header.gfx.cameraToObject);
	}
}

s32 MarioState::check_water_jump()
{
	s32 probe = (s32)(this->pos[1] + 1.5f);

	if(this->input & INPUT_A_PRESSED)
	{
		if(probe >= this->waterLevel - 80 && this->faceAngle[0] >= 0 && this->controller->stickY < -60.0f)
		{
			vec3s_set(this->angleVel, 0, 0, 0);

			this->vel[1] = 62.0f;

			if(this->heldObj == NULL)
			{
				return set_mario_action(ACT_WATER_JUMP, 0);
			}
			else
			{
				return set_mario_action(ACT_HOLD_WATER_JUMP, 0);
			}
		}
	}

	return FALSE;
}

s32 MarioState::act_breaststroke()
{
	if(this->actionArg == 0)
	{
		sSwimStrength = MIN_SWIM_STRENGTH;
	}

	if(this->flags & MARIO_METAL_CAP)
	{
		return set_mario_action(ACT_METAL_WATER_FALLING, 1);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_WATER_PUNCH, 0);
	}

	if(++this->actionTimer == 14 * FRAME_RATE_SCALER_INV)
	{
		return set_mario_action(ACT_FLUTTER_KICK, 0);
	}

	if(check_water_jump())
	{
		return TRUE;
	}

	if(this->actionTimer < 6 * FRAME_RATE_SCALER_INV)
	{
		this->forwardVel += 0.5f * FRAME_RATE_SCALER;
	}

	if(this->actionTimer >= 9 * FRAME_RATE_SCALER_INV)
	{
		this->forwardVel += 1.5f * FRAME_RATE_SCALER;
	}

	if(this->actionTimer >= 2 * FRAME_RATE_SCALER_INV)
	{
		if(this->actionTimer < 6 * FRAME_RATE_SCALER_INV && (this->input & INPUT_A_PRESSED))
		{
			this->actionState = 1;
		}

		if(this->actionTimer == 9 * FRAME_RATE_SCALER_INV && this->actionState == 1)
		{
			//set_anim_to_frame(0);
			anim_reset();
			this->actionState = 0;
			this->actionTimer = 1;
			sSwimStrength	  = MIN_SWIM_STRENGTH;
		}
	}

	if(this->actionTimer == 1 /** FRAME_RATE_SCALER_INV*/)
	{
		play_sound(sSwimStrength == MIN_SWIM_STRENGTH ? SOUND_ACTION_SWIM : SOUND_ACTION_SWIM_FAST, this->marioObj->header.gfx.cameraToObject);
		func_8027107C();
	}

	set_mario_animation(MARIO_ANIM_SWIM_PART1);
	common_swimming_step(sSwimStrength);

	return FALSE;
}

s32 MarioState::act_swimming_end()
{
	if(this->flags & MARIO_METAL_CAP)
	{
		return set_mario_action(ACT_METAL_WATER_FALLING, 1);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_WATER_PUNCH, 0);
	}

	if(this->actionTimer >= 15 * FRAME_RATE_SCALER_INV)
	{
		return set_mario_action(ACT_WATER_ACTION_END, 0);
	}

	if(check_water_jump())
	{
		return TRUE;
	}

	if((this->input & INPUT_A_DOWN) && this->actionTimer >= 7 * FRAME_RATE_SCALER_INV)
	{
		if(this->actionTimer == 7 * FRAME_RATE_SCALER_INV && sSwimStrength < 280)
		{
			sSwimStrength += 10;
		}
		return set_mario_action(ACT_BREASTSTROKE, 1);
	}

	if(this->actionTimer >= 7 * FRAME_RATE_SCALER_INV)
	{
		sSwimStrength = MIN_SWIM_STRENGTH;
	}

	this->actionTimer++;

	this->forwardVel -= 0.25f * FRAME_RATE_SCALER;
	set_mario_animation(MARIO_ANIM_SWIM_PART2);
	common_swimming_step(sSwimStrength);

	return FALSE;
}

s32 MarioState::act_flutter_kick()
{
	if(this->flags & MARIO_METAL_CAP)
	{
		return set_mario_action(ACT_METAL_WATER_FALLING, 1);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_WATER_PUNCH, 0);
	}

	if(!(this->input & INPUT_A_DOWN))
	{
		if(this->actionTimer == 0 && sSwimStrength < 280)
		{
			sSwimStrength += 10;
		}
		return set_mario_action(ACT_SWIMMING_END, 0);
	}

	this->forwardVel  = approach_f32(this->forwardVel, 12.0f, 0.1f * FRAME_RATE_SCALER, 0.15f * FRAME_RATE_SCALER);
	this->actionTimer = 1;
	sSwimStrength	  = MIN_SWIM_STRENGTH;

	if(this->forwardVel < 14.0f)
	{
		func_802713A8();
		set_mario_animation(MARIO_ANIM_FLUTTERKICK);
	}

	common_swimming_step(sSwimStrength);
	return FALSE;
}

s32 MarioState::act_hold_breaststroke()
{
	if(this->flags & MARIO_METAL_CAP)
	{
		return set_mario_action(ACT_HOLD_METAL_WATER_FALLING, 0);
	}

	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_WATER_IDLE, 0);
	}

	if(++this->actionTimer == 17 * FRAME_RATE_SCALER_INV)
	{
		return set_mario_action(ACT_HOLD_FLUTTER_KICK, 0);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_WATER_THROW, 0);
	}

	if(check_water_jump())
	{
		return TRUE;
	}

	if(this->actionTimer < 6 * FRAME_RATE_SCALER_INV)
	{
		this->forwardVel += 0.5f * FRAME_RATE_SCALER;
	}

	if(this->actionTimer >= 9 * FRAME_RATE_SCALER_INV)
	{
		this->forwardVel += 1.5f * FRAME_RATE_SCALER;
	}

	if(this->actionTimer >= 2 * FRAME_RATE_SCALER_INV)
	{
		if(this->actionTimer < 6 * FRAME_RATE_SCALER_INV && (this->input & INPUT_A_PRESSED))
		{
			this->actionState = 1;
		}

		if(this->actionTimer == 9 * FRAME_RATE_SCALER_INV && this->actionState == 1)
		{
			set_anim_to_frame(0);
			this->actionState = 0;
			this->actionTimer = 1;
		}
	}

	if(this->actionTimer == 1 * FRAME_RATE_SCALER_INV)
	{
		play_sound(SOUND_ACTION_SWIM, this->marioObj->header.gfx.cameraToObject);
		func_8027107C();
	}

	set_mario_animation(MARIO_ANIM_SWIM_WITH_OBJ_PART1);
	common_swimming_step(0x00A0);
	return FALSE;
}

s32 MarioState::act_hold_swimming_end()
{
	if(this->flags & MARIO_METAL_CAP)
	{
		return set_mario_action(ACT_HOLD_METAL_WATER_FALLING, 0);
	}

	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_WATER_IDLE, 0);
	}

	if(this->actionTimer >= 15 * FRAME_RATE_SCALER_INV)
	{
		return set_mario_action(ACT_HOLD_WATER_ACTION_END, 0);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_WATER_THROW, 0);
	}

	if(check_water_jump())
	{
		return TRUE;
	}

	if((this->input & INPUT_A_DOWN) && this->actionTimer >= 7 * FRAME_RATE_SCALER_INV)
	{
		return set_mario_action(ACT_HOLD_BREASTSTROKE, 0);
	}

	this->actionTimer++;

	this->forwardVel -= 0.25f * FRAME_RATE_SCALER;
	set_mario_animation(MARIO_ANIM_SWIM_WITH_OBJ_PART2);
	common_swimming_step(0x00A0);
	return FALSE;
}

s32 MarioState::act_hold_flutter_kick()
{
	if(this->flags & MARIO_METAL_CAP)
	{
		return set_mario_action(ACT_HOLD_METAL_WATER_FALLING, 0);
	}

	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_WATER_IDLE, 0);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_WATER_THROW, 0);
	}

	if(!(this->input & INPUT_A_DOWN))
	{
		return set_mario_action(ACT_HOLD_SWIMMING_END, 0);
	}

	this->forwardVel = approach_f32(this->forwardVel, 12.0f, 0.1f * FRAME_RATE_SCALER, 0.15f * FRAME_RATE_SCALER);
	if(this->forwardVel < 14.0f)
	{
		func_802713A8();
		set_mario_animation(MARIO_ANIM_FLUTTERKICK_WITH_OBJ);
	}
	common_swimming_step(0x00A0);
	return FALSE;
}

s32 MarioState::act_water_shell_swimming()
{
	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_WATER_IDLE, 0);
	}

	if(this->input & INPUT_B_PRESSED)
	{
		return set_mario_action(ACT_WATER_THROW, 0);
	}

	if(this->actionTimer++ == 240 * FRAME_RATE_SCALER_INV)
	{
		this->heldObj->oInteractStatus = INT_STATUS_STOP_RIDING;
		this->heldObj		       = NULL;
		stop_shell_music();
		set_mario_action(ACT_FLUTTER_KICK, 0);
	}

	this->forwardVel = approach_f32(this->forwardVel, 30.0f, 2.0f * FRAME_RATE_SCALER, 1.0f * FRAME_RATE_SCALER);

	func_802713A8();
	set_mario_animation(MARIO_ANIM_FLUTTERKICK_WITH_OBJ);
	common_swimming_step(0x012C);

	return FALSE;
}

s32 MarioState::check_water_grab()
{
	//! Heave hos have the grabbable interaction type but are not normally
	// grabbable. Since water grabbing doesn't check the appropriate input flag,
	// you can use water grab to pick up heave ho.
	if(this->marioObj->collidedObjInteractTypes & INTERACT_GRABBABLE)
	{
		struct Object* object = mario_get_collided_object(this, INTERACT_GRABBABLE);
		f32 dx		      = object->oPosX - this->pos[0];
		f32 dz		      = object->oPosZ - this->pos[2];
		s16 dAngleToObject    = atan2s(dz, dx) - this->faceAngle[1];

		if(dAngleToObject >= -0x2AAA && dAngleToObject <= 0x2AAA)
		{
			this->usedObj = object;
			mario_grab_used_object(this);
			this->marioBodyState->grabPos = GRAB_POS_LIGHT_OBJ;
			return TRUE;
		}
	}

	return FALSE;
}

s32 MarioState::act_water_throw()
{
	update_swimming_yaw();
	update_swimming_pitch();
	update_swimming_speed(MIN_SWIM_SPEED);
	perform_water_step();
	func_80270504();

	set_mario_animation(MARIO_ANIM_WATER_THROW_OBJ);
	play_sound_if_no_flag(SOUND_ACTION_SWIM, MARIO_ACTION_SOUND_PLAYED);

	this->marioBodyState->headAngle[0] = approach_s32(this->marioBodyState->headAngle[0], 0, 0x200 / FRAME_RATE_SCALER_INV, 0x200 / FRAME_RATE_SCALER_INV);

	if(this->actionTimer++ == 5 * FRAME_RATE_SCALER_INV)
	{
		mario_throw_held_object(this);
	}

	if(is_anim_at_end())
	{
		set_mario_action(ACT_WATER_IDLE, 0);
	}

	return FALSE;
}

s32 MarioState::act_water_punch()
{
	if(this->forwardVel < 7.0f)
	{
		this->forwardVel += 1.0f * FRAME_RATE_SCALER;
	}

	update_swimming_yaw();
	update_swimming_pitch();
	update_swimming_speed(MIN_SWIM_SPEED);
	perform_water_step();
	func_80270504();

	this->marioBodyState->headAngle[0] = approach_s32(this->marioBodyState->headAngle[0], 0, 0x200 / FRAME_RATE_SCALER_INV, 0x200 / FRAME_RATE_SCALER_INV);

	play_sound_if_no_flag(SOUND_ACTION_SWIM, MARIO_ACTION_SOUND_PLAYED);

	switch(this->actionState)
	{
		case 0:
			set_mario_animation(MARIO_ANIM_WATER_GRAB_OBJ_PART1);
			if(is_anim_at_end())
			{
				this->actionState = check_water_grab() + 1;
			}
			break;

		case 1:
			set_mario_animation(MARIO_ANIM_WATER_GRAB_OBJ_PART2);
			if(is_anim_at_end())
			{
				set_mario_action(ACT_WATER_ACTION_END, 0);
			}
			break;

		case 2:
			set_mario_animation(MARIO_ANIM_WATER_PICK_UP_OBJ);
			if(is_anim_at_end())
			{
				if(this->heldObj->behavior == segmented_to_virtual(sm64::bhv::bhvKoopaShellUnderwater()))
				{
					play_shell_music();
					set_mario_action(ACT_WATER_SHELL_SWIMMING, 0);
				}
				else
				{
					set_mario_action(ACT_HOLD_WATER_ACTION_END, 1);
				}
			}
			break;
	}

	return FALSE;
}

void MarioState::common_water_knockback_step(s32 animation, u32 endAction, s32 arg3)
{
	stationary_slow_down();
	perform_water_step();
	set_mario_animation(animation);

	this->marioBodyState->headAngle[0] = 0;

	if(is_anim_at_end())
	{
		if(arg3 > 0)
		{
			this->invincTimer = 30 * FRAME_RATE_SCALER_INV;
		}

		set_mario_action(this->health >= 0x100 ? endAction : ACT_WATER_DEATH, 0);
	}
}

s32 MarioState::act_backward_water_kb()
{
	common_water_knockback_step(MARIO_ANIM_BACKWARDS_WATER_KB, ACT_WATER_IDLE, this->actionArg);
	return FALSE;
}

s32 MarioState::act_forward_water_kb()
{
	common_water_knockback_step(MARIO_ANIM_WATER_FORWARD_KB, ACT_WATER_IDLE, this->actionArg);
	return FALSE;
}

s32 MarioState::act_water_shocked()
{
	play_sound_if_no_flag(SOUND_MARIO_WAAAOOOW, MARIO_ACTION_SOUND_PLAYED);
	play_sound(SOUND_MOVING_SHOCKED, this->marioObj->header.gfx.cameraToObject);
	set_camera_shake_from_hit(SHAKE_SHOCK);

	if(set_mario_animation(MARIO_ANIM_SHOCKED) == 0)
	{
		this->actionTimer++;
		this->flags |= MARIO_METAL_SHOCK;
	}

	if(this->actionTimer >= 6 * FRAME_RATE_SCALER_INV)
	{
		this->invincTimer = 30 * FRAME_RATE_SCALER_INV;
		set_mario_action(this->health < 0x100 ? ACT_WATER_DEATH : ACT_WATER_IDLE, 0);
	}

	stationary_slow_down();
	perform_water_step();
	this->marioBodyState->headAngle[0] = 0;
	return FALSE;
}

s32 MarioState::act_drowning()
{
	switch(this->actionState)
	{
		case 0:
			set_mario_animation(MARIO_ANIM_DROWNING_PART1);
			this->marioBodyState->eyeState = MARIO_EYES_HALF_CLOSED;
			if(is_anim_at_end())
			{
				this->actionState = 1;
			}
			break;

		case 1:
			set_mario_animation(MARIO_ANIM_DROWNING_PART2);
			this->marioBodyState->eyeState = MARIO_EYES_DEAD;
			if(this->marioObj->header.gfx.unk38.frame() == 30)
			{
				level_trigger_warp(this, WARP_OP_DEATH);
			}
			break;
	}

	play_sound_if_no_flag(SOUND_MARIO_DROWNING, MARIO_ACTION_SOUND_PLAYED);
	stationary_slow_down();
	perform_water_step();

	return FALSE;
}

s32 MarioState::act_water_death()
{
	stationary_slow_down();
	perform_water_step();

	this->marioBodyState->eyeState = MARIO_EYES_DEAD;

	set_mario_animation(MARIO_ANIM_WATER_DYING);
	if(set_mario_animation(MARIO_ANIM_WATER_DYING) == 35)
	{
		level_trigger_warp(this, WARP_OP_DEATH);
	}

	return FALSE;
}

s32 MarioState::act_water_plunge()
{
	u32 stepResult;
	s32 stateFlags = this->heldObj != NULL;

	f32 endVSpeed;
	if(swimming_near_surface())
	{
		endVSpeed = 0.0f;
	}
	else
	{
		endVSpeed = -5.0f;
	}

	if(this->flags & MARIO_METAL_CAP)
	{
		stateFlags |= 4;
	}
	else if((this->prevAction & ACT_FLAG_DIVING) || (this->input & INPUT_A_DOWN))
	{
		stateFlags |= 2;
	}

	this->actionTimer++;

	stationary_slow_down();

	stepResult = perform_water_step();

	if(this->actionState == 0)
	{
		play_sound(SOUND_ACTION_UNKNOWN430, this->marioObj->header.gfx.cameraToObject);
		if(this->peakHeight - this->pos[1] > 1150.0f)
		{
			play_sound(SOUND_MARIO_HAHA_2, this->marioObj->header.gfx.cameraToObject);
		}

		this->particleFlags |= PARTICLE_WATER_SPLASH;
		this->actionState = 1;
	}

	if(stepResult == WATER_STEP_HIT_FLOOR || this->vel[1] >= endVSpeed || this->actionTimer > 20 * FRAME_RATE_SCALER_INV)
	{
		switch(stateFlags)
		{
			case 0:
				set_mario_action(ACT_WATER_ACTION_END, 0);
				break;
			case 1:
				set_mario_action(ACT_HOLD_WATER_ACTION_END, 0);
				break;
			case 2:
				set_mario_action(ACT_FLUTTER_KICK, 0);
				break;
			case 3:
				set_mario_action(ACT_HOLD_FLUTTER_KICK, 0);
				break;
			case 4:
				set_mario_action(ACT_METAL_WATER_FALLING, 0);
				break;
			case 5:
				set_mario_action(ACT_HOLD_METAL_WATER_FALLING, 0);
				break;
		}
		D_80339FD2 = 0;
	}

	switch(stateFlags)
	{
		case 0:
			set_mario_animation(MARIO_ANIM_WATER_ACTION_END);
			break;
		case 1:
			set_mario_animation(MARIO_ANIM_WATER_ACTION_END_WITH_OBJ);
			break;
		case 2:
			set_mario_animation(MARIO_ANIM_FLUTTERKICK);
			break;
		case 3:
			set_mario_animation(MARIO_ANIM_FLUTTERKICK_WITH_OBJ);
			break;
		case 4:
			set_mario_animation(MARIO_ANIM_GENERAL_FALL);
			break;
		case 5:
			set_mario_animation(MARIO_ANIM_FALL_WITH_LIGHT_OBJ);
			break;
	}

	this->particleFlags |= PARTICLE_WATER_WAVES;
	return FALSE;
}

s32 MarioState::act_caught_in_whirlpool()
{
	f32 sinAngleChange;
	f32 cosAngleChange;
	f32 newDistance;
	s16 angleChange;

	struct Object* marioObj	 = this->marioObj;
	struct Object* whirlpool = this->usedObj;

	f32 dx	     = this->pos[0] - whirlpool->oPosX;
	f32 dz	     = this->pos[2] - whirlpool->oPosZ;
	f32 distance = sqrtf(dx * dx + dz * dz);

	if((marioObj->oMarioWhirlpoolPosY += this->vel[1] * FRAME_RATE_SCALER) < 0.0f)
	{
		marioObj->oMarioWhirlpoolPosY = 0.0f;
		if(distance < 16.1f && this->actionTimer++ == 16 * FRAME_RATE_SCALER_INV)
		{
			level_trigger_warp(this, WARP_OP_DEATH);
		}
	}

	if(distance <= 28.0f)
	{
		newDistance = 16.0f;
		angleChange = 0x1800;
	}
	else if(distance < 256.0f)
	{
		newDistance = distance - (12.0f - distance / 32.0f);
		angleChange = (s16)(0x1C00 - distance * 20.0f);
	}
	else
	{
		newDistance = distance - 4.0f;
		angleChange = 0x800;
	}

	this->vel[1] = -640.0f / (newDistance + 16.0f);

	sinAngleChange = sins(angleChange);
	cosAngleChange = coss(angleChange);

	if(distance < 1.0f)
	{
		dx = newDistance * sins(this->faceAngle[1]);
		dz = newDistance * coss(this->faceAngle[1]);
	}
	else
	{
		dx *= newDistance / distance;
		dz *= newDistance / distance;
	}

	this->pos[0] = whirlpool->oPosX + dx * cosAngleChange + dz * sinAngleChange;
	this->pos[2] = whirlpool->oPosZ - dx * sinAngleChange + dz * cosAngleChange;
	this->pos[1] = whirlpool->oPosY + marioObj->oMarioWhirlpoolPosY;

	this->faceAngle[1] = atan2s(dz, dx) + 0x8000;

	set_mario_animation(MARIO_ANIM_GENERAL_FALL);
	vec3f_copy(this->marioObj->header.gfx.pos, this->pos);
	vec3s_set(this->marioObj->header.gfx.angle, 0, this->faceAngle[1], 0);

	return FALSE;
}

void MarioState::play_metal_water_jumping_sound(u32 landing)
{
	if(!(this->flags & MARIO_ACTION_SOUND_PLAYED))
	{
		this->particleFlags |= PARTICLE_POUND_WHITE_PUFF;
	}

	play_sound_if_no_flag(landing ? SOUND_ACTION_METAL_LAND_WATER : SOUND_ACTION_METAL_JUMP_WATER, MARIO_ACTION_SOUND_PLAYED);
}

void MarioState::play_metal_water_walking_sound()
{
	if(is_anim_past_frame(10) || is_anim_past_frame(49))
	{
		play_sound(SOUND_ACTION_METAL_STEP_WATER, this->marioObj->header.gfx.cameraToObject);
		this->particleFlags |= PARTICLE_DUST;
	}
}

void MarioState::update_metal_water_walking_speed()
{
	f32 val = this->intendedMag / 1.5f;

	if(this->forwardVel <= 0.0f)
	{
		this->forwardVel += 1.1f * FRAME_RATE_SCALER;
	}
	else if(this->forwardVel <= val)
	{
		this->forwardVel += (1.1f - this->forwardVel / 43.0f) * FRAME_RATE_SCALER;
	}
	else if(this->floor->normal.y >= 0.95f)
	{
		this->forwardVel -= 1.0f * FRAME_RATE_SCALER;
	}

	if(this->forwardVel > 32.0f)
	{
		this->forwardVel = 32.0f;
	}

	this->faceAngle[1] = this->intendedYaw - approach_s32((s16)(this->intendedYaw - this->faceAngle[1]), 0, 0x800 / FRAME_RATE_SCALER_INV, 0x800 / FRAME_RATE_SCALER_INV);

	this->slideVelX = this->forwardVel * sins(this->faceAngle[1]);
	this->slideVelZ = this->forwardVel * coss(this->faceAngle[1]);

	this->vel[0] = this->slideVelX;
	this->vel[1] = 0.0f;
	this->vel[2] = this->slideVelZ;
}

s32 MarioState::update_metal_water_jump_speed()
{
	UNUSED f32 nextY = this->pos[1] + this->vel[1];
	f32 waterSurface = this->waterLevel - 100;

	if(this->vel[1] > 0.0f && this->pos[1] > waterSurface)
	{
		return TRUE;
	}

	if(this->input & INPUT_NONZERO_ANALOG)
	{
		s16 intendedDYaw = this->intendedYaw - this->faceAngle[1];
		this->forwardVel += 0.8f * FRAME_RATE_SCALER * coss(intendedDYaw);
		this->faceAngle[1] += 0x200 / FRAME_RATE_SCALER_INV * sins(intendedDYaw);
	}
	else
	{
		this->forwardVel = approach_f32(this->forwardVel, 0.0f, 0.25f * FRAME_RATE_SCALER, 0.25f * FRAME_RATE_SCALER);
	}

	if(this->forwardVel > 16.0f)
	{
		this->forwardVel -= 1.0f * FRAME_RATE_SCALER;
	}

	if(this->forwardVel < 0.0f)
	{
		this->forwardVel += 2.0f * FRAME_RATE_SCALER;
	}

	this->vel[0] = this->slideVelX = this->forwardVel * sins(this->faceAngle[1]);
	this->vel[2] = this->slideVelZ = this->forwardVel * coss(this->faceAngle[1]);
	return FALSE;
}

s32 MarioState::act_metal_water_standing()
{
	if(!(this->flags & MARIO_METAL_CAP))
	{
		return set_mario_action(ACT_WATER_IDLE, 0);
	}

	if(this->input & INPUT_A_PRESSED)
	{
		return set_mario_action(ACT_METAL_WATER_JUMP, 0);
	}

	if(this->input & INPUT_NONZERO_ANALOG)
	{
		return set_mario_action(ACT_METAL_WATER_WALKING, 0);
	}

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

	if(is_anim_at_end() && ++this->actionState == 3)
	{
		this->actionState = 0;
	}

	stop_and_set_height_to_floor();
	if(this->pos[1] >= this->waterLevel - 150)
	{
		this->particleFlags |= PARTICLE_SURFACE_WAVES;
	}

	return FALSE;
}

s32 MarioState::act_hold_metal_water_standing()
{
	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_METAL_WATER_STANDING, 0);
	}

	if(!(this->flags & MARIO_METAL_CAP))
	{
		return set_mario_action(ACT_HOLD_WATER_IDLE, 0);
	}

	if(this->input & INPUT_A_PRESSED)
	{
		return set_mario_action(ACT_HOLD_METAL_WATER_JUMP, 0);
	}

	if(this->input & INPUT_NONZERO_ANALOG)
	{
		return set_mario_action(ACT_HOLD_METAL_WATER_WALKING, 0);
	}

	stop_and_set_height_to_floor();
	set_mario_animation(MARIO_ANIM_IDLE_WITH_LIGHT_OBJ);
	return FALSE;
}

s32 MarioState::act_metal_water_walking()
{
	s32 val04;

	if(!(this->flags & MARIO_METAL_CAP))
	{
		return set_mario_action(ACT_WATER_IDLE, 0);
	}

	if(this->input & INPUT_FIRST_PERSON)
	{
		return set_mario_action(ACT_METAL_WATER_STANDING, 0);
	}

	if(this->input & INPUT_A_PRESSED)
	{
		return set_mario_action(ACT_METAL_WATER_JUMP, 0);
	}

	if(this->input & INPUT_UNKNOWN_5)
	{
		return set_mario_action(ACT_METAL_WATER_STANDING, 0);
	}

	if((val04 = (s32)(this->forwardVel / 4.0f * 0x10000)) < 0x1000)
	{
		val04 = 0x1000;
	}

	set_mario_anim_with_accel(MARIO_ANIM_WALKING, val04);
	play_metal_water_walking_sound();
	update_metal_water_walking_speed();

	switch(perform_ground_step())
	{
		case GROUND_STEP_LEFT_GROUND:
			set_mario_action(ACT_METAL_WATER_FALLING, 1);
			break;

		case GROUND_STEP_HIT_WALL:
			this->forwardVel = 0;
			break;
	}

	return FALSE;
}

s32 MarioState::act_hold_metal_water_walking()
{
	s32 val04;

	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_METAL_WATER_WALKING, 0);
	}

	if(!(this->flags & MARIO_METAL_CAP))
	{
		return set_mario_action(ACT_HOLD_WATER_IDLE, 0);
	}

	if(this->input & INPUT_A_PRESSED)
	{
		return set_mario_action(ACT_HOLD_METAL_WATER_JUMP, 0);
	}

	if(this->input & INPUT_UNKNOWN_5)
	{
		return set_mario_action(ACT_HOLD_METAL_WATER_STANDING, 0);
	}

	this->intendedMag *= 0.4f;

	if((val04 = (s32)(this->forwardVel / 2.0f * 0x10000)) < 0x1000)
	{
		val04 = 0x1000;
	}

	set_mario_anim_with_accel(MARIO_ANIM_RUN_WITH_LIGHT_OBJ, val04);
	play_metal_water_walking_sound();
	update_metal_water_walking_speed();

	switch(perform_ground_step())
	{
		case GROUND_STEP_LEFT_GROUND:
			set_mario_action(ACT_HOLD_METAL_WATER_FALLING, 1);
			break;

		case GROUND_STEP_HIT_WALL:
			this->forwardVel = 0.0f;
			break;
	}

	return FALSE;
}

s32 MarioState::act_metal_water_jump()
{
	if(!(this->flags & MARIO_METAL_CAP))
	{
		return set_mario_action(ACT_WATER_IDLE, 0);
	}

	if(update_metal_water_jump_speed())
	{
		return set_mario_action(ACT_WATER_JUMP, 1);
	}

	play_metal_water_jumping_sound(FALSE);
	set_mario_animation(MARIO_ANIM_SINGLE_JUMP);

	switch(perform_air_step(0))
	{
		case AIR_STEP_LANDED:
			set_mario_action(ACT_METAL_WATER_JUMP_LAND, 0);
			break;

		case AIR_STEP_HIT_WALL:
			this->forwardVel = 0.0f;
			break;
	}

	return FALSE;
}

s32 MarioState::act_hold_metal_water_jump()
{
	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_METAL_WATER_FALLING, 0);
	}

	if(!(this->flags & MARIO_METAL_CAP))
	{
		return set_mario_action(ACT_HOLD_WATER_IDLE, 0);
	}

	if(update_metal_water_jump_speed())
	{
		return set_mario_action(ACT_HOLD_WATER_JUMP, 1);
	}

	play_metal_water_jumping_sound(FALSE);
	set_mario_animation(MARIO_ANIM_JUMP_WITH_LIGHT_OBJ);

	switch(perform_air_step(0))
	{
		case AIR_STEP_LANDED:
			set_mario_action(ACT_HOLD_METAL_WATER_JUMP_LAND, 0);
			break;

		case AIR_STEP_HIT_WALL:
			this->forwardVel = 0.0f;
			break;
	}

	return FALSE;
}

s32 MarioState::act_metal_water_falling()
{
	if(!(this->flags & MARIO_METAL_CAP))
	{
		return set_mario_action(ACT_WATER_IDLE, 0);
	}

	if(this->input & INPUT_NONZERO_ANALOG)
	{
		this->faceAngle[1] += 0x400 / FRAME_RATE_SCALER_INV * sins(this->intendedYaw - this->faceAngle[1]);
	}

	set_mario_animation(this->actionArg == 0 ? MARIO_ANIM_GENERAL_FALL : MARIO_ANIM_FALL_FROM_WATER);
	stationary_slow_down();

	if(perform_water_step() & WATER_STEP_HIT_FLOOR)
	{ // hit floor or cancelled
		set_mario_action(ACT_METAL_WATER_FALL_LAND, 0);
	}

	return FALSE;
}

s32 MarioState::act_hold_metal_water_falling()
{
	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_METAL_WATER_FALLING, 0);
	}

	if(!(this->flags & MARIO_METAL_CAP))
	{
		return set_mario_action(ACT_HOLD_WATER_IDLE, 0);
	}

	if(this->input & INPUT_NONZERO_ANALOG)
	{
		this->faceAngle[1] += 0x400 / FRAME_RATE_SCALER_INV * sins(this->intendedYaw - this->faceAngle[1]);
	}

	set_mario_animation(MARIO_ANIM_FALL_WITH_LIGHT_OBJ);
	stationary_slow_down();

	if(perform_water_step() & WATER_STEP_HIT_FLOOR)
	{ // hit floor or cancelled
		set_mario_action(ACT_HOLD_METAL_WATER_FALL_LAND, 0);
	}

	return FALSE;
}

s32 MarioState::act_metal_water_jump_land()
{
	play_metal_water_jumping_sound(TRUE);

	if(!(this->flags & MARIO_METAL_CAP))
	{
		return set_mario_action(ACT_WATER_IDLE, 0);
	}

	if(this->input & INPUT_NONZERO_ANALOG)
	{
		return set_mario_action(ACT_METAL_WATER_WALKING, 0);
	}

	stop_and_set_height_to_floor();
	set_mario_animation(MARIO_ANIM_LAND_FROM_SINGLE_JUMP);

	if(is_anim_at_end())
	{
		return set_mario_action(ACT_METAL_WATER_STANDING, 0);
	}

	return FALSE;
}

s32 MarioState::act_hold_metal_water_jump_land()
{
	play_metal_water_jumping_sound(TRUE);

	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_METAL_WATER_STANDING, 0);
	}

	if(!(this->flags & MARIO_METAL_CAP))
	{
		return set_mario_action(ACT_HOLD_WATER_IDLE, 0);
	}

	if(this->input & INPUT_NONZERO_ANALOG)
	{
		return set_mario_action(ACT_HOLD_METAL_WATER_WALKING, 0);
	}

	stop_and_set_height_to_floor();
	set_mario_animation(MARIO_ANIM_JUMP_LAND_WITH_LIGHT_OBJ);

	if(is_anim_at_end())
	{
		return set_mario_action(ACT_HOLD_METAL_WATER_STANDING, 0);
	}

	return FALSE;
}

s32 MarioState::act_metal_water_fall_land()
{
	play_metal_water_jumping_sound(TRUE);

	if(!(this->flags & MARIO_METAL_CAP))
	{
		return set_mario_action(ACT_WATER_IDLE, 0);
	}

	if(this->input & INPUT_NONZERO_ANALOG)
	{
		return set_mario_action(ACT_METAL_WATER_WALKING, 0);
	}

	stop_and_set_height_to_floor();
	set_mario_animation(MARIO_ANIM_GENERAL_LAND);

	if(is_anim_at_end())
	{
		return set_mario_action(ACT_METAL_WATER_STANDING, 0);
	}

	return FALSE;
}

s32 MarioState::act_hold_metal_water_fall_land()
{
	play_metal_water_jumping_sound(TRUE);

	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_METAL_WATER_STANDING, 0);
	}

	if(!(this->flags & MARIO_METAL_CAP))
	{
		return set_mario_action(ACT_HOLD_WATER_IDLE, 0);
	}

	if(this->input & INPUT_NONZERO_ANALOG)
	{
		return set_mario_action(ACT_HOLD_METAL_WATER_WALKING, 0);
	}

	stop_and_set_height_to_floor();
	set_mario_animation(MARIO_ANIM_FALL_LAND_WITH_LIGHT_OBJ);

	if(is_anim_at_end())
	{
		return set_mario_action(ACT_HOLD_METAL_WATER_STANDING, 0);
	}

	return FALSE;
}

s32 MarioState::check_common_submerged_cancels()
{
	if(this->pos[1] > this->waterLevel - 80)
	{
		if(this->waterLevel - 80 > this->floorHeight)
		{
			this->pos[1] = this->waterLevel - 80;
		}
		else
		{
			//! If you press B to throw the shell, there is a ~5 frame window
			// where your held object is the shell, but you are not in the
			// water shell swimming action. This allows you to hold the water
			// shell on land (used for cloning in DDD).
			if(this->action == ACT_WATER_SHELL_SWIMMING && this->heldObj != NULL)
			{
				this->heldObj->oInteractStatus = INT_STATUS_STOP_RIDING;
				this->heldObj		       = NULL;
				stop_shell_music();
			}

			return transition_submerged_to_walking();
		}
	}

	if(this->health < 0x100 && !(this->action & (ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE)))
	{
		set_mario_action(ACT_DROWNING, 0);
	}

	return FALSE;
}

s32 MarioState::mario_execute_submerged_action()
{
	s32 cancel;

	if(check_common_submerged_cancels())
	{
		return TRUE;
	}

	this->quicksandDepth = 0.0f;

	this->marioBodyState->headAngle[1] = 0;
	this->marioBodyState->headAngle[2] = 0;

	/* clang-format off */
	switch(this->action)
	{
		case ACT_WATER_IDLE:                 cancel = act_water_idle();                 break;
		case ACT_HOLD_WATER_IDLE:            cancel = act_hold_water_idle();            break;
		case ACT_WATER_ACTION_END:           cancel = act_water_action_end();           break;
		case ACT_HOLD_WATER_ACTION_END:      cancel = act_hold_water_action_end();      break;
		case ACT_DROWNING:                   cancel = act_drowning();                   break;
		case ACT_BACKWARD_WATER_KB:          cancel = act_backward_water_kb();          break;
		case ACT_FORWARD_WATER_KB:           cancel = act_forward_water_kb();           break;
		case ACT_WATER_DEATH:                cancel = act_water_death();                break;
		case ACT_WATER_SHOCKED:              cancel = act_water_shocked();              break;
		case ACT_BREASTSTROKE:               cancel = act_breaststroke();               break;
		case ACT_SWIMMING_END:               cancel = act_swimming_end();               break;
		case ACT_FLUTTER_KICK:               cancel = act_flutter_kick();               break;
		case ACT_HOLD_BREASTSTROKE:          cancel = act_hold_breaststroke();          break;
		case ACT_HOLD_SWIMMING_END:          cancel = act_hold_swimming_end();          break;
		case ACT_HOLD_FLUTTER_KICK:          cancel = act_hold_flutter_kick();          break;
		case ACT_WATER_SHELL_SWIMMING:       cancel = act_water_shell_swimming();       break;
		case ACT_WATER_THROW:                cancel = act_water_throw();                break;
		case ACT_WATER_PUNCH:                cancel = act_water_punch();                break;
		case ACT_WATER_PLUNGE:               cancel = act_water_plunge();               break;
		case ACT_CAUGHT_IN_WHIRLPOOL:        cancel = act_caught_in_whirlpool();        break;
		case ACT_METAL_WATER_STANDING:       cancel = act_metal_water_standing();       break;
		case ACT_METAL_WATER_WALKING:        cancel = act_metal_water_walking();        break;
		case ACT_METAL_WATER_FALLING:        cancel = act_metal_water_falling();        break;
		case ACT_METAL_WATER_FALL_LAND:      cancel = act_metal_water_fall_land();      break;
		case ACT_METAL_WATER_JUMP:           cancel = act_metal_water_jump();           break;
		case ACT_METAL_WATER_JUMP_LAND:      cancel = act_metal_water_jump_land();      break;
		case ACT_HOLD_METAL_WATER_STANDING:  cancel = act_hold_metal_water_standing();  break;
		case ACT_HOLD_METAL_WATER_WALKING:   cancel = act_hold_metal_water_walking();   break;
		case ACT_HOLD_METAL_WATER_FALLING:   cancel = act_hold_metal_water_falling();   break;
		case ACT_HOLD_METAL_WATER_FALL_LAND: cancel = act_hold_metal_water_fall_land(); break;
		case ACT_HOLD_METAL_WATER_JUMP:      cancel = act_hold_metal_water_jump();      break;
		case ACT_HOLD_METAL_WATER_JUMP_LAND: cancel = act_hold_metal_water_jump_land(); break;
	}
	/* clang-format on */

	return cancel;
}
