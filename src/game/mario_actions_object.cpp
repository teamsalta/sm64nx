#include <ultra64.h>

#include "sm64.h"
#include "mario_actions_object.h"
#include "types.h"
#include "mario_step.h"
#include "mario.h"
#include "audio/external.h"
#include "interaction.h"
#include "audio_defines.h"
#include "engine/math_util.h"
#include "motor.h"

/**
 * Used by act_punching() to determine Mario's forward velocity during each
 * animation frame.
 */
s8 sPunchingForwardVelocities[8] = {0, 1, 1, 2, 3, 5, 7, 10};

void PlayerRecord::animated_stationary_ground_step(s32 animation, u32 endAction)
{
	waitProcess();
	setAnimation(animation);
	if(isLast1AnimeFrame())
	{
		ChangePlayerStatus(endAction, 0);
	}
}

s32 PlayerRecord::mario_update_punch_sequence()
{
	u32 endAction, crouchEndAction;
	s32 animFrame;

	if(status & ACT_FLAG_MOVING)
	{
		endAction = ACT_WALKING, crouchEndAction = ACT_CROUCH_SLIDE;
	}
	else
	{
		endAction = ACT_IDLE, crouchEndAction = ACT_CROUCHING;
	}

	switch(actionArg)
	{
		case 0:
			AudStartSound(SOUND_MARIO_PUNCH_YAH, marioObj->header.gfx.cameraToObject);
			// Fall-through:
		case 1:
			setAnimation(MARIO_ANIM_FIRST_PUNCH);
			if(isLast2AnimeFrame())
			{
				actionArg = 2;
			}
			else
			{
				actionArg = 1;
			}

			if(marioObj->header.gfx.unk38.frame() >= 2)
			{
				if(PL_CheckTakingEnemy(this))
				{
					return TRUE;
				}

				flags |= MARIO_PUNCHING;
			}

			if(actionArg == 2)
			{
				marioBodyState->punchState = 4;
			}
			break;

		case 2:
			setAnimation(MARIO_ANIM_FIRST_PUNCH_FAST);

			if(marioObj->header.gfx.unk38.frame() <= 0)
			{
				flags |= MARIO_PUNCHING;
			}

			if(input & INPUT_B_PRESSED)
			{
				actionArg = 3;
			}

			if(isLast1AnimeFrame())
			{
				ChangePlayerStatus(endAction, 0);
			}
			break;

		case 3:
			AudStartSound(SOUND_MARIO_PUNCH_WAH, marioObj->header.gfx.cameraToObject);
			// Fall-through:
		case 4:
			setAnimation(MARIO_ANIM_SECOND_PUNCH);
			if(isLast2AnimeFrame())
			{
				actionArg = 5;
			}
			else
			{
				actionArg = 4;
			}

			if(marioObj->header.gfx.unk38.frame() > 0)
			{
				flags |= MARIO_PUNCHING;
			}

			if(actionArg == 5)
			{
				marioBodyState->punchState = 68;
			}
			break;

		case 5:
			setAnimation(MARIO_ANIM_SECOND_PUNCH_FAST);
			if(marioObj->header.gfx.unk38.frame() <= 0)
			{
				flags |= MARIO_PUNCHING;
			}

			if(input & INPUT_B_PRESSED)
			{
				actionArg = 6;
			}

			if(isLast1AnimeFrame())
			{
				ChangePlayerStatus(endAction, 0);
			}
			break;

		case 6:
			trigSpecialEffect(SOUND_MARIO_PUNCH_HOO, 1);
			animFrame = setAnimation(MARIO_ANIM_GROUND_KICK);
			if(animFrame == 0)
			{
				marioBodyState->punchState = 134;
			}

			if(animFrame >= 0 && animFrame < 8)
			{
				flags |= MARIO_KICKING;
			}

			if(isLast1AnimeFrame())
			{
				ChangePlayerStatus(endAction, 0);
			}
			break;

		case 9:
			trigSpecialEffect(SOUND_MARIO_PUNCH_HOO, 1);
			setAnimation(MARIO_ANIM_BREAKDANCE);
			animFrame = marioObj->header.gfx.unk38.frame();

			if(animFrame >= 2 && animFrame < 8)
			{
				flags |= MARIO_TRIPPING;
			}

			if(isLast1AnimeFrame())
			{
				ChangePlayerStatus(crouchEndAction, 0);
			}
			break;
	}

	return FALSE;
}

