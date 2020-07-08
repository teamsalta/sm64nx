// bowser.c.inc

void ActionBowserTailAnchor0()
{
	struct Object* bowser = o->parentObj;
	obj_become_tangible();
	obj_scale(1.0f);
	if(bowser->oAction == 19)
		bowser->oIntangibleTimer = -1;
	else if(are_objects_collided(o, gMarioObject))
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
	obj_become_intangible();
}

void (*sBowserTailAnchorActions[])() = {ActionBowserTailAnchor0, ActionBowserTailAnchor1, ActionBowserTailAnchor2};
s8 D_8032F4FC[]				 = {7, 8, 9, 12, 13, 14, 15, 4, 3, 16, 17, 19, 3, 3, 3, 3};
s16 D_8032F50C[]			 = {60, 0};
s16 D_8032F510[]			 = {50, 0};
s8 D_8032F514[]				 = {24 * FRAME_RATE_SCALER_INV, 42 * FRAME_RATE_SCALER_INV, 60 * FRAME_RATE_SCALER_INV, -1};
s16 sBowserDefeatedDialogText[3]	 = {DIALOG_119, DIALOG_120, DIALOG_121};
s16 D_8032F520[][3]			 = {
	 {1, 10, 40}, {0, 0, 74}, {-1, -10, 114}, {1, -20, 134}, {-1, 20, 154}, {1, 40, 164}, {-1, -40, 174}, {1, -80, 179}, {-1, 80, 184}, {1, 160, 186}, {-1, -160, 186}, {1, 0, 0},
};

void bhv_bowser_tail_anchor_loop()
{
	obj_call_action_function(sBowserTailAnchorActions);

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
			PlaySound(SOUND_AIR_BOWSER_SPIT_FIRE);
			sp2C		   = uknArray[5 * nextFrame];
			sp28		   = uknArray[5 * nextFrame + 2];
			o->oPosX	   = bowser->oPosX + (sp28 * yawSin + sp2C * yawCos);
			o->oPosY	   = bowser->oPosY + uknArray[5 * nextFrame + 1];
			o->oPosZ	   = bowser->oPosZ + (sp28 * yawCos - sp2C * yawSin);
			o->oMoveAnglePitch = uknArray[5 * nextFrame + 4] + 0xC00;
			o->oMoveAngleYaw   = uknArray[5 * nextFrame + 3] + (s16)bowser->oMoveAngleYaw;

			if(nextFrame & 1)
			{
				spawn_object(o, MODEL_RED_FLAME, sm64::bhv::bhvFlameMovingForwardGrowing());
			}
		}
	}
}

void bhv_bowser_body_anchor_loop()
{
	copy_object_pos_and_angle(o, o->parentObj);
	if(o->parentObj->oAction == 4)
	{
#ifndef VERSION_JP
		if(o->parentObj->oSubAction == 11)
			o->oInteractType = 0;
		else
			o->oInteractType = 0x800000;
#else
		o->oInteractType = 0x800000;
#endif
	}
	else
	{
		o->oInteractType = 8;
		if(o->parentObj->oOpacity < 100)
			obj_become_intangible();
		else
			obj_become_tangible();
	}
	if(o->parentObj->oHeldState != HELD_FREE)
		obj_become_intangible();
	o->oInteractStatus = 0;
}

s32 func_802B38B4()
{
	struct Object* wave;
	if(o->oBehParams2ndByte == 2)
	{
		wave	    = spawn_object(o, MODEL_BOWSER_WAVE, sm64::bhv::bhvBowserShockWave());
		wave->oPosY = o->oFloorHeight;
		return 1;
	}
	return 0;
}

void func_802B392C(s32* a)
{
	if(o->oMoveFlags & 1)
	{
		a[0]++;
		if(a[0] < 4)
		{
			obj_start_cam_event(o, CAM_EVENT_BOWSER_THROW_BOUNCE);
			func_802AA618(0, 0, 60.0f);
			PlaySound2(SOUND_OBJ_BOWSER_WALK);
		}
	}
}

#define BITDW (o->oBehParams2ndByte == 0)
#define BITFS (o->oBehParams2ndByte == 1)
#define BITS (o->oBehParams2ndByte == 2)

s32 func_802B39B8()
{
	set_obj_animation_and_sound_state(15);
	if(obj_check_anim_frame(21))
		o->oForwardVel = 3.0f;
	if(func_8029F788())
		return 1;
	else
		return 0;
}

s32 func_802B3A30()
{
	o->oForwardVel = 3.0f;
	set_obj_animation_and_sound_state(13);
	if(func_8029F788())
		return 1;
	else
		return 0;
}

s32 func_802B3A98()
{
	set_obj_animation_and_sound_state(14);
	if(obj_check_anim_frame(20))
		o->oForwardVel = 0.0f;
	if(func_8029F788())
		return 1;
	else
		return 0;
}

void func_802B3B0C()
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

void bowser_act_text_wait() // not much
{
	o->oForwardVel = 0.0f;
	set_obj_animation_and_sound_state(12);
	func_802B3B0C();
}

void bowser_act_intro_walk()
{
	if(o->oSubAction == 0)
	{
		if(func_802B39B8())
			o->oSubAction++;
	}
	else if(o->oSubAction == 1)
	{
		if(func_802B3A30())
			o->oSubAction++;
	}
	else if(func_802B3A98())
	{
		if(o->oBowserUnk88 == 1)
			o->oBowserUnk88 = 0;
		func_802B3B0C();
	}
}

static void Unknown802B3CCC() // unused
{
	if(gDebugInfo[5][1] != 0)
	{
		o->oAction	 = D_8032F4FC[gDebugInfo[5][2] & 0xf];
		gDebugInfo[5][1] = 0;
	}
}

