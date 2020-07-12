#!/usr/bin/python3
# Copyright 2020
# All rights reserved.

import io
import struct
import json
import os.path
import copy
import re

from macro import *
from dialog import *
from collision import *
from model_ids import *

OPTIMIZE_TRIANGLES = 1
OPTIMIZE_VERTICIES = 1

if not OPTIMIZE_TRIANGLES:
	OPTIMIZE_VERTICIES = 0

def getRootDir(path):
	if os.path.exists(os.path.join(path, 'Makefile')):
		return os.path.abspath(path)
		
	parent = os.path.abspath(os.path.join(path, os.pardir))

	if parent == path:
		return os.getcwd()
		
	return getRootDir(parent)
	
rootDir = getRootDir(os.getcwd())	

gfx_map_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'gfx_map.json')
try:
	with open(gfx_map_path, 'r') as f:
		hashMap = json.loads(f.read())
except:
	hashMap = {}
	print('failed to load ' + gfx_map_path)
	
i = 0

def inc():
	global i
	i += 1
	return i

geo_act_selector_strings			= inc()
geo_camera_fov		 				= inc()
geo_camera_main						= inc()
geo_cannon_circle_base				= inc()
geo_enfvx_main		 = inc()
geo_exec_cake_end_screen		 = inc()
geo_exec_flying_carpet_create		 = inc()
geo_exec_flying_carpet_timer_update		 = inc()
geo_exec_inside_castle_light		 = inc()
geo_file_select_strings_and_menu_cursor		 = inc()
geo_mario_hand_foot_scaler		 = inc()
geo_mario_head_rotation		 = inc()
geo_mario_rotate_wing_cap_wings		 = inc()
geo_mario_tilt_torso		 = inc()
geo_rotate_coin		 = inc()
geo_mirror_mario_backface_culling		 = inc()
geo_mirror_mario_set_alpha		 = inc()
geo_move_mario_part_from_parent		 = inc()
geo_movtex_draw_colored		 = inc()
geo_movtex_draw_colored_2_no_update		 = inc()
geo_movtex_draw_colored_no_update		 = inc()
geo_movtex_draw_nocolor		 = inc()
geo_movtex_draw_water_regions		 = inc()
geo_movtex_pause_control		 = inc()
geo_movtex_update_horizontal		 = inc()
geo_render_mirror_mario		 = inc()
geo_switch_anim_state		 = inc()
geo_switch_area		 = inc()
geo_switch_bowser_eyes		 = inc()
geo_switch_mario_cap_effect		 = inc()
geo_switch_mario_cap_on_off		 = inc()
geo_switch_mario_eyes		 = inc()
geo_switch_mario_hand		 = inc()
geo_switch_mario_hand_grab_pos		 = inc()
geo_switch_mario_stand_run		 = inc()
geo_switch_peach_eyes		 = inc()
geo_switch_tuxie_mother_eyes		 = inc()
geo_update_layer_transparency		 = inc()
geo_update_projectile_pos_from_parent		 = inc()
geo_wdw_set_initial_water_level		 = inc()
Geo18_802764B0		 = inc()
Geo18_802A45E4		 = inc()
Geo18_802A719C		 = inc()
Geo18_802B798C		 = inc()
Geo18_802B7D44		 = inc()
Geo18_802BA2B0		 = inc()
Geo18_802D5B98		 = inc()
Geo18_802D5D0C		 = inc()
Geo18_8030D93C		 = inc()
Geo18_8030D9AC		 = inc()
geo18_fade_transition		 = inc()
geo18_game_over_tile		 = inc()
geo18_intro_backdrop		 = inc()
geo18_title_screen		 = inc()

geo_bits_bowser_coloring = Geo18_802B7D44
geo_update_body_rot_from_parent = Geo18_802B798C
geo_scale_bowser_key = Geo18_802BA2B0
geo_snufit_move_mask = Geo18_8030D93C
geo_snufit_scale_body = Geo18_8030D9AC

geo_update_held_mario_pos = geo_update_projectile_pos_from_parent
#geo_update_held_mario_pos = 
#geo_offset_klepto_debug = Unknown802A3E84
geo_offset_klepto_held_object = Geo18_802A45E4


OBJ_LIST_PLAYER			= 0
OBJ_LIST_UNUSED_1		= 1
OBJ_LIST_DESTRUCTIVE	= 2
OBJ_LIST_UNUSED_3		= 3
OBJ_LIST_GENACTOR		= 4
OBJ_LIST_PUSHABLE		= 5

OBJ_LIST_LEVEL			= 6
OBJ_LIST_UNUSED_7		= 7
OBJ_LIST_DEFAULT		= 8
OBJ_LIST_SURFACE		= 9
OBJ_LIST_POLELIKE		= 10
OBJ_LIST_SPAWNER		= 11
OBJ_LIST_UNIMPORTANT	= 12
NUM_OBJ_LISTS			= 13

def OBJECT_FIELD_U32(index):
	return index
def OBJECT_FIELD_S32(index):
	return index
def OBJECT_FIELD_S16(index, subIndex):
	return index
def OBJECT_FIELD_F32(index):
	return index
def OBJECT_FIELD_S16P(index):
	return index
def OBJECT_FIELD_S32P(index):
	return index
def OBJECT_FIELD_ANIMS(index):
	return index
def OBJECT_FIELD_WAYPOINT(index):
	return index
def OBJECT_FIELD_CHAIN_SEGMENT(index):
	return index
def OBJECT_FIELD_OBJ(index):
	return index
def OBJECT_FIELD_SURFACE(index):
	return index
def OBJECT_FIELD_VPTR(index):
	return index
def OBJECT_FIELD_CVPTR(index):
	return index
	
OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE         = (1 <<  0) 
OBJ_FLAG_MOVE_XZ_USING_FVEL               = (1 <<  1) 
OBJ_FLAG_MOVE_Y_WITH_TERMINAL_VEL         = (1 <<  2) 
OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW         = (1 <<  3) 
OBJ_FLAG_0010                             = (1 <<  4) 
OBJ_FLAG_0020                             = (1 <<  5) 
OBJ_FLAG_COMPUTE_DIST_TO_MARIO            = (1 <<  6) 
OBJ_FLAG_ACTIVE_FROM_AFAR                 = (1 <<  7) 
OBJ_FLAG_0100                             = (1 <<  8) 
OBJ_FLAG_TRANSFORM_RELATIVE_TO_PARENT     = (1 <<  9) 
OBJ_FLAG_HOLDABLE                         = (1 << 10) 
OBJ_FLAG_0800                             = (1 << 11) 
OBJ_FLAG_1000                             = (1 << 12) 
OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO           = (1 << 13) 
OBJ_FLAG_PERSISTENT_RESPAWN               = (1 << 14) 
OBJ_FLAG_8000                             = (1 << 15) 
OBJ_FLAG_30                               = (1 << 30) 
	
INTERACT_HOOT           =  (1 <<  0)
INTERACT_GRABBABLE      =  (1 <<  1)
INTERACT_DOOR           =  (1 <<  2)
INTERACT_DAMAGE         =  (1 <<  3)
INTERACT_COIN           =  (1 <<  4)
INTERACT_CAP            =  (1 <<  5)
INTERACT_POLE           =  (1 <<  6)
INTERACT_KOOPA          =  (1 <<  7)
INTERACT_UNKNOWN_08     =  (1 <<  8)
INTERACT_BREAKABLE      =  (1 <<  9)
INTERACT_WIND           =  (1 << 10)
INTERACT_WARP_DOOR      =  (1 << 11)
INTERACT_STAR_OR_KEY    =  (1 << 12)
INTERACT_WARP           =  (1 << 13)
INTERACT_CANNON_BASE    =  (1 << 14)
INTERACT_BOUNCE_TOP     =  (1 << 15)
INTERACT_WATER_RING     =  (1 << 16)
INTERACT_BULLY          =  (1 << 17)
INTERACT_FLAME          =  (1 << 18)
INTERACT_KOOPA_SHELL    =  (1 << 19)
INTERACT_TRAP_TURN      =  (1 << 20)
INTERACT_MR_BLIZZARD    =  (1 << 21)
INTERACT_HIT_FROM_BELOW =  (1 << 22)
INTERACT_TEXT           =  (1 << 23)
INTERACT_TORNADO        =  (1 << 24)
INTERACT_WHIRLPOOL      =  (1 << 25)
INTERACT_CLAM_OR_BUBBA  =  (1 << 26)
INTERACT_BBH_ENTRANCE   =  (1 << 27)
INTERACT_SNUFIT_BULLET  =  (1 << 28)
INTERACT_SHOCK          =  (1 << 29)
INTERACT_IGLOO_BARRIER  =  (1 << 30)
INTERACT_UNKNOWN_31     =  (1 << 31)

oFlags                       = OBJECT_FIELD_U32(0x01)
oDialogResponse              = OBJECT_FIELD_S16(0x02, 0)
oDialogState                 = OBJECT_FIELD_S16(0x02, 1)
oUnk94                       = OBJECT_FIELD_U32(0x03)

oIntangibleTimer             = OBJECT_FIELD_S32(0x05)
O_POS_INDEX                  = 0x06
oPosX                        = OBJECT_FIELD_F32(O_POS_INDEX + 0)
oPosY                        = OBJECT_FIELD_F32(O_POS_INDEX + 1)
oPosZ                        = OBJECT_FIELD_F32(O_POS_INDEX + 2)
oVelX                        = OBJECT_FIELD_F32(0x09)
oVelY                        = OBJECT_FIELD_F32(0x0A)
oVelZ                        = OBJECT_FIELD_F32(0x0B)
oForwardVel                  = OBJECT_FIELD_F32(0x0C)
oForwardVelS32               = OBJECT_FIELD_S32(0x0C)
oUnkBC                       = OBJECT_FIELD_F32(0x0D)
oUnkC0                       = OBJECT_FIELD_F32(0x0E)
O_MOVE_ANGLE_INDEX           = 0x0F
O_MOVE_ANGLE_PITCH_INDEX     = (O_MOVE_ANGLE_INDEX + 0)
O_MOVE_ANGLE_YAW_INDEX       = (O_MOVE_ANGLE_INDEX + 1)
O_MOVE_ANGLE_ROLL_INDEX      = (O_MOVE_ANGLE_INDEX + 2)
oMoveAnglePitch              = OBJECT_FIELD_S32(O_MOVE_ANGLE_PITCH_INDEX)
oMoveAngleYaw                = OBJECT_FIELD_S32(O_MOVE_ANGLE_YAW_INDEX)
oMoveAngleRoll               = OBJECT_FIELD_S32(O_MOVE_ANGLE_ROLL_INDEX)
O_FACE_ANGLE_INDEX           = 0x12
O_FACE_ANGLE_PITCH_INDEX     = (O_FACE_ANGLE_INDEX + 0)
O_FACE_ANGLE_YAW_INDEX       = (O_FACE_ANGLE_INDEX + 1)
O_FACE_ANGLE_ROLL_INDEX      = (O_FACE_ANGLE_INDEX + 2)
oFaceAnglePitch              = OBJECT_FIELD_S32(O_FACE_ANGLE_PITCH_INDEX)
oFaceAngleYaw                = OBJECT_FIELD_S32(O_FACE_ANGLE_YAW_INDEX)
oFaceAngleRoll               = OBJECT_FIELD_S32(O_FACE_ANGLE_ROLL_INDEX)
oGraphYOffset                = OBJECT_FIELD_F32(0x15)
oActiveParticleFlags         = OBJECT_FIELD_U32(0x16)
oGravity                     = OBJECT_FIELD_F32(0x17)
oFloorHeight                 = OBJECT_FIELD_F32(0x18)
oMoveFlags                   = OBJECT_FIELD_U32(0x19)
oAnimState                   = OBJECT_FIELD_S32(0x1A)

oAngleVelPitch               = OBJECT_FIELD_S32(0x23)
oAngleVelYaw                 = OBJECT_FIELD_S32(0x24)
oAngleVelRoll                = OBJECT_FIELD_S32(0x25)
oAnimations                  = OBJECT_FIELD_ANIMS(0x26)
oHeldState                   = OBJECT_FIELD_U32(0x27)
oWallHitboxRadius            = OBJECT_FIELD_F32(0x28)
oDragStrength                = OBJECT_FIELD_F32(0x29)
oInteractType                = OBJECT_FIELD_U32(0x2A)
oInteractStatus              = OBJECT_FIELD_S32(0x2B)
O_PARENT_RELATIVE_POS_INDEX  = 0x2C
oParentRelativePosX          = OBJECT_FIELD_F32(O_PARENT_RELATIVE_POS_INDEX + 0)
oParentRelativePosY          = OBJECT_FIELD_F32(O_PARENT_RELATIVE_POS_INDEX + 1)
oParentRelativePosZ          = OBJECT_FIELD_F32(O_PARENT_RELATIVE_POS_INDEX + 2)
oBehParams2ndByte            = OBJECT_FIELD_S32(0x2F)

oAction                      = OBJECT_FIELD_S32(0x31)
oSubAction                   = OBJECT_FIELD_S32(0x32)
oTimer                       = OBJECT_FIELD_S32(0x33)
oBounce                      = OBJECT_FIELD_F32(0x34)
oDistanceToMario             = OBJECT_FIELD_F32(0x35)
oAngleToMario                = OBJECT_FIELD_S32(0x36)
oHomeX                       = OBJECT_FIELD_F32(0x37)
oHomeY                       = OBJECT_FIELD_F32(0x38)
oHomeZ                       = OBJECT_FIELD_F32(0x39)
oFriction                    = OBJECT_FIELD_F32(0x3A)
oBuoyancy                    = OBJECT_FIELD_F32(0x3B)
oSoundStateID                = OBJECT_FIELD_S32(0x3C)
oOpacity                     = OBJECT_FIELD_S32(0x3D)
oDamageOrCoinValue           = OBJECT_FIELD_S32(0x3E)
oHealth                      = OBJECT_FIELD_S32(0x3F)
oBehParams                   = OBJECT_FIELD_S32(0x40)
oPrevAction                  = OBJECT_FIELD_S32(0x41)
oInteractionSubtype          = OBJECT_FIELD_U32(0x42)
oCollisionDistance           = OBJECT_FIELD_F32(0x43)
oNumLootCoins                = OBJECT_FIELD_S32(0x44)
oDrawingDistance             = OBJECT_FIELD_F32(0x45)
oRoom                        = OBJECT_FIELD_S32(0x46)

oUnk1A8                      = OBJECT_FIELD_U32(0x48)

oWallAngle                   = OBJECT_FIELD_U32(0x4B)
oFloorType                   = OBJECT_FIELD_S16(0x4C, 0)
oFloorRoom                   = OBJECT_FIELD_S16(0x4C, 1)
oUnk1BC                      = OBJECT_FIELD_S32(0x4D)
oFloor                       = OBJECT_FIELD_SURFACE(0x4E)
oDeathSound                  = OBJECT_FIELD_S32(0x4F)

oPathedWaypointsS16       = OBJECT_FIELD_VPTR(0x1D)
oPathedStartWaypoint      = OBJECT_FIELD_WAYPOINT(0x1D)
oPathedPrevWaypoint       = OBJECT_FIELD_WAYPOINT(0x1E)
oPathedPrevWaypointFlags  = OBJECT_FIELD_S32(0x1F)
oPathedTargetPitch        = OBJECT_FIELD_S32(0x20)
oPathedTargetYaw          = OBJECT_FIELD_S32(0x21)
oMacroUnk108  = OBJECT_FIELD_F32(0x20)
oMacroUnk10C  = OBJECT_FIELD_F32(0x21)
oMacroUnk110  = OBJECT_FIELD_F32(0x22)
oMarioParticleFlags     = OBJECT_FIELD_S32(0x1B)
oMarioPoleUnk108        = OBJECT_FIELD_S32(0x20)
oMarioReadingSignDYaw   = OBJECT_FIELD_S32(0x20)
oMarioPoleYawVel        = OBJECT_FIELD_S32(0x21)
oMarioCannonObjectYaw   = OBJECT_FIELD_S32(0x21)
oMarioTornadoYawVel     = OBJECT_FIELD_S32(0x21)
oMarioReadingSignDPosX  = OBJECT_FIELD_F32(0x21)
oMarioPolePos           = OBJECT_FIELD_F32(0x22)
oMarioCannonInputYaw    = OBJECT_FIELD_S32(0x22)
oMarioTornadoPosY       = OBJECT_FIELD_F32(0x22)
oMarioReadingSignDPosZ  = OBJECT_FIELD_F32(0x22)
oMarioWhirlpoolPosY     = OBJECT_FIELD_F32(0x22)
oMarioBurnTimer         = OBJECT_FIELD_S32(0x22)
oMarioLongJumpIsSlow    = OBJECT_FIELD_S32(0x22)
oMarioSteepJumpYaw      = OBJECT_FIELD_S32(0x22)
oMarioWalkingPitch      = OBJECT_FIELD_S32(0x22)
o1UpHiddenUnkF4  = OBJECT_FIELD_S32(0x1B)
oActivatedBackAndForthPlatformMaxOffset     = OBJECT_FIELD_F32(0x1B)
oActivatedBackAndForthPlatformOffset        = OBJECT_FIELD_F32(0x1C)
oActivatedBackAndForthPlatformVel           = OBJECT_FIELD_F32(0x1D)
oActivatedBackAndForthPlatformCountdown     = OBJECT_FIELD_S32(0x1E)
oActivatedBackAndForthPlatformStartYaw      = OBJECT_FIELD_S32(0x1F)
oActivatedBackAndForthPlatformVertical      = OBJECT_FIELD_S32(0x20)
oActivatedBackAndForthPlatformFlipRotation  = OBJECT_FIELD_S32(0x21)
oAmpRadiusOfRotation  = OBJECT_FIELD_F32(0x1B)
oAmpYPhase            = OBJECT_FIELD_S32(0x1C)
oHomingAmpLockedOn  = OBJECT_FIELD_S32(0x1B)
oHomingAmpAvgY      = OBJECT_FIELD_F32(0x1D)
oArrowLiftDisplacement        = OBJECT_FIELD_F32(0x1B)
oArrowLiftUnk100              = OBJECT_FIELD_S32(0x1E)
oBackAndForthPlatformUnkF4   = OBJECT_FIELD_F32(0x1B)
oBackAndForthPlatformUnkF8   = OBJECT_FIELD_F32(0x1C)
oBackAndForthPlatformUnkFC   = OBJECT_FIELD_F32(0x1D)
oBackAndForthPlatformUnk100  = OBJECT_FIELD_F32(0x1E)
oBirdSpeed        = OBJECT_FIELD_F32(0x1B)
oBirdTargetPitch  = OBJECT_FIELD_S32(0x1C)
oBirdTargetYaw    = OBJECT_FIELD_S32(0x1D)
oBirdChirpChirpUnkF4  = OBJECT_FIELD_S32(0x1B)
oEndBirdUnk104  = OBJECT_FIELD_F32(0x1F)
oHiddenBlueCoinSwitch  = OBJECT_FIELD_OBJ(0x1C)
oBobombBlinkTimer  = OBJECT_FIELD_S32(0x1B)
oBobombFuseLit     = OBJECT_FIELD_S32(0x1C)
oBobombFuseTimer   = OBJECT_FIELD_S32(0x1D)
oBobombBuddyBlinkTimer        = OBJECT_FIELD_S32(0x1B)
oBobombBuddyHasTalkedToMario  = OBJECT_FIELD_S32(0x1C)
oBobombBuddyRole              = OBJECT_FIELD_S32(0x1D)
oBobombBuddyCannonStatus      = OBJECT_FIELD_S32(0x1E)
oBobombBuddyPosXCopy          = OBJECT_FIELD_F32(0x20)
oBobombBuddyPosYCopy          = OBJECT_FIELD_F32(0x21)
oBobombBuddyPosZCopy          = OBJECT_FIELD_F32(0x22)
oBobombExpBubGfxScaleFacX  = OBJECT_FIELD_S32(0x1D)
oBobombExpBubGfxScaleFacY  = OBJECT_FIELD_S32(0x1E)
oBobombExpBubGfxExpRateX   = OBJECT_FIELD_S32(0x1F)
oBobombExpBubGfxExpRateY   = OBJECT_FIELD_S32(0x20)
oSmallBompInitX  = OBJECT_FIELD_F32(0x1E)
oBooDeathStatus            = OBJECT_FIELD_S32(0x00)
oBooTargetOpacity          = OBJECT_FIELD_S32(0x1B)
oBooBaseScale              = OBJECT_FIELD_F32(0x1C)
oBooOscillationTimer       = OBJECT_FIELD_S32(0x1D)
oBooMoveYawDuringHit       = OBJECT_FIELD_S32(0x1E)
oBooMoveYawBeforeHit       = OBJECT_FIELD_F32(0x1F)
oBooParentBigBoo           = OBJECT_FIELD_OBJ(0x20)
oBooNegatedAggressiveness  = OBJECT_FIELD_F32(0x21)
oBooInitialMoveYaw         = OBJECT_FIELD_S32(0x22)
oBooTurningSpeed           = OBJECT_FIELD_S16(0x4A, 0)
oBigBooNumMinionBoosKilled  = OBJECT_FIELD_S32(0x49)
oBookendUnkF4  = OBJECT_FIELD_S32(0x1B)
oBookendUnkF8  = OBJECT_FIELD_S32(0x1C)
oBookSwitchUnkF4  = OBJECT_FIELD_F32(0x1B)
oBookSwitchManagerUnkF4  = OBJECT_FIELD_S32(0x1B)
oBookSwitchManagerUnkF8  = OBJECT_FIELD_S32(0x1C)
oHauntedBookshelfShouldOpen  = OBJECT_FIELD_S32(0x00)
oBouncingFireBallUnkF4  = OBJECT_FIELD_S32(0x1B)
oBowlingBallTargetYaw  = OBJECT_FIELD_S32(0x1B)

