// spindrift.c.inc

struct ObjectHitbox sSpindriftHitbox = {
    /* interactType: */ INTERACT_BOUNCE_TOP,
    /* downOffset: */ 0,
    /* damageOrCoinValue: */ 2,
    /* health: */ 1,
    /* numLootCoins: */ 3,
    /* radius: */ 90,
    /* height: */ 80,
    /* hurtboxRadius: */ 80,
    /* hurtboxHeight: */ 70,
};

void bhv_spindrift_loop(void)
{
	o->activeFlags |= 0x400;
	if(s_hit_remove_enemy(&sSpindriftHitbox, SOUND_OBJ_DYING_ENEMY1, 0))
		s_set_newmode(1);
	s_enemybgcheck();
	switch(o->oAction)
	{
		case 0:
			lerp(&o->oForwardVel, 4.0f, 1.0f * FRAME_RATE_SCALER);
			if(s_calc_playerscope() > 1000.0f)
				o->oAngleToMario = s_calc_returnangle();
			else if(o->oDistanceToMario > 300.0f)
				o->oAngleToMario = s_calc_targetangle(o, gMarioObject);
			s_chase_angleY(o->oAngleToMario, 0x400 / FRAME_RATE_SCALER_INV);
			break;
		case 1:
			o->oFlags &= ~8;
			o->oForwardVel = -10.0f;
			if(o->oTimer > 20 * FRAME_RATE_SCALER_INV)
			{
				o->oAction	   = 0;
				o->oInteractStatus = 0;
				o->oFlags |= 8;
			}
			break;
	}
	s_enemymove(-60);
}
