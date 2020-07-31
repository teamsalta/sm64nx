// wind.c.inc

void func_802ADC20(s16 pitch, s16 yaw)
{
	s32 i;
	for(i = 0; i < 3; i++)
	{
		struct Object* wind   = s_makeobj_nowpos(o, MODEL_MIST, sm64::bhv::bhvWind());
		wind->oMoveAngleYaw   = yaw;
		wind->oMoveAnglePitch = pitch;
	}
}

void bhv_wind_loop(void)
{
	s16 sp2E = 500;
	f32 sp28 = 1.0f;
	if(o->oTimer == 0)
	{
		o->oOpacity = 100;

		if(o->oMoveAnglePitch == 0)
		{
			s_random_XZ_offset(o, 900.0f);
			o->oPosX += (sins(o->oMoveAngleYaw + 0x8000) * sp2E) / FRAME_RATE_SCALER_INV; // NOP as Pitch is 0
			o->oPosY += (80.0f + s_random_f(200.0f)) / FRAME_RATE_SCALER_INV;
			o->oPosZ += (coss(o->oMoveAngleYaw + 0x8000) * sp2E) / FRAME_RATE_SCALER_INV; // -coss(a) * sp2E
			o->oMoveAngleYaw += s_random_f(4000.0f * FRAME_RATE_SCALER);
			o->oForwardVel = Randomf() * 70.0f + 50.0f;
		}
		else
		{
			s_random_XZ_offset(o, 600.0f);
			o->oPosY -= (sp2E - 200) / FRAME_RATE_SCALER_INV; // 300
			o->oVelY	 = Randomf() * 30.0f + 50.0f;
			o->oMoveAngleYaw = RandomU16();
			o->oForwardVel	 = 10.0f;
		}
		s_softspritemodeON(o);
		s_set_scale(sp28);
	}
	if(o->oTimer > 8 * FRAME_RATE_SCALER_INV)
		s_remove_obj(o);

	o->oFaceAnglePitch += 4000.0f + 2000.0f * Randomf() * FRAME_RATE_SCALER;
	o->oFaceAngleYaw += 4000.0f + 2000.0f * Randomf() * FRAME_RATE_SCALER;
	s_optionmove_F();
}
