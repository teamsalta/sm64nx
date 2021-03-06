/**
 * Behavior for sm64::bhv::bhvBooCage().
 * This is the cage inside the big boo in the castle courtyard
 * that lets Mario enter BBH. It has its own special interaction type,
 * INTERACT_BBH_ENTRANCE.
 */

/**
 * Boo cage hitbox. It's not tangible; this is the hitbox
 * Mario has to enter to enter BBH.
 */
static struct ObjectHitbox sBooCageHitbox = {
    /* interactType: */ INTERACT_BBH_ENTRANCE,
    /* downOffset: */ 0,
    /* damageOrCoinValue: */ 0,
    /* health: */ 0,
    /* numLootCoins: */ 0,
    /* radius: */ 120,
    /* height: */ 300,
    /* hurtboxRadius: */ 0,
    /* hurtboxHeight: */ 0,
};

/**
 * Update function for sm64::bhv::bhvBooCage().
 */
void bhv_boo_cage_loop(void)
{
	UNUSED s32 unused;

	s_set_hitparam(o, &sBooCageHitbox);

	switch(o->oAction)
	{
		case BOO_CAGE_ACT_IN_BOO:
			// Don't let Mario enter BBH until the boo is killed
			s_hitOFF();

			// Useless scale. This is also found in the code for BOO_CAGE_ACT_ON_GROUND.
			// Was the boo cage originally meant to have been shrunk and grow while falling?
			s_set_scale(1.0f);

			// If the cage's parent boo is killed, set the action to BOO_CAGE_ACT_FALLING,
			// give the cage an initial Y velocity of 60 units/frame, and play the puzzle jingle.
			// Otherwise, stay inside the boo.
			if(o->parentObj->oBooDeathStatus != BOO_DEATH_STATUS_ALIVE)
			{
				o->oAction++;
				o->oVelY = 60.0f;
				Na_NazoClearBgm();
			}
			else
			{
				s_copy_worldXYZ_angleXYZ(o, o->parentObj);
			}

			break;
		case BOO_CAGE_ACT_FALLING:
			// Reset pitch and roll. This is useless, since the cage never rotates.
			// Was it meant to rotate inside the boo, like the beta boo key?
			o->oFaceAnglePitch = 0;
			o->oFaceAngleRoll  = 0;

			// Apply standard physics to the cage.
			s_enemybgcheck();
			s_enemymove(-78);

			// Spawn sparkles while the cage falls.
			s_makeobj_nowpos(o, MODEL_NONE, sm64::bhv::bhvSparkleSpawn());

			// When the cage lands/bounces, play a landing/bouncing sound.
			if(o->oMoveFlags & OBJ_MOVE_LANDED)
			{
				objsound(SOUND_GENERAL_SOFT_LANDING);
			}

			// Once the cage stops bouncing and settles on the ground,
			// set the action to BOO_CAGE_ACT_ON_GROUND.
			// This is the only use of the OBJ_MOVE_AT_WATER_SURFACE flag in the game.
			// It seems to serve no purpose here.
			if(o->oMoveFlags & (OBJ_MOVE_UNDERWATER_ON_GROUND | OBJ_MOVE_AT_WATER_SURFACE | OBJ_MOVE_ON_GROUND))
			{
				o->oAction++;
			}

			break;
		case BOO_CAGE_ACT_ON_GROUND:
			// Allow Mario to enter the cage once it's still on the ground.
			s_hitON();

			// The other useless scale
			s_set_scale(1.0f);

			// Set the action to BOO_CAGE_ACT_MARIO_JUMPING_IN when Mario jumps in.
			if(s_hitcheck(o, gMarioObject))
			{
				o->oAction++;
			}

			break;
		case BOO_CAGE_ACT_MARIO_JUMPING_IN:
			// All this action does is wait 100 frames after Mario starts
			// jumping into the cage to set the action to BOO_CAGE_ACT_USELESS,
			// which does nothing. By extension, this action is also useless.

			if(o->oTimer > 100 * FRAME_RATE_SCALER_INV)
			{
				o->oAction++;
			}

			break;
		case BOO_CAGE_ACT_USELESS:
			break;
	}
}
