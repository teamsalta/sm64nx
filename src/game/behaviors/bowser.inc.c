// bowser.c.inc

enum 
{ 
	 mode_kopatail_nohit, 
	 mode_kopatail_wait, 
	 mode_kopatail_hit 
};

enum 
{ 
   mode_s_kopa_wait, 
   mode_s_kopa_swingstart, 
   mode_s_kopa_superjumpstart, 
   mode_s_kopa_punch, 
   mode_s_kopa_dead, 
   mode_s_kopa_demo_wait, 
   mode_s_kopa_demo_walk, 
   mode_s_kopa_dash, 
   mode_s_kopa_sky_fire, 
   mode_s_kopa_small_fire, 
   mode_s_kopa_cliff_dance, 
   mode_s_kopa_turn, 
   mode_s_kopa_safe_return, 
   mode_s_kopa_jump_atom, 
   mode_s_kopa_search_walk, 
   mode_s_kopa_normal_fire, 
   mode_s_kopa_erasemove, 
   mode_s_kopa_jump, 
   mode_s_kopa_normal_wait, 
   mode_s_kopa_bg_rotate 
};

#define	KOPADASH_START		0
#define	KOPADASH_DASH		1
#define	KOPADASH_END		2
#define	KOPADASH_SLIP		3
#define	KOPADASH_TURN		4

#define		ai_normal			0
#define		ai_search			1

void ActionBowserTailAnchor0()
{
	struct Object* bowser = o->parentObj;
	s_hitON();
	s_set_scale(1.0f);
	if(bowser->oAction == 19)
		bowser->oIntangibleTimer = -1;
	else if(s_hitcheck(o, gMarioObject))
	{
		bowser->oIntangibleTimer = 0;
		o->oAction		 = 2;
	}
	else
		bowser->oIntangibleTimer = -1;
}

void ActionBowserTailAnchor1()
{
	if(o->oTimer > 30 * FRAME_RATE_SCALER_INV)
	{
		o->oAction = 0;
	}
}

void ActionBowserTailAnchor2()
{
	if(o->parentObj->oAction == 19)
	{
		o->parentObj->oIntangibleTimer = -1;
		o->oAction		       = 0;
	}
	s_hitOFF();
}

void (*sBowserTailAnchorActions[])() = {ActionBowserTailAnchor0, ActionBowserTailAnchor1, ActionBowserTailAnchor2};

static s8 kopamode_table[] = {
	mode_s_kopa_dash,
	mode_s_kopa_sky_fire,
	mode_s_kopa_small_fire,
	mode_s_kopa_safe_return,
	mode_s_kopa_jump_atom,
	mode_s_kopa_search_walk,
	mode_s_kopa_normal_fire,
	mode_s_kopa_dead,
	mode_s_kopa_punch,
	mode_s_kopa_erasemove,
	mode_s_kopa_jump,
	mode_s_kopa_bg_rotate,
	mode_s_kopa_punch,
	mode_s_kopa_punch,
	mode_s_kopa_punch,
	mode_s_kopa_punch
};

s16 speed_Y_data[]		     = {60, 0};
s16 speed_F_data[]		     = {50, 0};
s8 punchsound_tab[]			     = {24 * FRAME_RATE_SCALER_INV, 42 * FRAME_RATE_SCALER_INV, 60 * FRAME_RATE_SCALER_INV, -1};
s16 sBowserDefeatedDialogText[3]     = {DIALOG_119, DIALOG_120, DIALOG_121};
s16 kopabg_timedata[][3]		     = {
    {1, 10, 40}, {0, 0, 74}, {-1, -10, 114}, {1, -20, 134}, {-1, 20, 154}, {1, 40, 164}, {-1, -40, 174}, {1, -80, 179}, {-1, 80, 184}, {1, 160, 186}, {-1, -160, 186}, {1, 0, 0},
};

void bhv_bowser_tail_anchor_loop()
{
	s_modejmp(sBowserTailAnchorActions);

	o->oParentRelativePosX = 90.0f;

	if(o->parentObj->oAction == 4)
		o->parentObj->oIntangibleTimer = -1;

	o->oInteractStatus = 0;
}

void bhv_bowser_flame_spawn_loop()
{
	struct Object* bowser = o->parentObj;
	s32 nextFrame;
	f32 sp2C;
	f32 sp28;
	f32 yawCos    = coss(bowser->oMoveAngleYaw);
	f32 yawSin    = sins(bowser->oMoveAngleYaw);
	s16* uknArray = (s16*)segmented_to_virtual(bowser_seg6_unkmoveshorts_060576FC);

	if(bowser->oSoundStateID == 6)
	{
		nextFrame = bowser->header.gfx.unk38.frame() + 1.0f;
		if(bowser->header.gfx.unk38.curAnim->lastFrame == nextFrame)
			nextFrame = 0;

		if(nextFrame > 45 && nextFrame < 85 && !bowser->header.gfx.unk38.isPartialFrame())
		{
			objsound_level(SOUND_AIR_BOWSER_SPIT_FIRE);
			sp2C		   = uknArray[5 * nextFrame];
			sp28		   = uknArray[5 * nextFrame + 2];
			o->oPosX	   = bowser->oPosX + (sp28 * yawSin + sp2C * yawCos);
			o->oPosY	   = bowser->oPosY + uknArray[5 * nextFrame + 1];
			o->oPosZ	   = bowser->oPosZ + (sp28 * yawCos - sp2C * yawSin);
			o->oMoveAnglePitch = uknArray[5 * nextFrame + 4] + 0xC00;
			o->oMoveAngleYaw   = uknArray[5 * nextFrame + 3] + (s16)bowser->oMoveAngleYaw;

			if(nextFrame & 1)
			{
				s_makeobj_nowpos(o, MODEL_RED_FLAME, sm64::bhv::bhvFlameMovingForwardGrowing());
			}
		}
	}
}

void bhv_bowser_body_anchor_loop()
{
	s_copy_worldXYZ_angleXYZ(o, o->parentObj);

	if(o->parentObj->oAction == 4)
	{
		if(o->parentObj->oSubAction == 11)
			o->oInteractType = 0;
		else
			o->oInteractType = 0x800000;
	}
	else
	{
		o->oInteractType = 8;
		if(o->parentObj->oOpacity < 100)
			s_hitOFF();
		else
			s_hitON();
	}
	if(o->parentObj->oHeldState != HELD_FREE)
		s_hitOFF();

	o->oInteractStatus = 0;
}

s32 make_firering()
{
	struct Object* wave;
	if(o->oBehParams2ndByte == 2)
	{
		wave	    = s_makeobj_nowpos(o, MODEL_BOWSER_WAVE, sm64::bhv::bhvBowserShockWave());
		wave->oPosY = o->oFloorHeight;
		return 1;
	}
	return 0;
}

void kopa_boundsound(s32* a)
{
	if(o->oMoveFlags & 1)
	{
		a[0]++;
		if(a[0] < 4)
		{
			s_set_camerainfo(o, CAM_EVENT_BOWSER_THROW_BOUNCE);
			s_burneffect(0, 0, 60.0f);
			objsound(NA_SE3_KUPAWALK);
		}
	}
}

#define BITDW (o->oBehParams2ndByte == 0)
#define BITFS (o->oBehParams2ndByte == 1)
#define BITS (o->oBehParams2ndByte == 2)

s32 kopa_walk_start()
{
	s_set_skelanimeNo(15);
	if(s_check_animenumber(21))
		o->oForwardVel = 3.0f;
	if(s_check_animeend())
		return 1;
	else
		return 0;
}

s32 kopa_walk_loop()
{
	o->oForwardVel = 3.0f;
	s_set_skelanimeNo(13);
	if(s_check_animeend())
		return 1;
	else
		return 0;
}

s32 kopa_walk_end()
{
	s_set_skelanimeNo(14);
	if(s_check_animenumber(20))
		o->oForwardVel = 0.0f;
	if(s_check_animeend())
		return 1;
	else
		return 0;
}

