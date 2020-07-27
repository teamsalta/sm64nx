// controllable_platform.c.inc
#include "game/motor.h"

static s8 NEWSliftButton_flag = 0;

void NEWSbutton_ON(void)
{
	o->oParentRelativePosY -= 4.0f * FRAME_RATE_SCALER;
	if(o->oParentRelativePosY < 41.0f)
	{
		o->oParentRelativePosY = 41.0f;
		o->oAction	       = 2;
	}
}

void NEWSbutton_OFF(void)
{
	if(o->oBehParams2ndByte == NEWSliftButton_flag)
		return;

	o->oParentRelativePosY += 4.0f * FRAME_RATE_SCALER;

	if(o->oParentRelativePosY > 51.0f)
	{
		o->oParentRelativePosY = 51.0f;
		o->oAction	       = 0;
	}
}

void bhv_controllable_platform_sub_loop(void)
{
	switch(o->oAction)
	{
		case 0:
			if(o->oTimer < 30 * FRAME_RATE_SCALER_INV)
				break;

			if(gMarioObject->platform == o)
			{
				NEWSliftButton_flag = o->oBehParams2ndByte;
				o->oAction	    = 1;
				objsound(SOUND_GENERAL_MOVING_PLATFORM_SWITCH);
			}
			break;

		case 1:
			NEWSbutton_ON();
			break;

		case 2:
			NEWSbutton_OFF();
			break;
	}

	o->oVelX = o->parentObj->oVelX;
	o->oVelZ = o->parentObj->oVelZ;

	if(o->parentObj->activeFlags == 0)
		o->activeFlags = 0;
}

void bhv_controllable_platform_init(void)
{
	struct Object* sp34;
	sp34			= s_makeobj_relative(o, MODEL_HMC_METAL_ARROW_PLATFORM, sm64::bhv::bhvControllablePlatformSub(), 0, 51, 204, 0, 0, 0);
	sp34->oBehParams2ndByte = 1;
	sp34			= s_makeobj_relative(o, MODEL_HMC_METAL_ARROW_PLATFORM, sm64::bhv::bhvControllablePlatformSub(), 0, 51, -204, 0, -0x8000, 0);
	sp34->oBehParams2ndByte = 2;
	sp34			= s_makeobj_relative(o, MODEL_HMC_METAL_ARROW_PLATFORM, sm64::bhv::bhvControllablePlatformSub(), 204, 51, 0, 0, 0x4000, 0);
	sp34->oBehParams2ndByte = 3;
	sp34			= s_makeobj_relative(o, MODEL_HMC_METAL_ARROW_PLATFORM, sm64::bhv::bhvControllablePlatformSub(), -204, 51, 0, 0, -0x4000, 0);
	sp34->oBehParams2ndByte = 4;

	NEWSliftButton_flag = 0;

	o->oControllablePlatformUnkFC = o->oPosY;
}

void NEWSliftReflect(s8 sp1B)
{
	o->oControllablePlatformUnkF8 = sp1B;
	o->oTimer		      = 0;
	NEWSliftButton_flag	      = 5;

	objsound(SOUND_GENERAL_QUIET_POUND1);
	SendMotorEvent(50, 80);
}

void NEWSliftWallCheck(s8 code, s8 codeNo[3], Vec3f check1, UNUSED Vec3f check2, Vec3f check3)
{
	if(codeNo[1] == 1 || (codeNo[0] == 1 && codeNo[2] == 1))
		NEWSliftReflect(code);
	else
	{
		if(codeNo[0] == 1)
		{
			if(((code == 1 || code == 2) && (s32)check1[2] != 0) || ((code == 3 || code == 4) && (s32)check1[0] != 0))
			{
				NEWSliftReflect(code);
			}
			else
			{
				o->oPosX += check1[0] * FRAME_RATE_SCALER;
				o->oPosZ += check1[2] * FRAME_RATE_SCALER;
			}
		}

		if(codeNo[2] == 1)
		{
			if(((code == 1 || code == 2) && (s32)check3[2] != 0) || ((code == 3 || code == 4) && (s32)check3[0] != 0))
			{
				NEWSliftReflect(code);
			}
			else
			{
				o->oPosX += check3[0] * FRAME_RATE_SCALER;
				o->oPosZ += check3[2] * FRAME_RATE_SCALER;
			}
		}
	}

	if(!PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 400))
	{
		NEWSliftButton_flag	       = 6;
		o->oControllablePlatformUnk100 = 1;
		o->oTimer		       = 0;
	}
}

