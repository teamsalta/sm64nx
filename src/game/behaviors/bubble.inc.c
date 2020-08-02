// bubble.c.inc

void bhv_object_bubble_init(void)
{
	o->oPosX += Randomf() * 30.0f;
	o->oPosY += Randomf() * 30.0f;
	o->oPosZ += Randomf() * 30.0f;
}

void bhv_object_bubble_loop(void)
{
	struct Object* bubbleRipples;
	f32 waterY  = mcWaterCheck(o->oPosX, o->oPosZ);
	f32 bubbleY = o->oPosY;

	if(bubbleY > waterY)
	{
		if(gFreeObjectList.next)
		{
			bubbleRipples	     = s_makeobj(o, 0, MODEL_SPOT_ON_GROUND, sm64::bhv::bhvObjectBubbleRipples());
			bubbleRipples->oPosX = o->oPosX;
			bubbleRipples->oPosY = bubbleY + 5.0f;
			bubbleRipples->oPosZ = o->oPosZ;
		}

		o->activeFlags = 0;
	}
}
