// corkbox.c.inc
// TODO: This split seems weird. Investigate further?

void bhv_bobomb_bully_death_smoke_init(void)
{
	o->oPosY -= 300.0f;

	s_set_scale(10.0f);
}

void bhv_bobomb_explosion_bubble_init(void)
{
	stSetScale(o, 2.0f, 2.0f, 1.0f);

	o->oBobombExpBubGfxExpRateX = (s32)(Randomf() * 2048.0f) + 0x800;
	o->oBobombExpBubGfxExpRateY = (s32)(Randomf() * 2048.0f) + 0x800;
	o->oTimer		    = Randomf() * 10.0f / FRAME_RATE_SCALER;
	o->oVelY		    = (s32)(Randomf() * 4.0f) + 4;
}

void bhv_bobomb_explosion_bubble_loop(void)
{
	f32 waterY = playerWorks[0].waterLevel;

	o->header.gfx.scale[0] = sins(o->oBobombExpBubGfxScaleFacX) * 0.5 + 2.0;
	o->oBobombExpBubGfxScaleFacX += o->oBobombExpBubGfxExpRateX * FRAME_RATE_SCALER;

	o->header.gfx.scale[1] = sins(o->oBobombExpBubGfxScaleFacY) * 0.5 + 2.0;
	o->oBobombExpBubGfxScaleFacY += o->oBobombExpBubGfxExpRateY * FRAME_RATE_SCALER;

	if(o->oPosY > waterY)
	{
		o->activeFlags = 0;
		o->oPosY += 5.0f * FRAME_RATE_SCALER;
		s_makeobj_nowpos(o, MODEL_SPOT_ON_GROUND, sm64::bhv::bhvWaterSurfaceWhiteWave2());
	}

	if(o->oTimer >= 61 * FRAME_RATE_SCALER_INV)
		o->activeFlags = 0;

	o->oPosY += o->oVelY * FRAME_RATE_SCALER;
	o->oTimer++;
}

void bhv_respawner_loop(void)
{
	struct Object* spawnedObject;

	if(!PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, o->oRespawnerMinSpawnDist))
	{
		spawnedObject		  = s_makeobj_nowpos(o, o->oRespawnerModelToRespawn, (const BehaviorScript*)o->oRespawnerBehaviorToRespawn);
		spawnedObject->oBehParams = o->oBehParams;
		o->activeFlags		  = 0;
	}
}

void Obj_reset(s32 model, const BehaviorScript* behToSpawn, s32 minSpawnDist)
{
	struct Object* respawner	       = s_makeobj_absolute(o, 0, MODEL_NONE, sm64::bhv::bhvRespawner(), o->oHomeX, o->oHomeY, o->oHomeZ, 0, 0, 0);
	respawner->oBehParams		       = o->oBehParams;
	respawner->oRespawnerModelToRespawn    = model;
	respawner->oRespawnerMinSpawnDist      = minSpawnDist;
	respawner->oRespawnerBehaviorToRespawn = behToSpawn;
}
