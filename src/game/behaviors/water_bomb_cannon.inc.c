// water_bomb_cannon.inc.c

void bhv_bubble_cannon_barrel_loop(void)
{
	struct Object* val04;

	if(o->parentObj->oAction == 2)
	{
		s_remove_obj(o);
	}
	else
	{
		o->oMoveAngleYaw   = o->parentObj->oFaceAngleYaw;
		o->oMoveAnglePitch = o->parentObj->oMoveAnglePitch + 0x4000;
		o->oFaceAnglePitch = o->parentObj->oMoveAnglePitch;

		if((o->oCannonBarrelBubblesUnkF4 += o->oForwardVel * FRAME_RATE_SCALER) > 0.0f)
		{
			func_802A2A38();
			obj_forward_vel_approach(-5.0f, 18.0f * FRAME_RATE_SCALER); // bubble fx
		}
		else
		{
			o->oCannonBarrelBubblesUnkF4 = 0.0f;
			copy_object_pos(o, o->parentObj);

			// fire
			if(o->parentObj->oWaterCannonUnkF4 != 0)
			{
				if(o->oForwardVel == 0.0f)
				{
					o->oForwardVel = 35.0f;

					val04 = s_makeobj_nowpos(o, MODEL_WATER_BOMB, sm64::bhv::bhvWaterBomb());
					if(val04 != NULL)
					{
						val04->oForwardVel	   = -100.0f;
						val04->header.gfx.scale[1] = 1.7f;
					}

					set_camera_shake_from_point(SHAKE_POS_MEDIUM, o->oPosX, o->oPosY, o->oPosZ);
				}
			}
			else
			{
				o->oForwardVel = 0.0f;
			}
		}
	}
}

void water_bomb_cannon_act_0(void)
{
	if(o->oDistanceToMario < 2000.0f && !sm64::config().camera().disableDistanceClip())
	{
		s_makeobj_nowpos(o, MODEL_CANNON_BARREL, sm64::bhv::bhvCannonBarrelBubbles());
		s_shape_disp();

		o->oAction	   = 1;
		o->oMoveAnglePitch = o->oWaterCannonUnkFC = 0x1C00;
	}
}

void water_bomb_cannon_act_1(void)
{
	if(o->oDistanceToMario > 2500.0f && !sm64::config().camera().disableDistanceClip())
	{
		o->oAction = 2;
	}
	else if(o->oBehParams2ndByte == 0)
	{
		if(o->oWaterCannonUnkF4 != 0)
		{
			o->oWaterCannonUnkF4 -= 1;
		}
		else
		{
			obj_move_pitch_approach(o->oWaterCannonUnkFC, 0x80 / FRAME_RATE_SCALER_INV);
			obj_face_yaw_approach(o->oWaterCannonUnk100, 0x100 / FRAME_RATE_SCALER_INV);

			if((s16)o->oFaceAngleYaw == (s16)o->oWaterCannonUnk100)
			{
				if(o->oWaterCannonUnkF8 != 0)
				{
					o->oWaterCannonUnkF8 -= 1;
				}
				else
				{
					objsound(SOUND_OBJ_CANNON4);
					o->oWaterCannonUnkF4  = 70;
					o->oWaterCannonUnkFC  = 0x1000 + 0x400 * (RandomU16() & 0x3);
					o->oWaterCannonUnk100 = -0x2000 + o->oMoveAngleYaw + 0x1000 * (RandomU16() % 5);
					o->oWaterCannonUnkF8  = 60;
				}
			}
		}
	}
}

void water_bomb_cannon_act_2(void)
{
	s_shape_hide();
	o->oAction = 0;
}

void bhv_water_bomb_cannon_loop(void)
{
	s_player_slideout_RH(220.0f, 300.0f);

	switch(o->oAction)
	{
		case 0:
			water_bomb_cannon_act_0();
			break;
		case 1:
			water_bomb_cannon_act_1();
			break;
		case 2:
			water_bomb_cannon_act_2();
			break;
	}
}
