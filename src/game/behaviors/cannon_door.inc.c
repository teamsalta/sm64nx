// cannon.c.inc

void bhv_cannon_closed_init(void)
{
	struct Object* cannon;

	if(save_file_is_cannon_unlocked() == 1)
	{
		// If the cannon is open, spawn a cannon and despawn the object.
		cannon			  = spawn_object(o, MODEL_CANNON_BASE, sm64::bhv::bhvCannon());
		cannon->oBehParams2ndByte = o->oBehParams2ndByte;
		cannon->oPosX		  = o->oHomeX;
		cannon->oPosY		  = o->oHomeY;
		cannon->oPosZ		  = o->oHomeZ;

		o->oAction     = CANNON_TRAP_DOOR_ACT_OPEN;
		o->activeFlags = 0;
	}
}

void CannonTrapDoorOpeningLoop(void)
{
	if(o->oTimer == 0)
		PlaySound2(SOUND_GENERAL_CANNON_UP);

	if(o->oTimer < 30 * FRAME_RATE_SCALER_INV)
	{
		o->oVelY = -0.5f;
		o->oPosY += o->oVelY * FRAME_RATE_SCALER;
		o->oVelX = 0;
	}
	else
	{
		if(o->oTimer == 80 * FRAME_RATE_SCALER_INV)
		{
			bhv_cannon_closed_init();
			return;
		}

		o->oVelX = 4.0f;
		o->oVelY = 0;
		o->oPosX += o->oVelX * FRAME_RATE_SCALER;
	}
}

void bhv_cannon_closed_loop(void)
{
	switch(o->oAction)
	{
		case CANNON_TRAP_DOOR_ACT_CLOSED:
			o->oVelX	    = 0;
			o->oVelY	    = 0;
			o->oDrawingDistance = 4000.0f;

			if(save_file_is_cannon_unlocked() == 1)
				o->oAction = CANNON_TRAP_DOOR_ACT_CAM_ZOOM;
			break;

		case CANNON_TRAP_DOOR_ACT_CAM_ZOOM:
			if(o->oTimer == 60 * FRAME_RATE_SCALER_INV)
				o->oAction = CANNON_TRAP_DOOR_ACT_OPENING;

			o->oDrawingDistance = 20000.0f;
			break;

		case CANNON_TRAP_DOOR_ACT_OPENING:
			CannonTrapDoorOpeningLoop();
			break;
	}
}
