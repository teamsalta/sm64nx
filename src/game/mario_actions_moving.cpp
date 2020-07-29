#include <ultra64.h>

#include "sm64.h"
#include "mario.h"
#include "audio/external.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "mario_step.h"
#include "area.h"
#include "interaction.h"
#include "mario_actions_object.h"
#include "memory.h"
#include "behavior_data.h"
#include "motor.h"

struct LandingAction
{
	s16 numFrames;
	s16 unk02;
	u32 verySteepAction;
	u32 endAction;
	u32 aPressedAction;
	u32 offFloorAction;
	u32 slideAction;
};

struct LandingAction sJumpLandAction = {
    4, 5, ACT_FREEFALL, ACT_JUMP_LAND_STOP, ACT_DOUBLE_JUMP, ACT_FREEFALL, ACT_BEGIN_SLIDING,
};

struct LandingAction sFreefallLandAction = {
    4, 5, ACT_FREEFALL, ACT_FREEFALL_LAND_STOP, ACT_DOUBLE_JUMP, ACT_FREEFALL, ACT_BEGIN_SLIDING,
};

struct LandingAction sSideFlipLandAction = {
    4, 5, ACT_FREEFALL, ACT_SIDE_FLIP_LAND_STOP, ACT_DOUBLE_JUMP, ACT_FREEFALL, ACT_BEGIN_SLIDING,
};

struct LandingAction sHoldJumpLandAction = {
    4, 5, ACT_HOLD_FREEFALL, ACT_UNKNOWN_034, ACT_HOLD_JUMP, ACT_HOLD_FREEFALL, ACT_HOLD_BEGIN_SLIDING,
};

struct LandingAction sHoldFreefallLandAction = {
    4, 5, ACT_HOLD_FREEFALL, ACT_UNKNOWN_035, ACT_HOLD_JUMP, ACT_HOLD_FREEFALL, ACT_HOLD_BEGIN_SLIDING,
};

struct LandingAction sLongJumpLandAction = {
    6, 5, ACT_FREEFALL, ACT_LONG_JUMP_LAND_STOP, ACT_LONG_JUMP, ACT_FREEFALL, ACT_BEGIN_SLIDING,
};

struct LandingAction sDoubleJumpLandAction = {
    4, 5, ACT_FREEFALL, ACT_DOUBLE_JUMP_LAND_STOP, ACT_JUMP, ACT_FREEFALL, ACT_BEGIN_SLIDING,
};

struct LandingAction sTripleJumpLandAction = {
    4, 0, ACT_FREEFALL, ACT_TRIPLE_JUMP_LAND_STOP, ACT_UNINITIALIZED, ACT_FREEFALL, ACT_BEGIN_SLIDING,
};

struct LandingAction sBackflipLandAction = {
    4, 0, ACT_FREEFALL, ACT_BACKFLIP_LAND_STOP, ACT_BACKFLIP, ACT_FREEFALL, ACT_BEGIN_SLIDING,
};

Mat4 D_80339F50[2];

s16 PlayerRecord::func_80263A50()
{
	s16 pitch = find_floor_slope(0);
	pitch	  = pitch * forwardVel / 40.0f;
	return -pitch;
}

void PlayerRecord::func_80263AD4(s16 frame1, s16 frame2)
{
	if(is_anim_past_frame(frame1) || is_anim_past_frame(frame2))
	{
		if(flags & MARIO_METAL_CAP)
		{
			if(marioObj->header.gfx.unk38.animID == MARIO_ANIM_TIPTOE)
			{
				play_sound_and_spawn_particles(SOUND_ACTION_METAL_STEP_TIPTOE, 0);
			}
			else
			{
				play_sound_and_spawn_particles(SOUND_ACTION_METAL_STEP, 0);
			}
		}
		else if(sinking > 50.0f)
		{
			AudStartSound(SOUND_ACTION_QUICKSAND_STEP, marioObj->header.gfx.cameraToObject);
		}
		else if(marioObj->header.gfx.unk38.animID == MARIO_ANIM_TIPTOE)
		{
			play_sound_and_spawn_particles(SOUND_ACTION_TERRAIN_STEP_TIPTOE, 0);
		}
		else
		{
			play_sound_and_spawn_particles(SOUND_ACTION_TERRAIN_STEP, 0);
		}
	}
}

void PlayerRecord::func_80263C14()
{
	pos[1] = floorHeight;
	mtxf_align_terrain_triangle(D_80339F50[number], pos, faceAngle[1], 40.0f);
	marioObj->header.gfx.throwMatrix = &D_80339F50[number];
}

s32 PlayerRecord::begin_walking_action(f32 forwardVel, u32 action, u32 actionArg)
{
	faceAngle[1] = intendedYaw;
	mario_set_forward_vel(forwardVel);
	return ChangePlayerStatus(action, actionArg);
}

void PlayerRecord::check_ledge_climb_down()
{
	struct WallCollisionData wallCols;
	struct Surface* floor;
	f32 floorHeight;
	struct Surface* wall;
	s16 wallAngle;
	s16 wallDYaw;

	if(forwardVel < 10.0f)
	{
		wallCols.x	 = pos[0];
		wallCols.y	 = pos[1];
		wallCols.z	 = pos[2];
		wallCols.radius	 = 10.0f;
		wallCols.offsetY = -10.0f;

		if(find_wall_collisions(&wallCols) != 0)
		{
			floorHeight = mcBGGroundCheck(wallCols.x, wallCols.y, wallCols.z, &floor);
			if(floor != NULL)
			{
				if(wallCols.y - floorHeight > 160.0f)
				{
					wall	  = wallCols.walls[wallCols.numWalls - 1];
					wallAngle = atan2s(wall->normal.z, wall->normal.x);
					wallDYaw  = wallAngle - faceAngle[1];

					if(wallDYaw > -0x4000 && wallDYaw < 0x4000)
					{
						pos[0] = wallCols.x - 20.0f * wall->normal.x;
						pos[2] = wallCols.z - 20.0f * wall->normal.z;

						faceAngle[0] = 0;
						faceAngle[1] = wallAngle + 0x8000;

						ChangePlayerStatus(ACT_LEDGE_CLIMB_DOWN, 0);
						set_mario_animation(MARIO_ANIM_CLIMB_DOWN_LEDGE);
					}
				}
			}
		}
	}
}

void PlayerRecord::slide_bonk(u32 fastAction, u32 slowAction)
{
	if(forwardVel > 16.0f)
	{
		mario_bonk_reflection(TRUE);
		drop_and_set_mario_action(fastAction, 0);
	}
	else
	{
		mario_set_forward_vel(0.0f);
		ChangePlayerStatus(slowAction, 0);
	}
}

s32 PlayerRecord::set_triple_jump_action(UNUSED u32 action, UNUSED u32 actionArg)
{
	if(flags & MARIO_WING_CAP)
	{
		return ChangePlayerStatus(ACT_FLYING_TRIPLE_JUMP, 0);
	}
	else if(forwardVel > 20.0f)
	{
		return ChangePlayerStatus(ACT_TRIPLE_JUMP, 0);
	}
	else
	{
		return ChangePlayerStatus(ACT_JUMP, 0);
	}

	return 0;
}

s32 PlayerRecord::set_special_triple_jump_action(UNUSED u32 action, UNUSED u32 actionArg)
{
	if(forwardVel > 20.0f)
	{
		return ChangePlayerStatus(ACT_SPECIAL_TRIPLE_JUMP, 0);
	}
	else
	{
		return ChangePlayerStatus(ACT_JUMP, 0);
	}

	return 0;
}

void PlayerRecord::update_sliding_angle(f32 accel, f32 lossFactor)
{
	s32 newFacingDYaw;
	s16 facingDYaw;

	s16 slopeAngle	   = atan2s(floor->normal.z, floor->normal.x);
	f32 steepness	   = sqrtf(floor->normal.x * floor->normal.x + floor->normal.z * floor->normal.z);
	UNUSED f32 normalY = floor->normal.y;

	slideVelX += accel * steepness * sins(slopeAngle) * FRAME_RATE_SCALER;
	slideVelZ += accel * steepness * coss(slopeAngle) * FRAME_RATE_SCALER;

	slideVelX *= lossFactor;
	slideVelZ *= lossFactor;

	slideYaw = atan2s(slideVelZ, slideVelX);

	facingDYaw    = faceAngle[1] - slideYaw;
	newFacingDYaw = facingDYaw;

	//! -0x4000 not handled - can slide down a slope while facing perpendicular to it
	if(newFacingDYaw > 0 && newFacingDYaw <= 0x4000)
	{
		if((newFacingDYaw -= 0x200) < 0)
		{
			newFacingDYaw = 0;
		}
	}
	else if(newFacingDYaw > -0x4000 && newFacingDYaw < 0)
	{
		if((newFacingDYaw += 0x200 / FRAME_RATE_SCALER_INV) > 0)
		{
			newFacingDYaw = 0;
		}
	}
	else if(newFacingDYaw > 0x4000 && newFacingDYaw < 0x8000)
	{
		if((newFacingDYaw += 0x200 / FRAME_RATE_SCALER_INV) > 0x8000)
		{
			newFacingDYaw = 0x8000;
		}
	}
	else if(newFacingDYaw > -0x8000 && newFacingDYaw < -0x4000)
	{
		if((newFacingDYaw -= 0x200) < -0x8000)
		{
			newFacingDYaw = -0x8000;
		}
	}

	faceAngle[1] = slideYaw + newFacingDYaw;

	vel[0] = slideVelX;
	vel[1] = 0.0f;
	vel[2] = slideVelZ;

	mario_update_moving_sand();
	mario_update_windy_ground();

	//! Speed is capped a frame late (butt slide HSG)
	forwardVel = sqrtf(slideVelX * slideVelX + slideVelZ * slideVelZ);
	if(forwardVel > 100.0f)
	{
		slideVelX = slideVelX * 100.0f / forwardVel;
		slideVelZ = slideVelZ * 100.0f / forwardVel;
	}

	if(newFacingDYaw < -0x4000 || newFacingDYaw > 0x4000)
	{
		forwardVel *= -1.0f;
	}
}

