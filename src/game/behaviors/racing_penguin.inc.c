struct RacingPenguinData
{
	s16 text;
	f32 radius;
	f32 height;
};

static struct RacingPenguinData sRacingPenguinData[] = {
    {DIALOG_055, 200.0f, 200.0f},
    {DIALOG_164, 350.0f, 250.0f},
};

void bhv_racing_penguin_init(void)
{
	if(marioWorks->numStars == 120)
	{
		s_set_scale(8.0f);
		o->header.gfx.scale[1] = 5.0f;
		o->oBehParams2ndByte   = 1;
	}
}

static void racing_penguin_act_wait_for_mario(void)
{
	if(o->oTimer > o->oRacingPenguinInitTextCooldown * FRAME_RATE_SCALER_INV && o->oPosY - gMarioObject->oPosY <= 0.0f && obj_is_mario_in_range_and_ready_to_speak(400.0f, 400.0f))
	{
		o->oAction = RACING_PENGUIN_ACT_SHOW_INIT_TEXT;
	}
}

static void racing_penguin_act_show_init_text(void)
{
	s32 response;
	struct Object* child;

	response = obj_update_race_proposition_dialog(sRacingPenguinData[o->oBehParams2ndByte].text);
	if(response == 1)
	{
		child		 = s_find_obj(sm64::bhv::bhvPenguinRaceFinishLine());
		child->parentObj = o;

		child		 = s_find_obj(sm64::bhv::bhvPenguinRaceShortcutCheck());
		child->parentObj = o;

		o->oPathedStartWaypoint = o->oPathedPrevWaypoint = (Waypoint*)segmented_to_virtual(ccm_seg7_trajectory_penguin_race);
		o->oPathedPrevWaypointFlags			 = 0;

		o->oAction = RACING_PENGUIN_ACT_PREPARE_FOR_RACE;
		o->oVelY   = 60.0f;
		;
	}
	else if(response == 2)
	{
		o->oAction			  = RACING_PENGUIN_ACT_WAIT_FOR_MARIO;
		o->oRacingPenguinInitTextCooldown = 60;
	}
}

static void racing_penguin_act_prepare_for_race(void)
{
	if(obj_begin_race(TRUE))
	{
		o->oAction     = RACING_PENGUIN_ACT_RACE;
		o->oForwardVel = 20.0f;
	}

	s_chase_angleY(0x4000, 2500 / FRAME_RATE_SCALER_INV);
}

static void racing_penguin_act_race(void)
{
	f32 targetSpeed;
	f32 minSpeed;

	if(obj_follow_path(0) == PATH_REACHED_END)
	{
		o->oRacingPenguinReachedBottom = TRUE;
		o->oAction		       = RACING_PENGUIN_ACT_FINISH_RACE;
	}
	else
	{
		targetSpeed = o->oPosY - gMarioObject->oPosY;
		minSpeed    = 70.0f;

		objsound_level(SOUND_AIR_ROUGH_SLIDE);

		if(targetSpeed < 100.0f || (o->oPathedPrevWaypointFlags & WAYPOINT_MASK_00FF) >= 35)
		{
			if((o->oPathedPrevWaypointFlags & WAYPOINT_MASK_00FF) >= 35)
			{
				minSpeed = 60.0f;
			}

			approach_f32_ptr(&o->oRacingPenguinWeightedNewTargetSpeed, -500.0f, 100.0f * FRAME_RATE_SCALER);
		}
		else
		{
			approach_f32_ptr(&o->oRacingPenguinWeightedNewTargetSpeed, 1000.0f, 30.0f * FRAME_RATE_SCALER);
		}

		targetSpeed = 0.1f * (o->oRacingPenguinWeightedNewTargetSpeed + targetSpeed);
		clamp_f32(&targetSpeed, minSpeed, 150.0f);
		obj_forward_vel_approach(targetSpeed, 0.4f * FRAME_RATE_SCALER);

		s_set_skelanimeNo(1);
		s_chase_angleY(o->oPathedTargetYaw, (s32)(15.0f * FRAME_RATE_SCALER * o->oForwardVel));

		if(func_8029F828() && (o->oMoveFlags & 0x00000003))
		{
			s_makeobj_chain_scale(0, 0, -100, 0, 4.0f, o, MODEL_SMOKE, sm64::bhv::bhvWhitePuffSmoke2());
		}
	}

	if(mario_is_in_air_action())
	{
		if(o->oTimer > 60 * FRAME_RATE_SCALER_INV)
		{
			o->oRacingPenguinMarioCheated = TRUE;
		}
	}
	else
	{
		o->oTimer = 0;
	}
}

