#include <ultra64.h>

#include "sm64.h"
#include "game/options.h"
#include "obj_behaviors.h"
#include "rendering_graph_node.h"
#include "memory.h"
#include "engine/behavior_script.h"
#include "engine/surface_collision.h"
#include "engine/math_util.h"
#include "display.h"
#include "object_helpers.h"
#include "behavior_data.h"
#include "mario.h"
#include "game.h"
#include "camera.h"
#include "mario_actions_cutscene.h"
#include "object_list_processor.h"
#include "save_file.h"
#include "area.h"
#include "mario_misc.h"
#include "level_update.h"
#include "audio/external.h"
#include "behavior_actions.h"
#include "spawn_object.h"
#include "spawn_sound.h"
#include "room.h"
#include "envfx_bubbles.h"
#include "ingame_menu.h"
#include "interaction.h"
#include "level_table.h"
#include "dialog_ids.h"
#include "course_table.h"
#include "levels/header.h"
#include "player/players.h"

/**
 * @file obj_behaviors.c
 * This file contains a portion of the obj behaviors and many helper functions for those
 * specific behaviors. Few functions besides the bhv_ functions are used elsewhere in the repo.
 */

#define o gCurrentObject

#define OBJ_COL_FLAG_GROUNDED (1 << 0)
#define OBJ_COL_FLAG_HIT_WALL (1 << 1)
#define OBJ_COL_FLAG_UNDERWATER (1 << 2)
#define OBJ_COL_FLAG_NO_Y_VEL (1 << 3)
#define OBJ_COL_FLAGS_LANDED (OBJ_COL_FLAG_GROUNDED | OBJ_COL_FLAG_NO_Y_VEL)

/**
 * Current object floor as defined in ObjMoveEvent.
 */
static struct Surface* sObjFloor;

/**
 * Set to false when an object close to the floor should not be oriented in reference
 * to it. Happens with boulder, falling pillar, and the rolling snowman body.
 */
static s8 sOrientObjWithFloor = TRUE;

/**
 * Keeps track of Mario's previous non-zero room.
 * Helps keep track of room when Mario is over an object.
 */
s16 sPrevCheckMarioRoom = 0;

/**
 * Tracks whether or not Yoshi has walked/jumped off the roof.
 */
s8 yoshi_destFlag = FALSE;

// extern void *ccm_seg7_trajectory_snowman;

/**
 * Resets yoshi as spawned/despawned upon new file select.
 * Possibly a function with stubbed code.
 */
void iwa_StratInit(void)
{
	yoshi_destFlag = FALSE;
}

/**
 * An size geo function. Bears strong similarity to Geo18_802B7D44, and relates something
 * of the opacity of an object to something else. Perhaps like, giving a parent object the same
 * opacity?
 */
Gfx UNUSED* AlphaSet(s32 callContext, struct GraphNode* node, UNUSED Mat4* mtx)
{
	Gfx* gfxHead;
	Gfx* gfx;
	struct Object* heldObject;
	struct Object* obj;
	UNUSED struct Object* unusedObject;
	UNUSED s32 pad;

	gfxHead = NULL;

	if(callContext == GEO_CONTEXT_RENDER)
	{
		heldObject   = (struct Object*)gCurGraphNodeObject;
		obj	     = (struct Object*)node;
		unusedObject = (struct Object*)node;

		if(gCurGraphNodeHeldObject != NULL)
		{
			heldObject = gCurGraphNodeHeldObject->objNode;
		}

		gfxHead			   = (Gfx*)AllocDynamic(3 * sizeof(Gfx));
		gfx			   = gfxHead;
		obj->header.gfx.node.flags = (obj->header.gfx.node.flags & 0xFF) | (GRAPH_NODE_TYPE_FUNCTIONAL | GRAPH_NODE_TYPE_400); // sets bits 8, 10 and zeros upper byte

		gDPSetEnvColor(gfx++, 255, 255, 255, heldObject->oOpacity);

		gSPEndDisplayList(gfx);
	}

	return gfxHead;
}

