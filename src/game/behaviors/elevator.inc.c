// elevator.c.inc

void func_802AD01C(void)
{
	objsound(SOUND_GENERAL_QUIET_POUND1);
	s_call_Viewshake(SHAKE_POS_SMALL);
}

void ActionElevator0(void)
{
	o->oVelY = 0;
	if(o->oElevatorUnk100 == 2)
	{
		if(gMarioObject->platform == o)
		{
			if(o->oPosY > o->oElevatorUnkFC)
				o->oAction = 2;
			else
				o->oAction = 1;
		}
	}
	else if(gMarioObject->oPosY > o->oElevatorUnkFC || o->oElevatorUnk100 == 1)
	{
		o->oPosY = o->oElevatorUnkF8;
		if(gMarioObject->platform == o)
			o->oAction = 2;
	}
	else
	{
		o->oPosY = o->oElevatorUnkF4;
		if(gMarioObject->platform == o)
			o->oAction = 1;
	}
}

void ActionElevator1(void)
{
	objsound_level(SOUND_ENV_ELEVATOR1);
	if(o->oTimer == 0 && s_rideon_player())
		func_802AD01C();
	s_chase_speed(&o->oVelY, 10.0f, 2.0f * FRAME_RATE_SCALER);
	o->oPosY += o->oVelY * FRAME_RATE_SCALER;
	if(o->oPosY > o->oElevatorUnkF8)
	{
		o->oPosY = o->oElevatorUnkF8;
		if(o->oElevatorUnk100 == 2 || o->oElevatorUnk100 == 1)
			o->oAction = 3;
		else if(gMarioObject->oPosY < o->oElevatorUnkFC)
			o->oAction = 2;
		else
			o->oAction = 3;
	}
}

void ActionElevator2() // Pretty similar code to action 1
{
	objsound_level(SOUND_ENV_ELEVATOR1);
	if(o->oTimer == 0 && s_rideon_player())
		func_802AD01C();
	s_chase_speed(&o->oVelY, -10.0f, -2.0f * FRAME_RATE_SCALER);
	o->oPosY += o->oVelY * FRAME_RATE_SCALER;
	if(o->oPosY < o->oElevatorUnkF4)
	{
		o->oPosY = o->oElevatorUnkF4;
		if(o->oElevatorUnk100 == 1)
			o->oAction = 4;
		else if(o->oElevatorUnk100 == 2)
			o->oAction = 3;
		else if(gMarioObject->oPosY > o->oElevatorUnkFC)
			o->oAction = 1;
		else
			o->oAction = 3;
	}
}

void ActionElevator4()
{
	o->oVelY = 0;
	if(o->oTimer == 0)
	{
		s_call_Viewshake(SHAKE_POS_SMALL);
		objsound(SOUND_GENERAL_METAL_POUND);
	}
	if(!s_check_playerjump() && !s_rideon_player())
		o->oAction = 1;
}

void ActionElevator3() // nearly identical to action 2
{
	o->oVelY = 0;
	if(o->oTimer == 0)
	{
		s_call_Viewshake(SHAKE_POS_SMALL);
		objsound(SOUND_GENERAL_METAL_POUND);
	}
	if(!s_check_playerjump() && !s_rideon_player())
		o->oAction = 0;
}

void bhv_elevator_init(void)
{
	s32 sp1C = D_8032F38C[o->oBehParams2ndByte * 3 + 2];
	if(sp1C == 0)
	{
		o->oElevatorUnkF4  = D_8032F38C[o->oBehParams2ndByte * 3];
		o->oElevatorUnkF8  = o->oHomeY;
		o->oElevatorUnkFC  = (o->oElevatorUnkF4 + o->oElevatorUnkF8) / 2.0f;
		o->oElevatorUnk100 = s_check_pathname(sm64::bhv::bhvRrElevatorPlatform());
	}
	else
	{
		o->oElevatorUnkF4  = D_8032F38C[o->oBehParams2ndByte * 3];
		o->oElevatorUnkF8  = D_8032F38C[o->oBehParams2ndByte * 3 + 1];
		o->oElevatorUnkFC  = (o->oElevatorUnkF4 + o->oElevatorUnkF8) / 2.0f;
		o->oElevatorUnk100 = 2;
	}
}

void (*sElevatorActions[])(void) = {ActionElevator0, ActionElevator1, ActionElevator2, ActionElevator3, ActionElevator4};

struct SpawnParticlesInfo D_8032F3CC = {3, 20, MODEL_MIST, 20, 10, 5, 0, 0, 0, 30, 30.0f, 1.5f};

struct SpawnParticlesInfo D_8032F3E0 = {0, 5, MODEL_SAND_DUST, 0, 0, 20, 20, 0, 252, 30, 5.0f, 2.0f};

s16 D_8032F3F4[] = {2, -8, 1, 4};

struct SpawnParticlesInfo D_8032F3FC = {0, 5, MODEL_WHITE_PARTICLE_DL, 0, 0, 20, 20, 0, 252, 30, 2.0f, 2.0f};

void bhv_elevator_loop(void)
{
	s_modejmp(sElevatorActions);
}
