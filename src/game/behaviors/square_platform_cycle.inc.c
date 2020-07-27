// square_platform_cycle.c.inc

s32 func_802B02FC(u16 yaw, s32 a)
{
	o->oMoveAngleYaw = yaw;

	if(a < o->oTimer)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void bhv_squarish_path_moving_loop(void)
{
	o->oForwardVel = 10.0f;

	switch(o->oAction)
	{
		case 0:
			o->oAction = (o->oBehParams2ndByte & 3) + 1;
			break;
		case 1:
			if(func_802B02FC(0, 60 * FRAME_RATE_SCALER_INV))
				o->oAction++;
			break;
		case 2:
			if(func_802B02FC(0x4000, 60 * FRAME_RATE_SCALER_INV))
				o->oAction++;
			break;
		case 3:
			if(func_802B02FC(0x8000, 60 * FRAME_RATE_SCALER_INV))
				o->oAction++;
			break;
		case 4:
			if(func_802B02FC(0xc000, 60 * FRAME_RATE_SCALER_INV))
				o->oAction = 1;
			break;
		default:
			break;
	}
	s_optionmove_F();
	stMainMoveBG();
}