s32 PlayerRecord::update_sliding(f32 stopSpeed)
{
	f32 lossFactor;
	f32 accel;
	f32 oldSpeed;
	f32 newSpeed;

	s32 stopped = FALSE;

	s16 intendedDYaw = intendedYaw - slideYaw;
	f32 forward	 = coss(intendedDYaw);
	f32 sideward	 = sins(intendedDYaw);

	//! 10k glitch
	if(forward < 0.0f && forwardVel >= 0.0f)
	{
		forward *= 0.5f + 0.5f * forwardVel / 100.0f;
	}

	switch(mario_get_floor_class())
	{
		case SURFACE_CLASS_VERY_SLIPPERY:
			accel	   = 10.0f;
			lossFactor = intendedMag / 32.0f * forward * 0.02f + 0.98f;
			break;

		case SURFACE_CLASS_SLIPPERY:
			accel	   = 8.0f;
			lossFactor = intendedMag / 32.0f * forward * 0.02f + 0.96f;
			break;

		default:
			accel	   = 7.0f;
			lossFactor = intendedMag / 32.0f * forward * 0.02f + 0.92f;
			break;

		case SURFACE_CLASS_NOT_SLIPPERY:
			accel	   = 5.0f;
			lossFactor = intendedMag / 32.0f * forward * 0.02f + 0.92f;
			break;
	}

	lossFactor = scale_scaler(lossFactor, FRAME_RATE_SCALER);

	oldSpeed = sqrtf(slideVelX * slideVelX + slideVelZ * slideVelZ);

	//! This is attempting to use trig derivatives to rotate mario's speed.
	// It is slightly off/asymmetric since it uses the new X speed, but the old
	// Z speed.
	slideVelX += slideVelZ * (intendedMag / 32.0f) * sideward * 0.05f * FRAME_RATE_SCALER;
	slideVelZ -= slideVelX * (intendedMag / 32.0f) * sideward * 0.05f * FRAME_RATE_SCALER;

	newSpeed = sqrtf(slideVelX * slideVelX + slideVelZ * slideVelZ);

	if(oldSpeed > 0.0f && newSpeed > 0.0f)
	{
		slideVelX = slideVelX * oldSpeed / newSpeed;
		slideVelZ = slideVelZ * oldSpeed / newSpeed;
	}

	update_sliding_angle(accel, lossFactor);

	if(!mario_floor_is_slope() && forwardVel * forwardVel < stopSpeed * stopSpeed)
	{
		mario_set_forward_vel(0.0f);
		stopped = TRUE;
	}

	return stopped;
}

void PlayerRecord::apply_slope_accel()
{
	f32 slopeAccel;

	f32 steepness = sqrtf(floor->normal.x * floor->normal.x + floor->normal.z * floor->normal.z);

	UNUSED f32 normalY = floor->normal.y;
	s16 floorDYaw	   = floorAngle - faceAngle[1];

	if(mario_floor_is_slope())
	{
		s16 slopeClass = 0;

		if(status != ACT_SOFT_BACKWARD_GROUND_KB && status != ACT_SOFT_FORWARD_GROUND_KB)
		{
			slopeClass = mario_get_floor_class();
		}

		switch(slopeClass)
		{
			case SURFACE_CLASS_VERY_SLIPPERY:
				slopeAccel = 5.3f;
				break;
			case SURFACE_CLASS_SLIPPERY:
				slopeAccel = 2.7f;
				break;
			default:
				slopeAccel = 1.7f;
				break;
			case SURFACE_CLASS_NOT_SLIPPERY:
				slopeAccel = 0.0f;
				break;
		}

		if(floorDYaw > -0x4000 && floorDYaw < 0x4000)
		{
			forwardVel += slopeAccel * steepness * FRAME_RATE_SCALER;
		}
		else
		{
			forwardVel -= slopeAccel * steepness * FRAME_RATE_SCALER;
		}
	}

	slideYaw = faceAngle[1];

	slideVelX = forwardVel * sins(faceAngle[1]);
	slideVelZ = forwardVel * coss(faceAngle[1]);

	vel[0] = slideVelX;
	vel[1] = 0.0f;
	vel[2] = slideVelZ;

	pos[0] = pos[0];

	mario_update_moving_sand();
	mario_update_windy_ground();
}

s32 PlayerRecord::apply_landing_accel(f32 frictionFactor)
{
	s32 stopped = FALSE;

	apply_slope_accel();

	if(!mario_floor_is_slope())
	{
		forwardVel *= frictionFactor;
		if(forwardVel * forwardVel < 1.0f)
		{
			mario_set_forward_vel(0.0f);
			stopped = TRUE;
		}
	}

	return stopped;
}

void PlayerRecord::update_shell_speed()
{
	f32 maxTargetSpeed;
	f32 targetSpeed;

	if(floorHeight < waterLevel)
	{
		floorHeight	    = waterLevel;
		floor		    = &gWaterSurfacePseudoFloor;
		floor->originOffset = waterLevel; //! Negative origin offset
	}

	if(floor != NULL && floor->type == SURFACE_SLOW)
	{
		maxTargetSpeed = 48.0f;
	}
	else
	{
		maxTargetSpeed = 64.0f;
	}

	targetSpeed = intendedMag * 2.0f;
	if(targetSpeed > maxTargetSpeed)
	{
		targetSpeed = maxTargetSpeed;
	}
	if(targetSpeed < 24.0f)
	{
		targetSpeed = 24.0f;
	}

	if(forwardVel <= 0.0f)
	{
		forwardVel += 1.1f * FRAME_RATE_SCALER;
	}
	else if(forwardVel <= targetSpeed)
	{
		forwardVel += (1.1f - forwardVel / 58.0f) * FRAME_RATE_SCALER;
	}
	else if(floor->normal.y >= 0.95f)
	{
		forwardVel -= 1.0f;
	}

	//! No backward speed cap (shell hyperspeed)
	if(forwardVel > 64.0f)
	{
		forwardVel = 64.0f;
	}

	faceAngle[1] = intendedYaw - approach_s32((s16)(intendedYaw - faceAngle[1]), 0, 0x800 / FRAME_RATE_SCALER_INV, 0x800 / FRAME_RATE_SCALER_INV);

	apply_slope_accel();
}

s32 PlayerRecord::apply_slope_decel(f32 decelCoef)
{
	f32 decel;
	s32 stopped = FALSE;

	switch(mario_get_floor_class())
	{
		case SURFACE_CLASS_VERY_SLIPPERY:
			decel = decelCoef * 0.2f;
			break;
		case SURFACE_CLASS_SLIPPERY:
			decel = decelCoef * 0.7f;
			break;
		default:
			decel = decelCoef * 2.0f;
			break;
		case SURFACE_CLASS_NOT_SLIPPERY:
			decel = decelCoef * 3.0f;
			break;
	}

	if((forwardVel = approach_f32(forwardVel, 0.0f, decel * FRAME_RATE_SCALER, decel * FRAME_RATE_SCALER)) == 0.0f)
	{
		stopped = TRUE;
	}

	apply_slope_accel();
	return stopped;
}

s32 PlayerRecord::update_decelerating_speed()
{
	s32 stopped = FALSE;

	if((forwardVel = approach_f32(forwardVel, 0.0f, 1.0f * FRAME_RATE_SCALER, 1.0f * FRAME_RATE_SCALER)) == 0.0f)
	{
		stopped = TRUE;
	}

	mario_set_forward_vel(forwardVel);
	mario_update_moving_sand();
	mario_update_windy_ground();

	return stopped;
}

void PlayerRecord::update_walking_speed()
{
	f32 maxTargetSpeed;
	f32 targetSpeed;

	if(floor != NULL && floor->type == SURFACE_SLOW)
	{
		maxTargetSpeed = 24.0f * sm64::config().cheats().speed();
	}
	else
	{
		maxTargetSpeed = 32.0f * sm64::config().cheats().speed();
	}

	targetSpeed = intendedMag < maxTargetSpeed ? intendedMag : maxTargetSpeed;

	if(sinking > 10.0f)
	{
		targetSpeed *= 6.25 / sinking;
	}

	if(forwardVel <= 0.0f)
	{
		forwardVel += 1.1f * FRAME_RATE_SCALER;
	}
	else if(forwardVel <= targetSpeed)
	{
		float vel = ((1.1f * sm64::config().cheats().speed()) - forwardVel / (43.0f * sm64::config().cheats().speed())) * FRAME_RATE_SCALER;

		if (vel > 0.0f)
		{
			forwardVel += vel;
		}
	}
	else if(floor->normal.y >= 0.95f)
	{
		forwardVel -= 1.0f * FRAME_RATE_SCALER;
	}

	if(forwardVel > 48.0f * sm64::config().cheats().speed())
	{
		forwardVel = 48.0f * sm64::config().cheats().speed();
	}

	faceAngle[1] = intendedYaw - approach_s32((s16)(intendedYaw - faceAngle[1]), 0, 0x800 / FRAME_RATE_SCALER_INV, 0x800 / FRAME_RATE_SCALER_INV);
	apply_slope_accel();
}

