// star_door.c.inc

void func_802A4DB0(void)
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
	sp18 = obj_nearest_object_with_behavior(sm64::bhv::bhvStarDoor());
	switch(o->oAction)
	{
		case 0:
			obj_become_tangible();
			if(0x30000 & o->oInteractStatus)
				o->oAction = 1;
			if(sp18 != NULL && sp18->oAction != 0)
				o->oAction = 1;
			break;
		case 1:
			if(o->oTimer == 0 && (s16)(o->oMoveAngleYaw) >= 0)
				PlaySound2(SOUND_GENERAL_STAR_DOOR_OPEN);
			obj_become_intangible();
			o->oUnkBC = -8.0f;
			func_802A4DB0();
			if(o->oTimer >= 16 * FRAME_RATE_SCALER_INV)
				o->oAction++;
			break;
		case 2:
			if(o->oTimer >= 31 * FRAME_RATE_SCALER_INV)
				o->oAction++;
			break;
		case 3:
			if(o->oTimer == 0 && (s16)(o->oMoveAngleYaw) >= 0)
				PlaySound2(SOUND_GENERAL_STAR_DOOR_CLOSE);
			o->oUnkBC = 8.0f;
			func_802A4DB0();
			if(o->oTimer >= 16 * FRAME_RATE_SCALER_INV)
				o->oAction++;
			break;
		case 4:
			o->oInteractStatus = 0;
			o->oAction	   = 0;
			break;
	}
}
