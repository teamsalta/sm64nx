// chuckya.c.inc

void func_802A8D18(f32 sp28, f32 sp2C, s32 sp30)
{
	switch(o->parentObj->oChuckyaUnk88)
	{
		case 0:
			break;
		case 1:
			s_copy_mapwork(gMarioObject, o);
			break;
		case 2:
			gMarioObject->oInteractStatus |= (sp30 + INT_STATUS_MARIO_UNK2);
			playerWorks->forwardVel	    = sp28;
			playerWorks->vel[1]	    = sp2C;
			o->parentObj->oChuckyaUnk88 = 0;
			break;
		case 3:
			gMarioObject->oInteractStatus |= (INT_STATUS_MARIO_UNK2 + INT_STATUS_MARIO_UNK6); // loads 2 interactions at once?
			playerWorks->forwardVel	    = 10.0f;
			playerWorks->vel[1]	    = 10.0f;
			o->parentObj->oChuckyaUnk88 = 0;
			break;
	}
	o->oMoveAngleYaw = o->parentObj->oMoveAngleYaw;
	if(!o->parentObj->activeFlags)
		s_remove_obj(o);
}

void bhv_chuckya_anchor_mario_loop(void)
{
	func_802A8D18(40.0f, 40.0f, 64);
}

s32 Unknown802A8EC8(s32 sp20, f32 sp24, f32 sp28, s32 sp2C)
{
	s32 sp1C = 0;
	if(o->oChuckyaUnkF8 != 4)
	{
		if(sp24 < s_calc_playerscope())
		{
			if(s_calc_enemyscope() < 200.0f)
				sp1C = 0;
			else
			{
				sp1C		 = 1;
				o->oAngleToMario = s_calc_returnangle();
			}
		}
		else if(o->oDistanceToMario > sp28)
		{
			if((frameCounter / FRAME_RATE_SCALER_INV) % (s16)sp2C == 0)
			{
				o->oAngleToMario = s_calc_targetangle(o, gMarioObject);
			}

			sp1C = 2;
		}
		else
			sp1C = 3;

		if(sp20 && func_802C5A64(&o->oAngleToMario))
		{
			sp1C		 = 4;
			o->oChuckyaUnkF8 = 4;
		}
	}
	else
		sp1C = 4;
	return sp1C;
}

s32 lerp(f32* arr, f32 target, f32 dist)
{
	s32 sp4 = 0;
	if(arr[0] > target)
	{
		arr[0] -= dist;

		if(arr[0] < target)
			arr[0] = target;
	}
	else if(arr[0] < target)
	{
		arr[0] += dist;

		if(arr[0] > target)
			arr[0] = target;
	}
	else
	{
		sp4 = 1;
	}
	return sp4;
}

void ActionChuckya0(void)
{
	s32 sp3C;
	UNUSED u8 pad[16];
	s32 sp28;
	if(o->oTimer == 0)
		o->oChuckyaUnkFC = 0;
	o->oAngleToMario = s_calc_targetangle(o, gMarioObject);
	switch(sp28 = o->oSubAction)
	{
		case 0:
			o->oForwardVel = 0;
			if(s_calc_playerscope() < 2000.0f)
			{
				s_chase_angleY(o->oAngleToMario, 0x400 / FRAME_RATE_SCALER_INV);

				if(o->oChuckyaUnkFC > 40 || s_calc_dangle(o->oMoveAngleYaw, o->oAngleToMario) < 0x1000)
					o->oSubAction = 1;
			}
			else
				o->oSubAction = 3;
			break;
		case 1:
			lerp(&o->oForwardVel, 30.0f, 4.0f * FRAME_RATE_SCALER);

			if(s_calc_dangle(o->oMoveAngleYaw, o->oAngleToMario) > 0x4000)
				o->oSubAction = 2;

			if(s_calc_playerscope() > 2000.0f)
				o->oSubAction = 3;

			break;
		case 2:
			lerp(&o->oForwardVel, 0, 4.0f * FRAME_RATE_SCALER);

			if(o->oChuckyaUnkFC > 48)
				o->oSubAction = 0;
			break;
		case 3:
			if(s_calc_enemyscope() < 500.0f)
				o->oForwardVel = 0;
			else
			{
				lerp(&o->oForwardVel, 10.0f, 4.0f * FRAME_RATE_SCALER);
				o->oAngleToMario = s_calc_returnangle();
				s_chase_angleY(o->oAngleToMario, 0x800 / FRAME_RATE_SCALER_INV);
			}
			if(s_calc_playerscope() < 1900.0f)
				o->oSubAction = 0;
			break;
	}
	if(o->oSubAction != sp28)
		o->oChuckyaUnkFC = 0;
	else
		o->oChuckyaUnkFC++;
	s_set_skelanimeNo(4);
	if(o->oForwardVel > 1.0f)
		objsound_level(SOUND_AIR_CHUCKYA_MOVE);
}

