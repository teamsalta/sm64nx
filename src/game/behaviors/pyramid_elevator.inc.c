/**
 * Behaviors for sm64::bhv::bhvPyramidElevator() and sm64::bhv::bhvPyramidElevatorTrajectoryMarkerBall().
 *
 * This controls the elevator that descends Shifting Sand Land's pyramid, as
 * well as the small marker balls that demarcate its trajactory.
 */

/**
 * Generate the ten trajectory marker balls that indicate where the elevator
 * moves.
 */
void bhv_pyramid_elevator_init(void)
{
	s32 i;
	struct Object* ball;

	for(i = 0; i < 10; i++)
	{
		ball	    = s_makeobj_nowpos(o, MODEL_TRAJECTORY_MARKER_BALL, sm64::bhv::bhvPyramidElevatorTrajectoryMarkerBall());
		ball->oPosY = 4600 - i * 460;
	}
}

void bhv_pyramid_elevator_loop(void)
{
	switch(o->oAction)
	{
		/**
		 * Do not move until Mario stands on the elevator. When he does,
		 * transition to the starting state.
		 */
		case PYRAMID_ELEVATOR_IDLE:
			if(gMarioObject->platform == o)
				o->oAction = PYRAMID_ELEVATOR_START_MOVING;
			break;

		/**
		 * Use a sine wave to start the elevator's movement with a small jolt.
		 * After a certain amount of time, transition to a constant-velocity state.
		 */
		case PYRAMID_ELEVATOR_START_MOVING:
			o->oPosY = o->oHomeY - sins(o->oTimer * 0x1000 / FRAME_RATE_SCALER_INV) * 10.0f;
			if(o->oTimer == 8 * FRAME_RATE_SCALER_INV)
				o->oAction = PYRAMID_ELEVATOR_CONSTANT_VELOCITY;
			break;

		/**
		 * Move downwards with constant velocity. Once at the bottom of the
		 * track, transition to the final state.
		 */
		case PYRAMID_ELEVATOR_CONSTANT_VELOCITY:
			o->oVelY = -10.0f;
			o->oPosY += o->oVelY * FRAME_RATE_SCALER;
			if(o->oPosY < 128.0f)
			{
				o->oPosY   = 128.0f;
				o->oAction = PYRAMID_ELEVATOR_AT_BOTTOM;
			}
			break;

		/**
		 * Use a sine wave to stop the elevator's movement with a small jolt.
		 * Then, remain at the bottom of the track.
		 */
		case PYRAMID_ELEVATOR_AT_BOTTOM:
			o->oPosY = sins(o->oTimer * 0x1000 / FRAME_RATE_SCALER_INV) * 10.0f + 128.0f;
			if(o->oTimer >= 8 * FRAME_RATE_SCALER_INV)
			{
				o->oVelY = 0;
				o->oPosY = 128.0f;
			}
			break;
	}
}

/**
 * Deactivate the trajectory marker balls if the elevator is not moving.
 * Otherwise, set their scale.
 */
void bhv_pyramid_elevator_trajectory_marker_ball_loop(void)
{
	struct Object* elevator;

	s_set_scale(0.15f);
	elevator = s_find_obj(sm64::bhv::bhvPyramidElevator());

	if(elevator != NULL)
	{
		if(elevator->oAction != PYRAMID_ELEVATOR_IDLE)
		{
			o->activeFlags = 0;
		}
	}
}