s32 PlayerRecord::act_punching()
{
	if(input & INPUT_UNKNOWN_10)
	{
		return changePlayerDropping(ACT_UNKNOWN_026, 0);
	}

	if(input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE))
	{
		return checkAllMotions();
	}

	if(actionState == 0 && (input & INPUT_A_DOWN))
	{
		return ChangePlayerStatus(ACT_JUMP_KICK, 0);
	}

	actionState = 1;
	if(actionArg == 0)
	{
		actionTimer = 7 * FRAME_RATE_SCALER_INV;
	}

	setPlayerVelocity(sPunchingForwardVelocities[actionTimer / FRAME_RATE_SCALER_INV]);
	if(actionTimer > 0)
	{
		actionTimer--;
	}

	mario_update_punch_sequence();
	walkProcess();
	return FALSE;
}

s32 PlayerRecord::act_picking_up()
{
	if(input & INPUT_UNKNOWN_10)
	{
		return changePlayerDropping(ACT_UNKNOWN_026, 0);
	}

	if(input & INPUT_OFF_FLOOR)
	{
		return changePlayerDropping(ACT_FREEFALL, 0);
	}

	if(actionState == 0 && isLast1AnimeFrame())
	{
		//! While the animation is playing, it is possible for the used object
		// to unload. This allows you to pick up a vacant or newly loaded object
		// slot (cloning via fake object).
		PL_TakeObject(this);
		startSoundEffect(SOUND_MARIO_HRMM, MARIO_MARIO_SOUND_PLAYED);
		actionState = 1;
	}

	if(actionState == 1)
	{
		if(heldObj->oInteractionSubtype & INT_SUBTYPE_GRABS_MARIO)
		{
			marioBodyState->grabPos = GRAB_POS_HEAVY_OBJ;
			setAnimation(MARIO_ANIM_GRAB_HEAVY_OBJECT);
			if(isLast1AnimeFrame())
			{
				ChangePlayerStatus(ACT_HOLD_HEAVY_IDLE, 0);
			}
		}
		else
		{
			marioBodyState->grabPos = GRAB_POS_LIGHT_OBJ;
			setAnimation(MARIO_ANIM_PICK_UP_LIGHT_OBJ);
			if(isLast1AnimeFrame())
			{
				ChangePlayerStatus(ACT_HOLD_IDLE, 0);
			}
		}
	}

	waitProcess();
	return FALSE;
}

s32 PlayerRecord::act_dive_picking_up()
{
	if(input & INPUT_UNKNOWN_10)
	{
		return changePlayerDropping(ACT_UNKNOWN_026, 0);
	}

	//! Hands-free holding. Landing on a slope or being pushed off a ledge while
	// landing from a dive grab sets mario's action to a non-holding action
	// without dropping the object, causing the hands-free holding glitch.
	if(input & INPUT_OFF_FLOOR)
	{
		return ChangePlayerStatus(ACT_FREEFALL, 0);
	}

	if(input & INPUT_ABOVE_SLIDE)
	{
		return ChangePlayerStatus(ACT_BEGIN_SLIDING, 0);
	}

	animated_stationary_ground_step(MARIO_ANIM_STOP_SLIDE_LIGHT_OBJ, ACT_HOLD_IDLE);
	return FALSE;
}

s32 PlayerRecord::act_placing_down()
{
	if(input & INPUT_UNKNOWN_10)
	{
		return changePlayerDropping(ACT_UNKNOWN_026, 0);
	}

	if(input & INPUT_OFF_FLOOR)
	{
		return changePlayerDropping(ACT_FREEFALL, 0);
	}

	if(++actionTimer == 8 * FRAME_RATE_SCALER_INV)
	{
		PL_DropObject(this);
	}

	animated_stationary_ground_step(MARIO_ANIM_PLACE_LIGHT_OBJ, ACT_IDLE);
	return FALSE;
}