/**
 * An absolute value function.
 */
f32 absf_2(f32 f)
{
	if(f < 0)
	{
		f *= -1.0f;
	}
	return f;
}

/**
 * Turns an object away from floors/walls that it runs into.
 */
void turn_obj_away_from_surface(f32 velX, f32 velZ, f32 nX, UNUSED f32 nY, f32 nZ, f32* objYawX, f32* objYawZ)
{
	*objYawX = (nZ * nZ - nX * nX) * velX / (nX * nX + nZ * nZ) - 2 * velZ * (nX * nZ) / (nX * nX + nZ * nZ);

	*objYawZ = (nX * nX - nZ * nZ) * velZ / (nX * nX + nZ * nZ) - 2 * velX * (nX * nZ) / (nX * nX + nZ * nZ);
}

/**
 * Finds any wall collisions, applies them, and turns away from the surface.
 */
s32 obj_find_wall(f32 objNewX, f32 objY, f32 objNewZ, f32 objVelX, f32 objVelZ)
{
	struct WallCollisionData hitbox;
	f32 wall_nX, wall_nY, wall_nZ, objVelXCopy, objVelZCopy, objYawX, objYawZ;

	hitbox.x       = objNewX;
	hitbox.y       = objY;
	hitbox.z       = objNewZ;
	hitbox.offsetY = o->hitboxHeight / 2.0f;
	hitbox.radius  = o->hitboxRadius;

	if(find_wall_collisions(&hitbox) != 0)
	{
		o->oPosX = hitbox.x;
		o->oPosY = hitbox.y;
		o->oPosZ = hitbox.z;

		wall_nX = hitbox.walls[0]->normal.x;
		wall_nY = hitbox.walls[0]->normal.y;
		wall_nZ = hitbox.walls[0]->normal.z;

		objVelXCopy = objVelX;
		objVelZCopy = objVelZ;

		// Turns away from the first wall only.
		turn_obj_away_from_surface(objVelXCopy, objVelZCopy, wall_nX, wall_nY, wall_nZ, &objYawX, &objYawZ);

		o->oMoveAngleYaw = atan2s(objYawZ, objYawX);
		return FALSE;
	}

	return TRUE;
}

/**
 * Turns an object away from steep floors, similarly to walls.
 */
s32 turn_obj_away_from_steep_floor(struct Surface* objFloor, f32 floorY, f32 objVelX, f32 objVelZ)
{
	f32 floor_nX, floor_nY, floor_nZ, objVelXCopy, objVelZCopy, objYawX, objYawZ;

	if(objFloor == NULL)
	{
		//! (OOB Object Crash) TRUNC overflow exception after 36 minutes
		o->oMoveAngleYaw += 32767.999200000002; /* ¯\_(ツ)_/¯ */
		return FALSE;
	}

	floor_nX = objFloor->normal.x;
	floor_nY = objFloor->normal.y;
	floor_nZ = objFloor->normal.z;

	// If the floor is steep and we are below it (i.e. walking into it), turn away from the floor.
	if(floor_nY < 0.5 && floorY > o->oPosY)
	{
		objVelXCopy = objVelX;
		objVelZCopy = objVelZ;
		turn_obj_away_from_surface(objVelXCopy, objVelZCopy, floor_nX, floor_nY, floor_nZ, &objYawX, &objYawZ);
		o->oMoveAngleYaw = atan2s(objYawZ, objYawX);
		return FALSE;
	}

	return TRUE;
}

/**
 * Orients an object with the given normals, typically the surface under the object.
 */
