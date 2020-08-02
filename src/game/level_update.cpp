#include <ultra64.h>

#include "sm64.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "audio/external.h"
#include "level_update.h"
#include "game.h"
#include "level_update.h"
#include "main.h"
#include "engine/math_util.h"
#include "engine/graph_node.h"
#include "area.h"
#include "save_file.h"
#include "sound_init.h"
#include "mario.h"
#include "camera.h"
#include "object_list_processor.h"
#include "ingame_menu.h"
#include "obj_behaviors.h"
#include "display.h"
#include "save_file.h"
#include "game/options.h"
#include "debug_course.h"
#include "level_table.h"
#include "player/players.h"

#include "game/ending.h"
#include "game/motor.h"

#define GM_NORMAL 0
#define GM_GAMEOVER 1
#define GM_PAUSE 2
#define GM_FREEZE 3
#define GM_EXIT 4
#define GM_STILL 5

#define WARP_TYPE_NOT_WARPING 0
#define CHANGE_STAGE 1
#define CHANGE_SCENE 2
#define CHANGE_PORT 3

#define WARP_NODE_F0 0xF0
#define WARP_NODE_DEATH 0xF1
#define WARP_NODE_F2 0xF2
#define WARP_NODE_WARP_FLOOR 0xF3
#define WARP_NODE_CREDITS_START 0xF8
#define WARP_NODE_CREDITS_NEXT 0xF9
#define WARP_NODE_CREDITS_END 0xFA

#define WARP_NODE_CREDITS_MIN 0xF8

PlayerRecord playerWorks[1];
PlayerRecord* marioWorks = &playerWorks[0];

u8 unused1[4] = {0};

s8 middlePointSw = 0;

s16 gameProcess;
u16 gameCounter;

s16 freezeCounter;
void (*freezeProcedure)(s16*);

u8 unused3[4];

WarpDest playerEntry;

s16 gameExitCode;

s16 fadeoutMode;
s16 fadeoutCount;
s16 fadeoutPort;
s32 fadeoutInfo;

u8 unused4[2];

s8 stopWatchSw;

HudDisplay playerMeter;

s8 mesgCastle;

static void ExecuteGame(s16* arg);

u16 GmStopWatch(s32 mode)
{
	switch(mode)
	{
		case TIMER_CONTROL_SHOW:
			playerMeter.flags |= HUD_DISPLAY_FLAG_TIMER;
			stopWatchSw	  = FALSE;
			playerMeter.timer = 0;
			break;

		case TIMER_CONTROL_START:
			stopWatchSw = TRUE;
			break;

		case TIMER_CONTROL_STOP:
			stopWatchSw = FALSE;
			break;

		case TIMER_CONTROL_HIDE:
			playerMeter.flags &= ~HUD_DISPLAY_FLAG_TIMER;
			stopWatchSw	  = FALSE;
			playerMeter.timer = 0;
			break;
	}

	return playerMeter.timer;
}

static u32 CheckPauseRequest(void)
{
	u32 DispMesg = GetMessageNo() >= 0;
	u32 demoMode = (marioWorks->status & ACT_FLAG_INTANGIBLE) != 0;

	if(!demoMode && !DispMesg && !wipeControl.isActive && fadeoutMode == WARP_OP_NONE && (sm64::player(0).controller().buttonPressed & sm64::hid::START_BUTTON))
	{
		return TRUE;
	}

	return FALSE;
}

static void ChangeProcess(s16 playMode)
{
	gameProcess = playMode;
	gameCounter = 0;
}

static void ChangeExitMode(s32 arg)
{
	gameProcess  = GM_EXIT;
	gameCounter  = 0;
	gameExitCode = arg;
}

void PL_ExitGameMode(u32 code, u32 color)
{
	if(color != 0)
	{
		color = 0xFF;
	}

	AudStopMusic(190);
	SnStartFader(WARP_TRANSITION_FADE_INTO_COLOR, 0x10, color, color, color);
	FreezeGame(30, NULL);

	ChangeExitMode(code);
}

static void CheckReset(void)
{
}

void PL_DisplayMessage(u32 mesgno)
{
	s32 condition;
	u32 dialogID = snSceneInfo->dialog[mesgno];

	switch(dialogID)
	{
		case DIALOG_129:
			condition = BuGetItemFlag() & SAVE_FLAG_HAVE_VANISH_CAP;
			break;

		case DIALOG_130:
			condition = BuGetItemFlag() & SAVE_FLAG_HAVE_METAL_CAP;
			break;

		case DIALOG_131:
			condition = BuGetItemFlag() & SAVE_FLAG_HAVE_WING_CAP;
			break;

		case 255:
			condition = TRUE;
			break;

		default:
			condition = BuGetStarFlag(activePlayerNo - 1, activeCourseNo - 1);
			break;
	}

	if(!condition)
	{
		FreezeGame(-1, NULL);
		CallMessage(dialogID);
	}
}

void MoveToFrontDoor(struct SpawnInfo* actor, u32 arg1)
{
	if(arg1 & 0x00000002)
	{
		actor->startAngle[1] += ANGLE180;
	}

	actor->startPos[0] += 300.0f * sins(actor->startAngle[1]);
	actor->startPos[2] += 300.0f * coss(actor->startAngle[1]);
}

