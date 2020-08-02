#pragma once

// This file contains various data types used in Super Mario 64 that don't yet
// have an appropriate header.

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef ENABLE_120FPS
#define ENABLE_60FPS
#endif

#ifdef ENABLE_30FPS
#define FRAME_RATE_SCALER_INV 1
#define FRAME_RATE_SCALER 1.0
#else
#define ENABLE_60FPS
#define FRAME_RATE_SCALER_INV 2
#define FRAME_RATE_SCALER 0.5
//#define FRAME_RATE_SCALER_INV sm64::config().game().framerateScalerInv()
//#define FRAME_RATE_SCALER sm64::config().game().framerateScaler()
#endif

#include <ultra64.h>
#include "macros.h"
#include "controller/controller.h"
#include "engine/time.h"

#define	ANGLE180 0x8000

struct LevelCommand
{
	/*00*/ u8 type;
	/*01*/ u8 size;
	/*02*/ // variable sized argument data
};

typedef f32 Vec2f[2];
typedef f32 Vec3f[3]; // X, Y, Z, where Y is up
typedef s16 Vec3s[3];
typedef s32 Vec3i[3];
typedef f32 Vec4f[4];
typedef s16 Vec4s[4];

typedef f32 Mat4[4][4];

typedef uintptr_t GeoLayout;
typedef uintptr_t LevelScript;
typedef uintptr_t BehaviorScript;

typedef s16 Movtex;
typedef s16 MacroObject;
typedef s16 Collision;
typedef s16 Trajectory;
typedef s16 PaintingData;



typedef void(*BhvCallback)();

enum SpTaskState
{
	SPTASK_STATE_NOT_STARTED,
	SPTASK_STATE_RUNNING,
	SPTASK_STATE_INTERRUPTED,
	SPTASK_STATE_FINISHED,
	SPTASK_STATE_FINISHED_DP
};

struct SPTask
{
	/*0x00*/ OSTask task;
	/*0x40*/ OSMesgQueue *msgqueue;
	/*0x44*/ OSMesg msg;
	/*0x48*/ enum SpTaskState state;
}; // size = 0x4C, align = 0x8

struct VblankHandler
{
	OSMesgQueue *queue;
	OSMesg msg;
};

#define ANIM_FLAG_NOLOOP     (1 << 0) // 0x01
#define ANIM_FLAG_FORWARD    (1 << 1) // 0x02
#define ANIM_FLAG_PAUSE          (1 << 2) // 0x04
#define ANIM_FLAG_HOR_TRANS  (1 << 3) // 0x08
#define ANIM_FLAG_VERT_TRANS (1 << 4) // 0x10
#define ANIM_FLAG_5          (1 << 5) // 0x20
#define ANIM_FLAG_6          (1 << 6) // 0x40
#define ANIM_FLAG_7          (1 << 7) // 0x80


struct Animation
{
	enum Flags : s16
	{
		NONE = 0,
		NOLOOP = (1 << 0), // 0x01
		FORWARD = (1 << 1), // 0x02
		PAUSE = (1 << 2), // 0x04
		HOR_TRANS = (1 << 3), // 0x08
		VERT_TRANS = (1 << 4), // 0x10
		FLAG_5 = (1 << 5), // 0x20
		FLAG_6 = (1 << 6), // 0x40
		FLAG_7 = (1 << 7) // 0x80
	};

	/*0x00*/ s16 flags;
	/*0x02*/ s16 yTransDivisor; // divisor whose quotient is used to scale the Y axis
	/*0x04*/ s16 startFrame;
	/*0x06*/ s16 firstFrame;
	/*0x08*/ s16 lastFrame;
	/*0x0A*/ s16 unk0A;
	/*0x0C*/ const s16 *values;
	/*0x10*/ const u16 *index;
	/*0x14*/ u32 length; // only used with Mario animations to determine how much to load. 0 otherwise.
};

#define ANIMINDEX_NUMPARTS(animindex) (sizeof(animindex) / sizeof(u16) / 6 - 1)

struct GraphNode
{
	/*0x00*/ s16 type; // structure type
	/*0x02*/ s16 flags; // hi = drawing layer, lo = rendering modes
	/*0x04*/ struct GraphNode *prev;
	/*0x08*/ struct GraphNode *next;
	/*0x0C*/ struct GraphNode *parent;
	/*0x10*/ struct GraphNode *children;
};

// struct AnimInfo?
struct GraphNodeObject_sub
{
	/*0x00 0x38*/ s16 animID;
	/*0x02 0x3A*/ s16 animYTrans;
	/*0x04 0x3C*/ struct Animation *curAnim;
	/*0x08 0x40*/ s16 animFrameUnused;
	/*0x0A 0x42*/ u16 animTimer;
	/*0x0C 0x44*/ s32 animFrameAccelAssist;
	/*0x10 0x48*/ s32 animAccel;
#ifdef __cplusplus
	s16 frame() const;
	bool isPartialFrame() const;

	void setFrame(s16 frame);

	void setFrameRaw(s16 frame);

	s16 incrementFrame(s16 count = 1);
#endif
};

