/**
 * Behavior for the falling pillars inside the underwater cave area of
 * Jolly Roger Bay.
 *
 * Also includes behavior for the invisible hitboxes they spawn.
 */

static struct ObjectHitbox sFallingPillarHitbox = {
    /* interactType:      */ INTERACT_DAMAGE,
    /* downOffset:        */ 150,
    /* damageOrCoinValue: */ 3,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 150,
    /* height:            */ 300,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

/**
 * Initiates various physics params for the pillar.
 */
void s_wd_pillar_init(void)
{
	o->oGravity  = 0.5f;
	o->oFriction = 0.91f;
	o->oBuoyancy = 1.3f;
}

/**
 * Spawns 4 hitboxes with Y coordinates offset.
 */
void PillarMakeHit(void)
{
	s32 i;

	for(i = 0; i < 4; i++)
	{
		s_makeobj_chain(i, 0, i * 400 + 300, 0, o, MODEL_NONE, sm64::bhv::bhvFallingPillarHitbox());
	}
}

/**
 * Computes the angle from current pillar position to 500 units in front of
 * Mario.
 */
s16 pillar_targetON(void)
{
	// Calculate target to be 500 units in front of Mario in
	// the direction he is facing (angle[1] is yaw).
	float targetX = sins(gMarioObject->header.gfx.angle[1]) * 500.0f + gMarioObject->header.gfx.pos[0];
	float targetZ = coss(gMarioObject->header.gfx.angle[1]) * 500.0f + gMarioObject->header.gfx.pos[2];

	// Calculate the angle to the target from the pillar's current location.
	return atan2s(targetZ - o->oPosZ, targetX - o->oPosX);
}

/**
 * Falling pillar main logic loop.
 */
void s_wd_pillar_main(void)
{
	s16 angleInFrontOfMario;
	switch(o->oAction)
	{
		case FALLING_PILLAR_ACT_IDLE:
			// When Mario is within 1300 units of distance...
			if(PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 1300))
			{
				// Begin slightly moving towards Mario.
				o->oMoveAngleYaw = o->oAngleToMario;
				o->oForwardVel	 = 1.0f;

				// Spawn the invisible hitboxes.
				PillarMakeHit();

				// Start turning towards Mario.
				o->oAction = FALLING_PILLAR_ACT_TURNING;

				// Play the detaching sound.
				objsound(SOUND_GENERAL_POUND_ROCK);
			}
			break;

		case FALLING_PILLAR_ACT_TURNING:
			ObjMoveEvent_noInc();

			// Calculate angle in front of Mario and turn towards it.
			angleInFrontOfMario = pillar_targetON();
			o->oFaceAngleYaw    = s_chase_angle(o->oFaceAngleYaw, angleInFrontOfMario, 0x400 / FRAME_RATE_SCALER_INV);

			// After 10 ticks, start falling.
			if(o->oTimer > 10 * FRAME_RATE_SCALER_INV)
				o->oAction = FALLING_PILLAR_ACT_FALLING;
			break;

		case FALLING_PILLAR_ACT_FALLING:
			ObjMoveEvent_noInc();

			// Start falling slowly, with increasing acceleration each frame.
			o->oFallingPillarPitchAcceleration += 4.0f * FRAME_RATE_SCALER;
			o->oAngleVelPitch += o->oFallingPillarPitchAcceleration * FRAME_RATE_SCALER;
			o->oFaceAnglePitch += o->oAngleVelPitch * FRAME_RATE_SCALER;

			// Once the pillar has turned nearly 90 degrees (after ~22 frames),
			if(o->oFaceAnglePitch > 0x3900)
			{
				// Move 500 units in the direction of falling.
				o->oPosX += sins(o->oFaceAngleYaw) * 500.0f * FRAME_RATE_SCALER;
				o->oPosZ += coss(o->oFaceAngleYaw) * 500.0f * FRAME_RATE_SCALER;

				// Make the camera shake and spawn dust clouds.
				Viewshaking(SHAKE_POS_MEDIUM, o->oPosX, o->oPosY, o->oPosZ);
				s_burneffect(0, 0, 92.0f);

				// Go invisible.
				o->activeFlags = 0;

				// Play the hitting the ground sound.
				obj_remove_sound(SOUND_GENERAL_BIG_POUND);
			}
			break;
	}
}

/**
 * Main loop for the invisible hitboxes.
 */
void s_pillar_hit_main(void)
{
	// Get the state of the pillar.
	s32 pitch   = o->parentObj->oFaceAnglePitch;
	s32 yaw	    = o->parentObj->oFaceAngleYaw;
	f32 x	    = o->parentObj->oPosX;
	f32 y	    = o->parentObj->oPosY;
	f32 z	    = o->parentObj->oPosZ;
	f32 yOffset = o->oBehParams2ndByte * 400 + 300;

	// Update position of hitboxes so they fall with the pillar.
	o->oPosX = sins(pitch) * sins(yaw) * yOffset + x;
	o->oPosY = coss(pitch) * yOffset + y;
	o->oPosZ = sins(pitch) * coss(yaw) * yOffset + z;

	// Give these a hitbox so they can collide with Mario.
	s_set_hitparam(o, &sFallingPillarHitbox);

	// When the pillar goes inactive, the hitboxes also go inactive.
	if(o->parentObj->activeFlags == 0)
		o->activeFlags = 0;
}