void obj_orient_graph(struct Object* obj, f32 normalX, f32 normalY, f32 normalZ)
{
	Vec3f objVisualPosition, surfaceNormals;

	Mat4* throwMatrix;

	// Passes on orienting certain objects that shouldn't be oriented, like boulders.
	if(sOrientObjWithFloor == FALSE)
	{
		return;
	}

	// Passes on orienting billboard objects, i.e. coins, trees, etc.
	if((obj->header.gfx.node.flags & GRAPH_RENDER_BILLBOARD) != 0)
	{
		return;
	}

	throwMatrix = (Mat4*)AllocDynamic(sizeof(*throwMatrix));
	// If out of memory, fail to try orienting the object.
	if(throwMatrix == NULL)
	{
		return;
	}

	objVisualPosition[0] = obj->oPosX;
	objVisualPosition[1] = obj->oPosY + obj->oGraphYOffset;
	objVisualPosition[2] = obj->oPosZ;

	surfaceNormals[0] = normalX;
	surfaceNormals[1] = normalY;
	surfaceNormals[2] = normalZ;

	mtxf_align_terrain_normal(*throwMatrix, surfaceNormals, objVisualPosition, obj->oFaceAngleYaw);
	obj->header.gfx.throwMatrix = (void*)throwMatrix;
}

/**
 * Determines an object's forward speed multiplier.
 */
void calc_obj_friction(f32* objFriction, f32 floor_nY)
{
	if(floor_nY < 0.2 && o->oFriction < 0.9999)
	{
		*objFriction = 0;
	}
	else
	{
		*objFriction = scale_scaler(o->oFriction, FRAME_RATE_SCALER);
	}
}

/**
 * Updates an objects speed for gravity and updates Y position.
 */
void calc_new_obj_vel_and_pos_y(struct Surface* objFloor, f32 objFloorY, f32 objVelX, f32 objVelZ)
{
	f32 floor_nX = objFloor->normal.x;
	f32 floor_nY = objFloor->normal.y;
	f32 floor_nZ = objFloor->normal.z;
	f32 objFriction;

	// Caps vertical speed with a "terminal velocity".
	o->oVelY -= o->oGravity * FRAME_RATE_SCALER;
	if(o->oVelY > 75.0)
	{
		o->oVelY = 75.0;
	}

	if(o->oVelY < -75.0)
	{
		o->oVelY = -75.0;
	}

	o->oPosY += o->oVelY * FRAME_RATE_SCALER;

	// Snap the object up to the floor.
	if(o->oPosY < objFloorY)
	{
		o->oPosY = objFloorY;

		// Bounces an object if the ground is hit fast enough.
		if(o->oVelY < -17.5)
		{
			o->oVelY = -(o->oVelY / 2.0f);
		}
		else
		{
			o->oVelY = 0;
		}
	}

	//! (Obj Position Crash) If you got an object with height past 2^31, the game would crash.
	if((s32)o->oPosY >= (s32)objFloorY && (s32)o->oPosY < (s32)objFloorY + 37)
	{
		obj_orient_graph(o, floor_nX, floor_nY, floor_nZ);

		// Adds horizontal component of gravity for horizontal speed.
		objVelX += (floor_nX * (floor_nX * floor_nX + floor_nZ * floor_nZ) / (floor_nX * floor_nX + floor_nY * floor_nY + floor_nZ * floor_nZ) * o->oGravity * 2) * FRAME_RATE_SCALER;
		objVelZ += (floor_nZ * (floor_nX * floor_nX + floor_nZ * floor_nZ) / (floor_nX * floor_nX + floor_nY * floor_nY + floor_nZ * floor_nZ) * o->oGravity * 2) * FRAME_RATE_SCALER;

		if(objVelX < 0.000001 && objVelX > -0.000001)
		{
			objVelX = 0;
		}
		if(objVelZ < 0.000001 && objVelZ > -0.000001)
		{
			objVelZ = 0;
		}

		if(objVelX != 0 || objVelZ != 0)
		{
			o->oMoveAngleYaw = atan2s(objVelZ, objVelX);
		}

		calc_obj_friction(&objFriction, floor_nY);
		o->oForwardVel = sqrtf(objVelX * objVelX + objVelZ * objVelZ) * objFriction;
	}
}