oBBallSpawnerMaxSpawnDist  = OBJECT_FIELD_F32(0x1B)
oBBallSpawnerSpawnOdds     = OBJECT_FIELD_F32(0x1C)
oBBallSpawnerPeriodMinus1  = OBJECT_FIELD_S32(0x1D)
oBowserUnk88            = OBJECT_FIELD_S32(0x00)
oBowserUnkF4            = OBJECT_FIELD_S32(0x1B)
oBowserUnkF8            = OBJECT_FIELD_S32(0x1C)
oBowserDistToCentre     = OBJECT_FIELD_F32(0x1D)
oBowserUnk106           = OBJECT_FIELD_S16(0x1F, 1)
oBowserUnk108           = OBJECT_FIELD_S16(0x20, 0)
oBowserHeldAnglePitch   = OBJECT_FIELD_S16(0x20, 1)
oBowserHeldAngleVelYaw  = OBJECT_FIELD_S16(0x21, 0)
oBowserUnk10E           = OBJECT_FIELD_S16(0x21, 1)
oBowserUnk110           = OBJECT_FIELD_S16(0x22, 0)
oBowserAngleToCentre    = OBJECT_FIELD_S16(0x22, 1)
oBowserUnk1AC           = OBJECT_FIELD_S16(0x49, 0)
oBowserUnk1AE           = OBJECT_FIELD_S16(0x49, 1)
oBowserEyesShut         = OBJECT_FIELD_S16(0x4A, 0)
oBowserUnk1B2           = OBJECT_FIELD_S16(0x4A, 1)
oBowserShockWaveUnkF4  = OBJECT_FIELD_F32(0x1B)
oBlackSmokeBowserUnkF4  = OBJECT_FIELD_F32(0x1B)
oBowserKeyScale  = OBJECT_FIELD_F32(0x1B)
oBowserPuzzleCompletionFlags  = OBJECT_FIELD_S32(0x1B)
oBowserPuzzlePieceOffsetX                   = OBJECT_FIELD_F32(0x1D)
oBowserPuzzlePieceOffsetY                   = OBJECT_FIELD_F32(0x1E)
oBowserPuzzlePieceOffsetZ                   = OBJECT_FIELD_F32(0x1F)
oBowserPuzzlePieceContinuePerformingAction  = OBJECT_FIELD_S32(0x20)
oBowserPuzzlePieceActionList                = OBJECT_FIELD_VPTR(0x21)
oBowserPuzzlePieceNextAction                = OBJECT_FIELD_VPTR(0x22)
oBubbaUnkF4   = OBJECT_FIELD_F32(0x1B)
oBubbaUnkF8   = OBJECT_FIELD_S32(0x1C)
oBubbaUnkFC   = OBJECT_FIELD_S32(0x1D)
oBubbaUnk100  = OBJECT_FIELD_S32(0x1E)
oBubbaUnk104  = OBJECT_FIELD_S32(0x1F)
oBubbaUnk108  = OBJECT_FIELD_F32(0x20)
oBubbaUnk10C  = OBJECT_FIELD_F32(0x21)
oBubbaUnk1AC  = OBJECT_FIELD_S16(0x49, 0)
oBubbaUnk1AE  = OBJECT_FIELD_S16(0x49, + 1)
oBubbaUnk1B0  = OBJECT_FIELD_S16(0x4A, 0)
oBubbaUnk1B2  = OBJECT_FIELD_S16(0x4A, + 1)
oBulletBillUnkF8  = OBJECT_FIELD_S32(0x1C)
oBullySubtype                    = OBJECT_FIELD_S32(0x1B)
oBullyPrevX                      = OBJECT_FIELD_F32(0x1C)
oBullyPrevY                      = OBJECT_FIELD_F32(0x1D)
oBullyPrevZ                      = OBJECT_FIELD_F32(0x1E)
oBullyKBTimerAndMinionKOCounter  = OBJECT_FIELD_S32(0x1F)
oBullyMarioCollisionAngle        = OBJECT_FIELD_S32(0x20)
oButterflyYPhase  = OBJECT_FIELD_S32(0x1B)
oTripletButterflyScale              = OBJECT_FIELD_F32(0x1B)
oTripletButterflySpeed              = OBJECT_FIELD_F32(0x1C)
oTripletButterflyBaseYaw            = OBJECT_FIELD_F32(0x1D)
oTripletButterflyTargetPitch        = OBJECT_FIELD_S32(0x1E)
oTripletButterflyTargetYaw          = OBJECT_FIELD_S32(0x1F)
oTripletButterflyType               = OBJECT_FIELD_S32(0x20)
oTripletButterflyModel              = OBJECT_FIELD_S32(0x21)
oTripletButterflySelectedButterfly  = OBJECT_FIELD_S32(0x22)
oTripletButterflyScalePhase         = OBJECT_FIELD_S32(0x49)
oCannonUnkF4   = OBJECT_FIELD_S32(0x1B)
oCannonUnkF8   = OBJECT_FIELD_S32(0x1C)
oCannonUnk10C  = OBJECT_FIELD_S32(0x21)
oCapUnkF4  = OBJECT_FIELD_S32(0x1B)
oCapUnkF8  = OBJECT_FIELD_S32(0x1C)
oChainChompSegments                      = OBJECT_FIELD_CHAIN_SEGMENT(0x1B)
oChainChompMaxDistFromPivotPerChainPart  = OBJECT_FIELD_F32(0x1C)
oChainChompMaxDistBetweenChainParts      = OBJECT_FIELD_F32(0x1D)
oChainChompDistToPivot                   = OBJECT_FIELD_F32(0x1E)
oChainChompUnk104                        = OBJECT_FIELD_F32(0x1F)
oChainChompRestrictedByChain             = OBJECT_FIELD_S32(0x20)
oChainChompTargetPitch                   = OBJECT_FIELD_S32(0x21)
oChainChompNumLunges                     = OBJECT_FIELD_S32(0x22)
oChainChompReleaseStatus                 = OBJECT_FIELD_S32(0x49)
oChainChompHitGate                       = OBJECT_FIELD_S32(0x4A)
oCheckerBoardPlatformUnkF8   = OBJECT_FIELD_S32(0x1C) 
oCheckerBoardPlatformUnkFC   = OBJECT_FIELD_S32(0x1D)
oCheckerBoardPlatformUnk1AC  = OBJECT_FIELD_F32(0x49)
oCheepCheepUnkF4   = OBJECT_FIELD_F32(0x1B)
oCheepCheepUnkF8   = OBJECT_FIELD_F32(0x1C)
oCheepCheepUnkFC   = OBJECT_FIELD_F32(0x1D)
oCheepCheepUnk104  = OBJECT_FIELD_F32(0x1F)
oCheepCheepUnk108  = OBJECT_FIELD_F32(0x20)
oChuckyaUnk88   = OBJECT_FIELD_S32(0x00)
oChuckyaUnkF8   = OBJECT_FIELD_S32(0x1C)
oChuckyaUnkFC   = OBJECT_FIELD_S32(0x1D)
oChuckyaUnk100  = OBJECT_FIELD_S32(0x1E)
oClamUnkF4  = OBJECT_FIELD_S32(0x1B)
oCloudCenterX               = OBJECT_FIELD_F32(0x1B)
oCloudCenterY               = OBJECT_FIELD_F32(0x1C)
oCloudBlowing               = OBJECT_FIELD_S32(0x1D)
oCloudGrowSpeed             = OBJECT_FIELD_F32(0x1E)
oCloudFwooshMovementRadius  = OBJECT_FIELD_S16(0x49, 0)
oCoinUnkF4   = OBJECT_FIELD_S32(0x1B)
oCoinUnkF8   = OBJECT_FIELD_S32(0x1C)
oCoinUnk110  = OBJECT_FIELD_F32(0x22)
oCoinUnk1B0  = OBJECT_FIELD_S32(0x4A)
oCollisionParticleUnkF4		= OBJECT_FIELD_F32(0x1B)
oControllablePlatformUnkF8   = OBJECT_FIELD_S32(0x1C)
oControllablePlatformUnkFC   = OBJECT_FIELD_F32(0x1D)
oControllablePlatformUnk100  = OBJECT_FIELD_S32(0x1E)
oBreakableBoxSmallReleased             = OBJECT_FIELD_S32(0x1B)
oBreakableBoxSmallFramesSinceReleased  = OBJECT_FIELD_S32(0x1D)
oJumpingBoxUnkF4  = OBJECT_FIELD_S32(0x1B)
oJumpingBoxUnkF8  = OBJECT_FIELD_S32(0x1C)
oRRCruiserWingUnkF4  = OBJECT_FIELD_S32(0x1B)
oRRCruiserWingUnkF8  = OBJECT_FIELD_S32(0x1C)
oDonutPlatformSpawnerSpawnedPlatforms  = OBJECT_FIELD_S32(0x1B)
oDoorUnk88   = OBJECT_FIELD_S32(0x00)
oDoorUnkF8   = OBJECT_FIELD_S32(0x1C)
oDoorUnkFC   = OBJECT_FIELD_S32(0x1D)
oDoorUnk100  = OBJECT_FIELD_S32(0x1E)
oDorrieDistToHome          = OBJECT_FIELD_F32(0x1B)
oDorrieOffsetY             = OBJECT_FIELD_F32(0x1C)
oDorrieVelY                = OBJECT_FIELD_F32(0x1D)
oDorrieForwardDistToMario  = OBJECT_FIELD_F32(0x1E)
oDorrieYawVel              = OBJECT_FIELD_S32(0x1F)
oDorrieLiftingMario        = OBJECT_FIELD_S32(0x21)
oDorrieGroundPounded       = OBJECT_FIELD_S16(0x49, 0)
oDorrieAngleToHome         = OBJECT_FIELD_S16(0x49, + 1)
oDorrieNeckAngle           = OBJECT_FIELD_S16(0x4A, 0)
oDorrieHeadRaiseSpeed      = OBJECT_FIELD_S16(0x4A, + 1)
oElevatorUnkF4   = OBJECT_FIELD_F32(0x1B)
oElevatorUnkF8   = OBJECT_FIELD_F32(0x1C)
oElevatorUnkFC   = OBJECT_FIELD_F32(0x1D)
oElevatorUnk100  = OBJECT_FIELD_S32(0x1E)
oExclamationBoxUnkF4  = OBJECT_FIELD_F32(0x1B) 
oExclamationBoxUnkF8  = OBJECT_FIELD_F32(0x1C) 
oExclamationBoxUnkFC  = OBJECT_FIELD_S32(0x1D) 
oEyerokBossNumHands    = OBJECT_FIELD_S32(0x1C)
oEyerokBossUnkFC       = OBJECT_FIELD_S32(0x1D)
oEyerokBossActiveHand  = OBJECT_FIELD_S32(0x1E)
oEyerokBossUnk104      = OBJECT_FIELD_S32(0x1F)
oEyerokBossUnk108      = OBJECT_FIELD_F32(0x20)
oEyerokBossUnk10C      = OBJECT_FIELD_F32(0x21)
oEyerokBossUnk110      = OBJECT_FIELD_F32(0x22)
oEyerokBossUnk1AC      = OBJECT_FIELD_S32(0x49)
oEyerokHandWakeUpTimer  = OBJECT_FIELD_S32(0x1B)
oEyerokReceivedAttack   = OBJECT_FIELD_S32(0x1C)
oEyerokHandUnkFC        = OBJECT_FIELD_S32(0x1D)
oEyerokHandUnk100       = OBJECT_FIELD_S32(0x1E)
oFallingPillarPitchAcceleration  = OBJECT_FIELD_F32(0x1B)
oFireSpitterScaleVel  = OBJECT_FIELD_F32(0x1B)
oBlueFishUnkF4   = OBJECT_FIELD_F32(0x1B)
oBlueFishUnkF8   = OBJECT_FIELD_S32(0x1C)
oBlueFishUnk100  = OBJECT_FIELD_F32(0x1E)
oFishGroupUnkF4   = OBJECT_FIELD_F32(0x1B)
oFishGroupUnkF8   = OBJECT_FIELD_F32(0x1C)
oFishGroupUnkFC   = OBJECT_FIELD_F32(0x1D)
oFishGroupUnk100  = OBJECT_FIELD_S32(0x1E)
oFishGroupUnk104  = OBJECT_FIELD_F32(0x1F)
oFishGroupUnk108  = OBJECT_FIELD_F32(0x20)
oFishGroupUnk10C  = OBJECT_FIELD_F32(0x21)
oFishGroupUnk110  = OBJECT_FIELD_F32(0x22)
oFlameUnkF4   = OBJECT_FIELD_F32(0x1B)
oFlameUnkF8   = OBJECT_FIELD_S32(0x1C)
oFlameUnkFC   = OBJECT_FIELD_F32(0x1D)
oFlameUnk100  = OBJECT_FIELD_OBJ(0x1E)
oBlueFlameUnkF8  = OBJECT_FIELD_F32(0x1C)
oSmallPiranhaFlameUnkF4   = OBJECT_FIELD_F32(0x1B)
oSmallPiranhaFlameUnkF8   = OBJECT_FIELD_F32(0x1C)
oSmallPiranhaFlameUnkFC   = OBJECT_FIELD_S32(0x1D)
oSmallPiranhaFlameUnk100  = OBJECT_FIELD_S32(0x1E)
oSmallPiranhaFlameUnk104  = OBJECT_FIELD_F32(0x1F)
oMovingFlameTimer  = OBJECT_FIELD_S32(0x1B)
oFlameThowerFlameUnk110  = OBJECT_FIELD_S32(0x22)
oFlameThowerUnk110  = OBJECT_FIELD_S32(0x22)
oFloatingPlatformUnkF4   = OBJECT_FIELD_S32(0x1B)
oFloatingPlatformUnkF8   = OBJECT_FIELD_F32(0x1C)
oFloatingPlatformUnkFC   = OBJECT_FIELD_F32(0x1D)
oFloatingPlatformUnk100   = OBJECT_FIELD_S32(0x1E)
oFloorSwitchPressAnimationUnkF4   = OBJECT_FIELD_S32(0x1B)
oFloorSwitchPressAnimationUnkF8   = OBJECT_FIELD_S32(0x1C)
oFloorSwitchPressAnimationUnkFC   = OBJECT_FIELD_S32(0x1D)
oFloorSwitchPressAnimationUnk100  = OBJECT_FIELD_S32(0x1E)
oFlyGuyIdleTimer         = OBJECT_FIELD_S32(0x1B)
oFlyGuyOscTimer          = OBJECT_FIELD_S32(0x1C)
oFlyGuyUnusedJitter      = OBJECT_FIELD_S32(0x1D)
oFlyGuyLungeYDecel       = OBJECT_FIELD_F32(0x1E)
oFlyGuyLungeTargetPitch  = OBJECT_FIELD_S32(0x1F)
oFlyGuyTargetRoll        = OBJECT_FIELD_S32(0x20)
oFlyGuyScaleVel          = OBJECT_FIELD_F32(0x21)
oGrandStarUnk108  = OBJECT_FIELD_S32(0x20)
oHorizontalGrindelTargetYaw   = OBJECT_FIELD_S32(0x1B)
oHorizontalGrindelDistToHome  = OBJECT_FIELD_F32(0x1C)
oHorizontalGrindelOnGround    = OBJECT_FIELD_S32(0x1D)
oGoombaSize                 = OBJECT_FIELD_S32(0x1B)
oGoombaScale                = OBJECT_FIELD_F32(0x1C)
oGoombaWalkTimer            = OBJECT_FIELD_S32(0x1D)
oGoombaTargetYaw            = OBJECT_FIELD_S32(0x1E)
oGoombaBlinkTimer           = OBJECT_FIELD_S32(0x1F)
oGoombaTurningAwayFromWall  = OBJECT_FIELD_S32(0x20)
oGoombaRelativeSpeed        = OBJECT_FIELD_F32(0x21)
oHauntedChairUnkF4   = OBJECT_FIELD_S32(0x1B)
oHauntedChairUnkF8   = OBJECT_FIELD_F32(0x1C)
oHauntedChairUnkFC   = OBJECT_FIELD_F32(0x1D)
oHauntedChairUnk100  = OBJECT_FIELD_S32P(0x1E)
oHauntedChairUnk104  = OBJECT_FIELD_S32(0x1F)
oHeaveHoUnk88  = OBJECT_FIELD_S32(0x00)
oHeaveHoUnkF4  = OBJECT_FIELD_F32(0x1B)
oPurpleSwitch  = OBJECT_FIELD_OBJ(0x1B)
oHootAvailability      = OBJECT_FIELD_S32(0x1B)
oHootMarioReleaseTime  = OBJECT_FIELD_S32(0x22)
oHorizontalMovementUnkF4   = OBJECT_FIELD_S32(0x1B)
oHorizontalMovementUnkF8   = OBJECT_FIELD_S32(0x1C)
oHorizontalMovementUnk100  = OBJECT_FIELD_F32(0x1E)
oHorizontalMovementUnk104  = OBJECT_FIELD_S32(0x1F)
oHorizontalMovementUnk108  = OBJECT_FIELD_F32(0x20)
oKickableBoardF4  = OBJECT_FIELD_S32(0x1B)
oKickableBoardF8  = OBJECT_FIELD_S32(0x1C)
oKingBobombUnk88   = OBJECT_FIELD_S32(0x00)
oKingBobombUnkF8   = OBJECT_FIELD_S32(0x1C)
oKingBobombUnkFC   = OBJECT_FIELD_S32(0x1D)
oKingBobombUnk100  = OBJECT_FIELD_S32(0x1E)
oKingBobombUnk104  = OBJECT_FIELD_S32(0x1F)
oKingBobombUnk108  = OBJECT_FIELD_S32(0x20)
oKleptoDistanceToTarget       = OBJECT_FIELD_F32(0x1B)
oKleptoUnkF8                  = OBJECT_FIELD_F32(0x1C)
oKleptoUnkFC                  = OBJECT_FIELD_F32(0x1D)
oKleptoSpeed                  = OBJECT_FIELD_F32(0x1E)
oKleptoStartPosX              = OBJECT_FIELD_F32(0x1F)
oKleptoStartPosY              = OBJECT_FIELD_F32(0x20)
oKleptoStartPosZ              = OBJECT_FIELD_F32(0x21)
oKleptoTimeUntilTargetChange  = OBJECT_FIELD_S32(0x22)
oKleptoTargetNumber           = OBJECT_FIELD_S16(0x49, 0)
oKleptoUnkTimer                 = OBJECT_FIELD_S16(0x49, + 1)
oKleptoPitch                 = OBJECT_FIELD_S16(0x4A, 0)
oKleptoYawToTarget            = OBJECT_FIELD_S16(0x4A, + 1)
oKoopaAgility                      = OBJECT_FIELD_F32(0x1B)
oKoopaMovementType                 = OBJECT_FIELD_S32(0x1C)
oKoopaTargetYaw                    = OBJECT_FIELD_S32(0x1D)
oKoopaUnshelledTimeUntilTurn       = OBJECT_FIELD_S32(0x1E)
oKoopaTurningAwayFromWall          = OBJECT_FIELD_S32(0x1F)
oKoopaDistanceToMario              = OBJECT_FIELD_F32(0x20)
oKoopaAngleToMario                 = OBJECT_FIELD_S32(0x21)
oKoopaBlinkTimer                   = OBJECT_FIELD_S32(0x22)
oKoopaCountdown                    = OBJECT_FIELD_S16(0x49, 0)
oKoopaTheQuickRaceIndex            = OBJECT_FIELD_S16(0x49, 1)
oKoopaTheQuickInitTextboxCooldown  = OBJECT_FIELD_S16(0x4A, 0)

oKoopaRaceEndpointRaceBegun      = OBJECT_FIELD_S32(0x1B)
oKoopaRaceEndpointKoopaFinished  = OBJECT_FIELD_S32(0x1C)
oKoopaRaceEndpointRaceStatus     = OBJECT_FIELD_S32(0x1D)
oKoopaRaceEndpointUnk100         = OBJECT_FIELD_S32(0x1E)
oKoopaRaceEndpointRaceEnded      = OBJECT_FIELD_S32(0x1F)
oKoopaShellFlameUnkF4  = OBJECT_FIELD_F32(0x1B)
oKoopaShellFlameUnkF8  = OBJECT_FIELD_F32(0x1C)
oCameraLakituBlinkTimer      = OBJECT_FIELD_S32(0x1B)
oCameraLakituSpeed           = OBJECT_FIELD_F32(0x1C)
oCameraLakituCircleRadius    = OBJECT_FIELD_F32(0x1D)
oCameraLakituFinishedDialog  = OBJECT_FIELD_S32(0x1E)
oCameraLakituUnk104          = OBJECT_FIELD_S32(0x1F)
oCameraLakituPitchVel        = OBJECT_FIELD_S16(0x49, 0)
oCameraLakituYawVel          = OBJECT_FIELD_S16(0x49, + 1)
oEnemyLakituNumSpinies            = OBJECT_FIELD_S32(0x1B)
oEnemyLakituBlinkTimer            = OBJECT_FIELD_S32(0x1C)
oEnemyLakituSpinyCooldown         = OBJECT_FIELD_S32(0x1D)
oEnemyLakituFaceForwardCountdown  = OBJECT_FIELD_S32(0x1E)
oIntroLakituSplineSegmentProgress   = OBJECT_FIELD_F32(0x1C)
oIntroLakituSplineSegment           = OBJECT_FIELD_F32(0x1D)
oIntroLakituUnk100                  = OBJECT_FIELD_F32(0x1E)
oIntroLakituUnk104                  = OBJECT_FIELD_F32(0x1F)
oIntroLakituUnk108                  = OBJECT_FIELD_F32(0x20)
oIntroLakituUnk10C                  = OBJECT_FIELD_F32(0x21)
oIntroLakituUnk110                  = OBJECT_FIELD_F32(0x22)
oIntroLakituCloud                   = OBJECT_FIELD_OBJ(0x49)
oMenuButtonState        = OBJECT_FIELD_S32(0x1B)
oMenuButtonTimer        = OBJECT_FIELD_S32(0x1C)
oMenuButtonOrigPosX     = OBJECT_FIELD_F32(0x1D)
oMenuButtonOrigPosY     = OBJECT_FIELD_F32(0x1E)
oMenuButtonOrigPosZ     = OBJECT_FIELD_F32(0x1F)
oMenuButtonScale        = OBJECT_FIELD_F32(0x20)
oMenuButtonActionPhase  = OBJECT_FIELD_S32(0x21)
oMantaUnkF4   = OBJECT_FIELD_S32(0x1B)
oMantaUnkF8   = OBJECT_FIELD_S32(0x1C)
oMantaUnk1AC  = OBJECT_FIELD_S32(0x49)
oMerryGoRoundStopped          = OBJECT_FIELD_S32(0x00)
oMerryGoRoundMusicShouldPlay  = OBJECT_FIELD_S32(0x1C)
oMerryGoRoundMarioIsOutside   = OBJECT_FIELD_S32(0x1D)
oMerryGoRoundBooManagerNumBoosKilled   = OBJECT_FIELD_S32(0x00)
oMerryGoRoundBooManagerNumBoosSpawned  = OBJECT_FIELD_S32(0x1D)
oMipsStarStatus          = OBJECT_FIELD_S32(0x1B)
oMipsStartWaypointIndex  = OBJECT_FIELD_S32(0x1C)

oMipsForwardVelocity     = OBJECT_FIELD_F32(0x49)
oMoneybagJumpState  = OBJECT_FIELD_S32(0x1B)
oMontyMoleCurrentHole            = OBJECT_FIELD_OBJ(0x1B)
oMontyMoleHeightRelativeToFloor  = OBJECT_FIELD_F32(0x1C)
oMontyMoleHoleCooldown  = OBJECT_FIELD_S32(0x1B)
oMrBlizzardUnkF4   = OBJECT_FIELD_F32(0x1B)
oMrBlizzardUnkF8   = OBJECT_FIELD_OBJ(0x1C)
oMrBlizzardUnkFC   = OBJECT_FIELD_F32(0x1D)
oMrBlizzardUnk100  = OBJECT_FIELD_S32(0x1E)
oMrBlizzardUnk104  = OBJECT_FIELD_F32(0x1F)
oMrBlizzardUnk108  = OBJECT_FIELD_F32(0x20)
oMrBlizzardUnk10C  = OBJECT_FIELD_F32(0x21)
oMrBlizzardUnkTimer  = OBJECT_FIELD_S32(0x22)
oMrBlizzardUnk1AC  = OBJECT_FIELD_S32(0x49)
oMrIUnkF4   = OBJECT_FIELD_S32(0x1B)
oMrIUnkFC   = OBJECT_FIELD_S32(0x1D)
oMrIUnk100  = OBJECT_FIELD_S32(0x1E)
oMrIUnk104  = OBJECT_FIELD_S32(0x1F)
oMrIUnk108  = OBJECT_FIELD_S32(0x20)
oMrISize    = OBJECT_FIELD_F32(0x21)
oMrIUnk110  = OBJECT_FIELD_S32(0x22)
oRespawnerModelToRespawn     = OBJECT_FIELD_S32(0x1B)
oRespawnerMinSpawnDist       = OBJECT_FIELD_F32(0x1C)
oRespawnerBehaviorToRespawn  = OBJECT_FIELD_CVPTR(0x1D)
oOpenableGrillUnk88  = OBJECT_FIELD_S32(0x00)
oOpenableGrillUnkF4  = OBJECT_FIELD_OBJ(0x1B)
oIntroPeachYawFromFocus  = OBJECT_FIELD_F32(0x20)
oIntroPeachPitchFromFocus  = OBJECT_FIELD_F32(0x21)
oIntroPeachDistToCamera  = OBJECT_FIELD_F32(0x22)
oRacingPenguinInitTextCooldown        = OBJECT_FIELD_S32(0x1B)