static void ChangeSpecialCap(struct PlayerRecord* m)
{
	u32 capCourseIndex = activeCourseNo - COURSE_CAP_COURSES;

	switch(capCourseIndex)
	{
		case COURSE_COTMC - COURSE_CAP_COURSES:
			m->flags |= MARIO_METAL_CAP | MARIO_CAP_ON_HEAD;
			m->capTimer = 600 * FRAME_RATE_SCALER_INV;
			break;

		case COURSE_TOTWC - COURSE_CAP_COURSES:
			m->flags |= MARIO_WING_CAP | MARIO_CAP_ON_HEAD;
			m->capTimer = 1200 * FRAME_RATE_SCALER_INV;
			break;

		case COURSE_VCUTM - COURSE_CAP_COURSES:
			m->flags |= MARIO_VANISH_CAP | MARIO_CAP_ON_HEAD;
			m->capTimer = 600 * FRAME_RATE_SCALER_INV;
			break;
	}
}

static void ChangeStatus(struct PlayerRecord* player, u32 type, u32 auxinfo)
{
	switch(type)
	{
		case SN_TRIP_DOOR:
			player->ChangePlayerStatus(ACT_WARP_DOOR_SPAWN, auxinfo);
			break;
		case SN_TRIP_STAR:
			player->ChangePlayerStatus(ACT_IDLE, 0);
			break;
		case SN_TRIP_CHIMNEY:
			player->ChangePlayerStatus(ACT_EMERGE_FROM_PIPE, 0);
			break;
		case SN_WARP_POINT:
			player->ChangePlayerStatus(ACT_TELEPORT_FADE_IN, 0);
			break;
		case SN_ENTER_WAITING:
			player->ChangePlayerStatus(ACT_IDLE, 0);
			break;
		case SN_ENTER_LANDING:
			player->ChangePlayerStatus(ACT_SPAWN_NO_SPIN_AIRBORNE, 0);
			break;
		case SN_ENTER_FALLING:
			player->ChangePlayerStatus(ACT_HARD_BACKWARD_AIR_KB, 0);
			break;
		case SN_ENTER_ROLLING:
			player->ChangePlayerStatus(ACT_SPAWN_SPIN_AIRBORNE, 0);
			break;
		case SN_ENTER_DOWNING:
			player->ChangePlayerStatus(ACT_FALLING_DEATH_EXIT, 0);
			break;
		case SN_ENTER_PICTURE:
			player->ChangePlayerStatus(ACT_SPAWN_SPIN_AIRBORNE, 0);
			break;
		case SN_ENTER_FLIGHT:
			player->ChangePlayerStatus(ACT_FLYING, 2);
			break;
		case SN_ENTER_SWIMMING:
			player->ChangePlayerStatus(ACT_WATER_IDLE, 1);
			break;
		case SN_ENTER_WINNER:
			player->ChangePlayerStatus(ACT_EXIT_AIRBORNE, 0);
			break;
		case SN_ENTER_LOSER:
			player->ChangePlayerStatus(ACT_DEATH_EXIT, 0);
			break;
		case SN_ENTER_LANDWINNER:
			player->ChangePlayerStatus(ACT_FALLING_EXIT_AIRBORNE, 0);
			break;
		case SN_ENTER_LANDLOSER:
			player->ChangePlayerStatus(ACT_UNUSED_DEATH_EXIT, 0);
			break;
		case SN_ENTER_PUSHOUT:
			player->ChangePlayerStatus(ACT_SPECIAL_EXIT_AIRBORNE, 0);
			break;
		case SN_ENTER_PUSHDOWN:
			player->ChangePlayerStatus(ACT_SPECIAL_DEATH_EXIT, 0);
			break;
	}

	ChangeSpecialCap(player);
}

