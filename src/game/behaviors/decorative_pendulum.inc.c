/**
 * Behavior for bhvDecorativePendulum.
 * This controls the pendulum that lies underneath the Tick Tock Clock painting.
 */

void s_clock_furiko_init(void)
{
	o->oAngleVelRoll = 0x100;
	s_areastage_init();
}

/**
 * Smoothly swing the decorative pendulum back and forth using constant angular
 * acceleration.
 */
void s_clock_furiko_main(void)
{
	if(o->oFaceAngleRoll > 0)
	{
		o->oAngleVelRoll -= 0x08 / FRAME_RATE_SCALER_INV;
	}
	else
	{
		o->oAngleVelRoll += 0x08 / FRAME_RATE_SCALER_INV;
	}

	o->oFaceAngleRoll += o->oAngleVelRoll * FRAME_RATE_SCALER;

	/**
	 * This if-statement is true twice in the span of 5 frames when
	 * oAngleVelRoll takes values in {0x10, 0x08, 0x00, -0x08, -0x10}.
	 * This means the sound we hear when the pendulum hits its upswing is
	 * actually one sound played twice in rapid succession.
	 */
	if(o->oAngleVelRoll == 0x10 || o->oAngleVelRoll == -0x10)
	{
		objsound(SOUND_GENERAL_BIG_CLOCK);
	}
}