oRacingPenguinWeightedNewTargetSpeed  = OBJECT_FIELD_F32(0x22)
oRacingPenguinFinalTextbox            = OBJECT_FIELD_S16(0x49, 0)
oRacingPenguinMarioWon                = OBJECT_FIELD_S16(0x49, + 1)
oRacingPenguinReachedBottom           = OBJECT_FIELD_S16(0x4A, 0)
oRacingPenguinMarioCheated            = OBJECT_FIELD_S16(0x4A, + 1)
oSmallPenguinUnk88   = OBJECT_FIELD_S32(0x00)
oSmallPenguinUnk100  = OBJECT_FIELD_S32(0x1E) 
oSmallPenguinUnk104  = OBJECT_FIELD_F32(0x1F)
oSmallPenguinUnk108  = OBJECT_FIELD_F32(0x20)
oSmallPenguinUnk110  = OBJECT_FIELD_S32(0x22)
oWalkingPenguinUnk100  = OBJECT_FIELD_F32(0x1E)
oWalkingPenguinUnk104  = OBJECT_FIELD_F32(0x1F)
oWalkingPenguinMoveIndex  = OBJECT_FIELD_S32(0x21)
oWalkingPenguinMoveCounter  = OBJECT_FIELD_S32(0x22)
oPiranhaPlantSleepMusicState  = OBJECT_FIELD_S32(0x1B)
oPiranhaPlantScale            = OBJECT_FIELD_F32(0x1C)
oFirePiranhaPlantNeutralScale    = OBJECT_FIELD_F32(0x1B)
oFirePiranhaPlantScale           = OBJECT_FIELD_F32(0x1C) 
oFirePiranhaPlantActive          = OBJECT_FIELD_S32(0x1D)
oFirePiranhaPlantDeathSpinTimer  = OBJECT_FIELD_S32(0x1E)
oFirePiranhaPlantDeathSpinVel    = OBJECT_FIELD_F32(0x1F)
oPitouneUnkF4  = OBJECT_FIELD_F32(0x1B)
oPitouneUnkF8  = OBJECT_FIELD_F32(0x1C)
oPitouneUnkFC  = OBJECT_FIELD_F32(0x1D)
oPlatformTimer   = OBJECT_FIELD_S32(0x1B)
oPlatformUnkF8   = OBJECT_FIELD_OBJ(0x1C)
oPlatformUnkFC   = OBJECT_FIELD_S32(0x1D)
oPlatformCurrentVel  = OBJECT_FIELD_F32(0x21)
oPlatformTravelDistance  = OBJECT_FIELD_F32(0x22)
oPlatformOnTrackBaseBallIndex           = OBJECT_FIELD_S32(0x00)
oPlatformOnTrackDistMovedSinceLastBall  = OBJECT_FIELD_F32(0x1B)
oPlatformOnTrackSkiLiftRollVel          = OBJECT_FIELD_F32(0x1C)
oPlatformOnTrackStartWaypoint           = OBJECT_FIELD_WAYPOINT(0x1D)
oPlatformOnTrackPrevWaypoint            = OBJECT_FIELD_WAYPOINT(0x1E)
oPlatformOnTrackPrevWaypointFlags       = OBJECT_FIELD_S32(0x1F)
oPlatformOnTrackPitch                   = OBJECT_FIELD_S32(0x20)
oPlatformOnTrackYaw                     = OBJECT_FIELD_S32(0x21)
oPlatformOnTrackOffsetY                 = OBJECT_FIELD_F32(0x22)
oPlatformOnTrackIsNotSkiLift            = OBJECT_FIELD_S16(0x49, 0)
oPlatformOnTrackIsNotHMC                = OBJECT_FIELD_S16(0x49, + 1)
oPlatformOnTrackType                    = OBJECT_FIELD_S16(0x4A, 0)
oPlatformOnTrackWasStoodOn              = OBJECT_FIELD_S16(0x4A, + 1)
oPlatformSpawnerUnkF4   = OBJECT_FIELD_S32(0x1B)
oPlatformSpawnerUnkF8   = OBJECT_FIELD_S32(0x1C)
oPlatformSpawnerUnkFC   = OBJECT_FIELD_S32(0x1D)
oPlatformSpawnerUnk100  = OBJECT_FIELD_F32(0x1E)
oPlatformSpawnerUnk104  = OBJECT_FIELD_F32(0x1F)
oPlatformSpawnerUnk108  = OBJECT_FIELD_F32(0x20)
oPokeyAliveBodyPartFlags   = OBJECT_FIELD_U32(0x1B)
oPokeyNumAliveBodyParts    = OBJECT_FIELD_S32(0x1C)
oPokeyBottomBodyPartSize   = OBJECT_FIELD_F32(0x1D)
oPokeyHeadWasKilled        = OBJECT_FIELD_S32(0x1E)
oPokeyTargetYaw            = OBJECT_FIELD_S32(0x1F)
oPokeyChangeTargetTimer    = OBJECT_FIELD_S32(0x20)
oPokeyTurningAwayFromWall  = OBJECT_FIELD_S32(0x21)
oPokeyBodyPartDeathDelayAfterHeadKilled  = OBJECT_FIELD_S32(0x1C)
oPokeyBodyPartBlinkTimer                 = OBJECT_FIELD_S32(0x22)
oDDDPoleVel        = OBJECT_FIELD_F32(0x1B)
oDDDPoleMaxOffset  = OBJECT_FIELD_F32(0x1C)
oDDDPoleOffset     = OBJECT_FIELD_F32(0x1D)
oPyramidTopPillarsTouched  = OBJECT_FIELD_S32(0x1B)
oPyramidTopFragmentsScale  = OBJECT_FIELD_F32(0x1B)
oRollingLogUnkF4  = OBJECT_FIELD_F32(0x1B)
oLllRotatingHexFlameUnkF4  = OBJECT_FIELD_F32(0x1B)
oLllRotatingHexFlameUnkF8  = OBJECT_FIELD_F32(0x1C)
oLllRotatingHexFlameUnkFC  = OBJECT_FIELD_F32(0x1D)
oScuttlebugUnkF4  = OBJECT_FIELD_S32(0x1B)
oScuttlebugUnkF8  = OBJECT_FIELD_S32(0x1C)
oScuttlebugUnkFC  = OBJECT_FIELD_S32(0x1D)
oScuttlebugSpawnerUnk88  = OBJECT_FIELD_S32(0x00)
oScuttlebugSpawnerUnkF4  = OBJECT_FIELD_S32(0x1B)
oSeesawPlatformPitchVel  = OBJECT_FIELD_F32(0x1B)
oShipPart3UnkF4  = OBJECT_FIELD_S32(0x1B) 
oShipPart3UnkF8  = OBJECT_FIELD_S32(0x1C) 
oSinkWhenSteppedOnUnk104  = OBJECT_FIELD_S32(0x1F)
oSinkWhenSteppedOnUnk108  = OBJECT_FIELD_F32(0x20)
oSkeeterTargetAngle  = OBJECT_FIELD_S32(0x1B)
oSkeeterUnkF8        = OBJECT_FIELD_S32(0x1C)
oSkeeterUnkFC        = OBJECT_FIELD_F32(0x1D)
oSkeeterWaitTime     = OBJECT_FIELD_S32(0x1E)
oSkeeterUnk1AC       = OBJECT_FIELD_S16(0x49, 0)
oJrbSlidingBoxUnkF4  = OBJECT_FIELD_OBJ(0x1B)
oJrbSlidingBoxUnkF8  = OBJECT_FIELD_S32(0x1C)
oJrbSlidingBoxUnkFC  = OBJECT_FIELD_F32(0x1D)
oWFSlidBrickPtfmMovVel  = OBJECT_FIELD_F32(0x1B)
oSmokeTimer  = OBJECT_FIELD_S32(0x1B)
oSnowmansBottomUnkF4   = OBJECT_FIELD_F32(0x1B)
oSnowmansBottomUnkF8   = OBJECT_FIELD_S32(0x1C)
oSnowmansBottomUnk1AC  = OBJECT_FIELD_S32(0x49)

oSnowmansHeadUnkF4  = OBJECT_FIELD_S32(0x1B)
oSnowmanWindBlowingUnkF4  = OBJECT_FIELD_S32(0x1B)
oSnufitUnkF4   = OBJECT_FIELD_S32(0x1B)
oSnufitUnkF8   = OBJECT_FIELD_F32(0x1C)
oSnufitUnk100  = OBJECT_FIELD_S32(0x1E)
oSnufitUnk104  = OBJECT_FIELD_S32(0x1F)
oSnufitUnk108  = OBJECT_FIELD_S32(0x20)
oSnufitUnk10C  = OBJECT_FIELD_S32(0x21)
oSnufitUnk1AC  = OBJECT_FIELD_S16(0x49, 0)
oSnufitUnk1AE  = OBJECT_FIELD_S16(0x49, + 1)
oSnufitUnk1B0  = OBJECT_FIELD_S16(0x4A, 0)
oSnufitUnk1B2  = OBJECT_FIELD_S16(0x4A, + 1)
oSpindelUnkF4  = OBJECT_FIELD_S32(0x1B)
oSpindelUnkF8  = OBJECT_FIELD_S32(0x1C)
oSpinningHeartTotalSpin    = OBJECT_FIELD_S32(0x1B)
oSpinningHeartPlayedSound  = OBJECT_FIELD_S32(0x1C)
oSpinyTimeUntilTurn        = OBJECT_FIELD_S32(0x1B)
oSpinyTargetYaw            = OBJECT_FIELD_S32(0x1C)
oSpinyTurningAwayFromWall  = OBJECT_FIELD_S32(0x1E)
oSoundEffectUnkF4  = OBJECT_FIELD_S32(0x1B)
oStarSpawnDisFromHome  = OBJECT_FIELD_F32(0x1B)
oStarSpawnUnkFC        = OBJECT_FIELD_F32(0x1D)

oHiddenStarTriggerCounter  = OBJECT_FIELD_S32(0x1B)

oSparkleSpawnUnk1B0  = OBJECT_FIELD_S32(0x4A)
oUnlockDoorStarState   = OBJECT_FIELD_U32(0x20)
oUnlockDoorStarTimer   = OBJECT_FIELD_S32(0x21)
oUnlockDoorStarYawVel  = OBJECT_FIELD_S32(0x22)
oCelebStarUnkF4               = OBJECT_FIELD_S32(0x1B)
oCelebStarDiameterOfRotation  = OBJECT_FIELD_S32(0x20)
oStarSelectorType   = OBJECT_FIELD_S32(0x1B)
oStarSelectorTimer  = OBJECT_FIELD_S32(0x1C)
oStarSelectorSize   = OBJECT_FIELD_F32(0x20)
oSushiSharkUnkF4  = OBJECT_FIELD_S32(0x1B) 
oSwingPlatformAngle  = OBJECT_FIELD_F32(0x1B)
oSwingPlatformSpeed  = OBJECT_FIELD_F32(0x1C)
oSwoopBonkCountdown  = OBJECT_FIELD_S32(0x1B)
oSwoopTargetPitch    = OBJECT_FIELD_S32(0x1C)
oSwoopTargetYaw      = OBJECT_FIELD_S32(0x1D)
oThwompUnkF4  = OBJECT_FIELD_S32(0x1B)
oTiltingPyramidNormalX          = OBJECT_FIELD_F32(0x1B)
oTiltingPyramidNormalY          = OBJECT_FIELD_F32(0x1C)
oTiltingPyramidNormalZ          = OBJECT_FIELD_F32(0x1D)
oTiltingPyramidMarioOnPlatform  = OBJECT_FIELD_S32(0x21)
oToadMessageDialogId       = OBJECT_FIELD_U32(0x20)
oToadMessageRecentlyTalked  = OBJECT_FIELD_S32(0x21)
oToadMessageState           = OBJECT_FIELD_S32(0x22)
oToxBoxUnk1AC  = OBJECT_FIELD_VPTR(0x49)
oToxBoxUnk1B0  = OBJECT_FIELD_S32(0x4A)
oTTCRotatingSolidNumTurns       = OBJECT_FIELD_S32(0x1B)
oTTCRotatingSolidNumSides       = OBJECT_FIELD_S32(0x1C)
oTTCRotatingSolidRotationDelay  = OBJECT_FIELD_S32(0x1D)
oTTCRotatingSolidVelY           = OBJECT_FIELD_F32(0x1E)
oTTCRotatingSolidSoundTimer     = OBJECT_FIELD_S32(0x1F)
oTTCPendulumAccelDir    = OBJECT_FIELD_F32(0x1B)
oTTCPendulumAngle       = OBJECT_FIELD_F32(0x1C)
oTTCPendulumAngleVel    = OBJECT_FIELD_F32(0x1D)
oTTCPendulumAngleAccel  = OBJECT_FIELD_F32(0x1E)
oTTCPendulumDelay       = OBJECT_FIELD_S32(0x1F)
oTTCPendulumSoundTimer  = OBJECT_FIELD_S32(0x20)
oTTCTreadmillBigSurface       = OBJECT_FIELD_S16P(0x1B)
oTTCTreadmillSmallSurface     = OBJECT_FIELD_S16P(0x1C)
oTTCTreadmillSpeed            = OBJECT_FIELD_F32(0x1D)
oTTCTreadmillTargetSpeed      = OBJECT_FIELD_F32(0x1E)
oTTCTreadmillTimeUntilSwitch  = OBJECT_FIELD_S32(0x1F)
oTTCMovingBarDelay         = OBJECT_FIELD_S32(0x1B)
oTTCMovingBarStoppedTimer  = OBJECT_FIELD_S32(0x1C)
oTTCMovingBarOffset        = OBJECT_FIELD_F32(0x1D)
oTTCMovingBarSpeed         = OBJECT_FIELD_F32(0x1E)
oTTCMovingBarStartOffset   = OBJECT_FIELD_F32(0x1F)
oTTCCogDir        = OBJECT_FIELD_F32(0x1B)
oTTCCogSpeed      = OBJECT_FIELD_F32(0x1C)
oTTCCogTargetVel  = OBJECT_FIELD_F32(0x1D)
oTTCPitBlockPeakY     = OBJECT_FIELD_F32(0x1B)
oTTCPitBlockDir       = OBJECT_FIELD_S32(0x1C)
oTTCPitBlockWaitTime  = OBJECT_FIELD_S32(0x1D)
oTTCElevatorDir       = OBJECT_FIELD_F32(0x1B)
oTTCElevatorPeakY     = OBJECT_FIELD_F32(0x1C)
oTTCElevatorMoveTime  = OBJECT_FIELD_S32(0x1D)
oTTC2DRotatorMinTimeUntilNextTurn  = OBJECT_FIELD_S32(0x1B)
oTTC2DRotatorTargetYaw             = OBJECT_FIELD_S32(0x1C)
oTTC2DRotatorIncrement             = OBJECT_FIELD_S32(0x1D)
oTTC2DRotatorRandomDirTimer        = OBJECT_FIELD_S32(0x1F)
oTTC2DRotatorSpeed                 = OBJECT_FIELD_S32(0x20)
oTTCSpinnerDir      = OBJECT_FIELD_S32(0x1B)
oTTCChangeDirTimer  = OBJECT_FIELD_S32(0x1C)
oBetaTrampolineMarioOnTrampoline  = OBJECT_FIELD_S32(0x22)
oTreasureChestUnkF4  = OBJECT_FIELD_S32(0x1B)
oTreasureChestUnkF8  = OBJECT_FIELD_S32(0x1C)
oTreasureChestUnkFC  = OBJECT_FIELD_S32(0x1D)
oTreeSnowOrLeafUnkF4  = OBJECT_FIELD_S32(0x1B)
oTreeSnowOrLeafUnkF8  = OBJECT_FIELD_S32(0x1C)
oTreeSnowOrLeafUnkFC  = OBJECT_FIELD_S32(0x1D)
oTumblingBridgeUnkF4  = OBJECT_FIELD_S32(0x1B)
oTweesterUnkF4  = OBJECT_FIELD_S32(0x1B)
oTweesterUnkF8  = OBJECT_FIELD_S32(0x1C)
oUkikiTauntCounter    = OBJECT_FIELD_S16(0x1B, 0)
oUkikiTauntsToBeDone  = OBJECT_FIELD_S16(0x1B, 1)

oUkikiChaseFleeRange  = OBJECT_FIELD_F32(0x22)
oUkikiTextState       = OBJECT_FIELD_S16(0x49, 0)
oUkikiTextboxTimer    = OBJECT_FIELD_S16(0x49, 1)
oUkikiCageSpinTimer   = OBJECT_FIELD_S16(0x4A, 0)
oUkikiHasHat          = OBJECT_FIELD_S16(0x4A, 1)
oUkikiCageNextAction  = OBJECT_FIELD_S32(0x00)
oUnagiUnkF4   = OBJECT_FIELD_F32(0x1B)
oUnagiUnkF8   = OBJECT_FIELD_F32(0x1C)

oUnagiUnk110  = OBJECT_FIELD_F32(0x22)
oUnagiUnk1AC  = OBJECT_FIELD_F32(0x49)
oUnagiUnk1B0  = OBJECT_FIELD_S16(0x4A, 0)
oUnagiUnk1B2  = OBJECT_FIELD_S16(0x4A, + 1)
oWaterBombVerticalStretch  = OBJECT_FIELD_F32(0x1C)
oWaterBombStretchSpeed     = OBJECT_FIELD_F32(0x1D)
oWaterBombOnGround         = OBJECT_FIELD_S32(0x1E)
oWaterBombNumBounces       = OBJECT_FIELD_F32(0x1F)
oWaterBombSpawnerBombActive   = OBJECT_FIELD_S32(0x1B)
oWaterBombSpawnerTimeToSpawn  = OBJECT_FIELD_S32(0x1C)
oWaterCannonUnkF4   = OBJECT_FIELD_S32(0x1B)
oWaterCannonUnkF8   = OBJECT_FIELD_S32(0x1C)
oWaterCannonUnkFC   = OBJECT_FIELD_S32(0x1D)
oWaterCannonUnk100  = OBJECT_FIELD_S32(0x1E)
oCannonBarrelBubblesUnkF4  = OBJECT_FIELD_F32(0x1B)
oWaterLevelPillarUnkF8  = OBJECT_FIELD_S32(0x1C)
oWaterLevelTriggerUnkF4             = OBJECT_FIELD_S32(0x1B)
oWaterLevelTriggerTargetWaterLevel  = OBJECT_FIELD_S32(0x1C)
oWaterObjUnkF4   = OBJECT_FIELD_S32(0x1B)
oWaterObjUnkF8   = OBJECT_FIELD_S32(0x1C)
oWaterObjUnkFC   = OBJECT_FIELD_S32(0x1D)
oWaterObjUnk100  = OBJECT_FIELD_S32(0x1E)
oWaterRingScalePhaseX       = OBJECT_FIELD_S32(0x1B)
oWaterRingScalePhaseY       = OBJECT_FIELD_S32(0x1C)
oWaterRingScalePhaseZ       = OBJECT_FIELD_S32(0x1D)
oWaterRingNormalX           = OBJECT_FIELD_F32(0x1E)
oWaterRingNormalY           = OBJECT_FIELD_F32(0x1F)
oWaterRingNormalZ           = OBJECT_FIELD_F32(0x20)
oWaterRingMarioDistInFront  = OBJECT_FIELD_F32(0x21)
oWaterRingIndex             = OBJECT_FIELD_S32(0x22)
oWaterRingAvgScale          = OBJECT_FIELD_F32(0x49)
oWaterRingSpawnerRingsCollected  = OBJECT_FIELD_S32(0x49)
oWaterRingMgrNextRingIndex      = OBJECT_FIELD_S32(0x1B)
oWaterRingMgrLastRingCollected  = OBJECT_FIELD_S32(0x1C)
oWaveTrailUnkF8  = OBJECT_FIELD_F32(0x1C)
oWhirlpoolInitFacePitch  = OBJECT_FIELD_S32(0x1B)
oWhirlpoolInitFaceRoll   = OBJECT_FIELD_S32(0x1C)
oWhitePuffUnkF4  = OBJECT_FIELD_F32(0x1B)
oWhitePuffUnkF8  = OBJECT_FIELD_S32(0x1C)
oWhitePuffUnkFC  = OBJECT_FIELD_S32(0x1D)
oWhiteWindParticleUnkF4  = OBJECT_FIELD_OBJ(0x1B)
oWhompUnkF8  = OBJECT_FIELD_S32(0x1C)
oWigglerFallThroughFloorsHeight  = OBJECT_FIELD_F32(0x1B)
oWigglerSegments                 = OBJECT_FIELD_CHAIN_SEGMENT(0x1C)
oWigglerWalkAnimSpeed            = OBJECT_FIELD_F32(0x1D)
oWigglerSquishSpeed              = OBJECT_FIELD_F32(0x1F)
oWigglerTimeUntilRandomTurn      = OBJECT_FIELD_S32(0x20)
oWigglerTargetYaw                = OBJECT_FIELD_S32(0x21)
oWigglerWalkAwayFromWallTimer    = OBJECT_FIELD_S32(0x22)
oWigglerUnk1AC                   = OBJECT_FIELD_S16(0x49, 0)
oWigglerTextStatus               = OBJECT_FIELD_S16(0x49, + 1)
oLllWoodPieceUnkF4  = OBJECT_FIELD_S32(0x1B)
oWoodenPostTotalMarioAngle   = OBJECT_FIELD_S32(0x1B)
oWoodenPostPrevAngleToMario  = OBJECT_FIELD_S32(0x1C)
oWoodenPostSpeedY            = OBJECT_FIELD_F32(0x1D)
oWoodenPostMarioPounding     = OBJECT_FIELD_S32(0x1E)
oWoodenPostOffsetY           = OBJECT_FIELD_F32(0x1F)
oYoshiUnkF4       = OBJECT_FIELD_S32(0x1B)
oYoshiChosenHome  = OBJECT_FIELD_S32(0x1D)
oYoshiTargetYaw   = OBJECT_FIELD_S32(0x1E)
	

