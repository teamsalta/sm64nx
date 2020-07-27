// kickable_board.c.inc

s32 func_802A9A0C(UNUSED s32 sp18)
{
	if(s_hitcheck(o, gMarioObject))
	{
		if(s_calc_dangle(o->oMoveAngleYaw, gMarioObject->oMoveAngleYaw) > 0x6000)
		{
			if(playerWorks->status == ACT_SLIDE_KICK)
				return 1;
			if(playerWorks->status == ACT_PUNCHING)
				return 1;
			if(playerWorks->status == ACT_MOVE_PUNCHING)
				return 1;
			if(playerWorks->status == ACT_SLIDE_KICK_SLIDE)
				return 1;
			if(playerWorks->status == ACT_JUMP_KICK)
				return 2;
			if(playerWorks->status == ACT_WALL_KICK_AIR)
				return 2;
		}
	}
	return 0;
}

void func_802A9B54(void)
{
	o->oKickableBoardF8 = 1600;
	o->oKickableBoardF4 = 0;
}

void bhv_kickable_board_loop(void)
{
	s32 sp24;
	switch(o->oAction)
	{
		case 0:
			o->oFaceAnglePitch = 0;
			if(func_802A9A0C(0))
			{
				func_802A9B54();
				o->oAction++;
			}
			stMainMoveBG();
			break;
		case 1:
			o->oFaceAnglePitch = 0;
			stMainMoveBG();
			o->oFaceAnglePitch = -sins(o->oKickableBoardF4) * o->oKickableBoardF8;
			if(o->oTimer > 30 * FRAME_RATE_SCALER_INV && (sp24 = func_802A9A0C(0)))
			{
				if(gMarioObject->oPosY > o->oPosY + 160.0f && sp24 == 2)
				{
					o->oAction++;
					objsound(SOUND_GENERAL_BUTTON_PRESS_2);
				}
				else
					o->oTimer = 0;
			}
			if(o->oTimer != 0)
			{
				o->oKickableBoardF8 -= 8 / FRAME_RATE_SCALER_INV;
				if(o->oKickableBoardF8 < 0)
					o->oAction = 0;
			}
			else
				func_802A9B54();
			if(!(o->oKickableBoardF4 & 0x7FFF))
				objsound(SOUND_GENERAL_BUTTON_PRESS_2);
			o->oKickableBoardF4 += 0x400 / FRAME_RATE_SCALER_INV;
			break;
		case 2:
			s_hitOFF();
			s_change_shape(MODEL_WF_KICKABLE_BOARD_FELLED);
			o->oAngleVelPitch -= 0x80 / FRAME_RATE_SCALER_INV;
			o->oFaceAnglePitch += o->oAngleVelPitch * FRAME_RATE_SCALER;
			if(o->oFaceAnglePitch < -0x4000)
			{
				o->oFaceAnglePitch = -0x4000;
				o->oAngleVelPitch  = 0;
				o->oAction++;
				s_call_Viewshake(SHAKE_POS_SMALL);
				objsound(SOUND_GENERAL_UNKNOWN4);
			}
			stMainMoveBG();
			break;
		case 3:
			stMainMoveBG();
			break;
	}
	o->header.gfx.throwMatrix = NULL;
}
