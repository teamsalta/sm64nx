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
#include "player/players.h"
#include "game/motor.h"

u32 unused80339F10;
s8 filler80339F1C[20];

/**
 * These are the scaling values for the x and z axis for Mario
 * when he is close to unsquishing.
 */
u8 sSquishScaleOverTime[16] = {0x46, 0x32, 0x32, 0x3C, 0x46, 0x50, 0x50, 0x3C, 0x28, 0x14, 0x14, 0x1E, 0x32, 0x3C, 0x3C, 0x28};

/**
 * Is a binary representation of the frames to flicker Mario's cap when the timer
 * is running out.
 *
 * Equals [1000]^5 . [100]^8 . [10]^9 . [1] in binary, which is
 * 100010001000100010001001001001001001001001001010101010101010101.
 */
u64 sCapFlickerFrames = 0x4444449249255555;

// clang-format off
s8 sTerrainSounds[7][6] = {
	// default,              hard,                 slippery,
	// very slippery,        noisy default,        noisy slippery
	{ SOUND_TERRAIN_DEFAULT, SOUND_TERRAIN_STONE,  SOUND_TERRAIN_GRASS,
	  SOUND_TERRAIN_GRASS,   SOUND_TERRAIN_GRASS,  SOUND_TERRAIN_DEFAULT }, // TERRAIN_GRASS
	{ SOUND_TERRAIN_STONE,   SOUND_TERRAIN_STONE,  SOUND_TERRAIN_STONE,
	  SOUND_TERRAIN_STONE,   SOUND_TERRAIN_GRASS,  SOUND_TERRAIN_GRASS }, // TERRAIN_STONE
	{ SOUND_TERRAIN_SNOW,    SOUND_TERRAIN_ICE,    SOUND_TERRAIN_SNOW,
	  SOUND_TERRAIN_ICE,     SOUND_TERRAIN_STONE,  SOUND_TERRAIN_STONE }, // TERRAIN_SNOW
	{ SOUND_TERRAIN_SAND,    SOUND_TERRAIN_STONE,  SOUND_TERRAIN_SAND,
	  SOUND_TERRAIN_SAND,    SOUND_TERRAIN_STONE,  SOUND_TERRAIN_STONE }, // TERRAIN_SAND
	{ SOUND_TERRAIN_SPOOKY,  SOUND_TERRAIN_SPOOKY, SOUND_TERRAIN_SPOOKY,
	  SOUND_TERRAIN_SPOOKY,  SOUND_TERRAIN_STONE,  SOUND_TERRAIN_STONE }, // TERRAIN_SPOOKY
	{ SOUND_TERRAIN_DEFAULT, SOUND_TERRAIN_STONE,  SOUND_TERRAIN_GRASS,
	  SOUND_TERRAIN_ICE,     SOUND_TERRAIN_STONE,  SOUND_TERRAIN_ICE }, // TERRAIN_WATER
	{ SOUND_TERRAIN_STONE,   SOUND_TERRAIN_STONE,  SOUND_TERRAIN_STONE,
	  SOUND_TERRAIN_STONE,   SOUND_TERRAIN_ICE,    SOUND_TERRAIN_ICE }, // TERRAIN_SLIDE
};
// clang-format on

/**************************************************
 *                    ANIMATIONS                  *
 **************************************************/

/**
 * Checks if Mario's animation has reached its end point.
 */
s32 PlayerRecord::is_anim_at_end()
{
	struct Object* o = this->marioObj;
	auto& a		 = o->header.gfx.unk38;
	return !a.isPartialFrame() && (a.frame() + 1) == a.curAnim->lastFrame;
}

/**
 * Checks if Mario's animation has surpassed 2 frames before its end point.
 */
s32 PlayerRecord::is_anim_past_end()
{
	struct Object* o = this->marioObj;

	return o->header.gfx.unk38.frame() >= (o->header.gfx.unk38.curAnim->lastFrame - 2);
}

/**
 * Sets Mario's animation without any acceleration, running at its default rate.
 */
s16 PlayerRecord::set_mario_animation(s32 targetAnimID)
{
	return set_mario_anim_with_accel(targetAnimID, 0x10000);
}

/**
 * Sets Mario's animation where the animation is sped up or
 * slowed down via acceleration.
 */
s16 PlayerRecord::set_mario_anim_with_accel(s32 targetAnimID, s32 accel)
{
	struct Object* o	     = this->marioObj;
	struct Animation* targetAnim = this->animation->targetAnim;

	if(ReadPartialData(this->animation, targetAnimID))
	{
		targetAnim->values = (const s16*)VIRTUAL_TO_PHYSICAL((u8*)targetAnim + (uintptr_t)targetAnim->values);
		targetAnim->index  = (const u16*)VIRTUAL_TO_PHYSICAL((u8*)targetAnim + (uintptr_t)targetAnim->index);
	}

	if(o->header.gfx.unk38.animID != targetAnimID)
	{
		o->header.gfx.unk38.animID     = targetAnimID;
		o->header.gfx.unk38.curAnim    = targetAnim;
		o->header.gfx.unk38.animYTrans = this->unkB0;

		if(targetAnim->flags & ANIM_FLAG_PAUSE)
		{
			o->header.gfx.unk38.animFrameAccelAssist = (targetAnim->startFrame << 0x10);
		}
		else
		{
			if(targetAnim->flags & ANIM_FLAG_FORWARD)
			{
				o->header.gfx.unk38.animFrameAccelAssist = (targetAnim->startFrame << 0x10) + accel;
			}
			else
			{
				o->header.gfx.unk38.animFrameAccelAssist = (targetAnim->startFrame << 0x10) - accel;
			}

			/*if(o->header.gfx.unk38.animFrameAccelAssist < 0)
			{
				o->header.gfx.unk38.animFrameAccelAssist = 0;
			}*/
		}

		o->header.gfx.unk38.setFrameRaw(o->header.gfx.unk38.frame());
	}

	o->header.gfx.unk38.animAccel = accel;

	return o->header.gfx.unk38.frame();
}

void PlayerRecord::anim_reset()
{
	GraphNodeObject_sub* animInfo  = &this->marioObj->header.gfx.unk38;
	Animation* curAnim	       = animInfo->curAnim;
	animInfo->animFrameAccelAssist = (curAnim->firstFrame) << 0x10;
}

/**
 * Sets the animation to a specific "next" frame from the frame given.
 */
void PlayerRecord::set_anim_to_frame(s16 animFrame)
{
	struct GraphNodeObject_sub* animInfo = &this->marioObj->header.gfx.unk38;
	struct Animation* curAnim	     = animInfo->curAnim;

	if(animInfo->animAccel)
	{
		if(curAnim->flags & ANIM_FLAG_FORWARD)
		{
			animInfo->animFrameAccelAssist = (animFrame << 0x10) + animInfo->animAccel;
		}
		else
		{
			animInfo->animFrameAccelAssist = (animFrame << 0x10) - animInfo->animAccel;
		}
	}
	else
	{
		if(curAnim->flags & ANIM_FLAG_FORWARD)
		{
			animInfo->setFrameRaw(animFrame + 1);
		}
		else
		{
			animInfo->setFrameRaw(animFrame - 1);
		}
	}
}

s32 PlayerRecord::is_anim_past_frame(s16 animFrame)
{
	s32 isPastFrame;
	s32 acceleratedFrame		     = animFrame << 0x10;
	struct GraphNodeObject_sub* animInfo = &this->marioObj->header.gfx.unk38;
	struct Animation* curAnim	     = animInfo->curAnim;

	if(animInfo->animAccel)
	{
		if(curAnim->flags & ANIM_FLAG_FORWARD)
		{
			isPastFrame = (animInfo->animFrameAccelAssist > acceleratedFrame) && (acceleratedFrame >= (animInfo->animFrameAccelAssist - animInfo->animAccel));
		}
		else
		{
			isPastFrame = (animInfo->animFrameAccelAssist < acceleratedFrame) && (acceleratedFrame <= (animInfo->animFrameAccelAssist + animInfo->animAccel));
		}
	}
	else
	{
		if(curAnim->flags & ANIM_FLAG_FORWARD)
		{
			isPastFrame = (animInfo->frame() == (animFrame + 1));
		}
		else
		{
			isPastFrame = ((animInfo->frame() + 1) == animFrame);
		}
	}

	return isPastFrame;
}