static void ChangePort()
{
	ObjectWarpNode* port = SnGetPortPtr(playerEntry.nodeId);
	u32 type	     = SnGetPortType(port->object);

	if(marioWorks->status != ACT_UNINITIALIZED)
	{
		starringActor[0].startPos[0] = (s16)port->object->oPosX;
		starringActor[0].startPos[1] = (s16)port->object->oPosY;
		starringActor[0].startPos[2] = (s16)port->object->oPosZ;

		starringActor[0].startAngle[0] = 0;
		starringActor[0].startAngle[1] = port->object->oMoveAngleYaw;
		starringActor[0].startAngle[2] = 0;

		if(type == SN_TRIP_DOOR)
		{
			MoveToFrontDoor(&starringActor[0], playerEntry.arg);
		}

		if(playerEntry.type == CHANGE_STAGE || playerEntry.type == CHANGE_SCENE)
		{
			starringActor[0].areaIndex = playerEntry.areaIdx;
			SnEnterPlayer();
		}

		marioWorks->initPlayer();
		ChangeStatus(marioWorks, type, playerEntry.arg);

		marioWorks->interactObj = port->object;
		marioWorks->usedObj	= port->object;
	}

	InitGameCamera(snSceneInfo->camera);
	playerEntry.type = WARP_TYPE_NOT_WARPING;
	fadeoutMode	 = WARP_OP_NONE;

	switch(type)
	{
		case SN_TRIP_CHIMNEY:
			SnStartFader(WARP_TRANSITION_FADE_FROM_STAR, 0x10, 0x00, 0x00, 0x00);
			break;
		case SN_TRIP_DOOR:
			SnStartFader(WARP_TRANSITION_FADE_FROM_CIRCLE, 0x10, 0x00, 0x00, 0x00);
			break;
		case SN_WARP_POINT:
			SnStartFader(WARP_TRANSITION_FADE_FROM_COLOR, 0x14, 0xFF, 0xFF, 0xFF);
			break;
		case SN_ENTER_PICTURE:
			SnStartFader(WARP_TRANSITION_FADE_FROM_COLOR, 0x1A, 0xFF, 0xFF, 0xFF);
			break;
		case SN_ENTER_ROLLING:
			SnStartFader(WARP_TRANSITION_FADE_FROM_CIRCLE, 0x10, 0x00, 0x00, 0x00);
			break;
		case SN_ENTER_STAFFROLL:
			SnStartFader(WARP_TRANSITION_FADE_FROM_COLOR, 0x10, 0x00, 0x00, 0x00);
			break;
		default:
			SnStartFader(WARP_TRANSITION_FADE_FROM_STAR, 0x10, 0x00, 0x00, 0x00);
			break;
	}

	if(autoDemoPtr == NULL)
	{
		AudPlayMusic(snSceneInfo->musicParam, snSceneInfo->musicParam2, 0);

		if(marioWorks->flags & MARIO_METAL_CAP)
		{
			AudPlaySpecialMusic(SEQUENCE_ARGS(4, SEQ_EVENT_METAL_CAP));
		}

		if(marioWorks->flags & (MARIO_VANISH_CAP | MARIO_WING_CAP))
		{
			AudPlaySpecialMusic(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP));
		}

		if(activeStageNo == LEVEL_BOB && Na_GetPlayingBgmFlag() != SEQUENCE_ARGS(4, SEQ_LEVEL_SLIDE) && stopWatchSw != 0)
		{
			Na_MusicStart(0, SEQUENCE_ARGS(4, SEQ_LEVEL_SLIDE), 0);
		}

		if(playerEntry.levelNum == LEVEL_CASTLE && playerEntry.areaIdx == 1 && (playerEntry.nodeId == 31 || playerEntry.nodeId == 32))
		{
			AudStartSound(SOUND_MENU_MARIO_CASTLE_WARP, gDefaultSoundArgs);
		}

		if(playerEntry.levelNum == 16 && playerEntry.areaIdx == 1 && (playerEntry.nodeId == 7 || playerEntry.nodeId == 10 || playerEntry.nodeId == 20 || playerEntry.nodeId == 30))
		{
			AudStartSound(SOUND_MENU_MARIO_CASTLE_WARP, gDefaultSoundArgs);
		}
	}
}

// used for warps inside one level
static void ChangeScene(void)
{
	if(playerEntry.type != WARP_TYPE_NOT_WARPING)
	{
		if(playerEntry.type == CHANGE_SCENE)
		{
			GmStopWatch(TIMER_CONTROL_HIDE);
			SnExitPlayer();
			SnOpenScene(playerEntry.areaIdx);
		}

		ChangePort();
	}
}

// used for warps between levels
static void ChangeStage(void)
{
	activeStageNo = playerEntry.levelNum;

	GmStopWatch(TIMER_CONTROL_HIDE);

	SnOpenScene(playerEntry.areaIdx);
	ChangePort();
}

static void ChangeEndingStage(void)
{
	s32 status;

	switch(playerEntry.nodeId)
	{
		case WARP_NODE_CREDITS_START:
			status = ACT_END_PEACH_CUTSCENE;
			break;

		case WARP_NODE_CREDITS_NEXT:
			status = ACT_CREDITS_CUTSCENE;
			break;

		case WARP_NODE_CREDITS_END:
			status = ACT_END_WAVING_CUTSCENE;
			break;
	}

	activeStageNo = playerEntry.levelNum;

	SnOpenScene(playerEntry.areaIdx);

	SetSVector(starringActor[0].startPos, snEndingScene->marioPos[0], snEndingScene->marioPos[1], snEndingScene->marioPos[2]);

	SetSVector(starringActor[0].startAngle, 0, 0x100 * snEndingScene->marioAngle, 0);

	starringActor[0].areaIndex = playerEntry.areaIdx;

	SnEnterPlayer();
	marioWorks->initPlayer();

	marioWorks->ChangePlayerStatus(status, 0);

	InitGameCamera(snSceneInfo->camera);

	playerEntry.type = WARP_TYPE_NOT_WARPING;
	fadeoutMode	 = WARP_OP_NONE;

	SnStartFader(WARP_TRANSITION_FADE_FROM_COLOR, 0x14, 0x00, 0x00, 0x00);

	if(snEndingScene == NULL || snEndingScene == sCreditsSequence)
	{
		AudPlayMusic(snSceneInfo->musicParam, snSceneInfo->musicParam2, 0);
	}
}

static void CheckSceneConnection(void)
{
	s16 panning;
	Surface* plane;

	if(activeStageNo == LEVEL_CASTLE && BuGetSumStars(activePlayerNo - 1, COURSE_MIN - 1, COURSE_MAX - 1) >= 70)
	{
		return;
	}

	if((plane = marioWorks->floor) != NULL)
	{
		s32 index = plane->type - SURFACE_INSTANT_WARP_1B;
		if(index >= INSTANT_WARP_INDEX_START && index < INSTANT_WARP_INDEX_STOP && snSceneInfo->instantWarps != NULL)
		{
			InstantWarp* warp = &snSceneInfo->instantWarps[index];

			if(warp->id != 0)
			{
				marioWorks->pos[0] += warp->displacement[0];
				marioWorks->pos[1] += warp->displacement[1];
				marioWorks->pos[2] += warp->displacement[2];

				marioWorks->marioObj->oPosX = marioWorks->pos[0];
				marioWorks->marioObj->oPosY = marioWorks->pos[1];
				marioWorks->marioObj->oPosZ = marioWorks->pos[2];

				panning = marioWorks->area->camera->yaw;

				SnChangeScene(warp->area);
				marioWorks->area = snSceneInfo;

				Crou_camBlockofs(warp->displacement[0], warp->displacement[1], warp->displacement[2]);

				marioWorks->area->camera->yaw = panning;
			}
		}
	}
}

