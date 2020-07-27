// flying_bookend_switch.inc.c

struct Struct80331B30
{
	s16 unk00;
	s16 unk02;
};

struct ObjectHitbox sFlyingBookendHitbox = {
    /* interactType:      */ INTERACT_HIT_FROM_BELOW,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 2,
    /* health:            */ 0,
    /* numLootCoins:      */ -1,
    /* radius:            */ 60,
    /* height:            */ 30,
    /* hurtboxRadius:     */ 40,
    /* hurtboxHeight:     */ 30,
};

struct Struct80331B30 D_80331B30[] = {
    {52, 150},
    {135, 3},
    {-75, 78},
};

struct ObjectHitbox sBookSwitchHitbox = {
    /* interactType:      */ INTERACT_BREAKABLE,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 99,
    /* numLootCoins:      */ 0,
    /* radius:            */ 20,
    /* height:            */ 30,
    /* hurtboxRadius:     */ 20,
    /* hurtboxHeight:     */ 30,
};

void flying_bookend_act_0(void)
{
	if(obj_is_near_to_and_facing_mario(400.0f, 0x3000))
	{
		objsound(SOUND_OBJ_DEFAULT_DEATH);
		o->oAction	 = 1;
		o->oBookendUnkF4 = o->oFaceAnglePitch + 0x7FFF;
		o->oBookendUnkF8 = o->oFaceAngleRoll - 0x7FFF;
		s_change_shape(MODEL_BOOKEND_PART);
	}
}

void flying_bookend_act_1(void)
{
	if(obj_forward_vel_approach(3.0f, 1.0f * FRAME_RATE_SCALER))
	{
		if(func_802F92B0(2))
		{
			o->oAction     = 2;
			o->oForwardVel = 0.0f;
		}
		else
		{
			o->oForwardVel = 3.0f;
			if(o->oTimer > 5 * FRAME_RATE_SCALER_INV)
			{
				obj_face_pitch_approach(o->oBookendUnkF4, 2000 / FRAME_RATE_SCALER_INV);
				if(o->oTimer >= 10 * FRAME_RATE_SCALER_INV)
				{
					obj_face_roll_approach(o->oBookendUnkF8, 2000 / FRAME_RATE_SCALER_INV);
					if(o->oTimer >= 20 * FRAME_RATE_SCALER_INV)
					{
						approach_f32_ptr(&o->header.gfx.scale[0], 3.0f, 0.2f * FRAME_RATE_SCALER);
					}
				}
			}
		}
	}

	s_optionmove_F();
}

void flying_bookend_act_2(void)
{
	set_obj_animation_and_sound_state(1);
	obj_update_floor_and_walls();

	if(o->oForwardVel == 0.0f)
	{
		obj_turn_pitch_toward_mario(120.0f, 1000 / FRAME_RATE_SCALER_INV);
		o->oFaceAnglePitch = o->oMoveAnglePitch + 0x7FFF;
		obj_rotate_yaw_toward(o->oAngleToMario, 1000 / FRAME_RATE_SCALER_INV);

		if(o->oTimer > 30 * FRAME_RATE_SCALER_INV)
		{
			obj_compute_vel_from_move_pitch(50.0f);
		}
	}

	obj_move_standard(78);
}

void flying_bookend_act_3(void)
{
	o->oDamageOrCoinValue = 1;
	o->oNumLootCoins      = 0;

	if(o->oTimer >= 4 * FRAME_RATE_SCALER_INV)
	{
		o->oAction     = 2;
		o->oForwardVel = 50.0f;
	}

	obj_forward_vel_approach(50.0f, 2.0f * FRAME_RATE_SCALER);
	s_optionmove_F();
}

void bhv_flying_bookend_loop(void)
{
	if(!(o->activeFlags & 0x0008))
	{
		o->oDeathSound = SOUND_OBJ_POUNDING1;
		s_set_scale(o->header.gfx.scale[0]);

		switch(o->oAction)
		{
			case 0:
				flying_bookend_act_0();
				break;
			case 1:
				flying_bookend_act_1();
				break;
			case 2:
				flying_bookend_act_2();
				break;
			case 3:
				flying_bookend_act_3();
				break;
		}

		obj_check_attacks(&sFlyingBookendHitbox, -1);
		if(o->oAction == -1 || (o->oMoveFlags & 0x00000203))
		{
			o->oNumLootCoins = 0;
			obj_die_if_health_non_positive();
		}

		o->oGraphYOffset = 30.0f * o->header.gfx.scale[0];
	}
}

void bhv_bookend_spawn_loop(void)
{
	struct Object* sp1C;

	if(!(o->activeFlags & 0x0008))
	{
		if(o->oTimer > 40 * FRAME_RATE_SCALER_INV && obj_is_near_to_and_facing_mario(600.0f, 0x2000))
		{
			sp1C = s_makeobj_nowpos(o, MODEL_BOOKEND, sm64::bhv::bhvFlyingBookend());
			if(sp1C != NULL)
			{
				sp1C->oAction = 3;
				objsound(SOUND_OBJ_DEFAULT_DEATH);
			}
			o->oTimer = 0;
		}
	}
}

void bookshelf_manager_act_0(void)
{
	s32 val04;

	if(!(o->activeFlags & 0x0008))
	{
		for(val04 = 0; val04 < 3; val04++)
		{
			s_makeobj_chain(val04, D_80331B30[val04].unk00, D_80331B30[val04].unk02, 0, o, MODEL_BOOKEND, sm64::bhv::bhvBookSwitch());
		}

		o->oAction = 1;
	}
}