void calc_new_obj_vel_and_pos_y_underwater(struct Surface* objFloor, f32 floorY, f32 objVelX, f32 objVelZ, f32 waterY)
{
	f32 floor_nX = objFloor->normal.x;
	f32 floor_nY = objFloor->normal.y;
	f32 floor_nZ = objFloor->normal.z;

	f32 netYAccel = (1.0f - o->oBuoyancy) * (-1.0f * o->oGravity);
	o->oVelY -= netYAccel * FRAME_RATE_SCALER;

	// Caps vertical speed with a "terminal velocity".
	if(o->oVelY > 75.0)
	{
		o->oVelY = 75.0;
	}
	if(o->oVelY < -75.0)
	{
		o->oVelY = -75.0;
	}

	o->oPosY += o->oVelY;

	// Snap the object up to the floor.
	if(o->oPosY < floorY)
	{
		o->oPosY = floorY;

		// Bounces an object if the ground is hit fast enough.
		if(o->oVelY < -17.5)
		{
			o->oVelY = -(o->oVelY / 2);
		}
		else
		{
			o->oVelY = 0;
		}
	}

	// If moving fast near the surface of the water, flip vertical speed? To emulate skipping?
	if(o->oForwardVel > 12.5 && (waterY + 30.0f) > o->oPosY && (waterY - 30.0f) < o->oPosY)
	{
		o->oVelY = -o->oVelY;
	}

	if((s32)o->oPosY >= (s32)floorY && (s32)o->oPosY < (s32)floorY + 37)
	{
		obj_orient_graph(o, floor_nX, floor_nY, floor_nZ);

		// Adds horizontal component of gravity for horizontal speed.
		objVelX += floor_nX * (floor_nX * floor_nX + floor_nZ * floor_nZ) / (floor_nX * floor_nX + floor_nY * floor_nY + floor_nZ * floor_nZ) * netYAccel * FRAME_RATE_SCALER * 2;
		objVelZ += floor_nZ * (floor_nX * floor_nX + floor_nZ * floor_nZ) / (floor_nX * floor_nX + floor_nY * floor_nY + floor_nZ * floor_nZ) * netYAccel * FRAME_RATE_SCALER * 2;
	}

	if(objVelX < 0.000001 && objVelX > -0.000001)
	{
		objVelX = 0;
	}
	if(objVelZ < 0.000001 && objVelZ > -0.000001)
	{
		objVelZ = 0;
	}

	if(o->oVelY < 0.000001 && o->oVelY > -0.000001)
	{
		o->oVelY = 0;
	}

	if(objVelX != 0 || objVelZ != 0)
	{
		o->oMoveAngleYaw = atan2s(objVelZ, objVelX);
	}

	// Decreases both vertical velocity and forward velocity. Likely so that skips above
	// don't loop infinitely.
	o->oForwardVel = sqrtf(objVelX * objVelX + objVelZ * objVelZ) * 0.8;
	o->oVelY *= 0.8;
}

/**
 * Updates an objects position from oForwardVel and oMoveAngleYaw.
 */
void obj_update_pos_vel_xz(void)
{
	f32 xVel = o->oForwardVel * sins(o->oMoveAngleYaw) * FRAME_RATE_SCALER;
	f32 zVel = o->oForwardVel * coss(o->oMoveAngleYaw) * FRAME_RATE_SCALER;

	o->oPosX += xVel;
	o->oPosZ += zVel;
}

/**
 * Generates splashes if at surface of water, entering water, or bubbles
 * if underwater.
 */
void obj_splash(s32 waterY, s32 objY)
{
	u32 globalTimer = frameCounter;

	// Spawns waves if near surface of water and plays a noise if entering.
	if((f32)(waterY + 30) > o->oPosY && o->oPosY > (f32)(waterY - 30))
	{
		s_makeobj_nowpos(o, MODEL_WATER_WAVES_SURF, sm64::bhv::bhvObjectWaterWave());

		if(o->oVelY < -20.0f)
		{
			objsound(SOUND_OBJ_DIVING_INTO_WATER);
		}
	}

	// Spawns bubbles if underwater.
	if((objY + 50) < waterY && (globalTimer & 0x1F) == 0)
	{
		s_makeobj_nowpos(o, MODEL_WHITE_PARTICLE_SMALL, sm64::bhv::bhvObjectBubble());
	}
}

