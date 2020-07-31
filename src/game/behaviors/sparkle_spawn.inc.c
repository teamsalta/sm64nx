/**
 * Behavior for sm64::bhv::bhvSparkleSpawn().
 *
 * This spawns the sparkles used by various objects. After being given a
 * random local position and scale, each sparkle's behavior is thereafter
 * controlled by sm64::bhv::bhvSparkle(). This spawner is deleted after 1 frame.
 */
void bhv_sparkle_spawn_loop(void)
{
	struct Object* sparkle = s_makeobj_effect(0, 1.0f, o, MODEL_SPARKLES_ANIMATION, sm64::bhv::bhvSparkle());
	if(sparkle != NULL)
	{
		s_random_XYZ_offset(sparkle, 90.0f);
		s_random_scale(sparkle, 1.0f, 0.0f);
	}
	if(o->oTimer > 1 * FRAME_RATE_SCALER_INV)
	{
		s_remove_obj(o);
	}
}