/**
 * Updates Mario's position from his animation's translation.
 */
void PlayerRecord::update_mario_pos_for_anim()
{
	Vec3s translation;
	s16 flags;

	flags = find_mario_anim_flags_and_translation(this->marioObj, this->faceAngle[1], translation);

	if(flags & (ANIM_FLAG_HOR_TRANS | ANIM_FLAG_6))
	{
		this->pos[0] += (f32)translation[0];
		this->pos[2] += (f32)translation[2];
	}

	if(flags & (ANIM_FLAG_VERT_TRANS | ANIM_FLAG_6))
	{
		this->pos[1] += (f32)translation[1];
	}
}

/**
 * Finds the vertical translation from Mario's animation.
 */
s16 PlayerRecord::return_mario_anim_y_translation()
{
	Vec3s translation;
	find_mario_anim_flags_and_translation(this->marioObj, 0, translation);

	return translation[1];
}

/**************************************************
 *                      AUDIO                     *
 **************************************************/

/**
 * Plays a sound if if Mario doesn't have the flag being checked.
 */
void PlayerRecord::play_sound_if_no_flag(u32 soundBits, u32 flags)
{
	if((this->flags & flags) == 0)
	{
		AudStartSound(soundBits, this->marioObj->header.gfx.cameraToObject);
		this->flags |= flags;
	}
}

/**
 * Plays a jump sound if one has not been played since the last action change.
 */
void PlayerRecord::play_mario_jump_sound()
{
	if(!(this->flags & MARIO_MARIO_SOUND_PLAYED))
	{
		if(this->status == ACT_TRIPLE_JUMP)
		{
			AudStartSound(SOUND_MARIO_YAHOO_WAHA_YIPPEE + ((gAudioRandom % 5) << 16), this->marioObj->header.gfx.cameraToObject);
		}
		else
		{
			AudStartSound(SOUND_MARIO_YAH_WAH_HOO + ((gAudioRandom % 3) << 16), this->marioObj->header.gfx.cameraToObject);
		}

		this->flags |= MARIO_MARIO_SOUND_PLAYED;
	}
}

/**
 * Adjusts the volume/pitch of sounds from Mario's speed.
 */
void PlayerRecord::adjust_sound_for_speed()
{
	s32 absForwardVel = (this->forwardVel > 0.0f) ? this->forwardVel : -this->forwardVel;
	func_80320A4C(1, (absForwardVel > 100) ? 100 : absForwardVel);
}

/**
 * Spawns particles if the step sound says to, then either plays a step sound or relevant other sound.
 */
void PlayerRecord::play_sound_and_spawn_particles(u32 soundBits, u32 waveParticleType)
{
	if(this->terrainSoundAddend == (SOUND_TERRAIN_WATER << 16))
	{
		if(waveParticleType != 0)
		{
			this->particleFlags |= PARTICLE_WATER_WAVE_SHRINKING;
		}
		else
		{
			this->particleFlags |= PARTICLE_WAVES_GENERATOR;
		}
	}
	else
	{
		if(this->terrainSoundAddend == (SOUND_TERRAIN_SAND << 16))
		{
			this->particleFlags |= PARTICLE_GROUND_SAND;
		}
		else if(this->terrainSoundAddend == (SOUND_TERRAIN_SNOW << 16))
		{
			this->particleFlags |= PARTICLE_GROUND_SNOW;
		}
	}

	if((this->flags & MARIO_METAL_CAP) || soundBits == SOUND_ACTION_UNSTUCK_FROM_GROUND || soundBits == SOUND_MARIO_PUNCH_HOO)
	{
		AudStartSound(soundBits, this->marioObj->header.gfx.cameraToObject);
	}
	else
	{
		AudStartSound(this->terrainSoundAddend + soundBits, this->marioObj->header.gfx.cameraToObject);
	}
}

/**
 * Plays an environmental sound if one has not been played since the last action change.
 */
void PlayerRecord::play_mario_action_sound(u32 soundBits, u32 waveParticleType)
{
	if((this->flags & MARIO_ACTION_SOUND_PLAYED) == 0)
	{
		play_sound_and_spawn_particles(soundBits, waveParticleType);
		this->flags |= MARIO_ACTION_SOUND_PLAYED;
	}
}

/**
 * Plays a landing sound, accounting for metal cap.
 */
void PlayerRecord::play_mario_landing_sound(u32 soundBits)
{
	play_sound_and_spawn_particles((this->flags & MARIO_METAL_CAP) ? SOUND_ACTION_METAL_LANDING : soundBits, 1);
}

/**
 * Plays a landing sound, accounting for metal cap. Unlike play_mario_landing_sound,
 * this function uses play_mario_action_sound, making sure the sound is only
 * played once per action.
 */
void PlayerRecord::play_mario_landing_sound_once(u32 soundBits)
{
	play_mario_action_sound((this->flags & MARIO_METAL_CAP) ? SOUND_ACTION_METAL_LANDING : soundBits, 1);
}

/**
 * Plays a heavy landing (ground pound, etc.) sound, accounting for metal cap.
 */
void PlayerRecord::play_mario_heavy_landing_sound(u32 soundBits)
{
	play_sound_and_spawn_particles((this->flags & MARIO_METAL_CAP) ? SOUND_ACTION_METAL_HEAVY_LANDING : soundBits, 1);
}

/**
 * Plays a heavy landing (ground pound, etc.) sound, accounting for metal cap.
 * Unlike play_mario_heavy_landing_sound, this function uses play_mario_action_sound,
 * making sure the sound is only played once per action.
 */
void PlayerRecord::play_mario_heavy_landing_sound_once(u32 soundBits)
{
	play_mario_action_sound((this->flags & MARIO_METAL_CAP) ? SOUND_ACTION_METAL_HEAVY_LANDING : soundBits, 1);
}

/**
 * Plays action and mario sounds relevant to what was passed into the function.
 */
void PlayerRecord::play_mario_sound(s32 actionSound, s32 marioSound)
{
	if(actionSound == SOUND_ACTION_TERRAIN_JUMP)
	{
		play_mario_action_sound((this->flags & MARIO_METAL_CAP) ? SOUND_ACTION_METAL_JUMP : SOUND_ACTION_TERRAIN_JUMP, 1);
	}
	else
	{
		play_sound_if_no_flag(actionSound, MARIO_ACTION_SOUND_PLAYED);
	}

	if(marioSound == 0)
	{
		play_mario_jump_sound();
	}

	if(marioSound != -1)
	{
		play_sound_if_no_flag(marioSound, MARIO_MARIO_SOUND_PLAYED);
	}
}

/**************************************************
 *                     ACTIONS                    *
 **************************************************/

/**
 * Sets Mario's other velocities from his forward speed.
 */
void PlayerRecord::mario_set_forward_vel(f32 forwardVel)
{
	this->forwardVel = forwardVel;

	this->slideVelX = sins(this->faceAngle[1]) * this->forwardVel;
	this->slideVelZ = coss(this->faceAngle[1]) * this->forwardVel;

	this->vel[0] = (f32)this->slideVelX;
	this->vel[2] = (f32)this->slideVelZ;
}

/**
 * Returns the slipperines class of Mario's floor.
 */