s32 PlayerRecord::should_begin_sliding()
{
	if(input & INPUT_ABOVE_SLIDE)
	{
		s32 slideLevel	   = (area->terrainType & TERRAIN_MASK) == TERRAIN_SLIDE;
		s32 movingBackward = forwardVel <= -1.0f;

		if(slideLevel || movingBackward || mario_facing_downhill(FALSE))
		{
			return TRUE;
		}
	}

	return FALSE;
}

s32 PlayerRecord::analog_stick_held_back()
{
	s16 intendedDYaw = this->intendedYaw - this->faceAngle[1];
	return intendedDYaw < -0x471C || intendedDYaw > 0x471C;
}

s32 PlayerRecord::check_ground_dive_or_punch()
{
	UNUSED s32 unused;

	if(input & INPUT_B_PRESSED)
	{
		//! Speed kick (shoutouts to SimpleFlips)
		if(forwardVel >= 29.0f && controller->stickMag > 48.0f)
		{
			vel[1] = 20.0f;
			return ChangePlayerStatus(ACT_DIVE, 1);
		}

		return ChangePlayerStatus(ACT_MOVE_PUNCHING, 0);
	}

	return FALSE;
}

s32 PlayerRecord::begin_braking_action()
{
	mario_drop_held_object(this);

	if(actionState == 1)
	{
		faceAngle[1] = actionArg;
		return ChangePlayerStatus(ACT_STANDING_AGAINST_WALL, 0);
	}

	if(forwardVel >= 16.0f && floor->normal.y >= 0.17364818f)
	{
		return ChangePlayerStatus(ACT_BRAKING, 0);
	}

	return ChangePlayerStatus(ACT_DECELERATING, 0);
}

void PlayerRecord::func_802652F0()
{
	s32 nRunningVelocity;
	s32 shouldRun	= TRUE;
	s16 targetPitch = 0;
	f32 runningVelocity;

	runningVelocity = intendedMag > forwardVel ? intendedMag : forwardVel;

	if(runningVelocity < 4.0f)
	{
		runningVelocity = 4.0f;
	}

	if(sinking > 50.0f)
	{
		nRunningVelocity = (s32)(runningVelocity / 4.0f * 0x10000);
		set_mario_anim_with_accel(MARIO_ANIM_MOVE_IN_QUICKSAND, nRunningVelocity);
		func_80263AD4(19, 93);
		actionTimer = 0;
	}
	else
	{
		while(shouldRun)
		{
			switch(actionTimer)
			{
				case 0:
					if(runningVelocity > 8.0f)
					{
						actionTimer = 2;
					}
					else
					{
						//! (Speed Crash) If Mario's speed is more than 2^17.
						if((nRunningVelocity = (s32)(runningVelocity / 4.0f * 0x10000)) < 0x1000)
						{
							nRunningVelocity = 0x1000;
						}
						set_mario_anim_with_accel(MARIO_ANIM_START_TIPTOE, nRunningVelocity);
						func_80263AD4(7, 22);
						if(is_anim_past_frame(23))
						{
							actionTimer = 2;
						}

						shouldRun = FALSE;
					}
					break;

				case 1:
					if(runningVelocity > 8.0f)
					{
						actionTimer = 2;
					}
					else
					{
						//! (Speed Crash) If Mario's speed is more than 2^17.
						if((nRunningVelocity = (s32)(runningVelocity * 0x10000)) < 0x1000)
						{
							nRunningVelocity = 0x1000;
						}
						set_mario_anim_with_accel(MARIO_ANIM_TIPTOE, nRunningVelocity);
						func_80263AD4(14, 72);

						shouldRun = FALSE;
					}
					break;

				case 2:
					if(runningVelocity < 5.0f)
					{
						actionTimer = 1;
					}
					else if(runningVelocity > 22.0f)
					{
						actionTimer = 3;
					}
					else
					{
						//! (Speed Crash) If Mario's speed is more than 2^17.
						nRunningVelocity = (s32)(runningVelocity / 4.0f * 0x10000);
						set_mario_anim_with_accel(MARIO_ANIM_WALKING, nRunningVelocity);
						func_80263AD4(10, 49);

						shouldRun = FALSE;
					}
					break;

				case 3:
					if(runningVelocity < 18.0f)
					{
						actionTimer = 2;
					}
					else
					{
						//! (Speed Crash) If Mario's speed is more than 2^17.
						nRunningVelocity = (s32)(runningVelocity / 4.0f * 0x10000);
						set_mario_anim_with_accel(MARIO_ANIM_RUNNING, nRunningVelocity);
						func_80263AD4(9, 45);
						targetPitch = func_80263A50();

						shouldRun = FALSE;
					}
					break;
			}
		}
	}

	marioObj->oMarioWalkingPitch  = (s16)approach_s32(marioObj->oMarioWalkingPitch, targetPitch, 0x800 / FRAME_RATE_SCALER_INV, 0x800 / FRAME_RATE_SCALER_INV);
	marioObj->header.gfx.angle[0] = marioObj->oMarioWalkingPitch;
}

void PlayerRecord::func_8026570C()
{
	s32 val0C;
	s32 val08 = TRUE;
	f32 val04;

	val04 = intendedMag > forwardVel ? intendedMag : forwardVel;

	if(val04 < 2.0f)
	{
		val04 = 2.0f;
	}

	while(val08)
	{
		switch(actionTimer)
		{
			case 0:
				if(val04 > 6.0f)
				{
					actionTimer = 1;
				}
				else
				{
					//! (Speed Crash) Crashes if Mario's speed exceeds or equals 2^15.
					val0C = (s32)(val04 * 0x10000);
					set_mario_anim_with_accel(MARIO_ANIM_SLOW_WALK_WITH_LIGHT_OBJ, val0C);
					func_80263AD4(12, 62);

					val08 = FALSE;
				}
				break;

			case 1:
				if(val04 < 3.0f)
				{
					actionTimer = 0;
				}
				else if(val04 > 11.0f)
				{
					actionTimer = 2;
				}
				else
				{
					//! (Speed Crash) Crashes if Mario's speed exceeds or equals 2^15.
					val0C = (s32)(val04 * 0x10000);
					set_mario_anim_with_accel(MARIO_ANIM_WALK_WITH_LIGHT_OBJ, val0C);
					func_80263AD4(12, 62);

					val08 = FALSE;
				}
				break;

			case 2:
				if(val04 < 8.0f)
				{
					actionTimer = 1;
				}
				else
				{
					//! (Speed Crash) Crashes if Mario's speed exceeds or equals 2^16.
					val0C = (s32)(val04 / 2.0f * 0x10000);
					set_mario_anim_with_accel(MARIO_ANIM_RUN_WITH_LIGHT_OBJ, val0C);
					func_80263AD4(10, 49);

					val08 = FALSE;
				}
				break;
		}
	}
}

void PlayerRecord::func_80265980()
{
	s32 val04 = (s32)(intendedMag * 0x10000);
	set_mario_anim_with_accel(MARIO_ANIM_WALK_WITH_HEAVY_OBJ, val04);
	func_80263AD4(26, 79);
}

void PlayerRecord::func_802659E8(Vec3f startPos)
{
	s16 wallAngle;
	s16 dWallAngle;
	f32 dx		  = pos[0] - startPos[0];
	f32 dz		  = pos[2] - startPos[2];
	f32 movedDistance = sqrtf(dx * dx + dz * dz);
	//! (Speed Crash) If a wall is after moving 16384 distance, this crashes.
	s32 val04 = (s32)(movedDistance * 2.0f * 0x10000);

	if(forwardVel > 6.0f * sm64::config().cheats().speed())
	{
		mario_set_forward_vel(6.0f * sm64::config().cheats().speed());
	}

	if(wall != NULL)
	{
		wallAngle  = atan2s(wall->normal.z, wall->normal.x);
		dWallAngle = wallAngle - faceAngle[1];
	}

	if(wall == NULL || dWallAngle <= -0x71C8 || dWallAngle >= 0x71C8)
	{
		flags |= MARIO_UNKNOWN_31;
		set_mario_animation(MARIO_ANIM_PUSHING);
		func_80263AD4(6, 18);
	}
	else
	{
		if(dWallAngle < 0)
		{
			set_mario_anim_with_accel(MARIO_ANIM_SIDESTEP_RIGHT, val04);
		}
		else
		{
			set_mario_anim_with_accel(MARIO_ANIM_SIDESTEP_LEFT, val04);
		}

		if(marioObj->header.gfx.unk38.frame() < 20)
		{
			AudStartSound(SOUND_MOVING_TERRAIN_SLIDE + terrainSoundAddend, marioObj->header.gfx.cameraToObject);
			particleFlags |= PARTICLE_DUST;
		}

		actionState		      = 1;
		actionArg		      = wallAngle + 0x8000;
		marioObj->header.gfx.angle[1] = wallAngle + 0x8000;
		marioObj->header.gfx.angle[2] = find_floor_slope(0x4000);
	}
}

void PlayerRecord::func_80265C28(s16 startYaw)
{
	s16 animID = marioObj->header.gfx.unk38.animID;
	s16 dYaw;
	s16 val02;
	s16 val00;

	if(animID == MARIO_ANIM_WALKING || animID == MARIO_ANIM_RUNNING)
	{
		dYaw = faceAngle[1] - startYaw;
		//! (Speed Crash) These casts can cause a crash if (dYaw * forwardVel / 12) or
		//! (forwardVel * 170) exceed or equal 2^31.
		val02 = -(s16)(dYaw * forwardVel / 12.0f);
		val00 = (s16)(forwardVel * 170.0f);

		if(val02 > 0x1555)
		{
			val02 = 0x1555;
		}
		if(val02 < -0x1555)
		{
			val02 = -0x1555;
		}

		if(val00 > 0x1555)
		{
			val00 = 0x1555;
		}
		if(val00 < 0)
		{
			val00 = 0;
		}

		marioBodyState->torsoAngle[2] = approach_s32(marioBodyState->torsoAngle[2], val02, 0x400 / FRAME_RATE_SCALER_INV, 0x400 / FRAME_RATE_SCALER_INV);
		marioBodyState->torsoAngle[0] = approach_s32(marioBodyState->torsoAngle[0], val00, 0x400 / FRAME_RATE_SCALER_INV, 0x400 / FRAME_RATE_SCALER_INV);
	}
	else
	{
		marioBodyState->torsoAngle[2] = 0;
		marioBodyState->torsoAngle[0] = 0;
	}
}

