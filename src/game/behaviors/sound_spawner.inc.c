// sound_spawner.c.inc

void bhv_sound_spawner_init(void)
{
	s32 sp1C = o->oSoundEffectUnkF4;
	AudStartSound(sp1C, o->header.gfx.cameraToObject);
}
