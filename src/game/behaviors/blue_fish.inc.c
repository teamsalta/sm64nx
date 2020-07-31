// blue_fish.c.inc

void bhv_blue_fish_loop(void)
{
	if(frameCounter % FRAME_RATE_SCALER_INV == 0)
	{
		// return;
	}
	f32 sp24;
	switch(o->oAction)
	{
		case 0:
			s_set_skelanime_speed(0, 1.0f);
			if(o->oTimer == 0)
			{
				o->oBlueFishUnk100 = RandomSign() << 11;
				o->oBlueFishUnkF4  = Randomf() * 2;
				o->oBlueFishUnkF8  = (s32)(Randomf() * 30) & 0xFE;

				sp24 = Randomf() * 5;

				if(sp24 < 2.0f)
					o->oAngleVelPitch = s_random_f(128) / FRAME_RATE_SCALER_INV;
				else
					o->oAngleVelPitch = 0;
			}

			o->oForwardVel = o->oBlueFishUnkF4 + 3.0f;

			if(o->oTimer >= (o->oBlueFishUnkF8 + 60))
				o->oAction++;

			if(o->oTimer < ((o->oBlueFishUnkF8 + 60) / 2))
			{
				o->oFaceAnglePitch += o->oAngleVelPitch;
			}
			else
			{
				o->oFaceAnglePitch -= o->oAngleVelPitch;
			}

			o->oVelY = -sins(o->oFaceAnglePitch) * o->oForwardVel;
			break;
		case 1:
			s_set_skelanime_speed(0, 2.0f);

			o->oMoveAngleYaw = (s32)(o->oBlueFishUnk100 + o->oMoveAngleYaw);

			if(o->oTimer == 15)
			{
				o->oAction++;
			}
			break;
		case 2:
			s_set_skelanime_speed(0, 1.0f);
			if(o->oTimer >= (o->oBlueFishUnkF8 + 60))
			{
				o->oAction++;
			}

			if(o->oTimer < ((o->oBlueFishUnkF8 + 60) / 2))
			{
				o->oFaceAnglePitch -= o->oAngleVelPitch;
			}
			else
			{
				o->oFaceAnglePitch += o->oAngleVelPitch;
			}
			break;
		case 3:
			s_set_skelanime_speed(0, 2.0f);

			o->oMoveAngleYaw = (s32)(o->oBlueFishUnk100 + o->oMoveAngleYaw);

			if(o->oTimer == 15)
			{
				o->oAction = 0;
			}
			break;
	}
	o->oVelY = -sins(o->oFaceAnglePitch) * o->oForwardVel;
	s_optionmove_F();
	if(o->parentObj->oAction == 2)
		s_remove_obj(o);
}

void bhv_tank_fish_group_loop(void)
{
	struct Object* fish;
	s32 i;
	switch(o->oAction)
	{
		case 0:
			if(gMarioCurrentRoom == 15 || gMarioCurrentRoom == 7)
			{
				for(i = 0; i < 15; i++)
				{
					fish = s_makeobj_chain(0, 300, 0, -200, o, MODEL_FISH, sm64::bhv::bhvBlueFish());
					s_random_XYZ_offset(fish, 200.0f);
				}
				o->oAction++;
			}
			break;
		case 1:
			if(gMarioCurrentRoom != 15 && gMarioCurrentRoom != 7)
				o->oAction++;
			break;
		case 2:
			o->oAction = 0;
			break;
	}
}