void func_802B3D24()
{
	f32 rand = RandomFloat();
	if(o->oBowserUnk110 == 0)
	{
		if(o->oBowserUnkF4 & 2)
		{
			if(o->oDistanceToMario < 1500.0f)
				o->oAction = 15; // nearby
			else
				o->oAction = 17; // far away
		}
		else
			o->oAction = 14;
		o->oBowserUnk110++;
	}
	else
	{
		o->oBowserUnk110 = 0;
#ifndef VERSION_JP
		if(!gCurrDemoInput)
		{
			if(rand < 0.1)
				o->oAction = 3; // rare 1/10 chance
			else
				o->oAction = 14; // common
		}
		else
		{
			o->oAction = 14; // ensure demo starts with action 14.
		}
#else
		if(rand < 0.1)
			o->oAction = 3; // rare 1/10 chance
		else
			o->oAction = 14; // common
#endif
	}
}

void func_802B3E44()
{
	f32 rand = RandomFloat();
	if(o->oBowserUnk110 == 0)
	{
		if(o->oBowserUnkF4 & 2)
		{
			if(o->oDistanceToMario < 1300.0f) // nearby
			{
				if(rand < 0.5) // 50/50
					o->oAction = 16;
				else
					o->oAction = 9;
			}
			else // far away
			{
				o->oAction = 7;
				if(500.0f < o->oBowserDistToCentre && o->oBowserDistToCentre < 1500.0f && rand < 0.5) // away from centre and good luck
					o->oAction = 13;
			}
		}
		else
			o->oAction = 14;
		o->oBowserUnk110++;
	}
	else
	{
		o->oBowserUnk110 = 0;
		o->oAction	 = 14;
	}
}

void func_802B3FDC()
{
	f32 rand = RandomFloat();
	if(o->oBowserUnkF4 & 2)
	{
		if(o->oDistanceToMario < 1000.0f)
		{
			if(rand < 0.4)
				o->oAction = 9;
			else if(rand < 0.8)
				o->oAction = 8;
			else
				o->oAction = 15;
		}
		else if(rand < 0.5)
			o->oAction = 13;
		else
			o->oAction = 7;
	}
	else
		o->oAction = 14;
}

void func_802B4124()
{
	o->oAction = 13;
}

void func_802B4144()
{
	switch(o->oBowserUnk110)
	{
		case 0:
			if(o->oBowserUnk106 == 0)
				func_802B3FDC();
			else
				func_802B4124();
			o->oBowserUnk110 = 1;
			break;
		case 1:
			o->oBowserUnk110 = 0;
			o->oAction	 = 14;
			break;
	}
}

#ifndef VERSION_JP
void func_u_802B4AF4()
{
	if(o->oVelY < 0 && o->oPosY < (o->oHomeY - 300.0f))
	{
		o->oPosX = o->oPosZ = 0;
		o->oPosY	    = o->oHomeY + 2000.0f;
		o->oVelY	    = 0;
		o->oForwardVel	    = 0;
	}
}
#endif

void bowser_act_unused_slow_walk() // unused?
{
	if(func_802A4AB0(12))
		o->oAction = 0;
}

void bowser_act_default() // only lasts one frame
{
	o->oBowserEyesShut = 0;
	set_obj_animation_and_sound_state(12);
	// stop him still
	o->oAngleVelYaw = 0;
	o->oForwardVel	= 0.0f;
	o->oVelY	= 0.0f;
	if(BITDW)
		func_802B3D24();
	else if(BITFS)
		func_802B3E44();
	else
		func_802B4144();
	// Action 14 commonly follows
}

void bowser_act_breath_fire()
{
	o->oForwardVel = 0.0f;

	if(o->oTimer == 0)
	{
		PlaySound2(SOUND_OBJ_BOWSER_INHALING);
	}

	if(func_802A4AB0(6))
	{
		o->oAction = 0;
	}
}