s32 PlayerRecord::mario_get_floor_class()
{
	s32 floorClass;

	// The slide terrain type defaults to slide slipperiness.
	// This doesn't matter too much since normally the slide terrain
	// is checked for anyways.
	if((this->area->terrainType & TERRAIN_MASK) == TERRAIN_SLIDE)
	{
		floorClass = SURFACE_CLASS_VERY_SLIPPERY;
	}
	else
	{
		floorClass = SURFACE_CLASS_DEFAULT;
	}

	if(this->floor)
	{
		switch(this->floor->type)
		{
			case SURFACE_NOT_SLIPPERY:
			case SURFACE_HARD_NOT_SLIPPERY:
			case SURFACE_SWITCH:
				floorClass = SURFACE_CLASS_NOT_SLIPPERY;
				break;

			case SURFACE_SLIPPERY:
			case SURFACE_NOISE_SLIPPERY:
			case SURFACE_HARD_SLIPPERY:
			case SURFACE_NO_CAM_COL_SLIPPERY:
				floorClass = SURFACE_CLASS_SLIPPERY;
				break;

			case SURFACE_VERY_SLIPPERY:
			case SURFACE_ICE:
			case SURFACE_HARD_VERY_SLIPPERY:
			case SURFACE_NOISE_VERY_SLIPPERY_73:
			case SURFACE_NOISE_VERY_SLIPPERY_74:
			case SURFACE_NOISE_VERY_SLIPPERY:
			case SURFACE_NO_CAM_COL_VERY_SLIPPERY:
				floorClass = SURFACE_CLASS_VERY_SLIPPERY;
				break;
		}
	}

	// Crawling allows Mario to not slide on certain steeper surfaces.
	if(this->status == ACT_CRAWLING && this->floor->normal.y > 0.5f && floorClass == SURFACE_CLASS_DEFAULT)
	{
		floorClass = SURFACE_CLASS_NOT_SLIPPERY;
	}

	return floorClass;
}

/**
 * Computes a value that should be added to terrain sounds before playing them.
 * This depends on surfaces and terrain.
 */
u32 PlayerRecord::mario_get_terrain_sound_addend()
{
	s16 floorSoundType;
	s16 terrainType = this->area->terrainType & TERRAIN_MASK;
	s32 ret		= SOUND_TERRAIN_DEFAULT << 16;
	s32 floorType;

	if(this->floor)
	{
		floorType = this->floor->type;

		if((activeStageNo != LEVEL_LLL) && (this->floorHeight < (this->waterLevel - 10)))
		{
			// Water terrain sound, excluding LLL since it uses water in the volcano.
			ret = SOUND_TERRAIN_WATER << 16;
		}
		else if(SURFACE_IS_QUICKSAND(floorType))
		{
			ret = SOUND_TERRAIN_SAND << 16;
		}
		else
		{
			switch(floorType)
			{
				default:
					floorSoundType = 0;
					break;

				case SURFACE_NOT_SLIPPERY:
				case SURFACE_HARD:
				case SURFACE_HARD_NOT_SLIPPERY:
				case SURFACE_SWITCH:
					floorSoundType = 1;
					break;

				case SURFACE_SLIPPERY:
				case SURFACE_HARD_SLIPPERY:
				case SURFACE_NO_CAM_COL_SLIPPERY:
					floorSoundType = 2;
					break;

				case SURFACE_VERY_SLIPPERY:
				case SURFACE_ICE:
				case SURFACE_HARD_VERY_SLIPPERY:
				case SURFACE_NOISE_VERY_SLIPPERY_73:
				case SURFACE_NOISE_VERY_SLIPPERY_74:
				case SURFACE_NOISE_VERY_SLIPPERY:
				case SURFACE_NO_CAM_COL_VERY_SLIPPERY:
					floorSoundType = 3;
					break;

				case SURFACE_NOISE_DEFAULT:
					floorSoundType = 4;
					break;

				case SURFACE_NOISE_SLIPPERY:
					floorSoundType = 5;
					break;
			}

			ret = sTerrainSounds[terrainType][floorSoundType] << 16;
		}
	}

	return ret;
}

/**
 * Determines if Mario is facing "downhill."
 */
s32 PlayerRecord::mario_facing_downhill(s32 turnYaw)
{
	s16 faceAngleYaw = this->faceAngle[1];

	// This is never used in practice, as turnYaw is
	// always passed as zero.
	if(turnYaw && this->forwardVel < 0.0f)
	{
		faceAngleYaw += 0x8000;
	}

	faceAngleYaw = this->floorAngle - faceAngleYaw;

	return (-0x4000 < faceAngleYaw) && (faceAngleYaw < 0x4000);
}

/**
 * Determines if a surface is slippery based on the surface class.
 */
u32 PlayerRecord::mario_floor_is_slippery()
{
	f32 normY;

	if((this->area->terrainType & TERRAIN_MASK) == TERRAIN_SLIDE && this->floor->normal.y < 0.9998477f //~cos(1 deg)
	)
	{
		return TRUE;
	}

	switch(mario_get_floor_class())
	{
		case SURFACE_VERY_SLIPPERY:
			normY = 0.9848077f; //~cos(10 deg)
			break;

		case SURFACE_SLIPPERY:
			normY = 0.9396926f; //~cos(20 deg)
			break;

		default:
			normY = 0.7880108f; //~cos(38 deg)
			break;

		case SURFACE_NOT_SLIPPERY:
			normY = 0.0f;
			break;
	}

	return this->floor->normal.y <= normY;
}

/**
 * Determines if a surface is a slope based on the surface class.
 */
s32 PlayerRecord::mario_floor_is_slope()
{
	f32 normY;

	if((this->area->terrainType & TERRAIN_MASK) == TERRAIN_SLIDE && this->floor->normal.y < 0.9998477f //~cos(1 deg)
	)
	{
		return TRUE;
	}

	switch(mario_get_floor_class())
	{
		case SURFACE_VERY_SLIPPERY:
			normY = 0.9961947f; //~cos(5 deg)
			break;

		case SURFACE_SLIPPERY:
			normY = 0.9848077f; //~cos(10 deg)
			break;

		default:
			normY = 0.9659258f; //~cos(15 deg)
			break;

		case SURFACE_NOT_SLIPPERY:
			normY = 0.9396926f; //~cos(20 deg)
			break;
	}

	return this->floor->normal.y <= normY;
}

/**
 * Determines if a surface is steep based on the surface class.
 */
s32 PlayerRecord::mario_floor_is_steep()
{
	f32 normY;
	s32 result = FALSE;

	// Interestingly, this function does not check for the
	// slide terrain type. This means that steep behavior persists for
	// non-slippery and slippery surfaces.
	// This does not matter in vanilla game practice.
	if(!mario_facing_downhill(FALSE))
	{
		switch(mario_get_floor_class())
		{
			case SURFACE_VERY_SLIPPERY:
				normY = 0.9659258f; //~cos(15 deg)
				break;

			case SURFACE_SLIPPERY:
				normY = 0.9396926f; //~cos(20 deg)
				break;

			default:
				normY = 0.8660254f; //~cos(30 deg)
				break;

			case SURFACE_NOT_SLIPPERY:
				normY = 0.8660254f; //~cos(30 deg)
				break;
		}

		result = this->floor->normal.y <= normY;
	}

	return result;
}

/**
 * Finds the floor height relative from Mario given polar displacement.
 */
f32 PlayerRecord::find_floor_height_relative_polar(s16 angleFromMario, f32 distFromMario)
{
	struct Surface* floor;
	f32 floorY;

	f32 y = sins(this->faceAngle[1] + angleFromMario) * distFromMario;
	f32 x = coss(this->faceAngle[1] + angleFromMario) * distFromMario;

	floorY = mcBGGroundCheck(this->pos[0] + y, this->pos[1] + 100.0f, this->pos[2] + x, &floor);

	return floorY;
}

/**
 * Returns the slope of the floor based off points around Mario.
 */