static s16 CheckSameMusic(s16 arg)
{
	ObjectWarpNode* port = SnGetPortPtr(arg);
	s16 stage	     = port->node.destLevel & 0x7F;

#if BUGFIX_KOOPA_RACE_MUSIC

	s16 scene  = port->node.destArea;
	s16 result = TRUE;

	if(stage == LEVEL_BOB && stage == activeStageNo && scene == activeSceneNo)
	{
		s16 music = Na_GetPlayingBgmFlag();
		if(music == SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP | SEQ_VARIATION) || music == SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP))
		{
			result = 0;
		}
	}
	else
	{
		u16 mode  = stageScenes[scene].musicParam;
		u16 music = stageScenes[scene].musicParam2;

		result = stage == activeStageNo && mode == snSceneInfo->musicParam && music == snSceneInfo->musicParam2;

		if(Na_GetPlayingBgmFlag() != music)
		{
			result = FALSE;
		}
	}
	return result;

#else

	u16 mode  = stageScenes[port->node.scene].musicParam;
	u16 music = stageScenes[port->node.scene].musicParam2;

	s16 result = stage == activeStageNo && mode == snSceneInfo->musicParam && music == snSceneInfo->musicParam2;

	if(Na_GetPlayingBgmFlag() != music)
	{
		result = FALSE;
	}
	return result;

#endif
}

/**
 * Set the current warp type and destination level/area/node.
 */
static void SetPlayerEntrant(s16 destLevel, s16 destArea, s16 destWarpNode, s32 arg3)
{
	if(destWarpNode >= WARP_NODE_CREDITS_MIN)
	{
		playerEntry.type = CHANGE_STAGE;
	}
	else if(destLevel != activeStageNo)
	{
		playerEntry.type = CHANGE_STAGE;
	}
	else if(destArea != snSceneInfo->index)
	{
		playerEntry.type = CHANGE_SCENE;
	}
	else
	{
		playerEntry.type = CHANGE_PORT;
	}

	playerEntry.levelNum = destLevel;
	playerEntry.areaIdx  = destArea;
	playerEntry.nodeId   = destWarpNode;
	playerEntry.arg	     = arg3;
}

void initiateWarp(u16 destLevel, s16 destArea, s16 destWarpNode, s32 arg3)
{
}

// From Surface 0xD3 to 0xFC
#define PAINTING_WARP_INDEX_START 0x00 // Value greater than or equal to Surface 0xD3
#define PAINTING_WARP_INDEX_FA 0x2A    // THI Huge Painting index left
#define PAINTING_WARP_INDEX_END 0x2D   // Value less than Surface 0xFD

/**
 * Check if mario is above and close to a painting warp plane, and return the
 * corresponding warp node.
 */
static WarpNode* GetBGPortPointer(void)
{
	WarpNode* portpt = NULL;
	s32 portno	 = marioWorks->floor->type - SURFACE_PAINTING_WARP_D3;

	if(portno >= PAINTING_WARP_INDEX_START && portno < PAINTING_WARP_INDEX_END)
	{
		if(portno < PAINTING_WARP_INDEX_FA || marioWorks->pos[1] - marioWorks->floorHeight < 80.0f)
		{
			portpt = &snSceneInfo->paintingWarpNodes[portno];
		}
	}

	return portpt;
}

/**
 * Check is mario has entered a painting, and if so, initiate a warp.
 */
static void CheckBGPort(void)
{
	if(snSceneInfo->paintingWarpNodes != NULL && marioWorks->floor != NULL)
	{
		WarpNode bgport;
		WarpNode* pWarpNode = GetBGPortPointer();

		if(pWarpNode != NULL)
		{
			if(marioWorks->status & ACT_FLAG_INTANGIBLE)
			{
				AudPictWaveSound();
			}
			else if(pWarpNode->id != 0)
			{
				bgport = *pWarpNode;

				if(!(bgport.destLevel & 0x80))
				{
					middlePointSw = BuGetMidPoint(&bgport);
				}

				SetPlayerEntrant(bgport.destLevel & 0x7F, bgport.destArea, bgport.destNode, 0);
				BuSetMidPoint(&bgport);

				SnDelayFader(WARP_TRANSITION_FADE_INTO_COLOR, 30, 255, 255, 255, 45);
				FreezeGame(74, ExecuteGame);

				marioWorks->ChangePlayerStatus(ACT_DISAPPEARED, 0);

				marioWorks->marioObj->header.gfx.node.flags &= ~0x0001;

				AudStartSound(SOUND_MENU_STAR_SOUND, gDefaultSoundArgs);
				AudStopMusic(398);
				SendMotorEvent(80, 70);
				SendMotorDecay(1);
			}
		}
	}
}

/**
 * If there is not already a delayed warp, schedule one. The source node is
 * based on the warp operation and sometimes mario's used object.
 * Return the time left until the delayed warp is initiated.
 */