beh_yellow_background_menu_init		= 1
beh_yellow_background_menu_loop		= 2
BehClimbDetectLoop		= 3
BehDustSmokeLoop		= 4
BehEndPeachLoop		= 5
BehEndToadLoop		= 6
BehFadingWarpLoop		= 7
BehYoshiLoop		= 8
bhv_1up_common_init		= 9
bhv_1up_hidden_in_pole_loop		= 10
bhv_1up_hidden_in_pole_spawner_loop		= 11
bhv_1up_hidden_in_pole_trigger_loop		= 12
bhv_1up_hidden_loop		= 13
bhv_1up_hidden_trigger_loop		= 14
bhv_1up_init		= 15
bhv_1up_jump_on_approach_loop		= 16
bhv_1up_loop		= 17
bhv_1up_running_away_loop		= 18
bhv_1up_sliding_loop		= 19
bhv_1up_walking_loop		= 20
bhv_act_selector_init		= 21
bhv_act_selector_loop		= 22
bhv_act_selector_star_type_loop		= 23
bhv_activated_back_and_forth_platform_init		= 24
bhv_activated_back_and_forth_platform_update		= 25
bhv_alpha_boo_key_loop		= 26
bhv_ambient_sounds_init		= 27
bhv_animated_texture_loop		= 28
bhv_animates_on_floor_switch_press_init		= 29
bhv_animates_on_floor_switch_press_loop		= 30
bhv_arrow_lift_loop		= 31
bhv_bbh_tilting_trap_platform_loop		= 32
bhv_beta_boo_key_loop		= 33
bhv_beta_bowser_anchor_loop		= 34
bhv_beta_chest_bottom_init		= 35
bhv_beta_chest_bottom_loop		= 36
bhv_beta_chest_lid_loop		= 37
bhv_beta_fish_splash_spawner_loop		= 38
bhv_beta_holdable_object_init		= 39
bhv_beta_holdable_object_loop		= 40
bhv_beta_moving_flames_loop		= 41
bhv_beta_moving_flames_spawn_loop		= 42
bhv_beta_trampoline_spring_loop		= 43
bhv_beta_trampoline_top_loop		= 44
bhv_big_boo_loop		= 45
bhv_big_boulder_generator_loop		= 46
bhv_big_boulder_init		= 47
bhv_big_boulder_loop		= 48
bhv_big_bully_init		= 49
bhv_big_bully_with_minions_init		= 50
bhv_big_bully_with_minions_loop		= 51
bhv_bird_chirp_chirp_loop		= 52
bhv_bird_update		= 53
bhv_birds_sound_loop		= 54
bhv_bitfs_sinking_cage_platform_loop		= 55
bhv_bitfs_sinking_platform_loop		= 56
bhv_black_smoke_bowser_loop		= 57
bhv_black_smoke_mario_loop		= 58
bhv_black_smoke_upward_loop		= 59
bhv_blue_bowser_flame_init		= 60
bhv_blue_bowser_flame_loop		= 61
bhv_blue_coin_jumping_loop		= 62
bhv_blue_coin_sliding_jumping_init		= 63
bhv_blue_coin_sliding_loop		= 64
bhv_blue_coin_switch_loop		= 65
bhv_blue_fish_loop		= 66
bhv_blue_flames_group_loop		= 67
bhv_bob_pit_bowling_ball_init		= 68
bhv_bob_pit_bowling_ball_loop		= 69
bhv_bobomb_anchor_mario_loop		= 70
bhv_bobomb_buddy_init		= 71
bhv_bobomb_buddy_loop		= 72
bhv_bobomb_bully_death_smoke_init		= 73
bhv_bobomb_explosion_bubble_init		= 74
bhv_bobomb_explosion_bubble_loop		= 75
bhv_bobomb_fuse_smoke_init		= 76
bhv_bobomb_init		= 77
bhv_bobomb_loop		= 78
bhv_boo_boss_spawned_bridge_loop		= 79
bhv_boo_cage_loop		= 80
bhv_boo_in_castle_loop		= 81
bhv_boo_init		= 82
bhv_boo_loop		= 83
bhv_boo_with_cage_init		= 84
bhv_boo_with_cage_loop		= 85
bhv_book_switch_loop		= 86
bhv_bookend_spawn_loop		= 87
bhv_bouncing_fireball_flame_loop		= 88
bhv_bouncing_fireball_loop		= 89
bhv_bowling_ball_init		= 90
bhv_bowling_ball_loop		= 91
bhv_bowser_body_anchor_loop		= 92
bhv_bowser_bomb_explosion_loop		= 93
bhv_bowser_bomb_loop		= 94
bhv_bowser_bomb_smoke_loop		= 95
bhv_bowser_course_red_coin_star_loop		= 96
bhv_bowser_flame_spawn_loop		= 97
bhv_bowser_init		= 98
bhv_bowser_key_course_exit_loop		= 99
bhv_bowser_key_loop		= 100
bhv_bowser_key_unlock_door_loop		= 101
bhv_bowser_loop		= 102
bhv_bowser_shock_wave_loop		= 103
bhv_bowser_tail_anchor_loop		= 104
bhv_bowsers_sub_loop		= 105
bhv_breakable_box_loop		= 106
bhv_breakable_box_small_init		= 107
bhv_breakable_box_small_loop		= 108
bhv_bubba_loop		= 109
bhv_bubble_cannon_barrel_loop		= 110
bhv_bubble_maybe_loop		= 111
bhv_bubble_wave_init		= 112
bhv_bullet_bill_init		= 113
bhv_bullet_bill_loop		= 114
bhv_bully_loop		= 115
bhv_butterfly_init		= 116
bhv_butterfly_loop		= 117
bhv_camera_lakitu_init		= 118
bhv_camera_lakitu_update		= 119
bhv_cannon_barrel_loop		= 120
bhv_cannon_base_loop		= 121
bhv_cannon_base_unused_loop		= 122
bhv_cannon_closed_init		= 123
bhv_cannon_closed_loop		= 124
bhv_cap_switch_loop		= 125
bhv_castle_cannon_grate_init		= 126
bhv_castle_flag_init		= 127
bhv_castle_floor_trap_init		= 128
bhv_castle_floor_trap_loop		= 129
bhv_ccm_touched_star_spawn_loop		= 130
bhv_celebration_star_init		= 131
bhv_celebration_star_loop		= 132
bhv_celebration_star_sparkle_loop		= 133
bhv_chain_chomp_chain_part_update		= 134
bhv_chain_chomp_gate_init		= 135
bhv_chain_chomp_gate_update		= 136
bhv_chain_chomp_update		= 137
bhv_checkerboard_elevator_group_init		= 138
bhv_checkerboard_platform_init		= 139
bhv_checkerboard_platform_loop		= 140
bhv_cheep_cheep_loop		= 141
bhv_chuckya_anchor_mario_loop		= 142
bhv_chuckya_loop		= 143
bhv_circling_amp_init		= 144
bhv_circling_amp_loop		= 145
bhv_clam_loop		= 146
bhv_cloud_part_update		= 147
bhv_cloud_update		= 148
bhv_coffin_loop		= 149
bhv_coffin_manager_loop		= 150
bhv_coin_formation_init		= 151
bhv_coin_formation_loop		= 152
bhv_coin_formation_spawn_loop		= 153
bhv_coin_init		= 154
bhv_coin_inside_boo_loop		= 155
bhv_coin_loop		= 156
bhv_coin_sparkles_loop		= 157
bhv_collect_star_init		= 158
bhv_collect_star_loop		= 159
bhv_controllable_platform_init		= 160
bhv_controllable_platform_loop		= 161
bhv_controllable_platform_sub_loop		= 162
bhv_courtyard_boo_triplet_init		= 163
bhv_ddd_moving_pole_loop		= 164
bhv_ddd_pole_init		= 165
bhv_ddd_pole_update		= 166
bhv_ddd_warp_loop		= 167
bhv_decorative_pendulum_init		= 168
bhv_decorative_pendulum_loop		= 169
bhv_donut_platform_spawner_update		= 170
bhv_donut_platform_update		= 171
bhv_door_init		= 172
bhv_door_loop		= 173
bhv_dorrie_update		= 174
bhv_elevator_init		= 175
bhv_elevator_loop		= 176
bhv_end_birds_1_loop		= 177
bhv_end_birds_2_loop		= 178
bhv_enemy_lakitu_update		= 179
bhv_exclamation_box_loop		= 180
bhv_explosion_init		= 181
bhv_explosion_loop		= 182
bhv_eyerok_boss_loop		= 183
bhv_eyerok_hand_loop		= 184
bhv_falling_bowser_platform_loop		= 185
bhv_falling_pillar_hitbox_loop		= 186
bhv_falling_pillar_init		= 187
bhv_falling_pillar_loop		= 188
bhv_ferris_wheel_axle_init		= 189
bhv_ferris_wheel_platform_update		= 190
bhv_fire_piranha_plant_init		= 191
bhv_fire_piranha_plant_update		= 192
bhv_fire_spitter_update		= 193
bhv_fish_group_2_loop		= 194
bhv_fish_group_loop		= 195
bhv_fish_loop		= 196
bhv_flame_bouncing_init		= 197
bhv_flame_bouncing_loop		= 198
bhv_flame_bowser_init		= 199
bhv_flame_bowser_loop		= 200
bhv_flame_floating_landing_init		= 201
bhv_flame_floating_landing_loop		= 202
bhv_flame_large_burning_out_init		= 203
bhv_flame_mario_loop		= 204
bhv_flame_moving_forward_growing_init		= 205
bhv_flame_moving_forward_growing_loop		= 206
bhv_flamethrower_flame_loop		= 207
bhv_flamethrower_loop		= 208
bhv_floating_platform_loop		= 209
bhv_floor_trap_in_castle_loop		= 210
bhv_fly_guy_flame_loop		= 211
bhv_fly_guy_update		= 212
bhv_flying_bookend_loop		= 213
bhv_free_bowling_ball_init		= 214
bhv_free_bowling_ball_loop		= 215
bhv_generic_bowling_ball_spawner_init		= 216
bhv_generic_bowling_ball_spawner_loop		= 217
bhv_giant_pole_loop		= 218
bhv_golden_coin_sparkles_loop		= 219
bhv_goomba_init		= 220
bhv_goomba_triplet_spawner_update		= 221
bhv_goomba_update		= 222
bhv_grand_star_loop		= 223
bhv_grindel_thwomp_loop		= 224
bhv_ground_snow_init		= 225
bhv_haunted_bookshelf_loop		= 226
bhv_haunted_bookshelf_manager_loop		= 227
bhv_haunted_chair_init		= 228
bhv_haunted_chair_loop		= 229
bhv_heave_ho_loop		= 230
bhv_heave_ho_throw_mario_loop		= 231
bhv_hidden_blue_coin_loop		= 232
bhv_hidden_object_loop		= 233
bhv_hidden_red_coin_star_init		= 234
bhv_hidden_red_coin_star_loop		= 235
bhv_hidden_star_init		= 236
bhv_hidden_star_loop		= 237
bhv_hidden_star_trigger_loop		= 238
bhv_homing_amp_init		= 239
bhv_homing_amp_loop		= 240
bhv_hoot_init		= 241
bhv_hoot_loop		= 242
bhv_horizontal_grindel_init		= 243
bhv_horizontal_grindel_update		= 244
bhv_init_changing_water_level_loop		= 245
bhv_init_room		= 246
bhv_intro_lakitu_loop		= 247
bhv_intro_peach_loop		= 248
bhv_intro_scene_loop		= 249
bhv_invisible_objects_under_bridge_init		= 250
bhv_jet_stream_loop		= 251
bhv_jet_stream_ring_spawner_loop		= 252
bhv_jet_stream_water_ring_init		= 253
bhv_jet_stream_water_ring_loop		= 254
bhv_jrb_floating_box_loop		= 255
bhv_jrb_sliding_box_loop		= 256
bhv_jumping_box_loop		= 257
bhv_kickable_board_loop		= 258
bhv_king_bobomb_loop		= 259
bhv_klepto_init		= 260
bhv_klepto_update		= 261
bhv_koopa_init		= 262
bhv_koopa_race_endpoint_update		= 263
bhv_koopa_shell_flame_loop		= 264
bhv_koopa_shell_loop		= 265
bhv_koopa_shell_underwater_loop		= 266
bhv_koopa_update		= 267
bhv_large_bomp_init		= 268
bhv_large_bomp_loop		= 269
bhv_lll_bowser_puzzle_loop		= 270
bhv_lll_bowser_puzzle_piece_loop		= 271
bhv_lll_drawbridge_loop		= 272
bhv_lll_drawbridge_spawner_loop		= 273
bhv_lll_floating_wood_bridge_loop		= 274
bhv_lll_moving_octagonal_mesh_platform_loop		= 275
bhv_lll_rolling_log_init		= 276
bhv_lll_rotating_block_fire_bars_loop		= 277
bhv_lll_rotating_hex_flame_loop		= 278
bhv_lll_rotating_hexagonal_ring_loop		= 279
bhv_lll_sinking_rectangular_platform_loop		= 280
bhv_lll_sinking_rock_block_loop		= 281
bhv_lll_sinking_square_platforms_loop		= 282
bhv_lll_wood_piece_loop		= 283
bhv_mad_piano_update		= 284
bhv_manta_ray_init		= 285
bhv_manta_ray_loop		= 286
bhv_manta_ray_water_ring_init		= 287
bhv_manta_ray_water_ring_loop		= 288
bhv_mario_update		= 289
bhv_menu_button_init		= 290
bhv_menu_button_loop		= 291
bhv_menu_button_manager_init		= 292
bhv_menu_button_manager_loop		= 293
bhv_merry_go_round_boo_manager_loop		= 294
bhv_merry_go_round_loop		= 295
bhv_metal_cap_init		= 296
bhv_metal_cap_loop		= 297
bhv_mips_init		= 298
bhv_mips_loop		= 299
bhv_moat_grills_loop		= 300
bhv_moneybag_hidden_loop		= 301
bhv_moneybag_init		= 302
bhv_moneybag_loop		= 303
bhv_monty_mole_hole_update		= 304
bhv_monty_mole_init		= 305
bhv_monty_mole_rock_update		= 306
bhv_monty_mole_update		= 307
bhv_moving_blue_coin_init		= 308
bhv_moving_blue_coin_loop		= 309
bhv_moving_yellow_coin_init		= 310
bhv_moving_yellow_coin_loop		= 311
bhv_mr_blizzard_init		= 312
bhv_mr_blizzard_snowball		= 313
bhv_mr_blizzard_update		= 314
bhv_mr_i_body_loop		= 315
bhv_mr_i_loop		= 316
bhv_mr_i_particle_loop		= 317
bhv_normal_cap_init		= 318
bhv_normal_cap_loop		= 319
bhv_object_bubble_init		= 320
bhv_object_bubble_loop		= 321
bhv_object_bubble_ripples_init		= 322
bhv_object_water_wave_init		= 323
bhv_object_water_wave_loop		= 324
bhv_openable_cage_door_loop		= 325
bhv_openable_grill_loop		= 326
bhv_orange_number_init		= 327
bhv_orange_number_loop		= 328
bhv_particle_init		= 329
bhv_particle_loop		= 330
bhv_penguin_race_finish_line_update		= 331
bhv_penguin_race_shortcut_check_update		= 332
bhv_piranha_particle_loop		= 333
bhv_piranha_plant_bubble_loop		= 334
bhv_piranha_plant_loop		= 335
bhv_piranha_plant_waking_bubbles_loop		= 336
bhv_platform_normals_init		= 337
bhv_platform_on_track_init		= 338
bhv_platform_on_track_update		= 339
bhv_play_music_track_when_touched_loop		= 340
bhv_pokey_body_part_update		= 341
bhv_pokey_update		= 342
bhv_pole_init		= 343
bhv_pound_tiny_star_particle_init		= 344
bhv_pound_tiny_star_particle_loop		= 345
bhv_pound_white_puffs_init		= 346
bhv_punch_tiny_triangle_init		= 347
bhv_punch_tiny_triangle_loop		= 348
bhv_purple_switch_loop		= 349
bhv_pushable_loop		= 350
bhv_pyramid_elevator_init		= 351
bhv_pyramid_elevator_loop		= 352
bhv_pyramid_elevator_trajectory_marker_ball_loop		= 353
bhv_pyramid_pillar_touch_detector_loop		= 354
bhv_pyramid_top_fragment_init		= 355
bhv_pyramid_top_fragment_loop		= 356
bhv_pyramid_top_init		= 357
bhv_pyramid_top_loop		= 358
bhv_racing_penguin_init		= 359
bhv_racing_penguin_update		= 360
bhv_recovery_heart_loop		= 361
bhv_red_coin_init		= 362
bhv_red_coin_loop		= 363
bhv_red_coin_star_marker_init		= 364
bhv_respawner_loop		= 365
bhv_rolling_log_loop		= 366
bhv_rotatin_exclamation_box_loop		= 367
bhv_rotating_clock_arm_loop		= 368
bhv_rotating_octagonal_plat_init		= 369
bhv_rotating_octagonal_plat_loop		= 370
bhv_rotating_platform_loop		= 371
bhv_rr_cruiser_wing_init		= 372
bhv_rr_cruiser_wing_loop		= 373
bhv_rr_rotating_bridge_platform_loop		= 374
bhv_sand_sound_loop		= 375
bhv_scuttlebug_loop		= 376
bhv_scuttlebug_spawn_loop		= 377
bhv_seaweed_bundle_init		= 378
bhv_seaweed_init		= 379
bhv_seesaw_platform_init		= 380
bhv_seesaw_platform_update		= 381
bhv_ship_part_3_loop		= 382
bhv_skeeter_update		= 383
bhv_skeeter_wave_update		= 384
bhv_sliding_plat_2_init		= 385
bhv_sliding_plat_2_loop		= 386
bhv_sliding_snow_mound_loop		= 387
bhv_small_bomp_init		= 388
bhv_small_bomp_loop		= 389
bhv_small_bubbles_loop		= 390
bhv_small_bully_init		= 391
bhv_small_penguin_loop		= 392
bhv_small_piranha_flame_loop		= 393
bhv_small_water_wave_loop		= 394
bhv_snow_leaf_particle_spawn_init		= 395
bhv_snow_mound_spawn_loop		= 396
bhv_snowman_wind_blowing_loop		= 397
bhv_snowmans_body_checkpoint_loop		= 398
bhv_snowmans_bottom_init		= 399
bhv_snowmans_bottom_loop		= 400
bhv_snowmans_head_init		= 401
bhv_snowmans_head_loop		= 402
bhv_snufit_balls_loop		= 403
bhv_snufit_loop		= 404
bhv_sound_spawner_init		= 405
bhv_sparkle_spawn_loop		= 406
bhv_spawned_star_init		= 407
bhv_spawned_star_loop		= 408
bhv_spindel_init		= 409
bhv_spindel_loop		= 410
bhv_spindrift_loop		= 411
bhv_spiny_update		= 412
bhv_squarish_path_moving_loop		= 413
bhv_squishable_platform_loop		= 414
bhv_ssl_moving_pyramid_wall_init		= 415
bhv_ssl_moving_pyramid_wall_loop		= 416
bhv_star_door_loop		= 417
bhv_star_door_loop_2		= 418
bhv_star_key_collection_puff_spawner_loop		= 419
bhv_star_spawn_init		= 420
bhv_star_spawn_loop		= 421
bhv_static_checkered_platform_loop		= 422
bhv_sunken_ship_part_loop		= 423
bhv_surface_wave_shrinking_init		= 424
bhv_surface_waves_loop		= 425
bhv_sushi_shark_collision_loop		= 426
bhv_sushi_shark_loop		= 427
bhv_swing_platform_init		= 428
bhv_swing_platform_update		= 429
bhv_swoop_update		= 430
bhv_tank_fish_group_loop		= 431
bhv_temp_coin_loop		= 432
bhv_thi_bowling_ball_spawner_loop		= 433
bhv_thi_huge_island_top_loop		= 434
bhv_thi_tiny_island_top_loop		= 435
bhv_tilting_inverted_pyramid_loop		= 436
bhv_tiny_star_particles_init		= 437
bhv_tower_door_loop		= 438
bhv_tower_platform_group_loop		= 439
bhv_tox_box_loop		= 440
bhv_track_ball_update		= 441
bhv_treasure_chest_bottom_init		= 442
bhv_treasure_chest_bottom_loop		= 443
bhv_treasure_chest_init		= 444
bhv_treasure_chest_jrb_init		= 445
bhv_treasure_chest_jrb_loop		= 446
bhv_treasure_chest_loop		= 447
bhv_treasure_chest_ship_init		= 448
bhv_treasure_chest_ship_loop		= 449
bhv_treasure_chest_top_loop		= 450
bhv_tree_snow_or_leaf_loop		= 451
bhv_triplet_butterfly_update		= 452
bhv_ttc_2d_rotator_init		= 453
bhv_ttc_2d_rotator_update		= 454
bhv_ttc_cog_init		= 455
bhv_ttc_cog_update		= 456
bhv_ttc_elevator_init		= 457
bhv_ttc_elevator_update		= 458
bhv_ttc_moving_bar_init		= 459
bhv_ttc_moving_bar_update		= 460
bhv_ttc_pendulum_init		= 461
bhv_ttc_pendulum_update		= 462
bhv_ttc_pit_block_init		= 463
bhv_ttc_pit_block_update		= 464
bhv_ttc_rotating_solid_init		= 465
bhv_ttc_rotating_solid_update		= 466
bhv_ttc_spinner_update		= 467
bhv_ttc_treadmill_init		= 468
bhv_ttc_treadmill_update		= 469
bhv_ttm_rolling_log_init		= 470
bhv_tumbling_bridge_loop		= 471
bhv_tumbling_bridge_platform_loop		= 472
bhv_tuxies_mother_loop		= 473
bhv_tweester_loop		= 474
bhv_tweester_sand_particle_loop		= 475
bhv_ukiki_cage_loop		= 476
bhv_ukiki_cage_star_loop		= 477
bhv_ukiki_init		= 478
bhv_ukiki_loop		= 479
bhv_unagi_init		= 480
bhv_unagi_loop		= 481
bhv_unagi_subobject_loop		= 482
bhv_unused_0e40_init		= 483
bhv_unused_particle_spawn_loop		= 484
bhv_unused_poundable_platform		= 485
bhv_vanish_cap_init		= 486
bhv_volcano_flames_loop		= 487
bhv_volcano_sound_loop		= 488
bhv_walking_penguin_loop		= 489
bhv_wall_tiny_star_particle_loop		= 490
bhv_warp_loop		= 491
bhv_water_air_bubble_init		= 492
bhv_water_air_bubble_loop		= 493
bhv_water_bomb_cannon_loop		= 494
bhv_water_bomb_shadow_update		= 495
bhv_water_bomb_spawner_update		= 496
bhv_water_bomb_update		= 497
bhv_water_drops_loop		= 498
bhv_water_level_diamond_loop		= 499
bhv_water_level_pillar_init		= 500
bhv_water_level_pillar_loop		= 501
bhv_water_mist_2_loop		= 502
bhv_water_mist_loop		= 503
bhv_water_mist_spawn_loop		= 504
bhv_water_splash_loop		= 505
bhv_water_surface_white_wave_init		= 506
bhv_water_waves_init		= 507
bhv_waterfall_sound_loop		= 508
bhv_wave_trail_loop		= 509
bhv_wdw_express_elevator_loop		= 510
bhv_wf_breakable_wall_loop		= 511
bhv_wf_elevator_tower_platform_loop		= 512
bhv_wf_rotating_wooden_platform_loop		= 513
bhv_wf_sliding_platform_init		= 514
bhv_wf_sliding_platform_loop		= 515
bhv_wf_sliding_tower_platform_loop		= 516
bhv_wf_solid_tower_platform_loop		= 517
bhv_whirlpool_init		= 518
bhv_whirlpool_loop		= 519
bhv_white_puff_1_loop		= 520
bhv_white_puff_2_loop		= 521
bhv_white_puff_exploding_loop		= 522
bhv_white_puff_smoke_init		= 523
bhv_white_wind_particle_loop		= 524
bhv_whomp_loop		= 525
bhv_wiggler_body_part_update		= 526
bhv_wiggler_update		= 527
bhv_wind_loop		= 528
bhv_wing_cap_init		= 529
bhv_wing_vanish_cap_loop		= 530
bhv_wooden_post_update		= 531
bhv_yellow_coin_init		= 532
bhv_yellow_coin_loop		= 533
bhv_yoshi_init		= 534
bhvLllVolcanoFallingTrap_loop		= 535
bhvToadMessage_init		= 536
bhvToadMessage_loop		= 537
bhvUnlockDoorStar_init		= 538
bhvUnlockDoorStar_loop		= 539
load_object_collision_model		= 540
obj_compute_vel_xz		= 541
obj_move_using_fvel_and_gravity		= 542
obj_rotate_face_angle_using_vel		= 543
obj_update_floor_and_walls		= 544
try_do_mario_debug_object_spawn		= 545
try_print_debug_mario_level_info		= 546

bhv_pole_base_loop = BehClimbDetectLoop


if geo18_title_screen != 0x36:
	raise IOError('geo18_title_screen != 0x36')

SHADOW_CIRCLE_9_VERTS = 0
SHADOW_CIRCLE_4_VERTS = 1
SHADOW_CIRCLE_4_VERTS_FLAT_UNUSED = 2
SHADOW_SQUARE_PERMANENT = 10
SHADOW_SQUARE_SCALABLE = 11
SHADOW_SQUARE_TOGGLABLE = 12
SHADOW_RECTANGLE_HARDCODED_OFFSET = 50
SHADOW_CIRCLE_PLAYER = 99
		

NULL					= 0

LAYER_FORCE             = 0
LAYER_OPAQUE            = 1
LAYER_OPAQUE_DECAL      = 2
LAYER_OPAQUE_INTER      = 3
LAYER_ALPHA             = 4
LAYER_TRANSPARENT       = 5
LAYER_TRANSPARENT_DECAL = 6
LAYER_TRANSPARENT_INTER = 7

G_NOOP			= 0x00
G_RDPHALF_2		= 0xf1
G_SETOTHERMODE_H	= 0xe3
G_SETOTHERMODE_L	= 0xe2
G_RDPHALF_1		= 0xe1
G_SPNOOP		= 0xe0
G_ENDDL			= 0xdf
G_DL			= 0xde
G_LOAD_UCODE	= 0xdd
G_MOVEMEM		= 0xdc
G_MOVEWORD		= 0xdb
G_MTX			= 0xda
G_GEOMETRYMODE	= 0xd9
G_POPMTX		= 0xd8
G_TEXTURE		= 0xd7
G_DMA_IO		= 0xd6
G_SPECIAL_1		= 0xd5
G_SPECIAL_2		= 0xd4
G_SPECIAL_3		= 0xd3

G_VTX			= 0x01
G_MODIFYVTX		= 0x02
G_CULLDL		= 0x03
G_BRANCH_Z		= 0x04
G_TRI1			= 0x05
G_TRI2			= 0x06
G_QUAD			= 0x07
G_LINE3D		= 0x08
G_VTX_BUFFER	= 0x09
G_TRI_BUFFER	= 0x0A
G_VTX_BUF_PARAM	= 0x0B

G_MV_MMTX		= 2	
G_MV_PMTX		= 6
G_MV_VIEWPORT	= 8
G_MV_LIGHT		= 10
G_MV_POINT		= 12
G_MV_MATRIX		= 14
G_MVO_LOOKATX	= (0*24)
G_MVO_LOOKATY	= (1*24)
G_MVO_L0		= (2*24)
G_MVO_L1		= (3*24)
G_MVO_L2		= (4*24)
G_MVO_L3		= (5*24)
G_MVO_L4		= (6*24)
G_MVO_L5		= (7*24)
G_MVO_L6		= (8*24)
G_MVO_L7		= (9*24)

