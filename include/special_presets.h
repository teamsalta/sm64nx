#ifndef _SPECIAL_PRESETS_H
#define _SPECIAL_PRESETS_H

#include "special_preset_names.h"
#include "behavior_data.h"
#include "model_ids.h"

// Special Preset types
#define SPTYPE_NO_YROT_OR_PARAMS  0 // object is 8-bytes long, no y-rotation or any behavior params
#define SPTYPE_YROT_NO_PARAMS     1 // object is 10-bytes long, has y-rotation but no params
#define SPTYPE_PARAMS_AND_YROT    2 // object is 12-bytes long, has y-rotation and params
#define SPTYPE_UNKNOWN            3 // object is 14-bytes long, has 3 extra shorts that get converted to floats.
#define SPTYPE_DEF_PARAM_AND_YROT 4 // object is 10-bytes long, has y-rotation and uses the default param

struct SpecialPreset
{
    /*00*/ u8  preset_id;
    /*01*/ u8  type;      // Determines whether object is 8, 10, 12 or 14 bytes long.
    /*02*/ u8  defParam;  // Default parameter, only used when type is SPTYPE_DEF_PARAM_AND_YROT
    /*03*/ u8  model;     
    /*04*/ const BehaviorScript *behavior;
};

// Some Models ID's are missing their names because they are probably unused

