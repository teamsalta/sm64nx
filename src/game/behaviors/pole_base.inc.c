// pole_base.inc.c

void BehClimbDetectLoop(void)
{
	if(o->oPosY - 10.0f < gMarioObject->oPosY && gMarioObject->oPosY < o->oPosY + o->hitboxHeight + 30.0f)
		if(o->oTimer > 10 * FRAME_RATE_SCALER_INV)
			if(!(playerWorks->status & MARIO_PUNCHING))
				obj_push_mario_away(70.0f);
}
