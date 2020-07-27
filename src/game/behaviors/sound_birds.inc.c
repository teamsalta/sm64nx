// sound_birds.inc.c

void bhv_birds_sound_loop(void)
{
	if(gCamera->mode == CAMERA_MODE_BEHIND_MARIO)
		return;

	switch(o->oBehParams2ndByte)
	{
		case 0:
			objsound_level(SOUND_OBJ2_BIRD_CHIRP1);
			break;

		case 1:
			objsound_level(SOUND_GENERAL2_BIRD_CHIRP2);
			break;

		case 2:
			objsound_level(SOUND_OBJ_BIRD_CHIRP3);
			break;
	}
}