static struct SpecialPreset SpecialObjectPresets[] = 
{
    {0x00, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_NONE, NULL},
    {0x01, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_YELLOW_COIN, sm64::bhv::bhvYellowCoin()},
    {0x02, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_YELLOW_COIN, sm64::bhv::bhvYellowCoin()},
    {0x03, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_UNKNOWN_B8, sm64::bhv::bhvStaticObject()},
    {0x04, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_BOO, sm64::bhv::bhvCourtyardBooTriplet()},
    {0x05, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_UNKNOWN_AC, sm64::bhv::bhvCastleFloorTrap()},
    {0x06, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_LLL_MOVING_OCTAGONAL_MESH_PLATFORM, sm64::bhv::bhvLllMovingOctagonalMeshPlatform()},
    {0x07, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_CCM_SNOWMAN_HEAD, sm64::bhv::bhvSnowBall()},
    {0x08, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LLL_DRAWBRIDGE_PART, sm64::bhv::bhvLllDrawbridgeSpawner()},
    {0x09, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_NONE, sm64::bhv::bhvStaticObject()},
    {0x0A, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_LLL_ROTATING_BLOCK_FIRE_BARS, sm64::bhv::bhvLllRotatingBlockWithFireBars()},
    {0x0B, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_NONE, sm64::bhv::bhvLllFloatingWoodBridge()},
    {0x0C, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_NONE, sm64::bhv::bhvLllTumblingBridge()},
    {0x0D, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_LLL_ROTATING_HEXAGONAL_RING , sm64::bhv::bhvLllRotatingHexagonalRing()},
    {0x0E, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LLL_SINKING_RECTANGULAR_PLATFORM, sm64::bhv::bhvLllSinkingRectangularPlatform()},
    {0x0F, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_LLL_SINKING_SQUARE_PLATFORMS, sm64::bhv::bhvLllSinkingSquarePlatforms()},
    {0x10, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_LLL_TILTING_SQUARE_PLATFORM, sm64::bhv::bhvLllTiltingInvertedPyramid()},
    {0x11, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_NONE, sm64::bhv::bhvLllBowserPuzzle()},
    {0x12, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_NONE, sm64::bhv::bhvMrI()},
    {0x13, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_BULLY, sm64::bhv::bhvSmallBully()},
    {0x14, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_BULLY_BOSS, sm64::bhv::bhvBigBully()},
    {0x15, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_NONE, sm64::bhv::bhvStaticObject()},
    {0x16, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_NONE, sm64::bhv::bhvStaticObject()},
    {0x17, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_NONE, sm64::bhv::bhvStaticObject()},
    {0x18, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_NONE, sm64::bhv::bhvStaticObject()},
    {0x19, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_NONE, sm64::bhv::bhvStaticObject()},
    {0x1A, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_YELLOW_COIN, sm64::bhv::bhvMovingBlueCoin()},
    {0x1B, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_TREASURE_CHEST_BASE, sm64::bhv::bhvBetaChestBottom()},
    {0x1C, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_WATER_RING, sm64::bhv::bhvJetStreamRingSpawner()},
    {0x1D, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_WATER_MINE, sm64::bhv::bhvBowserBomb()},
    {0x1E, SPTYPE_UNKNOWN           , 0x00, MODEL_NONE, sm64::bhv::bhvStaticObject()},
    {0x1F, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_NONE, sm64::bhv::bhvStaticObject()},
    {0x20, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_BUTTERFLY, sm64::bhv::bhvButterfly()},
    {0x21, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_BOWSER, sm64::bhv::bhvBowser()},
    {0x22, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_WF_ROTATING_WOODEN_PLATFORM, sm64::bhv::bhvWfRotatingWoodenPlatform()},
    {0x23, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_WF_SMALL_BOMP, sm64::bhv::bhvSmallBomp()},
    {0x24, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_WF_SLIDING_PLATFORM, sm64::bhv::bhvWfSlidingPlatform()},
    {0x25, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_NONE, sm64::bhv::bhvTowerPlatformGroup()},
    {0x26, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_NONE, sm64::bhv::bhvRotatingCounterClockwise()},
    {0x27, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_WF_TUMBLING_BRIDGE, sm64::bhv::bhvWfTumblingBridge()},
    {0x28, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_WF_LARGE_BOMP, sm64::bhv::bhvLargeBomp()},
    {0x65, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_03, sm64::bhv::bhvStaticObject()},
    {0x66, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_04, sm64::bhv::bhvStaticObject()},
    {0x67, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_05, sm64::bhv::bhvStaticObject()},
    {0x68, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_06, sm64::bhv::bhvStaticObject()},
    {0x69, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_07, sm64::bhv::bhvStaticObject()},
    {0x6A, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_08, sm64::bhv::bhvStaticObject()},
    {0x6B, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_09, sm64::bhv::bhvStaticObject()},
    {0x6C, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_0A, sm64::bhv::bhvStaticObject()},
    {0x6D, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_0B, sm64::bhv::bhvStaticObject()},
    {0x6E, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_0C, sm64::bhv::bhvStaticObject()},
    {0x6F, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_0D, sm64::bhv::bhvStaticObject()},
    {0x70, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_0E, sm64::bhv::bhvStaticObject()},
    {0x71, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_0F, sm64::bhv::bhvStaticObject()},
    {0x72, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_10, sm64::bhv::bhvStaticObject()},
    {0x73, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_11, sm64::bhv::bhvStaticObject()},
    {0x74, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_12, sm64::bhv::bhvStaticObject()},
    {0x75, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_13, sm64::bhv::bhvStaticObject()},
    {0x76, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_14, sm64::bhv::bhvStaticObject()},
    {0x77, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_15, sm64::bhv::bhvStaticObject()},
    {0x78, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_LEVEL_GEOMETRY_16, sm64::bhv::bhvStaticObject()},
    {0x79, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_BOB_BUBBLY_TREE, sm64::bhv::bhvTree()},
    {0x7A, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_COURTYARD_SPIKY_TREE, sm64::bhv::bhvTree()},
    {0x7B, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_CCM_SNOW_TREE, sm64::bhv::bhvTree()},
    {0x7C, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_UNKNOWN_TREE_1A, sm64::bhv::bhvTree()},
    {0x7D, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_SSL_PALM_TREE, sm64::bhv::bhvTree()},
    {0x89, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_CASTLE_CASTLE_DOOR_UNUSED, sm64::bhv::bhvDoor()},
    {0x7E, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_CASTLE_WOODEN_DOOR_UNUSED, sm64::bhv::bhvDoor()},
    {0x7F, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_UNKNOWN_DOOR_1E, sm64::bhv::bhvDoor()},
    {0x80, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_HMC_METAL_DOOR, sm64::bhv::bhvDoor()},
    {0x81, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_HMC_HAZY_MAZE_DOOR, sm64::bhv::bhvDoor()},
    {0x82, SPTYPE_YROT_NO_PARAMS    , 0x00, MODEL_UNKNOWN_DOOR_21, sm64::bhv::bhvDoor()},
    {0x8A, SPTYPE_DEF_PARAM_AND_YROT, 0x00, MODEL_CASTLE_DOOR_0_STARS, sm64::bhv::bhvDoor()},
    {0x8B, SPTYPE_DEF_PARAM_AND_YROT, 0x01, MODEL_CASTLE_DOOR_1_STAR, sm64::bhv::bhvDoor()},
    {0x8C, SPTYPE_DEF_PARAM_AND_YROT, 0x03, MODEL_CASTLE_DOOR_3_STARS, sm64::bhv::bhvDoor()},
    {0x8D, SPTYPE_DEF_PARAM_AND_YROT, 0x00, MODEL_CASTLE_KEY_DOOR, sm64::bhv::bhvDoor()},
    {0x88, SPTYPE_PARAMS_AND_YROT   , 0x00, MODEL_CASTLE_CASTLE_DOOR, sm64::bhv::bhvDoorWarp()},
    {0x83, SPTYPE_PARAMS_AND_YROT   , 0x00, MODEL_CASTLE_WOODEN_DOOR, sm64::bhv::bhvDoorWarp()},
    {0x84, SPTYPE_PARAMS_AND_YROT   , 0x00, MODEL_UNKNOWN_DOOR_28, sm64::bhv::bhvDoorWarp()},
    {0x85, SPTYPE_PARAMS_AND_YROT   , 0x00, MODEL_CASTLE_METAL_DOOR, sm64::bhv::bhvDoorWarp()},
    {0x86, SPTYPE_PARAMS_AND_YROT   , 0x00, MODEL_UNKNOWN_DOOR_2A, sm64::bhv::bhvDoorWarp()},
    {0x87, SPTYPE_PARAMS_AND_YROT   , 0x00, MODEL_UNKNOWN_DOOR_2B, sm64::bhv::bhvDoorWarp()},
    {0xFF, SPTYPE_NO_YROT_OR_PARAMS , 0x00, MODEL_NONE, NULL}
};

#endif // _SPECIAL_PRESETS_H
