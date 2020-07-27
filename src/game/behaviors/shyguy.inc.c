
/**
 * Behavior for sm64::bhv::bhvShyGuy() and sm64::bhv::bhvShyGuyTripletSpawner(),
 * ShyGuys can either be spawned individually, or spawned by a triplet spawner.
 * The triplet spawner comes before its spawned shyguys in processing order.
 */

#define FIRE_SNIPE_DISTANCE 2500.0f
#define CHASE_DISTANCE 1000.0f

/**
 * Hitbox for shyguy.
 */
static struct ObjectHitbox sShyGuyHitbox = {
    /* interactType:      */ INTERACT_BOUNCE_TOP,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 0,
    /* numLootCoins:      */ 1,
    /* radius:            */ 72,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 42,
    /* hurtboxHeight:     */ 40,
};

/**
 * Properties that vary based on shyguy size.
 */
struct ShyGuyProperties
{
	f32 scale;
	u32 deathSound;
	s16 drawDistance;
	s8 damage;
};

/**
 * Properties for regular, huge, and tiny shyguys.
 */
static struct ShyGuyProperties sShyGuyProperties[] = {
    {1.5f, SOUND_OBJ_ENEMY_DEATH_HIGH, 4000, 2},
    {3.5f, SOUND_OBJ_ENEMY_DEATH_LOW, 4000, 3},
    {0.5f, SOUND_OBJ_ENEMY_DEATH_HIGH, 1500, 0},
};

/**
 * Attack handlers for shyguys.
 */
static u8 sShyGuyAttackHandlers[][6] = {
    // regular and tiny
    {
	/* ATTACK_PUNCH:                 */ ATTACK_HANDLER_KNOCKBACK,
	/* ATTACK_KICK_OR_TRIP:          */ ATTACK_HANDLER_KNOCKBACK,
	/* ATTACK_FROM_ABOVE:            */ ATTACK_HANDLER_SQUISHED,
	/* ATTACK_GROUND_POUND_OR_TWIRL: */ ATTACK_HANDLER_SQUISHED,
	/* ATTACK_FAST_ATTACK:           */ ATTACK_HANDLER_KNOCKBACK,
	/* ATTACK_FROM_BELOW:            */ ATTACK_HANDLER_KNOCKBACK,
    },
    // huge
    {
	/* ATTACK_PUNCH:                 */ ATTACK_HANDLER_SPECIAL_HUGE_GOOMBA_WEAKLY_ATTACKED,
	/* ATTACK_KICK_OR_TRIP:          */ ATTACK_HANDLER_SPECIAL_HUGE_GOOMBA_WEAKLY_ATTACKED,
	/* ATTACK_FROM_ABOVE:            */ ATTACK_HANDLER_SQUISHED,
	/* ATTACK_GROUND_POUND_OR_TWIRL: */ ATTACK_HANDLER_SQUISHED_WITH_BLUE_COIN,
	/* ATTACK_FAST_ATTACK:           */ ATTACK_HANDLER_SPECIAL_HUGE_GOOMBA_WEAKLY_ATTACKED,
	/* ATTACK_FROM_BELOW:            */ ATTACK_HANDLER_SPECIAL_HUGE_GOOMBA_WEAKLY_ATTACKED,
    },
};

/**
 * Update function for shyguy triplet spawner.
 */
void bhv_shyguy_triplet_spawner_update(void)
{
	UNUSED s32 unused1;
	s16 shyguyFlag;
	UNUSED s16 unused2;
	s32 angle;
	s32 dAngle;
	s16 dx;
	s16 dz;

	// If mario is close enough and the shyguys aren't currently loaded, then
	// spawn them
	if(o->oAction == GOOMBA_TRIPLET_SPAWNER_ACT_UNLOADED)
	{
		if(o->oDistanceToMario < 3000.0f && !sm64::config().camera().disableDistanceClip())
		{
			// The spawner is capable of spawning more than 3 shyguys, but this
			// is not used in the game
			dAngle = 0x10000 / (((o->oBehParams2ndByte & GOOMBA_TRIPLET_SPAWNER_BP_EXTRA_GOOMBAS_MASK) >> 2) + 3);

			shyguyFlag = 1 << 8;
			for(angle = 0; angle < 0xFFFF; angle += dAngle, shyguyFlag <<= 1)
			{
				// Only spawn shyguys which haven't been killed yet
				if(!(o->oBehParams & shyguyFlag))
				{
					dx = 500.0f * coss(angle);
					dz = 500.0f * sins(angle);

					s_makeobj_chain((o->oBehParams2ndByte & GOOMBA_TRIPLET_SPAWNER_BP_SIZE_MASK) | (shyguyFlag >> 6), dx, 0, dz, o, MODEL_GOOMBA, sm64::bhv::bhvShyGuy());
				}
			}

			o->oAction += 1;
		}
	}
	else if(o->oDistanceToMario > 4000.0f && !sm64::config().camera().disableDistanceClip())
	{
		// If mario is too far away, enter the unloaded action. The shyguys
		// will detect this and unload themselves
		o->oAction = GOOMBA_TRIPLET_SPAWNER_ACT_UNLOADED;
	}
}