void bowser_act_walk_to_mario() // turn towards Mario
{
	UNUSED s32 facing; // is Bowser facing Mario?
	s16 turnSpeed;
	s16 angleFromMario = abs_angle_diff(o->oMoveAngleYaw, o->oAngleToMario);
	if(BITFS)
		turnSpeed = 0x400;
	else if(o->oHealth > 2)
		turnSpeed = 0x400;
	else if(o->oHealth == 2)
		turnSpeed = 0x300;
	else
		turnSpeed = 0x200;
	facing = obj_rotate_yaw_toward(o->oAngleToMario, turnSpeed / FRAME_RATE_SCALER_INV);
	if(o->oSubAction == 0)
	{
		o->oBowserUnkF8 = 0;
		if(func_802B39B8())
			o->oSubAction++;
	}
	else if(o->oSubAction == 1)
	{
		if(func_802B3A30())
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
	else if(func_802B3A98())
		o->oAction = 0;
}

void bowser_act_teleport()
{
	switch(o->oSubAction)
	{
		case 0:
			obj_become_intangible();
			o->oBowserUnk1AC = 0;
			o->oBowserUnkF8	 = 30;
			if(o->oTimer == 0)
				PlaySound2(SOUND_OBJ2_BOWSER_TELEPORT);
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
			if(abs_angle_diff(o->oMoveAngleYaw, o->oAngleToMario) > 0x4000)
				if(o->oDistanceToMario > 500.0f)
				{
					o->oSubAction	 = 2;
					o->oMoveAngleYaw = o->oAngleToMario; // large change in angle?
					PlaySound2(SOUND_OBJ2_BOWSER_TELEPORT);
				}
			break;
		case 2:
			o->oForwardVel	 = 0.0f;
			o->oBowserUnk1AC = 0xFF;
			if(o->oOpacity == 0xFF)
				o->oAction = 0;
			obj_become_tangible();
			break;
	}
}

void bowser_act_spit_fire_into_sky() // only in sky
{
	s32 frame;
	set_obj_animation_and_sound_state(11);
	frame = o->header.gfx.unk38.frame();
	if(frame > 24 && frame < 36 && !o->header.gfx.unk38.isPartialFrame())
	{
		PlaySound(SOUND_AIR_BOWSER_SPIT_FIRE);
		if(frame == 35)
			spawn_object_relative(1, 0, 0x190, 0x64, o, MODEL_RED_FLAME, sm64::bhv::bhvBlueBowserFlame());
		else
			spawn_object_relative(0, 0, 0x190, 0x64, o, MODEL_RED_FLAME, sm64::bhv::bhvBlueBowserFlame());
	}
	if(func_8029F788())
		o->oAction = 0;
	o->oBowserUnkF4 |= 0x20000;
}

void bowser_act_hit_mine()
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
		set_obj_animation_and_sound_state(25);
		o->oSubAction++;
		o->oBowserUnkF8 = 0;
	}
	else if(o->oSubAction == 1)
	{
		set_obj_animation_and_sound_state(25);
		func_8029F728();
		func_802B392C(&o->oBowserUnkF8);
		if((o->oBowserUnkF8 > 2))
		{
			set_obj_animation_and_sound_state(26);
			o->oVelY       = 0.0f;
			o->oForwardVel = 0.0f;
			o->oSubAction++;
		}
	}
	else if(o->oSubAction == 2)
	{
		if(func_8029F788())
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

s32 func_802B4A44()
{
	set_obj_animation_and_sound_state(9);
	if(obj_check_anim_frame(11))
		return 1;
	else
		return 0;
}

s32 func_802B4A94()
{
	if(o->oMoveFlags & 1)
	{
		o->oForwardVel = 0;
		o->oVelY       = 0;
		func_802AA618(0, 0, 60.0f);
		set_obj_animation_and_sound_state(8);
		o->header.gfx.unk38.animFrameAccelAssist = 0;
		obj_start_cam_event(o, CAM_EVENT_BOWSER_JUMP);
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

void func_802B4BA4()
{
	if(BITS && o->oBowserUnkF4 & 0x10000)
		if(o->oBowserDistToCentre > 1000.0f)
			o->oForwardVel = 60.0f;
}

void bowser_act_jump()
{
	UNUSED s32 unused;
	if(o->oSubAction == 0)
	{
		if(func_802B4A44())
		{
			if(BITS && o->oBowserUnkF4 & 0x10000)
				o->oVelY = 70.0f;
			else
				o->oVelY = 80.0f;
			o->oBowserUnkF8 = 0;
			func_802B4BA4();
			o->oSubAction++;
		}
	}
	else if(o->oSubAction == 1)
	{
#ifndef VERSION_JP
		if(o->oBehParams2ndByte == 2 && o->oBowserUnkF4 & 0x10000)
			func_u_802B4AF4();
		if(func_802B4A94())
		{
			o->oBowserUnkF4 &= 0xfffeffff;
			o->oForwardVel = 0.0f;
			o->oSubAction++;
			func_802B38B4();
			if(BITFS)
				o->oAction = 19;
		}
		else
		{
		}
#else
		if(func_802B4A94())
		{
			o->oBowserUnkF4 &= 0xfffeffff;
			o->oForwardVel = 0.0f;
			o->oSubAction++;
			func_802B38B4();
			if(BITFS)
				o->oAction = 19;
		}
		else
		{
		}
#endif
	}
	else if(func_8029F788())
		o->oAction = 0;
}

void bowser_act_jump_towards_mario()
{
	f32 sp1C = D_8032F50C[0];
	f32 sp18 = D_8032F510[0];
	if(o->oSubAction == 0)
	{
		if(func_802B4A44())
		{
			o->oVelY	= sp1C;
			o->oForwardVel	= sp18;
			o->oBowserUnkF8 = 0;
			o->oSubAction++;
		}
	}
	else if(o->oSubAction == 1)
	{
		if(func_802B4A94())
			o->oSubAction++;
	}
	else if(func_8029F788())
		o->oAction = 0;
}

void bowser_act_hit_edge()
{
	o->oForwardVel = 0.0f;
	if(o->oTimer == 0)
		o->oBowserUnkF8 = 0;
	switch(o->oSubAction)
	{
		case 0:
			set_obj_animation_and_sound_state(23);
			if(func_8029F788())
				o->oBowserUnkF8++;
			if(o->oBowserUnkF8 > 0)
				o->oSubAction++;
			break;
		case 1:
			set_obj_animation_and_sound_state(24);
			if(func_8029F788())
				o->oAction = 11;
			break;
	}
}

void bowser_act_spit_fire_onto_floor()
{
	if(gHudDisplay.wedges < 4)
		o->oBowserUnk108 = 3;
	else
		o->oBowserUnk108 = RandomFloat() * 3.0f + 1.0f;
	set_obj_animation_and_sound_state(22);
	if(obj_check_anim_frame(5))
		obj_spit_fire(0, 200, 180, 7.0f, MODEL_RED_FLAME, 30.0f, 10.0f, 0x1000);
	if(func_8029F788())
		o->oSubAction++;
	if(o->oSubAction >= o->oBowserUnk108)
		o->oAction = 0;
}

s32 func_802B5108(s32 a0, s16 a1)
{
	if(o->oSubAction == 0)
	{
		if(func_802A4AB0(15))
			o->oSubAction++;
	}
	else if(o->oSubAction == 1)
	{
		if(func_802A4AB0(14))
			o->oSubAction++;
	}
	else
		set_obj_animation_and_sound_state(12);
	o->oForwardVel = 0.0f;
	o->oMoveAngleYaw += a1 / FRAME_RATE_SCALER_INV;
	if(o->oTimer >= a0 * FRAME_RATE_SCALER_INV)
		return 1;
	else
		return 0;
}

void bowser_act_turn_from_edge()
{
	if(func_802B5108(63, 0x200))
		o->oAction = 0;
}

void bowser_act_charge_mario()
{
	s32 sp34;
	if(o->oTimer == 0)
		o->oForwardVel = 0.0f;
	switch(o->oSubAction)
	{
		case 0:
			o->oBowserUnkF8 = 0;
			if(func_802A4AB0(18))
				o->oSubAction = 1;
			break;
		case 1:
			o->oForwardVel = 50.0f;
			if(func_802A4AB0(0x13) != 0)
			{
				o->oBowserUnkF8++;
				if(o->oBowserUnkF8 >= 6)
					o->oSubAction = 3;
				if(o->oBowserUnkF8 >= 2)
					if(abs_angle_diff(o->oAngleToMario, o->oMoveAngleYaw) > 0x2000)
						o->oSubAction = 3;
			}
			obj_rotate_yaw_toward(o->oAngleToMario, 0x200 / FRAME_RATE_SCALER_INV);
			break;
		case 3:
			o->oBowserUnkF8 = 0;
			set_obj_animation_and_sound_state(21);
			spawn_object_relative_with_scale(0, 100, -50, 0, 3.0f, o, MODEL_SMOKE, sm64::bhv::bhvWhitePuffSmoke2());
			spawn_object_relative_with_scale(0, -100, -50, 0, 3.0f, o, MODEL_SMOKE, sm64::bhv::bhvWhitePuffSmoke2());
			if(approach_f32_signed(&o->oForwardVel, 0, -1.0f * FRAME_RATE_SCALER))
				o->oSubAction = 2;
			func_8029F728();
			break;
		case 2:
			o->oForwardVel = 0.0f;
			set_obj_animation_and_sound_state(20);
			if(func_8029F788())
			{
				if(BITS)
					sp34 = 10;
				else
					sp34 = 30;
				if(o->oBowserUnkF8 > sp34)
					o->oAction = 0;
				o->oBowserUnkF8++;
			}
			func_8029F728();
			break;
	}
	if(o->oMoveFlags & 0x400)
		o->oAction = 10;
}

s32 func_802B5588()
{
	struct Object* mine;
	f32 sp18;
	mine = obj_find_nearest_object_with_behavior(sm64::bhv::bhvBowserBomb(), &sp18);
	if(mine != NULL && sp18 < 800.0f)
	{
		mine->oInteractStatus |= INTERACT_MR_BLIZZARD;
		return 1;
	}
	return 0;
}

void bowser_act_thrown_dropped() // unused?
{
	UNUSED s32 unused;
	if(o->oTimer < 2 * FRAME_RATE_SCALER_INV)
		o->oBowserUnkF8 = 0;
	if(o->oSubAction == 0)
	{
		set_obj_animation_and_sound_state(2);
		func_802B392C(&o->oBowserUnkF8);
		if(o->oMoveFlags & 2)
		{
			o->oForwardVel = 0.0f;
			o->oSubAction++;
		}
	}
	else if(func_802A4AB0(0))
		o->oAction = 0;
	if(func_802B5588())
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

void bowser_act_jump_onto_stage()
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
			set_obj_animation_and_sound_state(9);
			if(obj_check_anim_frame(11))
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
					if(absf(o->oFloorHeight - o->oHomeY) < 100.0f)
						approach_f32_signed(&o->oForwardVel, 0, -5.0f * FRAME_RATE_SCALER);
					else
						obj_forward_vel_approach_upward(150.0f, 2.0f * FRAME_RATE_SCALER);
				}
				else
					obj_forward_vel_approach_upward(150.0f, 2.0f * FRAME_RATE_SCALER);
			}
			if(func_802B4A94())
			{
				o->oDragStrength = 10.0f;
				o->oSubAction++;
				if(sp2C == 0)
					func_802B38B4();
				else if(BITS)
					o->oAction = 13;
				if(BITFS)
					o->oAction = 19;
			}
#ifndef VERSION_JP
			func_u_802B4AF4();
#else
			if(o->oVelY < 0.0f && o->oPosY < o->oHomeY - 300.0f)
			{
				o->oPosZ = 0.0f, o->oPosX = o->oPosZ;
				o->oPosY = o->oHomeY + 2000.0f;
				o->oVelY = 0.0f;
			}
#endif
			break;
		case 3:
			if(func_8029F788())
			{
				o->oAction = 0;
				o->oBowserUnkF4 &= ~0x10000;
				func_8029F728();
			}
			break;
	}
}