s16 PlayerRecord::find_floor_slope(s16 yawOffset)
{
	struct Surface* floor;
	f32 forwardFloorY, backwardFloorY;
	f32 forwardYDelta, backwardYDelta;
	s16 result;

	f32 x = sins(this->faceAngle[1] + yawOffset) * 5.0f;
	f32 z = coss(this->faceAngle[1] + yawOffset) * 5.0f;

	forwardFloorY  = mcBGGroundCheck(this->pos[0] + x, this->pos[1] + 100.0f, this->pos[2] + z, &floor);
	backwardFloorY = mcBGGroundCheck(this->pos[0] - x, this->pos[1] + 100.0f, this->pos[2] - z, &floor);

	//! If Mario is near OOB, these floorY's can sometimes be -11000.
	//  This will cause these to be off and give improper slopes.
	forwardYDelta  = forwardFloorY - this->pos[1];
	backwardYDelta = this->pos[1] - backwardFloorY;

	if(forwardYDelta * forwardYDelta < backwardYDelta * backwardYDelta)
	{
		result = atan2s(5.0f, forwardYDelta);
	}
	else
	{
		result = atan2s(5.0f, backwardYDelta);
	}

	return result;
}

/**
 * Adjusts Mario's camera and sound based on his action status.
 */
void PlayerRecord::update_mario_sound_and_camera()
{
	u32 action    = this->status;
	s32 camPreset = this->area->camera->mode;

	if(action == ACT_FIRST_PERSON)
	{
		AudEndMute(2);
		gameCameraControl &= ~CAM_MOVE_C_UP_MODE;
		// Go back to the last camera mode
		set_camera_mode(this->area->camera, -1, 1);
	}
	else if(action == ACT_SLEEPING)
	{
		AudEndMute(2);
	}

	if(!(action & (ACT_FLAG_SWIMMING | ACT_FLAG_METAL_WATER)))
	{
		if(camPreset == CAMERA_MODE_BEHIND_MARIO || camPreset == CAMERA_MODE_WATER_SURFACE)
		{
			set_camera_mode(this->area->camera, this->area->camera->defMode, 1);
		}
	}
}

/**
 * Transitions Mario to a steep jump action.
 */
void PlayerRecord::set_steep_jump_action()
{
	this->marioObj->oMarioSteepJumpYaw = this->faceAngle[1];

	if(this->forwardVel > 0.0f)
	{
		//! ((s16)0x8000) has undefined behavior. Therefore, this downcast has
		// undefined behavior if this->floorAngle >= 0.
		s16 angleTemp	  = this->floorAngle + 0x8000;
		s16 faceAngleTemp = this->faceAngle[1] - angleTemp;

		f32 y = sins(faceAngleTemp) * this->forwardVel;
		f32 x = coss(faceAngleTemp) * this->forwardVel * 0.75f;

		this->forwardVel   = sqrtf(y * y + x * x);
		this->faceAngle[1] = atan2s(x, y) + angleTemp;
	}

	drop_and_set_mario_action(ACT_STEEP_JUMP, 0);
}

/**
 * Set's Marios vertical speed from his forward speed.
 */
void PlayerRecord::set_mario_y_vel_based_on_fspeed(f32 initialVelY, f32 multiplier)
{
	// get_additive_y_vel_for_jumps is always 0 and a stubbed function.
	// It was likely trampoline related based on code location.
	float superJumpScaler = sm64::config().cheats().superJump() ? 1.25f : 1.0f;

	this->vel[1] = (initialVelY + get_additive_y_vel_for_jumps() + this->forwardVel * multiplier) * superJumpScaler;

	if(this->squishTimer != 0 || this->sinking > 1.0f)
	{
		this->vel[1] *= 0.5f;
	}
}

/**
 * Transitions for a variety of airborne actions.
 */
u32 PlayerRecord::set_mario_action_airborne(u32 action, u32 actionArg)
{
	f32 fowardVel;

	if(this->squishTimer != 0 || this->sinking >= 1.0f)
	{
		if(action == ACT_DOUBLE_JUMP || action == ACT_TWIRLING)
		{
			action = ACT_JUMP;
		}
	}

	switch(action)
	{
		case ACT_DOUBLE_JUMP:
			set_mario_y_vel_based_on_fspeed(52.0f, 0.25f);
			this->forwardVel *= 0.8f;
			break;

		case ACT_BACKFLIP:
			this->marioObj->header.gfx.unk38.animID = -1;
			this->forwardVel			= -16.0f;
			set_mario_y_vel_based_on_fspeed(62.0f, 0.0f);
			break;

		case ACT_TRIPLE_JUMP:
			set_mario_y_vel_based_on_fspeed(69.0f, 0.0f);
			this->forwardVel *= 0.8f;
			break;

		case ACT_FLYING_TRIPLE_JUMP:
			set_mario_y_vel_based_on_fspeed(82.0f, 0.0f);
			break;

		case ACT_WATER_JUMP:
		case ACT_HOLD_WATER_JUMP:
			if(actionArg == 0)
			{
				set_mario_y_vel_based_on_fspeed(42.0f, 0.0f);
			}
			break;

		case ACT_BURNING_JUMP:
			this->vel[1]	 = 31.5f;
			this->forwardVel = 8.0f;
			break;

		case ACT_RIDING_SHELL_JUMP:
			set_mario_y_vel_based_on_fspeed(42.0f, 0.25f);
			break;

		case ACT_JUMP:
		case ACT_HOLD_JUMP:
			this->marioObj->header.gfx.unk38.animID = -1;
			set_mario_y_vel_based_on_fspeed(42.0f, 0.25f);
			this->forwardVel *= 0.8f;
			break;

		case ACT_WALL_KICK_AIR:
		case ACT_TOP_OF_POLE_JUMP:
			set_mario_y_vel_based_on_fspeed(62.0f, 0.0f);
			if(this->forwardVel < 24.0f)
			{
				this->forwardVel = 24.0f;
			}
			this->wallKickTimer = 0;
			break;

		case ACT_SIDE_FLIP:
			set_mario_y_vel_based_on_fspeed(62.0f, 0.0f);
			this->forwardVel   = 8.0f;
			this->faceAngle[1] = this->intendedYaw;
			break;

		case ACT_STEEP_JUMP:
			this->marioObj->header.gfx.unk38.animID = -1;
			set_mario_y_vel_based_on_fspeed(42.0f, 0.25f);
			this->faceAngle[0] = -0x2000;
			break;

		case ACT_LAVA_BOOST:
			this->vel[1] = 84.0f;
			if(actionArg == 0)
			{
				this->forwardVel = 0.0f;
			}
			break;

		case ACT_DIVE:
			if((fowardVel = this->forwardVel + 15.0f) > 48.0f)
			{
				fowardVel = 48.0f;
			}
			mario_set_forward_vel(fowardVel);
			break;

		case ACT_LONG_JUMP:
			this->marioObj->header.gfx.unk38.animID = -1;
			set_mario_y_vel_based_on_fspeed(30.0f, 0.0f);
			this->marioObj->oMarioLongJumpIsSlow = this->forwardVel > 16.0f ? FALSE : TRUE;

			//! (BLJ's) This properly handles long jumps from getting forward speed with
			//  too much velocity, but misses backwards longs allowing high negative speeds.
			if((this->forwardVel *= 1.5f) > 48.0f)
			{
				this->forwardVel = 48.0f;
			}
			break;

		case ACT_SLIDE_KICK:
			this->vel[1] = 12.0f;
			if(this->forwardVel < 32.0f)
			{
				this->forwardVel = 32.0f;
			}
			break;

		case ACT_JUMP_KICK:
			this->vel[1] = 20.0f;
			break;
	}

	this->fallpos = this->pos[1];
	this->flags |= MARIO_UNKNOWN_08;

	return action;
}

