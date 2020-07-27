#include <ultra64.h>

#include "sm64.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "mario.h"
#include "audio/external.h"
#include "display.h"
#include "interaction.h"
#include "mario_step.h"
#include "game/options.h"

static s16 sMovingSandSpeeds[] = {12, 8, 4, 0};

struct Surface gWaterSurfacePseudoFloor = {
    SURFACE_VERY_SLIPPERY, 0, 0, 0, 0, 0, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0.0f, 1.0f, 0.0f}, 0.0f, NULL,
};

/**
 * Always returns zero. This may have been intended
 * to be used for the beta trampoline. Its return value
 * is used by set_mario_y_vel_based_on_fspeed as a constant
 * addition to Mario's Y velocity. Given the closeness of
 * this function to nop_80254E50, it is probable that this
 * was intended to check whether a trampoline had made itself
 * known through nop_80254E50 and whether Mario was on it,
 * and if so return a higher value than 0.
 */
f32 get_additive_y_vel_for_jumps(void)
{
	return 0.0f;
}

/**
 * Does nothing, but takes in a PlayerRecord. This is only ever
 * called by update_mario_inputs, which is called as part of Mario's
 * update routine. Due to its proximity to nop_80254E50, an
 * incomplete trampoline function, and get_additive_y_vel_for_jumps,
 * a potentially trampoline-related function, it is plausible that
 * this could be used for checking if Mario was on the trampoline.
 * It could, for example, make him bounce.
 */
void nop_80254E3C(UNUSED struct PlayerRecord* x)
{
}

/**
 * Does nothing. This is only called by the beta trampoline.
 * Due to its proximity to get_additive_y_vel_for_jumps, another
 * currently-pointless function, it is probable that this was used
 * by the trampoline to make itself known to get_additive_y_vel_for_jumps,
 * or to set a variable with its intended additive Y vel.
 */
void nop_80254E50(void)
{
}

void transfer_bully_speed(struct BullyCollisionData* obj1, struct BullyCollisionData* obj2)
{
	f32 rx = obj2->posX - obj1->posX;
	f32 rz = obj2->posZ - obj1->posZ;

	//! Bully NaN crash
	f32 projectedV1 = (rx * obj1->velX + rz * obj1->velZ) / (rx * rx + rz * rz);
	f32 projectedV2 = (-rx * obj2->velX - rz * obj2->velZ) / (rx * rx + rz * rz);

	// Kill speed along r. Convert one object's speed along r and transfer it to
	// the other object.
	obj2->velX += obj2->conversionRatio * projectedV1 * rx - projectedV2 * -rx;
	obj2->velZ += obj2->conversionRatio * projectedV1 * rz - projectedV2 * -rz;

	obj1->velX += -projectedV1 * rx + obj1->conversionRatio * projectedV2 * -rx;
	obj1->velZ += -projectedV1 * rz + obj1->conversionRatio * projectedV2 * -rz;

	//! Bully battery
}

void init_bully_collision_data(struct BullyCollisionData* data, f32 posX, f32 posZ, f32 forwardVel, s16 yaw, f32 conversionRatio, f32 radius)
{
	if(forwardVel < 0.0f)
	{
		forwardVel *= -1.0f;
		yaw += 0x8000;
	}

	data->radius	      = radius;
	data->conversionRatio = conversionRatio;
	data->posX	      = posX;
	data->posZ	      = posZ;
	data->velX	      = forwardVel * sins(yaw);
	data->velZ	      = forwardVel * coss(yaw);
}

void PlayerRecord::mario_bonk_reflection(u32 negateSpeed)
{
	if(this->wall != NULL)
	{
		s16 wallAngle	   = atan2s(this->wall->normal.z, this->wall->normal.x);
		this->faceAngle[1] = wallAngle - (s16)(this->faceAngle[1] - wallAngle);

		AudStartSound((this->flags & MARIO_METAL_CAP) ? SOUND_ACTION_METAL_BONK : SOUND_ACTION_BONK, this->marioObj->header.gfx.cameraToObject);
	}
	else
	{
		AudStartSound(SOUND_ACTION_HIT, this->marioObj->header.gfx.cameraToObject);
	}

	if(negateSpeed)
	{
		this->mario_set_forward_vel(-this->forwardVel);
	}
	else
	{
		this->faceAngle[1] += 0x8000;
	}
}