void bowser_act_dance()
{
	if(item_in_array(o->oTimer, D_8032F514))
		PlaySound2(SOUND_OBJ_BOWSER_WALK);
	if(func_802A4AB0(10))
		o->oAction = 0;
}

void bowser_spawn_grand_star_key()
{
	if(BITS)
		gSecondCameraFocus = spawn_object(o, MODEL_STAR, sm64::bhv::bhvGrandStar());
	else
	{
		gSecondCameraFocus = spawn_object(o, MODEL_BOWSER_KEY, sm64::bhv::bhvBowserKey());
		PlaySound2(SOUND_GENERAL2_BOWSER_KEY);
	}
	gSecondCameraFocus->oAngleVelYaw = o->oAngleVelYaw;
}

void bowser_fly_back_dead()
{
	set_obj_animation_and_sound_state(16);
	if(BITS)
		o->oForwardVel = -400.0f;
	else
		o->oForwardVel = -200.0f;
	o->oVelY	 = 100.0f;
	o->oMoveAngleYaw = o->oBowserAngleToCentre + 0x8000;
	o->oBowserUnkF8	 = 0;
	o->oSubAction++;
}

void bowser_dead_bounce()
{
	o->oBowserEyesShut = 1;
	func_802B392C(&o->oBowserUnkF8);
	if(o->oMoveFlags & 1)
		PlaySound2(SOUND_OBJ_BOWSER_WALK);
	if(o->oMoveFlags & 2)
	{
		o->oForwardVel = 0.0f;
		o->oSubAction++;
	}
}