void ActionChuckya1(void)
{
	if(o->oSubAction == 0)
	{
		if(s_setanime_endcheck(0))
			o->oSubAction++;
		o->oChuckyaUnkFC  = Randomf() * 30.0f + 10.0f;
		o->oChuckyaUnk100 = 0;
		o->oForwardVel	  = 0.0f;
	}
	else
	{
		if(o->oSubAction == 1)
		{
			o->oChuckyaUnk100 += key_gacha();

			if(o->oChuckyaUnk100 > 10)
			{
				o->oChuckyaUnk88 = 3;
				o->oAction	 = 3;
				o->oInteractStatus &= ~(INT_STATUS_GRABBED_MARIO);
			}
			else
			{
				s_set_skelanimeNo(1);
				o->oMoveAngleYaw += INT_STATUS_GRABBED_MARIO;

				if(o->oChuckyaUnkFC-- < 0)
					if(func_802B0C54(50.0f, 150.0f) || o->oChuckyaUnkFC < -16)
					{
						o->oSubAction++;
						;
					}
			}
		}
		else
		{
			s_set_skelanimeNo(3);
			if(s_check_animenumber(18))
			{
				objsound(SOUND_OBJ_UNKNOWN4);
				o->oChuckyaUnk88 = 2;
				o->oAction	 = 3;
				o->oInteractStatus &= ~(INT_STATUS_GRABBED_MARIO);
			}
		}
	}
}

void ActionChuckya3(void)
{
	o->oForwardVel = 0;
	o->oVelY       = 0;
	s_set_skelanimeNo(4);
	if(o->oTimer > 100 * FRAME_RATE_SCALER_INV)
		o->oAction = 0;
}

void ActionChuckya2(void)
{
	if(o->oMoveFlags & (0x200 | 0x40 | 0x20 | 0x10 | 0x8 | 0x1))
	{
		s_remove_obj(o);
		s_makecoin(o, 5, 20.0f);
		s_kemuri_sound(SOUND_OBJ_CHUCKYA_DEATH);
	}
}

void (*sChuckyaActions[])(void) = {ActionChuckya0, ActionChuckya1, ActionChuckya2, ActionChuckya3};

void func_802A97B8(void)
{
	s_enemybgcheck();
	s_modejmp(sChuckyaActions);
	s_enemymove(-30);
	if(o->oInteractStatus & INT_STATUS_GRABBED_MARIO)
	{
		o->oAction	 = 1;
		o->oChuckyaUnk88 = 1;
		objsound(SOUND_OBJ_UNKNOWN3);
	}
}

void bhv_chuckya_loop(void)
{
	f32 sp2C = 20.0f;
	f32 sp28 = 50.0f;
	s_set_scale(2.0f);
	o->oInteractionSubtype |= INT_SUBTYPE_GRABS_MARIO;
	switch(o->oHeldState)
	{
		case HELD_FREE:
			func_802A97B8();
			break;
		case HELD_HELD:
			s_mode_catch(2, 0);
			break;
		case HELD_THROWN:
		case HELD_DROPPED:
			s_mode_throw(sp2C, sp28, 2);
			break;
	}
	o->oInteractStatus = 0;
}