/**
 * Transitions for a variety of moving actions.
 */
u32 PlayerRecord::set_mario_action_moving(u32 action, UNUSED u32 actionArg)
{
	s16 floorClass = mario_get_floor_class();
	f32 forwardVel = this->forwardVel;
	f32 mag	       = MIN(this->intendedMag, 8.0f);

	switch(action)
	{
		case ACT_WALKING:
			if(floorClass != SURFACE_CLASS_VERY_SLIPPERY)
			{
				if(0.0f <= forwardVel && forwardVel < mag)
				{
					this->forwardVel = mag;
				}
			}

			this->marioObj->oMarioWalkingPitch = 0;
			break;

		case ACT_HOLD_WALKING:
			if(0.0f <= forwardVel && forwardVel < mag / 2.0f)
			{
				this->forwardVel = mag / 2.0f;
			}
			break;

		case ACT_BEGIN_SLIDING:
			if(mario_facing_downhill(FALSE))
			{
				action = ACT_BUTT_SLIDE;
			}
			else
			{
				action = ACT_STOMACH_SLIDE;
			}
			break;

		case ACT_HOLD_BEGIN_SLIDING:
			if(mario_facing_downhill(FALSE))
			{
				action = ACT_HOLD_BUTT_SLIDE;
			}
			else
			{
				action = ACT_HOLD_STOMACH_SLIDE;
			}
			break;
	}

	return action;
}

/**
 * Transition for certain submerged actions, which is actually just the metal jump actions.
 */
u32 PlayerRecord::set_mario_action_submerged(u32 action, UNUSED u32 actionArg)
{
	if(action == ACT_METAL_WATER_JUMP || action == ACT_HOLD_METAL_WATER_JUMP)
	{
		this->vel[1] = 32.0f;
	}

	return action;
}

/**
 * Transitions for a variety of cutscene actions.
 */
u32 PlayerRecord::set_mario_action_cutscene(u32 action, UNUSED u32 actionArg)
{
	switch(action)
	{
		case ACT_EMERGE_FROM_PIPE:
			this->vel[1] = 52.0f;
			break;

		case ACT_FALL_AFTER_STAR_GRAB:
			mario_set_forward_vel(0.0f);
			break;

		case ACT_SPAWN_SPIN_AIRBORNE:
			mario_set_forward_vel(2.0f);
			break;

		case ACT_SPECIAL_EXIT_AIRBORNE:
		case ACT_SPECIAL_DEATH_EXIT:
			this->vel[1] = 64.0f;
			break;
	}

	return action;
}

/**
 * Puts Mario into a given action, putting Mario through the appropriate
 * specific function if needed.
 */
u32 PlayerRecord::ChangePlayerStatus(u32 action, u32 actionArg)
{
	switch(action & ACT_GROUP_MASK)
	{
		case ACT_GROUP_MOVING:
			action = set_mario_action_moving(action, actionArg);
			break;

		case ACT_GROUP_AIRBORNE:
			action = set_mario_action_airborne(action, actionArg);
			break;

		case ACT_GROUP_SUBMERGED:
			action = set_mario_action_submerged(action, actionArg);
			break;

		case ACT_GROUP_CUTSCENE:
			action = set_mario_action_cutscene(action, actionArg);
			break;
	}

	// Resets the sound played flags, meaning Mario can play those sound types again.
	this->flags &= ~(MARIO_ACTION_SOUND_PLAYED | MARIO_MARIO_SOUND_PLAYED);

	if(!(this->status & ACT_FLAG_AIR))
	{
		this->flags &= ~MARIO_UNKNOWN_18;
	}

	// Initialize the action information.
	this->oldstatus	  = this->status;
	this->status	  = action;
	this->actionArg	  = actionArg;
	this->actionState = 0;
	this->actionTimer = 0;

	return TRUE;
}

/**
 * Puts Mario into a specific jumping action from a landing action.
 */
s32 PlayerRecord::set_jump_from_landing()
{
	if(this->sinking >= 11.0f)
	{
		if(this->heldObj == NULL)
		{
			return ChangePlayerStatus(ACT_QUICKSAND_JUMP_LAND, 0);
		}
		else
		{
			return ChangePlayerStatus(ACT_HOLD_QUICKSAND_JUMP_LAND, 0);
		}
	}

	if(mario_floor_is_steep())
	{
		set_steep_jump_action();
	}
	else
	{
		if((this->doubleJumpTimer == 0) || (this->squishTimer != 0))
		{
			ChangePlayerStatus(ACT_JUMP, 0);
		}
		else
		{
			switch(this->oldstatus)
			{
				case ACT_JUMP_LAND:
					ChangePlayerStatus(ACT_DOUBLE_JUMP, 0);
					break;

				case ACT_FREEFALL_LAND:
					ChangePlayerStatus(ACT_DOUBLE_JUMP, 0);
					break;

				case ACT_SIDE_FLIP_LAND_STOP:
					ChangePlayerStatus(ACT_DOUBLE_JUMP, 0);
					break;

				case ACT_DOUBLE_JUMP_LAND:
					// If Mario has a wing cap, he ignores the typical speed
					// requirement for a triple jump.
					if(this->flags & MARIO_WING_CAP)
					{
						ChangePlayerStatus(ACT_FLYING_TRIPLE_JUMP, 0);
					}
					else if(this->forwardVel > 20.0f)
					{
						ChangePlayerStatus(ACT_TRIPLE_JUMP, 0);
					}
					else
					{
						ChangePlayerStatus(ACT_JUMP, 0);
					}
					break;

				default:
					ChangePlayerStatus(ACT_JUMP, 0);
					break;
			}
		}
	}

	this->doubleJumpTimer = 0;

	return TRUE;
}

/**
 * Puts Mario in a given action, as long as it is not overruled by
 * either a quicksand or steep jump.
 */
s32 PlayerRecord::set_jumping_action(u32 action, u32 actionArg)
{
	UNUSED u32 currAction = this->status;

	if(this->sinking >= 11.0f)
	{
		// Checks whether Mario is holding an object or not.
		if(this->heldObj == NULL)
		{
			return ChangePlayerStatus(ACT_QUICKSAND_JUMP_LAND, 0);
		}
		else
		{
			return ChangePlayerStatus(ACT_HOLD_QUICKSAND_JUMP_LAND, 0);
		}
	}

	if(mario_floor_is_steep())
	{
		set_steep_jump_action();
	}
	else
	{
		ChangePlayerStatus(action, actionArg);
	}

	return TRUE;
}

/**
 * Drop anything Mario is holding and set a new action.
 */
s32 PlayerRecord::drop_and_set_mario_action(u32 action, u32 actionArg)
{
	mario_stop_riding_and_holding(this);

	return ChangePlayerStatus(action, actionArg);
}

/**
 * Increment Mario's hurt counter and set a new action.
 */
s32 PlayerRecord::hurt_and_set_mario_action(u32 action, u32 actionArg, s16 hurtCounter)
{
	this->hurtCounter = hurtCounter;

	return ChangePlayerStatus(action, actionArg);
}

/**
 * Checks a variety of inputs for common transitions between
 * many different actions. A common variant of the
 * below function.
 */
s32 PlayerRecord::check_common_action_exits()
{
	if(this->input & INPUT_A_PRESSED)
	{
		return ChangePlayerStatus(ACT_JUMP, 0);
	}
	if(this->input & INPUT_OFF_FLOOR)
	{
		return ChangePlayerStatus(ACT_FREEFALL, 0);
	}
	if(this->input & INPUT_NONZERO_ANALOG)
	{
		return ChangePlayerStatus(ACT_WALKING, 0);
	}
	if(this->input & INPUT_ABOVE_SLIDE)
	{
		return ChangePlayerStatus(ACT_BEGIN_SLIDING, 0);
	}

	return FALSE;
}

