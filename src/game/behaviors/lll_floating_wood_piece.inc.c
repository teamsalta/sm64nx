// lll_floating_wood_piece.c.inc

void bhv_lll_wood_piece_loop(void)
{
	if(o->oTimer == 0)
		o->oPosY -= 100.0f;
	o->oPosY += sins(o->oLllWoodPieceUnkF4) * 3.0f * FRAME_RATE_SCALER;
	o->oLllWoodPieceUnkF4 += 0x400 / FRAME_RATE_SCALER;
	if(o->parentObj->oAction == 2)
	{
		mark_object_for_deletion(o);
	}
}

void bhv_lll_floating_wood_bridge_loop(void)
{
	struct Object* sp3C;
	s32 i;
	switch(o->oAction)
	{
		case 0:

			if(o->oDistanceToMario < 2500.0f && !sm64::config().camera().disableDistanceClip())
			{
				for(i = 1; i < 4; i++)
				{
					sp3C			 = spawn_object_relative(0, (i - 2) * 300, 0, 0, o, MODEL_LLL_WOOD_BRIDGE, sm64::bhv::bhvLllWoodPiece());
					sp3C->oLllWoodPieceUnkF4 = i * 4096;
				}
				o->oAction = 1;
			}
			break;
		case 1:
			if(o->oDistanceToMario > 2600.0f && !sm64::config().camera().disableDistanceClip())
				o->oAction = 2;
			break;
		case 2:
			o->oAction = 0;
			break;
	}
}