void kopa_demomode_check()
{
	if(o->oBowserUnk88 == 0)
		o->oAction = 5;
	else if(o->oBowserUnk88 == 1)
		o->oAction = 6;
	else if(o->oBehParams2ndByte == 1)
		o->oAction = 13;
	else
		o->oAction = 0;
}

void s_kopa_demo_wait() // not much
{
	o->oForwardVel = 0.0f;
	s_set_skelanimeNo(12);
	kopa_demomode_check();
}

void s_kopa_demo_walk()
{
	if(o->oSubAction == 0)
	{
		if(kopa_walk_start())
			o->oSubAction++;
	}
	else if(o->oSubAction == 1)
	{
		if(kopa_walk_loop())
			o->oSubAction++;
	}
	else if(kopa_walk_end())
	{
		if(o->oBowserUnk88 == 1)
			o->oBowserUnk88 = 0;
		kopa_demomode_check();
	}
}

static void kopa_debug_program() // size
{
	if(gDebugInfo[5][1] != 0)
	{
		o->oAction	 = kopamode_table[gDebugInfo[5][2] & 0xf];
		gDebugInfo[5][1] = 0;
	}
}

void kopa_1_program()
{
	f32 rand = Randomf();
	if(o->oBowserUnk110 == 0)
	{
		if(o->oBowserUnkF4 & 2)
		{
			if(o->oDistanceToMario < 1500.0f)
				o->oAction = mode_s_kopa_normal_fire; // nearby
			else
				o->oAction = mode_s_kopa_jump; // far away
		}
		else
			o->oAction = mode_s_kopa_search_walk;
		o->oBowserUnk110++;
	}
	else
	{
		o->oBowserUnk110 = 0;
		if(!autoDemoPtr)
		{
			if(rand < 0.1)
				o->oAction = mode_s_kopa_punch; // rare 1/10 chance
			else
				o->oAction = mode_s_kopa_search_walk; // common
		}
		else
		{
			o->oAction = mode_s_kopa_search_walk; // ensure demo starts with action 14.
		}
	}
}

void kopa_2_program()
{
	f32 rand = Randomf();
	if(o->oBowserUnk110 == 0)
	{
		if(o->oBowserUnkF4 & 2)
		{
			if(o->oDistanceToMario < 1300.0f) // nearby
			{
				if(rand < 0.5) // 50/50
					o->oAction = mode_s_kopa_erasemove;
				else
					o->oAction = mode_s_kopa_small_fire;
			}
			else // far away
			{
				o->oAction = 7;
				if(500.0f < o->oBowserDistToCentre && o->oBowserDistToCentre < 1500.0f && rand < 0.5) // away from centre and good luck
					o->oAction = mode_s_kopa_jump_atom;
			}
		}
		else
			o->oAction = mode_s_kopa_search_walk;
		o->oBowserUnk110++;
	}
	else
	{
		o->oBowserUnk110 = 0;
		o->oAction	 = mode_s_kopa_search_walk;
	}
}

void kopa_3_level_0()
{
	f32 rand = Randomf();
	if(o->oBowserUnkF4 & 2)
	{
		if(o->oDistanceToMario < 1000.0f)
		{
			if(rand < 0.4)
				o->oAction = mode_s_kopa_small_fire;
			else if(rand < 0.8)
				o->oAction = mode_s_kopa_sky_fire;
			else
				o->oAction = mode_s_kopa_normal_fire;
		}
		else if(rand < 0.5)
			o->oAction = mode_s_kopa_jump_atom;
		else
			o->oAction = mode_s_kopa_dash;
	}
	else
		o->oAction = mode_s_kopa_search_walk;
}

void kopa_3_level_1()
{
	o->oAction = mode_s_kopa_jump_atom;
}

void kopa_3_program()
{
	switch(o->oBowserUnk110)
	{
		case 0:
			if(o->oBowserUnk106 == 0)
				kopa_3_level_0();
			else
				kopa_3_level_1();
			o->oBowserUnk110 = ai_search;
			break;
		case ai_search:
			o->oBowserUnk110 = 0;
			o->oAction	 = mode_s_kopa_search_walk;
			break;
	}
}

void kopa_jump_error_check()
{
	if(o->oVelY < 0 && o->oPosY < (o->oHomeY - 300.0f))
	{
		o->oPosX = o->oPosZ = 0;
		o->oPosY	    = o->oHomeY + 2000.0f;
		o->oVelY	    = 0;
		o->oForwardVel	    = 0;
	}
}

void s_kopa_normal_wait() // size?
{
	if(s_setanime_endcheck(12))
		o->oAction = 0;
}

void s_kopa_wait() // only lasts one frame
{
	o->oBowserEyesShut = 0;
	s_set_skelanimeNo(12);
	// stop him still
	o->oAngleVelYaw = 0;
	o->oForwardVel	= 0.0f;
	o->oVelY	= 0.0f;
	if(BITDW)
		kopa_1_program();
	else if(BITFS)
		kopa_2_program();
	else
		kopa_3_program();
	// Action 14 commonly follows
}

void s_kopa_normal_fire()
{
	o->oForwardVel = 0.0f;

	if(o->oTimer == 0)
	{
		objsound(SOUND_OBJ_BOWSER_INHALING);
	}

	if(s_setanime_endcheck(6))
	{
		o->oAction = 0;
	}
}

void s_kopa_search_walk() // turn towards Mario
{
	UNUSED s32 facing; // is Bowser facing Mario?
	s16 turnSpeed;
	s16 angleFromMario = s_calc_dangle(o->oMoveAngleYaw, o->oAngleToMario);
	if(BITFS)
		turnSpeed = 0x400;
	else if(o->oHealth > 2)
		turnSpeed = 0x400;
	else if(o->oHealth == 2)
		turnSpeed = 0x300;
	else
		turnSpeed = 0x200;
	facing = s_chase_angleY(o->oAngleToMario, turnSpeed / FRAME_RATE_SCALER_INV);
	if(o->oSubAction == 0)
	{
		o->oBowserUnkF8 = 0;
		if(kopa_walk_start())
			o->oSubAction++;
	}
	else if(o->oSubAction == 1)
	{
		if(kopa_walk_loop())
		{
			o->oBowserUnkF8++;
			if(o->oBowserUnkF4 & 0x20000)
			{
				if(o->oBowserUnkF8 > 4)
					o->oBowserUnkF4 &= ~0x20000;
			}
			else if(angleFromMario < 0x2000)
				o->oSubAction++;
		}
	}
	else if(kopa_walk_end())
		o->oAction = 0;
}

void s_kopa_erasemove()
{
	switch(o->oSubAction)
	{
		case 0:
			s_hitOFF();
			o->oBowserUnk1AC = 0;
			o->oBowserUnkF8	 = 30;
			if(o->oTimer == 0)
				objsound(SOUND_OBJ2_BOWSER_TELEPORT);
			if(o->oOpacity == 0)
			{
				o->oSubAction++;
				o->oMoveAngleYaw = o->oAngleToMario;
			}
			break;
		case 1:
			if(o->oBowserUnkF8--)
				o->oForwardVel = 100.0f;
			else
			{
				o->oSubAction	 = 2;
				o->oMoveAngleYaw = o->oAngleToMario;
			}
			if(s_calc_dangle(o->oMoveAngleYaw, o->oAngleToMario) > 0x4000)
				if(o->oDistanceToMario > 500.0f)
				{
					o->oSubAction	 = 2;
					o->oMoveAngleYaw = o->oAngleToMario; // large change in angle?
					objsound(SOUND_OBJ2_BOWSER_TELEPORT);
				}
			break;
		case 2:
			o->oForwardVel	 = 0.0f;
			o->oBowserUnk1AC = 0xFF;
			if(o->oOpacity == 0xFF)
				o->oAction = 0;
			s_hitON();
			break;
	}
}