G_SETCIMG		 = 0xff
G_SETZIMG		 = 0xfe
G_SETTIMG		 = 0xfd
G_SETCOMBINE		 = 0xfc
G_SETENVCOLOR		 = 0xfb
G_SETPRIMCOLOR		 = 0xfa
G_SETBLENDCOLOR		 = 0xf9
G_SETFOGCOLOR		 = 0xf8
G_SETFILLCOLOR		 = 0xf7
G_FILLRECT		 = 0xf6
G_SETTILE		 = 0xf5
G_LOADTILE		 = 0xf4
G_LOADBLOCK		 = 0xf3
G_SETTILESIZE		 = 0xf2
G_LOADTLUT		 = 0xf0
G_RDPSETOTHERMODE	 = 0xef
G_SETPRIMDEPTH		 = 0xee
G_SETSCISSOR		 = 0xed
G_SETCONVERT		 = 0xec
G_SETKEYR		 = 0xeb
G_SETKEYGB		 = 0xea
G_RDPFULLSYNC		 = 0xe9
G_RDPTILESYNC		 = 0xe8
G_RDPPIPESYNC		 = 0xe7
G_RDPLOADSYNC		 = 0xe6
G_TEXRECTFLIP		 = 0xe5
G_TEXRECT		 = 0xe4

G_DL_PUSH		= 0x00
G_DL_NOPUSH		= 0x01

G_CCMUX_COMBINED        = 0
G_CCMUX_TEXEL0          = 1
G_CCMUX_TEXEL1          = 2
G_CCMUX_PRIMITIVE       = 3
G_CCMUX_SHADE           = 4
G_CCMUX_ENVIRONMENT     = 5
G_CCMUX_CENTER          = 6
G_CCMUX_SCALE           = 6
G_CCMUX_COMBINED_ALPHA  = 7
G_CCMUX_TEXEL0_ALPHA    = 8
G_CCMUX_TEXEL1_ALPHA    = 9
G_CCMUX_PRIMITIVE_ALPHA = 10
G_CCMUX_SHADE_ALPHA     = 11
G_CCMUX_ENV_ALPHA       = 12
G_CCMUX_LOD_FRACTION    = 13
G_CCMUX_PRIM_LOD_FRAC   = 14
G_CCMUX_NOISE           = 7
G_CCMUX_K4              = 7
G_CCMUX_K5              = 15
G_CCMUX_1               = 6
G_CCMUX_0               = 31


G_ACMUX_COMBINED	    = 0
G_ACMUX_TEXEL0		    = 1
G_ACMUX_TEXEL1		    = 2
G_ACMUX_PRIMITIVE	    = 3
G_ACMUX_SHADE		    = 4
G_ACMUX_ENVIRONMENT	    = 5
G_ACMUX_LOD_FRACTION	= 0
G_ACMUX_PRIM_LOD_FRAC	= 6
G_ACMUX_1		        = 6
G_ACMUX_0		        = 7


G_TX_LOADTILE	= 7
G_TX_RENDERTILE	= 0
G_TX_NOMIRROR	= 0
G_TX_WRAP		= 0
G_TX_MIRROR		= 0x1
G_TX_CLAMP		= 0x2
G_TX_NOMASK		= 0
G_TX_NOLOD		= 0

G_TEXTURE_ENABLE	= 0x00000000
G_SHADING_SMOOTH	= 0x00200000
G_CULL_FRONT		= 0x00000200
G_CULL_BACK		= 0x00000400
G_CULL_BOTH		= 0x00000600


COMBINED	= 0xF000
TEXEL0		= 0xF001
TEXEL1		= 0xF002
PRIMITIVE	= 0xF003
SHADE		= 0xF004
ENVIRONMENT	= 0xF005
CENTER		= 0xF006
SCALE		= 0xF007
TEXEL0		= 0xF008
COMBINED_ALPHA	= 0xF009
TEXEL0_ALPHA	= 0xF00A
TEXEL1_ALPHA	= 0xF00B
PRIMITIVE_ALPHA	= 0xF00C
SHADE_ALPHA		= 0xF00D
ENV_ALPHA	= 0xF00E
LOD_FRACTION	= 0xF00F
PRIM_LOD_FRAC	= 0xF010
NOISE	= 0xF011
K4	= 0xF012
K5	= 0xF013

G_FOG			= 0x00010000
G_LIGHTING		= 0x00020000
G_TEXTURE_GEN		= 0x00040000
G_TEXTURE_GEN_LINEAR	= 0x00080000
G_LOD			= 0x00100000

G_IM_SIZ_4b	= 0
G_IM_SIZ_8b	= 1
G_IM_SIZ_16b	= 2
G_IM_SIZ_32b	= 3
G_IM_SIZ_DD	= 5
G_IM_SIZ_4b_BYTES		= 0
G_IM_SIZ_4b_TILE_BYTES	= G_IM_SIZ_4b_BYTES
G_IM_SIZ_4b_LINE_BYTES	= G_IM_SIZ_4b_BYTES
G_IM_SIZ_8b_BYTES		= 1
G_IM_SIZ_8b_TILE_BYTES	= G_IM_SIZ_8b_BYTES
G_IM_SIZ_8b_LINE_BYTES	= G_IM_SIZ_8b_BYTES
G_IM_SIZ_16b_BYTES		= 2
G_IM_SIZ_16b_TILE_BYTES	= G_IM_SIZ_16b_BYTES
G_IM_SIZ_16b_LINE_BYTES	= G_IM_SIZ_16b_BYTES
G_IM_SIZ_32b_BYTES		= 4
G_IM_SIZ_32b_TILE_BYTES	= 2
G_IM_SIZ_32b_LINE_BYTES	= 2
G_IM_SIZ_4b_LOAD_BLOCK	= G_IM_SIZ_16b
G_IM_SIZ_8b_LOAD_BLOCK	= G_IM_SIZ_16b
G_IM_SIZ_16b_LOAD_BLOCK	= G_IM_SIZ_16b
G_IM_SIZ_32b_LOAD_BLOCK	= G_IM_SIZ_32b
G_IM_SIZ_4b_SHIFT  = 2
G_IM_SIZ_8b_SHIFT  = 1
G_IM_SIZ_16b_SHIFT = 0
G_IM_SIZ_32b_SHIFT = 0
G_IM_SIZ_4b_INCR  = 3
G_IM_SIZ_8b_INCR  = 1
G_IM_SIZ_16b_INCR = 0
G_IM_SIZ_32b_INCR = 0

G_IM_FMT_RGBA	= 0
G_IM_FMT_YUV	= 1
G_IM_FMT_CI		= 2
G_IM_FMT_IA		= 3
G_IM_FMT_I		= 4

G_ON	= 1
G_OFF = 0
BOWTIE_VAL	= 0

def G_LOAD_BLOCK(p):
	if p == G_IM_SIZ_4b:
		return G_IM_SIZ_4b_LOAD_BLOCK
		
	if p == G_IM_SIZ_8b:
		return G_IM_SIZ_8b_LOAD_BLOCK
		
	if p == G_IM_SIZ_16b:
		return G_IM_SIZ_16b_LOAD_BLOCK
		
	if p == G_IM_SIZ_32b:
		return G_IM_SIZ_32b_LOAD_BLOCK
		
	raise IOError('unknown LOAD_BLOCK param: ' + str(p))
	
def G_LINE_BYTES(p):
	if p == G_IM_SIZ_4b:
		return G_IM_SIZ_4b_LINE_BYTES
		
	if p == G_IM_SIZ_8b:
		return G_IM_SIZ_8b_LINE_BYTES
		
	if p == G_IM_SIZ_16b:
		return G_IM_SIZ_16b_LINE_BYTES
		
	if p == G_IM_SIZ_32b:
		return G_IM_SIZ_32b_LINE_BYTES
		
	raise IOError('unknown LINE_BYTES param: ' + str(p))
	
def G_TILE_BYTES(p):
	if p == G_IM_SIZ_4b:
		return G_IM_SIZ_4b_TILE_BYTES
		
	if p == G_IM_SIZ_8b:
		return G_IM_SIZ_8b_TILE_BYTES
		
	if p == G_IM_SIZ_16b:
		return G_IM_SIZ_16b_TILE_BYTES
		
	if p == G_IM_SIZ_32b:
		return G_IM_SIZ_32b_TILE_BYTES
		
	raise IOError('unknown TILE_BYTES param: ' + str(p))
	
def G_SHIFT(p):
	if p == G_IM_SIZ_4b:
		return G_IM_SIZ_4b_SHIFT
		
	if p == G_IM_SIZ_8b:
		return G_IM_SIZ_8b_SHIFT
		
	if p == G_IM_SIZ_16b:
		return G_IM_SIZ_16b_SHIFT
		
	if p == G_IM_SIZ_32b:
		return G_IM_SIZ_32b_SHIFT
		
	raise IOError('unknown SHIFT param: ' + str(p))
	
def G_INCR(p):
	if p == G_IM_SIZ_4b:
		return G_IM_SIZ_4b_INCR
		
	if p == G_IM_SIZ_8b:
		return G_IM_SIZ_8b_INCR
		
	if p == G_IM_SIZ_16b:
		return G_IM_SIZ_16b_INCR
		
	if p == G_IM_SIZ_32b:
		return G_IM_SIZ_32b_INCR
		
	raise IOError('unknown INCR param: ' + str(p))
	
def G_BYTES(p):
	if p == G_IM_SIZ_4b:
		return G_IM_SIZ_4b_BYTES
		
	if p == G_IM_SIZ_8b:
		return G_IM_SIZ_8b_BYTES
		
	if p == G_IM_SIZ_16b:
		return G_IM_SIZ_16b_BYTES
		
	if p == G_IM_SIZ_32b:
		return G_IM_SIZ_32b_BYTES
		
	raise IOError('unknown BYTES param: ' + str(p))

def G_CCMUX(p):
	if p == COMBINED:
		return G_CCMUX_COMBINED
	if p == TEXEL0:
		return G_CCMUX_TEXEL0
	if p == TEXEL1:
		return G_CCMUX_TEXEL1
	if p == PRIMITIVE:
		return G_CCMUX_PRIMITIVE
	if p == SHADE:
		return G_CCMUX_SHADE
	if p == ENVIRONMENT:
		return G_CCMUX_ENVIRONMENT
	if p == CENTER:
		return G_CCMUX_CENTER
	if p == SCALE:
		return G_CCMUX_SCALE
	if p == COMBINED_ALPHA:
		return G_CCMUX_COMBINED_ALPHA
	if p == TEXEL0_ALPHA:
		return G_CCMUX_TEXEL0_ALPHA
	if p == TEXEL1_ALPHA:
		return G_CCMUX_TEXEL1_ALPHA
	if p == PRIMITIVE_ALPHA:
		return G_CCMUX_PRIMITIVE_ALPHA
	if p == SHADE_ALPHA:
		return G_CCMUX_SHADE_ALPHA
	if p == ENV_ALPHA:
		return G_CCMUX_ENV_ALPHA
	if p == LOD_FRACTION:
		return G_CCMUX_LOD_FRACTION
	if p == PRIM_LOD_FRAC:
		return G_CCMUX_PRIM_LOD_FRAC
	if p == NOISE:
		return G_CCMUX_NOISE
	if p == K4:
		return G_CCMUX_K4
	if p == K5:
		return G_CCMUX_K5
	if p == 1:
		return G_CCMUX_1
	if p == 0:
		return G_CCMUX_0
		
	raise IOError('unknown G_CCMUX param: ' + str(p))
	

def G_ACMUX(p):
	if p == COMBINED:
		return G_ACMUX_COMBINED
		
	if p == TEXEL0:
		return G_ACMUX_TEXEL0
		
	if p == TEXEL1:
		return G_ACMUX_TEXEL1
		
	if p == PRIMITIVE:
		return G_ACMUX_PRIMITIVE
		
	if p == SHADE:
		return G_ACMUX_SHADE
		
	if p == ENVIRONMENT:
		return G_ACMUX_ENVIRONMENT
		
	if p == LOD_FRACTION:
		return G_ACMUX_LOD_FRACTION
		
	if p == PRIM_LOD_FRAC:
		return PRIM_LOD_FRAC
		
	if p == 0:
		return G_ACMUX_0
		
	if p == 1:
		return G_ACMUX_1
		
	raise IOError('unknown G_ACMUX param: ' + str(p))
	
G_CC_PRIMITIVE              = (0, 0, 0, PRIMITIVE, 0, 0, 0, PRIMITIVE)
G_CC_SHADE                  = (0, 0, 0, SHADE, 0, 0, 0, SHADE)

G_CC_PASS2              = (0, 0, 0, COMBINED, 0, 0, 0, COMBINED)
G_CC_MODULATEI2         = (COMBINED, 0, SHADE, 0, 0, 0, 0, SHADE)
G_CC_MODULATEIA2        = (COMBINED, 0, SHADE, 0, COMBINED, 0, SHADE, 0)
G_CC_MODULATERGB2       = G_CC_MODULATEI2
G_CC_MODULATERGBA2      = G_CC_MODULATEIA2
G_CC_MODULATEI_PRIM2    = (COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE)
G_CC_MODULATEIA_PRIM2   = (COMBINED, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0)
G_CC_MODULATERGB_PRIM2  = G_CC_MODULATEI_PRIM2
G_CC_MODULATERGBA_PRIM2 = G_CC_MODULATEIA_PRIM2
G_CC_DECALRGB2          = (0, 0, 0, COMBINED, 0, 0, 0, SHADE)

G_CC_MODULATEI				= (TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE)
G_CC_MODULATEIDECALA        = (TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0)
G_CC_MODULATEIFADE          = (TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT)
G_CC_MODULATERGB            = G_CC_MODULATEI
G_CC_MODULATERGBDECALA      = G_CC_MODULATEIDECALA
G_CC_MODULATERGBFADE        = G_CC_MODULATEIFADE
G_CC_MODULATEIA             = (TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0)
G_CC_MODULATEIFADEA         = (TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0)
G_CC_MODULATEFADE           = (TEXEL0, 0, SHADE, 0, ENVIRONMENT, 0, TEXEL0, 0)
G_CC_MODULATERGBA           = G_CC_MODULATEIA
G_CC_MODULATERGBFADEA       = G_CC_MODULATEIFADEA
G_CC_MODULATEI_PRIM         = (TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE)
G_CC_MODULATEIA_PRIM        = (TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0)
G_CC_MODULATEIDECALA_PRIM   = (TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, TEXEL0)
G_CC_MODULATERGB_PRIM       = G_CC_MODULATEI_PRIM
G_CC_MODULATERGBA_PRIM      = G_CC_MODULATEIA_PRIM
G_CC_MODULATERGBDECALA_PRIM = G_CC_MODULATEIDECALA_PRIM

G_CC_FADE                    = (SHADE, 0, ENVIRONMENT, 0, SHADE, 0, ENVIRONMENT, 0)
G_CC_FADEA                   = (TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0)
G_CC_DECALRGB                = (0, 0, 0, TEXEL0, 0, 0, 0, SHADE)
G_CC_DECALRGBA               = (0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0)
G_CC_DECALFADE               = (0, 0, 0, TEXEL0, 0, 0, 0, ENVIRONMENT)
G_CC_DECALFADEA              = (0, 0, 0, TEXEL0, TEXEL0, 0, ENVIRONMENT, 0)
G_CC_BLENDI                  = (ENVIRONMENT, SHADE, TEXEL0, SHADE, 0, 0, 0, SHADE)
G_CC_BLENDIA                 = (ENVIRONMENT, SHADE, TEXEL0, SHADE, TEXEL0, 0, SHADE, 0)
G_CC_BLENDIDECALA            = (ENVIRONMENT, SHADE, TEXEL0, SHADE, 0, 0, 0, TEXEL0)
G_CC_BLENDRGBA               = (TEXEL0, SHADE, TEXEL0_ALPHA, SHADE, 0, 0, 0, SHADE)
G_CC_BLENDRGBDECALA          = (TEXEL0, SHADE, TEXEL0_ALPHA, SHADE, 0, 0, 0, TEXEL0)
G_CC_BLENDRGBFADEA           = (TEXEL0, SHADE, TEXEL0_ALPHA, SHADE, 0, 0, 0, ENVIRONMENT)
G_CC_ADDRGB                  = (TEXEL0, 0, TEXEL0, SHADE, 0, 0, 0, SHADE)
G_CC_ADDRGBDECALA            = (TEXEL0, 0, TEXEL0, SHADE, 0, 0, 0, TEXEL0)
G_CC_ADDRGBFADE              = (TEXEL0, 0, TEXEL0, SHADE, 0, 0, 0, ENVIRONMENT)
G_CC_REFLECTRGB              = (ENVIRONMENT, 0, TEXEL0, SHADE, 0, 0, 0, SHADE)
G_CC_REFLECTRGBDECALA        = (ENVIRONMENT, 0, TEXEL0, SHADE, 0, 0, 0, TEXEL0)
G_CC_HILITERGB               = (PRIMITIVE, SHADE, TEXEL0, SHADE, 0, 0, 0, SHADE)
G_CC_HILITERGBA              = (PRIMITIVE, SHADE, TEXEL0, SHADE, PRIMITIVE, SHADE, TEXEL0, SHADE)
G_CC_HILITERGBDECALA         = (PRIMITIVE, SHADE, TEXEL0, SHADE, 0, 0, 0, TEXEL0)
G_CC_SHADEDECALA             = (0, 0, 0, SHADE, 0, 0, 0, TEXEL0)
G_CC_SHADEFADEA              = (0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT)
G_CC_BLENDPE                 = (PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, SHADE, 0)
G_CC_BLENDPEDECALA           = (PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, 0, 0, 0, TEXEL0)


G_MDSFT_ALPHACOMPARE		= 0
G_MDSFT_ZSRCSEL		= 2
G_MDSFT_RENDERMODE		= 3
G_MDSFT_BLENDER		= 16
G_MDSFT_BLENDMASK		= 0
G_MDSFT_ALPHADITHER		= 4
G_MDSFT_RGBDITHER		= 6
G_MDSFT_COMBKEY		= 8
G_MDSFT_TEXTCONV		= 9
G_MDSFT_TEXTFILT		= 12
G_MDSFT_TEXTLUT		= 14
G_MDSFT_TEXTLOD		= 16
G_MDSFT_TEXTDETAIL		= 17
G_MDSFT_TEXTPERSP		= 19
G_MDSFT_CYCLETYPE		= 20
G_MDSFT_COLORDITHER		= 22
G_MDSFT_PIPELINE		= 23
G_PM_1PRIMITIVE		= (1 << G_MDSFT_PIPELINE)
G_PM_NPRIMITIVE		= (0 << G_MDSFT_PIPELINE)
G_CYC_1CYCLE		= (0 << G_MDSFT_CYCLETYPE)
G_CYC_2CYCLE		= (1 << G_MDSFT_CYCLETYPE)
G_CYC_COPY		= (2 << G_MDSFT_CYCLETYPE)
G_CYC_FILL		= (3 << G_MDSFT_CYCLETYPE)
G_TP_NONE		= (0 << G_MDSFT_TEXTPERSP)
G_TP_PERSP		= (1 << G_MDSFT_TEXTPERSP)
G_TD_CLAMP		= (0 << G_MDSFT_TEXTDETAIL)
G_TD_SHARPEN		= (1 << G_MDSFT_TEXTDETAIL)
G_TD_DETAIL		= (2 << G_MDSFT_TEXTDETAIL)
G_TL_TILE		= (0 << G_MDSFT_TEXTLOD)
G_TL_LOD		= (1 << G_MDSFT_TEXTLOD)
G_TT_NONE		= (0 << G_MDSFT_TEXTLUT)
G_TT_RGBA16		= (2 << G_MDSFT_TEXTLUT)
G_TT_IA16		= (3 << G_MDSFT_TEXTLUT)
G_TF_POINT		= (0 << G_MDSFT_TEXTFILT)
G_TF_AVERAGE		= (3 << G_MDSFT_TEXTFILT)
G_TF_BILERP		= (2 << G_MDSFT_TEXTFILT)
G_TC_CONV		= (0 << G_MDSFT_TEXTCONV)
G_TC_FILTCONV		= (5 << G_MDSFT_TEXTCONV)
G_TC_FILT		= (6 << G_MDSFT_TEXTCONV)
G_CK_NONE		= (0 << G_MDSFT_COMBKEY)
G_CK_KEY		= (1 << G_MDSFT_COMBKEY)
G_CD_MAGICSQ		= (0 << G_MDSFT_RGBDITHER)
G_CD_BAYER		= (1 << G_MDSFT_RGBDITHER)
G_CD_NOISE		= (2 << G_MDSFT_RGBDITHER)

G_CD_DISABLE		= (3 << G_MDSFT_RGBDITHER)
G_CD_ENABLE		= G_CD_NOISE

G_AD_PATTERN		= (0 << G_MDSFT_ALPHADITHER)
G_AD_NOTPATTERN		= (1 << G_MDSFT_ALPHADITHER)
G_AD_NOISE		= (2 << G_MDSFT_ALPHADITHER)
G_AD_DISABLE		= (3 << G_MDSFT_ALPHADITHER)
G_AC_NONE		= (0 << G_MDSFT_ALPHACOMPARE)
G_AC_THRESHOLD		= (1 << G_MDSFT_ALPHACOMPARE)
G_AC_DITHER		= (3 << G_MDSFT_ALPHACOMPARE)
G_ZS_PIXEL		= (0 << G_MDSFT_ZSRCSEL)
G_ZS_PRIM		= (1 << G_MDSFT_ZSRCSEL)
AA_EN		= 0x8
Z_CMP		= 0x10
Z_UPD		= 0x20
IM_RD		= 0x40
CLR_ON_CVG		= 0x80
CVG_DST_CLAMP		= 0
CVG_DST_WRAP		= 0x100
CVG_DST_FULL		= 0x200
CVG_DST_SAVE		= 0x300
ZMODE_OPA		= 0
ZMODE_INTER		= 0x400
ZMODE_XLU		= 0x800
ZMODE_DEC		= 0xc00
CVG_X_ALPHA		= 0x1000
ALPHA_CVG_SEL		= 0x2000
FORCE_BL		= 0x4000
TEX_EDGE		= 0x0000
G_BL_CLR_IN		= 0
G_BL_CLR_MEM		= 1
G_BL_CLR_BL		= 2
G_BL_CLR_FOG		= 3
G_BL_1MA		= 0
G_BL_A_MEM		= 1
G_BL_A_IN		= 0
G_BL_A_FOG		= 1
G_BL_A_SHADE		= 2
G_BL_1		= 2
G_BL_0		= 3




def	GBL_cX(x, m1a, m1b, m2a, m2b):
	if x == 1:
		return (m1a) << 30 | (m1b) << 26 | (m2a) << 22 | (m2b) << 18
	elif x == 2:
		return (m1a) << 28 | (m1b) << 24 | (m2a) << 20 | (m2b) << 16
		
def	GBL_c1(m1a, m1b, m2a, m2b):
	return GBL_cX(1, m1a, m1b, m2a, m2b)
	
def	GBL_c2(m1a, m1b, m2a, m2b):
	return GBL_cX(2, m1a, m1b, m2a, m2b)

def RM_AA_ZB_OPA_SURF(clk):
	return AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | ZMODE_OPA | ALPHA_CVG_SEL | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
	
def RM_RA_ZB_OPA_SURF(clk):
	return AA_EN | Z_CMP | Z_UPD | CVG_DST_CLAMP |	ZMODE_OPA | ALPHA_CVG_SEL |	GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
	
def RM_AA_ZB_XLU_SURF(clk):
	return AA_EN | Z_CMP | IM_RD | CVG_DST_WRAP | CLR_ON_CVG |	FORCE_BL | ZMODE_XLU |	GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_AA_ZB_OPA_DECAL(clk):
	return AA_EN | Z_CMP | IM_RD | CVG_DST_WRAP | ALPHA_CVG_SEL | ZMODE_DEC | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
	
def RM_RA_ZB_OPA_DECAL(clk):
	return AA_EN | Z_CMP | CVG_DST_WRAP | ALPHA_CVG_SEL | ZMODE_DEC | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
	
def RM_AA_ZB_XLU_DECAL(clk):
	return AA_EN | Z_CMP | IM_RD | CVG_DST_WRAP | CLR_ON_CVG | FORCE_BL | ZMODE_DEC | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_AA_ZB_OPA_INTER(clk):
	return AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | ALPHA_CVG_SEL |	ZMODE_INTER | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
	
def RM_RA_ZB_OPA_INTER(clk):
	return AA_EN | Z_CMP | Z_UPD | CVG_DST_CLAMP | ALPHA_CVG_SEL |	ZMODE_INTER | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
	
