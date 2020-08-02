// mushroom_1up.c.inc
#include "game/motor.h"

void OneUp_kinoko_Hit()
{
	UNUSED s32 sp1C;

	if(s_hitcheck(o, gMarioObject) == 1)
	{
		AudStartSound(SOUND_GENERAL_COLLECT_1UP, gDefaultSoundArgs);
		if(!sm64::config().cheats().unlimitedLives())
		{
			marioWorks->numLives++;
		}
		o->activeFlags = 0;
		SendMotorEvent(5, 80);
	}
}

void s_1up_kinoko_init()
{
	o->oMoveAnglePitch = -0x4000;
	o->oGravity	   = 3.0f;
	o->oFriction	   = 1.0f;
	o->oBuoyancy	   = 1.0f;
}

void s_1up_kuppa_init()
{
	s_1up_kinoko_init();
	if(o->oBehParams2ndByte == 1)
	{
		if((BuGetItemFlag() & 0x50) == 0)
			o->activeFlags = 0;
	}
	else if(o->oBehParams2ndByte == 2)
	{
		if((BuGetItemFlag() & 0xa0) == 0)
			o->activeFlags = 0;
	}
}

void OneUp_kinoko_Appear()
{
	if(o->oTimer < 5 * FRAME_RATE_SCALER_INV)
	{
		o->oVelY = 40.0f;
	}
	else
	{
		o->oAngleVelPitch = -0x1000;
		o->oMoveAnglePitch += o->oAngleVelPitch * FRAME_RATE_SCALER;
		o->oVelY       = coss(o->oMoveAnglePitch) * 30.0f + 2.0f;
		o->oForwardVel = -sins(o->oMoveAnglePitch) * 30.0f;
	}
}

void OneUp_kinoko_Get()
{
	f32 dx		 = gMarioObject->header.gfx.pos[0] - o->oPosX;
	f32 dy		 = gMarioObject->header.gfx.pos[1] + 120.0f - o->oPosY;
	f32 dz		 = gMarioObject->header.gfx.pos[2] - o->oPosZ;
	s16 targetAngleX = atan2s(sqrtf(sqr(dx) + sqr(dz)), dy);

	s_chase_obj_angle(o, gMarioObject, 16, 0x1000);
	o->oMoveAnglePitch = s_chase_angle(o->oMoveAnglePitch, targetAngleX, 0x1000 / FRAME_RATE_SCALER_INV);
	o->oVelY	   = sins(o->oMoveAnglePitch) * 30.0f;
	o->oForwardVel	   = coss(o->oMoveAnglePitch) * 30.0f;
	OneUp_kinoko_Hit();
}

void OneUp_kinoko_Escape(s16 move_flag)
{
	o->oForwardVel	 = 8.0f;
	o->oMoveAngleYaw = o->oAngleToMario + 0x8000;
	OneUp_kinoko_Hit();
	if(move_flag & 0x02)
		o->oAction = 2;

	if(!PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 3000))
		o->oAction = 2;
}

void s_1up_kinoko_main()
{
	ObjMoveEvent();

	switch(o->oAction)
	{
		case 0:
			if(o->oTimer >= 18 * FRAME_RATE_SCALER_INV)
				s_makeobj_nowpos(o, MODEL_NONE, sm64::bhv::bhvSparkleSpawn());

			if(o->oTimer == 0)
				AudStartSound(SOUND_GENERAL2_1UP_APPEAR, gDefaultSoundArgs);

			OneUp_kinoko_Appear();

			if(o->oTimer == 37 * FRAME_RATE_SCALER_INV)
			{
				s_hitON();
				o->oAction     = 1;
				o->oForwardVel = 2.0f;
			}
			break;

		case 1:
			if(o->oTimer > 300 * FRAME_RATE_SCALER_INV)
				o->oAction = 2;

			OneUp_kinoko_Hit();
			break;

		case 2:
			iwa_TimerRemove(o, 30);
			OneUp_kinoko_Hit();
			break;
	}

	PlayerApproachOnOff(o, 3000);
}

void s_1up_kinoko_escape_main()
{
	s16 move_flag = ObjMoveEvent();
	switch(o->oAction)
	{
		case 0:
			if(o->oTimer >= 18 * FRAME_RATE_SCALER_INV)
				s_makeobj_nowpos(o, MODEL_NONE, sm64::bhv::bhvSparkleSpawn());

			if(o->oTimer == 0)
				AudStartSound(SOUND_GENERAL2_1UP_APPEAR, gDefaultSoundArgs);

			OneUp_kinoko_Appear();

			if(o->oTimer == 37 * FRAME_RATE_SCALER_INV)
			{
				s_hitON();
				o->oAction     = 1;
				o->oForwardVel = 8.0f;
			}
			break;

		case 1:
			s_makeobj_nowpos(o, MODEL_NONE, sm64::bhv::bhvSparkleSpawn());
			OneUp_kinoko_Escape(move_flag);
			break;

		case 2:
			iwa_TimerRemove(o, 30);
			OneUp_kinoko_Hit();
			break;
	}

	PlayerApproachOnOff(o, 3000);
}