s16 PL_StartFadeout(struct PlayerRecord* m, s32 warpOp)
{
	s32 audioFadeout = TRUE;

	if(fadeoutMode == WARP_OP_NONE)
	{
		m->invincTimer = -1;
		fadeoutInfo    = 0;
		fadeoutMode    = warpOp;

		switch(warpOp)
		{
			case PL_FADE_GOTITLE:
			case PL_FADE_RESTART:
				fadeoutCount  = 20 * FRAME_RATE_SCALER_INV;
				fadeoutPort   = WARP_NODE_F0;
				ramSaveCourse = 0;
				audioFadeout  = FALSE;
				SnStartFader(WARP_TRANSITION_FADE_INTO_STAR, 0x14, 0x00, 0x00, 0x00);
				break;

			case PL_FADE_GAMEEND:
				fadeoutCount  = 60 * FRAME_RATE_SCALER_INV;
				fadeoutPort   = WARP_NODE_F0;
				audioFadeout  = FALSE;
				ramSaveCourse = 0;
				SnStartFader(WARP_TRANSITION_FADE_INTO_COLOR, 0x3C, 0x00, 0x00, 0x00);
				break;

			case PL_FADE_WINNING:
				fadeoutCount  = 32 * FRAME_RATE_SCALER_INV;
				fadeoutPort   = WARP_NODE_F0;
				ramSaveCourse = 0;
				SnStartFader(WARP_TRANSITION_FADE_INTO_MARIO, 0x20, 0x00, 0x00, 0x00);
				break;

			case PL_FADE_LOSING:
				if(m->numLives == 0)
				{
					fadeoutMode = PL_FADE_GAMEOVR;
				}
				fadeoutCount = 48 * FRAME_RATE_SCALER_INV;
				fadeoutPort  = WARP_NODE_DEATH;
				SnStartFader(WARP_TRANSITION_FADE_INTO_BOWSER, 0x30, 0x00, 0x00, 0x00);
				AudStartSound(SOUND_MENU_BOWSER_LAUGH, gDefaultSoundArgs);
				break;

			case PL_FADE_FALLING:
				fadeoutPort = WARP_NODE_WARP_FLOOR;
				if(SnGetPortPtr(fadeoutPort) == NULL)
				{
					if(m->numLives == 0)
					{
						fadeoutMode = PL_FADE_GAMEOVR;
					}
					else
					{
						fadeoutPort = WARP_NODE_DEATH;
					}
				}
				fadeoutCount = 20 * FRAME_RATE_SCALER_INV;
				SnStartFader(WARP_TRANSITION_FADE_INTO_CIRCLE, 0x14, 0x00, 0x00, 0x00);
				break;

			case PL_FADE_VIEWROOF: // enter totwc
				fadeoutCount = 30 * FRAME_RATE_SCALER_INV;
				fadeoutPort  = WARP_NODE_F2;
				SnStartFader(WARP_TRANSITION_FADE_INTO_COLOR, 0x1E, 0xFF, 0xFF, 0xFF);
				AudStartSound(SOUND_MENU_STAR_SOUND, gDefaultSoundArgs);
				break;

			case PL_FADE_TRIPKAGO: // bbh enter
				fadeoutCount = 30 * FRAME_RATE_SCALER_INV;
				fadeoutPort  = (m->usedObj->oBehParams & 0x00FF0000) >> 16;
				SnStartFader(WARP_TRANSITION_FADE_INTO_COLOR, 0x1E, 0xFF, 0xFF, 0xFF);
				break;

			case PL_FADE_WARPIN:
				fadeoutCount = 20 * FRAME_RATE_SCALER_INV;
				fadeoutPort  = (m->usedObj->oBehParams & 0x00FF0000) >> 16;
				audioFadeout = !CheckSameMusic(fadeoutPort);
				SnStartFader(WARP_TRANSITION_FADE_INTO_COLOR, 0x14, 0xFF, 0xFF, 0xFF);
				break;

			case PL_FADE_TRIPDOOR:
				fadeoutCount = 20 * FRAME_RATE_SCALER_INV;
				fadeoutInfo  = m->actionArg;
				fadeoutPort  = (m->usedObj->oBehParams & 0x00FF0000) >> 16;
				audioFadeout = !CheckSameMusic(fadeoutPort);
				SnStartFader(WARP_TRANSITION_FADE_INTO_CIRCLE, 0x14, 0x00, 0x00, 0x00);
				break;

			case PL_FADE_TRIPTUBE:
				fadeoutCount = 20 * FRAME_RATE_SCALER_INV;
				fadeoutPort  = (m->usedObj->oBehParams & 0x00FF0000) >> 16;
				audioFadeout = !CheckSameMusic(fadeoutPort);
				SnStartFader(WARP_TRANSITION_FADE_INTO_STAR, 0x14, 0x00, 0x00, 0x00);
				break;

			case PL_FADE_ENDING:
				fadeoutCount = 30 * FRAME_RATE_SCALER_INV;
				SnStartFader(WARP_TRANSITION_FADE_INTO_COLOR, 0x1E, 0x00, 0x00, 0x00);
				break;

			case PL_FADE_STAFFROLL:
				if(snEndingScene == &sCreditsSequence[0])
				{
					fadeoutCount = 60 * FRAME_RATE_SCALER_INV;
					SnStartFader(WARP_TRANSITION_FADE_INTO_COLOR, 0x3C, 0x00, 0x00, 0x00);
				}
				else
				{
					fadeoutCount = 20 * FRAME_RATE_SCALER_INV;
					SnStartFader(WARP_TRANSITION_FADE_INTO_COLOR, 0x14, 0x00, 0x00, 0x00);
				}
				audioFadeout = FALSE;
				break;
		}

		if(audioFadeout && autoDemoPtr == NULL)
		{
			AudStopMusic((3 * fadeoutCount / 2) * 8 - 2);
		}
	}

	return fadeoutCount;
}