u32 PlayerRecord::mario_update_quicksand(f32 sinkingSpeed)
{
	if(this->status & ACT_FLAG_RIDING_SHELL)
	{
		this->sinking = 0.0f;
	}
	else
	{
		if(this->sinking < 1.1f)
		{
			this->sinking = 1.1f;
		}

		switch(this->floor->type)
		{
			case SURFACE_SHALLOW_QUICKSAND:
				if((this->sinking += sinkingSpeed) >= 10.0f)
				{
					this->sinking = 10.0f;
				}
				break;

			case SURFACE_SHALLOW_MOVING_QUICKSAND:
				if((this->sinking += sinkingSpeed) >= 25.0f)
				{
					this->sinking = 25.0f;
				}
				break;

			case SURFACE_QUICKSAND:
			case SURFACE_MOVING_QUICKSAND:
				if((this->sinking += sinkingSpeed) >= 60.0f)
				{
					this->sinking = 60.0f;
				}
				break;

			case SURFACE_DEEP_QUICKSAND:
			case SURFACE_DEEP_MOVING_QUICKSAND:
				if((this->sinking += sinkingSpeed) >= 160.0f)
				{
					update_mario_sound_and_camera();
					return drop_and_set_mario_action(ACT_QUICKSAND_DEATH, 0);
				}
				break;

			case SURFACE_INSTANT_QUICKSAND:
			case SURFACE_INSTANT_MOVING_QUICKSAND:
				update_mario_sound_and_camera();
				return drop_and_set_mario_action(ACT_QUICKSAND_DEATH, 0);
				break;

			default:
				this->sinking = 0.0f;
				break;
		}
	}

	return 0;
}

u32 PlayerRecord::mario_push_off_steep_floor(u32 action, u32 actionArg)
{
	s16 floorDYaw = this->floorAngle - this->faceAngle[1];

	if(floorDYaw > -0x4000 && floorDYaw < 0x4000)
	{
		this->forwardVel   = 16.0f;
		this->faceAngle[1] = this->floorAngle;
	}
	else
	{
		this->forwardVel   = -16.0f;
		this->faceAngle[1] = this->floorAngle + 0x8000;
	}

	return ChangePlayerStatus(action, actionArg);
}

u32 PlayerRecord::mario_update_moving_sand()
{
	s32 floorType = floor->type;

	if(floorType == SURFACE_DEEP_MOVING_QUICKSAND || floorType == SURFACE_SHALLOW_MOVING_QUICKSAND || floorType == SURFACE_MOVING_QUICKSAND || floorType == SURFACE_INSTANT_MOVING_QUICKSAND)
	{
		s16 pushAngle = floor->force << 8;
		f32 pushSpeed = sMovingSandSpeeds[floor->force >> 8];

		this->vel[0] += pushSpeed * sins(pushAngle);
		this->vel[2] += pushSpeed * coss(pushAngle);

		return 1;
	}

	return 0;
}

u32 PlayerRecord::mario_update_windy_ground()
{
	if(floor->type == SURFACE_HORIZONTAL_WIND)
	{
		f32 pushSpeed;
		s16 pushAngle = floor->force << 8;

		if(status & ACT_FLAG_MOVING)
		{
			s16 pushDYaw = this->faceAngle[1] - pushAngle;

			pushSpeed = this->forwardVel > 0.0f ? -this->forwardVel * 0.5f : -8.0f;

			if(pushDYaw > -0x4000 && pushDYaw < 0x4000)
			{
				pushSpeed *= -1.0f;
			}

			pushSpeed *= coss(pushDYaw);
		}
		else
		{
			pushSpeed = 3.2f + ((frameCounter / FRAME_RATE_SCALER_INV) % 4);
		}

		vel[0] += pushSpeed * sins(pushAngle);
		vel[2] += pushSpeed * coss(pushAngle);

		return 1;
	}

	return 0;
}

void PlayerRecord::stop_and_set_height_to_floor()
{
	mario_set_forward_vel(0.0f);
	this->vel[1] = 0.0f;

	//! This is responsible for some downwarps.
	this->pos[1] = this->floorHeight;

	vec3f_copy(marioObj->header.gfx.pos, this->pos);
	vec3s_set(marioObj->header.gfx.angle, 0, this->faceAngle[1], 0);
}