s32 PlayerRecord::act_throwing()
{
	if(heldObj && (heldObj->oInteractionSubtype & INT_SUBTYPE_HOLDABLE_NPC))
	{
		return ChangePlayerStatus(ACT_PLACING_DOWN, 0);
	}

	if(input & INPUT_UNKNOWN_10)
	{
		return changePlayerDropping(ACT_UNKNOWN_026, 0);
	}

	if(input & INPUT_OFF_FLOOR)
	{
		return changePlayerDropping(ACT_FREEFALL, 0);
	}

	if(++actionTimer == 7 * FRAME_RATE_SCALER_INV)
	{
		PL_ThrowObject(this);
		startSoundEffect(SOUND_MARIO_WAH2, MARIO_MARIO_SOUND_PLAYED);
		startSoundEffect(SOUND_ACTION_THROW, MARIO_ACTION_SOUND_PLAYED);
		SendMotorEvent(3, 50);
	}

	animated_stationary_ground_step(MARIO_ANIM_GROUND_THROW, ACT_IDLE);
	return FALSE;
}

s32 PlayerRecord::act_heavy_throw()
{
	if(input & INPUT_UNKNOWN_10)
	{
		return changePlayerDropping(ACT_UNKNOWN_026, 0);
	}

	if(input & INPUT_OFF_FLOOR)
	{
		return changePlayerDropping(ACT_FREEFALL, 0);
	}

	if(++actionTimer == 13 * FRAME_RATE_SCALER_INV)
	{
		PL_DropObject(this);
		startSoundEffect(SOUND_MARIO_WAH2, MARIO_MARIO_SOUND_PLAYED);
		startSoundEffect(SOUND_ACTION_THROW, MARIO_ACTION_SOUND_PLAYED);
		SendMotorEvent(3, 50);
	}

	animated_stationary_ground_step(MARIO_ANIM_HEAVY_THROW, ACT_IDLE);
	return FALSE;
}

s32 PlayerRecord::act_stomach_slide_stop()
{
	if(input & INPUT_UNKNOWN_10)
	{
		return ChangePlayerStatus(ACT_UNKNOWN_026, 0);
	}

	if(input & INPUT_OFF_FLOOR)
	{
		return ChangePlayerStatus(ACT_FREEFALL, 0);
	}

	if(input & INPUT_ABOVE_SLIDE)
	{
		return ChangePlayerStatus(ACT_BEGIN_SLIDING, 0);
	}

	animated_stationary_ground_step(MARIO_ANIM_SLOW_LAND_FROM_DIVE, ACT_IDLE);
	return FALSE;
}

s32 PlayerRecord::act_picking_up_bowser()
{
	if(actionState == 0)
	{
		actionState		= 1;
		angleVel[1]		= 0;
		marioBodyState->grabPos = GRAB_POS_BOWSER;
		PL_TakeObject(this);
		AudStartSound(SOUND_MARIO_HRMM, marioObj->header.gfx.cameraToObject);
		SendMotorEvent(5, 80);
	}

	setAnimation(MARIO_ANIM_GRAB_BOWSER);
	if(isLast1AnimeFrame())
	{
		ChangePlayerStatus(ACT_HOLDING_BOWSER, 0);
	}

	waitProcess();
	return FALSE;
}

