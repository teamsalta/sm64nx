/**
 * Behavior for bhvFloorSwitchHardcodedModel, bhvFloorSwitchGrills, and
 * bhvFloorSwitchAnimatesObject.
 *
 * This controls the purple switches that Mario can step on to affect parts of
 * the environment.
 */

#include "game/motor.h"

void bhv_purple_switch_loop(void)
{
	UNUSED s32 unused;
	switch(o->oAction)
	{
		/**
		 * Set the switch's model and scale. If Mario is standing near the
		 * switch's middle section, transition to the pressed state.
		 */
		case PURPLE_SWITCH_IDLE:
			s_change_shape(MODEL_PURPLE_SWITCH);
			s_set_scale(1.5f);
			if(gMarioObject->platform == o && !(playerWorks->status & MARIO_UNKNOWN_13))
			{
				if(s_distanceXZ_obj2obj(o, gMarioObject) < 127.5)
				{
					o->oAction = PURPLE_SWITCH_PRESSED;
				}
			}
			break;
			/**
			 * Collapse the switch downward, play a sound, and shake the screen.
			 * Immediately transition to the ticking state.
			 */
		case PURPLE_SWITCH_PRESSED:
			s_scale_timezoom(2, 3 * FRAME_RATE_SCALER_INV, 1.5f, 0.2f);

			if(o->oTimer == 3 * FRAME_RATE_SCALER_INV)
			{
				objsound(SOUND_GENERAL2_PURPLE_SWITCH);
				o->oAction = PURPLE_SWITCH_TICKING;
				s_call_Viewshake(SHAKE_POS_SMALL);
				SendMotorEvent(5, 80);
			}
			break;
			/**
			 * Play a continuous ticking sound that gets faster when time is almost
			 * up. When time is up, move to a waiting-while-pressed state.
			 */
		case PURPLE_SWITCH_TICKING:
			if(o->oBehParams2ndByte != 0)
			{
				if(o->oBehParams2ndByte == 1 && gMarioObject->platform != o)
				{
					o->oAction++;
				}
				else
				{
					if(o->oTimer < 360 * FRAME_RATE_SCALER_INV)
					{
						AudStartSound(SOUND_GENERAL2_SWITCH_TICK_FAST, gDefaultSoundArgs);
					}
					else
					{
						AudStartSound(SOUND_GENERAL2_SWITCH_TICK_SLOW, gDefaultSoundArgs);
					}

					if(o->oTimer > 400 * FRAME_RATE_SCALER_INV)
					{
						o->oAction = PURPLE_SWITCH_WAIT_FOR_MARIO_TO_GET_OFF;
					}
				}
			}
			break;
			/**
			 * Make the switch look unpressed again, and transition back to the
			 * idle state.
			 */
		case PURPLE_SWITCH_UNPRESSED:
			s_scale_timezoom(2, 3 * FRAME_RATE_SCALER_INV, 0.2f, 1.5f);

			if(o->oTimer == 3 * FRAME_RATE_SCALER_INV)
			{
				o->oAction = PURPLE_SWITCH_IDLE;
			}
			break;
			/**
			 * Mario is standing on the switch, but time has expired. Wait for
			 * him to get off the switch, and when he does so, transition to the
			 * unpressed state.
			 */
		case PURPLE_SWITCH_WAIT_FOR_MARIO_TO_GET_OFF:
			if(!s_rideon_player())
			{
				o->oAction = PURPLE_SWITCH_UNPRESSED;
			}
			break;
	}
}