void PlayerRecord::func_80265DBC(s16 startYaw)
{
	s16 dYaw = faceAngle[1] - startYaw;
	//! (Speed Crash) These casts can cause a crash if (dYaw * forwardVel / 12) or
	//! (forwardVel * 170) exceed or equal 2^31. Harder (if not impossible to do)
	//! while on a Koopa Shell making this less of an issue.
	s16 val04 = -(s16)(dYaw * forwardVel / 12.0f);
	s16 val02 = (s16)(forwardVel * 170.0f);

	if(val04 > 0x1800)
	{
		val04 = 0x1800;
	}
	if(val04 < -0x1800)
	{
		val04 = -0x1800;
	}

	if(val02 > 0x1000)
	{
		val02 = 0x1000;
	}
	if(val02 < 0)
	{
		val02 = 0;
	}

	marioBodyState->torsoAngle[2] = approach_s32(marioBodyState->torsoAngle[2], val04, 0x200 / FRAME_RATE_SCALER_INV, 0x200 / FRAME_RATE_SCALER_INV);
	marioBodyState->torsoAngle[0] = approach_s32(marioBodyState->torsoAngle[0], val02, 0x200 / FRAME_RATE_SCALER_INV, 0x200 / FRAME_RATE_SCALER_INV);
	marioBodyState->headAngle[2]  = -marioBodyState->torsoAngle[2];

	marioObj->header.gfx.angle[2] = marioBodyState->torsoAngle[2];
	marioObj->header.gfx.pos[1] += 45.0f;
}

s32 PlayerRecord::act_walking()
{
	Vec3f startPos;
	s16 startYaw = faceAngle[1];

	mario_drop_held_object(this);

	if(should_begin_sliding())
	{
		return ChangePlayerStatus(ACT_BEGIN_SLIDING, 0);
	}

	if(input & INPUT_FIRST_PERSON)
	{
		return begin_braking_action();
	}

	if(input & INPUT_A_PRESSED)
	{
		return set_jump_from_landing();
	}

	if(check_ground_dive_or_punch())
	{
		return TRUE;
	}

	if(input & INPUT_UNKNOWN_5)
	{
		return begin_braking_action();
	}

	if(analog_stick_held_back() && forwardVel >= 16.0f)
	{
		return ChangePlayerStatus(ACT_TURNING_AROUND, 0);
	}

	if(input & INPUT_Z_PRESSED)
	{
		return ChangePlayerStatus(ACT_CROUCH_SLIDE, 0);
	}

	actionState = 0;

	vec3f_copy(startPos, pos);
	update_walking_speed();

	switch(perform_ground_step())
	{
		case GROUND_STEP_LEFT_GROUND:
			ChangePlayerStatus(ACT_FREEFALL, 0);
			set_mario_animation(MARIO_ANIM_GENERAL_FALL);
			break;

		case GROUND_STEP_NONE:
			func_802652F0(); // sets running animation
			if(intendedMag - forwardVel > 16.0f)
			{
				particleFlags |= PARTICLE_DUST;
			}
			break;

		case GROUND_STEP_HIT_WALL:
			func_802659E8(startPos);
			actionTimer = 0;
			break;
	}

	check_ledge_climb_down();
	func_80265C28(startYaw);
	return FALSE;
}

s32 PlayerRecord::act_move_punching()
{
	if(should_begin_sliding())
	{
		return ChangePlayerStatus(ACT_BEGIN_SLIDING, 0);
	}

	if(actionState == 0 && (input & INPUT_A_DOWN))
	{
		return ChangePlayerStatus(ACT_JUMP_KICK, 0);
	}

	actionState = 1;

	mario_update_punch_sequence();

	if(forwardVel >= 0.0f)
	{
		apply_slope_decel(0.5f);
	}
	else
	{
		if((forwardVel += 8.0f * FRAME_RATE_SCALER) >= 0.0f)
		{
			forwardVel = 0.0f;
		}
		apply_slope_accel();
	}

	switch(perform_ground_step())
	{
		case GROUND_STEP_LEFT_GROUND:
			ChangePlayerStatus(ACT_FREEFALL, 0);
			break;

		case GROUND_STEP_NONE:
			particleFlags |= PARTICLE_DUST;
			break;
	}

	return FALSE;
}

s32 PlayerRecord::act_hold_walking()
{
	if(heldObj->behavior == sm64::bhv::bhvJumpingBox())
	{
		return ChangePlayerStatus(ACT_CRAZY_BOX_BOUNCE, 0);
	}

	if(marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_WALKING, 0);
	}

	if(should_begin_sliding())
	{
		return ChangePlayerStatus(ACT_HOLD_BEGIN_SLIDING, 0);
	}

	if(input & INPUT_B_PRESSED)
	{
		return ChangePlayerStatus(ACT_THROWING, 0);
	}

	if(input & INPUT_A_PRESSED)
	{
		return set_jumping_action(ACT_HOLD_JUMP, 0);
	}

	if(input & INPUT_UNKNOWN_5)
	{
		return ChangePlayerStatus(ACT_HOLD_DECELERATING, 0);
	}

	if(input & INPUT_Z_PRESSED)
	{
		return drop_and_set_mario_action(ACT_CROUCH_SLIDE, 0);
	}

	intendedMag *= 0.4f;

	update_walking_speed();

	switch(perform_ground_step())
	{
		case GROUND_STEP_LEFT_GROUND:
			ChangePlayerStatus(ACT_HOLD_FREEFALL, 0);
			break;

		case GROUND_STEP_HIT_WALL:
			if(forwardVel > 16.0f)
			{
				mario_set_forward_vel(16.0f);
			}
			break;
	}

	func_8026570C();

	if(0.4f * intendedMag - forwardVel > 10.0f)
	{
		particleFlags |= PARTICLE_DUST;
	}

	return FALSE;
}

s32 PlayerRecord::act_hold_heavy_walking()
{
	if(input & INPUT_B_PRESSED)
	{
		return ChangePlayerStatus(ACT_HEAVY_THROW, 0);
	}

	if(should_begin_sliding())
	{
		return drop_and_set_mario_action(ACT_BEGIN_SLIDING, 0);
	}

	if(input & INPUT_UNKNOWN_5)
	{
		return ChangePlayerStatus(ACT_HOLD_HEAVY_IDLE, 0);
	}

	intendedMag *= 0.1f;

	update_walking_speed();

	switch(perform_ground_step())
	{
		case GROUND_STEP_LEFT_GROUND:
			drop_and_set_mario_action(ACT_FREEFALL, 0);
			break;

		case GROUND_STEP_HIT_WALL:
			if(forwardVel > 10.0f)
			{
				mario_set_forward_vel(10.0f);
			}
			break;
	}

	func_80265980();
	return FALSE;
}

s32 PlayerRecord::act_turning_around()
{
	if(input & INPUT_ABOVE_SLIDE)
	{
		return ChangePlayerStatus(ACT_BEGIN_SLIDING, 0);
	}

	if(input & INPUT_A_PRESSED)
	{
		return set_jumping_action(ACT_SIDE_FLIP, 0);
	}

	if(input & INPUT_UNKNOWN_5)
	{
		return ChangePlayerStatus(ACT_BRAKING, 0);
	}

	if(!analog_stick_held_back())
	{
		return ChangePlayerStatus(ACT_WALKING, 0);
	}

	if(apply_slope_decel(2.0f))
	{
		return begin_walking_action(8.0f, ACT_FINISH_TURNING_AROUND, 0);
	}

	AudStartSound(SOUND_MOVING_TERRAIN_SLIDE + terrainSoundAddend, marioObj->header.gfx.cameraToObject);

	adjust_sound_for_speed();

	switch(perform_ground_step())
	{
		case GROUND_STEP_LEFT_GROUND:
			ChangePlayerStatus(ACT_FREEFALL, 0);
			break;

		case GROUND_STEP_NONE:
			particleFlags |= PARTICLE_DUST;
			break;
	}

	if(forwardVel >= 18.0f)
	{
		set_mario_animation(MARIO_ANIM_TURNING_PART1);
	}
	else
	{
		set_mario_animation(MARIO_ANIM_TURNING_PART2);
		if(is_anim_at_end())
		{
			if(forwardVel > 0.0f)
			{
				begin_walking_action(-forwardVel, ACT_WALKING, 0);
			}
			else
			{
				begin_walking_action(8.0f, ACT_WALKING, 0);
			}
		}
	}

	return FALSE;
}

s32 PlayerRecord::act_finish_turning_around()
{
	if(input & INPUT_ABOVE_SLIDE)
	{
		return ChangePlayerStatus(ACT_BEGIN_SLIDING, 0);
	}

	if(input & INPUT_A_PRESSED)
	{
		return set_jumping_action(ACT_SIDE_FLIP, 0);
	}

	update_walking_speed();
	set_mario_animation(MARIO_ANIM_TURNING_PART2);

	if(perform_ground_step() == GROUND_STEP_LEFT_GROUND)
	{
		ChangePlayerStatus(ACT_FREEFALL, 0);
	}

	if(is_anim_at_end())
	{
		ChangePlayerStatus(ACT_WALKING, 0);
	}

	marioObj->header.gfx.angle[1] += 0x8000;
	return FALSE;
}