/**
 * Checks a variety of inputs for common transitions between
 * many different object holding actions. A holding variant of the
 * above function.
 */
s32 PlayerRecord::check_common_hold_action_exits()
{
	if(this->input & INPUT_A_PRESSED)
	{
		return ChangePlayerStatus(ACT_HOLD_JUMP, 0);
	}
	if(this->input & INPUT_OFF_FLOOR)
	{
		return ChangePlayerStatus(ACT_HOLD_FREEFALL, 0);
	}
	if(this->input & INPUT_NONZERO_ANALOG)
	{
		return ChangePlayerStatus(ACT_HOLD_WALKING, 0);
	}
	if(this->input & INPUT_ABOVE_SLIDE)
	{
		return ChangePlayerStatus(ACT_HOLD_BEGIN_SLIDING, 0);
	}

	return FALSE;
}

/**
 * Transitions Mario from a submerged action to a walking action.
 */
s32 PlayerRecord::transition_submerged_to_walking()
{
	set_camera_mode(this->area->camera, this->area->camera->defMode, 1);

	vec3s_set(this->angleVel, 0, 0, 0);

	if(this->heldObj == NULL)
	{
		return ChangePlayerStatus(ACT_WALKING, 0);
	}
	else
	{
		return ChangePlayerStatus(ACT_HOLD_WALKING, 0);
	}
}

/**
 * This is the transition function typically for entering a submerged action for a
 * non-submerged action. This also applies the water surface camera preset.
 */
s32 PlayerRecord::set_water_plunge_action()
{
	this->forwardVel = this->forwardVel / 4.0f;
	this->vel[1]	 = this->vel[1] / 2.0f;

	this->pos[1] = this->waterLevel - 100;

	this->faceAngle[2] = 0;

	vec3s_set(this->angleVel, 0, 0, 0);

	if((this->status & ACT_FLAG_DIVING) == 0)
	{
		this->faceAngle[0] = 0;
	}

	if(this->area->camera->mode != CAMERA_MODE_WATER_SURFACE)
	{
		set_camera_mode(this->area->camera, CAMERA_MODE_WATER_SURFACE, 1);
	}

	return ChangePlayerStatus(ACT_WATER_PLUNGE, 0);
}

/**
 * Applies the squish to Mario's model via scaling.
 */
void PlayerRecord::squish_mario_model()
{
	if(this->squishTimer != 0xFF * FRAME_RATE_SCALER_INV)
	{
		// If no longer squished, scale back to default.
		if(this->squishTimer == 0)
		{
			vec3f_set(this->marioObj->header.gfx.scale, 1.0f, 1.0f, 1.0f);
		}
		// If timer is less than 16, rubber-band Mario's size scale up and down.
		else if(this->squishTimer <= 16 * FRAME_RATE_SCALER_INV)
		{
			this->squishTimer--;

			this->marioObj->header.gfx.scale[1] = 1.0f - ((sSquishScaleOverTime[15 - (this->squishTimer / FRAME_RATE_SCALER_INV)] * 0.6f) / 100.0f);
			this->marioObj->header.gfx.scale[0] = ((sSquishScaleOverTime[15 - (this->squishTimer / FRAME_RATE_SCALER_INV)] * 0.4f) / 100.0f) + 1.0f;

			this->marioObj->header.gfx.scale[2] = this->marioObj->header.gfx.scale[0];
		}
		else
		{
			this->squishTimer--;

			vec3f_set(this->marioObj->header.gfx.scale, 1.4f, 0.4f, 1.4f);
		}
	}
}

/**
 * Update the button inputs for Mario.
 */
void PlayerRecord::update_mario_button_inputs()
{
	if(this->controller->buttonPressed & sm64::hid::A_BUTTON)
	{
		this->input |= INPUT_A_PRESSED;
	}

	if(this->controller->buttonDown & sm64::hid::A_BUTTON)
	{
		this->input |= INPUT_A_DOWN;
	}

	// Don't update for these buttons if squished.
	if(this->squishTimer == 0)
	{
		if(this->controller->buttonPressed & sm64::hid::B_BUTTON)
		{
			this->input |= INPUT_B_PRESSED;
		}

		if(this->controller->buttonDown & sm64::hid::Z_TRIG)
		{
			this->input |= INPUT_Z_DOWN;
		}

		if(this->controller->buttonPressed & sm64::hid::Z_TRIG)
		{
			this->input |= INPUT_Z_PRESSED;
		}
	}

	if(this->input & INPUT_A_PRESSED)
	{
		this->framesSinceA = 0;
	}
	else if(this->framesSinceA < 0xFF)
	{
		this->framesSinceA += 1;
	}

	if(this->input & INPUT_B_PRESSED)
	{
		this->framesSinceB = 0;
	}
	else if(this->framesSinceB < 0xff)
	{
		this->framesSinceB += 1;
	}
}

/**
 * Updates the joystick intended magnitude.
 */
void PlayerRecord::update_mario_joystick_inputs()
{
	f32 mag = ((controller->stickMag / 64.0f) * (controller->stickMag / 64.0f)) * 64.0f;

	if(this->squishTimer == 0)
	{
		this->intendedMag = mag / 2.0f;
	}
	else
	{
		this->intendedMag = mag / 8.0f;
	}

	if(this->intendedMag > 0.0f)
	{
#ifdef ENABLE_MOUSE2
		this->intendedYaw = atan2s(-controller->stickY, controller->stickX);
#else
		this->intendedYaw = atan2s(-controller->stickY, controller->stickX) + this->area->camera->yaw;
#endif
		this->input |= INPUT_NONZERO_ANALOG;
	}
	else
	{
		this->intendedYaw = this->faceAngle[1];
	}
}

/**
 * Resolves wall collisions, and updates a variety of inputs.
 */
void PlayerRecord::update_mario_geometry_inputs()
{
	f32 gasLevel;
	f32 ceilToFloorDist;

	f32_find_wall_collision(&this->pos[0], &this->pos[1], &this->pos[2], 60.0f, 50.0f);
	f32_find_wall_collision(&this->pos[0], &this->pos[1], &this->pos[2], 30.0f, 24.0f);

	this->floorHeight = mcBGGroundCheck(this->pos[0], this->pos[1], this->pos[2], &this->floor);

	// If Mario is OOB, move his position to his graphical position (which was not updated)
	// and check for the floor there.
	// This can cause errant behavior when combined with astral projection,
	// since the graphical position was not Mario's previous location.
	if(this->floor == NULL)
	{
		vec3f_copy(this->pos, this->marioObj->header.gfx.pos);
		this->floorHeight = mcBGGroundCheck(this->pos[0], this->pos[1], this->pos[2], &this->floor);
	}

	this->ceilHeight = vec3f_find_ceil(&this->pos[0], this->floorHeight, &this->ceil);
	gasLevel	 = find_poison_gas_level(this->pos[0], this->pos[2]);
	this->waterLevel = find_water_level(this->pos[0], this->pos[2]);

	if(this->floor)
	{
		this->floorAngle	 = atan2s(this->floor->normal.z, this->floor->normal.x);
		this->terrainSoundAddend = mario_get_terrain_sound_addend();

		if((this->pos[1] > this->waterLevel - 40) && mario_floor_is_slippery())
		{
			this->input |= INPUT_ABOVE_SLIDE;
		}

		if((this->floor->flags & SURFACE_FLAG_DYNAMIC) || (this->ceil && this->ceil->flags & SURFACE_FLAG_DYNAMIC))
		{
			ceilToFloorDist = this->ceilHeight - this->floorHeight;

			if((0.0f <= ceilToFloorDist) && (ceilToFloorDist <= 150.0f))
			{
				this->input |= INPUT_SQUISHED;
			}
		}

		if(this->pos[1] > this->floorHeight + 100.0f)
		{
			this->input |= INPUT_OFF_FLOOR;
		}

		if(this->pos[1] < (this->waterLevel - 10))
		{
			this->input |= INPUT_IN_WATER;
		}

		if(this->pos[1] < (gasLevel - 100.0f))
		{
			this->input |= INPUT_IN_POISON_GAS;
		}
	}
	else
	{
		PL_StartFadeout(this, PL_FADE_LOSING);
	}
}

