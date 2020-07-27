
/**
 * Behavior for bhvSwoop.
 * Has a native room.
 */

/**
 * Hitbox for swoop.
 */
static struct ObjectHitbox sSwoopHitbox = {
    /* interactType:      */ INTERACT_HIT_FROM_BELOW,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 0,
    /* numLootCoins:      */ 1,
    /* radius:            */ 100,
    /* height:            */ 80,
    /* hurtboxRadius:     */ 70,
    /* hurtboxHeight:     */ 70,
};

/**
 * If necessary, grow to full size. Wait for mario to enter range, then turn
 * toward him and enter the move action.
 */
static void swoop_act_idle(void)
{
	s_set_skelanimeNo(1);

	if(approach_f32_ptr(&o->header.gfx.scale[0], 1.0f, 0.05f * FRAME_RATE_SCALER) && o->oDistanceToMario < 1500.0f)
	{
		if(s_chase_angleY(o->oAngleToMario, 800 / FRAME_RATE_SCALER_INV))
		{
			objsound(SOUND_OBJ2_SWOOP);
			o->oAction = SWOOP_ACT_MOVE;
			o->oVelY   = -12.0f;
		}
	}

	o->oFaceAngleRoll = 0x8000;
}

/**
 * Swoop downward toward mario. Stop moving downward and speed up after reaching
 * him. Return to home once mario is far away.
 */
static void swoop_act_move(void)
{
	func_8029ED98(0, 2.0f);
	if(s_check_animeend())
	{
		objsound(SOUND_OBJ_UNKNOWN6);
	}

	if(o->oForwardVel == 0.0f)
	{
		// If we haven't started moving yet, begin swooping
		if(obj_face_roll_approach(0, 2500 / FRAME_RATE_SCALER_INV))
		{
			o->oForwardVel = 10.0f;
			o->oVelY       = -10.0f;
		}
	}
	else if(obj_mario_far_away())
	{
		// If mario far away, reset
		o->oAction = SWOOP_ACT_IDLE;
		obj_set_pos_to_home();
		o->header.gfx.scale[0] = o->oForwardVel = o->oVelY = 0.0f;
		o->oFaceAngleRoll				   = 0;
	}
	else
	{
		if(o->oSwoopBonkCountdown != 0)
		{
			o->oSwoopBonkCountdown -= 1;
		}
		else if(o->oVelY != 0.0f)
		{
			// If we're not done swooping, turn toward mario. When between
			// 0 and 200 units above mario, increase speed and stop swooping
			o->oSwoopTargetYaw = o->oAngleToMario;
			if(o->oPosY < gMarioObject->oPosY + 200.0f)
			{
				if(obj_y_vel_approach(0.0f, 0.5f * FRAME_RATE_SCALER))
				{
					o->oForwardVel *= 2.0f;
				}
			}
			else
			{
				obj_y_vel_approach(-10.0f, 0.5f * FRAME_RATE_SCALER);
			}
		}
		else if(o->oMoveFlags & OBJ_MOVE_HIT_WALL)
		{
			// Bounce off a wall and don't bounce again for 30 frames.
			o->oSwoopTargetYaw     = obj_reflect_move_angle_off_wall();
			o->oSwoopBonkCountdown = 30;
		}

		// Tilt upward when approaching mario
		if((o->oSwoopTargetPitch = obj_get_pitch_from_vel()) == 0)
		{
			o->oSwoopTargetPitch += o->oForwardVel * 500.0f * FRAME_RATE_SCALER;
		}
		obj_move_pitch_approach(o->oSwoopTargetPitch, 140 / FRAME_RATE_SCALER_INV);

		// Jitter yaw a bit
		s_chase_angleY(o->oSwoopTargetYaw + (s32)(3000 * coss(4000 * (frameCounter / FRAME_RATE_SCALER_INV))), 1200);
		obj_roll_to_match_yaw_turn(o->oSwoopTargetYaw, 0x3000, 500);

		// Jitter roll a bit
		o->oFaceAngleRoll += (s32)(1000 * coss(20000 * (frameCounter / FRAME_RATE_SCALER_INV))) / FRAME_RATE_SCALER_INV;
	}
}

/**
 * Update function for swoop.
 */
void bhv_swoop_update(void)
{
	// No partial update (only appears in roomed levels)

	if(!(o->activeFlags & ACTIVE_FLAG_IN_DIFFERENT_ROOM))
	{
		o->oDeathSound = SOUND_OBJ_SWOOP_DEATH;

		s_enemybgcheck();

		switch(o->oAction)
		{
			case SWOOP_ACT_IDLE:
				swoop_act_idle();
				break;
			case SWOOP_ACT_MOVE:
				swoop_act_move();
				break;
		}

		s_set_scale(o->header.gfx.scale[0]);
		s_enemymove(78);

		obj_check_attacks(&sSwoopHitbox, o->oAction);
	}
}