/**
 * Initialization function for shyguy.
 */
void bhv_shyguy_init(void)
{
	o->oGoombaSize = o->oBehParams2ndByte & GOOMBA_BP_SIZE_MASK;

	o->oGoombaScale = sShyGuyProperties[o->oGoombaSize].scale;
	o->oDeathSound	= sShyGuyProperties[o->oGoombaSize].deathSound;

	s_set_hitparam(o, &sShyGuyHitbox);

	o->oDrawingDistance   = sShyGuyProperties[o->oGoombaSize].drawDistance;
	o->oDamageOrCoinValue = sShyGuyProperties[o->oGoombaSize].damage;
	o->oNumLootCoins      = 2;

	o->oGravity = -8.0f / 3.0f * o->oGoombaScale;
}

/**
 * Enter the jump action and set initial y velocity.
 */
static void shyguy_begin_jump(void)
{
	objsound(SOUND_OBJ_GOOMBA_ALERT);
	o->oAction     = GOOMBA_ACT_JUMP;
	o->oForwardVel = 0.0f;
	o->oVelY       = 50.0f / 3.0f * o->oGoombaScale;
}

/**
 * If spawned by a triplet spawner, mark the flag in the spawner to indicate that
 * this shyguy died. This prevents it from spawning again when mario leaves and
 * comes back.
 */
static void mark_shyguy_as_dead(void)
{
	if(o->parentObj != o)
	{
		set_object_respawn_info_bits(o->parentObj, (o->oBehParams2ndByte & GOOMBA_BP_TRIPLET_FLAG_MASK) >> 2);

		o->parentObj->oBehParams = o->parentObj->oBehParams | (o->oBehParams2ndByte & GOOMBA_BP_TRIPLET_FLAG_MASK) << 6;
	}
}

static void shyguy_act_fire()
{
	static const u64 fireBallCount = 3;
	static const u64 interval      = 1;

	objsound(SOUND_OBJ_FLAME_BLOWN);
	if(o->oTimer % (interval * FRAME_RATE_SCALER_INV) == 0)
	{
		obj_spit_bouncing_fire(0, 0, 0, 5.0f / 2, MODEL_RED_FLAME_SHADOW, 20.0f * 2, 15.0f * 2, 0x1000 * 2);
	}

	if(o->oTimer > interval * fireBallCount * FRAME_RATE_SCALER_INV)
	{
		o->oAction = GOOMBA_ACT_WALK;
	}
}

/**
 * Walk around randomly occasionally jumping. If mario comes within range,
 * chase him.
 */
static void shyguy_act_walk(void)
{
	treat_far_home_as_mario(FIRE_SNIPE_DISTANCE);

	obj_forward_vel_approach(o->oGoombaRelativeSpeed * o->oGoombaScale, 0.4f * 3 * FRAME_RATE_SCALER);

	// If walking fast enough, play footstep sounds
	if(o->oGoombaRelativeSpeed > 4.0f / 3.0f)
	{
		func_802F9378(2, 17, SOUND_OBJ_GOOMBA_WALK);
	}

	//! By strategically hitting a wall, steep slope, or another shyguy, we can
	//  prevent the shyguy from turning back toward home for a while (shyguy
	//  chase extension)
	//! It seems theoretically possible to get 2-3 shyguys to repeatedly touch
	//  each other and move arbitrarily far from their home, but it's
	//  extremely precise and chaotic in practice, so probably can't be performed
	//  for nontrivial distances
	if(o->oGoombaTurningAwayFromWall)
	{
		o->oGoombaTurningAwayFromWall = obj_resolve_collisions_and_turn(o->oGoombaTargetYaw, 0x200);
	}
	else
	{
		// If far from home, walk toward home.
		if(o->oDistanceToMario >= 25000.0f)
		{
			o->oGoombaTargetYaw = o->oAngleToMario;
			o->oGoombaWalkTimer = random_linear_offset(20, 30) * FRAME_RATE_SCALER_INV;
		}

		if(!(o->oGoombaTurningAwayFromWall = obj_bounce_off_walls_edges_objects(&o->oGoombaTargetYaw)))
		{
			if(o->oDistanceToMario < CHASE_DISTANCE)
			{
				// If close to mario, begin chasing him. If not already chasing
				// him, jump first
				o->oGoombaTargetYaw = o->oAngleToMario + DEGREES(180.0f);

				if(o->oGoombaRelativeSpeed <= 2.0f)
				{
					shyguy_begin_jump();

					// o->oGoombaTargetYaw = o->oAngleToMario + DEGREES(180.0f);
					o->oGoombaRelativeSpeed = 40.0f * 2.0f;
				}
			}
			else if(o->oDistanceToMario < FIRE_SNIPE_DISTANCE)
			{
				o->oGoombaRelativeSpeed = 4.0f / 3.0f;

				auto aMario = angle_to_object(gCurrentObject, gMarioObject);
				s16 dist    = abs_angle_diff(aMario, o->oMoveAngleYaw);

				if(dist < DEGREES(1.0f) && o->oTimer > 3 * 30 * FRAME_RATE_SCALER_INV)
				{
					o->oAction = SHYGUY_ACT_FIRE;
				}
				else
				{
					o->oGoombaTargetYaw = o->oAngleToMario;
				}
			}
			else
			{
				// If mario is far away, walk at a normal pace, turning randomly
				// and occasionally jumping

				o->oGoombaRelativeSpeed = 4.0f / 3.0f;
				if(o->oGoombaWalkTimer != 0)
				{
					o->oGoombaWalkTimer -= 1;
				}
				else
				{
					if(RandomU16() & 3)
					{
						o->oGoombaTargetYaw = obj_random_fixed_turn(0x2000);
						o->oGoombaWalkTimer = random_linear_offset(100, 100) * FRAME_RATE_SCALER_INV;
					}
					else
					{
						shyguy_begin_jump();
						o->oGoombaTargetYaw = obj_random_fixed_turn(0x6000);
					}
				}
			}
		}
#ifdef DEBUG
		else
		{
			int y = 0;
		}
#endif

		obj_rotate_yaw_toward(o->oGoombaTargetYaw, 0x200 * 2 / FRAME_RATE_SCALER_INV);
	}
}