/**
 * Generic object move function. Handles walls, water, floors, and gravity.
 * Returns flags for certain interactions.
 */
s32 ObjMoveEvent(void)
{
	f32 objX = o->oPosX;
	f32 objY = o->oPosY;
	f32 objZ = o->oPosZ;

	f32 floorY;
	f32 waterY = -10000.0;

	f32 objVelX = (o->oForwardVel) * sins(o->oMoveAngleYaw);
	f32 objVelZ = (o->oForwardVel) * coss(o->oMoveAngleYaw);

	s16 collisionFlags = 0;

	// Find any wall collisions, receive the push, and set the flag.
	if(obj_find_wall(objX + objVelX, objY, objZ + objVelZ, objVelX, objVelZ) == 0)
	{
		collisionFlags += OBJ_COL_FLAG_HIT_WALL;
	}

	floorY = mcBGGroundCheck(objX + objVelX, objY, objZ + objVelZ, &sObjFloor);

	if(turn_obj_away_from_steep_floor(sObjFloor, floorY, objVelX, objVelZ) == 1)
	{
		waterY = mcWaterCheck(objX + objVelX, objZ + objVelZ);

		if(waterY > objY)
		{
			calc_new_obj_vel_and_pos_y_underwater(sObjFloor, floorY, objVelX, objVelZ, waterY);
			collisionFlags += OBJ_COL_FLAG_UNDERWATER;
		}
		else
		{
			calc_new_obj_vel_and_pos_y(sObjFloor, floorY, objVelX, objVelZ);
		}
	}
	else
	{
		// Treat any awkward floors similar to a wall.
		collisionFlags += ((collisionFlags & OBJ_COL_FLAG_HIT_WALL) ^ OBJ_COL_FLAG_HIT_WALL);
	}

	obj_update_pos_vel_xz();

	if((s32)o->oPosY == (s32)floorY)
	{
		collisionFlags += OBJ_COL_FLAG_GROUNDED;
	}

	if((s32)o->oVelY == 0)
	{
		collisionFlags += OBJ_COL_FLAG_NO_Y_VEL;
	}

	// Generate a splash if in water.
	obj_splash((s32)waterY, (s32)o->oPosY);
	return collisionFlags;
}

/**
 * Takes an object step but does not orient with the object's floor.
 * Used for boulders, falling pillars, and the rolling snowman body.
 *
 * TODO: Fix fake EU matching.
 */
s32 ObjMoveEvent_noInc(void)
{
	s16 collisionFlags  = 0;
	sOrientObjWithFloor = FALSE;
	collisionFlags	    = ObjMoveEvent();
	sOrientObjWithFloor = TRUE;

	return collisionFlags;
}

/**
 * Uses an object's forward velocity and yaw to move its X, Y, and Z positions.
 * This does accept an object as an argument, though it is always called with `o`.
 * If it wasn't called with `o`, it would modify `o`'s X and Z velocities based on
 * `obj`'s forward velocity and yaw instead of `o`'s, and wouldn't update `o`'s
 * position.
 */
void ObjSpeedOn(struct Object* obj)
{
	o->oVelX = obj->oForwardVel * sins(obj->oMoveAngleYaw);
	o->oVelZ = obj->oForwardVel * coss(obj->oMoveAngleYaw);

	obj->oPosX += o->oVelX * FRAME_RATE_SCALER;
	obj->oPosY += obj->oVelY * FRAME_RATE_SCALER;
	obj->oPosZ += o->oVelZ * FRAME_RATE_SCALER;
}

/**
 * Checks if a point is within distance from Mario's graphical position. Test is exclusive.
 */
s32 PlayerApproach(f32 x, f32 y, f32 z, s32 dist)
{
	f32 mGfxX = gMarioObject->header.gfx.pos[0];
	f32 mGfxY = gMarioObject->header.gfx.pos[1];
	f32 mGfxZ = gMarioObject->header.gfx.pos[2];

	if((x - mGfxX) * (x - mGfxX) + (y - mGfxY) * (y - mGfxY) + (z - mGfxZ) * (z - mGfxZ) < (f32)(dist * dist))
	{
		return TRUE;
	}

	return FALSE;
}

