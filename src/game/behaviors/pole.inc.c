/**
 * Behaviors for sm64::bhv::bhvPoleGrabbing() and sm64::bhv::bhvGiantPole().
 *
 * sm64::bhv::bhvPoleGrabbing() is a generic behavior used almost anywhere the player can
 * grab onto and climb up another object.
 *
 * sm64::bhv::bhvGiantPole() controls the large pole in Whomp's Fortress and differs from
 * other poles because it spawns a yellow ball that rests at its top.
 */

/**
 * Initialize the hitbox height field for sm64::bhv::bhvPoleGrabbing().
 */
void bhv_pole_init(void)
{
	/**
	 * This is equivalent to using `o->oBehParams2ndByte` according to
	 * `spawn_objects_from_info`.
	 */
	s32 tenthHitboxHeight = o->oBehParams >> 0x10 & 0xFF;
	o->hitboxHeight	      = tenthHitboxHeight * 10;
}

/**
 * Main loop for sm64::bhv::bhvGiantPole(). It is equivalent to BehClimbDetectLoop() except
 * for the fact that it spawns a yellow sphere at the top of the pole on the
 * first frame.
 */
void bhv_giant_pole_loop(void)
{
	struct Object* topBall;

	if(o->oTimer == 0)
	{
		topBall = s_makeobj_nowpos(o, MODEL_YELLOW_SPHERE, sm64::bhv::bhvYellowBall());
		topBall->oPosY += (o->hitboxHeight + 50.0f);
	}
	BehClimbDetectLoop();
}
