#include <ultra64.h>

#include "sm64.h"
#include "behavior_data.h"
#include "mario_actions_automatic.h"
#include "audio/external.h"
#include "area.h"
#include "mario.h"
#include "mario_step.h"
#include "engine/math_util.h"
#include "memory.h"
#include "engine/graph_node.h"
#include "save_file.h"
#include "engine/surface_collision.h"
#include "interaction.h"
#include "camera.h"
#include "level_table.h"
#include "game/motor.h"

#define POLE_NONE 0
#define POLE_TOUCHED_FLOOR 1
#define POLE_FELL_OFF 2

#define HANG_NONE 0
#define HANG_HIT_CEIL_OR_OOB 1
#define HANG_LEFT_CEIL 2

void PlayerRecord::add_tree_leaf_particles()
{
	f32 leafHeight;

	if(this->usedObj->behavior == segmented_to_virtual(sm64::bhv::bhvTree()))
	{
		// make leaf effect spawn higher on the Shifting Sand Land palm tree
		if(activeStageNo == LEVEL_SSL)
		{
			leafHeight = 250.0f;
		}
		else
		{
			leafHeight = 100.0f;
		}
		if(this->pos[1] - this->floorHeight > leafHeight)
		{
			this->particleFlags |= PARTICLE_LEAVES;
		}
	}
}

void PlayerRecord::play_climbing_sounds(s32 b)
{
	s32 isOnTree = (this->usedObj->behavior == segmented_to_virtual(sm64::bhv::bhvTree()));

	if(b == 1)
	{
		if(this->checkAnimeFrame(1))
		{
			AudStartSound(isOnTree ? SOUND_ACTION_CLIMB_UP_TREE : SOUND_ACTION_CLIMB_UP_POLE, this->marioObj->header.gfx.cameraToObject);
		}
	}
	else
	{
		AudStartSound(isOnTree ? SOUND_MOVING_SLIDE_DOWN_TREE : SOUND_MOVING_SLIDE_DOWN_POLE, this->marioObj->header.gfx.cameraToObject);
	}
}

s32 PlayerRecord::set_pole_position(f32 offsetY)
{
	UNUSED s32 unused1;
	UNUSED s32 unused2;
	UNUSED s32 unused3;
	struct Surface* floor;
	struct Surface* ceil;
	f32 floorHeight;
	f32 ceilHeight;
	s32 collided;
	s32 result		= POLE_NONE;
	f32 poleTop		= this->usedObj->hitboxHeight - 100.0f;
	struct Object* marioObj = this->marioObj;

	if(marioObj->oMarioPolePos > poleTop)
	{
		marioObj->oMarioPolePos = poleTop;
	}

	this->pos[0] = this->usedObj->oPosX;
	this->pos[2] = this->usedObj->oPosZ;
	this->pos[1] = this->usedObj->oPosY + marioObj->oMarioPolePos + offsetY;

	collided = WallCheck(&this->pos[0], &this->pos[1], &this->pos[2], 60.0f, 50.0f);
	collided |= WallCheck(&this->pos[0], &this->pos[1], &this->pos[2], 30.0f, 24.0f);

	ceilHeight = PL_CheckRoofPlane(this->pos, this->pos[1], &ceil);
	if(this->pos[1] > ceilHeight - 160.0f)
	{
		this->pos[1]		= ceilHeight - 160.0f;
		marioObj->oMarioPolePos = this->pos[1] - this->usedObj->oPosY;
	}

	floorHeight = mcBGGroundCheck(this->pos[0], this->pos[1], this->pos[2], &floor);
	if(this->pos[1] < floorHeight)
	{
		this->pos[1] = floorHeight;
		this->ChangePlayerStatus(ACT_IDLE, 0);
		result = POLE_TOUCHED_FLOOR;
	}
	else if(marioObj->oMarioPolePos < -this->usedObj->hitboxDownOffset)
	{
		this->pos[1] = this->usedObj->oPosY - this->usedObj->hitboxDownOffset;
		this->ChangePlayerStatus(ACT_FREEFALL, 0);
		result = POLE_FELL_OFF;
	}
	else if(collided)
	{
		if(this->pos[1] > floorHeight + 20.0f)
		{
			this->forwardVel = -2.0f;
			this->ChangePlayerStatus(ACT_SOFT_BONK, 0);
			result = POLE_FELL_OFF;
		}
		else
		{
			this->ChangePlayerStatus(ACT_IDLE, 0);
			result = POLE_TOUCHED_FLOOR;
		}
	}

	CopyFVector(this->marioObj->header.gfx.pos, this->pos);
	SetSVector(this->marioObj->header.gfx.angle, this->usedObj->oMoveAnglePitch, this->faceAngle[1], this->usedObj->oMoveAngleRoll);

	return result;
}

