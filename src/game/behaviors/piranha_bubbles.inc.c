/**
 * Behavior for sm64::bhv::bhvPiranhaPlantBubble() and sm64::bhv::bhvPiranhaPlantWakingBubbles().
 *
 * This controls the bubble that grows and shrinks from a Piranha Plant's nose
 * as it sleeps, and the smaller bubbles that explode outwards when the Piranha
 * Plant is woken up.
 */

/**
 * Main loop for sm64::bhv::bhvPiranhaPlantWakingBubbles(). Initialize a random 3D velocity
 * vector, then follow its path according to gravity.
 */
void bhv_piranha_plant_waking_bubbles_loop(void)
{
	if(o->oTimer == 0)
	{
		o->oVelY	 = Randomf() * 10.0f + 5.0f;
		o->oForwardVel	 = Randomf() * 10.0f + 5.0f;
		o->oMoveAngleYaw = RandomU16();
	}
	s_optionmove_F();
}

/**
 * Main loop for sm64::bhv::bhvPiranhaPlantBubble(). After it is initialized, the bubble
 * will grow and shrink continuously while the Piranha Plant sleeps. If the
 * Piranha Plant ever stops sleeping, the bubble will disappear and spawn many
 * bubbles to appear to burst. After bursting, the bubble is reinitialized
 * and the cycle can repeat.
 */
void bhv_piranha_plant_bubble_loop(void)
{
	struct Object* parent = o->parentObj; // the Piranha Plant
	f32 scale	      = 0;
	s32 i;
	s32 frame = parent->header.gfx.unk38.frame();
	// TODO: rename lastFrame if it is inaccurate
	s32 lastFrame = parent->header.gfx.unk38.curAnim->lastFrame - 2;
	s32 UNUSED unused;
	f32 doneShrinkingFrame; // the first frame after shrinking is done
	f32 beginGrowingFrame;	// the frame just before growing begins

	s_posoffset_mother(parent, 0, 72.0f, 180.0f);

	switch(o->oAction)
	{
		case PIRANHA_PLANT_BUBBLE_ACT_IDLE:
			s_shapeOFF();
			scale = 0;

			if(parent->oAction == PIRANHA_PLANT_ACT_SLEEPING)
			{
				o->oAction++; // move to PIRANHA_PLANT_BUBBLE_ACT_GROW_SHRINK_LOOP
			}
			break;

		case PIRANHA_PLANT_BUBBLE_ACT_GROW_SHRINK_LOOP:
			if(parent->oDistanceToMario < parent->oDrawingDistance)
			{
				s_shapeON();

				if(parent->oAction == PIRANHA_PLANT_ACT_SLEEPING)
				{
					/**
					 * Set the frame after shrinking is done to be slightly before
					 * halfway through the animation, and the frame before growing
					 * slightly after halfway. This leaves about 8 frames during
					 * which the bubble is at its smallest, where its scale is 1.0f.
					 */
					doneShrinkingFrame = lastFrame / 2.0f - 4.0f;
					beginGrowingFrame  = lastFrame / 2.0f + 4.0f;

					// Note that the bubble always starts this loop at its largest.
					if(frame < doneShrinkingFrame)
					{
						// Shrink from 5.0f to 1.0f.
						scale = coss(frame / doneShrinkingFrame * 0x4000) * 4.0f + 1.0;
					}
					else if(frame > beginGrowingFrame)
					{
						// Grow from 1.0f to 5.0f.
						scale = sins(
							    (
								// they should have used beginGrowingFrame here:
								(frame - (lastFrame / 2.0f + 4.0f)) / beginGrowingFrame) *
							    0x4000) *
							    4.0f +
							1.0;
					}
					else
					{
						// Stay at 1.0f for a few frames.
						scale = 1.0f;
					}
				}
				else
				{
					// Piranha Plant is no longer sleeping.
					o->oAction++; // move to PIRANHA_PLANT_BUBBLE_ACT_BURST
				}
			}
			else
			{
				s_shapeOFF();
			}
			break;

		case PIRANHA_PLANT_BUBBLE_ACT_BURST:
			s_shapeOFF();
			scale = 0;

			// Spawn 15 small bubbles to make it look like this bubble burst.
			for(i = 0; i < 15; i++)
			{
				s_makeobj_effect(0, 1.0f, o, MODEL_BUBBLE, sm64::bhv::bhvPiranhaPlantWakingBubbles());
			}

			o->oAction = PIRANHA_PLANT_BUBBLE_ACT_IDLE;
			scale	   = 1.0f; // this has no effect; it is set to 0 in the idle state
			break;
	}
	s_set_scale(scale);
}