/**
 * Handles Mario's input flags as well as a couple timers.
 */
void PlayerRecord::update_mario_inputs()
{
	this->particleFlags	       = 0;
	this->input		       = 0;
	this->collidedObjInteractTypes = this->marioObj->collidedObjInteractTypes;
	this->flags &= 0xFFFFFF;

	update_mario_button_inputs();
	update_mario_joystick_inputs();
	update_mario_geometry_inputs();

	if(gameCameraControl & CAM_MOVE_C_UP_MODE)
	{
		if(this->status & ACT_FLAG_ALLOW_FIRST_PERSON)
		{
			this->input |= INPUT_FIRST_PERSON;
		}
		else
		{
			gameCameraControl &= ~CAM_MOVE_C_UP_MODE;
		}
	}

	if(!(this->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED)))
	{
		this->input |= INPUT_UNKNOWN_5;
	}

	if(this->marioObj->oInteractStatus & (INT_STATUS_HOOT_GRABBED_BY_MARIO | INT_STATUS_MARIO_UNK1 | INT_STATUS_MARIO_UNK4))
	{
		this->input |= INPUT_UNKNOWN_10;
	}

	if(this->wallKickTimer > 0)
	{
		this->wallKickTimer--;
	}

	if(this->doubleJumpTimer > 0)
	{
		this->doubleJumpTimer--;
	}
}

/**
 * Set's the camera preset for submerged action behaviors.
 */
