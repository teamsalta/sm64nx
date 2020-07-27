// tree_particles.c.inc

void bhv_tree_snow_or_leaf_loop(void)
{
	obj_update_floor_height();
	if(o->oTimer == 0)
	{
		o->oAngleVelPitch	= (Randomf() - 0.5) * 0x1000;
		o->oAngleVelRoll	= (Randomf() - 0.5) * 0x1000;
		o->oTreeSnowOrLeafUnkF8 = 4;
		o->oTreeSnowOrLeafUnkFC = Randomf() * 0x400 + 0x600;
	}
	if(o->oPosY < o->oFloorHeight)
		s_remove_obj(o);
	if(o->oFloorHeight < -11000.0f)
		s_remove_obj(o);
	if(o->oTimer > 100 * FRAME_RATE_SCALER_INV)
		s_remove_obj(o);
	if(gPrevFrameObjectCount > 212)
		s_remove_obj(o);
	o->oFaceAnglePitch += o->oAngleVelPitch / FRAME_RATE_SCALER_INV;
	o->oFaceAngleRoll += o->oAngleVelRoll / FRAME_RATE_SCALER_INV;
	o->oVelY += -3.0f * FRAME_RATE_SCALER;

	if(o->oVelY < -8.0f)
		o->oVelY = -8.0f;

	if(o->oForwardVel > 0)
		o->oForwardVel -= 0.3 * FRAME_RATE_SCALER;
	else
		o->oForwardVel = 0;

	o->oPosX += (sins(o->oMoveAngleYaw) * sins(o->oTreeSnowOrLeafUnkF4) * o->oTreeSnowOrLeafUnkF8) / FRAME_RATE_SCALER_INV;
	o->oPosZ += (coss(o->oMoveAngleYaw) * sins(o->oTreeSnowOrLeafUnkF4) * o->oTreeSnowOrLeafUnkF8) / FRAME_RATE_SCALER_INV;

	o->oTreeSnowOrLeafUnkF4 += o->oTreeSnowOrLeafUnkFC / FRAME_RATE_SCALER_INV;
	o->oPosY += o->oVelY * FRAME_RATE_SCALER;
}

void bhv_snow_leaf_particle_spawn_init(void)
{
	struct Object* obj; // Either snow or leaf
	UNUSED s32 unused;
	s32 isSnow;
	f32 scale;
	UNUSED s32 unused2;
	gMarioObject->oActiveParticleFlags &= ~0x2000;
	if(activeStageNo == LEVEL_CCM || activeStageNo == LEVEL_SL)
		isSnow = 1;
	else
		isSnow = 0;
	if(isSnow)
	{
		if(Randomf() < 0.5)
		{
			obj   = s_makeobj_nowpos(o, MODEL_WHITE_PARTICLE_DL, sm64::bhv::bhvTreeSnow());
			scale = Randomf();
			scale_object_xyz(obj, scale, scale, scale);
			obj->oMoveAngleYaw = RandomU16();
			obj->oForwardVel   = Randomf() * 5.0f;
			obj->oVelY	   = Randomf() * 15.0f;
		}
	}
	else
	{
		if(Randomf() < 0.3)
		{
			obj   = s_makeobj_nowpos(o, MODEL_LEAVES, sm64::bhv::bhvTreeLeaf());
			scale = Randomf() * 3.0f;
			scale_object_xyz(obj, scale, scale, scale);
			obj->oMoveAngleYaw   = RandomU16();
			obj->oForwardVel     = Randomf() * 5.0f + 5.0f;
			obj->oVelY	     = Randomf() * 15.0f;
			obj->oFaceAnglePitch = RandomU16();
			obj->oFaceAngleRoll  = RandomU16();
			obj->oFaceAngleYaw   = RandomU16();
		}
	}
}
