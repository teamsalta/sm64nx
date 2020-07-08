// bowser_bomb.c.inc

void bhv_bowser_bomb_loop(void)
{
	if(are_objects_collided(o, gMarioObject) == 1)
	{
		o->oInteractStatus &= ~INT_STATUS_INTERACTED; /* bit 15 */
		spawn_object(o, MODEL_EXPLOSION, sm64::bhv::bhvExplosion());
		o->activeFlags = 0;
	}

	if(o->oInteractStatus & INTERACT_MR_BLIZZARD) /* bit 21 */
	{
		spawn_object(o, MODEL_BOWSER_FLAMES, sm64::bhv::bhvBowserBombExplosion());
		create_sound_spawner(SOUND_GENERAL_BOWSER_BOMB_EXPLOSION);
		set_camera_shake_from_point(SHAKE_POS_LARGE, o->oPosX, o->oPosY, o->oPosZ);
		o->activeFlags = 0;
	}

	set_object_visibility(o, 7000);
}

void bhv_bowser_bomb_explosion_loop(void)
{
	struct Object* mineSmoke;

	obj_scale((f32)o->oTimer / 14.0f * FRAME_RATE_SCALER * 9.0 + 1.0);

	if((o->oTimer % 4 == 0) && (o->oTimer < 20))
	{
		mineSmoke = spawn_object(o, MODEL_BOWSER_SMOKE, sm64::bhv::bhvBowserBombSmoke());
		mineSmoke->oPosX += RandomFloat() * FRAME_RATE_SCALER * 600.0f - 400.0f;
		mineSmoke->oPosZ += RandomFloat() * FRAME_RATE_SCALER * 600.0f - 400.0f;
		mineSmoke->oVelY += RandomFloat() * 10.0f * FRAME_RATE_SCALER;
	}

	if((o->oTimer / FRAME_RATE_SCALER_INV) % 2 == 0)
		o->oAnimState++;

	if(o->oTimer == 28 * FRAME_RATE_SCALER_INV)
		o->activeFlags = 0;
}

void bhv_bowser_bomb_smoke_loop(void)
{
	obj_scale((f32)o->oTimer / 14.0f * FRAME_RATE_SCALER * 9.0 + 1.0);
	if((o->oTimer / FRAME_RATE_SCALER_INV) % 2 == 0)
		o->oAnimState++;

	o->oOpacity -= 10 / FRAME_RATE_SCALER_INV;
	if(o->oOpacity < 10)
		o->oOpacity = 0;

	o->oPosY += o->oVelY * FRAME_RATE_SCALER;

	if(o->oTimer == 28 * FRAME_RATE_SCALER_INV)
		o->activeFlags = 0;
}
