// king_bobomb.c.inc

void bhv_bobomb_anchor_mario_loop(void)
{
	func_802A8D18(50.0f, 50.0f, 64);
}

void ActionKingBobomb0(void)
{
	o->oForwardVel = 0;
	o->oVelY       = 0;
	if(o->oSubAction == 0)
	{
		s_hitOFF();
		gSecondCameraFocus = o;
		s_set_skelanimeNo(5);
		s_copy_initpos();
		o->oHealth = 3;
		if(s_hitcheck_message(500.0f, 100.0f))
		{
			o->oSubAction++;
			Na_SeqVolMute(0, 60, 40);
		}
	}
	else if(s_call_talkdemo(2, 1, CUTSCENE_DIALOG, DIALOG_017))
	{
		o->oAction = 2;
		o->oFlags |= OBJ_FLAG_HOLDABLE;
	}
}

int func_802A6AF8(f32 arg0)
{
	if(arg0 < o->oPosY - gMarioObject->oPosY)
		return 1;
	else
		return 0;
}

void ActionKingBobomb2(void)
{
	s_hitON();
	if(o->oPosY - o->oHomeY < -100.0f)
	{ // Thrown off hill
		o->oAction = 5;
		s_hitOFF();
	}
	if(o->oKingBobombUnk100 == 0)
	{
		if(s_check_animenumber(15))
			s_call_Viewshake(SHAKE_POS_SMALL);
		if(s_setanime_endcheck(4))
			o->oKingBobombUnk100++;
	}
	else
	{
		if(o->oKingBobombUnk100 == 1)
		{
			s_setanime_start(11, 7);
			o->oKingBobombUnk100 = 2;
		}
		else
			s_set_skelanimeNo(11);
		if(o->oKingBobombUnk108 == 0)
		{
			o->oForwardVel = 3.0f;
			s_chase_angleY(o->oAngleToMario, 0x100 / FRAME_RATE_SCALER_INV);
		}
		else
		{
			o->oForwardVel = 0.0f;
			o->oKingBobombUnk108--;
		}
	}
	if(s_check_playercarry())
		o->oAction = 3;
	if(func_802A6AF8(1200.0f))
	{
		o->oAction = 0;
		stop_background_music(SEQUENCE_ARGS(4, SEQ_EVENT_BOSS));
	}
}

void ActionKingBobomb3(void)
{
	if(o->oSubAction == 0)
	{
		o->oForwardVel	     = 0;
		o->oKingBobombUnk104 = 0;
		o->oKingBobombUnkFC  = 0;
		if(o->oTimer == 0)
			objsound(SOUND_OBJ_UNKNOWN3);
		if(s_setanime_endcheck(0))
		{
			o->oSubAction++;
			s_setanime_start(1, 0);
		}
	}
	else
	{
		if(o->oSubAction == 1)
		{
			s_set_skelanimeNo(1);
			o->oKingBobombUnkFC += key_gacha();

			if(o->oKingBobombUnkFC > 10)
			{
				o->oKingBobombUnk88  = 3;
				o->oAction	     = 2;
				o->oKingBobombUnk108 = 35;
				o->oInteractStatus &= ~(INT_STATUS_GRABBED_MARIO);
			}
			else
			{
				o->oForwardVel = 3.0f;
				if(o->oKingBobombUnk104 > 20 && s_chase_angleY(0, 0x400 / FRAME_RATE_SCALER_INV))
				{
					o->oSubAction++;
					s_setanime_start(9, 22);
				}
			}
			o->oKingBobombUnk104++;
		}
		else
		{
			s_set_skelanimeNo(9);
			if(s_check_animenumber(31))
			{
				o->oKingBobombUnk88 = 2;
				objsound(SOUND_OBJ_UNKNOWN4);
			}
			else if(s_check_animeend())
			{
				o->oAction = 1;
				o->oInteractStatus &= ~(INT_STATUS_GRABBED_MARIO);
			}
		}
	}
}

void ActionKingBobomb1(void)
{
	o->oForwardVel = 0;
	o->oVelY       = 0;
	s_set_skelanimeNo(11);
	o->oMoveAngleYaw = s_chase_angle(o->oMoveAngleYaw, o->oAngleToMario, 512 / FRAME_RATE_SCALER_INV);
	if(o->oDistanceToMario < 2500.0f)
		o->oAction = 2;
	if(func_802A6AF8(1200.0f))
	{
		o->oAction = 0;
		stop_background_music(SEQUENCE_ARGS(4, SEQ_EVENT_BOSS));
	}
}

void ActionKingBobomb6(void)
{
	if(o->oSubAction == 0)
	{
		if(o->oTimer == 0)
		{
			o->oKingBobombUnk104 = 0;
			objsound(SOUND_OBJ_KING_BOBOMB);
			objsound(SOUND_OBJ2_KING_BOBOMB_DAMAGE);
			s_call_Viewshake(SHAKE_POS_SMALL);
			s_burneffect(0, 0, 100.0f);
			o->oInteractType = 8;
			s_hitON();
		}
		if(s_setanime_endcheck(2))
			o->oKingBobombUnk104++;
		if(o->oKingBobombUnk104 > 3)
		{
			o->oSubAction++;
			; // Needed to match
		}
	}
	else
	{
		if(o->oSubAction == 1)
		{
			if(s_setanime_endcheck(10))
			{
				o->oSubAction++;
				o->oInteractType = 2;
				s_hitOFF();
			}
		}
		else
		{
			s_set_skelanimeNo(11);
			if(s_chase_angleY(o->oAngleToMario, 0x800 / FRAME_RATE_SCALER_INV) == 1)
				o->oAction = 2;
		}
	}
}

