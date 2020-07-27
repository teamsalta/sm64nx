// star_door.c.inc
#include "game/motor.h"

void s_speedL_move(void)
{
	o->oVelX = (o->oUnkBC) * coss(o->oMoveAngleYaw);
	o->oVelZ = (o->oUnkBC) * -sins(o->oMoveAngleYaw);

	o->oPosX += o->oVelX * FRAME_RATE_SCALER;
	o->oPosZ += o->oVelZ * FRAME_RATE_SCALER;
}

void bhv_star_door_loop(void)
{
	UNUSED u8 pad[4];
	struct Object* sp18;
	sp18 = s_find_obj(sm64::bhv::bhvStarDoor());
	switch(o->oAction)
	{
		case 0:
			s_hitON();
			if(0x30000 & o->oInteractStatus)
				o->oAction = 1;
			if(sp18 != NULL && sp18->oAction != 0)
				o->oAction = 1;
			break;
		case 1:
			if(o->oTimer == 0 && (s16)(o->oMoveAngleYaw) >= 0)
			{
				objsound(SOUND_GENERAL_STAR_DOOR_OPEN);
				SendMotorEvent(35, 30);
			}
			s_hitOFF();
			o->oUnkBC = -8.0f;
			s_speedL_move();
			if(o->oTimer >= 16 * FRAME_RATE_SCALER_INV)
				o->oAction++;
			break;
		case 2:
			if(o->oTimer >= 31 * FRAME_RATE_SCALER_INV)
				o->oAction++;
			break;
		case 3:
			if(o->oTimer == 0 && (s16)(o->oMoveAngleYaw) >= 0)
			{
				objsound(SOUND_GENERAL_STAR_DOOR_CLOSE);
				SendMotorEvent(35, 30);
			}

			o->oUnkBC = 8.0f;
			s_speedL_move();
			if(o->oTimer >= 16 * FRAME_RATE_SCALER_INV)
				o->oAction++;
			break;
		case 4:
			o->oInteractStatus = 0;
			o->oAction	   = 0;
			break;
	}
}