/**
 * If a delayed warp is ready, initiate it.
 */
static void CheckGameFadeout(void)
{
	ObjectWarpNode* port;
	s32 destWarpNode;

	if(fadeoutMode != WARP_OP_NONE && --fadeoutCount == 0)
	{
		MessageEnforcedDestroy();

		if(autoDemoPtr != NULL)
		{
			if(fadeoutMode == PL_FADE_RESTART)
			{
				ChangeExitMode(PL_RETURN_LOGO);
			}
			else
			{
				ChangeExitMode(PL_RETURN_TITLE);
			}
		}
		else
		{
			switch(fadeoutMode)
			{
				case PL_FADE_GAMEOVR:
					BuRestoreBackUp();
					ChangeExitMode(PL_GAME_OVER);
					break;

				case PL_FADE_GAMEEND:
					ChangeExitMode(PL_GAME_CLEAR);
					Na_PortUnlock(2, 0x03F0);
					break;

				case PL_FADE_GOTITLE:
					ChangeExitMode(PL_RETURN_TITLE);
					break;

				case PL_FADE_ENDING:
					snEndingScene = &sCreditsSequence[0];
					SetPlayerEntrant(snEndingScene->stage, snEndingScene->scene, WARP_NODE_CREDITS_START, 0);
					break;

				case PL_FADE_STAFFROLL:
					Na_PortLock(2, 0x03FF);

					snEndingScene += 1;
					activeLevelNo = snEndingScene->info & 0x07;
					if((snEndingScene + 1)->stage == 0)
					{
						destWarpNode = WARP_NODE_CREDITS_END;
					}
					else
					{
						destWarpNode = WARP_NODE_CREDITS_NEXT;
					}

					SetPlayerEntrant(snEndingScene->stage, snEndingScene->scene, destWarpNode, 0);
					break;

				default:
					port = SnGetPortPtr(fadeoutPort);

					SetPlayerEntrant(port->node.destLevel & 0x7F, port->node.destArea, port->node.destNode, fadeoutInfo);

					BuSetMidPoint(&port->node);
					if(playerEntry.type != CHANGE_STAGE)
					{
						FreezeGame(2, NULL);
					}
					break;
			}
		}
	}
}

static void SetPlayerMeter(void)
{
	if(snEndingScene == NULL)
	{
		s16 power = marioWorks->health > 0 ? marioWorks->health >> 8 : 0;

		if(activeCourseNo > 0)
		{
			playerMeter.flags |= HUD_DISPLAY_FLAG_COIN_COUNT;
		}
		else
		{
			playerMeter.flags &= ~HUD_DISPLAY_FLAG_COIN_COUNT;
		}

		if(playerMeter.coins < marioWorks->numCoins)
		{
			if(frameCounter & FRAME_RATE_SCALER_INV)
			{
				u32 coinSound;
				if(marioWorks->status & (ACT_FLAG_SWIMMING | ACT_FLAG_METAL_WATER))
				{
					coinSound = SOUND_GENERAL_COIN_WATER;
				}
				else
				{
					coinSound = SOUND_GENERAL_COIN;
				}

				playerMeter.coins += 1;
				AudStartSound(coinSound, marioWorks->marioObj->header.gfx.cameraToObject);
			}
		}

		if(marioWorks->numLives > 100)
		{
			marioWorks->numLives = 100;
		}

#if BUGFIX_MAX_LIVES
		if(marioWorks->numCoins > 999)
		{
			marioWorks->numCoins = 999;
		}

		if(playerMeter.coins > 999)
		{
			playerMeter.coins = 999;
		}
#else
		if(marioWorks->numCoins > 999)
		{
			marioWorks->numLives = (s8)999; //! Wrong variable
		}
#endif

		playerMeter.stars = marioWorks->numStars;
		playerMeter.lives = sm64::config().cheats().unlimitedLives() ? 99 : marioWorks->numLives;
		playerMeter.keys  = marioWorks->numKeys;

		if(power > playerMeter.wedges)
		{
			AudStartSound(SOUND_MENU_POWER_METER, gDefaultSoundArgs);
		}
		playerMeter.wedges = power;

		if(marioWorks->hurtCounter > 0)
		{
			playerMeter.flags |= HUD_DISPLAY_FLAG_EMPHASIZE_POWER;
		}
		else
		{
			playerMeter.flags &= ~HUD_DISPLAY_FLAG_EMPHASIZE_POWER;
		}
	}
}

/**
 * Update objects, hud, and camera. This update function excludes things like
 * endless staircase, warps, pausing, etc. This is used when entering a painting,
 * presumably to allow painting and camera updating while avoiding triggering the
 * warp twice.
 */
static void ExecuteGame(s16* arg)
{
	SnExecuteStrategy();
	SetPlayerMeter();

	if(snSceneInfo != NULL)
	{
		CtrlGameCamera(snSceneInfo->camera);
	}
}