s32 PlayerRecord::act_braking()
{
	if(!(input & INPUT_FIRST_PERSON) && (input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE)))
	{
		return check_common_action_exits();
	}

	if(apply_slope_decel(2.0f))
	{
		return ChangePlayerStatus(ACT_BRAKING_STOP, 0);
	}

	if(input & INPUT_B_PRESSED)
	{
		return ChangePlayerStatus(ACT_MOVE_PUNCHING, 0);
	}

	switch(perform_ground_step())
	{
		case GROUND_STEP_LEFT_GROUND:
			ChangePlayerStatus(ACT_FREEFALL, 0);
			break;

		case GROUND_STEP_NONE:
			particleFlags |= PARTICLE_DUST;
			break;

		case GROUND_STEP_HIT_WALL:
			slide_bonk(ACT_BACKWARD_GROUND_KB, ACT_BRAKING_STOP);
			break;
	}

	AudStartSound(SOUND_MOVING_TERRAIN_SLIDE + terrainSoundAddend, marioObj->header.gfx.cameraToObject);
	adjust_sound_for_speed();
	set_mario_animation(MARIO_ANIM_SKID_ON_GROUND);
	return FALSE;
}

s32 PlayerRecord::act_decelerating()
{
	s32 val0C;
	s16 slopeClass = mario_get_floor_class();

	if(!(input & INPUT_FIRST_PERSON))
	{
		if(should_begin_sliding())
		{
			return ChangePlayerStatus(ACT_BEGIN_SLIDING, 0);
		}

		if(input & INPUT_A_PRESSED)
		{
			return set_jump_from_landing();
		}

		if(check_ground_dive_or_punch())
		{
			return TRUE;
		}

		if(input & INPUT_NONZERO_ANALOG)
		{
			return ChangePlayerStatus(ACT_WALKING, 0);
		}

		if(input & INPUT_Z_PRESSED)
		{
			return ChangePlayerStatus(ACT_CROUCH_SLIDE, 0);
		}
	}

	if(update_decelerating_speed())
	{
		return ChangePlayerStatus(ACT_IDLE, 0);
	}

	switch(perform_ground_step())
	{
		case GROUND_STEP_LEFT_GROUND:
			ChangePlayerStatus(ACT_FREEFALL, 0);
			break;

		case GROUND_STEP_HIT_WALL:
			if(slopeClass == SURFACE_CLASS_VERY_SLIPPERY)
			{
				mario_bonk_reflection(TRUE);
			}
			else
			{
				mario_set_forward_vel(0.0f);
			}
			break;
	}

	if(slopeClass == SURFACE_CLASS_VERY_SLIPPERY)
	{
		set_mario_animation(MARIO_ANIM_IDLE_HEAD_LEFT);
		AudStartSound(SOUND_MOVING_TERRAIN_SLIDE + terrainSoundAddend, marioObj->header.gfx.cameraToObject);
		adjust_sound_for_speed();
		particleFlags |= PARTICLE_DUST;
	}
	else
	{
		// (Speed Crash) Crashes if speed exceeds 2^17.
		if((val0C = (s32)(forwardVel / 4.0f * 0x10000)) < 0x1000)
		{
			val0C = 0x1000;
		}

		set_mario_anim_with_accel(MARIO_ANIM_WALKING, val0C);
		func_80263AD4(10, 49);
	}

	return FALSE;
}

s32 PlayerRecord::act_hold_decelerating()
{
	s32 val0C;
	s16 slopeClass = mario_get_floor_class();

	if(marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_WALKING, 0);
	}

	if(should_begin_sliding())
	{
		return ChangePlayerStatus(ACT_HOLD_BEGIN_SLIDING, 0);
	}

	if(input & INPUT_B_PRESSED)
	{
		return ChangePlayerStatus(ACT_THROWING, 0);
	}

	if(input & INPUT_A_PRESSED)
	{
		return set_jumping_action(ACT_HOLD_JUMP, 0);
	}

	if(input & INPUT_Z_PRESSED)
	{
		return drop_and_set_mario_action(ACT_CROUCH_SLIDE, 0);
	}

	if(input & INPUT_NONZERO_ANALOG)
	{
		return ChangePlayerStatus(ACT_HOLD_WALKING, 0);
	}

	if(update_decelerating_speed())
	{
		return ChangePlayerStatus(ACT_HOLD_IDLE, 0);
	}

	intendedMag *= 0.4f;

	switch(perform_ground_step())
	{
		case GROUND_STEP_LEFT_GROUND:
			ChangePlayerStatus(ACT_HOLD_FREEFALL, 0);
			break;

		case GROUND_STEP_HIT_WALL:
			if(slopeClass == SURFACE_CLASS_VERY_SLIPPERY)
			{
				mario_bonk_reflection(TRUE);
			}
			else
			{
				mario_set_forward_vel(0.0f);
			}
			break;
	}

	if(slopeClass == SURFACE_CLASS_VERY_SLIPPERY)
	{
		set_mario_animation(MARIO_ANIM_IDLE_WITH_LIGHT_OBJ);
		AudStartSound(SOUND_MOVING_TERRAIN_SLIDE + terrainSoundAddend, marioObj->header.gfx.cameraToObject);
		adjust_sound_for_speed();
		particleFlags |= PARTICLE_DUST;
	}
	else
	{
		//! (Speed Crash) This crashes if Mario has more speed than 2^15 speed.
		if((val0C = (s32)(forwardVel * 0x10000)) < 0x1000)
		{
			val0C = 0x1000;
		}

		set_mario_anim_with_accel(MARIO_ANIM_WALK_WITH_LIGHT_OBJ, val0C);
		func_80263AD4(12, 62);
	}

	return FALSE;
}

s32 PlayerRecord::act_riding_shell_ground()
{
	/*06*/ s16 startYaw = faceAngle[1];

	if(input & INPUT_A_PRESSED)
	{
		return ChangePlayerStatus(ACT_RIDING_SHELL_JUMP, 0);
	}

	if(input & INPUT_Z_PRESSED)
	{
		mario_stop_riding_object(this);
		if(forwardVel < 24.0f)
		{
			mario_set_forward_vel(24.0f);
		}
		return ChangePlayerStatus(ACT_CROUCH_SLIDE, 0);
	}

	update_shell_speed();
	set_mario_animation(actionArg == 0 ? MARIO_ANIM_START_RIDING_SHELL : MARIO_ANIM_RIDING_SHELL);

	switch(perform_ground_step())
	{
		case GROUND_STEP_LEFT_GROUND:
			ChangePlayerStatus(ACT_RIDING_SHELL_FALL, 0);
			break;

		case GROUND_STEP_HIT_WALL:
			mario_stop_riding_object(this);
			AudStartSound(flags & MARIO_METAL_CAP ? SOUND_ACTION_METAL_BONK : SOUND_ACTION_BONK, marioObj->header.gfx.cameraToObject);
			particleFlags |= PARTICLE_WALL_TINY_STAR;
			ChangePlayerStatus(ACT_BACKWARD_GROUND_KB, 0);
			break;
	}

	func_80265DBC(startYaw);
	if(floor->type == SURFACE_BURNING)
	{
		AudStartSound(SOUND_MOVING_RIDING_SHELL_LAVA, marioObj->header.gfx.cameraToObject);
	}
	else
	{
		AudStartSound(SOUND_MOVING_TERRAIN_RIDING_SHELL + terrainSoundAddend, marioObj->header.gfx.cameraToObject);
	}

	adjust_sound_for_speed();
	return FALSE;
}

s32 PlayerRecord::act_crawling()
{
	s32 val04;

	if(should_begin_sliding())
	{
		return ChangePlayerStatus(ACT_BEGIN_SLIDING, 0);
	}

	if(input & INPUT_FIRST_PERSON)
	{
		return ChangePlayerStatus(ACT_UNKNOWN_024, 0);
	}

	if(input & INPUT_A_PRESSED)
	{
		return set_jumping_action(ACT_JUMP, 0);
	}

	if(check_ground_dive_or_punch())
	{
		return TRUE;
	}

	if(input & INPUT_UNKNOWN_5)
	{
		return ChangePlayerStatus(ACT_UNKNOWN_024, 0);
	}

	if(!(input & INPUT_Z_DOWN))
	{
		return ChangePlayerStatus(ACT_UNKNOWN_024, 0);
	}

	intendedMag *= 0.1f;

	update_walking_speed();

	switch(perform_ground_step())
	{
		case GROUND_STEP_LEFT_GROUND:
			ChangePlayerStatus(ACT_FREEFALL, 0);
			break;

		case GROUND_STEP_HIT_WALL:
			if(forwardVel > 10.0f)
			{
				mario_set_forward_vel(10.0f);
			}
			//! Possibly unintended missing break

		case GROUND_STEP_NONE:
			func_80263C14();
			break;
	}

	val04 = (s32)(intendedMag * 2.0f * 0x10000);
	set_mario_anim_with_accel(MARIO_ANIM_CRAWLING, val04);
	func_80263AD4(26, 79);
	return FALSE;
}