s32 PlayerRecord::stationary_ground_step()
{
	u32 takeStep;
	u32 stepResult = GROUND_STEP_NONE;

	mario_set_forward_vel(0.0f);

	takeStep = mario_update_moving_sand();
	takeStep |= mario_update_windy_ground();
	if(takeStep)
	{
		stepResult = perform_ground_step();
	}
	else
	{
		//! This is responsible for several stationary downwarps.
		this->pos[1] = this->floorHeight;

		vec3f_copy(marioObj->header.gfx.pos, this->pos);
		vec3s_set(marioObj->header.gfx.angle, 0, this->faceAngle[1], 0);
	}

	return stepResult;
}

s32 PlayerRecord::perform_ground_quarter_step(Vec3f nextPos)
{
	UNUSED struct Surface* lowerWall;
	struct Surface* upperWall;
	struct Surface* ceil;
	struct Surface* floor;
	f32 ceilHeight;
	f32 floorHeight;
	f32 waterLevel;

	lowerWall = resolve_and_return_wall_collisions(nextPos, 30.0f, 24.0f);
	upperWall = resolve_and_return_wall_collisions(nextPos, 60.0f, 50.0f);

	floorHeight = mcBGGroundCheck(nextPos[0], nextPos[1], nextPos[2], &floor);
	ceilHeight  = vec3f_find_ceil(nextPos, floorHeight, &ceil);

	waterLevel = find_water_level(nextPos[0], nextPos[2]);

	this->wall = upperWall;

	if(floor == NULL)
	{
		return GROUND_STEP_HIT_WALL_STOP_QSTEPS;
	}

	if((this->status & ACT_FLAG_RIDING_SHELL) && floorHeight < waterLevel)
	{
		floorHeight	    = waterLevel;
		floor		    = &gWaterSurfacePseudoFloor;
		floor->originOffset = floorHeight; //! Wrong origin offset (no effect)
	}

	if(nextPos[1] > floorHeight + 100.0f)
	{
		if(nextPos[1] + 160.0f >= ceilHeight)
		{
			return GROUND_STEP_HIT_WALL_STOP_QSTEPS;
		}

		vec3f_copy(this->pos, nextPos);
		this->floor	  = floor;
		this->floorHeight = floorHeight;
		return GROUND_STEP_LEFT_GROUND;
	}

	if(floorHeight + 160.0f >= ceilHeight)
	{
		return GROUND_STEP_HIT_WALL_STOP_QSTEPS;
	}

	vec3f_set(this->pos, nextPos[0], floorHeight, nextPos[2]);
	this->floor	  = floor;
	this->floorHeight = floorHeight;

	if(upperWall != NULL)
	{
		s16 wallDYaw = atan2s(upperWall->normal.z, upperWall->normal.x) - this->faceAngle[1];

		if(wallDYaw >= 0x2AAA && wallDYaw <= 0x5555)
		{
			return GROUND_STEP_NONE;
		}
		if(wallDYaw <= -0x2AAA && wallDYaw >= -0x5555)
		{
			return GROUND_STEP_NONE;
		}

		return GROUND_STEP_HIT_WALL_CONTINUE_QSTEPS;
	}

	return GROUND_STEP_NONE;
}

s32 PlayerRecord::perform_ground_step()
{
	s32 i;
	u32 stepResult;
	Vec3f intendedPos;

	for(i = 0; i < 4; i++)
	{
		intendedPos[0] = this->pos[0] + this->floor->normal.y * (this->vel[0] / (4.0f / FRAME_RATE_SCALER));
		intendedPos[2] = this->pos[2] + this->floor->normal.y * (this->vel[2] / (4.0f / FRAME_RATE_SCALER));
		intendedPos[1] = this->pos[1];

		stepResult = perform_ground_quarter_step(intendedPos);
		if(stepResult == GROUND_STEP_LEFT_GROUND || stepResult == GROUND_STEP_HIT_WALL_STOP_QSTEPS)
		{
			break;
		}
	}

	this->terrainSoundAddend = mario_get_terrain_sound_addend();
	vec3f_copy(this->marioObj->header.gfx.pos, this->pos);
	vec3s_set(this->marioObj->header.gfx.angle, 0, this->faceAngle[1], 0);

	if(stepResult == GROUND_STEP_HIT_WALL_CONTINUE_QSTEPS)
	{
		stepResult = GROUND_STEP_HIT_WALL;
	}
	return stepResult;
}