def RM_AA_ZB_XLU_INTER(clk):
	return AA_EN | Z_CMP | IM_RD | CVG_DST_WRAP | CLR_ON_CVG | FORCE_BL | ZMODE_INTER | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_AA_ZB_XLU_LINE(clk):
	return AA_EN | Z_CMP | IM_RD | CVG_DST_CLAMP | CVG_X_ALPHA | ALPHA_CVG_SEL | FORCE_BL | ZMODE_XLU | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_AA_ZB_DEC_LINE(clk):
	return AA_EN | Z_CMP | IM_RD | CVG_DST_SAVE | CVG_X_ALPHA | ALPHA_CVG_SEL | FORCE_BL | ZMODE_DEC | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_AA_ZB_TEX_EDGE(clk):
	return AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | CVG_X_ALPHA | ALPHA_CVG_SEL | ZMODE_OPA | TEX_EDGE | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
	
def RM_AA_ZB_TEX_INTER(clk):
	return AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | CVG_X_ALPHA | ALPHA_CVG_SEL | ZMODE_INTER | TEX_EDGE | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
	
def RM_AA_ZB_SUB_SURF(clk):
	return AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_FULL | ZMODE_OPA | ALPHA_CVG_SEL | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
	
def RM_AA_ZB_PCL_SURF(clk):
	return AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | ZMODE_OPA | G_AC_DITHER | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_AA_ZB_OPA_TERR(clk):
	return AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | ZMODE_OPA | ALPHA_CVG_SEL | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_AA_ZB_TEX_TERR(clk):
	return AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | CVG_X_ALPHA | ALPHA_CVG_SEL | ZMODE_OPA | TEX_EDGE |	GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_AA_ZB_SUB_TERR(clk):
	return AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_FULL | ZMODE_OPA | ALPHA_CVG_SEL | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_AA_OPA_SURF(clk):
	return AA_EN | IM_RD | CVG_DST_CLAMP | ZMODE_OPA | ALPHA_CVG_SEL | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
	
def RM_RA_OPA_SURF(clk):
	return AA_EN | CVG_DST_CLAMP | ZMODE_OPA | ALPHA_CVG_SEL |	GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
	
def RM_AA_XLU_SURF(clk):
	return AA_EN | IM_RD | CVG_DST_WRAP | CLR_ON_CVG | FORCE_BL | ZMODE_OPA | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_AA_XLU_LINE(clk):
	return AA_EN | IM_RD | CVG_DST_CLAMP | CVG_X_ALPHA | ALPHA_CVG_SEL | FORCE_BL | ZMODE_OPA |	GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_AA_DEC_LINE(clk):
	return AA_EN | IM_RD | CVG_DST_FULL | CVG_X_ALPHA | ALPHA_CVG_SEL | FORCE_BL | ZMODE_OPA | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_AA_TEX_EDGE(clk):
	return AA_EN | IM_RD | CVG_DST_CLAMP | CVG_X_ALPHA | ALPHA_CVG_SEL | ZMODE_OPA | TEX_EDGE |	GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
	
def RM_AA_SUB_SURF(clk):
	return AA_EN | IM_RD | CVG_DST_FULL | ZMODE_OPA | ALPHA_CVG_SEL | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
	
def RM_AA_PCL_SURF(clk):
	return AA_EN | IM_RD | CVG_DST_CLAMP | ZMODE_OPA | G_AC_DITHER | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_AA_OPA_TERR(clk):
	return AA_EN | IM_RD | CVG_DST_CLAMP | ZMODE_OPA | ALPHA_CVG_SEL | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_AA_TEX_TERR(clk):
	return AA_EN | IM_RD | CVG_DST_CLAMP | CVG_X_ALPHA | ALPHA_CVG_SEL | ZMODE_OPA | TEX_EDGE | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_AA_SUB_TERR(clk):
	return AA_EN | IM_RD | CVG_DST_FULL | ZMODE_OPA | ALPHA_CVG_SEL | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_ZB_OPA_SURF(clk):
	return Z_CMP | Z_UPD | CVG_DST_FULL | ALPHA_CVG_SEL | ZMODE_OPA | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
	
def RM_ZB_XLU_SURF(clk):
	return Z_CMP | IM_RD | CVG_DST_FULL | FORCE_BL | ZMODE_XLU | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_ZB_OPA_DECAL(clk):
	return Z_CMP | CVG_DST_FULL | ALPHA_CVG_SEL | ZMODE_DEC | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
	
def RM_ZB_XLU_DECAL(clk):
	return Z_CMP | IM_RD | CVG_DST_FULL | FORCE_BL | ZMODE_DEC | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_ZB_CLD_SURF(clk):
	return Z_CMP | IM_RD | CVG_DST_SAVE | FORCE_BL | ZMODE_XLU | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_ZB_OVL_SURF(clk):
	return Z_CMP | IM_RD | CVG_DST_SAVE | FORCE_BL | ZMODE_DEC | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_ZB_PCL_SURF(clk):
	return Z_CMP | Z_UPD | CVG_DST_FULL | ZMODE_OPA | G_AC_DITHER | GBL_cX(clk, G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1)
	
def RM_OPA_SURF(clk):
	return CVG_DST_CLAMP | FORCE_BL | ZMODE_OPA | GBL_cX(clk, G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1)
	
def RM_XLU_SURF(clk):
	return IM_RD | CVG_DST_FULL | FORCE_BL | ZMODE_OPA | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_TEX_EDGE(clk):
	return CVG_DST_CLAMP | CVG_X_ALPHA | ALPHA_CVG_SEL | FORCE_BL | ZMODE_OPA | TEX_EDGE | AA_EN | GBL_cX(clk, G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1)
	
def RM_CLD_SURF(clk):
	return IM_RD | CVG_DST_SAVE | FORCE_BL | ZMODE_OPA | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
	
def RM_PCL_SURF(clk):
	return CVG_DST_FULL | FORCE_BL | ZMODE_OPA | G_AC_DITHER | GBL_cX(clk, G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1)
	
def RM_ADD(clk):
	return IM_RD | CVG_DST_SAVE | FORCE_BL | ZMODE_OPA | GBL_cX(clk, G_BL_CLR_IN, G_BL_A_FOG, G_BL_CLR_MEM, G_BL_1)
	
def RM_NOOP(clk):
	return GBL_cX(clk, 0, 0, 0, 0)
	
def RM_VISCVG(clk):
	return IM_RD | FORCE_BL | GBL_cX(clk, G_BL_CLR_IN, G_BL_0, G_BL_CLR_BL, G_BL_A_MEM)

def RM_OPA_CI(clk):
	return CVG_DST_CLAMP | ZMODE_OPA | GBL_cX(clk, G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1)

def RM_CUSTOM_AA_ZB_XLU_SURF(clk):
	return RM_AA_ZB_XLU_SURF(clk) | Z_UPD




G_RM_AA_ZB_OPA_SURF			= RM_AA_ZB_OPA_SURF(1)
G_RM_AA_ZB_OPA_SURF2			= RM_AA_ZB_OPA_SURF(2)
G_RM_AA_ZB_XLU_SURF			= RM_AA_ZB_XLU_SURF(1)
G_RM_AA_ZB_XLU_SURF2			= RM_AA_ZB_XLU_SURF(2)
G_RM_AA_ZB_OPA_DECAL			= RM_AA_ZB_OPA_DECAL(1)
G_RM_AA_ZB_OPA_DECAL2			= RM_AA_ZB_OPA_DECAL(2)
G_RM_AA_ZB_XLU_DECAL			= RM_AA_ZB_XLU_DECAL(1)
G_RM_AA_ZB_XLU_DECAL2			= RM_AA_ZB_XLU_DECAL(2)
G_RM_AA_ZB_OPA_INTER			= RM_AA_ZB_OPA_INTER(1)
G_RM_AA_ZB_OPA_INTER2			= RM_AA_ZB_OPA_INTER(2)
G_RM_AA_ZB_XLU_INTER			= RM_AA_ZB_XLU_INTER(1)
G_RM_AA_ZB_XLU_INTER2			= RM_AA_ZB_XLU_INTER(2)
G_RM_AA_ZB_XLU_LINE			= RM_AA_ZB_XLU_LINE(1)
G_RM_AA_ZB_XLU_LINE2			= RM_AA_ZB_XLU_LINE(2)
G_RM_AA_ZB_DEC_LINE			= RM_AA_ZB_DEC_LINE(1)
G_RM_AA_ZB_DEC_LINE2			= RM_AA_ZB_DEC_LINE(2)
G_RM_AA_ZB_TEX_EDGE			= RM_AA_ZB_TEX_EDGE(1)
G_RM_AA_ZB_TEX_EDGE2			= RM_AA_ZB_TEX_EDGE(2)
G_RM_AA_ZB_TEX_INTER			= RM_AA_ZB_TEX_INTER(1)
G_RM_AA_ZB_TEX_INTER2			= RM_AA_ZB_TEX_INTER(2)
G_RM_AA_ZB_SUB_SURF			= RM_AA_ZB_SUB_SURF(1)
G_RM_AA_ZB_SUB_SURF2			= RM_AA_ZB_SUB_SURF(2)
G_RM_AA_ZB_PCL_SURF			= RM_AA_ZB_PCL_SURF(1)
G_RM_AA_ZB_PCL_SURF2			= RM_AA_ZB_PCL_SURF(2)
G_RM_AA_ZB_OPA_TERR			= RM_AA_ZB_OPA_TERR(1)
G_RM_AA_ZB_OPA_TERR2			= RM_AA_ZB_OPA_TERR(2)
G_RM_AA_ZB_TEX_TERR			= RM_AA_ZB_TEX_TERR(1)
G_RM_AA_ZB_TEX_TERR2			= RM_AA_ZB_TEX_TERR(2)
G_RM_AA_ZB_SUB_TERR			= RM_AA_ZB_SUB_TERR(1)
G_RM_AA_ZB_SUB_TERR2			= RM_AA_ZB_SUB_TERR(2)
G_RM_RA_ZB_OPA_SURF			= RM_RA_ZB_OPA_SURF(1)
G_RM_RA_ZB_OPA_SURF2			= RM_RA_ZB_OPA_SURF(2)
G_RM_RA_ZB_OPA_DECAL			= RM_RA_ZB_OPA_DECAL(1)
G_RM_RA_ZB_OPA_DECAL2			= RM_RA_ZB_OPA_DECAL(2)
G_RM_RA_ZB_OPA_INTER			= RM_RA_ZB_OPA_INTER(1)
G_RM_RA_ZB_OPA_INTER2			= RM_RA_ZB_OPA_INTER(2)
G_RM_AA_OPA_SURF			= RM_AA_OPA_SURF(1)
G_RM_AA_OPA_SURF2			= RM_AA_OPA_SURF(2)
G_RM_AA_XLU_SURF			= RM_AA_XLU_SURF(1)
G_RM_AA_XLU_SURF2			= RM_AA_XLU_SURF(2)
G_RM_AA_XLU_LINE			= RM_AA_XLU_LINE(1)
G_RM_AA_XLU_LINE2			= RM_AA_XLU_LINE(2)
G_RM_AA_DEC_LINE			= RM_AA_DEC_LINE(1)
G_RM_AA_DEC_LINE2			= RM_AA_DEC_LINE(2)
G_RM_AA_TEX_EDGE			= RM_AA_TEX_EDGE(1)
G_RM_AA_TEX_EDGE2			= RM_AA_TEX_EDGE(2)
G_RM_AA_SUB_SURF			= RM_AA_SUB_SURF(1)
G_RM_AA_SUB_SURF2			= RM_AA_SUB_SURF(2)
G_RM_AA_PCL_SURF			= RM_AA_PCL_SURF(1)
G_RM_AA_PCL_SURF2			= RM_AA_PCL_SURF(2)
G_RM_AA_OPA_TERR			= RM_AA_OPA_TERR(1)
G_RM_AA_OPA_TERR2			= RM_AA_OPA_TERR(2)
G_RM_AA_TEX_TERR			= RM_AA_TEX_TERR(1)
G_RM_AA_TEX_TERR2			= RM_AA_TEX_TERR(2)
G_RM_AA_SUB_TERR			= RM_AA_SUB_TERR(1)
G_RM_AA_SUB_TERR2			= RM_AA_SUB_TERR(2)
G_RM_RA_OPA_SURF			= RM_RA_OPA_SURF(1)
G_RM_RA_OPA_SURF2			= RM_RA_OPA_SURF(2)
G_RM_ZB_OPA_SURF			= RM_ZB_OPA_SURF(1)
G_RM_ZB_OPA_SURF2			= RM_ZB_OPA_SURF(2)
G_RM_ZB_XLU_SURF			= RM_ZB_XLU_SURF(1)
G_RM_ZB_XLU_SURF2			= RM_ZB_XLU_SURF(2)
G_RM_ZB_OPA_DECAL			= RM_ZB_OPA_DECAL(1)
G_RM_ZB_OPA_DECAL2			= RM_ZB_OPA_DECAL(2)
G_RM_ZB_XLU_DECAL			= RM_ZB_XLU_DECAL(1)
G_RM_ZB_XLU_DECAL2			= RM_ZB_XLU_DECAL(2)
G_RM_ZB_CLD_SURF			= RM_ZB_CLD_SURF(1)
G_RM_ZB_CLD_SURF2			= RM_ZB_CLD_SURF(2)
G_RM_ZB_OVL_SURF			= RM_ZB_OVL_SURF(1)
G_RM_ZB_OVL_SURF2			= RM_ZB_OVL_SURF(2)
G_RM_ZB_PCL_SURF			= RM_ZB_PCL_SURF(1)
G_RM_ZB_PCL_SURF2			= RM_ZB_PCL_SURF(2)
G_RM_OPA_SURF				= RM_OPA_SURF(1)
G_RM_OPA_SURF2				= RM_OPA_SURF(2)
G_RM_XLU_SURF				= RM_XLU_SURF(1)
G_RM_XLU_SURF2				= RM_XLU_SURF(2)
G_RM_CLD_SURF				= RM_CLD_SURF(1)
G_RM_CLD_SURF2				= RM_CLD_SURF(2)
G_RM_TEX_EDGE				= RM_TEX_EDGE(1)
G_RM_TEX_EDGE2				= RM_TEX_EDGE(2)
G_RM_PCL_SURF				= RM_PCL_SURF(1)
G_RM_PCL_SURF2				= RM_PCL_SURF(2)
G_RM_ADD					= RM_ADD(1)
G_RM_ADD2					= RM_ADD(2)
G_RM_NOOP					= RM_NOOP(1)
G_RM_NOOP2					= RM_NOOP(2)
G_RM_VISCVG					= RM_VISCVG(1)
G_RM_VISCVG2				= RM_VISCVG(2)
G_RM_OPA_CI							= RM_OPA_CI(1)
G_RM_OPA_CI2						= RM_OPA_CI(2)
G_RM_CUSTOM_AA_ZB_XLU_SURF			= RM_CUSTOM_AA_ZB_XLU_SURF(1)
G_RM_CUSTOM_AA_ZB_XLU_SURF2			= RM_CUSTOM_AA_ZB_XLU_SURF(2)
G_RM_FOG_SHADE_A					= GBL_c1(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_IN, G_BL_1MA)
G_RM_FOG_PRIM_A						= GBL_c1(G_BL_CLR_FOG, G_BL_A_FOG, G_BL_CLR_IN, G_BL_1MA)
G_RM_PASS							= GBL_c1(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1)


G_MW_MATRIX		= 0x00
G_MW_NUMLIGHT		= 0x02
G_MW_CLIP		= 0x04
G_MW_SEGMENT		= 0x06
G_MW_FOG		= 0x08
G_MW_LIGHTCOL		= 0x0a
G_MW_FORCEMTX		= 0x0c
G_MW_PERSPNORM		= 0x0e

G_MWO_NUMLIGHT			= 0x00
G_MWO_CLIP_RNX			= 0x04
G_MWO_CLIP_RNY			= 0x0c
G_MWO_CLIP_RPX			= 0x14
G_MWO_CLIP_RPY			= 0x1c
G_MWO_SEGMENT_0			= 0x00
G_MWO_SEGMENT_1			= 0x01
G_MWO_SEGMENT_2			= 0x02
G_MWO_SEGMENT_3			= 0x03
G_MWO_SEGMENT_4			= 0x04
G_MWO_SEGMENT_5			= 0x05
G_MWO_SEGMENT_6			= 0x06
G_MWO_SEGMENT_7			= 0x07
G_MWO_SEGMENT_8			= 0x08
G_MWO_SEGMENT_9			= 0x09
G_MWO_SEGMENT_A			= 0x0a
G_MWO_SEGMENT_B			= 0x0b
G_MWO_SEGMENT_C			= 0x0c
G_MWO_SEGMENT_D			= 0x0d
G_MWO_SEGMENT_E			= 0x0e
G_MWO_SEGMENT_F			= 0x0f
G_MWO_FOG			= 0x00
G_MWO_aLIGHT_1			= 0x00
G_MWO_bLIGHT_1			= 0x04
G_MWO_aLIGHT_2			= 0x18
G_MWO_bLIGHT_2			= 0x1c
G_MWO_aLIGHT_3			= 0x30
G_MWO_bLIGHT_3			= 0x34
G_MWO_aLIGHT_4			= 0x48
G_MWO_bLIGHT_4			= 0x4c
G_MWO_aLIGHT_5			= 0x60
G_MWO_bLIGHT_5			= 0x64
G_MWO_aLIGHT_6			= 0x78
G_MWO_bLIGHT_6			= 0x7c
G_MWO_aLIGHT_7			= 0x90
G_MWO_bLIGHT_7			= 0x94
G_MWO_aLIGHT_8			= 0xa8
G_MWO_bLIGHT_8			= 0xac
G_MWO_MATRIX_XX_XY_I			= 0x00
G_MWO_MATRIX_XZ_XW_I			= 0x04
G_MWO_MATRIX_YX_YY_I			= 0x08
G_MWO_MATRIX_YZ_YW_I			= 0x0c
G_MWO_MATRIX_ZX_ZY_I			= 0x10
G_MWO_MATRIX_ZZ_ZW_I			= 0x14
G_MWO_MATRIX_WX_WY_I			= 0x18
G_MWO_MATRIX_WZ_WW_I			= 0x1c
G_MWO_MATRIX_XX_XY_F			= 0x20
G_MWO_MATRIX_XZ_XW_F			= 0x24
G_MWO_MATRIX_YX_YY_F			= 0x28
G_MWO_MATRIX_YZ_YW_F			= 0x2c
G_MWO_MATRIX_ZX_ZY_F			= 0x30
G_MWO_MATRIX_ZZ_ZW_F			= 0x34
G_MWO_MATRIX_WX_WY_F			= 0x38
G_MWO_MATRIX_WZ_WW_F			= 0x3c
G_MWO_POINT_RGBA			= 0x10
G_MWO_POINT_ST			= 0x14
G_MWO_POINT_XYSCREEN			= 0x18
G_MWO_POINT_ZSCREEN			= 0x1c

NUMLIGHTS_0	= 1
NUMLIGHTS_1	= 1
NUMLIGHTS_2	= 2
NUMLIGHTS_3	= 3
NUMLIGHTS_4	= 4
NUMLIGHTS_5	= 5
NUMLIGHTS_6	= 6
NUMLIGHTS_7	= 7

def NUML(n):
	return ((n)*24)

'''
loads ptr

gsDma0p() gDma0p() gDma1p() gsDma1p() gsDma2p() gImmp1() gsImmp1() gImmp21() gsImmp21() gSPBranchLessZrg() gsSPBranchLessZrg() gsSPBranchLessZraw()

G_VTX			= 01
G_MODIFYVTX		= 02
G_DMA_IO		= D6
G_MTX			= DA
G_MOVEWORD		= DB
G_MOVEMEM		= DC
G_LOAD_UCODE	= DD
G_DL			= DE
G_SPNOOP		= E0 (always 0, not needed)
G_RDPHALF_1		= E1
G_RDPHALF_2		= F1

'''

G_TX_DXT_FRAC	= 11
G_TX_LDBLK_MAX_TXL	= 2047

G_TEXTURE_IMAGE_FRAC	= 2
G_TEXTURE_SCALE_FRAC	= 16
G_SCALE_FRAC		= 8
G_ROTATE_FRAC		= 16

ASSET_MAGIC = 0xFFEF464F

def MIN(a, b):
	return min(a, b)
	
def MAX(a, b):
	return max(a, b)

payloads = []

def getPayloadIndex(o):
	for i in range(len(payloads)):
		if o is payloads[i]:
			return i

	payloads.append(o)
	return len(payloads) - 1
	
def contains(lst, o):
	for a in lst:
		if a is o:
			return True
	return False

class Asset:
	def __init__(self):
		self.buffer = io.BytesIO()
		self.ref = {}
		self.offset = 0
		self.hashId = 0
		self.type = 0
		
	def isRef(self):
		return self.hashId != 0
		
	def buf(self):
		return self.buffer
		
	def size(self):
		return self.buffer.getbuffer().nbytes
		
	def write(self, v):
		if len(v) == 0:
			raise IOError('tried to write 0 bytes')
		self.buffer.write(v)
		
	def write8(self, b):
		self.write(b.to_bytes(1, byteorder='little'))
		
	def write16(self, b):
		self.write(b.to_bytes(2, byteorder='little'))
		
	def writeS16(self, b):
		self.write((b & 0xFFFF).to_bytes(2, byteorder='little', signed=False))
		
	def write32(self, b):
		self.write(b.to_bytes(4, byteorder='little'))
		
	def write64(self, b):
		try:
			self.write(b.to_bytes(8, byteorder='little'))
		except BaseException as e:
			print(str(b))
			raise
			
	def writeFloat(self, b):
		self.write(struct.pack('f',float(b)))
		
	def op(self, v):
		if isinstance(v, Asset):
			self.ref[self.size()] = v
			v = (ASSET_MAGIC << 32) #| getPayloadIndex(v)
		
		return v.to_bytes(8, byteorder='little')
			
	def fingerprint(self):
		return self.buf()
	
	def listAssets(self):
		r = [self]
		for offset, asset in self.ref.items():
			r += asset.listAssets()
			
		return r
			
	def getRefs(self, ref = False):
		r = {}
		
		if ref:
			for offset, asset in self.ref.items():
				r[id(asset)] = asset
		else:
			for offset, asset in self.ref.items():
				r[offset + self.offset] = asset
			
		for offset, asset in self.ref.items():
			r.update(asset.getRefs())
		return r
			
	def get(self, align = 0x10):
		buffer = self.buf().getbuffer().tobytes()
		
		'''
		bits = buffer.split((ASSET_MAGIC << 32).to_bytes(8, byteorder='little'))
		b = b''
		for i in bits[0:-1]:
			b += i
			b += ((ASSET_MAGIC << 32) + len(b) + self.offset).to_bytes(8, byteorder='little')
		b += bits[-1]
		
		buffer = b
		'''
		
		if align != 0:
			return buffer + ((self.size() % align) * b'\xCC')
		else:
			return buffer
		
	def rebase(self, offset):
		self.offset = offset

	def build(self, debug = False):
		assets = {}
		inlined_assets = {}
		for a in self.listAssets():
			assets[id(a)] = a
			if id(a) != id(self) and not a.isRef():
				inlined_assets[id(a)] = a

		writer = Asset()
		writer.write(self.get())
		writer.write(b'X' * 8)
		

		for hashId, o in inlined_assets.items():
			o.rebase(writer.size())
			
			try:
				writer.write(o.get())
				writer.write(b'X' * 8)
			except BaseException as e:
				print('failed to write ' + str(o))
				raise
		
		refs = self.getRefs()
		
		headerSize = len(refs) * 0x20 + 0x10
		
		header = Asset()
		header.write64(0xf229924c86e8414f)
		header.write32(len(refs))
		header.write32(headerSize)

		
		for offset,asset in refs.items():
			if debug:
				print('offset: %x' % (assets[id(asset)].offset))
			header.write8(asset.type)
			header.write(b'\x00' * 3)
			header.write32(offset + headerSize)
			header.write(b'\xFF' * 8)
			header.write64(assets[id(asset)].offset + headerSize)
			header.write64(asset.hashId)
			
		return header.get(0) + writer.get(0)
		
		
		
	def compile(self, offset):
		return self.buf().getbuffer()
		
	def save(self, path):
		base = path[0:-4]

		if base in hashMap:
			path = '%s[%s]%s' % (base, hashMap[base].upper(), path[-4:])

		with open(path, 'wb+') as f:
			f.write(self.build(False))
		
class Buffer(Asset):
	def __init__(self, a):
		super().__init__()
		if a is not None:
			for i in a:
				self.write8(i)
			
	def __add__(self, n):
		o = Buffer(None)
		o.write(self.buffer.getbuffer()[n:])
		return o
			
