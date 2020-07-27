#include <ultra64.h>

#include "sm64.h"
#include "interaction.h"
#include "camera.h"
#include "level_update.h"
#include "engine/math_util.h"
#include "memory.h"
#include "area.h"
#include "save_file.h"
#include "engine/surface_collision.h"
#include "sound_init.h"
#include "display.h"
#include "mario.h"
#include "obj_behaviors.h"
#include "object_helpers.h"
#include "behavior_actions.h"
#include "audio/external.h"
#include "behavior_data.h"
#include "dialog_ids.h"
#include "course_table.h"
#include "actors/common1.h"
#include "game/motor.h"

#pragma GCC push_options
#pragma GCC optimize("O0")

#define INT_GROUND_POUND_OR_TWIRL (1 << 0) // 0x00000001
#define INT_PUNCH (1 << 1)		   // 0x00000002
#define INT_KICK (1 << 2)		   // 0x00000004
#define INT_TRIP (1 << 3)		   // 0x00000008
#define INT_SLIDE_KICK (1 << 4)		   // 0x00000010
#define INT_FAST_ATTACK_OR_SHELL (1 << 5)  // 0x00000020
#define INT_HIT_FROM_ABOVE (1 << 6)	   // 0x00000040
#define INT_HIT_FROM_BELOW (1 << 7)	   // 0x00000080

#define INT_ATTACK_NOT_FROM_BELOW (INT_GROUND_POUND_OR_TWIRL | INT_PUNCH | INT_KICK | INT_TRIP | INT_SLIDE_KICK | INT_FAST_ATTACK_OR_SHELL | INT_HIT_FROM_ABOVE)

#define INT_ANY_ATTACK (INT_GROUND_POUND_OR_TWIRL | INT_PUNCH | INT_KICK | INT_TRIP | INT_SLIDE_KICK | INT_FAST_ATTACK_OR_SHELL | INT_HIT_FROM_ABOVE | INT_HIT_FROM_BELOW)

#define INT_ATTACK_MASK_UNKNOWN_8F (INT_GROUND_POUND_OR_TWIRL | INT_PUNCH | INT_KICK | INT_TRIP | INT_HIT_FROM_BELOW)

u8 sDelayInvincTimer;
s16 sInvulnerable;

u32 interact_coin(struct PlayerRecord*, u32, struct Object*);
u32 interact_water_ring(struct PlayerRecord*, u32, struct Object*);
u32 interact_star_or_key(struct PlayerRecord*, u32, struct Object*);
u32 interact_bbh_entrance(struct PlayerRecord*, u32, struct Object*);
u32 interact_warp(struct PlayerRecord*, u32, struct Object*);
u32 interact_warp_door(struct PlayerRecord*, u32, struct Object*);
u32 interact_door(struct PlayerRecord*, u32, struct Object*);
u32 interact_cannon_base(struct PlayerRecord*, u32, struct Object*);
u32 interact_igloo_barrier(struct PlayerRecord*, u32, struct Object*);
u32 interact_tornado(struct PlayerRecord*, u32, struct Object*);
u32 interact_whirlpool(struct PlayerRecord*, u32, struct Object*);
u32 interact_wind(struct PlayerRecord*, u32, struct Object*);
u32 interact_flame(struct PlayerRecord*, u32, struct Object*);
u32 interact_snufit_bullet(struct PlayerRecord*, u32, struct Object*);
u32 interact_clam_or_bubba(struct PlayerRecord*, u32, struct Object*);
u32 interact_bully(struct PlayerRecord*, u32, struct Object*);
u32 interact_shock(struct PlayerRecord*, u32, struct Object*);
u32 interact_mr_blizzard(struct PlayerRecord*, u32, struct Object*);
u32 interact_hit_from_below(struct PlayerRecord*, u32, struct Object*);
u32 interact_bounce_top(struct PlayerRecord*, u32, struct Object*);
u32 interact_unknown_08(struct PlayerRecord*, u32, struct Object*);
u32 interact_damage(struct PlayerRecord*, u32, struct Object*);
u32 interact_breakable(struct PlayerRecord*, u32, struct Object*);
u32 interact_koopa_shell(struct PlayerRecord*, u32, struct Object*);
u32 interact_pole(struct PlayerRecord*, u32, struct Object*);
u32 interact_hoot(struct PlayerRecord*, u32, struct Object*);
u32 interact_cap(struct PlayerRecord*, u32, struct Object*);
u32 interact_grabbable(struct PlayerRecord*, u32, struct Object*);
u32 interact_text(struct PlayerRecord*, u32, struct Object*);

struct InteractionHandler
{
	u32 interactType;
	u32 (*handler)(struct PlayerRecord*, u32, struct Object*);
};

static struct InteractionHandler sInteractionHandlers[] = {
    {INTERACT_COIN, interact_coin},
    {INTERACT_WATER_RING, interact_water_ring},
    {INTERACT_STAR_OR_KEY, interact_star_or_key},
    {INTERACT_BBH_ENTRANCE, interact_bbh_entrance},
    {INTERACT_WARP, interact_warp},
    {INTERACT_WARP_DOOR, interact_warp_door},
    {INTERACT_DOOR, interact_door},
    {INTERACT_CANNON_BASE, interact_cannon_base},
    {INTERACT_IGLOO_BARRIER, interact_igloo_barrier},
    {INTERACT_TORNADO, interact_tornado},
    {INTERACT_WHIRLPOOL, interact_whirlpool},
    {INTERACT_WIND, interact_wind},
    {INTERACT_FLAME, interact_flame},
    {INTERACT_SNUFIT_BULLET, interact_snufit_bullet},
    {INTERACT_CLAM_OR_BUBBA, interact_clam_or_bubba},
    {INTERACT_BULLY, interact_bully},
    {INTERACT_SHOCK, interact_shock},
    {INTERACT_TRAP_TURN, interact_bounce_top},
    {INTERACT_MR_BLIZZARD, interact_mr_blizzard},
    {INTERACT_HIT_FROM_BELOW, interact_hit_from_below},
    {INTERACT_BOUNCE_TOP, interact_bounce_top},
    {INTERACT_DAMAGE, interact_damage},
    {INTERACT_POLE, interact_pole},
    {INTERACT_HOOT, interact_hoot},
    {INTERACT_BREAKABLE, interact_breakable},
    {INTERACT_KOOPA, interact_bounce_top},
    {INTERACT_KOOPA_SHELL, interact_koopa_shell},
    {INTERACT_UNKNOWN_08, interact_unknown_08},
    {INTERACT_CAP, interact_cap},
    {INTERACT_GRABBABLE, interact_grabbable},
    {INTERACT_TEXT, interact_text},
};

static u32 sForwardKnockbackActions[][3] = {
    {ACT_SOFT_FORWARD_GROUND_KB, ACT_FORWARD_GROUND_KB, ACT_HARD_FORWARD_GROUND_KB},
    {ACT_FORWARD_AIR_KB, ACT_FORWARD_AIR_KB, ACT_HARD_FORWARD_AIR_KB},
    {ACT_FORWARD_WATER_KB, ACT_FORWARD_WATER_KB, ACT_FORWARD_WATER_KB},
};

static u32 sBackwardKnockbackActions[][3] = {
    {ACT_SOFT_BACKWARD_GROUND_KB, ACT_BACKWARD_GROUND_KB, ACT_HARD_BACKWARD_GROUND_KB},
    {ACT_BACKWARD_AIR_KB, ACT_BACKWARD_AIR_KB, ACT_HARD_BACKWARD_AIR_KB},
    {ACT_BACKWARD_WATER_KB, ACT_BACKWARD_WATER_KB, ACT_BACKWARD_WATER_KB},
};

static u8 sDisplayingDoorText = FALSE;
static u8 sJustTeleported     = FALSE;
static u8 sPssSlideStarted    = FALSE;

u32 get_mario_cap_flag(struct Object* capObject)
{
	void* script = virtual_to_segmented(0x13, capObject->behavior);

	if(script == sm64::bhv::bhvNormalCap())
	{
		return MARIO_NORMAL_CAP;
	}
	else if(script == sm64::bhv::bhvMetalCap())
	{
		return MARIO_METAL_CAP;
	}
	else if(script == sm64::bhv::bhvWingCap())
	{
		return MARIO_WING_CAP;
	}
	else if(script == sm64::bhv::bhvVanishCap())
	{
		return MARIO_VANISH_CAP;
	}

	return 0;
}

u32 object_facing_mario(struct PlayerRecord* m, struct Object* o, s16 angleRange)
{
	f32 dx = m->pos[0] - o->oPosX;
	f32 dz = m->pos[2] - o->oPosZ;

	s16 angleToMario = atan2s(dz, dx);
	s16 dAngle	 = angleToMario - o->oMoveAngleYaw;

	if(-angleRange <= dAngle && dAngle <= angleRange)
	{
		return TRUE;
	}

	return FALSE;
}

