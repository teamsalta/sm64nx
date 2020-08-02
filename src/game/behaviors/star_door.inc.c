// star_door.c.inc
#include "game/motor.h"

void s_speedL_move(void)
{
	o->oVelX = (o->oStarDoorSpeed) * coss(o->oMoveAngleYaw);
	o->oVelZ = (o->oStarDoorSpeed) * -sins(o->oMoveAngleYaw);

	o->oPosX += o->oVelX * FRAME_RATE_SCALER;
	o->oPosZ += o->oVelZ * FRAME_RATE_SCALER;
}

void s_autodoor(void)
{
	Object*  stp = s_find_obj(sm64::bhv::bhvStarDoor());
	switch(o->oAction)
	{
		case 0:
			s_hitON();
			if(0x30000 & o->oInteractStatus)
				o->oAction = 1;
			if(stp != NULL && stp->oAction != 0)
				o->oAction = 1;
			break;
		case 1:
			if(o->oTimer == 0 && (s16)(o->oMoveAngleYaw) >= 0)
			{
				objsound(SOUND_GENERAL_STAR_DOOR_OPEN);
				SendMotorEvent(35, 30);
			}
			s_hitOFF();
			o->oStarDoorSpeed = -8.0f;
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

			o->oStarDoorSpeed = 8.0f;
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