s32 PlayerRecord::act_holding_pole()
{
	if((this->input & INPUT_Z_PRESSED) || this->health < 0x100)
	{
		add_tree_leaf_particles();
		this->forwardVel = -2.0f;
		return this->ChangePlayerStatus(ACT_SOFT_BONK, 0);
	}

	if(this->input & INPUT_A_PRESSED)
	{
		add_tree_leaf_particles();
		this->faceAngle[1] += 0x8000;
		return this->ChangePlayerStatus(ACT_WALL_KICK_AIR, 0);
	}

	if(this->controller->stickY > 16.0f)
	{
		f32 poleTop	   = this->usedObj->hitboxHeight - 100.0f;
		void* poleBehavior = virtual_to_segmented(0x13, this->usedObj->behavior);

		if(marioObj->oMarioPolePos < poleTop - 0.4f)
		{
			return this->ChangePlayerStatus(ACT_CLIMBING_POLE, 0);
		}

		if(poleBehavior != sm64::bhv::bhvGiantPole() && this->controller->stickY > 50.0f)
		{
			return this->ChangePlayerStatus(ACT_TOP_OF_POLE_TRANSITION, 0);
		}
	}

	if(this->controller->stickY < -16.0f)
	{
		marioObj->oMarioPoleYawVel -= this->controller->stickY * (2 * FRAME_RATE_SCALER);

		if(marioObj->oMarioPoleYawVel > 0x1000)
		{
			marioObj->oMarioPoleYawVel = 0x1000;
		}

		this->faceAngle[1] += marioObj->oMarioPoleYawVel * FRAME_RATE_SCALER;
		marioObj->oMarioPolePos -= (marioObj->oMarioPoleYawVel * FRAME_RATE_SCALER) / 0x100;

		if(this->usedObj->behavior == segmented_to_virtual(sm64::bhv::bhvTree()))
		{
			//! The Shifting Sand Land palm tree check is done climbing up in
			// add_tree_leaf_particles, but not here, when climbing down.
			if(this->pos[1] - this->floorHeight > 100.0f)
			{
				this->particleFlags |= PARTICLE_LEAVES;
			}
		}
		play_climbing_sounds(2);
		Na_SetObjSpeed(1, (marioObj->oMarioPoleYawVel * FRAME_RATE_SCALER) / 0x100 * 2);
	}
	else
	{
		marioObj->oMarioPoleYawVel = 0;
		this->faceAngle[1] -= this->controller->stickX * 16.0f * FRAME_RATE_SCALER;
	}

	if(set_pole_position(0.0f) == POLE_NONE)
	{
		this->setAnimation(MARIO_ANIM_IDLE_ON_POLE);
	}

	return FALSE;
}

s32 PlayerRecord::act_climbing_pole()
{
	s32 sp24;
	struct Object* marioObj = this->marioObj;
	s16 cameraAngle		= this->area->camera->yaw;

	if(this->health < 0x100)
	{
		add_tree_leaf_particles();
		this->forwardVel = -2.0f;
		return this->ChangePlayerStatus(ACT_SOFT_BONK, 0);
	}

	if(this->input & INPUT_A_PRESSED)
	{
		add_tree_leaf_particles();
		this->faceAngle[1] += 0x8000;
		return this->ChangePlayerStatus(ACT_WALL_KICK_AIR, 0);
	}

	if(this->controller->stickY < 8.0f)
	{
		return this->ChangePlayerStatus(ACT_HOLDING_POLE, 0);
	}

	marioObj->oMarioPolePos += this->controller->stickY / 8.0f * FRAME_RATE_SCALER;
	marioObj->oMarioPoleYawVel = 0;
	this->faceAngle[1]	   = cameraAngle - approach_s32((s16)(cameraAngle - this->faceAngle[1]), 0, 0x400 / FRAME_RATE_SCALER_INV, 0x400 / FRAME_RATE_SCALER_INV);

	if(set_pole_position(0.0f) == POLE_NONE)
	{
		sp24 = this->controller->stickY / 4.0f * 0x10000;
		this->setAnimation(MARIO_ANIM_CLIMB_UP_POLE, sp24);
		add_tree_leaf_particles();
		play_climbing_sounds(1);
	}

	return FALSE;
}

