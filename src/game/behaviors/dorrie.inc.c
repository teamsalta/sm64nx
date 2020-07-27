
static void dorrie_raise_head(void)
{
	const u8 firstFrame = 50;
	const u8 lastFrame  = 74;

	const float startHeight = 340.81152f;
	const float endHeight	= 715.02f;

	const float endLength	= 550.0f;
	const float startLength = 710.0f;

	s32 animFrame	  = o->header.gfx.unk38.frame();
	float neckPercent = (animFrame - firstFrame) / float(lastFrame - firstFrame);

	if(neckPercent < 0.0f)
	{
		neckPercent = 0.0f;
	}
	else if(neckPercent > 1.0f)
	{
		neckPercent = 1.0f;
	}

	float s = sin(neckPercent * 3.141592653f / 2.0f);

	float length = startLength + ((endLength - startLength) * s);

	float newX = o->oPosX + length * sins(o->oMoveAngleYaw);
	float newY = o->oPosY + startHeight + ((endHeight - startHeight) * s);
	float newZ = o->oPosZ + length * coss(o->oMoveAngleYaw);

	set_mario_pos(newX, newY, newZ);
}

static void dorrie_act_move(void)
{
	s16 startYaw;
	s16 targetYaw;
	s16 targetSpeed;
	s16 circularTurn;

	startYaw	    = o->oMoveAngleYaw;
	o->oDorrieNeckAngle = -0x26F4;
	s_set_skelanimeNo(1);

	if(o->oDorrieForwardDistToMario < 320.0f && o->oDorrieGroundPounded)
	{
		objsound(SOUND_OBJ_DORRIE);
		o->collisionData = segmented_to_virtual(dorrie_seg6_collision_0600FBB8);
		o->oAction	 = DORRIE_ACT_LOWER_HEAD;
		o->oForwardVel	 = 0.0f;
		o->oDorrieYawVel = 0;
	}
	else
	{
		if(gMarioObject->platform == o)
		{
			targetYaw   = gMarioObject->oFaceAngleYaw;
			targetSpeed = 10;
		}
		else
		{
			circularTurn = 0x4000 - atan2s(2000.0f, o->oDorrieDistToHome - 2000.0f);
			if((s16)(o->oMoveAngleYaw - o->oDorrieAngleToHome) < 0)
			{
				circularTurn = -circularTurn;
			}

			targetYaw   = o->oDorrieAngleToHome + circularTurn;
			targetSpeed = 5;
		}

		obj_forward_vel_approach(targetSpeed, 0.5f * FRAME_RATE_SCALER);
		o->oDorrieYawVel = approach_s16_symmetric(o->oDorrieYawVel, (s16)(targetYaw - o->oMoveAngleYaw) / (50 * FRAME_RATE_SCALER_INV), 5 * FRAME_RATE_SCALER);
		o->oMoveAngleYaw += o->oDorrieYawVel * FRAME_RATE_SCALER;
	}

	o->oAngleVelYaw = (o->oMoveAngleYaw - startYaw) * FRAME_RATE_SCALER_INV;
}

static void dorrie_begin_head_raise(s32 liftingMario)
{
	o->oDorrieLiftingMario	 = liftingMario;
	o->oAction		 = DORRIE_ACT_RAISE_HEAD;
	o->oDorrieHeadRaiseSpeed = 0;
}

static void dorrie_act_lower_head(void)
{
	if(func_802F92EC(2, 35) || s_check_animenumber(34))
	{
		func_8029F6F0();

		if(gMarioObject->platform == o)
		{
			if(o->oDorrieOffsetY == -17.0f && o->oDorrieForwardDistToMario > 780.0f && CtrlPlayerDialog(2) == 1)
			{
				dorrie_begin_head_raise(TRUE);
			}
			else if(o->oDorrieForwardDistToMario > 320.0f)
			{
				o->oTimer = 0;
			}
		}
		else if(o->oTimer > 150 * FRAME_RATE_SCALER_INV)
		{
			dorrie_begin_head_raise(FALSE);
		}
	}
	else
	{
		o->oDorrieNeckAngle += 0x115 / FRAME_RATE_SCALER_INV;
	}
}

static void dorrie_act_raise_head(void)
{
	o->collisionData = segmented_to_virtual(dorrie_seg6_collision_0600F644);
	if(s_check_animeend())
	{
		o->oAction = DORRIE_ACT_MOVE;
	}
	else if(o->oDorrieLiftingMario && o->header.gfx.unk38.animFrameAccelAssist < ((74 << 16) - 0x8000))
	{
		if(CtrlPlayerDialog(2) == 2)
		{
			o->oDorrieHeadRaiseSpeed += 0x1CC / FRAME_RATE_SCALER_INV;

			if(s_check_animenumber(73))
			{
				CtrlPlayerDialog(0);
			}
			dorrie_raise_head();
		}
		else
		{
			func_8029F6F0(); // lock frame
		}
	}
	else
	{
		int y = 0;
	}
}

void bhv_dorrie_update(void)
{
	// if(frameCounter % FRAME_RATE_SCALER_INV == 0)
	{
		f32 boundsShift;
		UNUSED s32 unused1;
		UNUSED s32 unused2;
		f32 maxOffsetY;

		if(!(o->activeFlags & 0x0008))
		{
			o->oDorrieForwardDistToMario = o->oDistanceToMario * coss(o->oAngleToMario - o->oMoveAngleYaw);

			obj_perform_position_op(0);
			s_optionmove_F();

			o->oDorrieAngleToHome = obj_angle_to_home();
			o->oDorrieDistToHome  = obj_lateral_dist_to_home();

			// Shift dorrie's bounds to account for her neck
			boundsShift = 440.0f * coss(o->oDorrieNeckAngle) * coss(o->oMoveAngleYaw - o->oDorrieAngleToHome);

			if(clamp_f32(&o->oDorrieDistToHome, 1650.0f + boundsShift, (2300.0f + boundsShift)) * FRAME_RATE_SCALER)
			{
				o->oPosX = o->oHomeX - o->oDorrieDistToHome * sins(o->oDorrieAngleToHome);
				o->oPosZ = o->oHomeZ - o->oDorrieDistToHome * coss(o->oDorrieAngleToHome);
			}

			o->oDorrieGroundPounded = s_checkplayer_hipaatack();

			if(gMarioObject->platform == o)
			{
				maxOffsetY = -17.0f;
				if(o->oDorrieOffsetY >= 0.0f)
				{
					if(o->oDorrieGroundPounded)
					{
						o->oDorrieVelY = -15.0f;
					}
					else
					{
						o->oDorrieVelY = -6.0f;
					}
				}
			}
			else
			{
				maxOffsetY = 0.0f;
			}

			o->oDorrieOffsetY += o->oDorrieVelY * FRAME_RATE_SCALER;
			approach_f32_ptr(&o->oDorrieVelY, 3.0f, 1.0f * FRAME_RATE_SCALER);
			if(o->oDorrieVelY > 0.0f && o->oDorrieOffsetY > maxOffsetY)
			{
				o->oDorrieOffsetY = maxOffsetY;
			}

			o->oPosY = o->oHomeY + o->oDorrieOffsetY;

			switch(o->oAction)
			{
				case DORRIE_ACT_MOVE:
					dorrie_act_move();
					break;
				case DORRIE_ACT_LOWER_HEAD:
					dorrie_act_lower_head();
					break;
				case DORRIE_ACT_RAISE_HEAD:
					dorrie_act_raise_head();
					break;
			}

			obj_perform_position_op(1);
		}
	}
}
