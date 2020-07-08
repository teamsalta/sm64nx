// bouncing_fireball.c.inc

void bhv_bouncing_fireball_flame_init(void)
{
}

void bhv_bouncing_fireball_flame_loop(void)
{
	o->activeFlags |= 0x400;
	obj_update_floor_and_walls();
	switch(o->oAction)
	{
		case 0:
			if(o->oTimer == 0)
			{
				o->oAnimState = RandomFloat() * 10.0f;
				o->oVelY      = 30.0f;
			}
			if(o->oMoveFlags & 1)
				o->oAction++;
			break;
		case 1:
			if(o->oTimer == 0)
			{
				o->oVelY       = o->oBouncingFireBallFlameSpeedY ? o->oBouncingFireBallFlameSpeedY : 50.0f;
				o->oForwardVel = o->oBouncingFireBallFlameSpeedX ? o->oBouncingFireBallFlameSpeedX : 30.0f;
			}

			if(o->oBouncingFireBallFlameDuration)
			{
				auto aMario = angle_to_object(gCurrentObject, gMarioObject);
				s16 dist    = abs_angle_diff(aMario, o->oMoveAngleYaw);

				if(dist < DEGREES(45.0f))
				{
					obj_rotate_yaw_toward(aMario, 0x100 / FRAME_RATE_SCALER_INV);
				}
			}

			if(o->oMoveFlags & (0x40 | 0x10 | 0x2) && o->oTimer > 100 * FRAME_RATE_SCALER_INV)
			{
				mark_object_for_deletion(o);
			}

			break;
	}

	if(o->oTimer > (o->oBouncingFireBallFlameDuration ? o->oBouncingFireBallFlameDuration : 300) * FRAME_RATE_SCALER_INV)
	{
		mark_object_for_deletion(o);
	}

	obj_move_standard(78);
	o->oInteractStatus = 0;
}

void bhv_bouncing_fireball_loop(void)
{
	struct Object* sp2C;
	f32 sp28;
	switch(o->oAction)
	{
		case 0:
			if(o->oDistanceToMario < 2000.0f)
			{
				o->oAction = 1;
			}
			break;
		case 1:
			sp2C = spawn_object(o, MODEL_RED_FLAME, sm64::bhv::bhvBouncingFireballFlame());
			sp28 = (10 * FRAME_RATE_SCALER_INV - o->oTimer) * 0.5 * FRAME_RATE_SCALER;
			scale_object_xyz(sp2C, sp28, sp28, sp28);

			if(o->oTimer == 0)
			{
				make_object_tangible(sp2C);
			}

			if(o->oTimer > 10 * FRAME_RATE_SCALER_INV)
			{
				o->oAction++;
			}
			break;
		case 2:
			if(o->oTimer == 0)
			{
				o->oBouncingFireBallUnkF4 = RandomFloat() * 100.0f * FRAME_RATE_SCALER_INV;
			}

			if(o->oBouncingFireBallUnkF4 + 100 * FRAME_RATE_SCALER_INV < o->oTimer)
			{
				o->oAction = 0;
			}

			break;
	}
}