s32 PlayerRecord::act_burning_ground()
{
	if(input & INPUT_A_PRESSED)
	{
		return ChangePlayerStatus(ACT_BURNING_JUMP, 0);
	}

	marioObj->oMarioBurnTimer += 2 * FRAME_RATE_SCALER_INV;

	if(marioObj->oMarioBurnTimer > 160 * FRAME_RATE_SCALER_INV)
	{
		return ChangePlayerStatus(ACT_WALKING, 0);
	}

	if(waterLevel - floorHeight > 50.0f)
	{
		AudStartSound(SOUND_GENERAL_FLAME_OUT, marioObj->header.gfx.cameraToObject);
		return ChangePlayerStatus(ACT_WALKING, 0);
	}

	if(forwardVel < 8.0f)
	{
		forwardVel = 8.0f;
	}

	if(forwardVel > 48.0f)
	{
		forwardVel = 48.0f;
	}

	forwardVel = approach_f32(forwardVel, 32.0f, 4.0f * FRAME_RATE_SCALER, 1.0f * FRAME_RATE_SCALER);

	if(input & INPUT_NONZERO_ANALOG)
	{
		faceAngle[1] = intendedYaw - approach_s32((s16)(intendedYaw - faceAngle[1]), 0, 0x600 / FRAME_RATE_SCALER_INV, 0x600 / FRAME_RATE_SCALER_INV);
	}

	apply_slope_accel();

	if(perform_ground_step() == GROUND_STEP_LEFT_GROUND)
	{
		ChangePlayerStatus(ACT_BURNING_FALL, 0);
	}

	set_mario_anim_with_accel(MARIO_ANIM_RUNNING, (s32)(forwardVel / 2.0f * 0x10000));
	func_80263AD4(9, 45);

	particleFlags |= PARTICLE_FLAME;
	AudStartSound(SOUND_MOVING_LAVA_BURN, marioObj->header.gfx.cameraToObject);

	if(!sm64::config().cheats().invincible())
	{
		health -= 10;
	}

	if(health < 0x100)
	{
		ChangePlayerStatus(ACT_STANDING_DEATH, 0);
	}

	marioBodyState->eyeState = MARIO_EYES_DEAD;
	return FALSE;
}

void PlayerRecord::func_80267814()
{
	s16 intendedDYaw	      = this->intendedYaw - this->faceAngle[1];
	marioBodyState->torsoAngle[0] = (s32)(5461.3335f * intendedMag / 32.0f * coss(intendedDYaw));
	marioBodyState->torsoAngle[2] = (s32)(-(5461.3335f * intendedMag / 32.0f * sins(intendedDYaw)));
}

void PlayerRecord::common_slide_action(u32 endAction, u32 airAction, s32 animation)
{
	Vec3f val14;

	vec3f_copy(val14, pos);
	AudStartSound(SOUND_MOVING_TERRAIN_SLIDE + terrainSoundAddend, marioObj->header.gfx.cameraToObject);

	adjust_sound_for_speed();

	switch(perform_ground_step())
	{
		case GROUND_STEP_LEFT_GROUND:
			ChangePlayerStatus(airAction, 0);
			if(forwardVel < -50.0f || 50.0f < forwardVel)
			{
				AudStartSound(SOUND_MARIO_HOOHOO, marioObj->header.gfx.cameraToObject);
			}
			break;

		case GROUND_STEP_NONE:
			set_mario_animation(animation);
			func_80263C14();
			particleFlags |= PARTICLE_DUST;
			break;

		case GROUND_STEP_HIT_WALL:
			if(!mario_floor_is_slippery())
			{
				if(forwardVel > 16.0f)
				{
					particleFlags |= PARTICLE_WALL_TINY_STAR;
				}
				slide_bonk(ACT_GROUND_BONK, endAction);
			}
			else if(wall != NULL)
			{
				s16 wallAngle  = atan2s(wall->normal.z, wall->normal.x);
				f32 slideSpeed = sqrtf(slideVelX * slideVelX + slideVelZ * slideVelZ);

				if((slideSpeed *= 0.9) < 4.0f)
				{
					slideSpeed = 4.0f;
				}

				slideYaw = wallAngle - (s16)(slideYaw - wallAngle) + 0x8000;

				vel[0] = slideVelX = slideSpeed * sins(slideYaw);
				vel[2] = slideVelZ = slideSpeed * coss(slideYaw);
			}

			func_80263C14();
			break;
	}
}

s32 PlayerRecord::common_slide_action_with_jump(u32 stopAction, u32 jumpAction, u32 airAction, s32 animation)
{
	if(actionTimer == 5 * FRAME_RATE_SCALER_INV)
	{
		if(input & INPUT_A_PRESSED)
		{
			return set_jumping_action(jumpAction, 0);
		}
	}
	else
	{
		actionTimer++;
	}

	if(update_sliding(4.0f))
	{
		return ChangePlayerStatus(stopAction, 0);
	}

	common_slide_action(stopAction, airAction, animation);
	return FALSE;
}

s32 PlayerRecord::act_butt_slide()
{
	s32 cancel = common_slide_action_with_jump(ACT_BUTT_SLIDE_STOP, ACT_JUMP, ACT_BUTT_SLIDE_AIR, MARIO_ANIM_SLIDE);
	func_80267814();
	return cancel;
}

s32 PlayerRecord::act_hold_butt_slide()
{
	s32 cancel;

	if(marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_BUTT_SLIDE, 0);
	}

	cancel = common_slide_action_with_jump(ACT_UNKNOWN_03F, ACT_HOLD_JUMP, ACT_HOLD_BUTT_SLIDE_AIR, MARIO_ANIM_SLIDING_ON_BOTTOM_WITH_LIGHT_OBJ);
	func_80267814();
	return cancel;
}

s32 PlayerRecord::act_crouch_slide()
{
	s32 cancel;

	if(input & INPUT_ABOVE_SLIDE)
	{
		return ChangePlayerStatus(ACT_BUTT_SLIDE, 0);
	}

	if(actionTimer < 30 * FRAME_RATE_SCALER_INV)
	{
		actionTimer++;
		if(input & INPUT_A_PRESSED)
		{
			if(forwardVel > 10.0f)
			{
				return set_jumping_action(ACT_LONG_JUMP, 0);
			}
		}
	}

	if(input & INPUT_B_PRESSED)
	{
		if(forwardVel >= 10.0f)
		{
			return ChangePlayerStatus(ACT_SLIDE_KICK, 0);
		}
		else
		{
			return ChangePlayerStatus(ACT_MOVE_PUNCHING, 0x0009);
		}
	}

	if(input & INPUT_A_PRESSED)
	{
		return set_jumping_action(ACT_JUMP, 0);
	}

	if(input & INPUT_FIRST_PERSON)
	{
		return ChangePlayerStatus(ACT_BRAKING, 0);
	}

	cancel = common_slide_action_with_jump(ACT_CROUCHING, ACT_JUMP, ACT_FREEFALL, MARIO_ANIM_START_CROUCHING);
	return cancel;
}

s32 PlayerRecord::act_slide_kick_slide()
{
	if(input & INPUT_A_PRESSED)
	{
		SendMotorEvent(5, 80);
		return set_jumping_action(ACT_FORWARD_ROLLOUT, 0);
	}

	set_mario_animation(MARIO_ANIM_SLIDE_KICK);
	if(is_anim_at_end() && forwardVel < 1.0f)
	{
		return ChangePlayerStatus(ACT_SLIDE_KICK_SLIDE_STOP, 0);
	}

	update_sliding(1.0f);
	switch(perform_ground_step())
	{
		case GROUND_STEP_LEFT_GROUND:
			ChangePlayerStatus(ACT_FREEFALL, 2);
			break;

		case GROUND_STEP_HIT_WALL:
			mario_bonk_reflection(TRUE);
			particleFlags |= PARTICLE_WALL_TINY_STAR;
			ChangePlayerStatus(ACT_BACKWARD_GROUND_KB, 0);
			break;
	}

	AudStartSound(SOUND_MOVING_TERRAIN_SLIDE + terrainSoundAddend, marioObj->header.gfx.cameraToObject);
	particleFlags |= PARTICLE_DUST;
	return FALSE;
}

s32 PlayerRecord::stomach_slide_action(u32 stopAction, u32 airAction, s32 animation)
{
	if(actionTimer == 5 * FRAME_RATE_SCALER_INV)
	{
		if(!(input & INPUT_ABOVE_SLIDE) && (input & (INPUT_A_PRESSED | INPUT_B_PRESSED)))
		{
			SendMotorEvent(5, 80);
			return drop_and_set_mario_action(forwardVel >= 0.0f ? ACT_FORWARD_ROLLOUT : ACT_BACKWARD_ROLLOUT, 0);
		}
	}
	else
	{
		actionTimer++;
	}

	if(update_sliding(4.0f))
	{
		return ChangePlayerStatus(stopAction, 0);
	}

	common_slide_action(stopAction, airAction, animation);
	return FALSE;
}

s32 PlayerRecord::act_stomach_slide()
{
	s32 cancel = stomach_slide_action(ACT_STOMACH_SLIDE_STOP, ACT_FREEFALL, MARIO_ANIM_SLIDE_DIVE);
	return cancel;
}

s32 PlayerRecord::act_hold_stomach_slide()
{
	s32 cancel;

	if(marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_STOMACH_SLIDE, 0);
	}

	cancel = stomach_slide_action(ACT_DIVE_PICKING_UP, ACT_HOLD_FREEFALL, MARIO_ANIM_SLIDE_DIVE);
	return cancel;
}