struct GraphNodeObject
{
	/*0x00*/ struct GraphNode node;
	/*0x14*/ struct GraphNode *sharedChild;
	/*0x18*/ s8 unk18;
	/*0x19*/ s8 unk19;
	/*0x1A*/ Vec3s angle;
	/*0x20*/ Vec3f pos;
	/*0x2C*/ Vec3f scale;
	/*0x38*/ struct GraphNodeObject_sub unk38;
	/*0x4C*/ struct SpawnInfo *unk4C;
	/*0x50*/ void *throwMatrix; // matrix ptr
	/*0x54*/ Vec3f cameraToObject;
};

struct ObjectNode
{
	struct GraphNodeObject gfx;
	struct ObjectNode *next;
	struct ObjectNode *prev;
};

// NOTE: Since ObjectNode is the first member of Object, it is difficult to determine
// whether some of these pointers point to ObjectNode or Object.

struct Object
{
	/*0x000*/ struct ObjectNode header;
	/*0x068*/ struct Object *parentObj;
	/*0x06C*/ struct Object *prevObj;
	/*0x070*/ u32 collidedObjInteractTypes;
	/*0x074*/ s16 activeFlags;
	/*0x076*/ s16 numCollidedObjs;
	/*0x078*/ struct Object *collidedObjs[4];
	/*0x088*/
	union
	{
		// Object fields. See object_fields.h.
		u32 asU32[0x50];
		s32 asS32[0x50];
		s16 asS16[0x50][2];
		f32 asF32[0x50];
#if !IS_64_BIT
		s16 *asS16P[0x50];
		s32 *asS32P[0x50];
		struct Animation **asAnims[0x50];
		struct Waypoint *asWaypoint[0x50];
		struct ChainSegment *asChainSegment[0x50];
		struct Object *asObject[0x50];
		struct Surface *asSurface[0x50];
		void *asVoidPtr[0x50];
		const void *asConstVoidPtr[0x50];
#endif
	} rawData;
#if IS_64_BIT
	union
	{
		s16 *asS16P[0x50];
		s32 *asS32P[0x50];
		struct Animation **asAnims[0x50];
		struct Waypoint *asWaypoint[0x50];
		struct ChainSegment *asChainSegment[0x50];
		struct Object *asObject[0x50];
		struct Surface *asSurface[0x50];
		void *asVoidPtr[0x50];
		const void *asConstVoidPtr[0x50];
	} ptrData;
#endif
	/*0x1C8*/ u32 unused1;
	/*0x1CC*/ const BehaviorScript *behScript;
	/*0x1D0*/ u32 stackIndex;
	/*0x1D4*/ uintptr_t stack[8];
	/*0x1F4*/ s16 unk1F4;
	/*0x1F6*/ s16 respawnInfoType;
	/*0x1F8*/ f32 hitboxRadius;
	/*0x1FC*/ f32 hitboxHeight;
	/*0x200*/ f32 hurtboxRadius;
	/*0x204*/ f32 hurtboxHeight;
	/*0x208*/ f32 hitboxDownOffset;
	/*0x20C*/ const BehaviorScript *behavior;
	/*0x210*/ u32 unused2;
	/*0x214*/ struct Object *platform;
	/*0x218*/ void *collisionData;
	/*0x21C*/ Mat4 transform;
	/*0x25C*/ void *respawnInfo;
};

struct ObjectHitbox
{
	/*0x00*/ u32 interactType;
	/*0x04*/ u8 downOffset;
	/*0x05*/ s8 damageOrCoinValue;
	/*0x06*/ s8 health;
	/*0x07*/ s8 numLootCoins;
	/*0x08*/ s16 radius;
	/*0x0A*/ s16 height;
	/*0x0C*/ s16 hurtboxRadius;
	/*0x0E*/ s16 hurtboxHeight;
};

struct Waypoint
{
	s16 flags;
	Vec3s pos;
};

struct Surface
{
	/*0x00*/ s16 type;
	/*0x02*/ s16 force;
	/*0x04*/ s8 flags;
	/*0x05*/ s8 room;
	/*0x06*/ s16 lowerY;
	/*0x08*/ s16 upperY;
	/*0x0A*/ Vec3s vertex1;
	/*0x10*/ Vec3s vertex2;
	/*0x16*/ Vec3s vertex3;
	/*0x1C*/ struct
	{
		f32 x;
		f32 y;
		f32 z;
	} normal;
	/*0x28*/ f32 originOffset;
	/*0x2C*/ struct Object *object;
};

struct MarioBodyState
{
	 u32 action;
	s8 capState;
	s8 eyeState;
	s8 handState;
	s8 unk07;
	s16 modelState;
	s8 grabPos;
	u8 punchState;
	Vec3s torsoAngle;
	Vec3s headAngle;
	Vec3f unk18;
	u8 padding[4];
};

struct OffsetSizePair
{
	u32 offset;
	u32 size;
};

struct MarioAnimDmaRelatedThing
{
	u32 count;
	u8 *srcAddr;
	struct OffsetSizePair anim[1]; // dynamic size
};

struct MarioAnimation
{
	struct MarioAnimDmaRelatedThing *animDmaTable;
	u8 *currentAnimAddr;
	struct Animation *targetAnim;
	u8 padding[4];
};

#include "game/player.h"