class Collision(Asset):
	def __init__(self, a):
		super().__init__()
		for i in a:
			if isinstance(i, list):
				for x in i:
					self.writeS16(x)
			else:
				self.writeS16(i)
				
class s16(Asset):
	def __init__(self, a):
		super().__init__()
		for i in a:
			if isinstance(i, list):
				for x in i:
					self.writeS16(x)
			else:
				self.writeS16(i)

def _SHIFTL(v, s, w):
    return (int(v) & int((0x01 << w) - 1)) << s
	
def _SHIFTR(v, s, w):
    return (v >> s) & ((0x01 << w) - 1)
	
def GCCc0w0(saRGB0, mRGB0, saA0, mA0):
	return (_SHIFTL((saRGB0), 20, 4) | _SHIFTL((mRGB0), 15, 5) | _SHIFTL((saA0), 12, 3) | _SHIFTL((mA0), 9, 3))
	
def GCCc1w0(saRGB1, mRGB1):
	return (_SHIFTL((saRGB1), 5, 4) | _SHIFTL((mRGB1), 0, 5))

def GCCc0w1(sbRGB0, aRGB0, sbA0, aA0):
	return (_SHIFTL((sbRGB0), 28, 4) | _SHIFTL((aRGB0), 15, 3) | _SHIFTL((sbA0), 12, 3) | _SHIFTL((aA0), 9, 3))

def GCCc1w1(sbRGB1, saA1, mA1, aRGB1, sbA1, aA1):			\
	return (_SHIFTL((sbRGB1), 24, 4) | _SHIFTL((saA1), 21, 3) |	_SHIFTL((mA1), 18, 3) | _SHIFTL((aRGB1), 6, 3) | _SHIFTL((sbA1), 3, 3) | _SHIFTL((aA1), 0, 3))
	
def TXL2WORDS(txls, b_txl):
	return MAX(1, ((txls)*(b_txl)/8))

def CALC_DXT(width, b_txl):
	return (((1 << G_TX_DXT_FRAC) + TXL2WORDS(width, b_txl) - 1) / TXL2WORDS(width, b_txl))

def TXL2WORDS_4b(txls):
	return MAX(1, ((txls)/16))

def CALC_DXT_4b(width):
	return (((1 << G_TX_DXT_FRAC) + TXL2WORDS_4b(width) - 1) / TXL2WORDS_4b(width))
	