s16 mario_angle_to_object(struct PlayerRecord* m, struct Object* o)
{
	f32 dx = o->oPosX - m->pos[0];
	f32 dz = o->oPosZ - m->pos[2];

	return atan2s(dz, dx);
}

u32 determine_interaction(struct PlayerRecord* m, struct Object* o)
{
	u32 interaction = 0;
	u32 action	= m->status;

	if(action & ACT_FLAG_ATTACKING)
	{
		if(action == ACT_PUNCHING || action == ACT_MOVE_PUNCHING || action == ACT_JUMP_KICK)
		{
			s16 dYawToObject = mario_angle_to_object(m, o) - m->faceAngle[1];

			if(m->flags & MARIO_PUNCHING)
			{
				if(-0x2AAA <= dYawToObject && dYawToObject <= 0x2AAA)
				{
					interaction = INT_PUNCH;
				}
			}
			if(m->flags & MARIO_KICKING)
			{
				if(-0x2AAA <= dYawToObject && dYawToObject <= 0x2AAA)
				{
					interaction = INT_KICK;
				}
			}
			if(m->flags & MARIO_TRIPPING)
			{
				if(-0x4000 <= dYawToObject && dYawToObject <= 0x4000)
				{
					interaction = INT_TRIP;
				}
			}
		}
		else if(action == ACT_GROUND_POUND || action == ACT_TWIRLING)
		{
			if(m->vel[1] < 0.0f)
			{
				interaction = INT_GROUND_POUND_OR_TWIRL;
			}
		}
		else if(action == ACT_GROUND_POUND_LAND || action == ACT_TWIRL_LAND)
		{
			if(m->vel[1] < 0.0f && m->actionState == 0)
			{
				interaction = INT_GROUND_POUND_OR_TWIRL;
			}
		}
		else if(action == ACT_SLIDE_KICK || action == ACT_SLIDE_KICK_SLIDE)
		{
			interaction = INT_SLIDE_KICK;
		}
		else if(action & ACT_FLAG_RIDING_SHELL)
		{
			interaction = INT_FAST_ATTACK_OR_SHELL;
		}
		else if(m->forwardVel <= -26.0f || 26.0f <= m->forwardVel)
		{
			interaction = INT_FAST_ATTACK_OR_SHELL;
		}
	}

	if(interaction == 0 && (action & ACT_FLAG_AIR))
	{
		if(m->vel[1] < 0.0f)
		{
			if(m->pos[1] > o->oPosY)
			{
				interaction = INT_HIT_FROM_ABOVE;
			}
		}
		else
		{
			if(m->pos[1] < o->oPosY)
			{
				interaction = INT_HIT_FROM_BELOW;
			}
		}
	}

	return interaction;
}

