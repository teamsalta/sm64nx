/**
 * Behavior for sm64::bhv::bhvHiddenBlueCoin() and sm64::bhv::bhvBlueCoinSwitch().
 * sm64::bhv::bhvHiddenBlueCoin() are the stationary blue coins that appear when
 * you press a blue coin switch (a.k.a. sm64::bhv::bhvBlueCoinSwitch()).
 */

/**
 * Update function for sm64::bhv::bhvHiddenBlueCoin().
 */
void bhv_hidden_blue_coin_loop(void)
{
	struct Object* blueCoinSwitch;

	switch(o->oAction)
	{
		case HIDDEN_BLUE_COIN_ACT_INACTIVE:
			// Become invisible and intangible
			s_shapeOFF();
			s_hitOFF();

			// Set action to HIDDEN_BLUE_COIN_ACT_WAITING after the blue coin switch is found.
			o->oHiddenBlueCoinSwitch = s_find_obj(sm64::bhv::bhvBlueCoinSwitch());

			if(o->oHiddenBlueCoinSwitch != NULL)
			{
				o->oAction++;
			}

			break;
		case HIDDEN_BLUE_COIN_ACT_WAITING:
			// Wait until the blue coin switch starts ticking to activate.
			blueCoinSwitch = o->oHiddenBlueCoinSwitch;

			if(blueCoinSwitch->oAction == BLUE_COIN_SWITCH_ACT_TICKING)
			{
				o->oAction++; // Set to HIDDEN_BLUE_COIN_ACT_ACTIVE
			}

			break;
		case HIDDEN_BLUE_COIN_ACT_ACTIVE:
			// Become tangible
			s_shapeON();
			s_hitON();

			// Delete the coin once collected
			if(o->oInteractStatus & INT_STATUS_INTERACTED)
			{
				s_makeobj_nowpos(o, MODEL_SPARKLES, sm64::bhv::bhvGoldenCoinSparkles());
				s_remove_obj(o);
			}

			// After 200 frames of waiting and 20 2-frame blinks (for 240 frames total),
			// delete the object.
			if(s_flash_shape(200 * FRAME_RATE_SCALER_INV, 20))
			{
				s_remove_obj(o);
			}

			break;
	}

	o->oInteractStatus = 0;
}

/**
 * Update function for sm64::bhv::bhvBlueCoinSwitch().
 */
void bhv_blue_coin_switch_loop(void)
{
	// The switch's model is 1/3 size.
	s_set_scale(3.0f);

	switch(o->oAction)
	{
		case BLUE_COIN_SWITCH_ACT_IDLE:
			// If Mario is on the switch and has ground-pounded,
			// recede and get ready to start ticking.
			if(gMarioObject->platform == o)
			{
				if(playerWorks->status == ACT_GROUND_POUND_LAND)
				{
					// Set to BLUE_COIN_SWITCH_ACT_RECEDING
					o->oAction++;

					// Recede at a rate of 20 units/frame.
					o->oVelY = -20.0f;
					// Set gravity to 0 so it doesn't accelerate when receding.
					o->oGravity = 0.0f;

					objsound(SOUND_GENERAL_SWITCH_DOOR_OPEN);
				}
			}

			// Have collision
			stMainMoveBG();

			break;
		case BLUE_COIN_SWITCH_ACT_RECEDING:
			// Recede for 6 frames before going invisible and ticking.
			// This is probably an off-by-one error, since the switch is 100 units tall
			// and recedes at 20 units/frame, which means it will fully recede after 5 frames.
			if(o->oTimer > 5 * FRAME_RATE_SCALER_INV)
			{
				s_shape_hide();

				// Set to BLUE_COIN_SWITCH_ACT_TICKING
				o->oAction++;
				// ???
				o->oPosY = gMarioObject->oPosY - 40.0f;

				// Spawn particles. There's a function that calls this same function
				// with the same arguments, s_kemuri, why didn't they just call that?
				s_burneffect(0, 0, 46.0f);
			}
			else
			{
				// Have collision while receding
				stMainMoveBG();
				// Recede
				s_optionmove_F();
			}

			break;
		case BLUE_COIN_SWITCH_ACT_TICKING:
			// Tick faster when the blue coins start blinking
			if(o->oTimer < 200 * FRAME_RATE_SCALER_INV)
			{
				AudStartSound(SOUND_GENERAL2_SWITCH_TICK_FAST, gDefaultSoundArgs);
			}
			else
			{
				AudStartSound(SOUND_GENERAL2_SWITCH_TICK_SLOW, gDefaultSoundArgs);
			}

			// Delete the switch (which stops the sound) after the last coin is collected,
			// or after the coins unload after the 240-frame timer expires.
			if(s_find_obj(sm64::bhv::bhvHiddenBlueCoin()) == NULL || o->oTimer > 240 * FRAME_RATE_SCALER_INV)
			{
				s_remove_obj(o);
			}

			break;
	}
}