extern void SnDisplayDemoMessage(void);
int gPressedStart = 0;

static s32 NormalProcess(void)
{
	if(autoDemoPtr != NULL)
	{
		SnDisplayDemoMessage();
		if(sm64::player(0).controller().buttonPressed & CONT_EXIT)
		{
			PL_StartFadeout(marioWorks, activeStageNo == LEVEL_PSS ? PL_FADE_RESTART : PL_FADE_GOTITLE);
		}
		else if(!wipeControl.isActive && fadeoutMode == WARP_OP_NONE && (sm64::player(0).controller().buttonPressed & sm64::hid::START_BUTTON))
		{
			gPressedStart = 1;
			PL_StartFadeout(marioWorks, PL_FADE_GOTITLE);
		}
	}

	ChangeScene();
	CheckSceneConnection();

	if(stopWatchSw && playerMeter.timer < 17999 * FRAME_RATE_SCALER_INV && (frameCounter % FRAME_RATE_SCALER_INV) == 0)
	{
		playerMeter.timer += 1;
	}

	SnExecuteStrategy();
	SetPlayerMeter();

	if(snSceneInfo != NULL)
	{
		CtrlGameCamera(snSceneInfo->camera);
	}

	CheckBGPort();
	CheckGameFadeout();

	// If either CheckBGPort or CheckGameFadeout initiated a
	// warp, change play mode accordingly.
	if(gameProcess == GM_NORMAL)
	{
		if(playerEntry.type == CHANGE_STAGE)
		{
			ChangeProcess(GM_EXIT);
		}
		else if(freezeCounter != 0)
		{
			ChangeProcess(GM_FREEZE);
		}
		else if(CheckPauseRequest())
		{
			AudStartMute(AUD_MUTE_PAUSE);
			ResetMotorPack();
			gameCameraControl |= CAM_MOVE_PAUSE_SCREEN;
			ChangeProcess(GM_PAUSE);
		}
	}

	return 0;
}

static s32 PauseProcess(void)
{
	if(mesgEvent == 0)
	{
		CallSelectMessage(RENDER_PAUSE_SCREEN);
	}
	else if(mesgEvent == 1)
	{
		AudEndMute(1);
		gameCameraControl &= ~CAM_MOVE_PAUSE_SCREEN;
		ChangeProcess(GM_NORMAL);
	}
	else
	{
		// Exit level

		SetPlayerEntrant(LEVEL_CASTLE, 1, 0x1F, 0);
		PL_ExitGameMode(0, 0);
		ramSaveCourse = 0;

		gameCameraControl &= ~CAM_MOVE_PAUSE_SCREEN;
	}

	return 0;
}

/**
 * Debug mode that lets you frame advance by pressing D-pad down. Unfortunately
 * it uses the pause camera, making it basically unusable in most levels.
 */
static s32 StillProcess(void)
{
	if(sm64::player(0).controller().buttonPressed & sm64::hid::D_JPAD)
	{
		gameCameraControl &= ~CAM_MOVE_PAUSE_SCREEN;
		NormalProcess();
	}
	else if(sm64::player(0).controller().buttonPressed & sm64::hid::START_BUTTON)
	{
		gameCameraControl &= ~CAM_MOVE_PAUSE_SCREEN;
		AudEndMute(AUD_MUTE_PAUSE);
		ChangeProcess(GM_NORMAL);
	}
	else
	{
		gameCameraControl |= CAM_MOVE_PAUSE_SCREEN;
	}

	return 0;
}

/**
 * Set the transition, which is a period of time after the warp is initiated
 * but before it actually occurs. If updateFunction is not NULL, it will be
 * called each frame during the transition.
 */
void FreezeGame(s16 length, FreezeProc updateFunction)
{
	freezeCounter	= length;
	freezeProcedure = updateFunction;
}

/**
 * Play the transition and then return to normal play mode.
 */
static s32 FreezeProcess()
{
	//! This maybe was supposed to be freezeCounter == -1? freezeProcedure
	// is never set to -1.
	if(freezeProcedure == (void (*)(s16*)) - 1)
	{
		CtrlGameCamera(snSceneInfo->camera);
	}
	else if(freezeProcedure != NULL)
	{
		freezeProcedure(&freezeCounter);
	}

	if(freezeCounter > 0)
	{
		freezeCounter -= 1;
	}

	//! If freezeCounter is -1, this will miss.
	if(freezeCounter == 0)
	{
		freezeProcedure = NULL;
		ChangeProcess(GM_NORMAL);
	}

	return 0;
}

/**
 * Play the transition and then return to normal play mode.
 */
static s32 ExitProcess(void)
{
	if(freezeProcedure != NULL)
	{
		freezeProcedure(&freezeCounter);
	}

	//! If freezeCounter is -1, this will miss.
	if(--freezeCounter == -1)
	{
		playerMeter.flags = HUD_DISPLAY_NONE;
		freezeCounter	  = 0;
		freezeProcedure	  = NULL;

		if(playerEntry.type != WARP_TYPE_NOT_WARPING)
		{
			return playerEntry.levelNum;
		}
		else
		{
			return gameExitCode;
		}
	}

	return 0;
}

/**
 * Unused play mode. Doesn't call transition update and doesn't reset transition
 * at the end.
 */