void s_kopa_sky_fire() // only in sky
{
	s32 frame;
	s_set_skelanimeNo(11);
	frame = o->header.gfx.unk38.frame();
	if(frame > 24 && frame < 36 && !o->header.gfx.unk38.isPartialFrame())
	{
		objsound_level(SOUND_AIR_BOWSER_SPIT_FIRE);
		if(frame == 35)
			s_makeobj_chain(1, 0, 0x190, 0x64, o, MODEL_RED_FLAME, sm64::bhv::bhvBlueBowserFlame());
		else
			s_makeobj_chain(0, 0, 0x190, 0x64, o, MODEL_RED_FLAME, sm64::bhv::bhvBlueBowserFlame());
	}
	if(s_check_animeend())
		o->oAction = 0;
	o->oBowserUnkF4 |= 0x20000;
}

void s_kopa_safe_return()
{
	if(o->oTimer == 0)
	{
		o->oForwardVel	   = -400.0f;
		o->oVelY	   = 100.0f;
		o->oMoveAngleYaw   = o->oBowserAngleToCentre + 0x8000;
		o->oBowserEyesShut = 1;
	}
	if(o->oSubAction == 0)
	{
		s_set_skelanimeNo(25);
		o->oSubAction++;
		o->oBowserUnkF8 = 0;
	}
	else if(o->oSubAction == 1)
	{
		s_set_skelanimeNo(25);
		s_stop_animeend();
		kopa_boundsound(&o->oBowserUnkF8);
		if((o->oBowserUnkF8 > 2))
		{
			s_set_skelanimeNo(26);
			o->oVelY       = 0.0f;
			o->oForwardVel = 0.0f;
			o->oSubAction++;
		}
	}
	else if(o->oSubAction == 2)
	{
		if(s_check_animeend())
		{
			if(o->oHealth == 1)
				o->oAction = 3;
			else
				o->oAction = 0;
			o->oBowserEyesShut = 0;
		}
	}
	else
	{
	}
}

s32 kopa_jump_check()
{
	s_set_skelanimeNo(9);
	if(s_check_animenumber(11))
		return 1;
	else
		return 0;
}

s32 kopa_jumpend_check()
{
	if(o->oMoveFlags & 1)
	{
		o->oForwardVel = 0;
		o->oVelY       = 0;
		s_burneffect(0, 0, 60.0f);
		s_set_skelanimeNo(8);
		o->header.gfx.unk38.animFrameAccelAssist = 0;
		s_set_camerainfo(o, CAM_EVENT_BOWSER_JUMP);
		if(BITDW)
		{
			if(o->oDistanceToMario < 850.0f)
				gMarioObject->oInteractStatus |= INTERACT_GRABBABLE;
			else
				gMarioObject->oInteractStatus |= INTERACT_HOOT; // hmm...
		}
		return 1;
	}
	else
		return 0;
}

void kopa_center_jump()
{
	if(BITS && o->oBowserUnkF4 & 0x10000)
		if(o->oBowserDistToCentre > 1000.0f)
			o->oForwardVel = 60.0f;
}

void s_kopa_jump_atom()
{
	UNUSED s32 unused;
	if(o->oSubAction == 0)
	{
		if(kopa_jump_check())
		{
			if(BITS && o->oBowserUnkF4 & 0x10000)
				o->oVelY = 70.0f;
			else
				o->oVelY = 80.0f;
			o->oBowserUnkF8 = 0;
			kopa_center_jump();
			o->oSubAction++;
		}
	}
	else if(o->oSubAction == 1)
	{
		if(o->oBehParams2ndByte == 2 && o->oBowserUnkF4 & 0x10000)
			kopa_jump_error_check();
		if(kopa_jumpend_check())
		{
			o->oBowserUnkF4 &= 0xfffeffff;
			o->oForwardVel = 0.0f;
			o->oSubAction++;
			make_firering();
			if(BITFS)
				o->oAction = 19;
		}
		else
		{
		}
	}
	else if(s_check_animeend())
		o->oAction = 0;
}

void s_kopa_jump()
{
	f32 speedY = speed_Y_data[0];
	f32 speedF = speed_F_data[0];
	if(o->oSubAction == 0)
	{
		if(kopa_jump_check())
		{
			o->oVelY	= speedY;
			o->oForwardVel	= speedF;
			o->oBowserUnkF8 = 0;
			o->oSubAction++;
		}
	}
	else if(o->oSubAction == 1)
	{
		if(kopa_jumpend_check())
			o->oSubAction++;
	}
	else if(s_check_animeend())
		o->oAction = 0;
}

void s_kopa_cliff_dance()
{
	o->oForwardVel = 0.0f;
	if(o->oTimer == 0)
		o->oBowserUnkF8 = 0;
	switch(o->oSubAction)
	{
		case 0:
			s_set_skelanimeNo(23);
			if(s_check_animeend())
				o->oBowserUnkF8++;
			if(o->oBowserUnkF8 > 0)
				o->oSubAction++;
			break;
		case 1:
			s_set_skelanimeNo(24);
			if(s_check_animeend())
				o->oAction = 11;
			break;
	}
}

void s_kopa_small_fire()
{
	if(playerMeter.wedges < 4)
		o->oBowserUnk108 = 3;
	else
		o->oBowserUnk108 = Randomf() * 3.0f + 1.0f;
	s_set_skelanimeNo(22);

	if(s_check_animenumber(5))
	{
		i_set_fireball(0, 200, 180, 7.0f, MODEL_RED_FLAME, 30.0f, 10.0f, 0x1000);
	}

	if(s_check_animeend())
		o->oSubAction++;

	if(o->oSubAction >= o->oBowserUnk108)
		o->oAction = 0;
}

s32 kopaturn(s32 a0, s16 a1)
{
	if(o->oSubAction == 0)
	{
		if(s_setanime_endcheck(15))
			o->oSubAction++;
	}
	else if(o->oSubAction == 1)
	{
		if(s_setanime_endcheck(14))
			o->oSubAction++;
	}
	else
		s_set_skelanimeNo(12);
	o->oForwardVel = 0.0f;
	o->oMoveAngleYaw += a1 / FRAME_RATE_SCALER_INV;
	if(o->oTimer >= a0 * FRAME_RATE_SCALER_INV)
		return 1;
	else
		return 0;
}

void s_kopa_turn()
{
	if(kopaturn(63, 0x200))
		o->oAction = mode_s_kopa_wait;
}


void s_kopa_dash()
{
	s32 timer;
	if(o->oTimer == 0)
		o->oForwardVel = 0.0f;
	switch(o->oSubAction)
	{
		case KOPADASH_START:
			o->oBowserUnkF8 = 0;
			if(s_setanime_endcheck(18))
				o->oSubAction = KOPADASH_DASH;
			break;
		case KOPADASH_DASH:
			o->oForwardVel = 50.0f;
			if(s_setanime_endcheck(0x13) != 0)
			{
				o->oBowserUnkF8++;
				if(o->oBowserUnkF8 >= 6)
					o->oSubAction = KOPADASH_SLIP;
				if(o->oBowserUnkF8 >= 2)
					if(s_calc_dangle(o->oAngleToMario, o->oMoveAngleYaw) > 0x2000)
						o->oSubAction = KOPADASH_SLIP;
			}
			s_chase_angleY(o->oAngleToMario, 0x200 / FRAME_RATE_SCALER_INV);
			break;
		case KOPADASH_SLIP:
			o->oBowserUnkF8 = 0;
			s_set_skelanimeNo(21);
			s_makeobj_chain_scale(0, 100, -50, 0, 3.0f, o, MODEL_SMOKE, sm64::bhv::bhvWhitePuffSmoke2());
			s_makeobj_chain_scale(0, -100, -50, 0, 3.0f, o, MODEL_SMOKE, sm64::bhv::bhvWhitePuffSmoke2());
			if(s_chase_speed(&o->oForwardVel, 0, -1.0f * FRAME_RATE_SCALER))
				o->oSubAction = KOPADASH_END;
			s_stop_animeend();
			break;
		case KOPADASH_END:
			o->oForwardVel = 0.0f;
			s_set_skelanimeNo(20);
			if(s_check_animeend())
			{
				if(BITS)
					timer = 10;
				else
					timer = 30;
				if(o->oBowserUnkF8 > timer)
					o->oAction = 0;
				o->oBowserUnkF8++;
			}
			s_stop_animeend();
			break;
	}
	if(o->oMoveFlags & 0x400)
		o->oAction = 10;
}