u32 attack_object(struct Object* o, s32 interaction)
{
	u32 attackType = 0;

	switch(interaction)
	{
		case INT_GROUND_POUND_OR_TWIRL:
			attackType = ATTACK_GROUND_POUND_OR_TWIRL;
			break;
		case INT_PUNCH:
			attackType = ATTACK_PUNCH;
			break;
		case INT_KICK:
		case INT_TRIP:
			attackType = ATTACK_KICK_OR_TRIP;
			break;
		case INT_SLIDE_KICK:
		case INT_FAST_ATTACK_OR_SHELL:
			attackType = ATTACK_FAST_ATTACK;
			break;
		case INT_HIT_FROM_ABOVE:
			attackType = ATTACK_FROM_ABOVE;
			break;
		case INT_HIT_FROM_BELOW:
			attackType = ATTACK_FROM_BELOW;
			break;
	}

	o->oInteractStatus = attackType + (INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
	return attackType;
}

void mario_stop_riding_object(struct PlayerRecord* m)
{
	if(m->riddenObj != NULL)
	{
		m->riddenObj->oInteractStatus = INT_STATUS_STOP_RIDING;
		stop_shell_music();
		m->riddenObj = NULL;
	}
}

void mario_grab_used_object(struct PlayerRecord* m)
{
	if(m->heldObj == NULL)
	{
		m->heldObj = m->usedObj;
		set_object_held_state(m->heldObj, sm64::bhv::bhvCarrySomething3());
	}
}

void mario_drop_held_object(struct PlayerRecord* m)
{
	if(m->heldObj != NULL)
	{
		if(m->heldObj->behavior == segmented_to_virtual(sm64::bhv::bhvKoopaShellUnderwater()))
		{
			stop_shell_music();
		}

		set_object_held_state(m->heldObj, sm64::bhv::bhvCarrySomething4());

		m->heldObj->oPosX = m->marioBodyState->unk18[0];
		m->heldObj->oPosY = m->pos[1];
		m->heldObj->oPosZ = m->marioBodyState->unk18[2];

		m->heldObj->oMoveAngleYaw = m->faceAngle[1];

		m->heldObj = NULL;
	}
}

void mario_throw_held_object(struct PlayerRecord* m)
{
	if(m->heldObj != NULL)
	{
		if(m->heldObj->behavior == segmented_to_virtual(sm64::bhv::bhvKoopaShellUnderwater()))
		{
			stop_shell_music();
		}

		set_object_held_state(m->heldObj, sm64::bhv::bhvCarrySomething5());

		m->heldObj->oPosX = m->marioBodyState->unk18[0] + 32.0f * sins(m->faceAngle[1]);
		m->heldObj->oPosY = m->marioBodyState->unk18[1];
		m->heldObj->oPosZ = m->marioBodyState->unk18[2] + 32.0f * coss(m->faceAngle[1]);

		m->heldObj->oMoveAngleYaw = m->faceAngle[1];

		m->heldObj = NULL;
	}
}

void mario_stop_riding_and_holding(struct PlayerRecord* m)
{
	mario_drop_held_object(m);
	mario_stop_riding_object(m);

	if(m->status == ACT_RIDING_HOOT)
	{
		m->usedObj->oInteractStatus	  = 0;
		m->usedObj->oHootMarioReleaseTime = frameCounter;
	}
}

u32 does_mario_have_hat(struct PlayerRecord* m)
{
	return (m->flags & MARIO_CAP_FLAGS) == (MARIO_NORMAL_CAP | MARIO_CAP_ON_HEAD);
}

void mario_blow_off_cap(struct PlayerRecord* m, f32 capSpeed)
{
	struct Object* capObject;

	if(does_mario_have_hat(m))
	{
		BuSetHatPosition(m->pos[0], m->pos[1], m->pos[2]);

		m->flags &= ~(MARIO_NORMAL_CAP | MARIO_CAP_ON_HEAD);

		capObject = s_makeobj_nowpos(m->marioObj, MODEL_MARIOS_CAP, sm64::bhv::bhvNormalCap());

		capObject->oPosY += (m->status & ACT_FLAG_SHORT_HITBOX) ? 120.0f : 180.0f;
		capObject->oForwardVel	 = capSpeed;
		capObject->oMoveAngleYaw = (s16)(m->faceAngle[1] + 0x400);

		if(m->forwardVel < 0.0f)
		{
			capObject->oMoveAngleYaw = (s16)(capObject->oMoveAngleYaw + 0x8000);
		}
	}
}

u32 mario_lose_cap_to_enemy(u32 arg)
{
	u32 wasWearingCap = FALSE;

	if(does_mario_have_hat(marioWorks))
	{
		BuSetItemFlag(arg == 1 ? SAVE_FLAG_CAP_ON_KLEPTO : SAVE_FLAG_CAP_ON_UKIKI);
		marioWorks->flags &= ~(MARIO_NORMAL_CAP | MARIO_CAP_ON_HEAD);
		wasWearingCap = TRUE;
	}

	return wasWearingCap;
}

void mario_retrieve_cap(void)
{
	mario_drop_held_object(marioWorks);
	BuClrItemFlag(SAVE_FLAG_CAP_ON_KLEPTO | SAVE_FLAG_CAP_ON_UKIKI);
	marioWorks->flags &= ~MARIO_CAP_ON_HEAD;
	marioWorks->flags |= MARIO_NORMAL_CAP | MARIO_CAP_IN_HAND;
}

u32 able_to_grab_object(struct PlayerRecord* m, UNUSED struct Object* o)
{
	u32 action = m->status;

	if(action == ACT_DIVE_SLIDE || action == ACT_DIVE)
	{
		if(!(o->oInteractionSubtype & INT_SUBTYPE_GRABS_MARIO))
		{
			return TRUE;
		}
	}
	else if(action == ACT_PUNCHING || action == ACT_MOVE_PUNCHING)
	{
		if(m->actionArg < 2)
		{
			return TRUE;
		}
	}

	return FALSE;
}

Object* mario_get_collided_object(struct PlayerRecord* m, u32 interactType)
{
	s32 i;
	struct Object* object;

	for(i = 0; i < m->marioObj->numCollidedObjs; i++)
	{
		object = m->marioObj->collidedObjs[i];

		if(object->oInteractType == interactType)
		{
			return object;
		}
	}

	return NULL;
}

u32 mario_check_object_grab(struct PlayerRecord* m)
{
	u32 result = FALSE;
	void* script;

	if(m->input & INPUT_INTERACT_OBJ_GRABBABLE)
	{
		script = virtual_to_segmented(0x13, m->interactObj->behavior);

		if(script == sm64::bhv::bhvBowser())
		{
			s16 facingDYaw = m->faceAngle[1] - m->interactObj->oMoveAngleYaw;
			if(facingDYaw >= -0x5555 && facingDYaw <= 0x5555)
			{
				m->faceAngle[1] = m->interactObj->oMoveAngleYaw;
				m->usedObj	= m->interactObj;
				result		= m->ChangePlayerStatus(ACT_PICKING_UP_BOWSER, 0);
			}
		}
		else
		{
			s16 facingDYaw = mario_angle_to_object(m, m->interactObj) - m->faceAngle[1];
			if(facingDYaw >= -0x2AAA && facingDYaw <= 0x2AAA)
			{
				m->usedObj = m->interactObj;

				if(!(m->status & ACT_FLAG_AIR))
				{
					m->ChangePlayerStatus((m->status & ACT_FLAG_DIVING) ? ACT_DIVE_PICKING_UP : ACT_PICKING_UP, 0);
				}

				result = TRUE;
			}
		}
	}

	return result;
}

u32 bully_knock_back_mario(struct PlayerRecord* mario)
{
	struct BullyCollisionData marioData;
	struct BullyCollisionData bullyData;
	s16 newMarioYaw;
	s16 newBullyYaw;
	s16 marioDYaw;
	UNUSED s16 bullyDYaw;

	u32 bonkAction = 0;

	struct Object* bully = mario->interactObj;

	//! Conversion ratios multiply to more than 1 (could allow unbounded speed
	// with bonk cancel - but this isn't important for regular bully battery)
	f32 bullyToMarioRatio = bully->hitboxRadius * 3 / 53;
	f32 marioToBullyRatio = 53.0f / bully->hitboxRadius;

	init_bully_collision_data(&marioData, mario->pos[0], mario->pos[2], mario->forwardVel, mario->faceAngle[1], bullyToMarioRatio, 52.0f);

	init_bully_collision_data(&bullyData, bully->oPosX, bully->oPosZ, bully->oForwardVel, bully->oMoveAngleYaw, marioToBullyRatio, bully->hitboxRadius + 2.0f);

	if(mario->forwardVel != 0.0f)
	{
		transfer_bully_speed(&marioData, &bullyData);
	}
	else
	{
		transfer_bully_speed(&bullyData, &marioData);
	}

	newMarioYaw = atan2s(marioData.velZ, marioData.velX);
	newBullyYaw = atan2s(bullyData.velZ, bullyData.velX);

	marioDYaw = newMarioYaw - mario->faceAngle[1];
	bullyDYaw = newBullyYaw - bully->oMoveAngleYaw;

	mario->faceAngle[1] = newMarioYaw;
	mario->forwardVel   = sqrtf(marioData.velX * marioData.velX + marioData.velZ * marioData.velZ);
	mario->pos[0]	    = marioData.posX;
	mario->pos[2]	    = marioData.posZ;

	bully->oMoveAngleYaw = newBullyYaw;
	bully->oForwardVel   = sqrtf(bullyData.velX * bullyData.velX + bullyData.velZ * bullyData.velZ);
	bully->oPosX	     = bullyData.posX;
	bully->oPosZ	     = bullyData.posZ;

	if(marioDYaw < -0x4000 || marioDYaw > 0x4000)
	{
		mario->faceAngle[1] += 0x8000;
		mario->forwardVel *= -1.0f;

		if(mario->status & ACT_FLAG_AIR)
		{
			bonkAction = ACT_BACKWARD_AIR_KB;
		}
		else
		{
			bonkAction = ACT_SOFT_BACKWARD_GROUND_KB;
		}
	}
	else
	{
		if(mario->status & ACT_FLAG_AIR)
		{
			bonkAction = ACT_FORWARD_AIR_KB;
		}
		else
		{
			bonkAction = ACT_SOFT_FORWARD_GROUND_KB;
		}
	}

	return bonkAction;
}

void bounce_off_object(struct PlayerRecord* m, struct Object* o, f32 velY)
{
	m->pos[1] = o->oPosY + o->hitboxHeight;
	m->vel[1] = velY;

	m->flags &= ~MARIO_UNKNOWN_08;

	AudStartSound(SOUND_ACTION_BOUNCE_OFF_OBJECT, m->marioObj->header.gfx.cameraToObject);
}

void hit_object_from_below(struct PlayerRecord* m, UNUSED struct Object* o)
{
	m->vel[1] = 0.0f;
	set_camera_shake_from_hit(SHAKE_HIT_FROM_BELOW);
}

static u32 unused_determine_knockback_action(struct PlayerRecord* m)
{
	u32 bonkAction;
	s16 angleToObject = mario_angle_to_object(m, m->interactObj);
	s16 facingDYaw	  = angleToObject - m->faceAngle[1];

	if(m->forwardVel < 16.0f)
	{
		m->forwardVel = 16.0f;
	}

	m->faceAngle[1] = angleToObject;

	if(facingDYaw >= -0x4000 && facingDYaw <= 0x4000)
	{
		m->forwardVel *= -1.0f;
		if(m->status & (ACT_FLAG_AIR | ACT_FLAG_ON_POLE | ACT_FLAG_HANGING))
		{
			bonkAction = ACT_BACKWARD_AIR_KB;
		}
		else
		{
			bonkAction = ACT_SOFT_BACKWARD_GROUND_KB;
		}
	}
	else
	{
		m->faceAngle[1] += 0x8000;
		if(m->status & (ACT_FLAG_AIR | ACT_FLAG_ON_POLE | ACT_FLAG_HANGING))
		{
			bonkAction = ACT_FORWARD_AIR_KB;
		}
		else
		{
			bonkAction = ACT_SOFT_FORWARD_GROUND_KB;
		}
	}

	return bonkAction;
}

u32 determine_knockback_action(struct PlayerRecord* m, UNUSED s32 arg)
{
	u32 bonkAction;

	s16 terrainIndex  = 0; // 1 = air, 2 = water, 0 = default
	s16 strengthIndex = 0;

	s16 angleToObject   = mario_angle_to_object(m, m->interactObj);
	s16 facingDYaw	    = angleToObject - m->faceAngle[1];
	s16 remainingHealth = m->health - 0x40 * m->hurtCounter;

	if(m->status & (ACT_FLAG_SWIMMING | ACT_FLAG_METAL_WATER))
	{
		terrainIndex = 2;
	}
	else if(m->status & (ACT_FLAG_AIR | ACT_FLAG_ON_POLE | ACT_FLAG_HANGING))
	{
		terrainIndex = 1;
	}

	if(remainingHealth < 0x100)
	{
		strengthIndex = 2;
	}
	else if(m->interactObj->oDamageOrCoinValue >= 4)
	{
		strengthIndex = 2;
	}
	else if(m->interactObj->oDamageOrCoinValue >= 2)
	{
		strengthIndex = 1;
	}

	m->faceAngle[1] = angleToObject;

	if(terrainIndex == 2)
	{
		if(m->forwardVel < 28.0f)
		{
			m->mario_set_forward_vel(28.0f);
		}

		if(m->pos[1] >= m->interactObj->oPosY)
		{
			if(m->vel[1] < 20.0f)
			{
				m->vel[1] = 20.0f;
			}
		}
		else
		{
			if(m->vel[1] > 0.0f)
			{
				m->vel[1] = 0.0f;
			}
		}
	}
	else
	{
		if(m->forwardVel < 16.0f)
		{
			m->mario_set_forward_vel(16.0f);
		}
	}

	if(-0x4000 <= facingDYaw && facingDYaw <= 0x4000)
	{
		m->forwardVel *= -1.0f;
		bonkAction = sBackwardKnockbackActions[terrainIndex][strengthIndex];
	}
	else
	{
		m->faceAngle[1] += 0x8000;
		bonkAction = sForwardKnockbackActions[terrainIndex][strengthIndex];
	}

	return bonkAction;
}

void push_mario_out_of_object(struct PlayerRecord* m, struct Object* o, f32 padding)
{
	f32 minDistance = o->hitboxRadius + m->marioObj->hitboxRadius + padding;

	f32 offsetX  = m->pos[0] - o->oPosX;
	f32 offsetZ  = m->pos[2] - o->oPosZ;
	f32 distance = sqrtf(offsetX * offsetX + offsetZ * offsetZ);

	if(distance < minDistance)
	{
		struct Surface* floor;
		s16 pushAngle;
		f32 newMarioX;
		f32 newMarioZ;

		if(distance == 0.0f)
		{
			pushAngle = m->faceAngle[1];
		}
		else
		{
			pushAngle = atan2s(offsetZ, offsetX);
		}

		newMarioX = o->oPosX + minDistance * sins(pushAngle);
		newMarioZ = o->oPosZ + minDistance * coss(pushAngle);

		f32_find_wall_collision(&newMarioX, &m->pos[1], &newMarioZ, 60.0f, 50.0f);

		mcBGGroundCheck(newMarioX, m->pos[1], newMarioZ, &floor);
		if(floor != NULL)
		{
			//! Doesn't update mario's referenced floor (allows oob death when
			// an object pushes you into a steep slope while in a ground action)
			m->pos[0] = newMarioX;
			m->pos[2] = newMarioZ;
		}
	}
}

void bounce_back_from_attack(struct PlayerRecord* m, u32 interaction)
{
	if(interaction & (INT_PUNCH | INT_KICK | INT_TRIP))
	{
		if(m->status == ACT_PUNCHING)
		{
			m->status = ACT_MOVE_PUNCHING;
		}

		if(m->status & ACT_FLAG_AIR)
		{
			m->mario_set_forward_vel(-16.0f);
		}
		else
		{
			m->mario_set_forward_vel(-48.0f);
		}

		set_camera_shake_from_hit(SHAKE_ATTACK);
		m->particleFlags |= 0x00040000;
	}

	if(interaction & (INT_PUNCH | INT_KICK | INT_TRIP | INT_FAST_ATTACK_OR_SHELL))
	{
		AudStartSound(SOUND_ACTION_HIT_2, m->marioObj->header.gfx.cameraToObject);
	}
}

u32 func_8024D664(struct PlayerRecord* m, struct Object* o)
{
	f32 dx = o->oPosX - m->pos[0];
	f32 dz = o->oPosZ - m->pos[2];

	s16 dYaw = o->oMoveAngleYaw - atan2s(dz, dx);

	return (dYaw >= -0x4000 && dYaw <= 0x4000) ? 0x00000001 : 0x00000002;
}

u32 take_damage_from_interact_object(struct PlayerRecord* m)
{
	s32 shake;
	s32 damage = m->interactObj->oDamageOrCoinValue;

	if(damage >= 4)
	{
		shake = SHAKE_LARGE_DAMAGE;
	}
	else if(damage >= 2)
	{
		shake = SHAKE_MED_DAMAGE;
	}
	else
	{
		shake = SHAKE_SMALL_DAMAGE;
	}

	if(!(m->flags & MARIO_CAP_ON_HEAD))
	{
		damage += (damage + 1) / 2;
	}

	if(m->flags & MARIO_METAL_CAP)
	{
		damage = 0;
	}

	m->hurtCounter += damage * 4;

	set_camera_shake_from_hit(shake);
	SendMotorEvent(5, 80);
	return damage;
}

u32 take_damage_and_knock_back(struct PlayerRecord* m, struct Object* o)
{
	u32 damage;

	if(!sInvulnerable && !(m->flags & MARIO_VANISH_CAP) && !(o->oInteractionSubtype & INT_SUBTYPE_DELAY_INVINCIBILITY))
	{
		o->oInteractStatus = INT_STATUS_INTERACTED | INT_STATUS_ATTACKED_MARIO;
		m->interactObj	   = o;

		damage = take_damage_from_interact_object(m);

		if(o->oInteractionSubtype & INT_SUBTYPE_BIG_KNOCKBACK)
		{
			m->forwardVel = 40.0f;
		}

		if(o->oDamageOrCoinValue > 0)
		{
			AudStartSound(SOUND_MARIO_ATTACKED, m->marioObj->header.gfx.cameraToObject);
		}

		m->update_mario_sound_and_camera();
		return m->drop_and_set_mario_action(determine_knockback_action(m, o->oDamageOrCoinValue), damage);
	}

	return FALSE;
}

void reset_mario_pitch(struct PlayerRecord* m)
{
	if(m->status == ACT_WATER_JUMP || m->status == ACT_SHOT_FROM_CANNON || m->status == ACT_FLYING)
	{
		set_camera_mode(m->area->camera, m->area->camera->defMode, 1);
		m->faceAngle[0] = 0;
	}
}

u32 interact_coin(struct PlayerRecord* m, u32 interactType, struct Object* o)
{
	m->numCoins += o->oDamageOrCoinValue;
	m->healCounter += 4 * o->oDamageOrCoinValue;

	o->oInteractStatus = INT_STATUS_INTERACTED;

	if(COURSE_IS_MAIN_COURSE(activeCourseNo) && m->numCoins - o->oDamageOrCoinValue < 100 && m->numCoins >= 100)
	{
		bhv_spawn_star_no_level_exit(6);
	}

	if(o->oDamageOrCoinValue >= 2)
	{
		SendMotorEvent(5, 80);
	}

	return FALSE;
}

u32 interact_water_ring(struct PlayerRecord* m, u32 interactType, struct Object* o)
{
	m->healCounter += 4 * o->oDamageOrCoinValue;
	o->oInteractStatus = INT_STATUS_INTERACTED;
	return FALSE;
}

u32 interact_star_or_key(struct PlayerRecord* m, u32 interactType, struct Object* o)
{
	u32 starIndex;
	u32 starGrabAction = ACT_STAR_DANCE_EXIT;
	u32 noExit	   = ((o->oInteractionSubtype & INT_SUBTYPE_NO_EXIT) != 0) || sm64::config().game().noStarExit();
	u32 grandStar	   = (o->oInteractionSubtype & INT_SUBTYPE_GRAND_STAR) != 0;

	if(m->health >= 0x100)
	{
		mario_stop_riding_and_holding(m);
		SendMotorEvent(5, 80);

		if(!noExit)
		{
			m->hurtCounter = 0;
			m->healCounter = 0;
			if(m->capTimer > 1)
			{
				m->capTimer = 1;
			}
		}

		if(noExit)
		{
			starGrabAction = ACT_STAR_DANCE_NO_EXIT;
		}

		if(m->status & ACT_FLAG_SWIMMING)
		{
			starGrabAction = ACT_STAR_DANCE_WATER;
		}

		if(m->status & ACT_FLAG_METAL_WATER)
		{
			starGrabAction = ACT_STAR_DANCE_WATER;
		}

		if(m->status & ACT_FLAG_AIR)
		{
			starGrabAction = ACT_FALL_AFTER_STAR_GRAB;
		}

		s_makeobj_nowpos(o, MODEL_NONE, sm64::bhv::bhvStarKeyCollectionPuffSpawner());

		o->oInteractStatus = INT_STATUS_INTERACTED;
		m->interactObj	   = o;
		m->usedObj	   = o;

		starIndex = (o->oBehParams >> 24) & 0x1F;
		BuStoreBackUp(m->numCoins, starIndex);

		m->numStars = BuGetSumStars(activePlayerNo - 1, COURSE_MIN - 1, COURSE_MAX - 1);

		if(!noExit)
		{
			drop_queued_background_music();
			func_8024924C(126);
		}

		AudStartSound(SOUND_MENU_STAR_SOUND, m->marioObj->header.gfx.cameraToObject);

		m->update_mario_sound_and_camera();

		if(grandStar)
		{
			return m->ChangePlayerStatus(ACT_JUMBO_STAR_CUTSCENE, 0);
		}

		if (!sm64::config().game().noStarExit())
		{
			return m->ChangePlayerStatus(starGrabAction, noExit + 2 * grandStar);
		}
		
		BuWriteStorage(activePlayerNo - 1);
	}

	return FALSE;
}

u32 interact_bbh_entrance(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	if(m->status != ACT_BBH_ENTER_SPIN && m->status != ACT_BBH_ENTER_JUMP)
	{
		mario_stop_riding_and_holding(m);

		o->oInteractStatus = INT_STATUS_INTERACTED;
		m->interactObj	   = o;
		m->usedObj	   = o;

		if(m->status & ACT_FLAG_AIR)
		{
			return m->ChangePlayerStatus(ACT_BBH_ENTER_SPIN, 0);
		}

		return m->ChangePlayerStatus(ACT_BBH_ENTER_JUMP, 0);
	}

	return FALSE;
}

u32 interact_warp(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	u32 action;

	if(o->oInteractionSubtype & INT_SUBTYPE_FADING_WARP)
	{
		action = m->status;

		if(action == ACT_TELEPORT_FADE_IN)
		{
			sJustTeleported = TRUE;
		}
		else if(!sJustTeleported)
		{
			if(action == ACT_IDLE || action == ACT_PANTING || action == ACT_STANDING_AGAINST_WALL || action == ACT_CROUCHING)
			{
				m->interactObj = o;
				m->usedObj     = o;

				sJustTeleported = TRUE;
				return m->ChangePlayerStatus(ACT_TELEPORT_FADE_OUT, 0);
			}
		}
	}
	else
	{
		if(m->status != ACT_EMERGE_FROM_PIPE)
		{
			o->oInteractStatus = INT_STATUS_INTERACTED;
			m->interactObj	   = o;
			m->usedObj	   = o;

			if(o->collisionData == segmented_to_virtual(warp_pipe_seg3_collision_03009AC8))
			{
				AudStartSound(SOUND_MENU_ENTER_PIPE, m->marioObj->header.gfx.cameraToObject);
				SendMotorEvent(15, 80);
			}
			else
			{
				AudStartSound(SOUND_MENU_ENTER_HOLE, m->marioObj->header.gfx.cameraToObject);
				SendMotorEvent(12, 80);
			}

			mario_stop_riding_object(m);
			return m->ChangePlayerStatus(ACT_DISAPPEARED, (PL_FADE_TRIPTUBE << 16) + 2);
		}
	}

	return FALSE;
}

u32 interact_warp_door(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	u32 doorAction = 0;
	u32 saveFlags  = BuGetItemFlag();
	s16 warpDoorId = o->oBehParams >> 24;
	u32 actionArg;

	if(m->status == ACT_WALKING || m->status == ACT_DECELERATING)
	{
		if(warpDoorId == 1 && !(saveFlags & SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR))
		{
			if(!(saveFlags & SAVE_FLAG_HAVE_KEY_2))
			{
				if(!sDisplayingDoorText)
				{
					m->ChangePlayerStatus(ACT_READING_AUTOMATIC_DIALOG, (saveFlags & SAVE_FLAG_HAVE_KEY_1) ? DIALOG_023 : DIALOG_022);
				}
				sDisplayingDoorText = TRUE;

				return FALSE;
			}

			doorAction = ACT_UNLOCKING_KEY_DOOR;
		}

		if(warpDoorId == 2 && !(saveFlags & SAVE_FLAG_UNLOCKED_BASEMENT_DOOR))
		{
			if(!(saveFlags & SAVE_FLAG_HAVE_KEY_1))
			{
				if(!sDisplayingDoorText)
				{
					// Moat door skip was intended confirmed
					m->ChangePlayerStatus(ACT_READING_AUTOMATIC_DIALOG, (saveFlags & SAVE_FLAG_HAVE_KEY_2) ? DIALOG_023 : DIALOG_022);
				}
				sDisplayingDoorText = TRUE;

				return FALSE;
			}

			doorAction = ACT_UNLOCKING_KEY_DOOR;
		}

		if(m->status == ACT_WALKING || m->status == ACT_DECELERATING)
		{
			actionArg = func_8024D664(m, o) + 0x00000004;

			if(doorAction == 0)
			{
				if(actionArg & 0x00000001)
				{
					doorAction = ACT_PULLING_DOOR;
				}
				else
				{
					doorAction = ACT_PUSHING_DOOR;
				}
			}

			m->interactObj = o;
			m->usedObj     = o;
			return m->ChangePlayerStatus(doorAction, actionArg);
		}
	}

	return FALSE;
}

u32 get_door_save_file_flag(struct Object* door)
{
	u32 saveFileFlag     = 0;
	s16 requiredNumStars = door->oBehParams >> 24;

	s16 isCcmDoor = door->oPosX < 0.0f;
	s16 isPssDoor = door->oPosY > 500.0f;

	switch(requiredNumStars)
	{
		case 1:
			if(isPssDoor)
			{
				saveFileFlag = SAVE_FLAG_UNLOCKED_PSS_DOOR;
			}
			else
			{
				saveFileFlag = SAVE_FLAG_UNLOCKED_WF_DOOR;
			}
			break;

		case 3:
			if(isCcmDoor)
			{
				saveFileFlag = SAVE_FLAG_UNLOCKED_CCM_DOOR;
			}
			else
			{
				saveFileFlag = SAVE_FLAG_UNLOCKED_JRB_DOOR;
			}
			break;

		case 8:
			saveFileFlag = SAVE_FLAG_UNLOCKED_BITDW_DOOR;
			break;

		case 30:
			saveFileFlag = SAVE_FLAG_UNLOCKED_BITFS_DOOR;
			break;

		case 50:
			saveFileFlag = SAVE_FLAG_UNLOCKED_50_STAR_DOOR;
			break;
	}

	return saveFileFlag;
}

u32 interact_door(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	s16 requiredNumStars = o->oBehParams >> 24;
	s16 numStars	     = BuGetSumStars(activePlayerNo - 1, COURSE_MIN - 1, COURSE_MAX - 1);

	if(m->status == ACT_WALKING || m->status == ACT_DECELERATING)
	{
		if(numStars >= requiredNumStars)
		{
			u32 actionArg = func_8024D664(m, o);
			u32 enterDoorAction;
			u32 doorSaveFileFlag;

			if(actionArg & 0x00000001)
			{
				enterDoorAction = ACT_PULLING_DOOR;
			}
			else
			{
				enterDoorAction = ACT_PUSHING_DOOR;
			}

			doorSaveFileFlag = get_door_save_file_flag(o);
			m->interactObj	 = o;
			m->usedObj	 = o;

			if(o->oInteractionSubtype & INT_SUBTYPE_STAR_DOOR)
			{
				enterDoorAction = ACT_ENTERING_STAR_DOOR;
			}

			if(doorSaveFileFlag != 0 && !(BuGetItemFlag() & doorSaveFileFlag))
			{
				enterDoorAction = ACT_UNLOCKING_STAR_DOOR;
			}

			return m->ChangePlayerStatus(enterDoorAction, actionArg);
		}
		else if(!sDisplayingDoorText)
		{
			u32 text = DIALOG_022 << 16;

			switch(requiredNumStars)
			{
				case 1:
					text = DIALOG_024 << 16;
					break;
				case 3:
					text = DIALOG_025 << 16;
					break;
				case 8:
					text = DIALOG_026 << 16;
					break;
				case 30:
					text = DIALOG_027 << 16;
					break;
				case 50:
					text = DIALOG_028 << 16;
					break;
				case 70:
					text = DIALOG_029 << 16;
					break;
			}

			text += requiredNumStars - numStars;

			sDisplayingDoorText = TRUE;
			return m->ChangePlayerStatus(ACT_READING_AUTOMATIC_DIALOG, text);
		}
	}
	else if(m->status == ACT_IDLE && sDisplayingDoorText == TRUE && requiredNumStars == 70)
	{
		m->interactObj = o;
		m->usedObj     = o;
		return m->ChangePlayerStatus(ACT_ENTERING_STAR_DOOR, func_8024D664(m, o));
	}

	return FALSE;
}

u32 interact_cannon_base(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	if(m->status != ACT_IN_CANNON)
	{
		mario_stop_riding_and_holding(m);
		o->oInteractStatus = INT_STATUS_INTERACTED;
		m->interactObj	   = o;
		m->usedObj	   = o;
		return m->ChangePlayerStatus(ACT_IN_CANNON, 0);
	}

	return FALSE;
}

u32 interact_igloo_barrier(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	//! Sets used object without changing action (LOTS of interesting glitches,
	// but unfortunately the igloo barrier is the only object with this interaction
	// type)
	m->interactObj = o;
	m->usedObj     = o;
	push_mario_out_of_object(m, o, 5.0f);
	return FALSE;
}

u32 interact_tornado(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	struct Object* marioObj = m->marioObj;

	if(m->status != ACT_TORNADO_TWIRLING && m->status != ACT_SQUISHED)
	{
		mario_stop_riding_and_holding(m);
		m->mario_set_forward_vel(0.0f);
		m->update_mario_sound_and_camera();

		o->oInteractStatus = INT_STATUS_INTERACTED;
		m->interactObj	   = o;
		m->usedObj	   = o;

		marioObj->oMarioTornadoYawVel = 0x400;
		marioObj->oMarioTornadoPosY   = m->pos[1] - o->oPosY;

		AudStartSound(SOUND_MARIO_WAAAOOOW, m->marioObj->header.gfx.cameraToObject);
		SendMotorEvent(30, 60);

		return m->ChangePlayerStatus(ACT_TORNADO_TWIRLING, m->status == ACT_TWIRLING);
	}

	return FALSE;
}

u32 interact_whirlpool(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	struct Object* marioObj = m->marioObj;

	if(m->status != ACT_CAUGHT_IN_WHIRLPOOL)
	{
		mario_stop_riding_and_holding(m);
		o->oInteractStatus = INT_STATUS_INTERACTED;
		m->interactObj	   = o;
		m->usedObj	   = o;

		m->forwardVel = 0.0f;

		marioObj->oMarioWhirlpoolPosY = m->pos[1] - o->oPosY;

		AudStartSound(SOUND_MARIO_WAAAOOOW, m->marioObj->header.gfx.cameraToObject);
		SendMotorEvent(30, 60);

		return m->ChangePlayerStatus(ACT_CAUGHT_IN_WHIRLPOOL, 0);
	}

	return FALSE;
}

u32 interact_wind(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	UNUSED struct Object* marioObj = m->marioObj;

	if(m->status != ACT_GETTING_BLOWN)
	{
		mario_stop_riding_and_holding(m);
		o->oInteractStatus = INT_STATUS_INTERACTED;
		m->interactObj	   = o;
		m->usedObj	   = o;

		m->faceAngle[1] = o->oMoveAngleYaw + 0x8000;
		m->gravity	= 0.4f;
		m->forwardVel	= -24.0f;
		m->vel[1]	= 12.0f;

		AudStartSound(SOUND_MARIO_WAAAOOOW, m->marioObj->header.gfx.cameraToObject);
		m->update_mario_sound_and_camera();
		return m->ChangePlayerStatus(ACT_GETTING_BLOWN, 0);
	}

	return FALSE;
}

u32 interact_flame(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	u32 burningAction = ACT_BURNING_JUMP;

	if(!sInvulnerable && !(m->flags & MARIO_METAL_CAP) && !(m->flags & MARIO_VANISH_CAP) && !(o->oInteractionSubtype & INT_SUBTYPE_DELAY_INVINCIBILITY))
	{
		o->oInteractStatus = INT_STATUS_INTERACTED;
		m->interactObj	   = o;

		SendMotorEvent(5, 80);

		if((m->status & (ACT_FLAG_SWIMMING | ACT_FLAG_METAL_WATER)) || m->waterLevel - m->pos[1] > 50.0f)
		{
			AudStartSound(SOUND_GENERAL_FLAME_OUT, m->marioObj->header.gfx.cameraToObject);
		}
		else
		{
			m->marioObj->oMarioBurnTimer = 0;
			m->update_mario_sound_and_camera();
			AudStartSound(SOUND_MARIO_ON_FIRE, m->marioObj->header.gfx.cameraToObject);

			if((m->status & ACT_FLAG_AIR) && m->vel[1] <= 0.0f)
			{
				burningAction = ACT_BURNING_FALL;
			}

			return m->drop_and_set_mario_action(burningAction, 1);
		}
	}

	return FALSE;
}

u32 interact_snufit_bullet(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	if(!sInvulnerable && !(m->flags & MARIO_VANISH_CAP))
	{
		if(m->flags & MARIO_METAL_CAP)
		{
			o->oInteractStatus = INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED;
			AudStartSound(SOUND_ACTION_UNKNOWN458, m->marioObj->header.gfx.cameraToObject);
		}
		else
		{
			o->oInteractStatus = INT_STATUS_INTERACTED | INT_STATUS_ATTACKED_MARIO;
			m->interactObj	   = o;
			take_damage_from_interact_object(m);

			AudStartSound(SOUND_MARIO_ATTACKED, m->marioObj->header.gfx.cameraToObject);
			m->update_mario_sound_and_camera();

			return m->drop_and_set_mario_action(determine_knockback_action(m, o->oDamageOrCoinValue), o->oDamageOrCoinValue);
		}
	}

	if(!(o->oInteractionSubtype & INT_SUBTYPE_DELAY_INVINCIBILITY))
	{
		sDelayInvincTimer = TRUE;
	}

	return FALSE;
}

u32 interact_clam_or_bubba(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	if(o->oInteractionSubtype & INT_SUBTYPE_EATS_MARIO)
	{
		o->oInteractStatus = INT_STATUS_INTERACTED;
		m->interactObj	   = o;
		return m->ChangePlayerStatus(ACT_EATEN_BY_BUBBA, 0);
	}
	else if(take_damage_and_knock_back(m, o))
	{
		return TRUE;
	}

	if(!(o->oInteractionSubtype & INT_SUBTYPE_DELAY_INVINCIBILITY))
	{
		sDelayInvincTimer = TRUE;
	}
	return TRUE;
}

u32 interact_bully(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	UNUSED u32 unused;

	u32 interaction;
	if(m->flags & MARIO_METAL_CAP)
	{
		interaction = INT_FAST_ATTACK_OR_SHELL;
	}
	else
	{
		interaction = determine_interaction(m, o);
	}

	m->interactObj = o;

	if(interaction & INT_ATTACK_NOT_FROM_BELOW)
	{
		push_mario_out_of_object(m, o, 5.0f);
		SendMotorEvent(5, 80);

		m->forwardVel	 = -16.0f;
		o->oMoveAngleYaw = m->faceAngle[1];
		o->oForwardVel	 = 3392.0f / o->hitboxRadius;

		attack_object(o, interaction);
		bounce_back_from_attack(m, interaction);
		return TRUE;
	}
	else if(!sInvulnerable && !(m->flags & MARIO_VANISH_CAP) && !(o->oInteractionSubtype & INT_SUBTYPE_DELAY_INVINCIBILITY))
	{
		o->oInteractStatus = INT_STATUS_INTERACTED;
		m->invincTimer	   = 2;

		m->update_mario_sound_and_camera();
		AudStartSound(SOUND_MARIO_EEUH, m->marioObj->header.gfx.cameraToObject);
		AudStartSound(SOUND_OBJ_BULLY_METAL, m->marioObj->header.gfx.cameraToObject);

		push_mario_out_of_object(m, o, 5.0f);
		m->drop_and_set_mario_action(bully_knock_back_mario(m), 0);
		SendMotorEvent(5, 80);
		return TRUE;
	}

	return FALSE;
}

u32 interact_shock(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	if(!sInvulnerable && !(m->flags & MARIO_VANISH_CAP) && !(o->oInteractionSubtype & INT_SUBTYPE_DELAY_INVINCIBILITY))
	{
		u32 actionArg = (m->status & (ACT_FLAG_AIR | ACT_FLAG_ON_POLE | ACT_FLAG_HANGING)) == 0;

		o->oInteractStatus = INT_STATUS_INTERACTED | INT_STATUS_ATTACKED_MARIO;
		m->interactObj	   = o;

		take_damage_from_interact_object(m);
		AudStartSound(SOUND_MARIO_ATTACKED, m->marioObj->header.gfx.cameraToObject);
		SendMotorEvent(70, 60);

		if(m->status & (ACT_FLAG_SWIMMING | ACT_FLAG_METAL_WATER))
		{
			return m->drop_and_set_mario_action(ACT_WATER_SHOCKED, 0);
		}
		else
		{
			m->update_mario_sound_and_camera();
			return m->drop_and_set_mario_action(ACT_SHOCKED, actionArg);
		}
	}

	if(!(o->oInteractionSubtype & INT_SUBTYPE_DELAY_INVINCIBILITY))
	{
		sDelayInvincTimer = TRUE;
	}
	return FALSE;
}

static u32 func_8024EF8C(UNUSED struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	if(!(o->oInteractionSubtype & INT_SUBTYPE_DELAY_INVINCIBILITY))
	{
		sDelayInvincTimer = TRUE;
	}
	return FALSE;
}

u32 interact_mr_blizzard(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	if(take_damage_and_knock_back(m, o))
	{
		return TRUE;
	}

	if(!(o->oInteractionSubtype & INT_SUBTYPE_DELAY_INVINCIBILITY))
	{
		sDelayInvincTimer = TRUE;
	}

	return FALSE;
}

u32 interact_hit_from_below(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	UNUSED u32 unused;

	u32 interaction;
	if(m->flags & MARIO_METAL_CAP)
	{
		interaction = INT_FAST_ATTACK_OR_SHELL;
	}
	else
	{
		interaction = determine_interaction(m, o);
	}

	if(interaction & INT_ANY_ATTACK)
	{
		attack_object(o, interaction);
		bounce_back_from_attack(m, interaction);
		SendMotorEvent(5, 80);

		if(interaction & INT_HIT_FROM_BELOW)
		{
			hit_object_from_below(m, o);
		}

		if(interaction & INT_HIT_FROM_ABOVE)
		{
			if(o->oInteractionSubtype & INT_SUBTYPE_TWIRL_BOUNCE)
			{
				bounce_off_object(m, o, 80.0f);
				reset_mario_pitch(m);
				AudStartSound(SOUND_MARIO_TWIRL_BOUNCE, m->marioObj->header.gfx.cameraToObject);
				return m->drop_and_set_mario_action(ACT_TWIRLING, 0);
			}
			else
			{
				bounce_off_object(m, o, 30.0f);
			}
		}
	}
	else if(take_damage_and_knock_back(m, o))
	{
		return TRUE;
	}

	if(!(o->oInteractionSubtype & INT_SUBTYPE_DELAY_INVINCIBILITY))
	{
		sDelayInvincTimer = TRUE;
	}
	return FALSE;
}

u32 interact_bounce_top(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	u32 interaction;
	if(m->flags & MARIO_METAL_CAP)
	{
		interaction = INT_FAST_ATTACK_OR_SHELL;
	}
	else
	{
		interaction = determine_interaction(m, o);
	}

	if(interaction & INT_ATTACK_NOT_FROM_BELOW)
	{
		attack_object(o, interaction);
		bounce_back_from_attack(m, interaction);
		SendMotorEvent(5, 80);

		if(interaction & INT_HIT_FROM_ABOVE)
		{
			if(o->oInteractionSubtype & INT_SUBTYPE_TWIRL_BOUNCE)
			{
				bounce_off_object(m, o, 80.0f);
				reset_mario_pitch(m);
				AudStartSound(SOUND_MARIO_TWIRL_BOUNCE, m->marioObj->header.gfx.cameraToObject);
				return m->drop_and_set_mario_action(ACT_TWIRLING, 0);
			}
			else
			{
				bounce_off_object(m, o, 30.0f);
			}
		}
	}
	else if(take_damage_and_knock_back(m, o))
	{
		return TRUE;
	}

	if(!(o->oInteractionSubtype & INT_SUBTYPE_DELAY_INVINCIBILITY))
	{
		sDelayInvincTimer = TRUE;
	}
	return FALSE;
}

u32 interact_unknown_08(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	u32 interaction = determine_interaction(m, o);

	if(interaction & INT_PUNCH)
	{
		o->oInteractStatus = INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED | ATTACK_PUNCH;
		bounce_back_from_attack(m, interaction);
	}
	else if(take_damage_and_knock_back(m, o))
	{
		return TRUE;
	}

	if(!(o->oInteractionSubtype & INT_SUBTYPE_DELAY_INVINCIBILITY))
	{
		sDelayInvincTimer = TRUE;
	}
	return FALSE;
}

u32 interact_damage(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	if(take_damage_and_knock_back(m, o))
	{
		return TRUE;
	}

	if(!(o->oInteractionSubtype & INT_SUBTYPE_DELAY_INVINCIBILITY))
	{
		sDelayInvincTimer = TRUE;
	}
	return FALSE;
}

u32 interact_breakable(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	u32 interaction = determine_interaction(m, o);

	if(interaction & INT_ATTACK_MASK_UNKNOWN_8F)
	{
		attack_object(o, interaction);
		bounce_back_from_attack(m, interaction);

		m->interactObj = o;

		switch(interaction)
		{
			case INT_HIT_FROM_ABOVE:
				bounce_off_object(m, o, 30.0f); //! Not in the 0x8F mask
				break;

			case INT_HIT_FROM_BELOW:
				hit_object_from_below(m, o);
				break;
		}

		return TRUE;
	}

	return FALSE;
}

u32 interact_koopa_shell(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	if(!(m->status & ACT_FLAG_RIDING_SHELL))
	{
		u32 interaction = determine_interaction(m, o);

		if(interaction == INT_HIT_FROM_ABOVE || m->status == ACT_WALKING || m->status == ACT_HOLD_WALKING)
		{
			m->interactObj = o;
			m->usedObj     = o;
			m->riddenObj   = o;

			attack_object(o, interaction);
			m->update_mario_sound_and_camera();
			play_shell_music();
			mario_drop_held_object(m);

			//! Puts mario in ground action even when in air, making it easy to
			// escape air actions into crouch slide (shell cancel)
			return m->ChangePlayerStatus(ACT_RIDING_SHELL_GROUND, 0);
		}

		push_mario_out_of_object(m, o, 2.0f);
	}

	return FALSE;
}

u32 check_object_grab_mario(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	if((!(m->status & (ACT_FLAG_AIR | ACT_FLAG_INVULNERABLE | ACT_FLAG_ATTACKING)) || !sInvulnerable) && (o->oInteractionSubtype & INT_SUBTYPE_GRABS_MARIO))
	{
		if(object_facing_mario(m, o, 0x2AAA))
		{
			mario_stop_riding_and_holding(m);
			o->oInteractStatus = INT_STATUS_INTERACTED | INT_STATUS_GRABBED_MARIO;

			m->faceAngle[1] = o->oMoveAngleYaw;
			m->interactObj	= o;
			m->usedObj	= o;

			m->update_mario_sound_and_camera();
			AudStartSound(SOUND_MARIO_OOOF, m->marioObj->header.gfx.cameraToObject);
			SendMotorEvent(5, 80);
			return m->ChangePlayerStatus(ACT_GRABBED, 0);
		}
	}

	push_mario_out_of_object(m, o, -5.0f);
	return FALSE;
}

u32 interact_pole(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	s32 actionId = m->status & ACT_ID_MASK;
	if(actionId >= 0x080 && actionId < 0x0A0)
	{
		if(!(m->oldstatus & ACT_FLAG_ON_POLE) || m->usedObj != o)
		{
			u32 lowSpeed		= m->forwardVel <= 10.0f;
			struct Object* marioObj = m->marioObj;

			mario_stop_riding_and_holding(m);

			m->interactObj = o;
			m->usedObj     = o;
			m->vel[1]      = 0.0f;
			m->forwardVel  = 0.0f;

			marioObj->oMarioPoleUnk108 = 0;
			marioObj->oMarioPoleYawVel = 0;
			marioObj->oMarioPolePos	   = m->pos[1] - o->oPosY;

			if(lowSpeed)
			{
				return m->ChangePlayerStatus(ACT_GRAB_POLE_SLOW, 0);
			}

			marioObj->oMarioPoleYawVel = (s32)(m->forwardVel * 0x100 + 0x1000);
			reset_mario_pitch(m);
			SendMotorEvent(5, 80);
			return m->ChangePlayerStatus(ACT_GRAB_POLE_FAST, 0);
		}
	}

	return FALSE;
}

u32 interact_hoot(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	s32 actionId = m->status & ACT_ID_MASK;

	//! Can pause to advance the global timer without falling too far, allowing
	// you to regrab after letting go.
	if(actionId >= 0x080 && actionId < 0x098 && (frameCounter - m->usedObj->oHootMarioReleaseTime > 30 * FRAME_RATE_SCALER_INV))
	{
		mario_stop_riding_and_holding(m);
		o->oInteractStatus = INT_STATUS_HOOT_GRABBED_BY_MARIO;
		m->interactObj	   = o;
		m->usedObj	   = o;

		m->update_mario_sound_and_camera();
		SendMotorEvent(5, 80);
		return m->ChangePlayerStatus(ACT_RIDING_HOOT, 0);
	}

	return FALSE;
}

u32 interact_cap(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	u32 capFlag  = get_mario_cap_flag(o);
	u16 capMusic = 0;
	u16 capTime  = 0;

	if(m->status != ACT_GETTING_BLOWN && capFlag != 0)
	{
		m->interactObj	   = o;
		o->oInteractStatus = INT_STATUS_INTERACTED;

		m->flags &= ~MARIO_CAP_ON_HEAD & ~MARIO_CAP_IN_HAND;
		m->flags |= capFlag;

		switch(capFlag)
		{
			case MARIO_VANISH_CAP:
				capTime	 = 600 * FRAME_RATE_SCALER_INV;
				capMusic = 0x040E;
				break;

			case MARIO_METAL_CAP:
				capTime	 = 600 * FRAME_RATE_SCALER_INV;
				capMusic = 0x040F;
				break;

			case MARIO_WING_CAP:
				capTime	 = 1800 * FRAME_RATE_SCALER_INV;
				capMusic = 0x040E;
				break;
		}

		if(capTime > m->capTimer)
		{
			m->capTimer = capTime;
		}

		if((m->status & ACT_FLAG_IDLE) || m->status == ACT_WALKING)
		{
			m->flags |= MARIO_CAP_IN_HAND;
			m->ChangePlayerStatus(ACT_PUTTING_ON_CAP, 0);
		}
		else
		{
			m->flags |= MARIO_CAP_ON_HEAD;
		}

		AudStartSound(SOUND_MENU_STAR_SOUND, m->marioObj->header.gfx.cameraToObject);
		AudStartSound(SOUND_MARIO_HERE_WE_GO, m->marioObj->header.gfx.cameraToObject);

		if(capMusic != 0)
		{
			AudPlaySpecialMusic(capMusic);
		}

		return TRUE;
	}

	return FALSE;
}

u32 interact_grabbable(struct PlayerRecord* m, u32 interactType, struct Object* o)
{
	void* script = virtual_to_segmented(0x13, o->behavior);

	if(o->oInteractionSubtype & INT_SUBTYPE_KICKABLE)
	{
		u32 interaction = determine_interaction(m, o);
		if(interaction & (INT_KICK | INT_TRIP))
		{
			attack_object(o, interaction);
			bounce_back_from_attack(m, interaction);
			return FALSE;
		}
	}

	if((o->oInteractionSubtype & INT_SUBTYPE_GRABS_MARIO))
	{
		if(check_object_grab_mario(m, interactType, o))
		{
			return TRUE;
		}
	}

	if(able_to_grab_object(m, o))
	{
		if(!(o->oInteractionSubtype & INT_SUBTYPE_NOT_GRABBABLE))
		{
			m->interactObj = o;
			m->input |= INPUT_INTERACT_OBJ_GRABBABLE;
			return TRUE;
		}
	}

	if(script != sm64::bhv::bhvBowser())
	{
		push_mario_out_of_object(m, o, -5.0f);
	}
	return FALSE;
}

u32 func_8024FC94(struct PlayerRecord* m, u32 arg)
{
	s16 val6;

	if((m->status & ACT_FLAG_IDLE) != 0x00000000)
	{
		return TRUE;
	}

	if(m->status == ACT_WALKING)
	{
		if(arg)
		{
			return TRUE;
		}

		val6 = m->marioObj->header.gfx.unk38.animID;

		if(val6 == 0x0080 || val6 == 0x007F || val6 == 0x006C)
		{
			return TRUE;
		}
	}

	return FALSE;
}

#define READ_MASK (INPUT_B_PRESSED | INPUT_A_PRESSED)

#define SIGN_RANGE 0x4000

u32 func_8024FD2C(struct PlayerRecord* m, struct Object* o)
{
	if((m->input & READ_MASK) && func_8024FC94(m, 0) && object_facing_mario(m, o, SIGN_RANGE))
	{
		s16 facingDYaw = (s16)(o->oMoveAngleYaw + 0x8000) - m->faceAngle[1];
		if(facingDYaw >= -SIGN_RANGE && facingDYaw <= SIGN_RANGE)
		{
			f32 targetX = o->oPosX + 105.0f * sins(o->oMoveAngleYaw);
			f32 targetZ = o->oPosZ + 105.0f * coss(o->oMoveAngleYaw);

			m->marioObj->oMarioReadingSignDYaw  = facingDYaw;
			m->marioObj->oMarioReadingSignDPosX = targetX - m->pos[0];
			m->marioObj->oMarioReadingSignDPosZ = targetZ - m->pos[2];

			m->interactObj = o;
			m->usedObj     = o;
			return m->ChangePlayerStatus(ACT_READING_SIGN, 0);
		}
	}

	return FALSE;
}

u32 func_8024FEC0(struct PlayerRecord* m, struct Object* o)
{
	if((m->input & READ_MASK) && func_8024FC94(m, 1))
	{
		s16 facingDYaw = mario_angle_to_object(m, o) - m->faceAngle[1];
		if(facingDYaw >= -0x4000 && facingDYaw <= 0x4000)
		{
			o->oInteractStatus = INT_STATUS_INTERACTED;

			m->interactObj = o;
			m->usedObj     = o;

			push_mario_out_of_object(m, o, -10.0f);
			return m->ChangePlayerStatus(ACT_WAITING_FOR_DIALOG, 0);
		}
	}

	push_mario_out_of_object(m, o, -10.0f);
	return FALSE;
}

u32 interact_text(struct PlayerRecord* m, UNUSED u32 interactType, struct Object* o)
{
	u32 interact = FALSE;

	if(o->oInteractionSubtype & INT_SUBTYPE_SIGN)
	{
		interact = func_8024FD2C(m, o);
	}
	else if(o->oInteractionSubtype & INT_SUBTYPE_NPC)
	{
		interact = func_8024FEC0(m, o);
	}
	else
	{
		push_mario_out_of_object(m, o, 2.0f);
	}

	return interact;
}

void check_kick_or_punch_wall(struct PlayerRecord* m)
{
	if(m->flags & (MARIO_PUNCHING | MARIO_KICKING | MARIO_TRIPPING))
	{
		Vec3f detector;
		detector[0] = m->pos[0] + 50.0f * sins(m->faceAngle[1]);
		detector[2] = m->pos[2] + 50.0f * coss(m->faceAngle[1]);
		detector[1] = m->pos[1];

		if(resolve_and_return_wall_collisions(detector, 80.0f, 5.0f) != NULL)
		{
			if(m->status != ACT_MOVE_PUNCHING || m->forwardVel >= 0.0f)
			{
				if(m->status == ACT_PUNCHING)
				{
					m->status = ACT_MOVE_PUNCHING;
				}

				m->mario_set_forward_vel(-48.0f);
				AudStartSound(SOUND_ACTION_HIT_2, m->marioObj->header.gfx.cameraToObject);
				m->particleFlags |= 0x00040000;
			}
			else if(m->status & ACT_FLAG_AIR)
			{
				m->mario_set_forward_vel(-16.0f);
				AudStartSound(SOUND_ACTION_HIT_2, m->marioObj->header.gfx.cameraToObject);
				m->particleFlags |= 0x00040000;
			}
		}
	}
}

void mario_process_interactions(struct PlayerRecord* m)
{
	sDelayInvincTimer = FALSE;
	sInvulnerable	  = (m->status & ACT_FLAG_INVULNERABLE) || m->invincTimer != 0;

	if(!(m->status & ACT_FLAG_INTANGIBLE) && m->collidedObjInteractTypes != 0)
	{
		s32 i;
		for(i = 0; i < 31; i++)
		{
			u32 interactType = sInteractionHandlers[i].interactType;
			if(m->collidedObjInteractTypes & interactType)
			{
				struct Object* object = mario_get_collided_object(m, interactType);

				m->collidedObjInteractTypes &= ~interactType;

				if(!(object->oInteractStatus & INT_STATUS_INTERACTED))
				{
					if(sInteractionHandlers[i].handler(m, interactType, object))
					{
						break;
					}
				}
			}
		}
	}

	if(m->invincTimer > 0 && !sDelayInvincTimer)
	{
		m->invincTimer -= 1;
	}

	//! If the kick/punch flags are set and an object collision changes mario's
	// action, he will get the kick/punch wall speed anyway.
	check_kick_or_punch_wall(m);
	m->flags &= ~MARIO_PUNCHING & ~MARIO_KICKING & ~MARIO_TRIPPING;

	if(!(m->marioObj->collidedObjInteractTypes & (INTERACT_WARP_DOOR | INTERACT_DOOR)))
	{
		sDisplayingDoorText = FALSE;
	}
	if(!(m->marioObj->collidedObjInteractTypes & INTERACT_WARP))
	{
		sJustTeleported = FALSE;
	}
}

void check_death_barrier(struct PlayerRecord* m)
{
	if(m->pos[1] < m->floorHeight + 2048.0f)
	{
		if(PL_StartFadeout(m, PL_FADE_FALLING) == 20 && !(m->flags & MARIO_UNKNOWN_18))
		{
			AudStartSound(SOUND_MARIO_WAAAOOOW, m->marioObj->header.gfx.cameraToObject);
		}
	}
}

void check_lava_boost(struct PlayerRecord* m)
{
	if(!(m->status & ACT_FLAG_RIDING_SHELL) && m->pos[1] < m->floorHeight + 10.0f)
	{
		if(!(m->flags & MARIO_METAL_CAP))
		{
			m->hurtCounter += (m->flags & MARIO_CAP_ON_HEAD) ? 12 : 18;
		}

		m->update_mario_sound_and_camera();
		m->drop_and_set_mario_action(ACT_LAVA_BOOST, 0);
	}
}

void pss_begin_slide(UNUSED struct PlayerRecord* m)
{
	if(!(playerMeter.flags & HUD_DISPLAY_FLAG_TIMER))
	{
		GmStopWatch(TIMER_CONTROL_SHOW);
		GmStopWatch(TIMER_CONTROL_START);
		sPssSlideStarted = TRUE;
	}
}

void pss_end_slide(struct PlayerRecord* m)
{
	//! This flag isn't set on death or level entry, allowing double star spawn
	if(sPssSlideStarted)
	{
		u16 slideTime = GmStopWatch(TIMER_CONTROL_STOP);
		if(slideTime < 630)
		{
			m->marioObj->oBehParams = (1 << 24);
			s_enemyset_star(-6358.0f, -4300.0f, 4700.0f);
		}
		sPssSlideStarted = FALSE;
	}
}

void mario_handle_special_floors(struct PlayerRecord* m)
{
	if((m->status & ACT_GROUP_MASK) == ACT_GROUP_CUTSCENE)
	{
		return;
	}

	if(m->floor != NULL)
	{
		s32 floorType = m->floor->type;

		switch(floorType)
		{
			case SURFACE_DEATH_PLANE:
			case SURFACE_VERTICAL_WIND:
				check_death_barrier(m);
				break;

			case SURFACE_WARP:
				PL_StartFadeout(m, PL_FADE_FALLING);
				break;

			case SURFACE_TIMER_START:
				pss_begin_slide(m);
				break;

			case SURFACE_TIMER_END:
				pss_end_slide(m);
				break;
		}

		if(!(m->status & ACT_FLAG_AIR) && !(m->status & ACT_FLAG_SWIMMING))
		{
			switch(floorType)
			{
				case SURFACE_BURNING:
					check_lava_boost(m);
					break;
			}
		}
	}
}

#pragma GCC pop_options