s32 PlayerRecord::act_grab_pole_slow()
{
	this->startSoundEffect(SOUND_MARIO_WHOA, MARIO_MARIO_SOUND_PLAYED);

	if(set_pole_position(0.0f) == POLE_NONE)
	{
		this->setAnimation(MARIO_ANIM_GRAB_POLE_SHORT);
		if(this->isLast1AnimeFrame())
		{
			this->ChangePlayerStatus(ACT_HOLDING_POLE, 0);
		}
		add_tree_leaf_particles();
	}

	return FALSE;
}

s32 PlayerRecord::act_grab_pole_fast()
{
	struct Object* marioObj = this->marioObj;

	this->startSoundEffect(SOUND_MARIO_WHOA, MARIO_MARIO_SOUND_PLAYED);
	this->faceAngle[1] += marioObj->oMarioPoleYawVel * FRAME_RATE_SCALER;
	marioObj->oMarioPoleYawVel = marioObj->oMarioPoleYawVel * 8 / 10;

	if(set_pole_position(0.0f) == POLE_NONE)
	{
		if(marioObj->oMarioPoleYawVel > 0x800)
		{
			this->setAnimation(MARIO_ANIM_GRAB_POLE_SWING_PART1);
		}
		else
		{
			this->setAnimation(MARIO_ANIM_GRAB_POLE_SWING_PART2);
			if(this->isLast1AnimeFrame() != 0)
			{
				marioObj->oMarioPoleYawVel = 0;
				this->ChangePlayerStatus(ACT_HOLDING_POLE, 0);
			}
		}
		add_tree_leaf_particles();
	}

	return FALSE;
}

s32 PlayerRecord::act_top_of_pole_transition()
{
	struct Object* marioObj = this->marioObj;

	marioObj->oMarioPoleYawVel = 0;
	if(this->actionArg == 0)
	{
		this->setAnimation(MARIO_ANIM_START_HANDSTAND);
		if(this->isLast1AnimeFrame())
		{
			return this->ChangePlayerStatus(ACT_TOP_OF_POLE, 0);
		}
	}
	else
	{
		this->setAnimation(MARIO_ANIM_RETURN_FROM_HANDSTAND);
		if(this->marioObj->header.gfx.unk38.frame() == 0)
		{
			return this->ChangePlayerStatus(ACT_HOLDING_POLE, 0);
		}
	}

	set_pole_position(this->return_mario_anim_y_translation());
	return FALSE;
}

s32 PlayerRecord::act_top_of_pole()
{
	UNUSED struct Object* marioObj = this->marioObj;

	if(this->input & INPUT_A_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_TOP_OF_POLE_JUMP, 0);
	}
	if(this->controller->stickY < -16.0f)
	{
		return this->ChangePlayerStatus(ACT_TOP_OF_POLE_TRANSITION, 1);
	}

	this->faceAngle[1] -= this->controller->stickX * 16.0f * FRAME_RATE_SCALER;

	this->setAnimation(MARIO_ANIM_HANDSTAND_IDLE);
	set_pole_position(this->return_mario_anim_y_translation());
	return FALSE;
}

s32 PlayerRecord::perform_hanging_step(Vec3f nextPos)
{
	UNUSED s32 unused;
	struct Surface* ceil;
	struct Surface* floor;
	f32 ceilHeight;
	f32 floorHeight;
	f32 ceilOffset;

	this->wall  = resolve_and_return_wall_collisions(nextPos, 50.0f, 50.0f);
	floorHeight = mcBGGroundCheck(nextPos[0], nextPos[1], nextPos[2], &floor);
	ceilHeight  = PL_CheckRoofPlane(nextPos, floorHeight, &ceil);

	if(floor == NULL)
	{
		return HANG_HIT_CEIL_OR_OOB;
	}
	if(ceil == NULL)
	{
		return HANG_LEFT_CEIL;
	}
	if(ceilHeight - floorHeight <= 160.0f)
	{
		return HANG_HIT_CEIL_OR_OOB;
	}
	if(ceil->type != SURFACE_HANGABLE)
	{
		return HANG_LEFT_CEIL;
	}

	ceilOffset = ceilHeight - (nextPos[1] + 160.0f);
	if(ceilOffset < -30.0f)
	{
		return HANG_HIT_CEIL_OR_OOB;
	}
	if(ceilOffset > 30.0f)
	{
		return HANG_LEFT_CEIL;
	}

	nextPos[1] = this->ceilHeight - 160.0f;
	CopyFVector(this->pos, nextPos);

	this->floor	  = floor;
	this->floorHeight = floorHeight;
	this->ceil	  = ceil;
	this->ceilHeight  = ceilHeight;

	return HANG_NONE;
}