s32 kirai_damagecheck()
{
	struct Object* mine;
	f32 distance;
	mine = s_search_nearobject(sm64::bhv::bhvBowserBomb(), &distance);
	if(mine != NULL && distance < 800.0f)
	{
		mine->oInteractStatus |= INTERACT_MR_BLIZZARD;
		return 1;
	}
	return 0;
}

void s_kopa_swingstart() // size?
{
	UNUSED s32 unused;
	if(o->oTimer < 2 * FRAME_RATE_SCALER_INV)
		o->oBowserUnkF8 = 0;
	if(o->oSubAction == 0)
	{
		s_set_skelanimeNo(2);
		kopa_boundsound(&o->oBowserUnkF8);
		if(o->oMoveFlags & 2)
		{
			o->oForwardVel = 0.0f;
			o->oSubAction++;
		}
	}
	else if(s_setanime_endcheck(0))
		o->oAction = 0;
	if(kirai_damagecheck())
	{
		o->oHealth--;
		if(o->oHealth <= 0)
			o->oAction = 4;
		else
			o->oAction = 12;
	}
}

void bowser_set_goal_invisible()
{
	o->oBowserUnk1AC = 0;
	if(o->oOpacity == 0)
	{
		o->oForwardVel = 0.0f;
		o->oVelY       = 0.0f;
		o->oPosY       = o->oHomeY - 1000.0f;
	}
}

void s_kopa_superjumpstart()
{
	s32 sp2C;
	UNUSED s32 unused;
	s8* sp24 = (s8*)o->oFloor;
	if(sp24 != NULL && sp24[4] & 1)
		sp2C = 1;
	else
		sp2C = 0;
	o->oBowserUnkF4 |= 0x10000;
	switch(o->oSubAction)
	{
		case 0:
			if(o->oTimer == 0)
			{
				o->oFaceAnglePitch = 0;
				o->oFaceAngleRoll  = 0;
			} //? missing else
			o->oFaceAnglePitch += 0x800 / FRAME_RATE_SCALER_INV;
			o->oFaceAngleRoll += 0x800 / FRAME_RATE_SCALER_INV;
			if(!(o->oFaceAnglePitch & 0xFFFF))
				o->oSubAction++;
			bowser_set_goal_invisible();
			break;
		case 1:
			s_set_skelanimeNo(9);
			if(s_check_animenumber(11))
			{
				o->oMoveAngleYaw = o->oBowserAngleToCentre;
				o->oVelY	 = 150.0f;
				o->oBowserUnk1AC = 0xFF;
				o->oBowserUnkF8	 = 0;
				o->oSubAction++;
			}
			else
				bowser_set_goal_invisible();
			break;
		case 2:
			if(o->oPosY > o->oHomeY)
			{
				o->oDragStrength = 0.0f;
				if(o->oBowserDistToCentre < 2500.0f)
				{
					if(s_abs_f(o->oFloorHeight - o->oHomeY) < 100.0f)
						s_chase_speed(&o->oForwardVel, 0, -5.0f * FRAME_RATE_SCALER);
					else
						obj_forward_vel_approach_upward(150.0f, 2.0f * FRAME_RATE_SCALER);
				}
				else
					obj_forward_vel_approach_upward(150.0f, 2.0f * FRAME_RATE_SCALER);
			}

			if(kopa_jumpend_check())
			{
				o->oDragStrength = 10.0f;
				o->oSubAction++;
				if(sp2C == 0)
					make_firering();
				else if(BITS)
					o->oAction = 13;
				if(BITFS)
					o->oAction = 19;
			}
			kopa_jump_error_check();
			break;
		case 3:
			if(s_check_animeend())
			{
				o->oAction = 0;
				o->oBowserUnkF4 &= ~0x10000;
				s_stop_animeend();
			}
			break;
	}
}

void s_kopa_punch()
{
	if(s_check_chartable(o->oTimer, punchsound_tab))
		objsound(NA_SE3_KUPAWALK);
	if(s_setanime_endcheck(10))
		o->oAction = 0;
}

void kopa_dead_makekey()
{
	if(BITS)
		gSecondCameraFocus = s_makeobj_nowpos(o, MODEL_STAR, sm64::bhv::bhvGrandStar());
	else
	{
		gSecondCameraFocus = s_makeobj_nowpos(o, MODEL_BOWSER_KEY, sm64::bhv::bhvBowserKey());
		objsound(SOUND_GENERAL2_BOWSER_KEY);
	}
	gSecondCameraFocus->oAngleVelYaw = o->oAngleVelYaw;
}

void bowser_fly_back_dead()
{
	s_set_skelanimeNo(16);
	if(BITS)
		o->oForwardVel = -400.0f;
	else
		o->oForwardVel = -200.0f;
	o->oVelY	 = 100.0f;
	o->oMoveAngleYaw = o->oBowserAngleToCentre + 0x8000;
	o->oBowserUnkF8	 = 0;
	o->oSubAction++;
}

void kopadead_entry()
{
	o->oBowserEyesShut = 1;
	kopa_boundsound(&o->oBowserUnkF8);
	if(o->oMoveFlags & 1)
		objsound(NA_SE3_KUPAWALK);
	if(o->oMoveFlags & 2)
	{
		o->oForwardVel = 0.0f;
		o->oSubAction++;
	}
}

s32 kopadead_wait()
{
	s32 ret = 0;
	s_hitOFF();
	if(s_setanime_endcheck(17) && o->oDistanceToMario < 700.0f && s_calc_dangle(gMarioObject->oMoveAngleYaw, o->oAngleToMario) > 0x6000)
	{
		ret = 1;
	}
	s_stop_animeend();
	o->oBowserUnkF8 = 0;
	return ret;
}

s32 bowser_dead_twirl_into_trophy()
{
	s32 ret = 0;

	if(o->header.gfx.scale[0] < 0.8)
	{
		o->oAngleVelYaw += 0x80 / FRAME_RATE_SCALER_INV;
	}

	if(o->header.gfx.scale[0] > 0.2)
	{
		o->header.gfx.scale[0] = o->header.gfx.scale[0] - 0.02;
		o->header.gfx.scale[2] = o->header.gfx.scale[2] - 0.02;
	}
	else
	{
		o->header.gfx.scale[1] = o->header.gfx.scale[1] - 0.01;
		o->oVelY	       = 20.0f;
		o->oGravity	       = 0.0f;
	}
	if(o->header.gfx.scale[1] < 0.5)
		ret = 1;

	o->oMoveAngleYaw += o->oAngleVelYaw * FRAME_RATE_SCALER;

	if(o->oOpacity >= 3)
	{
		o->oOpacity -= 2 / FRAME_RATE_SCALER_INV;
	}
	return ret;
}

void s_erase_kopa()
{
	s_set_scale(0);
	o->oForwardVel = 0.0f;
	o->oVelY       = 0.0f;
	o->oGravity    = 0.0f;
}

s32 kopadead_keydemo()
{
	s32 ret = 0;
	if(o->oBowserUnkF8 < 2)
	{
		if(o->oBowserUnkF8 == 0)
		{
			Na_SeqVolMute(0, 60, 40);
			o->oBowserUnkF8++;
		}
		if(s_call_enemydemo(2, 18, sBowserDefeatedDialogText[o->oBehParams2ndByte], 0))
		{
			o->oBowserUnkF8++;
			objsound(SOUND_GENERAL2_BOWSER_EXPLODE);
			Na_SeqVolRecover(0, 60);
			Na_BossBgmStop(0, 1);
		}
	}
	else if(bowser_dead_twirl_into_trophy())
	{
		s_erase_kopa();
		s_boxeffect(20, 116, 1.0f, 0);
		kopa_dead_makekey();
		CtrlPlayerDialog(0);
		ret = 1;
	}
	return ret;
}