void PlayerRecord::set_submerged_cam_preset_and_spawn_bubbles()
{
	f32 heightBelowWater;
	s16 camPreset;

	if((this->status & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED)
	{
		heightBelowWater = (f32)(this->waterLevel - 80) - this->pos[1];
		camPreset	 = this->area->camera->mode;

		if((this->status & ACT_FLAG_METAL_WATER))
		{
			if(camPreset != CAMERA_MODE_CLOSE)
			{
				set_camera_mode(this->area->camera, CAMERA_MODE_CLOSE, 1);
			}
		}
		else
		{
			if((heightBelowWater > 800.0f) && (camPreset != CAMERA_MODE_BEHIND_MARIO))
			{
				set_camera_mode(this->area->camera, CAMERA_MODE_BEHIND_MARIO, 1);
			}

			if((heightBelowWater < 400.0f) && (camPreset != CAMERA_MODE_WATER_SURFACE))
			{
				set_camera_mode(this->area->camera, CAMERA_MODE_WATER_SURFACE, 1);
			}

			if((this->status & ACT_FLAG_INTANGIBLE) == 0)
			{
				if((this->pos[1] < (f32)(this->waterLevel - 160)) || (this->faceAngle[0] < -0x800))
				{
					this->particleFlags |= PARTICLE_WATER_BUBBLES;
				}
			}
		}
	}
}

/**
 * Both increments and decrements Mario's HP.
 */
void PlayerRecord::update_mario_health()
{
	s32 terrainIsSnow;

	if(this->health >= 0x100)
	{
		if((frameCounter % FRAME_RATE_SCALER_INV) == 0)
		{
			// When already healing or hurting Mario, Mario's HP is not changed any more here.
			if(((u32)this->healCounter | (u32)this->hurtCounter) == 0)
			{
				if((this->input & INPUT_IN_POISON_GAS) && ((this->status & ACT_FLAG_INTANGIBLE) == 0))
				{
					if(((this->flags & MARIO_METAL_CAP) == 0))
					{
						if(!sm64::config().cheats().invincible())
						{
							this->health -= 4;
						}
					}
				}
				else
				{
					if((this->status & ACT_FLAG_SWIMMING) && ((this->status & ACT_FLAG_INTANGIBLE) == 0))
					{
						terrainIsSnow = (this->area->terrainType & TERRAIN_MASK) == TERRAIN_SNOW;

						// When Mario is near the water surface, recover health (unless in snow),
						// when in snow terrains lose 3 health.
						// If using the debug level select, do not lose any HP to water.
						if((this->pos[1] >= (this->waterLevel - 140)) && !terrainIsSnow)
						{
							this->health += 0x1A;
						}
						else
						{
							if(!sm64::config().cheats().invincible())
							{
								this->health -= (terrainIsSnow ? 3 : 1);
							}
						}
					}
				}
			}

			if(this->healCounter > 0)
			{
				this->health += 0x40;
				this->healCounter--;
			}

			if(this->hurtCounter > 0)
			{
				if(!sm64::config().cheats().invincible())
				{
					this->health -= 0x40;
				}
				this->hurtCounter--;
			}

			if(this->health >= 0x881)
			{
				this->health = 0x880;
			}

			if(this->health < 0x100)
			{
				this->health = 0xFF;
			}
		}

		// Play a noise to alert the player when Mario is close to drowning.
		if(((this->status & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED) && (this->health < 0x300))
		{
			AudStartSound(SOUND_MOVING_ALMOST_DROWNING, gDefaultSoundArgs);

			if(frameCounter % (36 * FRAME_RATE_SCALER_INV) == 0)
			{
				SendMotorEvent(3, 30);
			}
		}
	}
}

/**
 * Updates some basic info for camera usage.
 */
void PlayerRecord::update_mario_info_for_cam()
{
	this->marioBodyState->action  = this->status;
	this->statusForCamera->action = this->status;

	vec3s_copy(this->statusForCamera->faceAngle, this->faceAngle);

	if((this->flags & MARIO_UNKNOWN_25) == 0)
	{
		vec3f_copy(this->statusForCamera->pos, this->pos);
	}
}

/**
 * Resets Mario's model, done every time an action is executed.
 */
void PlayerRecord::mario_reset_bodystate()
{
	struct MarioBodyState* bodyState = this->marioBodyState;

	bodyState->capState   = MARIO_HAS_DEFAULT_CAP_OFF;
	bodyState->eyeState   = MARIO_EYES_BLINK;
	bodyState->handState  = MARIO_HAND_FISTS;
	bodyState->modelState = 0;
	bodyState->unk07      = 0;

	this->flags &= ~MARIO_METAL_SHOCK;
}

/**
 * Adjusts Mario's graphical height for quicksand.
 */
void PlayerRecord::sink_mario_in_quicksand()
{
	struct Object* o = this->marioObj;

	if(o->header.gfx.throwMatrix)
	{
		// TODO: throwMatrix should probably be an actual matrix pointer
		*(f32*)((u8*)o->header.gfx.throwMatrix + 0x34) -= this->sinking;
	}

	o->header.gfx.pos[1] -= this->sinking;
}

/**
 * Updates the cap flags mainly based on the cap timer.
 */
u32 PlayerRecord::update_and_return_cap_flags()
{
	u32 flags = this->flags;
	u32 action;

	if(this->capTimer > 0)
	{
		action = this->status;

		if((this->capTimer <= 60 * FRAME_RATE_SCALER_INV) || ((action != ACT_READING_AUTOMATIC_DIALOG) && (action != ACT_READING_NPC_DIALOG) && (action != ACT_READING_SIGN) && (action != ACT_IN_CANNON)))
		{
			this->capTimer -= 1;
		}

		if(this->capTimer == 0)
		{
			stop_cap_music();

			this->flags &= ~(MARIO_VANISH_CAP | MARIO_METAL_CAP | MARIO_WING_CAP);
			if((this->flags & (MARIO_NORMAL_CAP | MARIO_VANISH_CAP | MARIO_METAL_CAP | MARIO_WING_CAP)) == 0)
			{
				this->flags &= ~MARIO_CAP_ON_HEAD;
			}
		}

		if(this->capTimer == 60 * FRAME_RATE_SCALER_INV)
		{
			fadeout_cap_music();
		}

		// This code flickers the cap through a long binary string, increasing in how
		// common it flickers near the end.
		u16 capTimer = this->capTimer / FRAME_RATE_SCALER_INV;
		if((capTimer < 64) && ((1ULL << capTimer) & sCapFlickerFrames))
		{
			flags &= ~(MARIO_VANISH_CAP | MARIO_METAL_CAP | MARIO_WING_CAP);
			if((flags & (MARIO_NORMAL_CAP | MARIO_VANISH_CAP | MARIO_METAL_CAP | MARIO_WING_CAP)) == 0)
			{
				flags &= ~MARIO_CAP_ON_HEAD;
			}
		}
	}

	return flags;
}

/**
 * Updates the Mario's cap, rendering, and hitbox.
 */
void PlayerRecord::mario_update_hitbox_and_cap_model()
{
	struct MarioBodyState* bodyState = this->marioBodyState;
	s32 flags			 = update_and_return_cap_flags();

	if(flags & MARIO_VANISH_CAP)
	{
		bodyState->modelState = MODEL_STATE_NOISE_ALPHA;
	}

	if(flags & MARIO_METAL_CAP)
	{
		bodyState->modelState |= MODEL_STATE_METAL;
	}

	if(flags & MARIO_METAL_SHOCK)
	{
		bodyState->modelState |= MODEL_STATE_METAL;
	}

	if(this->invincTimer >= 3 * FRAME_RATE_SCALER_INV)
	{
		//! (Pause buffered hitstun) Since the global timer increments while paused,
		//  this can be paused through to give continual invisibility. This leads to
		//  no interaction with objects.
		if(frameCounter & FRAME_RATE_SCALER_INV)
		{
			marioWorks->marioObj->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
		}
	}

	if(flags & MARIO_CAP_IN_HAND)
	{
		if(flags & MARIO_WING_CAP)
		{
			bodyState->handState = MARIO_HAND_HOLDING_WING_CAP;
		}
		else
		{
			bodyState->handState = MARIO_HAND_HOLDING_CAP;
		}
	}

	if(flags & MARIO_CAP_ON_HEAD)
	{
		if(flags & MARIO_WING_CAP)
		{
			bodyState->capState = MARIO_HAS_WING_CAP_ON;
		}
		else
		{
			bodyState->capState = MARIO_HAS_DEFAULT_CAP_ON;
		}
	}

	// Short hitbox for crouching/crawling/etc.
	if(this->status & ACT_FLAG_SHORT_HITBOX)
	{
		this->marioObj->hitboxHeight = 100.0f;
	}
	else
	{
		this->marioObj->hitboxHeight = 160.0f;
	}

	if((this->flags & MARIO_TELEPORTING) && (this->fadeWarpOpacity != 0xFF))
	{
		bodyState->modelState &= ~0xFF;
		bodyState->modelState |= (this->fadeWarpOpacity | 0x100);
	}
}

/**************************************************
 *                  INITIALIZATION                *
 **************************************************/

void PlayerRecord::init()
{
	Vec3s capPos;
	struct Object* capObject;

	unused80339F10 = 0;

	marioWorks->actionTimer	 = 0;
	marioWorks->framesSinceA = 0xFF;
	marioWorks->framesSinceB = 0xFF;

	marioWorks->invincTimer = 0;

	if(BuGetItemFlag() & (SAVE_FLAG_CAP_ON_GROUND | SAVE_FLAG_CAP_ON_KLEPTO | SAVE_FLAG_CAP_ON_UKIKI | SAVE_FLAG_CAP_ON_MR_BLIZZARD))
	{
		marioWorks->flags = 0;
	}
	else
	{
		marioWorks->flags = (MARIO_CAP_ON_HEAD | MARIO_NORMAL_CAP);
	}

	marioWorks->forwardVel	= 0.0f;
	marioWorks->squishTimer = 0;

	marioWorks->hurtCounter = 0;
	marioWorks->healCounter = 0;

	marioWorks->capTimer = 0;
	marioWorks->sinking  = 0.0f;

	marioWorks->heldObj   = NULL;
	marioWorks->riddenObj = NULL;
	marioWorks->usedObj   = NULL;

	marioWorks->waterLevel = find_water_level(marioActor->startPos[0], marioActor->startPos[2]);

	marioWorks->area			      = snSceneInfo;
	marioWorks->marioObj			      = gMarioObject;
	marioWorks->marioObj->header.gfx.unk38.animID = -1;
	vec3s_copy(marioWorks->faceAngle, marioActor->startAngle);
	vec3s_set(marioWorks->angleVel, 0, 0, 0);
	vec3s_to_vec3f(marioWorks->pos, marioActor->startPos);
	vec3f_set(marioWorks->vel, 0, 0, 0);
	marioWorks->floorHeight = mcBGGroundCheck(marioWorks->pos[0], marioWorks->pos[1], marioWorks->pos[2], &marioWorks->floor);

	if(marioWorks->pos[1] < marioWorks->floorHeight)
	{
		marioWorks->pos[1] = marioWorks->floorHeight;
	}

	marioWorks->marioObj->header.gfx.pos[1] = marioWorks->pos[1];

	marioWorks->status = (marioWorks->pos[1] <= (marioWorks->waterLevel - 100)) ? ACT_WATER_IDLE : ACT_IDLE;

	mario_reset_bodystate();
	update_mario_info_for_cam();
	marioWorks->marioBodyState->punchState = 0;

	marioWorks->marioObj->oPosX = marioWorks->pos[0];
	marioWorks->marioObj->oPosY = marioWorks->pos[1];
	marioWorks->marioObj->oPosZ = marioWorks->pos[2];

	marioWorks->marioObj->oMoveAnglePitch = marioWorks->faceAngle[0];
	marioWorks->marioObj->oMoveAngleYaw   = marioWorks->faceAngle[1];
	marioWorks->marioObj->oMoveAngleRoll  = marioWorks->faceAngle[2];

	vec3f_copy(marioWorks->marioObj->header.gfx.pos, marioWorks->pos);
	vec3s_set(marioWorks->marioObj->header.gfx.angle, 0, marioWorks->faceAngle[1], 0);

	if(BuGetHatPosition(capPos))
	{
		capObject = s_makeobj_nowpos(marioWorks->marioObj, MODEL_MARIOS_CAP, sm64::bhv::bhvNormalCap());

		capObject->oPosX = capPos[0];
		capObject->oPosY = capPos[1];
		capObject->oPosZ = capPos[2];

		capObject->oForwardVelS32 = 0;

		capObject->oMoveAngleYaw = 0;
	}
}

void PlayerRecord::init_mario_from_save_file()
{
	marioWorks->number	    = 0;
	marioWorks->flags	    = 0;
	marioWorks->status	    = 0;
	marioWorks->spawnInfo	    = &starringActor[0];
	marioWorks->statusForCamera = &camPlayerInfo[0];
	marioWorks->marioBodyState  = &gBodyStates[0];
	marioWorks->controller	    = &sm64::player(0).controller();
	marioWorks->animation	    = &D_80339D10;

	marioWorks->numCoins = 0;
	marioWorks->numStars = BuGetSumStars(activePlayerNo - 1, COURSE_MIN - 1, COURSE_MAX - 1);
	marioWorks->numKeys  = 0;

	marioWorks->numLives = 4;
	marioWorks->health   = 0x880;

	marioWorks->reserved2 = marioWorks->numStars;
	marioWorks->unkB0     = 0xBD;

	playerMeter.coins  = 0;
	playerMeter.wedges = 8;
}

PlayerRecord::PlayerRecord()
{
}

PlayerRecord::~PlayerRecord()
{
}