/**
 * Checks whether a point is within distance of a given point. Test is exclusive.
 */
s32 ObjApproach(struct Object* obj, f32 x, f32 y, f32 z, s32 dist)
{
	f32 objX = obj->oPosX;
	f32 objY = obj->oPosY;
	f32 objZ = obj->oPosZ;

	if((x - objX) * (x - objX) + (y - objY) * (y - objY) + (z - objZ) * (z - objZ) < (f32)(dist * dist))
	{
		return TRUE;
	}

	return FALSE;
}

/**
 * Sets an object as visible if within a certain distance of Mario's graphical position.
 */
void PlayerApproachOnOff(struct Object* obj, s32 dist)
{
	f32 objX = obj->oPosX;
	f32 objY = obj->oPosY;
	f32 objZ = obj->oPosZ;

	if(sm64::config().camera().disableDistanceClip() || PlayerApproach(objX, objY, objZ, dist) == TRUE)
	{
		obj->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
	}
	else
	{
		obj->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
	}
}

/**
 * Turns an object towards home if Mario is not near to it.
 */
s32 ShapePatrol(struct Object* obj, f32 homeX, f32 y, f32 homeZ, s32 dist)
{
	f32 homeDistX	     = homeX - obj->oPosX;
	f32 homeDistZ	     = homeZ - obj->oPosZ;
	s16 angleTowardsHome = atan2s(homeDistZ, homeDistX);

	if(PlayerApproach(homeX, y, homeZ, dist) == TRUE)
	{
		return TRUE;
	}
	else
	{
		obj->oMoveAngleYaw = s_chase_angle(obj->oMoveAngleYaw, angleTowardsHome, 320 / FRAME_RATE_SCALER_INV);
	}

	return FALSE;
}

/**
 * Randomly displaces an objects home if RNG says to, and turns the object towards its home.
 */
void ShapeRandomAngle(struct Object* obj, f32 homeX, UNUSED f32 homeY, f32 homeZ, s32 baseDisp)
{
	s16 angleToNewHome;
	f32 homeDistX, homeDistZ;

	if((s32)(Randomf() * 50.0f) == 0)
	{
		obj->oHomeX = (f32)(baseDisp * 2) * Randomf() - (f32)baseDisp + homeX;
		obj->oHomeZ = (f32)(baseDisp * 2) * Randomf() - (f32)baseDisp + homeZ;
	}

	homeDistX	   = obj->oHomeX - obj->oPosX;
	homeDistZ	   = obj->oHomeZ - obj->oPosZ;
	angleToNewHome	   = atan2s(homeDistZ, homeDistX);
	obj->oMoveAngleYaw = s_chase_angle(obj->oMoveAngleYaw, angleToNewHome, 320 / FRAME_RATE_SCALER_INV);
}

/**
 * A series of checks using sin and cos to see if a given angle is facing in the same direction
 * of a given angle, within a certain range.
 */
s32 ShapeSameAngle(u32 base, u32 goal, s16 range)
{
	s16 dAngle = (u16)goal - (u16)base;

	if(((f32)sins(-range) < (f32)sins(dAngle)) && ((f32)sins(dAngle) < (f32)sins(range)) && (coss(dAngle) > 0))
	{
		return TRUE;
	}

	return FALSE;
}

/**
 * Finds any wall collisions and returns what the displacement vector would be.
 */
