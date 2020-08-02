// whomp.c.inc

void wallman_sound(void)
{
	UNUSED s32 sp2C = o->header.gfx.unk38.frame();
	s32 sp28	= 0;
	if(o->oForwardVel < 5.0f)
	{
		sp28 = s_check_animenumber(0);
		sp28 |= s_check_animenumber(23);
	}
	else
	{
		sp28 = s_check_animenumber_speed(0, 3);
		sp28 |= s_check_animenumber_speed(23, 3);
	}
	if(sp28)
		objsound(SOUND_OBJ_POUNDING1);
}

void wallman_init(void)
{
	s_set_skelanime_speed(0, 1.0f);
	s_copy_initpos();
	if(o->oBehParams2ndByte != 0)
	{
		gSecondCameraFocus = o;
		s_set_scale(2.0f);
		if(o->oSubAction == 0)
		{
			if(o->oDistanceToMario < 600.0f)
			{
				o->oSubAction++;
				Na_SeqVolMute(0, 60, 40);
			}
			else
			{
				s_copy_initpos();
				o->oHealth = 3;
			}
		}
		else if(s_call_talkdemo(2, 1, CUTSCENE_DIALOG, DIALOG_114))
			o->oAction = 2;
	}
	else if(o->oDistanceToMario < 500.0f)
		o->oAction = 1;
	wallman_sound();
}

void wallman_walkstart(void)
{
	if(o->oSubAction == 0)
	{
		o->oForwardVel = 0.0f;
		s_set_skelanime_speed(0, 1.0f);
		if(o->oTimer > 31 * FRAME_RATE_SCALER_INV)
			o->oSubAction++;
		else
			o->oMoveAngleYaw += 0x400 / FRAME_RATE_SCALER_INV;
	}
	else
	{
		o->oForwardVel = 3.0f;
		if(o->oTimer > 42 * FRAME_RATE_SCALER_INV)
			o->oAction = 1;
	}
	wallman_sound();
}

void wallman_walk(void)
{
	s16 sp26;
	f32 sp20;
	f32 sp1C;
	sp26 = s_calc_dangle(o->oAngleToMario, o->oMoveAngleYaw);
	sp20 = s_calc_enemyscope();
	if(activeStageNo == LEVEL_BITS)
		sp1C = 200.0f;
	else
		sp1C = 700.0f;
	s_set_skelanime_speed(0, 1.0f);
	o->oForwardVel = 3.0f;
	if(sp20 > sp1C)
		o->oAction = 7;
	else if(sp26 < 0x2000)
	{
		if(o->oDistanceToMario < 1500.0f)
		{
			o->oForwardVel = 9.0f;
			s_set_skelanime_speed(0, 3.0f);
		}
		if(o->oDistanceToMario < 300.0f)
			o->oAction = 3;
	}
	wallman_sound();
}

void wallman_search(void)
{
	s16 sp1E;
	s_set_skelanime_speed(0, 1.0f);
	o->oForwardVel = 3.0f;
	s_chase_angleY(o->oAngleToMario, 0x200 / FRAME_RATE_SCALER_INV);
	if(o->oTimer > 30 * FRAME_RATE_SCALER_INV)
	{
		sp1E = s_calc_dangle(o->oAngleToMario, o->oMoveAngleYaw);
		if(sp1E < 0x2000)
		{
			if(o->oDistanceToMario < 1500.0f)
			{
				o->oForwardVel = 9.0f;
				s_set_skelanime_speed(0, 3.0f);
			}
			if(o->oDistanceToMario < 300.0f)
				o->oAction = 3;
		}
	}
	wallman_sound();
	if(func_802A6AF8(1000.0f))
	{
		o->oAction = 0;
		stop_background_music(SEQUENCE_ARGS(4, SEQ_EVENT_BOSS));
	}
}

void wallman_attack(void)
{
	o->oForwardVel = 0.0f;
	s_set_skelanime_speed(1, 1.0f);
	if(s_check_animeend())
		o->oAction = 4;
}

void wallman_down(void)
{
	if(o->oTimer == 0)
		o->oVelY = 40.0f;

	if(o->oTimer < 8 * FRAME_RATE_SCALER_INV)
	{
	}
	else
	{
		o->oAngleVelPitch += 0x100 / FRAME_RATE_SCALER_INV;
		o->oFaceAnglePitch += o->oAngleVelPitch / FRAME_RATE_SCALER_INV;

		if(o->oFaceAnglePitch > 0x4000)
		{
			o->oAngleVelPitch  = 0;
			o->oFaceAnglePitch = 0x4000;
			o->oAction	   = 5;
		}
	}
}

void wallman_down_end(void)
{
	if(o->oSubAction == 0 && o->oMoveFlags & 1)
	{
		objsound(SOUND_OBJ_WHOMP_LOWPRIO);
		s_call_Viewshake(SHAKE_POS_SMALL);
		o->oVelY = 0.0f;
		o->oSubAction++;
	}
	if(o->oMoveFlags & 2)
		o->oAction = 6;
}