void OneUp_kinoko_Slider()
{
	s16 move_flag = ObjMoveEvent();
	if(move_flag & 0x01)
	{
		o->oForwardVel += 25.0f * FRAME_RATE_SCALER;
		o->oVelY = 0;
	}
	else
	{
		o->oForwardVel *= 0.98;
	}

	if(o->oForwardVel > 40.0)
		o->oForwardVel = 40.0f;

	if(!PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 5000))
		o->oAction = 2;
}

void s_1up_kinoko_slider_main()
{
	switch(o->oAction)
	{
		case 0:
			PlayerApproachOnOff(o, 3000);
			if(PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 1000))
				o->oAction = 1;
			break;

		case 1:
			OneUp_kinoko_Slider();
			break;

		case 2:
			iwa_TimerRemove(o, 30);
			OneUp_kinoko_Hit();
			break;
	}

	OneUp_kinoko_Hit();
	s_makeobj_nowpos(o, MODEL_NONE, sm64::bhv::bhvSparkleSpawn());
}

void s_1up_kinoko_stop_main()
{
	OneUp_kinoko_Hit();
	PlayerApproachOnOff(o, 3000);
}

void s_set_1up_kinoko_main()
{
	s16 sp26;

	switch(o->oAction)
	{
		case 0:
			if(PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 1000))
			{
				o->oVelY   = 40.0f;
				o->oAction = 1;
			}
			break;

		case 1:
			sp26 = ObjMoveEvent();
			OneUp_kinoko_Escape(sp26);
			s_makeobj_nowpos(o, MODEL_NONE, sm64::bhv::bhvSparkleSpawn());
			break;

		case 2:
			sp26 = ObjMoveEvent();
			OneUp_kinoko_Hit();
			iwa_TimerRemove(o, 30);
			break;
	}

	PlayerApproachOnOff(o, 3000);
}

void s_1up_kinoko_secret_main()
{
	s16 move_flag;
	switch(o->oAction)
	{
		case 0:
			o->header.gfx.node.flags |= 0x10;
			if(o->o1UpHiddenUnkF4 == o->oBehParams2ndByte)
			{
				o->oVelY   = 40.0f;
				o->oAction = 3;
				o->header.gfx.node.flags &= ~0x10;
				AudStartSound(SOUND_GENERAL2_1UP_APPEAR, gDefaultSoundArgs);
			}
			break;

		case 1:
			move_flag = ObjMoveEvent();
			OneUp_kinoko_Escape(move_flag);
			s_makeobj_nowpos(o, MODEL_NONE, sm64::bhv::bhvSparkleSpawn());
			break;

		case 2:
			move_flag = ObjMoveEvent();
			OneUp_kinoko_Hit();
			iwa_TimerRemove(o, 30);
			break;

		case 3:
			move_flag = ObjMoveEvent();
			if(o->oTimer >= 18 * FRAME_RATE_SCALER_INV)
				s_makeobj_nowpos(o, MODEL_NONE, sm64::bhv::bhvSparkleSpawn());

			OneUp_kinoko_Appear();

			if(o->oTimer == 37 * FRAME_RATE_SCALER_INV)
			{
				s_hitON();
				o->oAction     = 1;
				o->oForwardVel = 8.0f;
			}
			break;
	}
}

void s_dummy_kinoko()
{
	if(s_hitcheck(o, gMarioObject) == 1)
	{
		Object* sp1C = s_find_obj(sm64::bhv::bhvHidden1up());
		if(sp1C != NULL)
			sp1C->o1UpHiddenUnkF4++;

		o->activeFlags = 0;
	}
}

void s_1up_secret_chase_main()
{
	switch(o->oAction)
	{
		case 0:
			o->header.gfx.node.flags |= 0x10;
			if(o->o1UpHiddenUnkF4 == o->oBehParams2ndByte)
			{
				o->oVelY   = 40.0f;
				o->oAction = 3;
				o->header.gfx.node.flags &= ~0x10;
				AudStartSound(SOUND_GENERAL2_1UP_APPEAR, gDefaultSoundArgs);
			}
			break;

		case 1:
			OneUp_kinoko_Get();
			ObjMoveEvent();
			break;

		case 3:
			ObjMoveEvent();
			if(o->oTimer >= 18 * FRAME_RATE_SCALER_INV)
				s_makeobj_nowpos(o, MODEL_NONE, sm64::bhv::bhvSparkleSpawn());

			OneUp_kinoko_Appear();

			if(o->oTimer == 37 * FRAME_RATE_SCALER_INV)
			{
				s_hitON();
				o->oAction     = 1;
				o->oForwardVel = 10.0f;
			}
			break;
	}
}

void s_dummy_kinoko_chase()
{
	if(s_hitcheck(o, gMarioObject) == 1)
	{
		Object* stratp = s_find_obj(sm64::bhv::bhvHidden1upInPole());
		if(stratp != NULL)
		{
			stratp->o1UpHiddenUnkF4++;
		}

		o->activeFlags = 0;
	}
}

void s_1up_secret_tate_main()
{
	s8 sp2F;

	if(PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 700))
	{
		s_makeobj_chain(2, 0, 50, 0, o, MODEL_1UP, sm64::bhv::bhvHidden1upInPole());
		for(sp2F = 0; sp2F < 2; sp2F++)
		{
			s_makeobj_chain(0, 0, sp2F * -200, 0, o, MODEL_NONE, sm64::bhv::bhvHidden1upInPoleTrigger());
		}

		o->activeFlags = 0;
	}
}
