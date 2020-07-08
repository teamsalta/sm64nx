// lll_hexagonal_ring.c.inc

void bhv_lll_rotating_hexagonal_ring_loop(void)
{
	UNUSED s32 unused;
	o->oCollisionDistance = 4000.0f;
	o->oDrawingDistance   = 8000.0f;

	switch(o->oAction)
	{
		case 0:
			if(gMarioObject->platform == o)
				o->oAction++;
			o->oAngleVelYaw = 0x100;
			break;
		case 1:
			o->oAngleVelYaw = 256.0f - sins((o->oTimer / FRAME_RATE_SCALER_INV) << 7) * 256.0f;
			if(o->oTimer > 128 * FRAME_RATE_SCALER_INV)
				o->oAction++;
			break;
		case 2:
			if(gMarioObject->platform != o)
				o->oAction++;
			if(o->oTimer > 128 * FRAME_RATE_SCALER_INV)
				o->oAction++;
			o->oAngleVelYaw = 0;
			func_802BBFDC();
			break;
		case 3:
			o->oAngleVelYaw = sins((o->oTimer / FRAME_RATE_SCALER_INV) << 7) * 256.0f;
			if(o->oTimer > 128 * FRAME_RATE_SCALER_INV)
				o->oAction = 0;
			break;
		case 4:
			o->oAction = 0;
			break;
	}
	o->oAngleVelYaw = -o->oAngleVelYaw * FRAME_RATE_SCALER;
	o->oMoveAngleYaw += o->oAngleVelYaw * FRAME_RATE_SCALER;
}