u32 PlayerRecord::check_ledge_grab(struct Surface* wall, Vec3f intendedPos, Vec3f nextPos)
{
	struct Surface* ledgeFloor;
	Vec3f ledgePos;
	f32 displacementX;
	f32 displacementZ;

	if(this->vel[1] > 0)
	{
		return 0;
	}

	displacementX = nextPos[0] - intendedPos[0];
	displacementZ = nextPos[2] - intendedPos[2];

	// Only ledge grab if the wall displaced mario in the opposite direction of
	// his velocity.
	if(displacementX * this->vel[0] + displacementZ * this->vel[2] > 0.0f)
	{
		return 0;
	}

	//! Since the search for floors starts at y + 160, we will sometimes grab
	// a higher ledge than expected (glitchy ledge grab)
	ledgePos[0] = nextPos[0] - wall->normal.x * 60.0f;
	ledgePos[2] = nextPos[2] - wall->normal.z * 60.0f;
	ledgePos[1] = mcBGGroundCheck(ledgePos[0], nextPos[1] + 160.0f, ledgePos[2], &ledgeFloor);

	if(ledgePos[1] - nextPos[1] <= 100.0f)
	{
		return 0;
	}

	vec3f_copy(this->pos, ledgePos);
	this->floor	  = ledgeFloor;
	this->floorHeight = ledgePos[1];

	this->floorAngle = atan2s(ledgeFloor->normal.z, ledgeFloor->normal.x);

	this->faceAngle[0] = 0;
	this->faceAngle[1] = atan2s(wall->normal.z, wall->normal.x) + 0x8000;
	return 1;
}

s32 PlayerRecord::perform_air_quarter_step(Vec3f intendedPos, u32 stepArg)
{
	s16 wallDYaw;
	Vec3f nextPos;
	struct Surface* upperWall;
	struct Surface* lowerWall;
	struct Surface* ceil;
	struct Surface* floor;
	f32 ceilHeight;
	f32 floorHeight;
	f32 waterLevel;

	vec3f_copy(nextPos, intendedPos);

	upperWall = resolve_and_return_wall_collisions(nextPos, 170.0f, 50.0f); // TODO gross hack, changed from 150 to 160 to ensure ledge grabs would happen on tiny big island
	lowerWall = resolve_and_return_wall_collisions(nextPos, 30.0f, 50.0f);

	floorHeight = mcBGGroundCheck(nextPos[0], nextPos[1], nextPos[2], &floor);
	ceilHeight  = vec3f_find_ceil(nextPos, floorHeight, &ceil);

	waterLevel = find_water_level(nextPos[0], nextPos[2]);

	this->wall = NULL;

	//! The water pseudo floor is not referenced when your intended qstep is
	// out of bounds, so it won't detect you as landing.

	if(floor == NULL)
	{
		if(nextPos[1] <= this->floorHeight)
		{
			this->pos[1] = this->floorHeight;
			return AIR_STEP_LANDED;
		}

		this->pos[1] = nextPos[1];
		return AIR_STEP_HIT_WALL;
	}

	if((this->status & ACT_FLAG_RIDING_SHELL) && floorHeight < waterLevel)
	{
		floorHeight	    = waterLevel;
		floor		    = &gWaterSurfacePseudoFloor;
		floor->originOffset = floorHeight; //! Incorrect origin offset (no effect)
	}

	//! This check uses f32, but findFloor uses short (overflow jumps)
	if(nextPos[1] <= floorHeight)
	{
		if(ceilHeight - floorHeight > 160.0f)
		{
			this->pos[0]	  = nextPos[0];
			this->pos[2]	  = nextPos[2];
			this->floor	  = floor;
			this->floorHeight = floorHeight;
		}

		//! When ceilHeight - floorHeight <= 160, the step result says that
		// mario landed, but his movement is cancelled and his referenced floor
		// isn't updated (pedro spots)
		this->pos[1] = floorHeight;
		return AIR_STEP_LANDED;
	}

	if(nextPos[1] + 160.0f > ceilHeight)
	{
		if(this->vel[1] >= 0.0f)
		{
			this->vel[1] = 0.0f;

			//! Uses referenced ceiling instead of ceil (ceiling hang upwarp)
			if((stepArg & AIR_STEP_CHECK_HANG) && this->ceil != NULL && this->ceil->type == SURFACE_HANGABLE)
			{
				return AIR_STEP_GRABBED_CEILING;
			}

			return AIR_STEP_NONE;
		}

		//! Potential subframe downwarp->upwarp?
		if(nextPos[1] <= this->floorHeight)
		{
			this->pos[1] = this->floorHeight;
			return AIR_STEP_LANDED;
		}

		this->pos[1] = nextPos[1];
		return AIR_STEP_HIT_WALL;
	}

	//! When the wall is not completely vertical or there is a slight wall
	// misalignment, you can activate these conditions in unexpected situations
	if((stepArg & AIR_STEP_CHECK_LEDGE_GRAB) && (upperWall == NULL) && lowerWall != NULL)
	{
		if(check_ledge_grab(lowerWall, intendedPos, nextPos))
		{
			return AIR_STEP_GRABBED_LEDGE;
		}

		vec3f_copy(this->pos, nextPos);
		this->floor	  = floor;
		this->floorHeight = floorHeight;
		return AIR_STEP_NONE;
	}

	vec3f_copy(this->pos, nextPos);
	this->floor	  = floor;
	this->floorHeight = floorHeight;

	if(upperWall != NULL || lowerWall != NULL)
	{
		this->wall = upperWall != NULL ? upperWall : lowerWall;
		wallDYaw   = atan2s(this->wall->normal.z, this->wall->normal.x) - this->faceAngle[1];

		if(this->wall->type == SURFACE_BURNING)
		{
			return AIR_STEP_HIT_LAVA_WALL;
		}

		if(wallDYaw < -0x6000 || wallDYaw > 0x6000)
		{
			this->flags |= MARIO_UNKNOWN_30;
			return AIR_STEP_HIT_WALL;
		}
	}

	return AIR_STEP_NONE;
}