s32 kopadead_stardemo()
{
	UNUSED s32 unused;
	s32 ret = 0;
	s32 dialogID;
	if(o->oBowserUnkF8 < 2)
	{
		if(playerMeter.stars < 120)
			dialogID = DIALOG_121;
		else
			dialogID = DIALOG_163;
		if(o->oBowserUnkF8 == 0)
		{
			Na_SeqVolMute(0, 60, 40);
			o->oBowserUnkF8++;
		}
		if(s_call_enemydemo(2, 18, dialogID, 0))
		{
			s_change_shape(MODEL_BOWSER2);
			Na_SeqVolRecover(0, 60);
			Na_BossBgmStop(0, 1);
			kopa_dead_makekey();
			o->oBowserUnkF8++;
		}
	}
	else if(o->oOpacity > 4)
		o->oOpacity -= 4;
	else
	{
		s_erase_kopa();
		ret = 1;
	}
	return ret;
}

void s_kopa_dead()
{
	switch(o->oSubAction)
	{
		case 0:
			bowser_fly_back_dead();
			break;
		case 1:
			kopadead_entry();
			break;
		case 2:
			if(kopadead_wait())
			{
				o->oBowserUnkF8 = 0;
				if(BITS)
					o->oSubAction = 10;
				else
				{
					o->activeFlags |= 0x80;
					o->oSubAction++;
				}
			}
			break;
		case 3:
			if(kopadead_keydemo())
				o->oSubAction++;
			break;
		case 4:
			break;
		case 10:
			if(kopadead_stardemo())
				o->oSubAction++;
			break;
		case 11:
			break;
	}
}

void s_rotate_entry(struct Object* bowser, s16 a1)
{
	s16 angle;
	angle		       = o->oBowserAngleToCentre + 0x8000;
	bowser->oAngleVelPitch = coss(angle) * a1;
	bowser->oAngleVelRoll  = -sins(angle) * a1;
}

void s_kopa_bg_rotate()
{
	struct Object* platform = s_find_obj(sm64::bhv::bhvTiltingBowserLavaPlatform());
	UNUSED s16 angle		= o->oBowserAngleToCentre + 0x8000;
	s16 speed;
	s32 dp;
	s32 flag;

	if(platform == NULL)
	{
		o->oAction = 0;
	}
	else
	{
		dp    = 0;
		flag = 1;
		while(kopabg_timedata[dp][2] != 0)
		{
			if(o->oTimer < kopabg_timedata[dp][2])
			{
				speed = kopabg_timedata[dp][1];
				if(kopabg_timedata[dp][0] > 0)
					speed = (kopabg_timedata[dp][2] - o->oTimer - 1) * speed;
				else
					speed = (o->oTimer - kopabg_timedata[dp - 1][2]) * speed;
				s_rotate_entry(platform, speed);
				if(speed != 0)
					AudStartSound(SOUND_ENV_UNKNOWN4, platform->header.gfx.cameraToObject);
				flag = 0;
				break;
			}
			dp++;
		}
		if(flag)
		{
			o->oAction		  = 0;
			platform->oAngleVelPitch  = 0;
			platform->oAngleVelRoll	  = 0;
			platform->oFaceAnglePitch = 0;
			platform->oFaceAngleRoll  = 0;
		}
	}
	s_stop_animeend();
}

s32 kopa_areaout_check() // bowser off stage?
{
	s32 action = o->oAction;
	if(o->oAction != 2 && o->oAction != mode_s_kopa_bg_rotate)
	{
		if(o->oPosY < o->oHomeY - 1000.0f)
			return 1;

		if(o->oMoveFlags & 1)
		{
			if(o->oFloorType == 1)
				return 1;
			if(o->oFloorType == 10)
				return 1;
		}
	}
	return 0;
}

void (*sBowserActions[])() = {
    s_kopa_wait,
    s_kopa_swingstart,
    s_kopa_superjumpstart,
    s_kopa_punch,
    s_kopa_dead,
    s_kopa_demo_wait,
    s_kopa_demo_walk,
    s_kopa_dash,
    s_kopa_sky_fire,
    s_kopa_small_fire,
    s_kopa_cliff_dance,
    s_kopa_turn,
    s_kopa_safe_return,
    s_kopa_jump_atom,
    s_kopa_search_walk,
    s_kopa_normal_fire,
    s_kopa_erasemove,
    s_kopa_jump,
    s_kopa_normal_wait,
    s_kopa_bg_rotate
};

struct SoundState kopa_sound[] = {
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {1, 0, -1, NA_SE3_KUPAWALK},
    {1, 0, -1, NA_SE3_KUPAGAO},
    {1, 0, -1, NA_SE3_KUPAGAO},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {1, 20, 40, NA_SE3_KUPAWALK},
    {1, 20, -1, NA_SE3_KUPAWALK},
    {1, 20, 40, NA_SE3_KUPAWALK},
    {1, 0, -1, NA_SE3_KUPADAMAGE},
    {1, 0, -1, NA_SE3_KUPADOWN},
    {1, 8, -1, NA_SE3_KUPAWALK},
    {1, 8, 17, NA_SE3_KUPAWALK},
    {1, 8, -10, NA_SE3_KUPAWALK},
    {0, 0, 0, NO_SOUND},
    {1, 5, -1, SOUND_OBJ_FLAME_BLOWN},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {1, 0, -1, NA_SE3_KUPADAMAGE},
    {1, 0, -1, NA_SE3_KUPAGAO}};

static s8 initkopa_light[4] = {0, 0, 1, 0};
static s8 initkopa_hp[4] = {1, 1, 3, 0};

extern const Collision bowser_3_seg7_collision_07004B94[];
extern const Collision bowser_3_seg7_collision_07004C18[];
extern const Collision bowser_3_seg7_collision_07004C9C[];
extern const Collision bowser_3_seg7_collision_07004D20[];
extern const Collision bowser_3_seg7_collision_07004DA4[];
extern const Collision bowser_3_seg7_collision_07004E28[];
extern const Collision bowser_3_seg7_collision_07004EAC[];
extern const Collision bowser_3_seg7_collision_07004F30[];
extern const Collision bowser_3_seg7_collision_07004FB4[];
extern const Collision bowser_3_seg7_collision_07005038[];
struct Struct8032F698 D_8032F698[] = {
    {NULL, 0, 0, 0, 0},
    {bowser_3_seg7_collision_07004B94, -800, -1000, -20992, 0},
    {bowser_3_seg7_collision_07004C18, -1158, 390, -18432, 0},
    {bowser_3_seg7_collision_07004C9C, -1158, 390, -7680, 0},
    {bowser_3_seg7_collision_07004D20, 0, 1240, -6144, 0},
    {bowser_3_seg7_collision_07004DA4, 0, 1240, 6144, 0},
    {bowser_3_seg7_collision_07004E28, 1158, 390, 7680, 0},
    {bowser_3_seg7_collision_07004EAC, 1158, 390, 18432, 0},
    {bowser_3_seg7_collision_07004F30, 800, -1000, 20992, 0},
    {bowser_3_seg7_collision_07004FB4, 800, -1000, -31744, 0},
    {bowser_3_seg7_collision_07005038, -800, -1000, 31744, 0}};

void kopa_main()
{
	struct Surface* floor;
	struct Object* platform;
	UNUSED f32 floorHeight;
	if((platform = o->platform) != NULL)
		matrix_offset(0, platform);
	o->oBowserUnk10E = 0;
	s_enemybgcheck();
	s_modejmp(sBowserActions);
	s_enemymove(-78);

	if(kopa_areaout_check())
	{
		o->oAction = mode_s_kopa_superjumpstart; // bowser go home?
	}

	floorHeight = mcBGGroundCheck(o->oPosX, o->oPosY, o->oPosZ, &floor);

	if((floor != NULL) && (floor->object != 0))
		o->platform = floor->object;
	else
		o->platform = NULL;

	s_enemysound(kopa_sound);
}