def alignSizeOffset(sz):
	return ((sz // 0x10) * 0x10 + 0x10) - sz
	
	
def linkScript(script):
	return script

class Ambient(Asset):
	def __init__(self, c):
		super().__init__()
		self.write8(c[0])
		self.write8(c[1])
		self.write8(c[2])
		self.write8(0)
		
		self.write8(c[0])
		self.write8(c[1])
		self.write8(c[2])
		self.write8(0)
		
class Light(Asset):
	def __init__(self, c, d):
		super().__init__()
		self.write8(c[0])
		self.write8(c[1])
		self.write8(c[2])
		self.write8(0)
		
		self.write8(c[0])
		self.write8(c[1])
		self.write8(c[2])
		self.write8(0)
		
		self.write8(d[0])
		self.write8(d[1])
		self.write8(d[2])
		self.write8(0)
		
		self.write8(0)
		self.write8(0)
		self.write8(0)
		self.write8(0)
		
		self.c = c
		self.d = d
		
	def __getitem__(self, key):
		return self.c[key]
		
		
class Lights1(Asset):
	def __init__(self, a, l1c, l1d):
		super().__init__()
		self.l = [Light(l1c, l1d)]
		self.a = Ambient(a)

class Geo(Asset):
	def __init__(self):
		super().__init__()
		
	def CMD_BBH(self, a, b, c):
		self.buffer.write(self.op(_SHIFTL(a, 0, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(c, 16, 16)))
		
	def CMD_HH(self, a, b):
		self.buffer.write(self.op(_SHIFTL(a, 0, 16) | _SHIFTL(b, 16, 16)))
		
	def CMD_W(self, a):
		self.buffer.write(self.op(a))
		
	def CMD_HHHHHH(self, a, b, c, d, e, f):
		self.CMD_HH(a, b)
		self.CMD_HH(c, d)
		self.CMD_HH(e, f)
		
	def GEO_BILLBOARD_WITH_PARAMS(self, layer, tx, ty, tz):
		self.CMD_BBH(0x14, layer, tx)
		self.CMD_HH(ty, tz)
		
	def GEO_BILLBOARD_WITH_PARAMS_AND_DL(self, layer, tx, ty, tz, displayList):
		self.CMD_BBH(0x14, (layer | 0x80), tx)
		self.CMD_HH(ty, tz)
		self.CMD_PTR(displayList)
		
	def GEO_BILLBOARD(self):
		self.GEO_BILLBOARD_WITH_PARAMS(0, 0, 0, 0)
		
	def GEO_NODE_START(self):
		self.CMD_BBH(0x0B, 0x00, 0x0000)
		
	def GEO_ZBUFFER(self, enable):
		self.CMD_BBH(0x0C, enable, 0x0000)
		
	def GEO_RENDER_RANGE(self, minDistance, maxDistance):
		self.CMD_BBH(0x0D, 0x00, 0x0000)
		self.CMD_HH(minDistance, maxDistance)
		
	def GEO_CAMERA(self, type, x1, y1, z1, x2, y2, z2, function):
		self.CMD_BBH(0x0F, 0x00, type)
		self.CMD_HHHHHH(x1, y1, z1, x2, y2, z2)
		self.CMD_PTR(function)
		
	def GEO_CAMERA_FRUSTUM_WITH_FUNC(self, fov, near, far, func):
		self.CMD_BBH(0x0A, 0x01, fov)
		self.CMD_HH(near, far)
		self.CMD_PTR(func)
		
	def GEO_HELD_OBJECT(self, param, ux, uy, uz, nodeFunc):
		self.CMD_BBH(0x1C, param, ux)
		self.CMD_HH(uy, uz)
		self.CMD_PTR(nodeFunc)
		
	def GEO_OPEN_NODE(self):
		self.CMD_BBH(0x04, 0x00, 0x0000)
		
	def GEO_CLOSE_NODE(self):
		self.CMD_BBH(0x05, 0x00, 0x0000)
		
	def GEO_ASSIGN_AS_VIEW(self, index):
		self.CMD_BBH(0x06, 0x00, index)
		
	def GEO_UPDATE_NODE_FLAGS(self, operation, flagBits):
		self.CMD_BBH(0x07, operation, flagBits)
		
	def GEO_NODE_SCREEN_AREA(self, numEntries, x, y, width, height):
		self.CMD_BBH(0x08, 0x00, numEntries)
		self.CMD_HH(x, y)
		self.CMD_HH(width, height)
		
	def GEO_NODE_ORTHO(self, scale):
		self.CMD_BBH(0x09, 0x00, scale)
		
	def GEO_RETURN(self):
		self.CMD_BBH(0x03, 0x00, 0x0000)
		
	def GEO_SCALE(self, layer, scale):
		self.CMD_BBH(0x1D, layer, 0x0000)
		self.CMD_W(scale)

	def GEO_SCALE_WITH_DL(self, layer, scale, displayList):
		self.CMD_BBH(0x1D, (layer | 0x80), 0x0000)
		self.CMD_W(scale)
		self.CMD_PTR(displayList)
		
	def GEO_ROTATE_Y(self, layer, ry):
		self.CMD_BBH(0x10, (0x30 | layer), ry)

	def GEO_ROTATE_Y_WITH_DL(self, layer, ry, displayList):
		self.CMD_BBH(0x10, (0x30 | layer | 0x80), ry)
		self.CMD_PTR(displayList)
		
	def GEO_TRANSLATE_NODE(self, layer, ux, uy, uz):
		self.CMD_BBH(0x11, layer, ux)
		self.CMD_HH(uy, uz)
		
	def GEO_TRANSLATE_NODE_WITH_DL(self, layer, ux, uy, uz, displayList):
		self.CMD_BBH(0x11, (layer | 0x80), ux)
		self.CMD_HH(uy, uz)
		self.CMD_PTR(displayList)
		
	def GEO_ROTATE(self, layer, rx, ry, rz):
		self.CMD_BBH(0x10, (0x20 | layer), rx), 
		self.CMD_HH(ry, rz)
		
	def GEO_ROTATE_WITH_DL(self, layer, rx, ry, rz, displayList):
		self.CMD_BBH(0x10, (0x20 | layer | 0x80), rx)
		self.CMD_HH(ry, rz)
		self.CMD_PTR(displayList)
		
	def GEO_ANIMATED_PART(self, layer, x, y, z, displayList):
		self.CMD_BBH(0x13, layer, x)
		self.CMD_HH(y, z)
		self.CMD_PTR(displayList)
		
	def GEO_TRANSLATE(self, layer, tx, ty, tz):
		self.CMD_BBH(0x10, (0x10 | layer), tx)
		self.CMD_HH(ty, tz)
		
	def GEO_TRANSLATE_WITH_DL(self, layer, tx, ty, tz, displayList):
		self.CMD_BBH(0x10, (0x10 | layer | 0x80), tx)
		self.CMD_HH(ty, tz)
		self.CMD_PTR(displayList)
		
	def GEO_TRANSLATE_ROTATE(self, layer, tx, ty, tz, rx, ry, rz):
		self.CMD_BBH(0x10, (0x00 | layer), 0x0000)
		self.CMD_HHHHHH(tx, ty, tz, rx, ry, rz)
	
	def GEO_TRANSLATE_ROTATE_WITH_DL(self, layer, tx, ty, tz, rx, ry, rz, displayList):
		self.CMD_BBH(0x10, (0x00 | layer | 0x80), 0x0000)
		self.CMD_HHHHHH(tx, ty, tz, rx, ry, rz)
		self.CMD_PTR(displayList)
		
	def GEO_SWITCH_CASE(self, count, function):
		self.CMD_BBH(0x0E, 0x00, count)
		self.CMD_PTR(function)
		
	def GEO_BRANCH_AND_LINK(self, scriptTarget):
		self.CMD_BBH(0x00, 0x00, 0x0000)
		self.CMD_PTR(scriptTarget)

	def GEO_END(self):
		self.CMD_BBH(0x01, 0x00, 0x0000)

	def GEO_BRANCH(self, type, scriptTarget):
		self.CMD_BBH(0x02, type, 0x0000)
		self.CMD_PTR(linkScript(scriptTarget))
		
	def GEO_DISPLAY_LIST(self, layer, displayList):
		self.CMD_BBH(0x15, layer, 0x0000)
		self.CMD_PTR(displayList)
		
	def CMD_PTR(self, a):
		self.buffer.write(self.op(a))
		
	def GEO_ASM(self, param, function):
		self.CMD_BBH(0x18, 0x00, param)
		self.CMD_PTR(function)
		
	def GEO_ROTATION_NODE(self, layer, ux, uy, uz):
		self.CMD_BBH(0x12, layer, ux)
		self.CMD_HH(uy, uz)
		
	def GEO_ROTATION_NODE_WITH_DL(self, layer, ux, uy, uz, displayList):
		self.CMD_BBH(0x12, (layer | 0x80), ux)
		self.CMD_HH(uy, uz)
		self.CMD_PTR(displayList)
		
	def GEO_SHADOW(self, type, solidity, scale):
		self.CMD_BBH(0x16, 0x00, type)
		self.CMD_HH(solidity, scale)
		
	def GEO_RENDER_OBJ(self):
		self.CMD_BBH(0x17, 0x00, 0x0000)
		
	def GEO_BACKGROUND(self, background, function):
		self.CMD_BBH(0x19, 0x00, background)
		self.CMD_PTR(function)
		
	def GEO_BACKGROUND_COLOR(self, background):
		self.GEO_BACKGROUND(background, NULL)
		
	def GEO_NOP_1A(self):
		self.CMD_BBH(0x1A, 0x00, 0x0000)
		self.CMD_HH(0x0000, 0x0000)
		
	def GEO_NOP_1E(self):
		self.CMD_BBH(0x1E, 0x00, 0x0000)
		self.CMD_HH(0x0000, 0x0000)
		
	def GEO_NOP_1F(self):
		self.CMD_BBH(0x1F, 0x00, 0x0000)
		self.CMD_HH(0x0000, 0x0000)
		self.CMD_HH(0x0000, 0x0000)
		self.CMD_HH(0x0000, 0x0000)
		
	def GEO_COPY_VIEW(index):
		self.CMD_BBH(0x1B, 0x00, index)
		
	def GEO_CULLING_RADIUS(self, cullingRadius):
		self.CMD_BBH(0x20, 0x00, cullingRadius)
		
	def compile(self, offset):
		result = b''
		suffix = b''
		
		sz = self.buffer.getbuffer().nbytes
		alignOffset = alignSizeOffset(sz)
		totalSize = sz + alignOffset
		
		
		for opcode in memoryview(self.buffer.getbuffer()).cast('Q'):
			if (opcode >> 32) == ASSET_MAGIC:
				index = (opcode & 0xFFFFFFFF)
				suffix += b'\xCC' * alignSizeOffset(len(suffix))
				
				payloadBase = offset + len(suffix) + totalSize
				opcode = (ASSET_MAGIC << 32) | payloadBase
				suffix += payloads[index].compile(payloadBase)

			result += opcode.to_bytes(8, byteorder='little')
			
		result += b'\xCD' * alignOffset
		result += suffix
		return result
	'''	
	def save(self, path):
		base = path[0:-4]

		if base in hashMap:
			path = '%s[%s]%s' % (base, hashMap[base].upper(), path[-4:])
		with open(path, 'wb+') as f:
			f.write(self.compile(0))
	'''

'''
	float		ob[3];	/* x, y, z */
	unsigned short	flag;
	short		tc[2];	/* texture coord */
	signed char	n[3];	/* normal */
	unsigned char   a;      /* alpha  */
'''
def floatBytes(f):
	return struct.pack('f',float(f))

def shortBytes(s):
	s = int(s)
	return s.to_bytes(2, byteorder='little')
	
def bite(b):
	b = int(b)
	return b.to_bytes(1, byteorder='little')
	
def gsSP1Triangle_w1(v0, v1, v2):
	return (_SHIFTL((v0)*2,16,8)|_SHIFTL((v1)*2,8,8)|_SHIFTL((v2)*2,0,8))
	 
def gsSP1Triangle_w1f(v0, v1, v2, flag):
	if flag == 0:
		return gsSP1Triangle_w1(v0, v1, v2)
	elif flag == 1:
		return gsSP1Triangle_w1(v1, v2, v0)
	else:
		return gsSP1Triangle_w1(v2, v0, v1)
		
		
class Bhv(Asset):
	def __init__(self):
		super().__init__()
		
	def BC_B(self, a):
		self.write64(_SHIFTL(a, 24, 8))
		
	def BC_BB(self, a, b):
		self.write64(_SHIFTL(a, 24, 8) | _SHIFTL(b, 16, 8))
		
	def BC_BBBB(self, a, b, c, d):
		self.write64(_SHIFTL(a, 24, 8) | _SHIFTL(b, 16, 8) | _SHIFTL(c, 8, 8) | _SHIFTL(d, 0, 8))
		
	def BC_BBH(self, a, b, c):
		self.write64(_SHIFTL(a, 24, 8) | _SHIFTL(b, 16, 8) | _SHIFTL(c, 0, 16))
		
	def BC_B0H(self, a, b):
		self.write64(_SHIFTL(a, 24, 8) | _SHIFTL(b, 0, 16))

	def BC_H(self, a):
		self.write64(_SHIFTL(a, 16, 16))
		
	def BC_HH(self, a, b):
		self.write64(_SHIFTL(a, 16, 16) | _SHIFTL(b, 0, 16))
		
	def BC_W(self, a):
		self.write64(a)
		
	def BC_PTR(self, a):
		self.write(self.op(a))
		
	def BEGIN(self, objList):
		self.BC_BB(0x00, objList)

	def DELAY(self, frames):
		self.BC_B0H(0x01, frames)

	def CALL(self, addr):
		self.BC_B(0x02), 
		self.BC_PTR(addr)

	def RETURN(self):
		self.BC_B(0x03)

	def GOTO(self, addr):
		self.BC_B(0x04), 
		self.BC_PTR(addr)

	def BEGIN_REPEAT(self, times):
		self.BC_B0H(0x05, times)

	def END_REPEAT(self):
		self.BC_B(0x06)

	def END_REPEAT_CONTINUE(self):
		self.BC_B(0x07)


	def BEGIN_LOOP(self):
		self.BC_B(0x08)

	def END_LOOP(self):
		self.BC_B(0x09)

	def BREAK(self):
		self.BC_B(0x0A)

	def CALL_NATIVE(self, addr):
		self.BC_B(0x0C), 
		self.BC_PTR(addr)

	def ADD_FLOAT(self, field, value):
		self.BC_BBH(0x0D, field, value)


	def SET_FLOAT(self, field, value):
		self.BC_BBH(0x0E, field, value)


	def ADD_INT(self, field, value):
		self.BC_BBH(0x0F, field, value)


	def SET_INT(self, field, value):
		self.BC_BBH(0x10, field, value)



	def OR_INT(self, field, value):
		self.BC_BBH(0x11, field, value)


	def BIT_CLEAR_INT(self, field, value):
		self.BC_BBH(0x12, field, value)


	def SET_INT_RAND_RSHIFT(self, field, min, rshift):
		self.BC_BBH(0x13, field, min), 
		self.BC_H(rshift)


	def SET_RANDOM_FLOAT(self, field, min, max):
		self.BC_BBH(0x14, field, min), 
		self.BC_H(max)


	def SET_RANDOM_INT(self, field, min, max):
		self.BC_BBH(0x15, field, min), 
		self.BC_H(max)


	def ADD_RANDOM_FLOAT(self, field, min, max):
		self.BC_BBH(0x16, field, min), 
		self.BC_H(max)



	def ADD_INT_RAND_RSHIFT(self, field, min, rshift):
		self.BC_BBH(0x17, field, min), 
		self.BC_H(rshift)


	def SET_MODEL(self, modelID):
		self.BC_B0H(0x1B, modelID)


	def SPAWN_CHILD(self, modelID, bhv):
		self.BC_B(0x1C), 
		self.BC_W(modelID), 
		self.BC_PTR(bhv)

	def DEACTIVATE(self):
		self.BC_B(0x1D)


	def DROP_TO_FLOOR(self):
		self.BC_B(0x1E)


	def SUM_FLOAT(self, fieldDest, fieldSrc1, fieldSrc2):
		self.BC_BBBB(0x1F, fieldDest, fieldSrc1, fieldSrc2)


	def SUM_INT(self, fieldDest, fieldSrc1, fieldSrc2):
		self.BC_BBBB(0x20, fieldDest, fieldSrc1, fieldSrc2)


	def BILLBOARD(self):
		self.BC_B(0x21)


	def HIDE(self):
		self.BC_B(0x22)


	def SET_HITBOX(self, radius, height):
		self.BC_B(0x23), 
		self.BC_HH(radius, height)


	def DELAY_VAR(self, field):
		self.BC_BB(0x25, field)


	def LOAD_ANIMATIONS(self, field, value):
		self.BC_BB(0x27, field), 
		self.BC_PTR(value[0])


	def ANIMATE(self, animIndex):
		self.BC_BB(0x28, animIndex)


	def SPAWN_CHILD_WITH_PARAM(self, behParam, modelID, bhv):
		self.BC_B0H(0x29, behParam), 
		self.BC_W(modelID), 
		self.BC_PTR(bhv)


	def LOAD_COLLISION_DATA(self, addr):
		self.BC_B(0x2A), 
		self.BC_PTR(addr)


	def SET_HITBOX_WITH_OFFSET(self, radius, height, downOffset):
		self.BC_B(0x2B), 
		self.BC_HH(radius, height), 
		self.BC_H(downOffset)


	def SPAWN_OBJ(self, modelID, bhv):
		self.BC_B(0x2C), 
		self.BC_W(modelID), 
		self.BC_PTR(bhv)


	def SET_HOME(self):
		self.BC_B(0x2D)


	def SET_HURTBOX(self, radius, height):
		self.BC_B(0x2E), 
		self.BC_HH(radius, height)


	def SET_INTERACT_TYPE(self, type):
		self.BC_B(0x2F), 
		self.BC_PTR(type)


	def SET_OBJ_PHYSICS(self, wallHitboxRadius, gravity, bounciness, drag, friction, buoyancy, unused1, unused2):
		self.BC_B(0x30)
		self.BC_HH(wallHitboxRadius, gravity)
		self.BC_HH(bounciness, drag)
		self.BC_HH(friction, buoyancy)
		self.BC_HH(unused1, unused2)


	def SCALE(self, percent):
		self.BC_B0H(0x32, percent)


	def BIT_CLEAR_INT32(self, field, value):
		self.BC_BB(0x33, field)
		self.BC_W(value)


	def TEX_ANIM_RATE(self, field, rate):
		self.BC_BBH(0x34, field, rate)


	def DISABLE_RENDERING(self):
		self.BC_B(0x35)


	def SPAWN_WATER_SPLASH(self, params):
		self.BC_B(0x37)
		self.BC_PTR(params)
		
	'''
	def save(self, path):
		base = path[0:-4]

		if base in hashMap:
			path = '%s[%s]%s' % (base, hashMap[base].upper(), path[-4:])
		with open(path, 'wb+') as f:
			f.write(self.compile(0))
	'''
	
'''
/*
 * SetCombineMode macros are NOT redunant. It allow the C preprocessor
 * to substitute single parameter which includes commas in the token and
 * rescan for higher parameter count macro substitution.
 *
 * eg.	gsDPSetCombineMode(G_CC_MODULATE, G_CC_MODULATE) turns into
 *	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0,
 *		TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0)
 */
 '''
class Gfx(Asset):
	def __init__(self):
		super().__init__()
		self.tri_buffer = Asset()
		self.vtx_buffer = None
		self.vtx_offset = 0
		
	def gsMoveWd(self, index, offset, data):
		self.gsDma1p(G_MOVEWORD, data, offset, index)
		
	def gsSPNumLights(self, n):
		self.gsMoveWd(G_MW_NUMLIGHT, G_MWO_NUMLIGHT, NUML(n))
		
	def gsSPBranchList(self, dl):
		self.gsDma1p(G_DL, dl, 0, G_DL_NOPUSH)
		
	def gsSPSetLights1(self, name):
		self.gsSPNumLights(NUMLIGHTS_1)
		self.gsSPLight(name.l[0],1)
		self.gsSPLight(name.a,2)
		
	def gsSPSetOtherMode(self, cmd, sft, len, data):
		self.write64(_SHIFTL(cmd,24,8)|_SHIFTL(32-(sft)-(len),8,8)|_SHIFTL((len)-1,0,8))
		self.write64(data)
		
	def gsSPFogPosition(self, min, max):
		self.gsMoveWd(G_MW_FOG, G_MWO_FOG, (_SHIFTL((128000/((max)-(min))),16,16) |	_SHIFTL(((500-(min))*256/((max)-(min))),0,16)))
		
	def gsDPSetCycleType(self, type):
		self.gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_CYCLETYPE, 2, type)
		
	def gsDPSetTextureFilter(self, type):
		self.gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_TEXTFILT, 2, type)

	def gsDPSetTextureLUT(self, type):
		self.gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_TEXTLUT, 2, type)
		
	def gsDPLoadTLUTCmd(self, tile, count):
		self.write64(_SHIFTL(G_LOADTLUT, 24, 8))
		self.write64(_SHIFTL((tile), 24, 3) | _SHIFTL((count), 14, 10))
		

	def gsDPSetAlphaCompare(self, type):
		self.gsSPSetOtherMode(G_SETOTHERMODE_L, G_MDSFT_ALPHACOMPARE, 2, type)

	def	gsDPSetDepthSource(self, src):
		self.gsSPSetOtherMode(G_SETOTHERMODE_L, G_MDSFT_ZSRCSEL, 1, src)

	def	gsDPSetRenderMode(self, c0, c1):
		self.gsSPSetOtherMode(G_SETOTHERMODE_L, G_MDSFT_RENDERMODE, 29, 	(c0) | (c1))
		
	def gsDPSetColor(self, c, d):
		self.write64(_SHIFTL(c, 24, 8))
		self.write64(d)
		
	def DPRGBColor(self, pkt, cmd, r, g, b, a):
		self.gDPSetColor(pkt, cmd, (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)))

	def sDPRGBColor(self, cmd, r, g, b, a):
		self.gsDPSetColor(cmd, (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)))
				
	def gsDPSetEnvColor(self, r, g, b, a):
		self.sDPRGBColor(G_SETENVCOLOR, r,g,b,a)
				
				
	def gsDPSetBlendColor(self, r, g, b, a):
		self.sDPRGBColor(G_SETBLENDCOLOR, r,g,b,a)
				
				
	def gsDPSetFogColor(self, r, g, b, a):
		self.sDPRGBColor(G_SETFOGCOLOR, r,g,b,a)
		
				
	def gsDPSetFillColor(self, d):
		self.gsDPSetColor(G_SETFILLCOLOR, (d))

					
	def gsDPSetPrimDepth(self, z, dz):
		self.gsDPSetColor(G_SETPRIMDEPTH, _SHIFTL(z, 16, 16) | _SHIFTL(dz, 0, 16))
		
	def gsSPTexture(self, s, t, level, tile, on):
		self.write64(_SHIFTL(G_TEXTURE,24,8) | _SHIFTL(BOWTIE_VAL,16,8) | _SHIFTL((level),11,3) | _SHIFTL((tile),8,3) | _SHIFTL((on),1,7))
		self.write64(_SHIFTL((s),16,16) | _SHIFTL((t),0,16))
		
	def gsSPSetGeometryMode(self, word):
		self.gsSPGeometryMode(0,(word))
	def gsSPClearGeometryMode(self, word):
		self.gsSPGeometryMode((word),0)
	def gsSPLoadGeometryMode(self, word):
		self.gsSPGeometryMode(-1,(word))
	def gsSPGeometryModeSetFirst(self, c, s):
		self.gsSPGeometryMode(c, s)

		
	def gsDPSetTile(self, fmt, siz, line, tmem, tile, palette, cmt, maskt, shiftt, cms, masks, shifts):
		self.write64(_SHIFTL(G_SETTILE, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL(line, 9, 9) | _SHIFTL(tmem, 0, 9))
		self.write64(_SHIFTL(tile, 24, 3) | _SHIFTL(palette, 20, 4) | _SHIFTL(cmt, 18, 2) | _SHIFTL(maskt, 14, 4) | _SHIFTL(shiftt, 10, 4) | _SHIFTL(cms, 8, 2) | _SHIFTL(masks, 4, 4) | _SHIFTL(shifts, 0, 4))
		
	def gsSetImage(self, cmd, fmt, siz, width, i):
		self.write64(_SHIFTL(cmd, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL((width)-1, 0, 12))
		self.write(self.op(i))
		
	def gsDPSetTextureImage(self, f, s, w, i):
		self.gsSetImage(G_SETTIMG, f, s, w, i)
		
	def gsDPLoadBlock(self, tile, uls, ult, lrs, dxt):
		self.write64(_SHIFTL(G_LOADBLOCK, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12))
		self.write64(_SHIFTL(tile, 24, 3) | _SHIFTL((MIN(lrs,G_TX_LDBLK_MAX_TXL)), 12, 12) | _SHIFTL(dxt, 0, 12))

	def gsDPLoadTileGeneric(self, c, tile, uls, ult, lrs, lrt):
		self.write64(_SHIFTL(c, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12))
		self.write64(_SHIFTL(tile, 24, 3) | _SHIFTL(lrs, 12, 12) | _SHIFTL(lrt, 0, 12))
		
	def gDPSetTileSize(self, pkt, t, uls, ult, lrs, lrt):
		self.gDPLoadTileGeneric(pkt, G_SETTILESIZE, t, uls, ult, lrs, lrt)
		
	def gsDPSetTileSize(self, t, uls, ult, lrs, lrt):
		self.gsDPLoadTileGeneric(G_SETTILESIZE, t, uls, ult, lrs, lrt)
		
	def gDPLoadTile(self, pkt, t, uls, ult, lrs, lrt):
		self.gDPLoadTileGeneric(pkt, G_LOADTILE, t, uls, ult, lrs, lrt)
		
	def gsDPLoadTile(self, t, uls, ult, lrs, lrt):
		self.gsDPLoadTileGeneric(G_LOADTILE, t, uls, ult, lrs, lrt)
		
	def gsDPLoadTextureBlock(self, timg, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt):
		self.gsDPSetTextureImage(fmt, G_LOAD_BLOCK(siz), 1, timg)
		self.gsDPSetTile(fmt, G_LOAD_BLOCK(siz), 0, 0, G_TX_LOADTILE, 0 , cmt, maskt,	shiftt, cms, masks, shifts)
		self.gsDPLoadSync()
		self.gsDPLoadBlock(G_TX_LOADTILE, 0, 0, (((width)*(height) + G_INCR(siz)) >> G_SHIFT(siz))-1, CALC_DXT(width, G_BYTES(siz)))
		self.gsDPPipeSync()
		self.gsDPSetTile(fmt, siz, ((((width) * G_LINE_BYTES(siz))+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts)
		self.gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC)
		
	def gsDPNoParam(self, cmd):
		self.write64(_SHIFTL(cmd, 24, 8))
		self.write64(0)
		

	def gsDPFullSync(self):
		self.gsDPNoParam(G_RDPFULLSYNC)
		
	def gsDPTileSync(self):
		self.gsDPNoParam(G_RDPTILESYNC)
		
	def gsDPPipeSync(self):
		self.gsDPNoParam(G_RDPPIPESYNC)
		
	def gsDPLoadSync(self):
		self.gsDPNoParam(G_RDPLOADSYNC)
		
	def gsDPNoOp(self):
		self.gsDPNoParam(G_NOOP)
		
	def gsDPNoOpTag(self, tag):
		self.gsDPParam(G_NOOP, tag)
		
	def gsDPLoadSync(self):
		self.gsDPNoParam(G_RDPLOADSYNC)

	def gsDPSetCombineLERP(self, a0, b0, c0, d0, Aa0, Ab0, Ac0, Ad0, a1, b1, c1, d1, Aa1, Ab1, Ac1, Ad1):
		self.write64(_SHIFTL(G_SETCOMBINE, 24, 8) | _SHIFTL(GCCc0w0(G_CCMUX(a0), G_CCMUX(c0), G_ACMUX(Aa0), G_ACMUX(Ac0)) | GCCc1w0(G_CCMUX(a1), G_CCMUX(c1)), 0, 24))
		self.write64(GCCc0w1(G_CCMUX(b0), G_CCMUX(d0), G_ACMUX(Ab0), G_ACMUX(Ad0)) | GCCc1w1(G_CCMUX(b1), G_ACMUX(Aa1), G_ACMUX(Ac1), G_CCMUX(d1), G_ACMUX(Ab1), G_ACMUX(Ad1)))
		
	def gsSPGeometryMode(self, c, s):
		self.write64(_SHIFTL(G_GEOMETRYMODE,24,8) | _SHIFTL(~int(c),0,24))
		self.write64(int(s))
		
	def gsDPSetCombineMode(self, a, b):
		self.gsDPSetCombineLERP(*a, *b)
		
	def	gsDma2p(self, c, adrs, len, idx, ofs):
		self.write64(_SHIFTL((c),24,8) | _SHIFTL(((len)-1)/8,19,5) | _SHIFTL((ofs)/8,8,8) | _SHIFTL((idx),0,8))
		self.write(self.op(adrs))
		
	def gsDma1p(self, c, s, l, p):
		self.write64(_SHIFTL((c), 24, 8) | _SHIFTL((p), 16, 8) | _SHIFTL((l), 0, 16))
		self.write(self.op(s))

	def gsDPPipeSync(self):
		self.gsDPNoParam(G_RDPPIPESYNC)
		
	def gsSPVertex(self, v, n, v0):
		if OPTIMIZE_VERTICIES:
			self.flushVerticies(v, n, v0)
		else:
			self.buffer.write(self.op( _SHIFTL(G_VTX,24,8)|_SHIFTL((n),12,8)|_SHIFTL((v0)+(n),1,7) ))
			
			if n != v.vertexCount():
				v = v.slice(0, n)
			self.buffer.write(self.op(v))

	def gsSP2Triangles(self, v00, v01, v02, flag0, v10, v11, v12, flag1):
		if not OPTIMIZE_TRIANGLES:
			self.buffer.write(self.op(_SHIFTL(G_TRI2, 24, 8) | gsSP1Triangle_w1f(v00, v01, v02, flag0)))
			self.buffer.write(self.op(gsSP1Triangle_w1f(v10, v11, v12, flag1)))
		else:
			self.tri_buffer.write32(v00 + self.vtx_offset)
			self.tri_buffer.write32(v01 + self.vtx_offset)
			self.tri_buffer.write32(v02 + self.vtx_offset)
			self.tri_buffer.write32(v10 + self.vtx_offset)
			self.tri_buffer.write32(v11 + self.vtx_offset)
			self.tri_buffer.write32(v12 + self.vtx_offset)
		
	def gsSP1Triangle(self, v0, v1, v2, flag):
		if not OPTIMIZE_TRIANGLES:
			self.buffer.write(self.op( _SHIFTL(G_TRI1, 24, 8) | gsSP1Triangle_w1f(v0, v1, v2, flag) ))
			self.buffer.write(self.op(0))
		else:
			self.tri_buffer.write32(v0 + self.vtx_offset)
			self.tri_buffer.write32(v1 + self.vtx_offset)
			self.tri_buffer.write32(v2 + self.vtx_offset)
	
	def logVertexBuffer(self):
		#print('VERTEX BUFFER %d' % self.vtx_buffer.vertexCount())
		pass
		
	def flushVerticies(self, v = None, n = None, v0 = None):
		if not OPTIMIZE_VERTICIES:
			self.flushTriangles()
		else:
			if v is None:
				self.flushTriangles()
				return

			if v0 != 0:
				print('param not 0: %d at offset %d' % (v0, v.vertexOffset()))
			if v.vertexParent is None:
				self.buffer.write(self.op( _SHIFTL(G_VTX,24,8)|_SHIFTL((n),12,8)|_SHIFTL((v0)+(n),1,7) ))
			
				if n != v.vertexCount():
					v = v.slice(0, n)
				self.buffer.write(self.op(v))
				
				self.vtx_buffer = None
				self.vtx_offset = 0
			else:
				if self.vtx_buffer is None:
					self.buffer.write(self.op( _SHIFTL(G_VTX_BUF_PARAM,24,8)))
					self.buffer.write(v.vertexRoot().vertexCount().to_bytes(4, byteorder='little'))
					self.buffer.write(v0.to_bytes(4, byteorder='little'))
					self.buffer.write(self.op( _SHIFTL(G_VTX_BUFFER,24,8)|_SHIFTL((v.vertexRoot().vertexCount()),0,24) ))
					self.buffer.write(self.op(v.vertexRoot()))
					self.vtx_buffer = v.vertexRoot()
					self.logVertexBuffer()
				self.vtx_offset = v.vertexOffset()

		
	def flushTriangles(self):
		if not OPTIMIZE_TRIANGLES:
			return
		if self.tri_buffer.size() > 0:
			#print('TRIANGLE BUFFER %d' % (self.tri_buffer.size() // (3*4)))
			if (self.tri_buffer.size() // (3*4)) == 70:
				with open('bleh.bin', 'wb') as f:
					f.write(self.tri_buffer.get(0))
			self.buffer.write(self.op(_SHIFTL(G_TRI_BUFFER, 24, 8) | _SHIFTL((self.tri_buffer.size() // (3*4)), 0, 24)))
			self.write(self.op(self.tri_buffer))
		self.tri_buffer = Asset()
		
	def gsSPEndDisplayList(self):
		self.flushVerticies()
		self.buffer.write(self.op( _SHIFTL(G_ENDDL, 24, 8) ))
		self.buffer.write(self.op(0))
		
	def gsSPDisplayList(self, dl):
		self.gsDma1p(G_DL, dl, 0, G_DL_PUSH)
		
	def gsSPLight(self, l, n):
		if isinstance(l, list):
			l = l[0]
		self.gsDma2p(G_MOVEMEM, l, 0x10, G_MV_LIGHT, n*24+24)
		
	def compile(self, offset):
		result = b''
		suffix = b''
		
		sz = self.buffer.getbuffer().nbytes
		alignOffset = alignSizeOffset(sz)
		totalSize = sz + alignOffset
		
		
		for opcode in memoryview(self.buffer.getbuffer()).cast('Q'):
			if (opcode >> 32) == ASSET_MAGIC:
				index = (opcode & 0xFFFFFFFF)
				suffix += b'\xCC' * alignSizeOffset(len(suffix))
				
				payloadBase = offset + len(suffix) + totalSize
				opcode = (ASSET_MAGIC << 32) | payloadBase
				suffix += payloads[index].compile(payloadBase)

			result += opcode.to_bytes(8, byteorder='little')
			
		result += b'\xCD' * alignOffset
		result += suffix
		return result
		
	'''
	def save(self, path):
		base = path[0:-4]

		if base in hashMap:
			path = '%s[%s]%s' % (base, hashMap[base].upper(), path[-4:])
		with open(path, 'wb+') as f:
			f.write(self.compile(0))
	'''

class Mcr(Asset):
	def __init__(self):
		super().__init__()
		
	def MACRO_OBJECT_WITH_BEH_PARAM(self, preset, yaw, posX, posY, posZ, beh):
		self.writeS16(((yaw * 0x10 // 45) << 9) | (preset + 0x1F))
		self.writeS16(posX)
		self.writeS16(posY)
		self.writeS16(posZ)
		self.writeS16(beh)
		
	def MACRO_OBJECT(self, preset, yaw, posX, posY, posZ):
		self.MACRO_OBJECT_WITH_BEH_PARAM(preset, yaw, posX, posY, posZ, 0)
		
	def MACRO_OBJECT_END(self):
		self.write16(0x1E)
		
	'''
	def save(self, path):
		base = path[0:-4]

		if base in hashMap:
			path = '%s[%s]%s' % (base, hashMap[base].upper(), path[-4:])

		with open(path, 'wb+') as f:
			f.write(self.buffer.getbuffer())
	'''

	
class Vtx(Asset):
	def __init__(self):
		super().__init__()
		self.vertexParent = None
		self._vertexOffset = 0
		
	def vertexRoot(self):
		if self.vertexParent is not None:
			return self.vertexParent
		return self
		
	def vertexOffset(self):
		return self._vertexOffset

	def __add__(self, n):
		o = Vtx()
		o.write(self.buffer.getbuffer()[n * 24:])
		o.vertexParent = self
		o._vertexOffset = n
		return o
		
	def vertexCount(self):
		return self.buffer.getbuffer().nbytes // 24
		
	def slice(self, start, n):
		r = Vtx()
		r.write(self.buffer.getbuffer()[start*24:(start+n)*24])
		return r
		
	def push(self, a):
		self.writeFloat(a[0][0])
		self.writeFloat(a[0][1])
		self.writeFloat(a[0][2])
		
		self.writeS16(a[1])
		self.writeS16(a[2][0])
		self.writeS16(a[2][1])
		
		self.write8(a[3][0])
		self.write8(a[3][1])
		self.write8(a[3][2])
		self.write8(a[3][3])
		self.buffer.write(b'\x00\x00')
		
def searchPaths(path):
	r = [path, os.path.join(rootDir, path)]
	
	for b in ['./', 'build/us_pc']:
		r.append(os.path.join(os.path.join(os.path.join(rootDir, b), 'levels'), path))
		r.append(os.path.join(os.path.join(os.path.join(rootDir, b), 'actors'), path))
		
	r.append(os.path.abspath(os.path.basename(path)))
	
	for f in r:
		if os.path.exists(f):
			return [f]
	return []
	return [path, os.path.join(rootDir, path), os.path.join(os.path.join(rootDir, 'levels'), path), os.path.join(os.path.join(rootDir, 'actors'), path), os.path.join('../', path), os.path.join('../../', path), os.path.abspath(os.path.basename(path))]
		
if __name__ == '__main__':
	import sys
	
	gfxList = []
	geoList = []
	bhvList = []
	lvlList = []
	mcrList = []
	geoMagic = 'GeoLayout '
	vtxMagic = 'Vtx '
	gfxMagic = 'Gfx '
	bhvMagic = 'BehaviorScript '
	lvlMagic = 'LevelScript '
	mcrMagic = 'MacroObject '
	
	lights1Magic = 'Lights1 '
	bufferMagic = 'u8 '	
	
	types = ['Collision', 'Animation', 's16']
	
	
	print('from sm64 import *\n\n')
	
	def open_source(file_path):
		currentVariable = None
		mode = None
		lastMode = None
		
		global gfxList
		global geoList
		global bhvList
		global lvlList
		global mcrList
		
		lines = []
		
		with open(file_path, 'r') as f:
			for line in f.read().replace(');', '\n);').replace(';', ';\n').replace('\r', '\n').split('\n'):
				i = line.find('//')
				
				if i >= 0:
					line = line[0:i]
					
				line = re.sub(re.compile("/\*.*?\*/",re.DOTALL ) ,"" , line)
				line = line.replace('(const u8*)', '')
				
				line = line.strip()
				
				if line.startswith('UNUSED '):
					line = line[len('UNUSED '):].strip()
				
				if line.startswith('ALIGNED8 '):
					line = line[len('ALIGNED8 '):].strip()
				
				if line.startswith('static '):
					line = line[len('static '):].strip()
					
				if line.startswith('const '):
					line = line[len('const '):].strip()
					
				if line.startswith('struct '):
					line = line[len('struct '):].strip()
					
				if line.startswith('extern '):
					continue
				
				if len(line) == 0:
					continue

				if (line[0] == '}' or line[0] == ')') and line[1] == ';':
					if currentVariable[0] == '*':
						pass
					elif mode == 'geo':
						geoList.append(currentVariable)
						for line in lines:
							if line.startswith('GEO_'):
								if currentVariable is None:
									raise IOError('variable name not set: ' + line)
								if line[-1] == ',':
									line = line[0:-1]
								print('%s.%s' % (currentVariable, line))
							else:
								raise IOError('unknown line: ' + line + ', path: ' + file_path)
					elif mode == 'gfx':
						gfxList.append(currentVariable)
						for line in lines:
							if line.startswith('gs'):
								if currentVariable is None:
									raise IOError('variable name not set: ' + line)

								if line[-1] == ',':
									line = line[0:-1]
								print('%s.%s' % (currentVariable, line.replace('&', '')))
							else:
								raise IOError('unknown line: ' + line + ', path: ' + file_path)
					elif mode == 'bhv':
						bhvList.append(currentVariable)
						for line in lines:
							if currentVariable is None:
								raise IOError('variable name not set: ' + line)

							if line[-1] == ',':
								line = line[0:-1]
							print('%s.%s' % (currentVariable, line.replace('&', '')))
					elif mode == 'lvl':
						lvlList.append(currentVariable)
						for line in lines:
							if currentVariable is None:
								raise IOError('variable name not set: ' + line)

							if line[-1] == ',':
								line = line[0:-1]
							print('%s.%s' % (currentVariable, line.replace('&', '')))
					elif mode == 'mcr':
						mcrList.append(currentVariable)
						for line in lines:
							if currentVariable is None:
								raise IOError('variable name not set: ' + line)

							if line[-1] == ',':
								line = line[0:-1]
							print('%s.%s' % (currentVariable, line.replace('&', '')))
					elif mode == 'vtx':
						for line in lines:
							print('%s.push(%s)' % (currentVariable, line.replace('{', '[').replace('}', ']')[1:-2]))
					elif mode == 'lights1':
						numbers = (' '.join(lines)).split(',')
						if len(numbers) != 9:
							raise IOError('not enough light1 params: ' + (''.join(lines).replace(' ', '')))
						print('%s = Lights1([%s, %s, %s], [%s, %s, %s], [%s, %s, %s])' % (currentVariable, numbers[0], numbers[1], numbers[2], numbers[3], numbers[4], numbers[5], numbers[6], numbers[7], numbers[8]))
					elif mode == 'buffer':
						print('%s = Buffer([%s])' % (currentVariable, ' '.join(lines)))
					elif mode == 'Animation':
						pass
					else:
						print('%s = %s([%s])' % (currentVariable, mode, ' '.join(lines)))

					lines = []
					lastMode = mode
					mode = None
					print('\n\n')
				elif line.startswith(geoMagic):
					line = line[len(geoMagic):]
					i = line.find('[')
					if i < 0:
						raise IOError('unknown line: ' + line + ', path: ' + file_path)
					currentVariable = line[0:i]
					print('%s = Geo()' % currentVariable)
					mode = 'geo'
					
				elif line.startswith(bhvMagic):
					line = line[len(bhvMagic):]
					i = line.find('[')
					if i < 0:
						raise IOError('unknown line: ' + line + ', path: ' + file_path)
					currentVariable = line[0:i]
					print('%s = Bhv()' % currentVariable)
					mode = 'bhv'
					
				elif line.startswith(lvlMagic):
					line = line[len(lvlMagic):]
					i = line.find('[')
					if i < 0:
						raise IOError('unknown line: ' + line + ', path: ' + file_path)
					currentVariable = line[0:i]
					print('%s = Lvl()' % currentVariable)
					mode = 'lvl'
					
				elif line.startswith(vtxMagic):
					line = line[len(vtxMagic):]
					i = line.find('[')
					if i < 0:
						raise IOError('unknown line: ' + line + ', path: ' + file_path)
					currentVariable = line[0:i]
					print('%s = Vtx()' % currentVariable)
					mode = 'vtx'
					
				elif line.startswith(gfxMagic):
					line = line[len(gfxMagic):]
					i = line.find('[')
					if i < 0:
						raise IOError('unknown line: ' + line + ', path: ' + file_path)
					currentVariable = line[0:i]
					print('%s = Gfx()' % currentVariable)
					mode = 'gfx'
					
				elif line.startswith(lights1Magic):
					line = line[len(lights1Magic):]
					i = line.find('=')
					if i < 0:
						raise IOError('unknown line: ' + line + ', path: ' + file_path)
					currentVariable = line[0:i].strip()
					mode = 'lights1'
				elif line.startswith(mcrMagic):
					line = line[len(mcrMagic):]
					i = line.find('[')
					if i < 0:
						raise IOError('unknown line: ' + line + ', path: ' + file_path)
					currentVariable = line[0:i]
					print('%s = Mcr()' % currentVariable)
					mode = 'mcr'
				elif line.startswith(bufferMagic):
					line = line[len(bufferMagic):]
					i = line.find('[')
					if i < 0:
						raise IOError('unknown line: ' + line + ', path: ' + file_path)
					currentVariable = line[0:i].strip()
					mode = 'buffer'
				else:
					found = False
					for t in types:
						if line.startswith(t + ' '):
							line = line[len(t)+1:]
							i = line.find('[')
							if i < 0:
								raise IOError('unknown line: ' + line + ', path: ' + file_path)
							currentVariable = line[0:i].strip()
							mode = t
							found = True
							break
					if found:
						continue
					if mode == None:
						if line.startswith('#include'):
							if line.strip() == '#include "src/game/envfx_snow.h"':
								continue
							line = line[len('#include '):].strip()
							for tryPath in searchPaths(line[1:-1]):
								try:
									open_source(tryPath)
								except:
									raise
							continue
						elif line.startswith('u64 '):
							continue
						elif line[0] == '#':
							continue
						elif line.startswith('namespace'):
							break
						elif '_unused_' in line:
							continue
						raise IOError('bad line ' + line + ', path: ' + file_path)
					line = line.strip()
					
					if line.startswith('#include '):
						line = line[len('#include '):].strip()
						
						tryPaths = searchPaths(line[1:-1])
						success = False
						for tryPath in tryPaths:
							try:
								with open(tryPath, 'r') as f2:
									for line in f2.readlines():
										lines.append(line)
									success = True
							except:
								success = False
								
							if success:
								break
						if not success and len(tryPaths) != 0:
							raise IOError('Could not open: ' + str(tryPaths))
					else:
						lines.append(line)
						
	for file_path in sys.argv[1:]:
		open_source(file_path)
		

	for currentVariable in gfxList:
		print('%s.save(\'%s.gfx\')' % (currentVariable, currentVariable))
		
	for currentVariable in geoList:
		print('%s.save(\'%s.geo\')' % (currentVariable, currentVariable))
		
	for currentVariable in bhvList:
		print('%s.save(\'%s.bhv\')' % (currentVariable, currentVariable))
		
	for currentVariable in lvlList:
		print('%s.save(\'%s.lvl\')' % (currentVariable, currentVariable))
		
	for currentVariable in mcrList:
		print('%s.save(\'%s.mcr\')' % (currentVariable, currentVariable))
			