/**
 * This action occurs when either the shyguy attacks mario normally, or mario
 * attacks a huge shyguy with an attack that doesn't kill it.
 */
static void shyguy_act_attacked_mario(void)
{
	if(o->oGoombaSize == GOOMBA_SIZE_TINY)
	{
		mark_shyguy_as_dead();
		o->oNumLootCoins = 0;
		obj_die_if_health_non_positive();
	}
	else
	{
		//! This can happen even when the shyguy is already in the air. It's
		//  hard to chain these in practice
		shyguy_begin_jump();
		o->oGoombaTargetYaw	      = o->oAngleToMario;
		o->oGoombaTurningAwayFromWall = FALSE;
	}
}

/**
 * Move until landing, and rotate toward target yaw.
 */
static void shyguy_act_jump(void)
{
	obj_resolve_object_collisions(NULL);

	//! If we move outside the shyguy's drawing radius the frame it enters the
	//  jump action, then it will keep its velY, but it will still be counted
	//  as being on the ground.
	//  Next frame, the jump action will think it has already ended because it is
	//  still on the ground.
	//  This puts the shyguy back in the walk action, but the positive velY will
	//  make it hop into the air. We can then trigger another jump.
	if(o->oMoveFlags & OBJ_MOVE_MASK_ON_GROUND)
	{
		o->oAction = GOOMBA_ACT_WALK;
	}
	else
	{
		obj_rotate_yaw_toward(o->oGoombaTargetYaw, 0x800 / FRAME_RATE_SCALER_INV);
	}
}

/**
 * Attack handler for when mario attacks a huge shyguy with an attack that
 * doesn't kill it.
 * From the shyguy's perspective, this is the same as the shyguy attacking
 * mario.
 */
void huge_shyguy_weakly_attacked(void)
{
	o->oAction = GOOMBA_ACT_ATTACKED_MARIO;
}

/**
 * Update function for shyguy.
 */
void bhv_shyguy_update(void)
{
	// PARTIAL_UPDATE

	f32 animSpeed;

	if(obj_update_standard_actions(o->oGoombaScale))
	{
		// If this shyguy has a spawner and mario moved away from the spawner,
		// unload
		if(o->parentObj != o)
		{
			if(o->parentObj->oAction == GOOMBA_TRIPLET_SPAWNER_ACT_UNLOADED)
			{
				s_remove_obj(o);
			}
		}

		s_set_scale(o->oGoombaScale);
		obj_update_blinking(&o->oGoombaBlinkTimer, 30, 50, 5);
		obj_update_floor_and_walls();

		if((animSpeed = o->oForwardVel / o->oGoombaScale * 0.4f) < 1.0f)
		{
			animSpeed = 1.0f;
		}
		func_8029ED98(0, animSpeed);

		switch(o->oAction)
		{
			case SHYGUY_ACT_WALK:
				shyguy_act_walk();
				break;
			case SHYGUY_ACT_ATTACKED_MARIO:
				shyguy_act_attacked_mario();
				break;
			case SHYGUY_ACT_JUMP:
				shyguy_act_jump();
				break;
			case SHYGUY_ACT_FIRE:
				shyguy_act_fire();
				break;
		}

		//! @bug Weak attacks on huge shyguys in a triplet mark them as dead even if they're not.
		// obj_handle_attacks returns the type of the attack, which is non-zero
		// even for Mario's weak attacks. Thus, if Mario weakly attacks a huge shyguy
		// without harming it (e.g. by punching it), the shyguy will be marked as dead
		// and will not respawn if Mario leaves and re-enters the spawner's radius
		// even though the shyguy isn't actually dead.
		if(obj_handle_attacks(&sShyGuyHitbox, GOOMBA_ACT_ATTACKED_MARIO, sShyGuyAttackHandlers[o->oGoombaSize & 1]))
		{
			mark_shyguy_as_dead();
		}

		obj_move_standard(-78);
	}
	else
	{
		o->oAnimState = TRUE;
	}
}