void NEWSlift_Shock(void)
{
	if(o->oControllablePlatformUnkF8 == 1 || o->oControllablePlatformUnkF8 == 2)
	{
		o->oFaceAnglePitch = sins(o->oTimer * 0x1000 / FRAME_RATE_SCALER_INV) * 182.04444 * 10.0;
		o->oPosY	   = o->oControllablePlatformUnkFC + sins(o->oTimer * 0x2000 / FRAME_RATE_SCALER_INV) * 20.0f;
	}
	else
	{
		o->oFaceAngleRoll = sins(o->oTimer * 0x1000 / FRAME_RATE_SCALER_INV) * 182.04444 * 10.0;
		o->oPosY	  = o->oControllablePlatformUnkFC + sins(o->oTimer * 0x2000 / FRAME_RATE_SCALER_INV) * 20.0f;
	}

	if(o->oTimer == 32 * FRAME_RATE_SCALER_INV)
	{
		NEWSliftButton_flag = o->oControllablePlatformUnkF8;
		o->oFaceAnglePitch  = 0;
		o->oFaceAngleRoll   = 0;
		o->oPosY	    = o->oControllablePlatformUnkFC;
	}
}

void NEWSlift_PlayerRideCheck(void)
{
	s16 sp1E = gMarioObject->header.gfx.pos[0] - o->oPosX;
	s16 sp1C = gMarioObject->header.gfx.pos[2] - o->oPosZ;

	if(gMarioObject->platform == o || gMarioObject->platform == s_find_obj(sm64::bhv::bhvControllablePlatformSub()))
	{
		o->oFaceAnglePitch = sp1C * 4;
		o->oFaceAngleRoll  = -sp1E * 4;
		if(NEWSliftButton_flag == 6)
		{
			NEWSliftButton_flag = 0;
			o->oTimer	    = 0;
			o->header.gfx.node.flags &= ~0x10;
		}
	}
	else
	{
	}
}

void bhv_controllable_platform_loop(void)
{
	s8 ch[3];
	Vec3f check1;
	Vec3f check2;
	Vec3f check3;

	o->oAngleVelRoll  = 0;
	o->oAngleVelPitch = 0;
	o->oVelX	  = 0;
	o->oVelZ	  = 0;

	switch(NEWSliftButton_flag)
	{
		case 0:
			o->oFaceAnglePitch /= 2;
			o->oFaceAngleRoll /= 2;
			if(o->oControllablePlatformUnk100 == 1 && o->oTimer > 30 * FRAME_RATE_SCALER_INV)
			{
				NEWSliftButton_flag = 6;
				o->oTimer	    = 0;
			}
			break;

		case 1:
			o->oVelZ = 10.0f;
			ch[0]	 = PositionWallCheck(check1, o->oPosX + 250.0, o->oPosY, o->oPosZ + 300.0, 50.0f);
			ch[1]	 = PositionWallCheck(check2, o->oPosX, o->oPosY, o->oPosZ + 300.0, 50.0f);
			ch[2]	 = PositionWallCheck(check3, o->oPosX - 250.0, o->oPosY, o->oPosZ + 300.0, 50.0f);
			NEWSliftWallCheck(2, ch, check1, check2, check3);
			break;

		case 2:
			o->oVelZ = -10.0f;
			ch[0]	 = PositionWallCheck(check1, o->oPosX + 250.0, o->oPosY, o->oPosZ - 300.0, 50.0f);
			ch[1]	 = PositionWallCheck(check2, o->oPosX, o->oPosY, o->oPosZ - 300.0, 50.0f);
			ch[2]	 = PositionWallCheck(check3, o->oPosX - 250.0, o->oPosY, o->oPosZ - 300.0, 50.0f);
			NEWSliftWallCheck(1, ch, check1, check2, check3);
			break;

		case 3:
			o->oVelX = 10.0f;
			ch[0]	 = PositionWallCheck(check1, o->oPosX + 300.0, o->oPosY, o->oPosZ + 250.0, 50.0f);
			ch[1]	 = PositionWallCheck(check2, o->oPosX + 300.0, o->oPosY, o->oPosZ, 50.0f);
			ch[2]	 = PositionWallCheck(check3, o->oPosX + 300.0, o->oPosY, o->oPosZ - 250.0, 50.0f);
			NEWSliftWallCheck(4, ch, check1, check2, check3);
			break;

		case 4:
			o->oVelX = -10.0f;
			ch[0]	 = PositionWallCheck(check1, o->oPosX - 300.0, o->oPosY, o->oPosZ + 250.0, 50.0f);
			ch[1]	 = PositionWallCheck(check2, o->oPosX - 300.0, o->oPosY, o->oPosZ, 50.0f);
			ch[2]	 = PositionWallCheck(check3, o->oPosX - 300.0, o->oPosY, o->oPosZ - 250.0, 50.0f);
			NEWSliftWallCheck(3, ch, check1, check2, check3);
			break;

		case 5:
			NEWSlift_Shock();
			return;
			break;

		case 6:
			if(iwa_TimerRemove(o, 150))
				s_makeobj_absolute(o, 0, MODEL_HMC_METAL_PLATFORM, sm64::bhv::bhvControllablePlatform(), o->oHomeX, o->oHomeY, o->oHomeZ, 0, 0, 0);
			break;
	}

	NEWSlift_PlayerRideCheck();
	o->oPosX += o->oVelX * FRAME_RATE_SCALER;
	o->oPosZ += o->oVelZ * FRAME_RATE_SCALER;
	if(NEWSliftButton_flag != 0 && NEWSliftButton_flag != 6)
		objsound_level(SOUND_ENV_ELEVATOR2);
}