static void racing_penguin_act_finish_race(void)
{
	if(o->oForwardVel != 0.0f)
	{
		if(o->oTimer > 5 * FRAME_RATE_SCALER_INV && (o->oMoveFlags & 0x00000200))
		{
			objsound(SOUND_OBJ_POUNDING_LOUD);
			set_camera_shake_from_point(SHAKE_POS_SMALL, o->oPosX, o->oPosY, o->oPosZ);
			o->oForwardVel = 0.0f;
		}
	}
	else if(func_802F92B0(2) != 0)
	{
		o->oAction = RACING_PENGUIN_ACT_SHOW_FINAL_TEXT;
	}
}

static void racing_penguin_act_show_final_text(void)
{
	s32 textResult;

	if(o->oRacingPenguinFinalTextbox == 0)
	{
		if(s_chase_angleY(0, 200 / FRAME_RATE_SCALER_INV))
		{
			s_set_skelanimeNo(3);
			o->oForwardVel = 0.0f;

			if(obj_is_mario_in_range_and_ready_to_speak(400.0f, 400.0f))
			{
				if(o->oRacingPenguinMarioWon)
				{
					if(o->oRacingPenguinMarioCheated)
					{
						o->oRacingPenguinFinalTextbox = DIALOG_132;
						o->oRacingPenguinMarioWon     = FALSE;
					}
					else
					{
						o->oRacingPenguinFinalTextbox = DIALOG_056;
					}
				}
				else
				{
					o->oRacingPenguinFinalTextbox = DIALOG_037;
				}
			}
		}
		else
		{
			s_set_skelanimeNo(0);

			play_penguin_walking_sound(1);

			o->oForwardVel = 4.0f;
		}
	}
	else if(o->oRacingPenguinFinalTextbox > 0)
	{
		if((textResult = obj_update_dialog_with_cutscene(2, 1, CUTSCENE_DIALOG, o->oRacingPenguinFinalTextbox)) != 0)
		{
			o->oRacingPenguinFinalTextbox = -1;
			o->oTimer		      = 0;
		}
	}
	else if(o->oRacingPenguinMarioWon)
	{
		obj_spawn_star_at_y_offset(-7339.0f, -5700.0f, -6774.0f, 200.0f);
		o->oRacingPenguinMarioWon = FALSE;
	}
}

void bhv_racing_penguin_update(void)
{
	s_enemybgcheck();

	switch(o->oAction)
	{
		case RACING_PENGUIN_ACT_WAIT_FOR_MARIO:
			racing_penguin_act_wait_for_mario();
			break;
		case RACING_PENGUIN_ACT_SHOW_INIT_TEXT:
			racing_penguin_act_show_init_text();
			break;
		case RACING_PENGUIN_ACT_PREPARE_FOR_RACE:
			racing_penguin_act_prepare_for_race();
			break;
		case RACING_PENGUIN_ACT_RACE:
			racing_penguin_act_race();
			break;
		case RACING_PENGUIN_ACT_FINISH_RACE:
			racing_penguin_act_finish_race();
			break;
		case RACING_PENGUIN_ACT_SHOW_FINAL_TEXT:
			racing_penguin_act_show_final_text();
			break;
	}

	s_enemymove(78);
	s_set_mtxmode();
	s_player_slideout_RH(sRacingPenguinData[o->oBehParams2ndByte].radius, sRacingPenguinData[o->oBehParams2ndByte].height);
}

void bhv_penguin_race_finish_line_update(void)
{
	if(o->parentObj->oRacingPenguinReachedBottom || (o->oDistanceToMario < 1000.0f && gMarioObject->oPosZ - o->oPosZ < 0.0f))
	{
		if(!o->parentObj->oRacingPenguinReachedBottom)
		{
			o->parentObj->oRacingPenguinMarioWon = TRUE;
		}
	}
}

void bhv_penguin_race_shortcut_check_update(void)
{
	if(o->oDistanceToMario < 500.0f)
	{
		o->parentObj->oRacingPenguinMarioCheated = TRUE;
	}
}