s32 PlayerRecord::update_hang_moving()
{
	s32 stepResult;
	Vec3f nextPos;
	f32 maxSpeed = 4.0f;

	this->forwardVel += 1.0f * FRAME_RATE_SCALER;

	if(this->forwardVel > maxSpeed)
	{
		this->forwardVel = maxSpeed;
	}

	this->faceAngle[1] = this->intendedYaw - approach_s32((s16)(this->intendedYaw - this->faceAngle[1]), 0, 0x800 / FRAME_RATE_SCALER_INV, 0x800 / FRAME_RATE_SCALER_INV);

	this->slideYaw	= this->faceAngle[1];
	this->slideVelX = this->forwardVel * sins(this->faceAngle[1]);
	this->slideVelZ = this->forwardVel * coss(this->faceAngle[1]);

	this->vel[0] = this->slideVelX;
	this->vel[1] = 0.0f;
	this->vel[2] = this->slideVelZ;

	nextPos[0] = this->pos[0] - this->ceil->normal.y * this->vel[0];
	nextPos[2] = this->pos[2] - this->ceil->normal.y * this->vel[2];
	nextPos[1] = this->pos[1];

	stepResult = perform_hanging_step(nextPos);

	CopyFVector(this->marioObj->header.gfx.pos, this->pos);
	SetSVector(this->marioObj->header.gfx.angle, 0, this->faceAngle[1], 0);
	return stepResult;
}

void PlayerRecord::update_hang_stationary()
{
	this->forwardVel = 0.0f;
	this->slideVelX	 = 0.0f;
	this->slideVelZ	 = 0.0f;

	this->pos[1] = this->ceilHeight - 160.0f;
	CopyFVector(this->vel, gVec3fZero);
	CopyFVector(this->marioObj->header.gfx.pos, this->pos);
}

s32 PlayerRecord::act_start_hanging()
{
	if(this->actionTimer++ == 0)
	{
		SendMotorEvent(5, 80);
	}

	if((this->input & INPUT_NONZERO_ANALOG) && this->actionTimer >= 31)
	{
		return this->ChangePlayerStatus(ACT_HANGING, 0);
	}

	if(!(this->input & INPUT_A_DOWN))
	{
		return this->ChangePlayerStatus(ACT_FREEFALL, 0);
	}

	if(this->input & INPUT_Z_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_GROUND_POUND, 0);
	}

	//! Crash if mario's referenced ceiling is NULL (same for other hanging actions)
	if(this->ceil->type != SURFACE_HANGABLE)
	{
		return this->ChangePlayerStatus(ACT_FREEFALL, 0);
	}

	this->setAnimation(MARIO_ANIM_HANG_ON_CEILING);
	this->startSoundEffect(SOUND_ACTION_HANGING_STEP, MARIO_ACTION_SOUND_PLAYED);
	update_hang_stationary();

	if(this->isLast1AnimeFrame())
	{
		this->ChangePlayerStatus(ACT_HANGING, 0);
	}

	return FALSE;
}

s32 PlayerRecord::act_hanging()
{
	if(this->input & INPUT_NONZERO_ANALOG)
	{
		return this->ChangePlayerStatus(ACT_HANG_MOVING, this->actionArg);
	}

	if(!(this->input & INPUT_A_DOWN))
	{
		return this->ChangePlayerStatus(ACT_FREEFALL, 0);
	}

	if(this->input & INPUT_Z_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_GROUND_POUND, 0);
	}

	if(this->ceil->type != SURFACE_HANGABLE)
	{
		return this->ChangePlayerStatus(ACT_FREEFALL, 0);
	}

	if(this->actionArg & 1)
	{
		this->setAnimation(MARIO_ANIM_HANDSTAND_LEFT);
	}
	else
	{
		this->setAnimation(MARIO_ANIM_HANDSTAND_RIGHT);
	}

	update_hang_stationary();

	return FALSE;
}