s32 bowser_dead_wait_for_mario()
{
	s32 ret = 0;
	obj_become_intangible();
	if(func_802A4AB0(17) && o->oDistanceToMario < 700.0f && abs_angle_diff(gMarioObject->oMoveAngleYaw, o->oAngleToMario) > 0x6000)
	{
		ret = 1;
	}
	func_8029F728();
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

void bowser_dead_hide()
{
	obj_scale(0);
	o->oForwardVel = 0.0f;
	o->oVelY       = 0.0f;
	o->oGravity    = 0.0f;
}

s32 bowser_dead_not_bits_end()
{
	s32 ret = 0;
	if(o->oBowserUnkF8 < 2)
	{
		if(o->oBowserUnkF8 == 0)
		{
			func_8031FFB4(0, 60, 40);
			o->oBowserUnkF8++;
		}
		if(obj_update_dialog(2, 18, sBowserDefeatedDialogText[o->oBehParams2ndByte], 0))
		{
			o->oBowserUnkF8++;
			PlaySound2(SOUND_GENERAL2_BOWSER_EXPLODE);
			sequence_player_unlower(0, 60);
			sequence_player_fade_out(0, 1);
		}
	}
	else if(bowser_dead_twirl_into_trophy())
	{
		bowser_dead_hide();
		spawn_triangle_break_particles(20, 116, 1.0f, 0);
		bowser_spawn_grand_star_key();
		set_mario_npc_dialog(0);
		ret = 1;
	}
	return ret;
}

s32 bowser_dead_bits_end()
{
	UNUSED s32 unused;
	s32 ret = 0;
	s32 dialogID;
	if(o->oBowserUnkF8 < 2)
	{
		if(gHudDisplay.stars < 120)
			dialogID = DIALOG_121;
		else
			dialogID = DIALOG_163;
		if(o->oBowserUnkF8 == 0)
		{
			func_8031FFB4(0, 60, 40);
			o->oBowserUnkF8++;
		}
		if(obj_update_dialog(2, 18, dialogID, 0))
		{
			obj_set_model(MODEL_BOWSER2);
			sequence_player_unlower(0, 60);
			sequence_player_fade_out(0, 1);
			bowser_spawn_grand_star_key();
			o->oBowserUnkF8++;
		}
	}
	else if(o->oOpacity > 4)
		o->oOpacity -= 4;
	else
	{
		bowser_dead_hide();
		ret = 1;
	}
	return ret;
}

void bowser_act_dead()
{
	switch(o->oSubAction)
	{
		case 0:
			bowser_fly_back_dead();
			break;
		case 1:
			bowser_dead_bounce();
			break;
		case 2:
			if(bowser_dead_wait_for_mario())
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
			if(bowser_dead_not_bits_end())
				o->oSubAction++;
			break;
		case 4:
			break;
		case 10:
			if(bowser_dead_bits_end())
				o->oSubAction++;
			break;
		case 11:
			break;
	}
}

void bowser_tilt_platform(struct Object* bowser, s16 a1)
{
	s16 angle;
	angle		       = o->oBowserAngleToCentre + 0x8000;
	bowser->oAngleVelPitch = coss(angle) * a1;
	bowser->oAngleVelRoll  = -sins(angle) * a1;
}

void bowser_act_ride_tilting_platform()
{
	struct Object* platform = obj_nearest_object_with_behavior(sm64::bhv::bhvTiltingBowserLavaPlatform());
	UNUSED s16 sp2A		= o->oBowserAngleToCentre + 0x8000;
	s16 sp28;
	UNUSED s32 unused;
	s32 i;
	s32 sp1C;
	if(platform == NULL)
		o->oAction = 0;
	else
	{
		i    = 0;
		sp1C = 1;
		while(D_8032F520[i][2] != 0)
		{
			if(o->oTimer < D_8032F520[i][2])
			{
				sp28 = D_8032F520[i][1];
				if(D_8032F520[i][0] > 0)
					sp28 = (D_8032F520[i][2] - o->oTimer - 1) * sp28;
				else
					sp28 = (o->oTimer - D_8032F520[i - 1][2]) * sp28;
				bowser_tilt_platform(platform, sp28);
				if(sp28 != 0)
					play_sound(SOUND_ENV_UNKNOWN4, platform->header.gfx.cameraToObject);
				sp1C = 0;
				break;
			}
			i++;
		}
		if(sp1C)
		{
			o->oAction		  = 0;
			platform->oAngleVelPitch  = 0;
			platform->oAngleVelRoll	  = 0;
			platform->oFaceAnglePitch = 0;
			platform->oFaceAngleRoll  = 0;
		}
	}
	func_8029F728();
}

s32 bowser_check_fallen_off_stage() // bowser off stage?
{
	s32 action = o->oAction;
	if(o->oAction != 2 && o->oAction != 19)
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

void (*sBowserActions[])() = {bowser_act_default,	bowser_act_thrown_dropped,	bowser_act_jump_onto_stage,	bowser_act_dance,
								bowser_act_dead,  bowser_act_text_wait,	bowser_act_intro_walk,	bowser_act_charge_mario,
								bowser_act_spit_fire_into_sky, bowser_act_spit_fire_onto_floor,				  bowser_act_hit_edge,	bowser_act_turn_from_edge, 
								bowser_act_hit_mine, bowser_act_jump,	      bowser_act_walk_to_mario,				  bowser_act_breath_fire,
								bowser_act_teleport, bowser_act_jump_towards_mario, bowser_act_unused_slow_walk,	      bowser_act_ride_tilting_platform};
struct SoundState D_8032F5B8[] = {
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {1, 0, -1, SOUND_OBJ_BOWSER_WALK},
    {1, 0, -1, SOUND_OBJ2_BOWSER_ROAR},
    {1, 0, -1, SOUND_OBJ2_BOWSER_ROAR},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {1, 20, 40, SOUND_OBJ_BOWSER_WALK},
    {1, 20, -1, SOUND_OBJ_BOWSER_WALK},
    {1, 20, 40, SOUND_OBJ_BOWSER_WALK},
    {1, 0, -1, SOUND_OBJ_BOWSER_TAIL_PICKUP},
    {1, 0, -1, SOUND_OBJ_BOWSER_DEFEATED},
    {1, 8, -1, SOUND_OBJ_BOWSER_WALK},
    {1, 8, 17, SOUND_OBJ_BOWSER_WALK},
    {1, 8, -10, SOUND_OBJ_BOWSER_WALK},
    {0, 0, 0, NO_SOUND},
    {1, 5, -1, SOUND_OBJ_FLAME_BLOWN},
    {0, 0, 0, NO_SOUND},
    {0, 0, 0, NO_SOUND},
    {1, 0, -1, SOUND_OBJ_BOWSER_TAIL_PICKUP},
    {1, 0, -1, SOUND_OBJ2_BOWSER_ROAR}};
s8 D_8032F690[4] = {0, 0, 1, 0};
s8 D_8032F694[4] = {1, 1, 3, 0};
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

void func_802B688C()
{
	struct Surface* floor;
	struct Object* platform;
	UNUSED f32 floorHeight;
	if((platform = o->platform) != NULL)
		apply_platform_displacement(0, platform);
	o->oBowserUnk10E = 0;
	obj_update_floor_and_walls();
	obj_call_action_function(sBowserActions);
	obj_move_standard(-78);

	if(bowser_check_fallen_off_stage())
	{
		o->oAction = 2; // bowser go home?
	}

	floorHeight = find_floor(o->oPosX, o->oPosY, o->oPosZ, &floor);
	if((floor != NULL) && (floor->object != 0))
		o->platform = floor->object;
	else
		o->platform = NULL;
	exec_anim_sound_state(D_8032F5B8);
}

void func_802B697C() // bowser held
{
	o->oBowserUnkF4 &= ~0x20000;
	obj_become_intangible();

	switch(o->oBowserUnk10E) // state
	{
		case 0:
			PlaySound2(SOUND_OBJ_BOWSER_TAIL_PICKUP);
			func_8029FA5C(3, 1);
			o->oBowserUnk10E++;
			break;
		case 1:
			if(func_8029F788())
			{
				set_obj_animation_and_sound_state(2);
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

void func_802B6AC0() // thrown
{
	f32 throwVelocity;
	o->oBowserUnk10E = 0;

	obj_get_thrown_or_placed(1.0f, 1.0f, 1);
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
		Object* bomb = obj_find_nearest_yaw_angle_object_with_behavior(sm64::bhv::bhvBowserBomb(), &distance);

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

	obj_become_intangible();

	o->prevObj->oAction    = 1; // not sure what prevObj is
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

	angleToMario  = abs_angle_diff(o->oMoveAngleYaw, o->oAngleToMario);
	angleToCentre = abs_angle_diff(o->oMoveAngleYaw, o->oBowserAngleToCentre);

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
			func_802B688C();
			break;
		case HELD_HELD:
			func_802B697C();
			break;
		case HELD_THROWN:
			func_802B6AC0();
			break;
		case HELD_DROPPED:
			func_802B6AC0();
			break;
	}

	obj_align_gfx_with_floor();
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
	if(gCurrLevelNum == LEVEL_BOWSER_2)
		level = 1;
	else if(gCurrLevelNum == LEVEL_BOWSER_3)
		level = 2;
	else
		level = 0;
	o->oBehParams2ndByte = level;
	o->oBowserUnk1B2     = D_8032F690[level];
	o->oHealth	     = D_8032F694[level];
	obj_start_cam_event(o, CAM_EVENT_BOWSER_INIT);
	o->oAction	   = 5;
	o->oBowserUnk1AE   = 0;
	o->oBowserEyesShut = 0;
}

#undef BITDW
#undef BITFS
#undef BITS

Gfx* Geo18_802B798C(s32 run, UNUSED struct GraphNode* node, Mat4 mtx)
{
	Mat4 sp20;
	struct Object* sp1C;

	if(run == TRUE)
	{
		sp1C = (struct Object*)gCurGraphNodeObject;
		if(sp1C->prevObj != NULL)
		{
			create_transformation_from_matrices(sp20, mtx, *(Mat4*)gCurGraphNodeCamera->matrixPtr);
			obj_update_pos_from_parent_transformation(sp20, sp1C->prevObj);
			obj_set_gfx_pos_from_pos(sp1C->prevObj);
		}
	}
	return NULL;
}

void func_802B70C8(struct Object* a0, struct GraphNodeSwitchCase* switchCase)
{
	s32 sp1C;
	s16 sp1A;
	sp1A = abs_angle_diff(a0->oMoveAngleYaw, a0->oAngleToMario);
	sp1C = switchCase->selectedCase;
	switch(sp1C)
	{
		case 0:
			if(sp1A > 0x2000)
			{
				if(a0->oAngleVelYaw > 0)
				{
					switchCase->selectedCase = 5;
				}

				if(a0->oAngleVelYaw < 0)
				{
					switchCase->selectedCase = 3;
				}
			}
			if(a0->oBowserUnk1AE > 50)
				switchCase->selectedCase = 1;
			break;
		case 1:
			if(a0->oBowserUnk1AE > 2)
				switchCase->selectedCase = 2;
			break;
		case 2:
			if(a0->oBowserUnk1AE > 2)
				switchCase->selectedCase = 9;
			break;
		case 9:
			if(a0->oBowserUnk1AE > 2)
				switchCase->selectedCase = 0;
			break;
		case 5:
			if(a0->oBowserUnk1AE > 2)
			{
				switchCase->selectedCase = 6;
				if(a0->oAngleVelYaw <= 0)
					switchCase->selectedCase = 0;
			}
			break;
		case 6:
			if(a0->oAngleVelYaw <= 0)
				switchCase->selectedCase = 5;
			break;
		case 3:
			if(a0->oBowserUnk1AE > 2)
			{
				switchCase->selectedCase = 4;
				if(a0->oAngleVelYaw >= 0)
					switchCase->selectedCase = 0;
			}
			break;
		case 4:
			if(a0->oAngleVelYaw >= 0)
				switchCase->selectedCase = 3;
			break;
		default:
			switchCase->selectedCase = 0;
	}
	if(switchCase->selectedCase != sp1C)
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
				func_802B70C8(obj, switchCase);
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
		sp28 = sp2C = (Gfx*)alloc_display_list(2 * sizeof(Gfx));

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
	o->oPlatformUnkF8 = obj_nearest_object_with_behavior(sm64::bhv::bhvBowser());
	set_object_collision_data(o, D_8032F698[o->oBehParams2ndByte].unk0);
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
		PlaySound2(SOUND_GENERAL_BOWSER_PLATFORM_2);
	if(o->oTimer < 22 * FRAME_RATE_SCALER_INV)
	{
		set_environmental_camera_shake(SHAKE_ENV_FALLING_BITS_PLAT);
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
		func_802AA618(4, 0, 100.0f);
		o->oPosX = D_8032F698[o->oBehParams2ndByte].unk1 + sins(sp22 - 5296) * sp1C;
		o->oPosZ = D_8032F698[o->oBehParams2ndByte].unk2 + coss(sp22 - 5296) * sp1C;
		func_802AA618(4, 0, 100);
		func_802B8F7C(&o->oPosX, sp24);
	}
	obj_move_using_fvel_and_gravity();
	if(o->oTimer > 300 * FRAME_RATE_SCALER_INV)
		mark_object_for_deletion(o);
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
	obj_call_action_function(sFallingBowserPlatformActions);
}

void func_802B7A58()
{
	mark_object_for_deletion(o);
	spawn_object_with_scale(o, MODEL_NONE, sm64::bhv::bhvBlackSmokeUpward(), 1.0f);
	if(RandomFloat() < 0.1)
		spawn_object(o, MODEL_YELLOW_COIN, sm64::bhv::bhvTemporaryYellowCoin());
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
	o->oAnimState	 = (s32)(RandomFloat() * 10.0f);
	o->oMoveAngleYaw = RandomU16();
	if(RandomFloat() < 0.2)
		o->oVelY = 80.0f;
	else
		o->oVelY = 20.0f;
	o->oForwardVel = 10.0f;
	o->oGravity    = -1.0f;
	o->oFlameScale = RandomFloat() + 1.0f;
}

void bhv_flame_large_burning_out_init()
{
	o->oAnimState	 = (s32)(RandomFloat() * 10.0f);
	o->oMoveAngleYaw = RandomU16();
	o->oVelY	 = 10.0f;
	o->oForwardVel	 = 0.0f;
	o->oFlameScale	 = 7.0f;
}

void func_802B7CFC()
{
	s32 sp4;
	sp4 = ((o->oFlameUnkF8 + (gGlobalTimer / FRAME_RATE_SCALER_INV)) & 0x3F) << 10;
	o->oPosX += sins(o->oMoveAngleYaw) * sins(sp4) * 4.0f * FRAME_RATE_SCALER;
	o->oPosZ += coss(o->oMoveAngleYaw) * sins(sp4) * 4.0f * FRAME_RATE_SCALER;
}

void bhv_flame_bowser_loop()
{
	obj_update_floor_and_walls();
	obj_move_standard(78);

	if(o->oVelY < -4.0f)
		o->oVelY = -4.0f;

	if(o->oAction == 0)
	{
		obj_become_intangible();
		func_802B7CFC();

		if(o->oMoveFlags & 1)
		{
			o->oAction++;
			if(obj_has_behavior(sm64::bhv::bhvFlameLargeBurningOut()))
				o->oFlameScale = 8.0f;
			else
				o->oFlameScale = RandomFloat() * 2 + 6.0f;
			o->oForwardVel = 0;
			o->oVelY       = 0;
			o->oGravity    = 0;
		}
	}
	else
	{
		obj_become_tangible();
		if(o->oTimer > (o->oFlameScale * 10 + 5.0f) * FRAME_RATE_SCALER_INV)
		{
			o->oFlameScale -= 0.15 * FRAME_RATE_SCALER;
			if(o->oFlameScale <= 0.0f)
				func_802B7A58();
		}
	}

	obj_scale(o->oFlameScale);
	o->oGraphYOffset = o->header.gfx.scale[1] * 14.0f;
	set_object_hitbox(o, &sBowserFlameHitbox);
}

void bhv_flame_moving_forward_growing_init()
{
	o->oForwardVel = 30.0f;
	translate_object_xz_random(o, 80.0f);
	o->oAnimState  = (s32)(RandomFloat() * 10.0f);
	o->oFlameScale = 3.0f;
}

void bhv_flame_moving_forward_growing_loop()
{
	UNUSED s32 unused;
	UNUSED struct Object* sp18;
	set_object_hitbox(o, &sGrowingBowserFlameHitbox);
	o->oFlameScale = o->oFlameScale + 0.5 * FRAME_RATE_SCALER;
	obj_scale(o->oFlameScale);
	if(o->oMoveAnglePitch > 0x800)
		o->oMoveAnglePitch -= 0x200 / FRAME_RATE_SCALER_INV;
	func_802A2A38();
	obj_update_floor_height();
	if(o->oFlameScale > 30.0f)
		mark_object_for_deletion(o);
	if(o->oPosY < o->oFloorHeight)
	{
		o->oPosY = o->oFloorHeight;
		sp18	 = spawn_object(o, MODEL_RED_FLAME, sm64::bhv::bhvFlameBowser());
		mark_object_for_deletion(o);
	}
}

void bhv_flame_floating_landing_init()
{
	o->oAnimState	 = (s32)(RandomFloat() * 10.0f);
	o->oMoveAngleYaw = RandomU16();
	if(o->oBehParams2ndByte != 0)
		o->oForwardVel = RandomFloat() * 5.0f;
	else
		o->oForwardVel = RandomFloat() * 70.0f;
	o->oVelY       = RandomFloat() * 20.0f;
	o->oGravity    = -1.0f;
	o->oFlameUnkF8 = RandomFloat() * 64.0f;
}

void bhv_flame_floating_landing_loop()
{
	UNUSED s32 unused;
	obj_update_floor_and_walls();
	obj_move_standard(0x4e);
	func_802B7CFC();
	if(func_802B7ADC(900))
		mark_object_for_deletion(o);
	if(o->oVelY < D_8032F748[o->oBehParams2ndByte])
		o->oVelY = D_8032F748[o->oBehParams2ndByte];
	if(o->oMoveFlags & 1)
	{
		if(o->oBehParams2ndByte == 0)
			spawn_object(o, MODEL_RED_FLAME, sm64::bhv::bhvFlameLargeBurningOut());
		else
			spawn_object(o, MODEL_NONE, sm64::bhv::bhvBlueFlamesGroup()); //? wonder if they meant MODEL_BLUE_FLAME?
		mark_object_for_deletion(o);
	}
	o->oGraphYOffset = o->header.gfx.scale[1] * 14.0f;
}

void bhv_blue_bowser_flame_init()
{
	translate_object_xz_random(o, 80.0f);
	o->oAnimState  = (s32)(RandomFloat() * 10.0f);
	o->oVelY       = 7.0f;
	o->oForwardVel = 35.0f;
	o->oFlameScale = 3.0f;
	o->oFlameUnkFC = RandomFloat() * 0.5;
	o->oGravity    = 1.0f;
	o->oFlameUnkF8 = (s32)(RandomFloat() * 64.0f);
}

void bhv_blue_bowser_flame_loop()
{
	s32 i;
	set_object_hitbox(o, &sGrowingBowserFlameHitbox);
	if(o->oFlameScale < 16.0f)
		o->oFlameScale = o->oFlameScale + (0.5 * FRAME_RATE_SCALER);

	obj_scale(o->oFlameScale);
	obj_update_floor_and_walls();
	obj_move_standard(0x4e);

	if(o->oTimer > 0x14 * FRAME_RATE_SCALER_INV)
	{
		if(o->oBehParams2ndByte == 0)
			for(i = 0; i < 3; i++)
				spawn_object_relative_with_scale(0, 0, 0, 0, 5.0f, o, MODEL_RED_FLAME, sm64::bhv::bhvFlameFloatingLanding());
		else
		{
			spawn_object_relative_with_scale(1, 0, 0, 0, 8.0f, o, MODEL_BLUE_FLAME, sm64::bhv::bhvFlameFloatingLanding());
			spawn_object_relative_with_scale(2, 0, 0, 0, 8.0f, o, MODEL_BLUE_FLAME, sm64::bhv::bhvFlameFloatingLanding());
		}
		mark_object_for_deletion(o);
	}
}

void bhv_flame_bouncing_init()
{
	o->oAnimState  = (s32)(RandomFloat() * 10.0f);
	o->oVelY       = 30.0f;
	o->oForwardVel = 20.0f;
	o->oFlameScale = o->header.gfx.scale[0];
	o->oFlameUnkF8 = (s32)(RandomFloat() * 64.0f);
}

void bhv_flame_bouncing_loop()
{
	struct Object* sp1C;
	if(o->oTimer == 0)
		o->oFlameBowserObj = obj_nearest_object_with_behavior(sm64::bhv::bhvBowser());
	sp1C	       = o->oFlameBowserObj;
	o->oForwardVel = 15.0f;
	o->oBounce     = -1.0f;
	obj_scale(o->oFlameScale);
	set_object_hitbox(o, &sGrowingBowserFlameHitbox);
	obj_update_floor_and_walls();
	obj_move_standard(78);
	if(func_802B7ADC(300))
		mark_object_for_deletion(o);

	if(sp1C != NULL)
		if(sp1C->oHeldState == 0)
			if(lateral_dist_between_objects(o, sp1C) < 300.0f)
				mark_object_for_deletion(o);
}

void bhv_blue_flames_group_loop()
{
	struct Object* flame;
	s32 i;

	if(o->oTimer == 0)
	{
		o->oMoveAngleYaw   = angle_to_object(o, gMarioObject);
		o->oBlueFlameUnkF8 = 5.0f;
	}

	if(o->oTimer < 16 * FRAME_RATE_SCALER_INV)
	{
		if((o->oTimer % (2 * FRAME_RATE_SCALER_INV)) == 0)
		{
			for(i = 0; i < 3; i++)
			{
				flame = spawn_object(o, MODEL_BLUE_FLAME, sm64::bhv::bhvFlameBouncing());
				flame->oMoveAngleYaw += (i * 0x5555) / FRAME_RATE_SCALER_INV;
				flame->header.gfx.scale[0] = o->oBlueFlameUnkF8;
			}
			o->oBlueFlameUnkF8 -= 0.5 * FRAME_RATE_SCALER;
		}
	}
	else
		mark_object_for_deletion(o);
}