static s32 FreezePlayerProcess(void)
{
	if(--freezeCounter == -1)
	{
		playerMeter.flags = HUD_DISPLAY_NONE;

		if(playerEntry.type != WARP_TYPE_NOT_WARPING)
		{
			return playerEntry.levelNum;
		}
		else
		{
			return gameExitCode;
		}
	}

	return 0;
}

static s32 PlayGame()
{
	s32 changeLevel = 0;

	switch(gameProcess)
	{
		case GM_NORMAL:
			changeLevel = NormalProcess();
			break;
		case GM_PAUSE:
			changeLevel = PauseProcess();
			break;
		case GM_FREEZE:
			changeLevel = FreezeProcess();
			break;
		case GM_EXIT:
			changeLevel = ExitProcess();
			break;
		case GM_STILL:
			changeLevel = StillProcess();
			break;
	}

	if(changeLevel)
	{
		AudResetMute();
		AudUnlockSound();
	}

	return changeLevel;
}

static s32 InitGame(void)
{
	s32 whitein = 0;

	ChangeProcess(GM_NORMAL);

	fadeoutMode	  = WARP_OP_NONE;
	freezeCounter	  = 0;
	gameExitCode	  = 0;
	playerMeter.flags = (snEndingScene == NULL) ? HUD_DISPLAY_DEFAULT : HUD_DISPLAY_NONE;

	stopWatchSw = 0;

	if(playerEntry.type != WARP_TYPE_NOT_WARPING)
	{
		if(playerEntry.nodeId >= WARP_NODE_CREDITS_MIN)
		{
			ChangeEndingStage();
		}
		else
		{
			ChangeStage();
		}
	}
	else
	{
		if(starringActor[0].areaIndex >= 0)
		{
			SnEnterPlayer();
			marioWorks->initPlayer();
		}

		if(snSceneInfo != NULL)
		{
			InitGameCamera(snSceneInfo->camera);

			if(autoDemoPtr != NULL)
			{
				marioWorks->ChangePlayerStatus(ACT_IDLE, 0);
			}
			else
			{
				if(marioWorks->status != ACT_UNINITIALIZED)
				{
					if(BuIsActive(activePlayerNo - 1))
					{
						marioWorks->ChangePlayerStatus(ACT_IDLE, 0);
					}
					else
					{
						marioWorks->ChangePlayerStatus(ACT_INTRO_CUTSCENE, 0);
						whitein = 1;
					}
				}
			}
		}

		if(whitein != 0)
		{
			SnStartFader(WARP_TRANSITION_FADE_FROM_COLOR, 0x5A, 0xFF, 0xFF, 0xFF);
		}
		else
		{
			SnStartFader(WARP_TRANSITION_FADE_FROM_STAR, 0x10, 0xFF, 0xFF, 0xFF);
		}

		if(autoDemoPtr == NULL)
		{
			AudPlayMusic(snSceneInfo->musicParam, snSceneInfo->musicParam2, 0);
		}
	}

	if(autoDemoPtr == NULL)
	{
		ResetMotorPack();
	}

	if(marioWorks->status == ACT_INTRO_CUTSCENE)
	{
		Na_PortLock(2, 0x0330);
	}

	return 1;
}

/**
 * Initialize the current level if initOrUpdate is 0, or update the level if it
 * is 1.
 */
s32 GameProcess(s16 initOrUpdate, s32 unused)
{
	s32 result = 0;

	switch(initOrUpdate)
	{
		case 0:
			result = InitGame();
			break;
		case 1:
			result = PlayGame();
			break;
	}

	return result;
}

s32 GameInitialize(s16 code, s32 levelNum)
{
	playerEntry.type = WARP_TYPE_NOT_WARPING;
	fadeoutMode	 = WARP_OP_NONE;
	mesgCastle	 = !BuIsActive(activePlayerNo - 1);

	activeStageNo	   = levelNum;
	activeCourseNo	   = COURSE_NONE;
	ramSaveCourse	   = 0;
	snEndingScene	   = NULL;
	buYosshiJump = 0;

	marioWorks->initPlayerWorks();
	disable_warp_checkpoint();
	BuRestoreHat();
	select_mario_cam_mode();
	iwa_StratInit();

	return levelNum;
}

s32 CheckCourseMenu(s16 code, s32 levelNum)
{
	s32 val4 = middlePointSw;

	middlePointSw  = 0;
	activeStageNo  = levelNum;
	activeCourseNo = gLevelToCourseNumTable[levelNum - 1];

	if(autoDemoPtr != NULL || snEndingScene != NULL || activeCourseNo == COURSE_NONE)
	{
		return 0;
	}

	if(activeStageNo != LEVEL_BOWSER_1 && activeStageNo != LEVEL_BOWSER_2 && activeStageNo != LEVEL_BOWSER_3)
	{
		marioWorks->numCoins = 0;
		playerMeter.coins    = 0;
		gCurrCourseStarFlags = BuGetStarFlag(activePlayerNo - 1, activeCourseNo - 1);
	}

	if(ramSaveCourse != activeCourseNo)
	{
		ramSaveCourse = activeCourseNo;
		nop_change_course();
		disable_warp_checkpoint();
	}

	if(activeCourseNo > COURSE_STAGES_MAX || val4 != 0)
	{
		return 0;
	}

	return 1;
}

/**
 * Play the "thank you so much for to playing my game" sound.
 */
s32 GameTheEnd(s16 code, s32 param)
{
	AudStartSound(SOUND_MENU_THANK_YOU_PLAYING_MY_GAME, gDefaultSoundArgs);
	return 1;
}