s32 PlayerRecord::act_dive_slide()
{
	if(!(input & INPUT_ABOVE_SLIDE) && (input & (INPUT_A_PRESSED | INPUT_B_PRESSED)))
	{
		SendMotorEvent(5, 80);
		return ChangePlayerStatus(forwardVel > 0.0f ? ACT_FORWARD_ROLLOUT : ACT_BACKWARD_ROLLOUT, 0);
	}

	play_mario_landing_sound_once(SOUND_ACTION_TERRAIN_BODY_HIT_GROUND);

	//! If the dive slide ends on the same frame that we pick up on object,
	// mario will not be in the dive slide action for the call to
	// mario_check_object_grab, and so will end up in the regular picking action,
	// rather than the picking up after dive action.

	if(update_sliding(8.0f) && is_anim_at_end())
	{
		mario_set_forward_vel(0.0f);
		ChangePlayerStatus(ACT_STOMACH_SLIDE_STOP, 0);
	}

	if(mario_check_object_grab(this))
	{
		mario_grab_used_object(this);
		marioBodyState->grabPos = GRAB_POS_LIGHT_OBJ;
		return TRUE;
	}

	common_slide_action(ACT_STOMACH_SLIDE_STOP, ACT_FREEFALL, MARIO_ANIM_DIVE);
	return FALSE;
}

s32 PlayerRecord::common_ground_knockback_action(s32 animation, s32 arg2, s32 arg3, s32 arg4)
{
	s32 val04;

	if(arg3)
	{
		play_mario_heavy_landing_sound_once(SOUND_ACTION_TERRAIN_BODY_HIT_GROUND);
	}

	if(arg4 > 0)
	{
		play_sound_if_no_flag(SOUND_MARIO_ATTACKED, MARIO_MARIO_SOUND_PLAYED);
	}
	else
	{
		play_sound_if_no_flag(SOUND_MARIO_OOOF2, MARIO_MARIO_SOUND_PLAYED);
	}

	if(forwardVel > 32.0f)
	{
		forwardVel = 32.0f;
	}
	if(forwardVel < -32.0f)
	{
		forwardVel = -32.0f;
	}

	val04 = set_mario_animation(animation);
	if(val04 < arg2)
	{
		apply_landing_accel(0.9f);
	}
	else if(forwardVel >= 0.0f)
	{
		mario_set_forward_vel(0.1f);
	}
	else
	{
		mario_set_forward_vel(-0.1f);
	}

	if(perform_ground_step() == GROUND_STEP_LEFT_GROUND)
	{
		if(forwardVel >= 0.0f)
		{
			ChangePlayerStatus(ACT_FORWARD_AIR_KB, arg4);
		}
		else
		{
			ChangePlayerStatus(ACT_BACKWARD_AIR_KB, arg4);
		}
	}
	else if(is_anim_at_end())
	{
		if(health < 0x100)
		{
			ChangePlayerStatus(ACT_STANDING_DEATH, 0);
		}
		else
		{
			if(arg4 > 0)
			{
				invincTimer = 30 * FRAME_RATE_SCALER_INV;
			}
			ChangePlayerStatus(ACT_IDLE, 0);
		}
	}

	return val04;
}

s32 PlayerRecord::act_hard_backward_ground_kb()
{
	s32 val04 = common_ground_knockback_action(MARIO_ANIM_FALL_OVER_BACKWARDS, 0x2B, TRUE, actionArg);

	if(val04 == 0x2B && health < 0x100)
	{
		ChangePlayerStatus(ACT_DEATH_ON_BACK, 0);
	}

	if(val04 == 0x36 && oldstatus == ACT_SPECIAL_DEATH_EXIT)
	{
		AudStartSound(SOUND_MARIO_MAMA_MIA, marioObj->header.gfx.cameraToObject);
	}

	if(val04 == 0x45)
	{
		play_mario_landing_sound_once(SOUND_ACTION_TERRAIN_LANDING);
	}

	return FALSE;
}

s32 PlayerRecord::act_hard_forward_ground_kb()
{
	s32 val04 = common_ground_knockback_action(MARIO_ANIM_LAND_ON_STOMACH, 0x15, TRUE, actionArg);
	if(val04 == 0x17 && health < 0x100)
	{
		ChangePlayerStatus(ACT_DEATH_ON_STOMACH, 0);
	}

	return FALSE;
}

s32 PlayerRecord::act_backward_ground_kb()
{
	common_ground_knockback_action(MARIO_ANIM_BACKWARD_KB, 0x16, TRUE, actionArg);
	return FALSE;
}

s32 PlayerRecord::act_forward_ground_kb()
{
	common_ground_knockback_action(MARIO_ANIM_FORWARD_KB, 0x14, TRUE, actionArg);
	return FALSE;
}

s32 PlayerRecord::act_soft_backward_ground_kb()
{
	common_ground_knockback_action(MARIO_ANIM_SOFT_BACK_KB, 0x64, FALSE, actionArg);
	return FALSE;
}

s32 PlayerRecord::act_soft_forward_ground_kb()
{
	common_ground_knockback_action(MARIO_ANIM_SOFT_FRONT_KB, 0x64, FALSE, actionArg);
	return FALSE;
}

s32 PlayerRecord::act_ground_bonk()
{
	s32 val04 = common_ground_knockback_action(MARIO_ANIM_GROUND_BONK, 0x20, TRUE, actionArg);

	if(val04 == 0x20)
	{
		play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
	}

	return FALSE;
}

s32 PlayerRecord::act_death_exit_land()
{
	s32 val04;

	apply_landing_accel(0.9f);
	play_mario_heavy_landing_sound_once(SOUND_ACTION_TERRAIN_BODY_HIT_GROUND);

	val04 = set_mario_animation(MARIO_ANIM_FALL_OVER_BACKWARDS);

	if(val04 == 0x36)
	{
		AudStartSound(SOUND_MARIO_MAMA_MIA, marioObj->header.gfx.cameraToObject);
	}
	if(val04 == 0x44)
	{
		play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
	}

	if(is_anim_at_end())
	{
		ChangePlayerStatus(ACT_IDLE, 0);
	}

	return FALSE;
}

u32 PlayerRecord::common_landing_action(s16 animation, u32 airAction)
{
	u32 stepResult;

	if(input & INPUT_NONZERO_ANALOG)
	{
		apply_landing_accel(0.98f);
	}
	else if(forwardVel >= 16.0f)
	{
		apply_slope_decel(2.0f);
	}
	else
	{
		vel[1] = 0.0f;
	}

	stepResult = perform_ground_step();
	switch(stepResult)
	{
		case GROUND_STEP_LEFT_GROUND:
			ChangePlayerStatus(airAction, 0);
			break;

		case GROUND_STEP_HIT_WALL:
			set_mario_animation(MARIO_ANIM_PUSHING);
			break;
	}

	if(forwardVel > 16.0f)
	{
		particleFlags |= PARTICLE_DUST;
	}

	set_mario_animation(animation);
	play_mario_landing_sound_once(SOUND_ACTION_TERRAIN_LANDING);

	if(floor->type >= SURFACE_SHALLOW_QUICKSAND && floor->type <= SURFACE_MOVING_QUICKSAND)
	{
		sinking += ((4 - (actionTimer / FRAME_RATE_SCALER_INV)) * 3.5f - 0.5f);
	}

	return stepResult;
}

s32 PlayerRecord::common_landing_cancels(struct LandingAction* landingAction, s32 (PlayerRecord::*setAPressAction)(u32, u32))
{
	//! Everything here, incuding floor steepness, is checked before checking
	// if mario is actually on the floor. This leads to e.g. remote sliding.

	if(floor->normal.y < 0.2923717f)
	{
		return mario_push_off_steep_floor(landingAction->verySteepAction, 0);
	}

	doubleJumpTimer = landingAction->unk02 * FRAME_RATE_SCALER_INV;

	if(should_begin_sliding())
	{
		return ChangePlayerStatus(landingAction->slideAction, 0);
	}

	if(input & INPUT_FIRST_PERSON)
	{
		return ChangePlayerStatus(landingAction->endAction, 0);
	}

	if(++actionTimer >= landingAction->numFrames * FRAME_RATE_SCALER_INV)
	{
		return ChangePlayerStatus(landingAction->endAction, 0);
	}

	if(input & INPUT_A_PRESSED)
	{
		return (this->*setAPressAction)(landingAction->aPressedAction, 0);
	}

	if(input & INPUT_OFF_FLOOR)
	{
		return ChangePlayerStatus(landingAction->offFloorAction, 0);
	}

	return FALSE;
}

s32 PlayerRecord::act_jump_land()
{
	if(common_landing_cancels(&sJumpLandAction, &PlayerRecord::set_jumping_action))
	{
		return TRUE;
	}

	common_landing_action(MARIO_ANIM_LAND_FROM_SINGLE_JUMP, ACT_FREEFALL);
	return FALSE;
}

s32 PlayerRecord::act_freefall_land()
{
	if(common_landing_cancels(&sFreefallLandAction, &PlayerRecord::set_jumping_action))
	{
		return TRUE;
	}

	common_landing_action(MARIO_ANIM_GENERAL_LAND, ACT_FREEFALL);
	return FALSE;
}

s32 PlayerRecord::act_side_flip_land()
{
	if(common_landing_cancels(&sSideFlipLandAction, &PlayerRecord::set_jumping_action))
	{
		return TRUE;
	}

	if(common_landing_action(MARIO_ANIM_SLIDEFLIP_LAND, ACT_FREEFALL) != GROUND_STEP_HIT_WALL)
	{
		marioObj->header.gfx.angle[1] += 0x8000;
	}
	return FALSE;
}

s32 PlayerRecord::act_hold_jump_land()
{
	if(marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_JUMP_LAND_STOP, 0);
	}

	if(common_landing_cancels(&sHoldJumpLandAction, &PlayerRecord::set_jumping_action))
	{
		return TRUE;
	}

	common_landing_action(MARIO_ANIM_JUMP_LAND_WITH_LIGHT_OBJ, ACT_HOLD_FREEFALL);
	return FALSE;
}