s32 PlayerRecord::act_hang_moving()
{
	if(!(this->input & INPUT_A_DOWN))
	{
		return this->ChangePlayerStatus(ACT_FREEFALL, 0);
	}

	if(this->input & INPUT_Z_PRESSED)
	{
		return this->ChangePlayerStatus(ACT_GROUND_POUND, 0);
	}

	if(this->ceil->type != SURFACE_HANGABLE)
	{
		return this->ChangePlayerStatus(ACT_FREEFALL, 0);
	}

	if(this->actionArg & 1)
	{
		this->setAnimation(MARIO_ANIM_MOVE_ON_WIRE_NET_RIGHT);
	}
	else
	{
		this->setAnimation(MARIO_ANIM_MOVE_ON_WIRE_NET_LEFT);
	}

	if(this->marioObj->header.gfx.unk38.frame() == 12)
	{
		AudStartSound(SOUND_ACTION_HANGING_STEP, this->marioObj->header.gfx.cameraToObject);
		SendMotorEvent(5, 80);
	}

	if(this->isLast2AnimeFrame())
	{
		this->actionArg ^= 1;
		if(this->input & INPUT_UNKNOWN_5)
		{
			return this->ChangePlayerStatus(ACT_HANGING, this->actionArg);
		}
	}

	if(update_hang_moving() == HANG_LEFT_CEIL)
	{
		this->ChangePlayerStatus(ACT_FREEFALL, 0);
	}

	return FALSE;
}

s32 PlayerRecord::let_go_of_ledge()
{
	f32 floorHeight;
	struct Surface* floor;

	this->vel[1]	 = 0.0f;
	this->forwardVel = -8.0f;
	this->pos[0] -= 60.0f * FRAME_RATE_SCALER * sins(this->faceAngle[1]);
	this->pos[2] -= 60.0f * FRAME_RATE_SCALER * coss(this->faceAngle[1]);

	floorHeight = mcBGGroundCheck(this->pos[0], this->pos[1], this->pos[2], &floor);
	if(floorHeight < this->pos[1] - 100.0f)
	{
		this->pos[1] -= 100.0f * FRAME_RATE_SCALER;
	}
	else
	{
		this->pos[1] = floorHeight;
	}

	return this->ChangePlayerStatus(ACT_SOFT_BONK, 0);
}

void PlayerRecord::func_8025F0DC()
{
	this->setAnimation(MARIO_ANIM_IDLE_HEAD_LEFT);
	this->pos[0] += 14.0f * FRAME_RATE_SCALER * sins(this->faceAngle[1]);
	this->pos[2] += 14.0f * FRAME_RATE_SCALER * coss(this->faceAngle[1]);
	CopyFVector(this->marioObj->header.gfx.pos, this->pos);
}

void PlayerRecord::func_8025F188()
{
	f32 sp4;

	if(this->actionTimer < 14)
	{
		sp4 = this->actionTimer;
	}
	else
	{
		sp4 = 14.0f;
	}
	this->statusForCamera->pos[0] = this->pos[0] + sp4 * sins(this->faceAngle[1]);
	this->statusForCamera->pos[2] = this->pos[2] + sp4 * coss(this->faceAngle[1]);
	this->statusForCamera->pos[1] = this->pos[1];
	this->actionTimer++;
	this->flags |= MARIO_UNKNOWN_25;
}

void PlayerRecord::update_ledge_climb(s32 animation, u32 endAction)
{
	stopProcess();

	this->setAnimation(animation);
	if(this->isLast1AnimeFrame())
	{
		this->ChangePlayerStatus(endAction, 0);
		if(endAction == ACT_IDLE)
		{
			func_8025F0DC();
		}
	}
}