void PlayerRecord::apply_twirl_gravity()
{
	f32 terminalVelocity;
	f32 heaviness = 1.0f;

	if(this->angleVel[1] > 1024)
	{
		heaviness = 1024.0f / this->angleVel[1];
	}

	terminalVelocity = -75.0f * heaviness;

	this->vel[1] -= 4.0f * heaviness;
	if(this->vel[1] < terminalVelocity)
	{
		this->vel[1] = terminalVelocity;
	}
}

u32 PlayerRecord::should_strengthen_gravity_for_jump_ascent()
{
	if(!(this->flags & MARIO_UNKNOWN_08))
	{
		return FALSE;
	}

	if(this->status & (ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE))
	{
		return FALSE;
	}

	if(!(this->input & INPUT_A_DOWN) && this->vel[1] > 20.0f)
	{
		return (this->status & ACT_FLAG_CONTROL_JUMP_HEIGHT) != 0;
	}

	return FALSE;
}

void PlayerRecord::apply_gravity()
{
	double moonGravityMultiplier = sm64::config().cheats().moonJump() ? 0.5f : 1.0f;

	if(this->status == ACT_TWIRLING && this->vel[1] < 0.0f)
	{
		apply_twirl_gravity();
	}
	else if(this->status == ACT_SHOT_FROM_CANNON)
	{
		this->vel[1] -= 1.0 * moonGravityMultiplier * FRAME_RATE_SCALER;
		if(this->vel[1] < -75.0f)
		{
			this->vel[1] = -75.0f;
		}
	}
	else if(this->status == ACT_LONG_JUMP || this->status == ACT_SLIDE_KICK || this->status == ACT_BBH_ENTER_SPIN)
	{
		this->vel[1] -= 2.0 * moonGravityMultiplier * FRAME_RATE_SCALER;
		if(this->vel[1] < -75.0f)
		{
			this->vel[1] = -75.0f;
		}
	}
	else if(this->status == ACT_LAVA_BOOST || this->status == ACT_FALL_AFTER_STAR_GRAB)
	{
		this->vel[1] -= 3.2 * moonGravityMultiplier * FRAME_RATE_SCALER;
		if(this->vel[1] < -65.0f)
		{
			this->vel[1] = -65.0f;
		}
	}
	else if(this->status == ACT_GETTING_BLOWN)
	{
		this->vel[1] -= this->gravity * moonGravityMultiplier * FRAME_RATE_SCALER;
		if(this->vel[1] < -75.0f)
		{
			this->vel[1] = -75.0f;
		}
	}
	else if(should_strengthen_gravity_for_jump_ascent())
	{
		this->vel[1] /= 4.0 * FRAME_RATE_SCALER;
	}
	else if(this->status & ACT_FLAG_METAL_WATER)
	{
		this->vel[1] -= 1.6 * moonGravityMultiplier * FRAME_RATE_SCALER;
		if(this->vel[1] < -16.0f)
		{
			this->vel[1] = -16.0f;
		}
	}
	else if((this->flags & MARIO_WING_CAP) && this->vel[1] < 0.0f && (this->input & INPUT_A_DOWN))
	{
		this->marioBodyState->unk07 = 1;

		this->vel[1] -= 2.0 * moonGravityMultiplier * FRAME_RATE_SCALER;
		if(this->vel[1] < -37.5f)
		{
			if((this->vel[1] += 4.0f * moonGravityMultiplier * FRAME_RATE_SCALER) > -37.5f)
			{
				this->vel[1] = -37.5f;
			}
		}
	}
	else
	{
		this->vel[1] -= 4.0 * moonGravityMultiplier * FRAME_RATE_SCALER;

		if(this->vel[1] < -75.0f)
		{
			this->vel[1] = -75.0f;
		}
	}
}