void wallman_boss_sleep(void)
{
	Vec3f pos;
	if(o->oSubAction == 0)
	{
		if(s_checkplayer_hipaatack())
		{
			o->oHealth--;
			objsound(SOUND_OBJ2_WHOMP_SOUND_SHORT);
			objsound(SOUND_OBJ_KING_WHOMP_DEATH);
			if(o->oHealth == 0)
				o->oAction = 8;
			else
			{
				s_copy_f(pos, &o->oPosX);
				s_copy_f(&o->oPosX, &gMarioObject->oPosX);
				s_burneffect(0, 0, 100.0f);
				s_boxeffect(20, 138, 3.0f, 4);
				s_call_Viewshake(SHAKE_POS_SMALL);
				s_copy_f(&o->oPosX, pos);
			}
			o->oSubAction++;
		}
		o->oWhompUnkF8 = 0;
	}
	else
	{
		if(o->oWhompUnkF8 < 10)
		{
			if(o->oWhompUnkF8 % 2)
				o->oPosY += 8.0f * FRAME_RATE_SCALER;
			else
				o->oPosY -= 8.0f * FRAME_RATE_SCALER;
		}
		else
			o->oSubAction = 10;
		o->oWhompUnkF8++;
	}
}

void wallman_mini_sleep(void)
{
	if(o->oSubAction == 0)
	{
		if(gMarioObject->platform == o)
		{
			if(s_checkplayer_hipaatack())
			{
				o->oNumLootCoins = 5;
				s_makecoin(o, 5, 20.0f);
				o->oAction = 8;
			}
			else
			{
				s_makecoin_playerpos();
				o->oSubAction++;
			}
		}
	}
	else if(!s_rideon_player())
		o->oSubAction = 0;
}

void wallman_sleep(void)
{
	if(o->oSubAction != 10)
	{
		o->oForwardVel	  = 0.0f;
		o->oAngleVelPitch = 0;
		o->oAngleVelYaw	  = 0;
		o->oAngleVelRoll  = 0;
		if(o->oBehParams2ndByte != 0)
			wallman_boss_sleep();
		else
			wallman_mini_sleep();
		if(o->oTimer > 100 * FRAME_RATE_SCALER_INV || (marioWorks->status == ACT_SQUISHED && o->oTimer > 30 * FRAME_RATE_SCALER_INV))
			o->oSubAction = 10;
	}
	else
	{
		if(o->oFaceAnglePitch > 0)
		{
			o->oAngleVelPitch = -0x200;
			o->oFaceAnglePitch += o->oAngleVelPitch / FRAME_RATE_SCALER_INV;
		}
		else
		{
			o->oAngleVelPitch  = 0;
			o->oFaceAnglePitch = 0;
			if(o->oBehParams2ndByte != 0)
				o->oAction = 2;
			else
				o->oAction = 1;
		}
	}
}

void wallman_burn(void)
{
	if(o->oBehParams2ndByte != 0)
	{
		if(s_call_talkdemo(2, 2, CUTSCENE_DIALOG, DIALOG_115))
		{
			s_set_angle(o, 0, 0, 0);
			s_shape_hide();
			s_hitOFF();
			s_burneffect(0, 0, 200.0f);
			s_boxeffect(20, 138, 3.0f, 4);
			s_call_Viewshake(SHAKE_POS_SMALL);
			o->oPosY += 100.0f * FRAME_RATE_SCALER;
			s_enemyset_star(180.0f, 3880.0f, 340.0f);
			objsound(SOUND_OBJ_KING_WHOMP_DEATH);
			o->oAction = 9;
		}
	}
	else
	{
		s_burneffect(0, 0, 100.0f);
		s_boxeffect(20, 138, 3.0f, 4);
		s_call_Viewshake(SHAKE_POS_SMALL);
		obj_remove_sound(SOUND_OBJ_THWOMP);
		s_remove_obj(o);
	}
}

void wallman_dead_stop(void)
{
	if(o->oTimer == 60 * FRAME_RATE_SCALER_INV)
		stop_background_music(SEQUENCE_ARGS(4, SEQ_EVENT_BOSS));
}

void (*sWhompActions[])(void) = {wallman_init, wallman_walk, wallman_search, wallman_attack, wallman_down, wallman_down_end, wallman_sleep, wallman_walkstart, wallman_burn, wallman_dead_stop};

// MM
void s_wallman(void)
{
	s_enemybgcheck();
	s_modejmp(sWhompActions);
	s_enemymove(-20);

	if(o->oAction != 9)
	{
		if(!sm64::config().camera().disableDistanceClip())
		{
			if(o->oBehParams2ndByte != 0)
				s_erase_height(2000.0f);
			else
				s_erase_height(1000.0f);
		}
		stMainMoveBG();
	}
}