void kopa_catch() // bowser held
{
	o->oBowserUnkF4 &= ~0x20000;
	s_hitOFF();

	switch(o->oBowserUnk10E) // state
	{
		case 0:
			objsound(NA_SE3_KUPADAMAGE);
			s_mode_catch(3, 1);
			o->oBowserUnk10E++;
			break;
		case 1:
			if(s_check_animeend())
			{
				s_set_skelanimeNo(2);
				o->oBowserUnk10E++;
			}
			break;
		case 2:
			break;
	}

	o->oMoveFlags		  = 0;
	o->oBowserHeldAnglePitch  = gMarioObject->oMoveAnglePitch;
	o->oBowserHeldAngleVelYaw = gMarioObject->oAngleVelYaw;
	o->oMoveAngleYaw	  = gMarioObject->oMoveAngleYaw;
}

static void kopa_throw() // thrown
{
	f32 throwVelocity;
	o->oBowserUnk10E = 0;

	s_mode_throw(1.0f, 1.0f, 1);
	throwVelocity = o->oBowserHeldAngleVelYaw / 3000.0 * 70.0f;

	if(throwVelocity < 0.0f)
	{
		throwVelocity = -throwVelocity;
	}

	if(throwVelocity > 90.0f)
		throwVelocity *= 2.5; // > 90 => get bigger?

	const s16 align = DEGREES(sm64::config().cheats().bowserAimAssist());

	if(align)
	{
		s16 distance = 0;
		Object* bomb = s_get_effectobj(sm64::bhv::bhvBowserBomb(), &distance);

		if(bomb && distance < align)
		{
			Vec3f bombPos, bowserPos;
			f32 dist;
			s16 pitch = 0, yaw = 0;

			vec3f_set(bombPos, bomb->oPosX, bomb->oPosY, bomb->oPosZ);
			vec3f_set(bowserPos, o->oPosX, o->oPosY, o->oPosZ);

			vec3f_get_dist_and_angle(bowserPos, bombPos, &dist, &pitch, &yaw);
			s16 currentYaw	 = o->oMoveAngleYaw;
			o->oMoveAngleYaw = yaw;
		}
	}

	o->oForwardVel = coss(o->oBowserHeldAnglePitch) * (throwVelocity);
	o->oVelY       = -sins(o->oBowserHeldAnglePitch) * (throwVelocity);

	s_hitOFF();

	o->prevObj->oAction    = mode_kopatail_wait; // not sure what prevObj is
	o->prevObj->oTimer     = 0;
	o->prevObj->oSubAction = 0;

	o->oTimer     = 0;
	o->oSubAction = 0;
}

void bhv_bowser_loop()
{
	s16 angleToMario;  // AngleToMario    from Bowser's perspective
	s16 angleToCentre; // AngleToCentre from Bowser's perspective

	o->oBowserDistToCentre	= sqrtf(o->oPosX * o->oPosX + o->oPosZ * o->oPosZ);
	o->oBowserAngleToCentre = atan2s(0.0f - o->oPosZ, 0.0f - o->oPosX);

	angleToMario  = s_calc_dangle(o->oMoveAngleYaw, o->oAngleToMario);
	angleToCentre = s_calc_dangle(o->oMoveAngleYaw, o->oBowserAngleToCentre);

	o->oBowserUnkF4 &= ~0xFF;

	if(angleToMario < 0x2000)
		o->oBowserUnkF4 |= 2;

	if(angleToCentre < 0x3800)
		o->oBowserUnkF4 |= 4;

	if(o->oBowserDistToCentre < 1000.0f)
		o->oBowserUnkF4 |= 0x10;

	if(o->oDistanceToMario < 850.0f)
		o->oBowserUnkF4 |= 8;

	switch(o->oHeldState)
	{
		case HELD_FREE:
			kopa_main();
			break;
		case HELD_HELD:
			kopa_catch();
			break;
		case HELD_THROWN:
			kopa_throw();
			break;
		case HELD_DROPPED:
			kopa_throw();
			break;
	}

	s_set_mtxmode();

	if(o->oAction != 4)
	{
		if(o->oBowserUnk1AC != o->oOpacity)
		{
			if(o->oBowserUnk1AC > o->oOpacity)
			{
				o->oOpacity += 20 / FRAME_RATE_SCALER_INV;
				if(o->oOpacity >= 0x100)
					o->oOpacity = 0xFF;
			}
			else
			{
				o->oOpacity -= 20 / FRAME_RATE_SCALER_INV;
				if(o->oOpacity < 0)
					o->oOpacity = 0;
			}
		}
	}
}

void bhv_bowser_init()
{
	s32 level; // 0 is dw, 1 is fs, 2 is sky
	o->oBowserUnk110 = 1;
	o->oOpacity	 = 0xFF;
	o->oBowserUnk1AC = 0xFF;
	if(activeStageNo == LEVEL_BOWSER_2)
		level = 1;
	else if(activeStageNo == LEVEL_BOWSER_3)
		level = 2;
	else
		level = 0;

	o->oBehParams2ndByte = level;
	o->oBowserUnk1B2     = initkopa_light[level];
	o->oHealth	     = initkopa_hp[level];

	s_set_camerainfo(o, CAM_EVENT_BOWSER_INIT);
	o->oAction	   = 5;
	o->oBowserUnk1AE   = 0;
	o->oBowserEyesShut = 0;
}

#undef BITDW
#undef BITFS
#undef BITS

Gfx* KopaProc1(s32 run, UNUSED struct GraphNode* node, Mat4 mtx)
{
	Mat4 sp20;
	struct Object* sp1C;

	if(run == TRUE)
	{
		sp1C = (struct Object*)gCurGraphNodeObject;
		if(sp1C->prevObj != NULL)
		{
			s_calc_skeleton_glbmtx(sp20, mtx, *(Mat4*)gCurGraphNodeCamera->matrixPtr);
			s_calc_skeleton_glbpos(sp20, sp1C->prevObj);
			s_copy_worldXYZmappos(sp1C->prevObj);
		}
	}
	return NULL;
}

enum
{
	kopaeye_0,
	kopaeye_1,
	kopaeye_2,
	kopaeye_R1,
	kopaeye_R2,
	kopaeye_L1,
	kopaeye_L2,
	kopaeye_damage,
	kopaeye_angry,
	kopaeye_3
};

void kopaeye_normal(struct Object* a0, struct GraphNodeSwitchCase* switchCase)
{
	s16 dangle = s_calc_dangle(a0->oMoveAngleYaw, a0->oAngleToMario);
	s32 mode = switchCase->selectedCase;
	switch(mode)
	{
		case kopaeye_0:
			if(dangle > 0x2000)
			{
				if(a0->oAngleVelYaw > 0)
				{
					switchCase->selectedCase = kopaeye_L1;
				}

				if(a0->oAngleVelYaw < 0)
				{
					switchCase->selectedCase = kopaeye_R1;
				}
			}
			if(a0->oBowserUnk1AE > 50)
				switchCase->selectedCase = kopaeye_1;
			break;
		case kopaeye_1:
			if(a0->oBowserUnk1AE > 2)
				switchCase->selectedCase = kopaeye_2;
			break;
		case kopaeye_2:
			if(a0->oBowserUnk1AE > 2)
				switchCase->selectedCase = kopaeye_3;
			break;
		case kopaeye_3:
			if(a0->oBowserUnk1AE > 2)
				switchCase->selectedCase = kopaeye_0;
			break;
		case kopaeye_L1:
			if(a0->oBowserUnk1AE > 2)
			{
				switchCase->selectedCase = kopaeye_L2;
				if(a0->oAngleVelYaw <= 0)
					switchCase->selectedCase = kopaeye_0;
			}
			break;
		case kopaeye_L2:
			if(a0->oAngleVelYaw <= 0)
				switchCase->selectedCase = 5;
			break;
		case kopaeye_R1:
			if(a0->oBowserUnk1AE > 2)
			{
				switchCase->selectedCase = kopaeye_R2;
				if(a0->oAngleVelYaw >= 0)
					switchCase->selectedCase = 0;
			}
			break;
		case kopaeye_R2:
			if(a0->oAngleVelYaw >= 0)
				switchCase->selectedCase = kopaeye_R1;
			break;
		default:
			switchCase->selectedCase = kopaeye_0;
	}
	if(switchCase->selectedCase != mode)
		a0->oBowserUnk1AE = -1;
}