s32 PlayerRecord::act_ledge_grab()
{
	f32 heightAboveFloor;
	s16 intendedDYaw     = this->intendedYaw - this->faceAngle[1];
	s32 hasSpaceForMario = (this->ceilHeight - this->floorHeight >= 160.0f);

	if(this->actionTimer < 10)
	{
		this->actionTimer++;
	}

	if(this->floor->normal.y < 0.9063078f)
	{
		return let_go_of_ledge();
	}

	if(this->input & (INPUT_Z_PRESSED | INPUT_OFF_FLOOR))
	{
		return let_go_of_ledge();
	}

	if((this->input & INPUT_A_PRESSED) && hasSpaceForMario)
	{
		return this->ChangePlayerStatus(ACT_LEDGE_CLIMB_FAST, 0);
	}

	if(this->input & INPUT_UNKNOWN_10)
	{
		if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_UNK1)
		{
			this->hurtCounter += (this->flags & MARIO_CAP_ON_HEAD) ? 12 : 18;
		}
		return let_go_of_ledge();
	}
	if(this->actionTimer == 10 && (this->input & INPUT_NONZERO_ANALOG))
	{
		if(intendedDYaw >= -0x4000 && intendedDYaw <= 0x4000)
		{
			if(hasSpaceForMario)
			{
				return this->ChangePlayerStatus(ACT_LEDGE_CLIMB_SLOW_1, 0);
			}
		}
		else
		{
			return let_go_of_ledge();
		}
	}

	heightAboveFloor = this->pos[1] - this->checkPlayerAround(-0x8000, 30.0f);
	if(hasSpaceForMario && heightAboveFloor < 100.0f)
	{
		return this->ChangePlayerStatus(ACT_LEDGE_CLIMB_FAST, 0);
	}

	if(this->actionArg == 0)
	{
		this->startSoundEffect(SOUND_MARIO_WHOA, MARIO_MARIO_SOUND_PLAYED);
	}

	stopProcess();
	this->setAnimation(MARIO_ANIM_IDLE_ON_LEDGE);

	return FALSE;
}

s32 PlayerRecord::act_ledge_climb_slow()
{
	if(this->input & INPUT_OFF_FLOOR)
	{
		return let_go_of_ledge();
	}

	if(this->actionTimer >= 28 * FRAME_RATE_SCALER_INV && (this->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE)))
	{
		func_8025F0DC();
		return this->checkAllMotions();
	}

	if(this->actionTimer == 10 * FRAME_RATE_SCALER_INV)
	{
		this->startSoundEffect(SOUND_MARIO_EEUH, MARIO_MARIO_SOUND_PLAYED);
	}

	update_ledge_climb(MARIO_ANIM_SLOW_LEDGE_GRAB, ACT_IDLE);

	func_8025F188();
	if(this->marioObj->header.gfx.unk38.frame() == 17)
	{
		this->status = ACT_LEDGE_CLIMB_SLOW_2;
	}

	return FALSE;
}

s32 PlayerRecord::act_ledge_climb_down()
{
	if(this->input & INPUT_OFF_FLOOR)
	{
		return let_go_of_ledge();
	}

	this->startSoundEffect(SOUND_MARIO_WHOA, MARIO_MARIO_SOUND_PLAYED);

	update_ledge_climb(MARIO_ANIM_CLIMB_DOWN_LEDGE, ACT_LEDGE_GRAB);
	this->actionArg = 1;

	return FALSE;
}

s32 PlayerRecord::act_ledge_climb_fast()
{
	if(this->input & INPUT_OFF_FLOOR)
	{
		return let_go_of_ledge();
	}

	this->startSoundEffect(SOUND_MARIO_UH2, MARIO_MARIO_SOUND_PLAYED);

	update_ledge_climb(MARIO_ANIM_FAST_LEDGE_GRAB, ACT_IDLE);

	if(this->marioObj->header.gfx.unk38.frame() == 8)
	{
		this->startLandingEffect(SOUND_ACTION_TERRAIN_LANDING);
	}
	func_8025F188();

	return FALSE;
}

s32 PlayerRecord::act_grabbed()
{
	if(this->marioObj->oInteractStatus & INT_STATUS_MARIO_UNK2)
	{
		s32 thrown = (this->marioObj->oInteractStatus & INT_STATUS_MARIO_UNK6) == 0;

		this->faceAngle[1] = this->usedObj->oMoveAngleYaw;
		CopyFVector(this->pos, this->marioObj->header.gfx.pos);
		SendMotorEvent(5, 60);

		return this->ChangePlayerStatus((this->forwardVel >= 0.0f) ? ACT_THROWN_FORWARD : ACT_THROWN_BACKWARD, thrown);
	}

	this->setAnimation(MARIO_ANIM_BEING_GRABBED);
	return FALSE;
}