void ActionKingBobomb7(void)
{
	s_set_skelanimeNo(2);
	if(s_call_talkdemo(2, 2, CUTSCENE_DIALOG, DIALOG_116))
	{
		obj_remove_sound(SOUND_OBJ_KING_WHOMP_DEATH);
		s_shape_hide();
		s_hitOFF();
		s_burneffect(0, 0, 200.0f);
		s_boxeffect(20, 138, 3.0f, 4);
		s_call_Viewshake(SHAKE_POS_SMALL);
		sv2_enemyset_star(2000.0f, 4500.0f, -4500.0f, 200.0f);
		o->oAction = 8;
	}
}

void ActionKingBobomb8(void)
{
	if(o->oTimer == 60 * FRAME_RATE_SCALER_INV)
		stop_background_music(SEQUENCE_ARGS(4, SEQ_EVENT_BOSS));
}

void ActionKingBobomb4()
{ // bobomb been thrown
	if(o->oPosY - o->oHomeY > -100.0f)
	{ // not thrown off hill
		if(o->oMoveFlags & 1)
		{
			o->oHealth--;
			o->oForwardVel = 0;
			o->oVelY       = 0;
			objsound(SOUND_OBJ_KING_BOBOMB);
			if(o->oHealth)
				o->oAction = 6;
			else
				o->oAction = 7;
		}
	}
	else
	{
		if(o->oSubAction == 0)
		{
			if(o->oMoveFlags & 2)
			{
				o->oForwardVel = 0;
				o->oVelY       = 0;
				o->oSubAction++;
			}
			else if(o->oMoveFlags & 1)
				objsound(SOUND_OBJ_KING_BOBOMB);
		}
		else
		{
			if(s_setanime_endcheck(10))
				o->oAction = 5; // Go back to top of hill
			o->oSubAction++;
		}
	}
}

void ActionKingBobomb5()
{ // bobomb returns home
	switch(o->oSubAction)
	{
		case 0:
			if(o->oTimer == 0)
				objsound(SOUND_OBJ_KING_BOBOMB_JUMP);
			o->oKingBobombUnkF8 = 1;
			s_setanime_endstop(8);
			o->oMoveAngleYaw = s_calc_returnangle();
			if(o->oPosY < o->oHomeY)
				o->oVelY = 100.0f;
			else
			{
				func_802B2894(&o->oHomeX, &o->oPosX, 100.0f, -4.0f);
				o->oSubAction++;
			}
			break;
		case 1:
			s_setanime_endstop(8);
			if(o->oVelY < 0 && o->oPosY < o->oHomeY)
			{
				o->oPosY	    = o->oHomeY;
				o->oVelY	    = 0;
				o->oForwardVel	    = 0;
				o->oGravity	    = -4.0f;
				o->oKingBobombUnkF8 = 0;
				s_set_skelanimeNo(7);
				objsound(SOUND_OBJ_KING_BOBOMB);
				s_call_Viewshake(SHAKE_POS_SMALL);
				o->oSubAction++;
			}
			break;
		case 2:
			if(s_setanime_endcheck(7))
				o->oSubAction++;
			break;
		case 3:
			if(func_802A6AF8(1200.0f))
			{
				o->oAction = 0;
				stop_background_music(SEQUENCE_ARGS(4, SEQ_EVENT_BOSS));
			}
			if(s_hitcheck_message(500.0f, 100.0f))
				o->oSubAction++;
			break;
		case 4:
			if(s_call_talkdemo(2, 1, CUTSCENE_DIALOG, DIALOG_128))
				o->oAction = 2;
			break;
	}
}

void (*sKingBobombActions[])(void) = {
    ActionKingBobomb0, ActionKingBobomb1, ActionKingBobomb2, ActionKingBobomb3, ActionKingBobomb4, ActionKingBobomb5, ActionKingBobomb6, ActionKingBobomb7, ActionKingBobomb8,
};
struct SoundState sKingBobombSoundStates[] = {
    {0, 0, 0, NO_SOUND},
    {1, 1, 20, SOUND_OBJ_POUNDING1_HIGHPRIO},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {1, 15, -1, SOUND_OBJ_POUNDING1_HIGHPRIO},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {1, 33, -1, SOUND_OBJ_POUNDING1_HIGHPRIO},
    {0, 0, 0, NO_SOUND},
    {1, 1, 15, SOUND_OBJ_POUNDING1_HIGHPRIO},
};

void func_802A7748(void)
{
	s_enemybgcheck();
	if(o->oKingBobombUnkF8 == 0)
		s_enemymove(-78);
	else
		s_optionmove_F();
	s_modejmp(sKingBobombActions);
	s_enemysound(sKingBobombSoundStates);

	if(!sm64::config().camera().disableDistanceClip())
	{
		if(o->oDistanceToMario < 5000.0f)
			s_shapeON();
		else
			s_shapeOFF();
	}
	else
	{
		s_shapeON();
	}
}

void bhv_king_bobomb_loop(void)
{
	f32 sp34 = 20.0f;
	f32 sp30 = 50.0f;
	UNUSED u8 pad[8];
	o->oInteractionSubtype |= INT_SUBTYPE_GRABS_MARIO;
	switch(o->oHeldState)
	{
		case HELD_FREE:
			func_802A7748();
			break;
		case HELD_HELD:
			s_mode_catch(6, 1);
			break;
		case HELD_THROWN:
		case HELD_DROPPED:
			s_mode_throw(sp34, sp30, 4);
			s_hitOFF();
			o->oPosY += 20.0f * FRAME_RATE_SCALER;
			break;
	}
	o->oInteractStatus = 0;
}