s32 PositionWallCheck(Vec3f dist, f32 x, f32 y, f32 z, f32 radius)
{
	struct WallCollisionData hitbox;
	UNUSED u8 filler[0x20];

	hitbox.x       = x;
	hitbox.y       = y;
	hitbox.z       = z;
	hitbox.offsetY = 10.0f;
	hitbox.radius  = radius;

	if(find_wall_collisions(&hitbox) != 0)
	{
		dist[0] = hitbox.x - x;
		dist[1] = hitbox.y - y;
		dist[2] = hitbox.z - z;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
 * Spawns a number of coins at the location of an object
 * with a random forward velocity, y velocity, and direction.
 */
void iwa_MakeCoin(struct Object* obj, s8 nCoins)
{
	struct Object* coin;
	s8 count;

	for(count = 0; count < nCoins; count++)
	{
		coin		    = s_makeobj_nowpos(obj, MODEL_YELLOW_COIN, sm64::bhv::bhvMovingYellowCoin());
		coin->oForwardVel   = Randomf() * 20;
		coin->oVelY	    = Randomf() * 40 + 20;
		coin->oMoveAngleYaw = RandomU16();
	}
}

/**
 * Controls whether certain objects should flicker/when to despawn.
 */
s32 iwa_TimerRemove(struct Object* obj, s16 lifeSpan)
{
	if(obj->oTimer < lifeSpan * FRAME_RATE_SCALER_INV)
	{
		return FALSE;
	}

	if(obj->oTimer < (lifeSpan + 40) * FRAME_RATE_SCALER_INV)
	{
		if(obj->oTimer % 2 != 0)
		{
			obj->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
		}
		else
		{
			obj->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
		}
	}
	else
	{
		obj->activeFlags = 0;
		return TRUE;
	}

	return FALSE;
}

/**
 * Checks if a given room is Mario's current room, even if on an object.
 */
s8 iwa_MapAreaCheck(s16 room)
{
	s16 result;

	// Since object surfaces have room 0, this tests if the surface is an
	// object first and uses the last room if so.
	if(gMarioCurrentRoom == 0)
	{
		if(room == sPrevCheckMarioRoom)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if(room == gMarioCurrentRoom)
		{
			result = TRUE;
		}
		else
		{
			result = FALSE;
		}

		sPrevCheckMarioRoom = gMarioCurrentRoom;
	}

	return result;
}

/**
 * Triggers dialog when Mario is facing an object and controls it while in the dialog.
 */
s16 iwa_ObjMessage(s32* inDialog, s16 dialogID, f32 dist, s32 actionArg)
{
	s16 dialogueResponse;

	if((PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, (s32)dist) == 1 && ShapeSameAngle(o->oFaceAngleYaw, gMarioObject->header.gfx.angle[1] + 0x8000, 0x1000) == 1 && ShapeSameAngle(o->oMoveAngleYaw, o->oAngleToMario, 0x1000) == 1) || (*inDialog == 1))
	{
		*inDialog = 1;

		if(CtrlPlayerDialog(actionArg) == 2)
		{ // If Mario is speaking.
			dialogueResponse = cameraDemoStratMsgNum(CUTSCENE_DIALOG, o, dialogID);
			if(dialogueResponse != 0)
			{
				CtrlPlayerDialog(0);
				*inDialog = 0;
				return dialogueResponse;
			}
			return 0;
		}
	}

	return 0;
}

/**
 *Checks if a floor is one that should cause an object to "die".
 */
void ObjDangerCheck(s16 collisionFlags, struct Surface* floor)
{
	if(floor == NULL)
	{
		return;
	}

	if((collisionFlags & OBJ_COL_FLAG_GROUNDED) == 1)
	{
		switch(floor->type)
		{
			case SURFACE_BURNING:
				o->oAction = OBJ_ACT_LAVA_DEATH;
				break;
				//! @BUG Doesn't check for the vertical wind death floor.
			case SURFACE_DEATH_PLANE:
				o->oAction = OBJ_ACT_DEATH_PLANE_DEATH;
				break;
			default:
				break;
		}
	}
}

/**
 * Controls an object dying in lava by creating smoke, sinking the object, playing
 * audio, and eventually despawning it. Returns TRUE when the obj is dead.
 */
s32 ObjMeltEvent(void)
{
	struct Object* deathSmoke;

	if(o->oTimer >= 31 * FRAME_RATE_SCALER_INV)
	{
		o->activeFlags = 0;
		return TRUE;
	}
	else
	{
		// Sinking effect
		o->oPosY -= 10.0f * FRAME_RATE_SCALER;
	}

	if((o->oTimer % (8 * FRAME_RATE_SCALER_INV)) == 0)
	{
		objsound(SOUND_OBJ_BULLY_EXPLODE_2);
		deathSmoke = s_makeobj_nowpos(o, MODEL_SMOKE, sm64::bhv::bhvBobombBullyDeathSmoke());
		deathSmoke->oPosX += Randomf() * 20.0f;
		deathSmoke->oPosY += Randomf() * 20.0f;
		deathSmoke->oPosZ += Randomf() * 20.0f;
		deathSmoke->oForwardVel = Randomf() * 10.0f;
	}

	return FALSE;
}

/**
 * Spawns an orange number object relatively, such as those that count up for secrets.
 */
void AppNumber(s8 behParam, s16 relX, s16 relY, s16 relZ)
{
	struct Object* orangeNumber;

	if(behParam >= 10)
	{
		return;
	}

	orangeNumber = s_makeobj_chain(behParam, relX, relY, relZ, o, MODEL_NUMBER, sm64::bhv::bhvOrangeNumber());
	orangeNumber->oPosY += 25.0f;
}

#include "behaviors/moving_coin.inc.c"
#include "behaviors/seaweed.inc.c"
#include "behaviors/bobomb.inc.c"
#include "behaviors/cannon_door.inc.c"
#include "behaviors/whirlpool.inc.c"
#include "behaviors/amp.inc.c"
#include "behaviors/butterfly.inc.c"
#include "behaviors/hoot.inc.c"
#include "behaviors/beta_holdable_object.inc.c"
#include "behaviors/bubble.inc.c"
#include "behaviors/water_wave.inc.c"
#include "behaviors/explosion.inc.c"
#include "behaviors/corkbox.inc.c"
#include "behaviors/bully.inc.c"
#include "behaviors/water_ring.inc.c"
#include "behaviors/bowser_bomb.inc.c"
#include "behaviors/celebration_star.inc.c"
#include "behaviors/drawbridge.inc.c"
#include "behaviors/bomp.inc.c"
#include "behaviors/sliding_platform.inc.c"
#include "behaviors/moneybag.inc.c"
#include "behaviors/bowling_ball.inc.c"
#include "behaviors/cruiser.inc.c"
#include "behaviors/spindel.inc.c"
#include "behaviors/pyramid_wall.inc.c"
#include "behaviors/pyramid_elevator.inc.c"
#include "behaviors/pyramid_top.inc.c"
#include "behaviors/sound_waterfall.inc.c"
#include "behaviors/sound_volcano.inc.c"
#include "behaviors/castle_flag.inc.c"
#include "behaviors/sound_birds.inc.c"
#include "behaviors/sound_ambient.inc.c"
#include "behaviors/sound_sand.inc.c"
#include "behaviors/castle_cannon_grate.inc.c"
#include "behaviors/snowman.inc.c"
#include "behaviors/boulder.inc.c"
#include "behaviors/cap.inc.c"
#include "behaviors/spawn_star.inc.c"
#include "behaviors/red_coin.inc.c"
#include "behaviors/hidden_star.inc.c"
#include "behaviors/rolling_log.inc.c"
#include "behaviors/mushroom_1up.inc.c"
#include "behaviors/controllable_platform.inc.c"
#include "behaviors/breakable_box_small.inc.c"
#include "behaviors/snow_mound.inc.c"
#include "behaviors/floating_platform.inc.c"
#include "behaviors/arrow_lift.inc.c"
#include "behaviors/orange_number.inc.c"
#include "behaviors/manta_ray.inc.c"
#include "behaviors/falling_pillar.inc.c"
#include "behaviors/floating_box.inc.c"
#include "behaviors/decorative_pendulum.inc.c"
#include "behaviors/treasure_chest.inc.c"
#include "behaviors/mips.inc.c"
#include "behaviors/yoshi.inc.c"