s32 PlayerRecord::act_in_cannon()
{
	struct Object* marioObj = this->marioObj;
	s16 startFacePitch	= this->faceAngle[0];
	s16 startFaceYaw	= this->faceAngle[1];

	switch(this->actionState)
	{
		case 0:
			this->marioObj->header.gfx.node.flags &= ~0x0001;
			this->usedObj->oInteractStatus = INT_STATUS_INTERACTED;

			this->statusForCamera->cameraEvent = CAM_EVENT_CANNON;
			this->statusForCamera->usedObj	   = this->usedObj;

			vec3f_set(this->vel, 0.0f, 0.0f, 0.0f);

			this->pos[0] = this->usedObj->oPosX;
			this->pos[1] = this->usedObj->oPosY + 350.0f;
			this->pos[2] = this->usedObj->oPosZ;

			this->forwardVel = 0.0f;

			this->actionState = 1;
			break;

		case 1:
			if(this->usedObj->oAction == 1)
			{
				this->faceAngle[0] = this->usedObj->oMoveAnglePitch;
				this->faceAngle[1] = this->usedObj->oMoveAngleYaw;

				marioObj->oMarioCannonObjectYaw = this->usedObj->oMoveAngleYaw;
				marioObj->oMarioCannonInputYaw	= 0;

				this->actionState = 2;
			}
			break;

		case 2:
			this->faceAngle[0] -= (s16)(this->controller->stickY * 10.0f * FRAME_RATE_SCALER);
			marioObj->oMarioCannonInputYaw -= (s16)(this->controller->stickX * 10.0f * FRAME_RATE_SCALER);

			if(this->faceAngle[0] > 0x38E3)
			{
				this->faceAngle[0] = 0x38E3;
			}
			if(this->faceAngle[0] < 0)
			{
				this->faceAngle[0] = 0;
			}

			if(marioObj->oMarioCannonInputYaw > 0x4000)
			{
				marioObj->oMarioCannonInputYaw = 0x4000;
			}
			if(marioObj->oMarioCannonInputYaw < -0x4000)
			{
				marioObj->oMarioCannonInputYaw = -0x4000;
			}

			this->faceAngle[1] = marioObj->oMarioCannonObjectYaw + marioObj->oMarioCannonInputYaw;
			if(this->input & INPUT_A_PRESSED)
			{
				this->forwardVel = 100.0f * coss(this->faceAngle[0]);

				this->vel[1] = 100.0f * sins(this->faceAngle[0]);

				this->pos[0] += 120.0f * FRAME_RATE_SCALER * coss(this->faceAngle[0]) * sins(this->faceAngle[1]);
				this->pos[1] += 120.0f * FRAME_RATE_SCALER * sins(this->faceAngle[0]);
				this->pos[2] += 120.0f * FRAME_RATE_SCALER * coss(this->faceAngle[0]) * coss(this->faceAngle[1]);

				AudStartSound(SOUND_ACTION_FLYING_FAST, this->marioObj->header.gfx.cameraToObject);
				AudStartSound(SOUND_OBJ_POUNDING_CANNON, this->marioObj->header.gfx.cameraToObject);

				this->marioObj->header.gfx.node.flags |= 0x0001;

				this->ChangePlayerStatus(ACT_SHOT_FROM_CANNON, 0);
				SendMotorEvent(60, 70);
				this->usedObj->oAction = 2;
				return FALSE;
			}
			else
			{
				if(this->faceAngle[0] != startFacePitch || this->faceAngle[1] != startFaceYaw)
				{
					AudStartSound(SOUND_MOVING_AIM_CANNON, this->marioObj->header.gfx.cameraToObject);
				}
			}
	}

	CopyFVector(this->marioObj->header.gfx.pos, this->pos);
	SetSVector(this->marioObj->header.gfx.angle, 0, this->faceAngle[1], 0);
	this->setAnimation(MARIO_ANIM_DIVE);

	return FALSE;
}