/** Geo switch for controlling the state of bowser's eye direction and open/closed
 * state. Checks whether oBowserEyesShut is TRUE and closes eyes if so and processes
 * direction otherwise.
 */
Gfx* geo_switch_bowser_eyes(s32 run, struct GraphNode* node, UNUSED Mat4* mtx)
{
	UNUSED s16 sp36;
	UNUSED s32 unused;
	struct Object* obj		       = (struct Object*)gCurGraphNodeObject;
	struct GraphNodeSwitchCase* switchCase = (struct GraphNodeSwitchCase*)node;
	if(run == TRUE)
	{
		if(gCurGraphNodeHeldObject != NULL)
			obj = gCurGraphNodeHeldObject->objNode;
		switch(sp36 = obj->oBowserEyesShut)
		{
			case 0: // eyes open, handle eye looking direction
				kopaeye_normal(obj, switchCase);
				break;
			case 1: // eyes closed, blinking
				switchCase->selectedCase = 2;
				break;
		}
		obj->oBowserUnk1AE++;
	}
	return NULL;
}

Gfx* Geo18_802B7D44(s32 a0, struct GraphNode* node, UNUSED s32 a2)
{
	Gfx* sp2C = NULL;
	Gfx* sp28;
	struct Object* sp24;
	struct GraphNodeGenerated* sp20;

	if(a0 == 1)
	{
		sp24 = (struct Object*)gCurGraphNodeObject;
		sp20 = (struct GraphNodeGenerated*)node;
		if(gCurGraphNodeHeldObject != 0)
			sp24 = gCurGraphNodeHeldObject->objNode;
		if(sp24->oOpacity == 0xFF)
			sp20->fnNode.node.flags = (sp20->fnNode.node.flags & 0xFF) | GRAPH_NODE_TYPE_FUNCTIONAL;
		else
			sp20->fnNode.node.flags = (sp20->fnNode.node.flags & 0xFF) | (GRAPH_NODE_TYPE_FUNCTIONAL | GRAPH_NODE_TYPE_400);
		sp28 = sp2C = (Gfx*)AllocDynamic(2 * sizeof(Gfx));

		if(sp24->oBowserUnk1B2 != 0)
		{
			gSPClearGeometryMode(sp28++, G_LIGHTING);
		}
		gSPEndDisplayList(sp28);
	}
	return sp2C;
}

void ActionFallingBowserPlatform0()
{
	o->oPlatformUnkF8 = s_find_obj(sm64::bhv::bhvBowser());
	s_set_shapeinfo(o, D_8032F698[o->oBehParams2ndByte].unk0);
	if(o->oPlatformUnkF8 != 0)
		o->oAction = 1;
}

void ActionFallingBowserPlatform1()
{
	UNUSED s32 unused;
	struct Object* sp0 = o->oPlatformUnkF8;
	if(sp0->platform == o)
		if(sp0->oAction == 13 && sp0->oBowserUnkF4 & 0x10000)
		{
			o->oAction = 2;
		}
	if(sp0->oHealth == 1 && (sp0->oAction == 3 || sp0->oHeldState != HELD_FREE))
		o->oSubAction = 1;
	if(o->oSubAction == 0)
		o->oPlatformUnkFC = 0;
	else
	{
		if((gDebugInfo[4][6] + 20) * (o->oBehParams2ndByte - 1) < o->oPlatformUnkFC)
		{
			o->oAction = 2;
		}
		o->oPlatformUnkFC++;
	}
}

void ActionFallingBowserPlatform2()
{
	Vec3f sp24;
	s16 sp22;
	f32 sp1C;
	UNUSED struct Object* sp18 = o->oPlatformUnkF8;
	if(o->oTimer == 0 || o->oTimer == 22 * FRAME_RATE_SCALER_INV)
		objsound(SOUND_GENERAL_BOWSER_PLATFORM_2);
	if(o->oTimer < 22 * FRAME_RATE_SCALER_INV)
	{
		Viewshake(SHAKE_ENV_FALLING_BITS_PLAT);
		o->oVelY    = 8.0f;
		o->oGravity = 0.0f;
	}
	else
		o->oGravity = -4.0f;
	if((o->oTimer & 1) == 0 && o->oTimer < 14 * FRAME_RATE_SCALER_INV)
	{
		sp22 = D_8032F698[o->oBehParams2ndByte].unk3 + (gDebugInfo[4][1] << 8);
		sp1C = -((o->oTimer / FRAME_RATE_SCALER_INV) / 2) * 290 + 1740;
		func_802B8F7C(sp24, &o->oPosX);
		o->oPosX = D_8032F698[o->oBehParams2ndByte].unk1 + sins(sp22 + 5296) * sp1C;
		o->oPosZ = D_8032F698[o->oBehParams2ndByte].unk2 + coss(sp22 + 5296) * sp1C;
		o->oPosY = 307.0f;
		s_burneffect(4, 0, 100.0f);
		o->oPosX = D_8032F698[o->oBehParams2ndByte].unk1 + sins(sp22 - 5296) * sp1C;
		o->oPosZ = D_8032F698[o->oBehParams2ndByte].unk2 + coss(sp22 - 5296) * sp1C;
		s_burneffect(4, 0, 100);
		func_802B8F7C(&o->oPosX, sp24);
	}
	s_optionmove_F();
	if(o->oTimer > 300 * FRAME_RATE_SCALER_INV)
		s_remove_obj(o);
}

void (*sFallingBowserPlatformActions[])() = {ActionFallingBowserPlatform0, ActionFallingBowserPlatform1, ActionFallingBowserPlatform2};

struct ObjectHitbox sGrowingBowserFlameHitbox = {
    /* interactType: */ INTERACT_FLAME,
    /* downOffset: */ 20,
    /* damageOrCoinValue: */ 1,
    /* health: */ 0,
    /* numLootCoins: */ 0,
    /* radius: */ 10,
    /* height: */ 40,
    /* hurtboxRadius: */ 0,
    /* hurtboxHeight: */ 0,
};

struct ObjectHitbox sBowserFlameHitbox = {
    /* interactType: */ INTERACT_FLAME,
    /* downOffset: */ 0,
    /* damageOrCoinValue: */ 1,
    /* health: */ 0,
    /* numLootCoins: */ 0,
    /* radius: */ 10,
    /* height: */ 40,
    /* hurtboxRadius: */ 0,
    /* hurtboxHeight: */ 0,
};

f32 D_8032F748[] = {-8.0f, -6.0f, -3.0f};

void bhv_falling_bowser_platform_loop()
{
	s_modejmp(sFallingBowserPlatformActions);
}

void func_802B7A58()
{
	s_remove_obj(o);
	s_makeobj_nowpos_scale(o, MODEL_NONE, sm64::bhv::bhvBlackSmokeUpward(), 1.0f);
	if(Randomf() < 0.1)
		s_makeobj_nowpos(o, MODEL_YELLOW_COIN, sm64::bhv::bhvTemporaryYellowCoin());
}

s32 func_802B7ADC(s32 a0)
{
	if(a0 * FRAME_RATE_SCALER_INV < o->oTimer)
		return 1;

	if(o->oFloorType == 1)
		return 1;

	if(o->oFloorType == 10)
		return 1;

	return 0;
}

void bhv_flame_bowser_init()
{
	o->oAnimState	 = (s32)(Randomf() * 10.0f);
	o->oMoveAngleYaw = RandomU16();
	if(Randomf() < 0.2)
		o->oVelY = 80.0f;
	else
		o->oVelY = 20.0f;
	o->oForwardVel = 10.0f;
	o->oGravity    = -1.0f;
	o->oFlameScale = Randomf() + 1.0f;
}

void bhv_flame_large_burning_out_init()
{
	o->oAnimState	 = (s32)(Randomf() * 10.0f);
	o->oMoveAngleYaw = RandomU16();
	o->oVelY	 = 10.0f;
	o->oForwardVel	 = 0.0f;
	o->oFlameScale	 = 7.0f;
}