void bookshelf_manager_act_1(void)
{
	if(o->oBookSwitchManagerUnkF8 == 0)
	{
		if(obj_is_near_to_and_facing_mario(500.0f, 0x3000))
		{
			o->oBookSwitchManagerUnkF8 = 1;
		}
	}
	else if(o->oTimer > 60 * FRAME_RATE_SCALER_INV)
	{
		o->oAction		   = 2;
		o->oBookSwitchManagerUnkF8 = 0;
	}
}

void bookshelf_manager_act_2(void)
{
	if(!(o->activeFlags & 0x0008))
	{
		if(o->oBookSwitchManagerUnkF4 < 0)
		{
			if(o->oTimer > 30 * FRAME_RATE_SCALER_INV)
			{
				o->oBookSwitchManagerUnkF4 = o->oBookSwitchManagerUnkF8 = 0;
			}
			else if(o->oTimer > 10 * FRAME_RATE_SCALER_INV)
			{
				o->oBookSwitchManagerUnkF8 = 1;
			}
		}
		else
		{
			if(o->oBookSwitchManagerUnkF4 >= 3)
			{
				if(o->oTimer > 100 * FRAME_RATE_SCALER_INV)
				{
					o->parentObj	      = s_find_obj(sm64::bhv::bhvHauntedBookshelf());
					o->parentObj->oAction = 1;
					o->oPosX	      = o->parentObj->oPosX;
					o->oAction	      = 3;
				}
				else if(o->oTimer == 30 * FRAME_RATE_SCALER_INV)
				{
					Na_NazoClearBgm();
				}
			}
			else
			{
				o->oTimer = 0;
			}
		}
	}
	else
	{
		o->oAction = 4;
	}
}

void bookshelf_manager_act_3(void)
{
	if(o->oTimer > 85 * FRAME_RATE_SCALER_INV)
	{
		o->oAction = 4;
	}
	else
	{
		o->oForwardVel = o->parentObj->oPosX - o->oPosX;
		o->oPosX       = o->parentObj->oPosX;
	}
}

void bookshelf_manager_act_4(void)
{
	if(o->oBookSwitchManagerUnkF4 >= 3)
	{
		s_remove_obj(o);
	}
	else
	{
		o->oAction = 0;
	}
}

void bhv_haunted_bookshelf_manager_loop(void)
{
	switch(o->oAction)
	{
		case 0:
			bookshelf_manager_act_0();
			break;
		case 2:
			bookshelf_manager_act_2();
			break;
		case 1:
			bookshelf_manager_act_1();
			break;
		case 3:
			bookshelf_manager_act_3();
			break;
		case 4:
			bookshelf_manager_act_4();
			break;
	}
}

void bhv_book_switch_loop(void)
{
	s32 sp3C;
	struct Object* sp38;
	s16 sp36;
	s16 sp34;

	o->header.gfx.scale[0] = 2.0f;
	o->header.gfx.scale[1] = 0.9f;

	if(o->parentObj->oAction == 4)
	{
		s_remove_obj(o);
	}
	else
	{
		sp3C = obj_check_attacks(&sBookSwitchHitbox, o->oAction);
		if(o->parentObj->oBookSwitchManagerUnkF8 != 0 || o->oAction == 1)
		{
			if(o->oDistanceToMario < 100.0f)
			{
				s_hitON();
			}
			else
			{
				s_hitOFF();
			}

			o->oAction = 1;
			if(o->oBookSwitchUnkF4 == 0.0f)
			{
				objsound(SOUND_OBJ_DEFAULT_DEATH);
			}

			if(approach_f32_ptr(&o->oBookSwitchUnkF4, 50.0f, 20.0f * FRAME_RATE_SCALER))
			{
				if(o->parentObj->oBookSwitchManagerUnkF4 >= 0 && o->oTimer > 60 * FRAME_RATE_SCALER_INV)
				{
					if(sp3C == 1 || sp3C == 2 || sp3C == 6)
					{
						o->oAction = 2;
					}
				}
			}
			else
			{
				o->oTimer = 0;
			}
		}
		else
		{
			s_hitOFF();
			if(approach_f32_ptr(&o->oBookSwitchUnkF4, 0.0f, 20.0f * FRAME_RATE_SCALER))
			{
				if(o->oAction != 0)
				{
					if(o->parentObj->oBookSwitchManagerUnkF4 == o->oBehParams2ndByte)
					{
						AudStartSound(SOUND_GENERAL2_RIGHT_ANSWER, gDefaultSoundArgs);
						o->parentObj->oBookSwitchManagerUnkF4 += 1;
					}
					else
					{
						sp36 = RandomU16() & 0x1;
						sp34 = gMarioObject->oPosZ + 1.5f * playerWorks[0].vel[2];

						AudStartSound(SOUND_MENU_CAMERA_BUZZ, gDefaultSoundArgs);
						if(sp34 > 0)
						{
							sp34 = 0;
						}

						sp38 = s_makeobj_absolute(o, 0, MODEL_BOOKEND, sm64::bhv::bhvFlyingBookend(), 0x1FC * sp36 - 0x8CA, 890, sp34, 0, 0x8000 * sp36 + 0x4000, 0);

						if(sp38 != NULL)
						{
							sp38->oAction = 3;
						}

						o->parentObj->oBookSwitchManagerUnkF4 = -1;
					}

					o->oAction = 0;
				}
			}
		}

		o->oPosX += o->parentObj->oForwardVel * FRAME_RATE_SCALER;
		o->oPosZ = o->oHomeZ - o->oBookSwitchUnkF4;
		s_player_slideout_RH(70.0f, 70.0f);
	}
}