s32 PlayerRecord::act_tornado_twirling()
{
	struct Surface* floor;
	Vec3f nextPos;
	f32 sinAngleVel;
	f32 cosAngleVel;
	f32 floorHeight;
	struct Object* marioObj = this->marioObj;
	struct Object* usedObj	= this->usedObj;
	s16 prevTwirlYaw	= this->twirlYaw;

	f32 dx = (this->pos[0] - usedObj->oPosX) * 0.95f;
	f32 dz = (this->pos[2] - usedObj->oPosZ) * 0.95f;

	if(this->vel[1] < 60.0f)
	{
		this->vel[1] += 1.0f * FRAME_RATE_SCALER;
	}

	if((marioObj->oMarioTornadoPosY += this->vel[1] * FRAME_RATE_SCALER) < 0.0f)
	{
		marioObj->oMarioTornadoPosY = 0.0f;
	}
	if(marioObj->oMarioTornadoPosY > usedObj->hitboxHeight)
	{
		if(this->vel[1] < 20.0f)
		{
			this->vel[1] = 20.0f;
		}
		return this->ChangePlayerStatus(ACT_TWIRLING, 1);
	}

	if(this->angleVel[1] < 0x3000)
	{
		this->angleVel[1] += 0x100 / FRAME_RATE_SCALER_INV;
	}

	if(marioObj->oMarioTornadoYawVel < 0x1000)
	{
		marioObj->oMarioTornadoYawVel += 0x100 / FRAME_RATE_SCALER_INV;
	}

	this->twirlYaw += this->angleVel[1] * FRAME_RATE_SCALER;

	sinAngleVel = sins(marioObj->oMarioTornadoYawVel);
	cosAngleVel = coss(marioObj->oMarioTornadoYawVel);

	nextPos[0] = usedObj->oPosX + dx * cosAngleVel + dz * sinAngleVel;
	nextPos[2] = usedObj->oPosZ - dx * sinAngleVel + dz * cosAngleVel;
	nextPos[1] = usedObj->oPosY + marioObj->oMarioTornadoPosY;

	WallCheck(&nextPos[0], &nextPos[1], &nextPos[2], 60.0f, 50.0f);

	floorHeight = mcBGGroundCheck(nextPos[0], nextPos[1], nextPos[2], &floor);
	if(floor != NULL)
	{
		this->floor	  = floor;
		this->floorHeight = floorHeight;
		CopyFVector(this->pos, nextPos);
	}
	else
	{
		if(nextPos[1] >= this->floorHeight)
		{
			this->pos[1] = nextPos[1];
		}
		else
		{
			this->pos[1] = this->floorHeight;
		}
	}

	this->actionTimer++;

	this->setAnimation((this->actionArg == 0) ? MARIO_ANIM_START_TWIRL : MARIO_ANIM_TWIRL);

	if(this->isLast2AnimeFrame())
	{
		this->actionArg = 1;
	}

	// Play sound on angle overflow
	if(prevTwirlYaw > this->twirlYaw)
	{
		AudStartSound(SOUND_ACTION_TWIRL, this->marioObj->header.gfx.cameraToObject);
	}

	CopyFVector(this->marioObj->header.gfx.pos, this->pos);
	SetSVector(this->marioObj->header.gfx.angle, 0, this->faceAngle[1] + this->twirlYaw, 0);

	return FALSE;
}

s32 PlayerRecord::check_common_automatic_cancels()
{
	if(this->pos[1] < this->waterLevel - 100)
	{
		return this->changeWaterMode();
	}

	return FALSE;
}

s32 PlayerRecord::playerSpecMain()
{
	s32 cancel;

	if(check_common_automatic_cancels())
	{
		return TRUE;
	}

	this->sinking = 0.0f;

	/* clang-format off */
	switch(this->status)
	{
		case ACT_HOLDING_POLE:           cancel = act_holding_pole();           break;
		case ACT_GRAB_POLE_SLOW:         cancel = act_grab_pole_slow();         break;
		case ACT_GRAB_POLE_FAST:         cancel = act_grab_pole_fast();         break;
		case ACT_CLIMBING_POLE:          cancel = act_climbing_pole();          break;
		case ACT_TOP_OF_POLE_TRANSITION: cancel = act_top_of_pole_transition(); break;
		case ACT_TOP_OF_POLE:            cancel = act_top_of_pole();            break;
		case ACT_START_HANGING:          cancel = act_start_hanging();          break;
		case ACT_HANGING:                cancel = act_hanging();                break;
		case ACT_HANG_MOVING:            cancel = act_hang_moving();            break;
		case ACT_LEDGE_GRAB:             cancel = act_ledge_grab();             break;
		case ACT_LEDGE_CLIMB_SLOW_1:     cancel = act_ledge_climb_slow();       break;
		case ACT_LEDGE_CLIMB_SLOW_2:     cancel = act_ledge_climb_slow();       break;
		case ACT_LEDGE_CLIMB_DOWN:       cancel = act_ledge_climb_down();       break;
		case ACT_LEDGE_CLIMB_FAST:       cancel = act_ledge_climb_fast();       break;
		case ACT_GRABBED:                cancel = act_grabbed();                break;
		case ACT_IN_CANNON:              cancel = act_in_cannon();              break;
		case ACT_TORNADO_TWIRLING:       cancel = act_tornado_twirling();       break;
	}
	/* clang-format on */

	return cancel;
}