s32 PlayerRecord::act_holding_bowser()
{
	s16 spin;

	if(input & INPUT_B_PRESSED)
	{
		if(angleVel[1] <= -0xE00 || angleVel[1] >= 0xE00)
		{
			AudStartSound(SOUND_MARIO_SO_LONGA_BOWSER, marioObj->header.gfx.cameraToObject);
		}
		else
		{
			AudStartSound(SOUND_MARIO_HERE_WE_GO, marioObj->header.gfx.cameraToObject);
		}
		return ChangePlayerStatus(ACT_RELEASING_BOWSER, 0);
	}

	if(angleVel[1] == 0)
	{
		if(actionTimer++ > 120 * FRAME_RATE_SCALER_INV)
		{
			return ChangePlayerStatus(ACT_RELEASING_BOWSER, 1);
		}

		setAnimation(MARIO_ANIM_HOLDING_BOWSER);
	}
	else
	{
		actionTimer = 0;
		setAnimation(MARIO_ANIM_SWINGING_BOWSER);
	}

	if(intendedMag > 20.0f)
	{
		if(actionArg == 0)
		{
			actionArg = 1;
			twirlYaw  = intendedYaw;
		}
		else
		{
			// spin = acceleration
			spin = (s16)(intendedYaw - twirlYaw) / 0x80;

			if(spin < -0x80)
			{
				spin = -0x80;
			}
			if(spin > 0x80)
			{
				spin = 0x80;
			}

			twirlYaw = intendedYaw;
			angleVel[1] += spin / FRAME_RATE_SCALER_INV;

			if(angleVel[1] > 0x1000)
			{
				angleVel[1] = 0x1000;
			}
			if(angleVel[1] < -0x1000)
			{
				angleVel[1] = -0x1000;
			}
		}
	}
	else
	{
		actionArg   = 0;
		angleVel[1] = approach_s32(angleVel[1], 0, 64 / FRAME_RATE_SCALER_INV, 64 / FRAME_RATE_SCALER_INV);
	}

	// spin = starting yaw
	spin = faceAngle[1];
	faceAngle[1] += angleVel[1] / FRAME_RATE_SCALER_INV;

	// play sound on overflow
	if(angleVel[1] <= -0x100 && spin < faceAngle[1])
	{
		AudStartSound(SOUND_OBJ_BOWSER_SPINNING, marioObj->header.gfx.cameraToObject);
		SendMotorEvent(4, 20);
	}

	if(angleVel[1] >= 0x100 && spin > faceAngle[1])
	{
		AudStartSound(SOUND_OBJ_BOWSER_SPINNING, marioObj->header.gfx.cameraToObject);
		SendMotorEvent(4, 20);
	}

	waitProcess();

	if(angleVel[1] >= 0)
	{
		marioObj->header.gfx.angle[0] = -angleVel[1];
	}
	else
	{
		marioObj->header.gfx.angle[0] = angleVel[1];
	}

	return FALSE;
}

s32 PlayerRecord::act_releasing_bowser()
{
	if(++actionTimer == 1)
	{
		if(actionArg == 0)
		{
			PL_ThrowObject(this);
			SendMotorEvent(4, 50);
		}
		else
		{
			PL_DropObject(this);
			SendMotorEvent(4, 50);
		}
	}

	angleVel[1] = 0;
	animated_stationary_ground_step(MARIO_ANIM_RELEASE_BOWSER, ACT_IDLE);
	return FALSE;
}

s32 PlayerRecord::check_common_object_cancels()
{
	f32 waterSurface = waterLevel - 100;
	if(pos[1] < waterSurface)
	{
		return changeWaterMode();
	}

	if(input & INPUT_SQUISHED)
	{
		return changePlayerDropping(ACT_SQUISHED, 0);
	}

	if(health < 0x100)
	{
		return changePlayerDropping(ACT_STANDING_DEATH, 0);
	}

	return FALSE;
}

s32 PlayerRecord::playerActnMain()
{
	s32 cancel;

	if(check_common_object_cancels())
	{
		return TRUE;
	}

	if(checkPlayerSinking(0.5f))
	{
		return TRUE;
	}

	/* clang-format off */
	switch(status)
	{
		case ACT_PUNCHING:           cancel = act_punching();           break;
		case ACT_PICKING_UP:         cancel = act_picking_up();         break;
		case ACT_DIVE_PICKING_UP:    cancel = act_dive_picking_up();    break;
		case ACT_STOMACH_SLIDE_STOP: cancel = act_stomach_slide_stop(); break;
		case ACT_PLACING_DOWN:       cancel = act_placing_down();       break;
		case ACT_THROWING:           cancel = act_throwing();           break;
		case ACT_HEAVY_THROW:        cancel = act_heavy_throw();        break;
		case ACT_PICKING_UP_BOWSER:  cancel = act_picking_up_bowser();  break;
		case ACT_HOLDING_BOWSER:     cancel = act_holding_bowser();     break;
		case ACT_RELEASING_BOWSER:   cancel = act_releasing_bowser();   break;
	}
	/* clang-format on */

	if(!cancel && (input & INPUT_IN_WATER))
	{
		particleFlags |= PARTICLE_SURFACE_WAVES;
	}

	return cancel;
}
