/**
 * Behavior for bhvUnusedPoundablePlatform.
 *
 * This size behavior controls a platform that shatters into small triangles
 * if Mario ground pounds it.
 *
 * Its collision model perfectly aligns with the hole leading to In the
 * Deep Freeze in Snowman's Land. It is likely that players early in development
 * would have to ground pound this platform before being able to access the star.
 *
 * Curiously, the triangles spawned when the platform breaks use a model that
 * is not used anywhere else.
 */

void bhv_unused_poundable_platform(void)
{
	s_set_scale(1.02f);

	if(o->oAction == 0)
	{
		if(s_checkplayer_hipaatack())
		{
			s_kemuri();
			s_boxeffect(20, 56, 3.0f, 0);
			o->oAction++;
		}
	}
	else if(o->oTimer > 7 * FRAME_RATE_SCALER_INV)
	{
		s_remove_obj(o);
	}
	stMainMoveBG();
}