s32 PlayerRecord::act_hold_freefall_land()
{
	if(marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
	{
		return drop_and_set_mario_action(ACT_FREEFALL_LAND_STOP, 0);
	}

	if(common_landing_cancels(&sHoldFreefallLandAction, &PlayerRecord::set_jumping_action))
	{
		return TRUE;
	}

	common_landing_action(MARIO_ANIM_FALL_LAND_WITH_LIGHT_OBJ, ACT_HOLD_FREEFALL);
	return FALSE;
}

s32 PlayerRecord::act_long_jump_land()
{
	if(!(input & INPUT_Z_DOWN))
	{
		input &= ~INPUT_A_PRESSED;
	}

	if(common_landing_cancels(&sLongJumpLandAction, &PlayerRecord::set_jumping_action))
	{
		return TRUE;
	}

	if(!(input & INPUT_NONZERO_ANALOG))
	{
		play_sound_if_no_flag(SOUND_MARIO_UH2_2, MARIO_MARIO_SOUND_PLAYED);
	}

	common_landing_action(!marioObj->oMarioLongJumpIsSlow ? MARIO_ANIM_CROUCH_FROM_FAST_LONGJUMP : MARIO_ANIM_CROUCH_FROM_SLOW_LONGJUMP, ACT_FREEFALL);
	return FALSE;
}

s32 PlayerRecord::act_double_jump_land()
{
	if(common_landing_cancels(&sDoubleJumpLandAction, &PlayerRecord::set_triple_jump_action))
	{
		return TRUE;
	}
	common_landing_action(MARIO_ANIM_LAND_FROM_DOUBLE_JUMP, ACT_FREEFALL);
	return FALSE;
}

s32 PlayerRecord::act_triple_jump_land()
{
	if(sm64::config().cheats().quadrupleJump())
	{
		if(common_landing_cancels(&sTripleJumpLandAction, &PlayerRecord::set_special_triple_jump_action))
		{
			return TRUE;
		}
	}
	else
	{
		input &= ~INPUT_A_PRESSED;

		if(common_landing_cancels(&sTripleJumpLandAction, &PlayerRecord::set_jumping_action))
		{
			return TRUE;
		}
	}

	if(!(input & INPUT_NONZERO_ANALOG))
	{
		play_sound_if_no_flag(SOUND_MARIO_HAHA, MARIO_MARIO_SOUND_PLAYED);
	}

	common_landing_action(MARIO_ANIM_TRIPLE_JUMP_LAND, ACT_FREEFALL);
	return FALSE;
}

s32 PlayerRecord::act_backflip_land()
{
	if(!(input & INPUT_Z_DOWN))
	{
		input &= ~INPUT_A_PRESSED;
	}

	if(common_landing_cancels(&sBackflipLandAction, &PlayerRecord::set_jumping_action))
	{
		return TRUE;
	}

	if(!(input & INPUT_NONZERO_ANALOG))
	{
		play_sound_if_no_flag(SOUND_MARIO_HAHA, MARIO_MARIO_SOUND_PLAYED);
	}

	common_landing_action(MARIO_ANIM_TRIPLE_JUMP_LAND, ACT_FREEFALL);
	return FALSE;
}

s32 PlayerRecord::quicksand_jump_land_action(s32 animation1, s32 animation2, u32 endAction, u32 airAction)
{
	if(actionTimer++ < 6 * FRAME_RATE_SCALER_INV)
	{
		sinking -= (7 - (actionTimer / FRAME_RATE_SCALER_INV)) * 0.8f;
		if(sinking < 1.0f)
		{
			sinking = 1.1f;
		}

		play_mario_jump_sound();
		set_mario_animation(animation1);
	}
	else
	{
		if(actionTimer >= 13 * FRAME_RATE_SCALER_INV)
		{
			return ChangePlayerStatus(endAction, 0);
		}

		set_mario_animation(animation2);
	}

	apply_landing_accel(0.95f);
	if(perform_ground_step() == GROUND_STEP_LEFT_GROUND)
	{
		ChangePlayerStatus(airAction, 0);
	}

	return FALSE;
}

s32 PlayerRecord::act_quicksand_jump_land()
{
	s32 cancel = quicksand_jump_land_action(MARIO_ANIM_SINGLE_JUMP, MARIO_ANIM_LAND_FROM_SINGLE_JUMP, ACT_JUMP_LAND_STOP, ACT_FREEFALL);
	return cancel;
}

s32 PlayerRecord::act_hold_quicksand_jump_land()
{
	s32 cancel = quicksand_jump_land_action(MARIO_ANIM_JUMP_WITH_LIGHT_OBJ, MARIO_ANIM_JUMP_LAND_WITH_LIGHT_OBJ, ACT_UNKNOWN_034, ACT_HOLD_FREEFALL);
	return cancel;
}

s32 PlayerRecord::check_common_moving_cancels()
{
	if(pos[1] < waterLevel - 100)
	{
		return set_water_plunge_action();
	}

	if(!(status & ACT_FLAG_INVULNERABLE) && (input & INPUT_UNKNOWN_10))
	{
		return drop_and_set_mario_action(ACT_UNKNOWN_026, 0);
	}

	if(input & INPUT_SQUISHED)
	{
		return drop_and_set_mario_action(ACT_SQUISHED, 0);
	}

	if(!(status & ACT_FLAG_INVULNERABLE))
	{
		if(health < 0x100)
		{
			return drop_and_set_mario_action(ACT_STANDING_DEATH, 0);
		}
	}

	return FALSE;
}

s32 PlayerRecord::mario_execute_moving_action()
{
	s32 cancel;

	if(check_common_moving_cancels())
	{
		return TRUE;
	}

	if(mario_update_quicksand(0.25f))
	{
		return TRUE;
	}

	/* clang-format off */
	switch(status)
	{
		case ACT_WALKING:                  cancel = act_walking();                  break;
		case ACT_HOLD_WALKING:             cancel = act_hold_walking();             break;
		case ACT_HOLD_HEAVY_WALKING:       cancel = act_hold_heavy_walking();       break;
		case ACT_TURNING_AROUND:           cancel = act_turning_around();           break;
		case ACT_FINISH_TURNING_AROUND:    cancel = act_finish_turning_around();    break;
		case ACT_BRAKING:                  cancel = act_braking();                  break;
		case ACT_RIDING_SHELL_GROUND:      cancel = act_riding_shell_ground();      break;
		case ACT_CRAWLING:                 cancel = act_crawling();                 break;
		case ACT_BURNING_GROUND:           cancel = act_burning_ground();           break;
		case ACT_DECELERATING:             cancel = act_decelerating();             break;
		case ACT_HOLD_DECELERATING:        cancel = act_hold_decelerating();        break;
		case ACT_BUTT_SLIDE:               cancel = act_butt_slide();               break;
		case ACT_STOMACH_SLIDE:            cancel = act_stomach_slide();            break;
		case ACT_HOLD_BUTT_SLIDE:          cancel = act_hold_butt_slide();          break;
		case ACT_HOLD_STOMACH_SLIDE:       cancel = act_hold_stomach_slide();       break;
		case ACT_DIVE_SLIDE:               cancel = act_dive_slide();               break;
		case ACT_MOVE_PUNCHING:            cancel = act_move_punching();            break;
		case ACT_CROUCH_SLIDE:             cancel = act_crouch_slide();             break;
		case ACT_SLIDE_KICK_SLIDE:         cancel = act_slide_kick_slide();         break;
		case ACT_HARD_BACKWARD_GROUND_KB:  cancel = act_hard_backward_ground_kb();  break;
		case ACT_HARD_FORWARD_GROUND_KB:   cancel = act_hard_forward_ground_kb();   break;
		case ACT_BACKWARD_GROUND_KB:       cancel = act_backward_ground_kb();       break;
		case ACT_FORWARD_GROUND_KB:        cancel = act_forward_ground_kb();        break;
		case ACT_SOFT_BACKWARD_GROUND_KB:  cancel = act_soft_backward_ground_kb();  break;
		case ACT_SOFT_FORWARD_GROUND_KB:   cancel = act_soft_forward_ground_kb();   break;
		case ACT_GROUND_BONK:              cancel = act_ground_bonk();              break;
		case ACT_DEATH_EXIT_LAND:          cancel = act_death_exit_land();          break;
		case ACT_JUMP_LAND:                cancel = act_jump_land();                break;
		case ACT_FREEFALL_LAND:            cancel = act_freefall_land();            break;
		case ACT_DOUBLE_JUMP_LAND:         cancel = act_double_jump_land();         break;
		case ACT_SIDE_FLIP_LAND:           cancel = act_side_flip_land();           break;
		case ACT_HOLD_JUMP_LAND:           cancel = act_hold_jump_land();           break;
		case ACT_HOLD_FREEFALL_LAND:       cancel = act_hold_freefall_land();       break;
		case ACT_TRIPLE_JUMP_LAND:         cancel = act_triple_jump_land();         break;
		case ACT_BACKFLIP_LAND:            cancel = act_backflip_land();            break;
		case ACT_QUICKSAND_JUMP_LAND:      cancel = act_quicksand_jump_land();      break;
		case ACT_HOLD_QUICKSAND_JUMP_LAND: cancel = act_hold_quicksand_jump_land(); break;
		case ACT_LONG_JUMP_LAND:           cancel = act_long_jump_land();           break;
	}
	/* clang-format on */

	if(!cancel && (input & INPUT_IN_WATER))
	{
		particleFlags |= PARTICLE_WATER_WAKE;
		particleFlags &= ~PARTICLE_DUST;
	}

	return cancel;
}
