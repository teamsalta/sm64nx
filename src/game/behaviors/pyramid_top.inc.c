/**
 * Behaviors for sm64::bhv::bhvPyramidTop(), sm64::bhv::bhvPyramidTopFragment(), and
 * sm64::bhv::bhvPyramidPillarTouchDetector().
 *
 * sm64::bhv::bhvPyramidTop() controls Shifting Sand Land's pyramid's top piece, which
 *      rotates and explodes when Mario stands on all four pillars.
 * sm64::bhv::bhvPyramidTopFragment() controls the shards that the pyramid's top emits when
 *      it is spinning and exploding.
 * sm64::bhv::bhvPyramidPillarTouchDetector() controls the intangible collision boxes that
 *      Mario touches when on top of each pillar.
 */

/**
 * Spawn the four pillars' touch detectors.
 */
void bhv_pyramid_top_init(void)
{
	s_makeobj_absolute(o, 0, MODEL_NONE, sm64::bhv::bhvPyramidPillarTouchDetector(), 1789, 1024, 764, 0, 0, 0);
	s_makeobj_absolute(o, 0, MODEL_NONE, sm64::bhv::bhvPyramidPillarTouchDetector(), 1789, 896, -2579, 0, 0, 0);
	s_makeobj_absolute(o, 0, MODEL_NONE, sm64::bhv::bhvPyramidPillarTouchDetector(), -5883, 1024, -2579, 0, 0, 0);
	s_makeobj_absolute(o, 0, MODEL_NONE, sm64::bhv::bhvPyramidPillarTouchDetector(), -5883, 1024, 764, 0, 0, 0);
}

/**
 * Animate the pyramid top as rising and then spinning. Generate some pyramid
 * fragments in the process.
 */
void bhv_pyramid_top_spinning(void)
{
	struct Object* pyramidFragment;

	// (TODO: What is this doing)
	o->oPosX = o->oHomeX + sins(o->oTimer * 0x4000 / FRAME_RATE_SCALER_INV) * 40.0f;

	// At first, move upward smoothly without rotating.
	if(o->oTimer < 60 * FRAME_RATE_SCALER_INV)
	{
		o->oPosY = o->oHomeY + absf_2(sins(o->oTimer * 0x2000 / FRAME_RATE_SCALER_INV) * 10.0f);
	}
	else
	{
		// Then, rotate at an accelerating rate, and move upward at a constant rate.
		o->oAngleVelYaw += 0x100 / FRAME_RATE_SCALER_INV;
		if(o->oAngleVelYaw > 0x1800)
		{
			o->oAngleVelYaw = 0x1800;
			o->oVelY	= 5.0f;
		}

		o->oFaceAngleYaw += o->oAngleVelYaw * FRAME_RATE_SCALER;
		o->oPosY += o->oVelY * FRAME_RATE_SCALER;
	}

	// Every frame until 90 frames have passed, generate a pyramid fragment
	// with a random velocity and angle.
	if(o->oTimer < 90 * FRAME_RATE_SCALER_INV)
	{
		pyramidFragment				   = s_makeobj_nowpos(o, MODEL_DIRT_ANIMATION, sm64::bhv::bhvPyramidTopFragment());
		pyramidFragment->oForwardVel		   = RandomFloat() * 10.0f + 20.0f;
		pyramidFragment->oMoveAngleYaw		   = RandomU16();
		pyramidFragment->oPyramidTopFragmentsScale = 0.8f;
		pyramidFragment->oGravity		   = RandomFloat() + 2.0f;
	}

	// After enough time, transition to the exploding state.
	if(o->oTimer == 150 * FRAME_RATE_SCALER_INV)
	{
		o->oAction = PYRAMID_TOP_ACT_EXPLODE;
	}
}

/**
 * Explode the pyramid top, generating dust and pyramid fragments.
 */
void bhv_pyramid_top_explode(void)
{
	struct Object* pyramidFragment;
	s16 i;

	s_burneffect(0, 0, 690);

	// Generate 30 pyramid fragments with random properties.
	for(i = 0; i < 30; i++)
	{
		pyramidFragment				   = s_makeobj_nowpos(o, MODEL_DIRT_ANIMATION, sm64::bhv::bhvPyramidTopFragment());
		pyramidFragment->oForwardVel		   = RandomFloat() * 50 + 80;
		pyramidFragment->oVelY			   = RandomFloat() * 80 + 20;
		pyramidFragment->oMoveAngleYaw		   = RandomU16();
		pyramidFragment->oPyramidTopFragmentsScale = 3;
		pyramidFragment->oGravity		   = RandomFloat() * 2 + 5;
	}

	// Deactivate the pyramid top.
	o->activeFlags = 0;
}

void bhv_pyramid_top_loop(void)
{
	switch(o->oAction)
	{
		case PYRAMID_TOP_ACT_CHECK_IF_SOLVED:
			if(o->oPyramidTopPillarsTouched == 4)
			{
				Na_NazoClearBgm();
				o->oAction = PYRAMID_TOP_ACT_SPINNING;
			}
			break;

		case PYRAMID_TOP_ACT_SPINNING:
			if(o->oTimer == 0)
			{
				objsound(SOUND_GENERAL2_PYRAMID_TOP_SPIN);
			}

			bhv_pyramid_top_spinning();
			break;

		case PYRAMID_TOP_ACT_EXPLODE:
			if(o->oTimer == 0)
			{
				obj_remove_sound(SOUND_GENERAL2_PYRAMID_TOP_EXPLOSION);
			}

			bhv_pyramid_top_explode();
			break;
	}
}

/**
 * Initialize the pyramid fragment.
 */
void bhv_pyramid_top_fragment_init(void)
{
	o->oFriction  = 0.999f;
	o->oBuoyancy  = 2.0f;
	o->oAnimState = 3;
	s_set_scale(o->oPyramidTopFragmentsScale);
}

/**
 * Rotate the pyramid fragment along the yaw and pitch axes. After some time,
 * deactivate it.
 */
void bhv_pyramid_top_fragment_loop(void)
{
	ObjMoveEvent();
	o->oFaceAngleYaw += 0x1000 / FRAME_RATE_SCALER_INV;
	o->oFaceAnglePitch += 0x1000 / FRAME_RATE_SCALER_INV;

	if(o->oTimer == 60 * FRAME_RATE_SCALER_INV)
	{
		o->activeFlags = 0;
	}
}

/**
 * If Mario touches a pillar's touch detector, count it towards the pyramid
 * top's total count of touched detectors, and deactivate the detector.
 */
void bhv_pyramid_pillar_touch_detector_loop(void)
{
	s_hitON();
	if(s_hitcheck(o, gMarioObject) == 1)
	{
		// Increase the pyramid top's count of pillars touched.
		o->parentObj->oPyramidTopPillarsTouched++;
		o->activeFlags = 0;
	}
}
