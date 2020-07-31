// capswitch.c.inc
#include "game/motor.h"

void bikkuri_init(void)
{
	o->oAnimState = o->oBehParams2ndByte;
	s_set_scale(0.5f);
	o->oPosY += 71.0f * FRAME_RATE_SCALER;
	s_makeobj_chain_scale(0, 0, -71, 0, 0.5f, o, MODEL_CAP_SWITCH_BASE, sm64::bhv::bhvCapSwitchBase());
	if(activeStageNo != LEVEL_UNKNOWN_32)
	{
		if(BuGetItemFlag() & D_8032F0C0[o->oBehParams2ndByte])
		{
			o->oAction	       = 3;
			o->header.gfx.scale[1] = 0.1f;
		}
		else
			o->oAction = 1;
	}
	else
		o->oAction = 1;
}

void bikkuri_oncheck(void)
{
	if(s_rideon_player())
	{
		BuSetItemFlag(D_8032F0C0[o->oBehParams2ndByte]);
		o->oAction = 2;
		objsound(SOUND_GENERAL_ACTIVATE_CAP_SWITCH);
	}
}

void bikkuri_on(void)
{
	s32 sp1C;
	if(o->oTimer < 5 * FRAME_RATE_SCALER_INV)
	{
		s_scale_timezoom(2, 4, 0.5f, 0.1f);
		if(o->oTimer == 4 * FRAME_RATE_SCALER_INV)
		{
			s_call_Viewshake(SHAKE_POS_SMALL);
			s_kemuri();
			s_boxeffect(60, 139, 0.3f, o->oBehParams2ndByte);
			SendMotorEvent(5, 80);
		}
	}
	else
	{
		sp1C = s_call_talkdemo(1, 0x0C, CUTSCENE_CAP_SWITCH_PRESS, 0);
		if(sp1C)
			o->oAction = 3;
	}
}

void bikkuri_off()
{
} // dead function

void (*sCapSwitchActions[])(void) = {bikkuri_init, bikkuri_oncheck, bikkuri_on, bikkuri_off};

void bhv_cap_switch_loop(void)
{
	s_modejmp(sCapSwitchActions);
}

Gfx* Geo18_802A719C(s32 run, UNUSED struct GraphNode* node, Mat4 mtx)
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