void PlayerRecord::apply_vertical_wind()
{
	f32 maxVelY;
	f32 offsetY;

	if(this->status != ACT_GROUND_POUND)
	{
		offsetY = this->pos[1] - -1500.0f;

		if(this->floor->type == SURFACE_VERTICAL_WIND && -3000.0f < offsetY && offsetY < 2000.0f)
		{
			if(offsetY >= 0.0f)
			{
				maxVelY = 10000.0f / (offsetY + 200.0f);
			}
			else
			{
				maxVelY = 50.0f;
			}

			if(this->vel[1] < maxVelY)
			{
				if((this->vel[1] += maxVelY / 8.0f) > maxVelY)
				{
					this->vel[1] = maxVelY;
				}
			}
		}
	}
}

s32 PlayerRecord::perform_air_step(u32 stepArg)
{
	Vec3f intendedPos;
	s32 i;
	s32 quarterStepResult;
	s32 stepResult = AIR_STEP_NONE;

	this->wall = NULL;

	for(i = 0; i < 4; i++)
	{
		intendedPos[0] = this->pos[0] + this->vel[0] / (4.0f / FRAME_RATE_SCALER);
		intendedPos[1] = this->pos[1] + this->vel[1] / (4.0f / FRAME_RATE_SCALER);
		intendedPos[2] = this->pos[2] + this->vel[2] / (4.0f / FRAME_RATE_SCALER);

		quarterStepResult = perform_air_quarter_step(intendedPos, stepArg);

		//! On one qf, hit OOB/ceil/wall to store the 2 return value, and continue
		// getting 0s until your last qf. Graze a wall on your last qf, and it will
		// return the stored 2 with a sharply angled reference wall. (some gwks)

		if(quarterStepResult != AIR_STEP_NONE)
		{
			stepResult = quarterStepResult;
		}

		if(quarterStepResult == AIR_STEP_LANDED || quarterStepResult == AIR_STEP_GRABBED_LEDGE || quarterStepResult == AIR_STEP_GRABBED_CEILING || quarterStepResult == AIR_STEP_HIT_LAVA_WALL)
		{
			break;
		}
	}

	if(this->vel[1] >= 0.0f)
	{
		this->fallpos = this->pos[1];
	}

	this->terrainSoundAddend = mario_get_terrain_sound_addend();

	if(this->status != ACT_FLYING)
	{
		apply_gravity();
	}
	apply_vertical_wind();

	vec3f_copy(this->marioObj->header.gfx.pos, this->pos);
	vec3s_set(this->marioObj->header.gfx.angle, 0, this->faceAngle[1], 0);

	return stepResult;
}

// They had these functions the whole time and never used them? Lol

void PlayerRecord::set_vel_from_pitch_and_yaw()
{
	this->vel[0] = this->forwardVel * coss(this->faceAngle[0]) * sins(this->faceAngle[1]);
	this->vel[1] = this->forwardVel * sins(this->faceAngle[0]);
	this->vel[2] = this->forwardVel * coss(this->faceAngle[0]) * coss(this->faceAngle[1]);
}

void PlayerRecord::set_vel_from_yaw()
{
	this->vel[0] = this->slideVelX = this->forwardVel * sins(this->faceAngle[1]);
	this->vel[1]		       = 0.0f;
	this->vel[2] = this->slideVelZ = this->forwardVel * coss(this->faceAngle[1]);
}
