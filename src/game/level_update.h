#ifndef _LEVEL_UPDATE_H
#define _LEVEL_UPDATE_H

#include "types.h"

#define TIMER_CONTROL_SHOW 0
#define TIMER_CONTROL_START 1
#define TIMER_CONTROL_STOP 2
#define TIMER_CONTROL_HIDE 3

#define WARP_OP_NONE 0x00
#define PL_FADE_VIEWROOF 0x01
#define PL_FADE_TRIPKAGO 0x02
#define PL_FADE_TRIPDOOR 0x03
#define PL_FADE_TRIPTUBE 0x04
#define PL_FADE_WARPIN 0x05
#define PL_FADE_WINNING 0x11
#define PL_FADE_LOSING 0x12
#define PL_FADE_FALLING 0x13
#define PL_FADE_GAMEOVR 0x14
#define PL_FADE_GAMEEND 0x15
#define PL_FADE_GOTITLE 0x16
#define PL_FADE_ENDING 0x17
#define PL_FADE_STAFFROLL 0x18
#define PL_FADE_RESTART 0x19

#define WARP_OP_TRIGGERS_LEVEL_SELECT 0x10

#define PL_RETURN_SELECT -9
#define PL_RETURN_LOGO -8
#define PL_RETURN_TITLE -2
#define PL_GAME_OVER -3
#define PL_GAME_CLEAR -1

typedef void (*FreezeProc)(short* counter);

#include "game/scene.h"

extern struct CreditsEntry* snEndingScene;

extern PlayerRecord playerWorks[];
extern PlayerRecord* marioWorks;

extern s16 gameProcess;
extern u16 gameCounter;
extern s16 freezeCounter;
extern void (*freezeProcedure)(s16*);
extern u8 unused3[4];

struct WarpDest
{
	u8 type;
	u8 levelNum;
	u8 areaIdx;
	u8 nodeId;
	u32 arg;
};

extern struct WarpDest playerEntry;

extern s16 gameExitCode;
extern s16 fadeoutMode;
extern s16 fadeoutCount;
extern s16 fadeoutPort;
extern s32 fadeoutInfo;
extern u8 unused4[2];
extern s8 stopWatchSw;

struct HudDisplay
{
	/*0x00*/ s16 lives;
	/*0x02*/ s16 coins;
	/*0x04*/ s16 stars;
	/*0x06*/ s16 wedges;
	/*0x08*/ s16 keys;
	/*0x0A*/ s16 flags;
	/*0x0C*/ u16 timer;
};

extern struct HudDisplay playerMeter;
extern s8 mesgCastle;

enum HUDDisplayFlag
{
	HUD_DISPLAY_FLAG_LIVES		  = 0x0001,
	HUD_DISPLAY_FLAG_COIN_COUNT	  = 0x0002,
	HUD_DISPLAY_FLAG_STAR_COUNT	  = 0x0004,
	HUD_DISPLAY_FLAG_CAMERA_AND_POWER = 0x0008,
	HUD_DISPLAY_FLAG_KEYS		  = 0x0010,
	HUD_DISPLAY_FLAG_UNKNOWN_0020	  = 0x0020,
	HUD_DISPLAY_FLAG_TIMER		  = 0x0040,
	HUD_DISPLAY_FLAG_EMPHASIZE_POWER  = 0x8000,

	HUD_DISPLAY_NONE    = 0x0000,
	HUD_DISPLAY_DEFAULT = HUD_DISPLAY_FLAG_LIVES | HUD_DISPLAY_FLAG_COIN_COUNT | HUD_DISPLAY_FLAG_STAR_COUNT | HUD_DISPLAY_FLAG_CAMERA_AND_POWER | HUD_DISPLAY_FLAG_KEYS | HUD_DISPLAY_FLAG_UNKNOWN_0020
};

u16 GmStopWatch(s32 timerOp);
void PL_ExitGameMode(u32 arg, u32 color);
void PL_DisplayMessage(u32 arg);
s16 PL_StartFadeout(struct PlayerRecord* m, s32 warpOp);
void FreezeGame(s16 length, FreezeProc proc);

s32 GameProcess(s16 initOrUpdate, s32);
s32 GameInitialize(s16, s32 levelNum);
s32 CheckCourseMenu(s16, s32 levelNum);
s32 GameTheEnd(s16, s32);

#endif