void func_802B7CFC()
{
	s32 sp4;
	sp4 = ((o->oFlameUnkF8 + (frameCounter / FRAME_RATE_SCALER_INV)) & 0x3F) << 10;
	o->oPosX += sins(o->oMoveAngleYaw) * sins(sp4) * 4.0f * FRAME_RATE_SCALER;
	o->oPosZ += coss(o->oMoveAngleYaw) * sins(sp4) * 4.0f * FRAME_RATE_SCALER;
}

void bhv_flame_bowser_loop()
{
	s_enemybgcheck();
	s_enemymove(78);

	if(o->oVelY < -4.0f)
		o->oVelY = -4.0f;

	if(o->oAction == 0)
	{
		s_hitOFF();
		func_802B7CFC();

		if(o->oMoveFlags & 1)
		{
			o->oAction++;
			if(s_check_pathname(sm64::bhv::bhvFlameLargeBurningOut()))
				o->oFlameScale = 8.0f;
			else
				o->oFlameScale = Randomf() * 2 + 6.0f;
			o->oForwardVel = 0;
			o->oVelY       = 0;
			o->oGravity    = 0;
		}
	}
	else
	{
		s_hitON();
		if(o->oTimer > (o->oFlameScale * 10 + 5.0f) * FRAME_RATE_SCALER_INV)
		{
			o->oFlameScale -= 0.15 * FRAME_RATE_SCALER;
			if(o->oFlameScale <= 0.0f)
				func_802B7A58();
		}
	}

	s_set_scale(o->oFlameScale);
	o->oGraphYOffset = o->header.gfx.scale[1] * 14.0f;
	s_set_hitparam(o, &sBowserFlameHitbox);
}

void bhv_flame_moving_forward_growing_init()
{
	o->oForwardVel = 30.0f;
	s_random_XZ_offset(o, 80.0f);
	o->oAnimState  = (s32)(Randomf() * 10.0f);
	o->oFlameScale = 3.0f;
}

void bhv_flame_moving_forward_growing_loop()
{
	UNUSED s32 unused;
	UNUSED struct Object* sp18;
	s_set_hitparam(o, &sGrowingBowserFlameHitbox);
	o->oFlameScale = o->oFlameScale + 0.5 * FRAME_RATE_SCALER;
	s_set_scale(o->oFlameScale);
	if(o->oMoveAnglePitch > 0x800)
		o->oMoveAnglePitch -= 0x200 / FRAME_RATE_SCALER_INV;
	s_3Dmove();
	s_groundcheck();
	if(o->oFlameScale > 30.0f)
		s_remove_obj(o);
	if(o->oPosY < o->oFloorHeight)
	{
		o->oPosY = o->oFloorHeight;
		sp18	 = s_makeobj_nowpos(o, MODEL_RED_FLAME, sm64::bhv::bhvFlameBowser());
		s_remove_obj(o);
	}
}

void bhv_flame_floating_landing_init()
{
	o->oAnimState	 = (s32)(Randomf() * 10.0f);
	o->oMoveAngleYaw = RandomU16();
	if(o->oBehParams2ndByte != 0)
		o->oForwardVel = Randomf() * 5.0f;
	else
		o->oForwardVel = Randomf() * 70.0f;
	o->oVelY       = Randomf() * 20.0f;
	o->oGravity    = -1.0f;
	o->oFlameUnkF8 = Randomf() * 64.0f;
}

void bhv_flame_floating_landing_loop()
{
	UNUSED s32 unused;
	s_enemybgcheck();
	s_enemymove(0x4e);
	func_802B7CFC();
	if(func_802B7ADC(900))
		s_remove_obj(o);
	if(o->oVelY < D_8032F748[o->oBehParams2ndByte])
		o->oVelY = D_8032F748[o->oBehParams2ndByte];
	if(o->oMoveFlags & 1)
	{
		if(o->oBehParams2ndByte == 0)
			s_makeobj_nowpos(o, MODEL_RED_FLAME, sm64::bhv::bhvFlameLargeBurningOut());
		else
			s_makeobj_nowpos(o, MODEL_NONE, sm64::bhv::bhvBlueFlamesGroup()); //? wonder if they meant MODEL_BLUE_FLAME?
		s_remove_obj(o);
	}
	o->oGraphYOffset = o->header.gfx.scale[1] * 14.0f;
}

void bhv_blue_bowser_flame_init()
{
	s_random_XZ_offset(o, 80.0f);
	o->oAnimState  = (s32)(Randomf() * 10.0f);
	o->oVelY       = 7.0f;
	o->oForwardVel = 35.0f;
	o->oFlameScale = 3.0f;
	o->oFlameUnkFC = Randomf() * 0.5;
	o->oGravity    = 1.0f;
	o->oFlameUnkF8 = (s32)(Randomf() * 64.0f);
}

void bhv_blue_bowser_flame_loop()
{
	s32 i;
	s_set_hitparam(o, &sGrowingBowserFlameHitbox);
	if(o->oFlameScale < 16.0f)
		o->oFlameScale = o->oFlameScale + (0.5 * FRAME_RATE_SCALER);

	s_set_scale(o->oFlameScale);
	s_enemybgcheck();
	s_enemymove(0x4e);

	if(o->oTimer > 0x14 * FRAME_RATE_SCALER_INV)
	{
		if(o->oBehParams2ndByte == 0)
			for(i = 0; i < 3; i++)
				s_makeobj_chain_scale(0, 0, 0, 0, 5.0f, o, MODEL_RED_FLAME, sm64::bhv::bhvFlameFloatingLanding());
		else
		{
			s_makeobj_chain_scale(1, 0, 0, 0, 8.0f, o, MODEL_BLUE_FLAME, sm64::bhv::bhvFlameFloatingLanding());
			s_makeobj_chain_scale(2, 0, 0, 0, 8.0f, o, MODEL_BLUE_FLAME, sm64::bhv::bhvFlameFloatingLanding());
		}
		s_remove_obj(o);
	}
}

void bhv_flame_bouncing_init()
{
	o->oAnimState  = (s32)(Randomf() * 10.0f);
	o->oVelY       = 30.0f;
	o->oForwardVel = 20.0f;
	o->oFlameScale = o->header.gfx.scale[0];
	o->oFlameUnkF8 = (s32)(Randomf() * 64.0f);
}

void bhv_flame_bouncing_loop()
{
	struct Object* sp1C;
	if(o->oTimer == 0)
		o->oFlameBowserObj = s_find_obj(sm64::bhv::bhvBowser());
	sp1C	       = o->oFlameBowserObj;
	o->oForwardVel = 15.0f;
	o->oBounce     = -1.0f;
	s_set_scale(o->oFlameScale);
	s_set_hitparam(o, &sGrowingBowserFlameHitbox);
	s_enemybgcheck();
	s_enemymove(78);
	if(func_802B7ADC(300))
		s_remove_obj(o);

	if(sp1C != NULL)
		if(sp1C->oHeldState == 0)
			if(s_distanceXZ_obj2obj(o, sp1C) < 300.0f)
				s_remove_obj(o);
}

void bhv_blue_flames_group_loop()
{
	struct Object* flame;
	s32 i;

	if(o->oTimer == 0)
	{
		o->oMoveAngleYaw   = s_calc_targetangle(o, gMarioObject);
		o->oBlueFlameUnkF8 = 5.0f;
	}

	if(o->oTimer < 16 * FRAME_RATE_SCALER_INV)
	{
		if((o->oTimer % (2 * FRAME_RATE_SCALER_INV)) == 0)
		{
			for(i = 0; i < 3; i++)
			{
				flame = s_makeobj_nowpos(o, MODEL_BLUE_FLAME, sm64::bhv::bhvFlameBouncing());
				flame->oMoveAngleYaw += (i * 0x5555) / FRAME_RATE_SCALER_INV;
				flame->header.gfx.scale[0] = o->oBlueFlameUnkF8;
			}
			o->oBlueFlameUnkF8 -= 0.5 * FRAME_RATE_SCALER;
		}
	}
	else
		s_remove_obj(o);
}
