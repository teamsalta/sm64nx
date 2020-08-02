#include <ultra64.h>

#include "sm64.h"
#include "memory.h"
#include "load.h"
#include "data.h"
#include "seqplayer.h"
#include "external.h"
#include "playback.h"
#include "synthesis.h"
#include "game/mario.h"
#include "game/level_update.h"
#include "game/area.h"
#include "game/room.h"
#include "game/camera.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "level_table.h"

// N.B. sound banks are different from the audio banks referred to in other
// files. We should really fix our naming to be less ambiguous...
#define MAX_BG_MUSIC_QUEUE_SIZE 6
#define SOUND_BANK_COUNT 10
#define MAX_CHANNELS_PER_SOUND 1

#define SEQUENCE_NONE 0xFF

#define SAMPLES_TO_OVERPRODUCE 0x10
#define EXTRA_BUFFERED_AI_SAMPLES_TARGET 0x40

// No-op printf macro which leaves string literals in rodata in IDO. (IDO
// doesn't support variadic macros, so instead they let the parameter list
// expand to a no-op comma expression.) See also goddard/gd_main.h.
#ifdef __sgi
#define stubbed_printf
#else
#define stubbed_printf(...)
#endif

struct Sound
{
	s32 soundBits;
	f32* position;
}; // size = 0x8

struct ChannelVolumeScaleFade
{
	f32 velocity;
	u8 target;
	f32 current;
	u16 remDuration;
}; // size = 0x10

struct SoundCharacteristics
{
	f32* x;
	f32* y;
	f32* z;
	f32 distance;
	u32 priority;
	u32 soundBits; // packed bits, same as first arg to AudStartSound
	u8 soundStatus;
	u8 unk19; // ttl? sometimes set to 10
	u8 prev;
	u8 next;
}; // size = 0x1C

struct SequenceQueueItem
{
	u8 seqId;
	u8 priority;
}; // size = 0x2

// data
s32 gAudioErrorFlags = 0;
s32 sGameLoopTicked  = 0;

// Dialog sounds
// The US difference is the sound for DIALOG_037 ("I win! You lose! Ha ha ha ha!
// You're no slouch, but I'm a better sledder! Better luck next time!"), spoken
// by Koopa instead of the penguin in JP.

#define UKIKI 0
#define TUXIE 1
#define BOWS1 2 // Bowser Intro / Doors Laugh
#define KOOPA 3
#define KBOMB 4
#define BOO 5
#define BOMB 6
#define BOWS2 7 // Bowser Battle Laugh
#define GRUNT 8
#define WIGLR 9
#define YOSHI 10
#define _ 0xFF

#define DIFF TUXIE

u8 sDialogSpeaker[] = {
    //       0      1      2      3      4      5      6      7      8      9
    /* 0*/ _,	  BOMB,	 BOMB,	BOMB,  BOMB,  KOOPA, KOOPA, KOOPA, _,	  KOOPA,
    /* 1*/ _,	  _,	 _,	_,     _,     _,     _,	    KBOMB, _,	  _,
    /* 2*/ _,	  BOWS1, BOWS1, BOWS1, BOWS1, BOWS1, BOWS1, BOWS1, BOWS1, BOWS1,
    /* 3*/ _,	  _,	 _,	_,     _,     _,     _,	    DIFF,  _,	  _,
    /* 4*/ _,	  KOOPA, _,	_,     _,     _,     _,	    BOMB,  _,	  _,
    /* 5*/ _,	  _,	 _,	_,     _,     TUXIE, TUXIE, TUXIE, TUXIE, TUXIE,
    /* 6*/ _,	  _,	 _,	_,     _,     _,     _,	    BOWS2, _,	  _,
    /* 7*/ _,	  _,	 _,	_,     _,     _,     _,	    _,	   _,	  UKIKI,
    /* 8*/ UKIKI, _,	 _,	_,     _,     BOO,   _,	    _,	   _,	  _,
    /* 9*/ BOWS2, _,	 BOWS2, BOWS2, _,     _,     _,	    _,	   BOO,	  BOO,
    /*10*/ UKIKI, UKIKI, _,	_,     _,     BOMB,  BOMB,  BOO,   BOO,	  _,
    /*11*/ _,	  _,	 _,	_,     GRUNT, GRUNT, KBOMB, GRUNT, GRUNT, _,
    /*12*/ _,	  _,	 _,	_,     _,     _,     _,	    _,	   KBOMB, _,
    /*13*/ _,	  _,	 TUXIE, _,     _,     _,     _,	    _,	   _,	  _,
    /*14*/ _,	  _,	 _,	_,     _,     _,     _,	    _,	   _,	  _,
    /*15*/ WIGLR, WIGLR, WIGLR, _,     _,     _,     _,	    _,	   _,	  _,
    /*16*/ _,	  YOSHI, _,	_,     _,     _,     _,	    _,	   WIGLR, _};
#undef _
static_assert(ARRAY_COUNT(sDialogSpeaker) == DIALOG_COUNT, "change this array if you are adding dialogs");

s32 sDialogSpeakerVoice[15] = {
    SOUND_OBJ_UKIKI_CHATTER_LONG,
    SOUND_OBJ_BIG_PENGUIN_YELL,
    SOUND_OBJ_BOWSER_INTRO_LAUGH,
    SOUND_OBJ_KOOPA_TALK,
    SOUND_OBJ_KING_BOBOMB_TALK,
    SOUND_OBJ_BOO_LAUGH_LONG,
    SOUND_OBJ_BOBOMB_BUDDY_TALK,
    SOUND_OBJ_BOWSER_LAUGH,
    SOUND_OBJ2_BOSS_DIALOG_GRUNT,
    SOUND_OBJ_WIGGLER_TALK,
    SOUND_GENERAL_YOSHI_TALK,
    NO_SOUND,
    NO_SOUND,
    NO_SOUND,
    NO_SOUND,
};

u8 sNumProcessedSoundRequests = 0;
u8 sSoundRequestCount	      = 0;

// Music dynamic tables. A dynamic describes which volumes to apply to which
// channels of a sequence (I think?), and different parts of a level can have
// different dynamics. Each table below specifies first the sequence to apply
// the dynamics to, then a bunch of conditions for when each dynamic applies
// (e.g. "only if Mario's X position is between 100 and 300"), and finally a
// fallback dynamic. Due to the encoding of the tables, the conditions must
// come in the same order as the macros.
// TODO: dynamic isn't a great term for this, it means other things in music...

#define MARIO_X_GE 0
#define MARIO_Y_GE 1
#define MARIO_Z_GE 2
#define MARIO_X_LT 3
#define MARIO_Y_LT 4
#define MARIO_Z_LT 5
#define MARIO_IS_IN_AREA 6
#define MARIO_IS_IN_ROOM 7

#define DYN1(cond1, val1, res) (s16)(1 << (15 - cond1) | res), val1
#define DYN2(cond1, val1, cond2, val2, res) (s16)(1 << (15 - cond1) | 1 << (15 - cond2) | res), val1, val2
#define DYN3(cond1, val1, cond2, val2, cond3, val3, res) (s16)(1 << (15 - cond1) | 1 << (15 - cond2) | 1 << (15 - cond3) | res), val1, val2, val3

s16 sDynBbh[] = {
    SEQ_LEVEL_SPOOKY,
    DYN1(MARIO_IS_IN_ROOM, BBH_OUTSIDE_ROOM, 6),
    DYN1(MARIO_IS_IN_ROOM, BBH_NEAR_MERRY_GO_ROUND_ROOM, 6),
    5,
};
s16 sDynDdd[] = {
    SEQ_LEVEL_WATER, DYN2(MARIO_X_LT, -800, MARIO_IS_IN_AREA, AREA_DDD_WHIRLPOOL & 0xf, 0), DYN3(MARIO_Y_GE, -2000, MARIO_X_LT, 470, MARIO_IS_IN_AREA, AREA_DDD_WHIRLPOOL & 0xf, 0), DYN2(MARIO_Y_GE, 100, MARIO_IS_IN_AREA, AREA_DDD_SUB & 0xf, 2), 1,
};
s16 sDynJrb[] = {
    SEQ_LEVEL_WATER,
    DYN2(MARIO_Y_GE, 945, MARIO_X_LT, -5260, 0),
    DYN1(MARIO_IS_IN_AREA, AREA_JRB_SHIP & 0xf, 0),
    DYN1(MARIO_Y_GE, 1000, 0),
    DYN2(MARIO_Y_GE, -3100, MARIO_Z_LT, -900, 2),
    1,
    5, // bogus entry, ignored (was JRB originally intended to have spooky music?)
};
s16 sDynWdw[] = {
    SEQ_LEVEL_UNDERGROUND,
    DYN2(MARIO_Y_LT, -670, MARIO_IS_IN_AREA, AREA_WDW_MAIN & 0xf, 4),
    DYN1(MARIO_IS_IN_AREA, AREA_WDW_TOWN & 0xf, 4),
    3,
};
s16 sDynHmc[] = {
    SEQ_LEVEL_UNDERGROUND,
    DYN2(MARIO_X_GE, 0, MARIO_Y_LT, -203, 4),
    DYN2(MARIO_X_LT, 0, MARIO_Y_LT, -2400, 4),
    3,
};
s16 sDynUnk38[] = {
    SEQ_LEVEL_UNDERGROUND, DYN1(MARIO_IS_IN_AREA, 1, 3), DYN1(MARIO_IS_IN_AREA, 2, 4), DYN1(MARIO_IS_IN_AREA, 3, 7), 0,
};
s16 sDynNone[] = {SEQ_SOUND_PLAYER, 0};

u8 sCurrentMusicDynamic	       = 0xff;
u8 sBackgroundMusicForDynamics = SEQUENCE_NONE;

#define STUB_LEVEL(_0, _1, _2, _3, _4, _5, _6, leveldyn, _8) leveldyn,
#define DEFINE_LEVEL(_0, _1, _2, _3, _4, _5, _6, _7, _8, leveldyn, _10) leveldyn,

#define _ sDynNone
s16* sLevelDynamics[LEVEL_COUNT] = {
    _, // LEVEL_NONE
#include "levels/level_defines.h"
};
#undef _
#undef STUB_LEVEL
#undef DEFINE_LEVEL

struct MusicDynamic
{
	/*0x0*/ s16 bits1;
	/*0x2*/ u16 volScale1;
	/*0x4*/ s16 dur1;
	/*0x6*/ s16 bits2;
	/*0x8*/ u16 volScale2;
	/*0xA*/ s16 dur2;
}; // size = 0xC

struct MusicDynamic sMusicDynamics[8] = {
    {0x0000, 127, 100, 0x0e43, 0, 100}, // SEQ_LEVEL_WATER
    {0x0003, 127, 100, 0x0e40, 0, 100}, // SEQ_LEVEL_WATER
    {0x0e43, 127, 200, 0x0000, 0, 200}, // SEQ_LEVEL_WATER
    {0x02ff, 127, 100, 0x0100, 0, 100}, // SEQ_LEVEL_UNDERGROUND
    {0x03f7, 127, 100, 0x0008, 0, 100}, // SEQ_LEVEL_UNDERGROUND
    {0x0070, 127, 10, 0x0000, 0, 100},	// SEQ_LEVEL_SPOOKY
    {0x0000, 127, 100, 0x0070, 0, 10},	// SEQ_LEVEL_SPOOKY
    {0xffff, 127, 100, 0x0000, 0, 100}, // any (size)
};

#define STUB_LEVEL(_0, _1, _2, _3, echo1, echo2, echo3, _7, _8) {echo1, echo2, echo3},
#define DEFINE_LEVEL(_0, _1, _2, _3, _4, _5, echo1, echo2, echo3, _9, _10) {echo1, echo2, echo3},

u8 gAreaEchoLevel[LEVEL_COUNT][3] = {
    {0x00, 0x00, 0x00}, // LEVEL_NONE
#include "levels/level_defines.h"
};
#undef STUB_LEVEL
#undef DEFINE_LEVEL

#define STUB_LEVEL(_0, _1, _2, volume, _4, _5, _6, _7, _8) volume,
#define DEFINE_LEVEL(_0, _1, _2, _3, _4, volume, _6, _7, _8, _9, _10) volume,

u16 D_80332028[LEVEL_COUNT] = {
    20000, // LEVEL_NONE
#include "levels/level_defines.h"
};

#undef STUB_LEVEL
#undef DEFINE_LEVEL

#define AUDIO_MAX_DISTANCE US_FLOAT(22000.0)

// Default volume for background music sequences (playing on player 0).
u8 sBackgroundMusicDefaultVolume[] = {
    127, // SEQ_SOUND_PLAYER
    80,	 // SEQ_EVENT_CUTSCENE_COLLECT_STAR
    80,	 // SEQ_MENU_TITLE_SCREEN
    75,	 // SEQ_LEVEL_GRASS
    70,	 // SEQ_LEVEL_INSIDE_CASTLE
    75,	 // SEQ_LEVEL_WATER
    75,	 // SEQ_LEVEL_HOT
    75,	 // SEQ_LEVEL_BOSS_KOOPA
    70,	 // SEQ_LEVEL_SNOW
    65,	 // SEQ_LEVEL_SLIDE
    80,	 // SEQ_LEVEL_SPOOKY
    65,	 // SEQ_EVENT_PIRANHA_PLANT
    85,	 // SEQ_LEVEL_UNDERGROUND
    75,	 // SEQ_MENU_STAR_SELECT
    65,	 // SEQ_EVENT_POWERUP
    70,	 // SEQ_EVENT_METAL_CAP
    65,	 // SEQ_EVENT_KOOPA_MESSAGE
    70,	 // SEQ_LEVEL_KOOPA_ROAD
    70,	 // SEQ_EVENT_HIGH_SCORE
    65,	 // SEQ_EVENT_MERRY_GO_ROUND
    80,	 // SEQ_EVENT_RACE
    70,	 // SEQ_EVENT_CUTSCENE_STAR_SPAWN
    85,	 // SEQ_EVENT_BOSS
    75,	 // SEQ_EVENT_CUTSCENE_COLLECT_KEY
    75,	 // SEQ_EVENT_ENDLESS_STAIRS
    85,	 // SEQ_LEVEL_BOSS_KOOPA_FINAL
    70,	 // SEQ_EVENT_CUTSCENE_CREDITS
    80,	 // SEQ_EVENT_SOLVE_PUZZLE
    80,	 // SEQ_EVENT_TOAD_MESSAGE
    70,	 // SEQ_EVENT_PEACH_MESSAGE
    75,	 // SEQ_EVENT_CUTSCENE_INTRO
    80,	 // SEQ_EVENT_CUTSCENE_VICTORY
    70,	 // SEQ_EVENT_CUTSCENE_ENDING
    65,	 // SEQ_MENU_FILE_SELECT
    0,	 // SEQ_EVENT_CUTSCENE_LAKITU (not in JP)
};

static_assert(ARRAY_COUNT(sBackgroundMusicDefaultVolume) == SEQ_COUNT, "change this array if you are adding sequences");

u8 sPlayer0CurSeqId			      = SEQUENCE_NONE;
u8 music_num_wait			      = 0;
u8 D_803320A4[SOUND_BANK_COUNT]		      = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // pointers to head of list
u8 D_803320B0[SOUND_BANK_COUNT]		      = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // pointers to head of list
u8 D_803320BC[SOUND_BANK_COUNT]		      = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // only used for debugging
u8 sMaxChannelsForSoundBank[SOUND_BANK_COUNT] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

// Banks 2 and 7 both grew from 0x30 sounds to 0x40 in size in US.
#define BANK27_SIZE 0x40
u8 sNumSoundsPerBank[SOUND_BANK_COUNT] = {
    0x70, 0x30, BANK27_SIZE, 0x80, 0x20, 0x80, 0x20, BANK27_SIZE, 0x80, 0x80,
};
#undef BANK27_SIZE

f32 gDefaultSoundArgs[3]     = {0.0f, 0.0f, 0.0f};
f32 sUnusedSoundArgs[3]	     = {1.0f, 1.0f, 1.0f};
u8 lockstat[16]    = {0};
u8 D_80332108		     = 0;
u8 SoundFadeFlag	     = FALSE;
u16 D_80332110		     = 0;
u8 sUnused80332114	     = 0; // never read, set to 0
u16 sUnused80332118	     = 0; // never read, set to 0
u8 Seq1PlayingMuteCount		     = 0;
u8 Sub1MusicNum		     = 0;
u8 Sub1MusicVol		     = 0;
u8 entrybgms = 0;
u8 sUnused8033323C	     = 0; // never read, set to 0

// bss
u16* gCurrAiBuffer;
struct Sound sSoundRequests[0x100];
struct ChannelVolumeScaleFade D_80360928[SEQUENCE_PLAYERS][CHANNELS_MAX];
u8 sUsedChannelsForSoundBank[SOUND_BANK_COUNT];
u8 sCurrentSound[SOUND_BANK_COUNT][MAX_CHANNELS_PER_SOUND]; // index into gSoundBanks
// list item memory for D_803320A4 and D_803320B0
struct SoundCharacteristics gSoundBanks[SOUND_BANK_COUNT][40];
u8 ObjSpeed[SOUND_BANK_COUNT];
u8 Seq1AddFlag;
static u8 sCapVolumeTo40;
struct SequenceQueueItem sBackgroundMusicQueue[MAX_BG_MUSIC_QUEUE_SIZE];

typedef s32 FadeT;

static void Na_SeBgmMuteOff(u8 bankIndex, u8 item);

// Local functions that could be static but are defined in/called from GLOBAL_ASM blocks,
// or not part of the large block of static functions.
void Naa_AllSeCheck(void);
void Na_SeqStart(u8 player, u8 seqId, u16 fadeTimer);
void Na_SeqSubVolumeTrans(u8 player, u8 channelId, u8 targetScale, u16 fadeTimer);
void Na_SeqSubVolumeMove(u8 player);
void Na_AllMusicCheck(void);
u8 Na_Seq0MuteVolSet(u16 fadeTimer);
void Na_Seq1PlayingCheck(void);

void unused_8031E4F0(void)
{
	// This is a debug function which is almost entirely optimized away,
	// except for loops, string literals, and a read of a volatile variable.
	// The string literals have allowed it to be partially reconstructed.
	s32 i;

	stubbed_printf("AUTOSEQ ");
	stubbed_printf("%2x %2x <%5x : %5x / %5x>\n", gSeqLoadedPool.temporary.entries[0].id, gSeqLoadedPool.temporary.entries[1].id, gSeqLoadedPool.temporary.entries[0].size, gSeqLoadedPool.temporary.entries[1].size, gSeqLoadedPool.temporary.pool.size);

	stubbed_printf("AUTOBNK ");
	stubbed_printf(
	    "%2x %3x <%5x : %5x / %5x>\n", gBankLoadedPool.temporary.entries[0].id, gBankLoadedPool.temporary.entries[1].id, gBankLoadedPool.temporary.entries[0].size, gBankLoadedPool.temporary.entries[1].size, gBankLoadedPool.temporary.pool.size);

	stubbed_printf("STAYSEQ ");
	stubbed_printf("[%2x] <%5x / %5x>\n", gSeqLoadedPool.persistent.numEntries, gSeqLoadedPool.persistent.pool.cur - gSeqLoadedPool.persistent.pool.start, gSeqLoadedPool.persistent.pool.size);
	for(i = 0; (u32)i < gSeqLoadedPool.persistent.numEntries; i++)
	{
		stubbed_printf("%2x ", gSeqLoadedPool.persistent.entries[i].id);
	}
	stubbed_printf("\n");

	stubbed_printf("STAYBNK ");
	stubbed_printf("[%2x] <%5x / %5x>\n", gBankLoadedPool.persistent.numEntries, gBankLoadedPool.persistent.pool.cur - gBankLoadedPool.persistent.pool.start, gBankLoadedPool.persistent.pool.size);
	for(i = 0; (u32)i < gBankLoadedPool.persistent.numEntries; i++)
	{
		stubbed_printf("%2x ", gBankLoadedPool.persistent.entries[i].id);
	}
	stubbed_printf("\n\n");

	stubbed_printf("    0123456789ABCDEF0123456789ABCDEF01234567\n");
	stubbed_printf("--------------------------------------------\n");

	// gSeqLoadStatus/gBankLoadStatus, 4 entries at a time?
	stubbed_printf("SEQ ");
	for(i = 0; i < 40; i++)
	{
		stubbed_printf("%1x", 0);
	}
	stubbed_printf("\n");

	stubbed_printf("BNK ");
	for(i = 0; i < 40; i += 4)
	{
		stubbed_printf("%1x", 0);
	}
	stubbed_printf("\n");

	stubbed_printf("FIXHEAP ");
	stubbed_printf("%4x / %4x\n", 0, 0);
	stubbed_printf("DRVHEAP ");
	stubbed_printf("%5x / %5x\n", 0, 0);
	stubbed_printf("DMACACHE  %4d Blocks\n", 0);
	stubbed_printf("CHANNELS  %2d / MAX %3d \n", 0, 0);

	stubbed_printf("TEMPOMAX  %d\n", gTempoInternalToExternal / TEMPO_SCALE);
	stubbed_printf("TEMPO G0  %d\n", GROUP_TRACK[0].tempo / TEMPO_SCALE);
	stubbed_printf("TEMPO G1  %d\n", GROUP_TRACK[1].tempo / TEMPO_SCALE);
	stubbed_printf("TEMPO G2  %d\n", GROUP_TRACK[2].tempo / TEMPO_SCALE);
	stubbed_printf("DEBUGFLAG  %8x\n", gAudioErrorFlags);
}

void unused_8031E568(void)
{
	stubbed_printf("COUNT %8d\n", gAudioFrameCount);
}

/**
 * Fade out a sequence player
 */
static void Nap_SetS32(s32 player, FadeT fadeOutTime)
{
	struct SequencePlayer* seqPlayer = &GROUP_TRACK[player];
	if(fadeOutTime == 0)
	{
		fadeOutTime++;
	}

	seqPlayer->fadeVelocity = -(seqPlayer->fadeVolume / fadeOutTime);
	seqPlayer->state	= SEQUENCE_PLAYER_STATE_FADE_OUT;
	seqPlayer->fadeTimer	= fadeOutTime;
}

static void func_8031D690(s32 player, FadeT fadeInTime)
{
	struct SequencePlayer* seqPlayer = &GROUP_TRACK[player];

	if(fadeInTime == 0 || seqPlayer->state == SEQUENCE_PLAYER_STATE_FADE_OUT)
	{
		return;
	}

	seqPlayer->state	= SEQUENCE_PLAYER_STATE_2;
	seqPlayer->fadeTimer	= fadeInTime;
	seqPlayer->fadeVolume	= 0.0f;
	seqPlayer->fadeVelocity = 0.0f;
}

static void GroupVolMove(s32 player, FadeT fadeTimer, u8 fadePercentage)
{
	struct SequencePlayer* seqPlayer = &GROUP_TRACK[player];
	f32 fadeVolume;

	if(seqPlayer->state == SEQUENCE_PLAYER_STATE_FADE_OUT)
	{
		return;
	}

	fadeVolume	     = (FLOAT_CAST(fadePercentage) / 100.0 * seqPlayer->fadeVolume);
	seqPlayer->volume    = seqPlayer->fadeVolume;
	seqPlayer->fadeTimer = 0;
	if(fadeTimer == 0)
	{
		seqPlayer->fadeVolume = fadeVolume;
		return;
	}
	seqPlayer->fadeVelocity = (fadeVolume - seqPlayer->fadeVolume) / fadeTimer;
	seqPlayer->state	= SEQUENCE_PLAYER_STATE_4;
	seqPlayer->fadeTimer	= fadeTimer;
}

static void GroupVolRestore(s32 player, FadeT fadeTimer)
{
	struct SequencePlayer* seqPlayer = &GROUP_TRACK[player];

	if(seqPlayer->state == SEQUENCE_PLAYER_STATE_FADE_OUT)
	{
		return;
	}

	seqPlayer->fadeTimer = 0;
	if(fadeTimer == 0)
	{
		seqPlayer->fadeVolume = seqPlayer->volume;
		return;
	}
	seqPlayer->fadeVelocity = (seqPlayer->volume - seqPlayer->fadeVolume) / fadeTimer;
	seqPlayer->state	= SEQUENCE_PLAYER_STATE_2;
	seqPlayer->fadeTimer	= fadeTimer;
}

static void GroupVolMoveDirect(s32 player, FadeT fadeInTime, u8 targetVolume)
{
	struct SequencePlayer* seqPlayer = &GROUP_TRACK[player];

	if(seqPlayer->state == SEQUENCE_PLAYER_STATE_FADE_OUT)
	{
		return;
	}

	seqPlayer->fadeTimer = 0;
	if(fadeInTime == 0)
	{
		seqPlayer->fadeVolume = (FLOAT_CAST(targetVolume) / 127.0);
		return;
	}
	seqPlayer->fadeVelocity = (((f32)(FLOAT_CAST(targetVolume) / 127.0) - seqPlayer->fadeVolume) / (f32)fadeInTime);
	seqPlayer->state	= SEQUENCE_PLAYER_STATE_4;
	seqPlayer->fadeTimer	= fadeInTime;
}

struct SPTask* create_next_audio_frame_task(void)
{
	return NULL;
}

void create_next_audio_buffer(s16* samples, u32 num_samples)
{
	gAudioFrameCount++;
	if(sGameLoopTicked != 0)
	{
		Naa_AllSeCheck();
		sGameLoopTicked = 0;
	}
	s32 writtenCmds;
	synthesis_execute(gAudioCmdBuffers[0], &writtenCmds, (u16*)samples, num_samples);
	gAudioRandom = ((gAudioRandom + gAudioFrameCount) * gAudioFrameCount);
	decrease_sample_dma_ttls();
}

void AudStartSound(s32 soundBits, f32* pos)
{
	sSoundRequests[sSoundRequestCount].soundBits = soundBits;
	sSoundRequests[sSoundRequestCount].position  = pos;
	sSoundRequestCount++;
}

static void process_sound_request(u32 bits, f32* pos)
{
	u8 bankIndex;
	u8 index;
	u8 counter = 0;
	u8 soundId;
	f32 dist;
	const f32 one = 1.0f;

	bankIndex = (bits & SOUNDARGS_MASK_BANK) >> SOUNDARGS_SHIFT_BANK;
	soundId	  = (bits & SOUNDARGS_MASK_SOUNDID) >> SOUNDARGS_SHIFT_SOUNDID;
	if(soundId >= sNumSoundsPerBank[bankIndex] || lockstat[bankIndex])
	{
		return;
	}

	index = gSoundBanks[bankIndex][0].next;
	while(index != 0xff && index != 0)
	{
		if(gSoundBanks[bankIndex][index].x == pos)
		{
			if((gSoundBanks[bankIndex][index].soundBits & SOUNDARGS_MASK_PRIORITY) <= (bits & SOUNDARGS_MASK_PRIORITY))
			{
				if((gSoundBanks[bankIndex][index].soundBits & SOUND_LO_BITFLAG_UNK8) != 0 || (bits & SOUNDARGS_MASK_SOUNDID) != (gSoundBanks[bankIndex][index].soundBits & SOUNDARGS_MASK_SOUNDID))
				{
					Na_SeBgmMuteOff(bankIndex, index);
					gSoundBanks[bankIndex][index].soundBits	  = bits;
					gSoundBanks[bankIndex][index].soundStatus = bits & SOUNDARGS_MASK_STATUS;
				}
				gSoundBanks[bankIndex][index].unk19 = 10;
			}
			index = 0;
		}
		else
		{
			index = gSoundBanks[bankIndex][index].next;
		}
		counter++;
	}

	if(counter == 0)
	{
		ObjSpeed[bankIndex] = 32;
	}

	if(gSoundBanks[bankIndex][D_803320B0[bankIndex]].next != 0xff && index != 0)
	{
		index						   = D_803320B0[bankIndex];
		dist						   = sqrtf(pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2]) * one;
		gSoundBanks[bankIndex][index].x			   = &pos[0];
		gSoundBanks[bankIndex][index].y			   = &pos[1];
		gSoundBanks[bankIndex][index].z			   = &pos[2];
		gSoundBanks[bankIndex][index].distance		   = dist;
		gSoundBanks[bankIndex][index].soundBits		   = bits;
		gSoundBanks[bankIndex][index].soundStatus	   = bits & SOUNDARGS_MASK_STATUS;
		gSoundBanks[bankIndex][index].unk19		   = 10;
		gSoundBanks[bankIndex][index].prev		   = D_803320A4[bankIndex];
		gSoundBanks[bankIndex][D_803320A4[bankIndex]].next = D_803320B0[bankIndex];
		D_803320A4[bankIndex]				   = D_803320B0[bankIndex];
		D_803320B0[bankIndex]				   = gSoundBanks[bankIndex][D_803320B0[bankIndex]].next;
		gSoundBanks[bankIndex][D_803320B0[bankIndex]].prev = 0xff;
		gSoundBanks[bankIndex][index].next		   = 0xff;
	}
}

/**
 * Processes all sound requests
 */
static void Na_AllSeBufferEntry(void)
{
	struct Sound* sound;

	while(sSoundRequestCount != sNumProcessedSoundRequests)
	{
		sound = &sSoundRequests[sNumProcessedSoundRequests];
		process_sound_request(sound->soundBits, sound->position);
		sNumProcessedSoundRequests++;
	}
}

static void Na_SeFlagFree(u8 bankIndex, u8 item)
{
	// move item from list D_803320A4 to list D_803320B0
	if(D_803320A4[bankIndex] == item)
	{
		D_803320A4[bankIndex] = gSoundBanks[bankIndex][item].prev;
	}
	else
	{
		gSoundBanks[bankIndex][gSoundBanks[bankIndex][item].next].prev = gSoundBanks[bankIndex][item].prev;
	}

	gSoundBanks[bankIndex][gSoundBanks[bankIndex][item].prev].next = gSoundBanks[bankIndex][item].next;
	gSoundBanks[bankIndex][item].next			       = D_803320B0[bankIndex];
	gSoundBanks[bankIndex][item].prev			       = -1;
	gSoundBanks[bankIndex][D_803320B0[bankIndex]].prev	       = item;
	D_803320B0[bankIndex]					       = item;
}

static void Na_SeBgmMuteOff(u8 bankIndex, u8 item)
{
	if(gSoundBanks[bankIndex][item].soundBits & SOUND_LO_BITFLAG_UNK1)
	{
		D_80332110 &= (1 << bankIndex) ^ 0xffff;
		Na_Seq0MuteVolSet(50);
	}
}

static void Na_SeTrackEntry(u8 bankIndex)
{
	u32 val2;
	u8 spDB;
	u8 i;
	u8 j;
	u8 soundIndex;
	u32 sp98[16] = {0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000};
	u8 sp88[16]  = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	u8 sp78[16]  = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	u8 sp77	     = 0;
	u8 val;

	soundIndex = gSoundBanks[bankIndex][0].next;
	while(soundIndex != 0xff)
	{
		spDB = soundIndex;

		if((gSoundBanks[bankIndex][soundIndex].soundBits & (SOUND_LO_BITFLAG_UNK8 | SOUNDARGS_MASK_STATUS)) == (SOUND_LO_BITFLAG_UNK8 | SOUND_STATUS_STARTING))
		{
			if(gSoundBanks[bankIndex][soundIndex].unk19-- == 0)
			{
				gSoundBanks[bankIndex][soundIndex].soundBits = NO_SOUND;
			}
		}
		else if((gSoundBanks[bankIndex][soundIndex].soundBits & SOUND_LO_BITFLAG_UNK8) == 0)
		{
			if(gSoundBanks[bankIndex][soundIndex].unk19-- == 8)
			{
				Na_SeBgmMuteOff(bankIndex, soundIndex);
				gSoundBanks[bankIndex][soundIndex].soundBits = NO_SOUND;
			}
		}

		if(gSoundBanks[bankIndex][soundIndex].soundBits == NO_SOUND && gSoundBanks[bankIndex][soundIndex].soundStatus == SOUND_STATUS_STARTING)
		{
			spDB					       = gSoundBanks[bankIndex][soundIndex].prev;
			gSoundBanks[bankIndex][soundIndex].soundStatus = SOUND_STATUS_STOPPED;
			Na_SeFlagFree(bankIndex, soundIndex);
		}

		if(gSoundBanks[bankIndex][soundIndex].soundStatus != SOUND_STATUS_STOPPED && soundIndex == spDB)
		{
			gSoundBanks[bankIndex][soundIndex].distance = sqrtf(
									  (*gSoundBanks[bankIndex][soundIndex].x * *gSoundBanks[bankIndex][soundIndex].x) + (*gSoundBanks[bankIndex][soundIndex].y * *gSoundBanks[bankIndex][soundIndex].y) +
									  (*gSoundBanks[bankIndex][soundIndex].z * *gSoundBanks[bankIndex][soundIndex].z)) *
								      1;

			val = (gSoundBanks[bankIndex][soundIndex].soundBits & SOUNDARGS_MASK_PRIORITY) >> SOUNDARGS_SHIFT_PRIORITY;
			if(gSoundBanks[bankIndex][soundIndex].soundBits & SOUND_PL_BITFLAG_UNK4)
			{
				gSoundBanks[bankIndex][soundIndex].priority = 0x4c * (0xff - val);
			}
			else if(*gSoundBanks[bankIndex][soundIndex].z > 0.0f)
			{
				gSoundBanks[bankIndex][soundIndex].priority = (u32)gSoundBanks[bankIndex][soundIndex].distance + (u32)(*gSoundBanks[bankIndex][soundIndex].z / US_FLOAT(6.0)) + 0x4c * (0xff - val);
			}
			else
			{
				gSoundBanks[bankIndex][soundIndex].priority = (u32)gSoundBanks[bankIndex][soundIndex].distance + 0x4c * (0xff - val);
			}

			for(i = 0; i < sMaxChannelsForSoundBank[bankIndex]; i++)
			{
				if(sp98[i] >= gSoundBanks[bankIndex][soundIndex].priority)
				{
					for(j = sMaxChannelsForSoundBank[bankIndex] - 1; j > i; j--)
					{
						sp98[j] = sp98[j - 1];
						sp88[j] = sp88[j - 1];
						sp78[j] = sp78[j - 1];
					}
					sp98[i] = gSoundBanks[bankIndex][soundIndex].priority;
					sp88[i] = soundIndex;
					sp78[i] = gSoundBanks[bankIndex][soundIndex].soundStatus;
					i	= sMaxChannelsForSoundBank[bankIndex];
				}
			}
			sp77++;
		}
		soundIndex = gSoundBanks[bankIndex][spDB].next;
	}

	D_803320BC[bankIndex]		     = sp77;
	sUsedChannelsForSoundBank[bankIndex] = sMaxChannelsForSoundBank[bankIndex];

	for(i = 0; i < sUsedChannelsForSoundBank[bankIndex]; i++)
	{
		for(soundIndex = 0; soundIndex < sUsedChannelsForSoundBank[bankIndex]; soundIndex++)
		{
			if(sp88[soundIndex] != 0xff && sCurrentSound[bankIndex][i] == sp88[soundIndex])
			{
				sp88[soundIndex] = 0xff;
				soundIndex	 = 0xfe;
			}
		}

		if(soundIndex != 0xff)
		{
			if(sCurrentSound[bankIndex][i] != 0xff)
			{
				if(gSoundBanks[bankIndex][sCurrentSound[bankIndex][i]].soundBits == NO_SOUND)
				{
					if(gSoundBanks[bankIndex][sCurrentSound[bankIndex][i]].soundStatus == SOUND_STATUS_PLAYING)
					{
						gSoundBanks[bankIndex][sCurrentSound[bankIndex][i]].soundStatus = SOUND_STATUS_STOPPED;
						Na_SeFlagFree(bankIndex, sCurrentSound[bankIndex][i]);
					}
				}
				val2 = gSoundBanks[bankIndex][sCurrentSound[bankIndex][i]].soundBits & (SOUND_LO_BITFLAG_UNK8 | SOUNDARGS_MASK_STATUS);
				if(val2 >= (SOUND_LO_BITFLAG_UNK8 | SOUND_STATUS_PLAYING) && gSoundBanks[bankIndex][sCurrentSound[bankIndex][i]].soundStatus != SOUND_STATUS_STOPPED)
				{
					Na_SeBgmMuteOff(bankIndex, sCurrentSound[bankIndex][i]);
					gSoundBanks[bankIndex][sCurrentSound[bankIndex][i]].soundBits	= NO_SOUND;
					gSoundBanks[bankIndex][sCurrentSound[bankIndex][i]].soundStatus = SOUND_STATUS_STOPPED;
					Na_SeFlagFree(bankIndex, sCurrentSound[bankIndex][i]);
				}
				else
				{
					if(val2 == SOUND_STATUS_PLAYING && gSoundBanks[bankIndex][sCurrentSound[bankIndex][i]].soundStatus != SOUND_STATUS_STOPPED)
					{
						gSoundBanks[bankIndex][sCurrentSound[bankIndex][i]].soundStatus = SOUND_STATUS_STARTING;
					}
				}
			}
			sCurrentSound[bankIndex][i] = 0xff;
		}
	}

	for(soundIndex = 0; soundIndex < sUsedChannelsForSoundBank[bankIndex]; soundIndex++)
	{
		if(sp88[soundIndex] != 0xff)
		{
			for(i = 0; i < sUsedChannelsForSoundBank[bankIndex]; i++)
			{
				if(sCurrentSound[bankIndex][i] == 0xff)
				{
					sCurrentSound[bankIndex][i]			   = sp88[soundIndex];
					gSoundBanks[bankIndex][sp88[soundIndex]].soundBits = (gSoundBanks[bankIndex][sp88[soundIndex]].soundBits & ~SOUNDARGS_MASK_STATUS) + 1;
					sp88[i]						   = 0xff;
					i						   = 0xfe; // "break;"
				}
			}
		}
	}
}

/**
 * Given an x and z coordinates, return the pan. This is a value between 0 and
 * 1 that represents the audio direction.
 *
 * Pan:
 * 0.0 - fully left
 * 0.5 - center pan
 * 1.0 - fully right
 */
static f32 Na_SePan(f32 x, f32 z)
{
	f32 absX;
	f32 absZ;
	f32 pan;

	absX = (x < 0 ? -x : x);
	if(absX > AUDIO_MAX_DISTANCE)
	{
		absX = AUDIO_MAX_DISTANCE;
	}

	absZ = (z < 0 ? -z : z);
	if(absZ > AUDIO_MAX_DISTANCE)
	{
		absZ = AUDIO_MAX_DISTANCE;
	}

	// There are 4 panning equations (12-hr clock used for angles)
	// 1. (0,0) fully-centered pan
	// 2. far right pan: between 1:30 and 4:30
	// 3. far left pan: between 7:30 and 10:30
	// 4. center pan: between 4:30 and 7:30 or between 10:30 and 1:30
	if(x == US_FLOAT(0.0) && z == US_FLOAT(0.0))
	{
		// x and y being 0 results in a center pan
		pan = US_FLOAT(0.5);
	}
	else if(x >= US_FLOAT(0.0) && absX >= absZ)
	{
		// far right pan
		pan = US_FLOAT(1.0) - (US_FLOAT(44000.0) - absX) / (US_FLOAT(3.0) * (US_FLOAT(44000.0) - absZ));
	}
	else if(x < 0 && absX > absZ)
	{
		// far left pan
		pan = (US_FLOAT(44000.0) - absX) / (US_FLOAT(3.0) * (US_FLOAT(44000.0) - absZ));
	}
	else
	{
		// center pan
		pan = 0.5 + x / (US_FLOAT(6.0) * absZ);
	}

	return pan;
}

static f32 Na_SeVolume(u8 bankIndex, u8 item, f32 arg2)
{
	f32 f0;
	f32 intensity;
	s32 div = bankIndex < 3 ? 2 : 3;

	if(!(gSoundBanks[bankIndex][item].soundBits & SOUND_PL_BITFLAG_UNK1))
	{
		if(gSoundBanks[bankIndex][item].distance > AUDIO_MAX_DISTANCE)
		{
			intensity = 0.0f;
		}
		else
		{
			f0 = D_80332028[activeStageNo] / div;
			if(f0 < gSoundBanks[bankIndex][item].distance)
			{
				intensity = ((AUDIO_MAX_DISTANCE - gSoundBanks[bankIndex][item].distance) / (AUDIO_MAX_DISTANCE - f0)) * (1.0f - arg2);
			}
			else
			{
				intensity = 1.0f - gSoundBanks[bankIndex][item].distance / f0 * arg2;
			}
		}

		if(gSoundBanks[bankIndex][item].soundBits & SOUND_PL_BITFLAG_UNK2)
		{
			if(intensity >= 0.08f)
			{
				intensity -= (f32)(gAudioRandom & 0xf) / US_FLOAT(192.0);
			}
		}
	}
	else
	{
		intensity = 1.0f;
	}

	return arg2 * intensity * intensity + 1.0f - arg2;
}

static f32 Na_SePitch(u8 bankIndex, u8 item)
{
	f32 f2;

	if(!(gSoundBanks[bankIndex][item].soundBits & SOUND_PL_BITFLAG_UNK8))
	{
		f2 = gSoundBanks[bankIndex][item].distance / AUDIO_MAX_DISTANCE;
		if(gSoundBanks[bankIndex][item].soundBits & SOUND_PL_BITFLAG_UNK2)
		{
			f2 += (f32)(gAudioRandom & 0xff) / US_FLOAT(64.0);
		}
	}
	else
	{
		f2 = 0.0f;
	}
	return f2 / US_FLOAT(15.0) + US_FLOAT(1.0);
}

#define VAL 40.0f
static u8 Na_SeEffect(UNUSED u8 bankIndex, UNUSED u8 item, u8 channelIndex)
{
	u8 area;
	u8 level;
	u8 reverb;

	if(gSoundBanks[bankIndex][item].soundBits & SOUND_NO_ECHO)
	{
		level = 0;
		area  = 0;
	}
	else
	{
		level = (activeStageNo > LEVEL_MAX ? LEVEL_MAX : activeStageNo);
		area  = activeSceneNo - 1;
		if(area > 2)
		{
			area = 2;
		}
	}

	reverb = (u8)((u8)GROUP_TRACK[2].channels[channelIndex]->soundScriptIO[5] + gAreaEchoLevel[level][area] + (US_FLOAT(1.0) - GROUP_TRACK[2].channels[channelIndex]->volume) * VAL);

	if(reverb > 0x7f)
	{
		reverb = 0x7f;
	}
	return reverb;
}

#undef VAL
static void Na_AudioDprint(void)
{
}

void audio_signal_game_loop_tick(void)
{
	sGameLoopTicked = 1;
	Na_AudioDprint();
}

#define ARG2_VAL1 0.9f
#define ARG2_VAL2 0.8f

void Naa_AllSeCheck(void)
{
	u8 soundStatus;
	u8 soundId;
	u8 bankIndex;
	u8 j;
	u8 channelIndex = 0;
	u8 index;
	f32 ret;

	Na_AllSeBufferEntry();
	Na_AllMusicCheck();
	if(GROUP_TRACK[2].channels[0] == &DUMMYSUB)
	{
		return;
	}

	for(bankIndex = 0; bankIndex < SOUND_BANK_COUNT; bankIndex++)
	{
		Na_SeTrackEntry(bankIndex);
		for(j = 0; j < MAX_CHANNELS_PER_SOUND; j++)
		{
			index = sCurrentSound[bankIndex][j];
			if(index < 0xff && gSoundBanks[bankIndex][index].soundStatus != SOUND_STATUS_STOPPED)
			{
				soundStatus				  = gSoundBanks[bankIndex][index].soundBits & SOUNDARGS_MASK_STATUS;
				soundId					  = (gSoundBanks[bankIndex][index].soundBits >> SOUNDARGS_SHIFT_SOUNDID);
				gSoundBanks[bankIndex][index].soundStatus = soundStatus;
				if(soundStatus == SOUND_STATUS_STARTING)
				{
					if(gSoundBanks[bankIndex][index].soundBits & SOUND_LO_BITFLAG_UNK1)
					{
						D_80332110 |= 1 << bankIndex;
						Na_Seq0MuteVolSet(50);
					}

					gSoundBanks[bankIndex][index].soundBits++;
					gSoundBanks[bankIndex][index].soundStatus		     = SOUND_STATUS_PLAYING;
					GROUP_TRACK[2].channels[channelIndex]->soundScriptIO[4] = soundId;
					GROUP_TRACK[2].channels[channelIndex]->soundScriptIO[0] = 1;

					switch(bankIndex)
					{
						case 1:
							if(!(gSoundBanks[bankIndex][index].soundBits & SOUND_PL_BITFLAG_UNK8))
							{
								if(ObjSpeed[bankIndex] > 8)
								{
									ret						   = Na_SeVolume(bankIndex, index, ARG2_VAL1);
									GROUP_TRACK[2].channels[channelIndex]->volume = ret;
								}
								else
								{
									ret						   = Na_SeVolume(bankIndex, index, ARG2_VAL1);
									GROUP_TRACK[2].channels[channelIndex]->volume = (ObjSpeed[bankIndex] + 8.0f) / 16 * ret;
								}
								GROUP_TRACK[2].channels[channelIndex]->pan = Na_SePan(*gSoundBanks[bankIndex][index].x, *gSoundBanks[bankIndex][index].z);

								if((gSoundBanks[bankIndex][index].soundBits & SOUNDARGS_MASK_SOUNDID) == (SOUND_MOVING_FLYING & SOUNDARGS_MASK_SOUNDID))
								{
									ret						      = Na_SePitch(bankIndex, index);
									GROUP_TRACK[2].channels[channelIndex]->freqScale = ((f32)ObjSpeed[bankIndex] / US_FLOAT(80.0)) + ret;
								}
								else
								{
									ret						      = Na_SePitch(bankIndex, index);
									GROUP_TRACK[2].channels[channelIndex]->freqScale = ((f32)ObjSpeed[bankIndex] / US_FLOAT(400.0)) + ret;
								}
								GROUP_TRACK[2].channels[channelIndex]->reverb = Na_SeEffect(bankIndex, index, channelIndex);

								break;
							}
							// fallthrough
						case 7:
							GROUP_TRACK[2].channels[channelIndex]->volume    = 1.0f;
							GROUP_TRACK[2].channels[channelIndex]->pan	      = 0.5f;
							GROUP_TRACK[2].channels[channelIndex]->freqScale = 1.0f;
							break;
						case 0:
						case 2:
							GROUP_TRACK[2].channels[channelIndex]->volume    = Na_SeVolume(bankIndex, index, ARG2_VAL1);
							GROUP_TRACK[2].channels[channelIndex]->pan	      = Na_SePan(*gSoundBanks[bankIndex][index].x, *gSoundBanks[bankIndex][index].z);
							GROUP_TRACK[2].channels[channelIndex]->freqScale = Na_SePitch(bankIndex, index);
							GROUP_TRACK[2].channels[channelIndex]->reverb    = Na_SeEffect(bankIndex, index, channelIndex);
							break;
						case 3:
						case 4:
						case 5:
						case 6:
						case 8:
						case 9:
							GROUP_TRACK[2].channels[channelIndex]->reverb    = Na_SeEffect(bankIndex, index, channelIndex);
							GROUP_TRACK[2].channels[channelIndex]->volume    = Na_SeVolume(bankIndex, index, ARG2_VAL2);
							GROUP_TRACK[2].channels[channelIndex]->pan	      = Na_SePan(*gSoundBanks[bankIndex][index].x, *gSoundBanks[bankIndex][index].z);
							GROUP_TRACK[2].channels[channelIndex]->freqScale = Na_SePitch(bankIndex, index);
							break;
					}
				}
				else if(GROUP_TRACK[2].channels[channelIndex]->layers[0] == NULL)
				{
					Na_SeBgmMuteOff(bankIndex, index);
					gSoundBanks[bankIndex][index].soundStatus = SOUND_STATUS_STOPPED;
					Na_SeFlagFree(bankIndex, index);
				}
				else if(soundStatus == SOUND_STATUS_STOPPED && GROUP_TRACK[2].channels[channelIndex]->layers[0]->finished == FALSE)
				{
					Na_SeBgmMuteOff(bankIndex, index);
					GROUP_TRACK[2].channels[channelIndex]->soundScriptIO[0] = 0;
					Na_SeFlagFree(bankIndex, index);
				}
				else if(GROUP_TRACK[2].channels[channelIndex]->layers[0]->enabled == FALSE)
				{
					Na_SeBgmMuteOff(bankIndex, index);
					gSoundBanks[bankIndex][index].soundStatus = SOUND_STATUS_STOPPED;
					Na_SeFlagFree(bankIndex, index);
				}
				else
				{
					// Exactly the same code as before. Unfortunately we can't
					// make a macro out of this, because then everything ends up
					// on the same line after preprocessing, and the compiler,
					// somehow caring about line numbers, makes it not match (it
					// computes function arguments in the wrong order).
					switch(bankIndex)
					{
						case 1:
							if(!(gSoundBanks[bankIndex][index].soundBits & SOUND_PL_BITFLAG_UNK8))
							{
								if(ObjSpeed[bankIndex] > 8)
								{
									ret						   = Na_SeVolume(bankIndex, index, ARG2_VAL1);
									GROUP_TRACK[2].channels[channelIndex]->volume = ret;
								}
								else
								{
									ret						   = Na_SeVolume(bankIndex, index, ARG2_VAL1);
									GROUP_TRACK[2].channels[channelIndex]->volume = (ObjSpeed[bankIndex] + 8.0f) / 16 * ret;
								}
								GROUP_TRACK[2].channels[channelIndex]->pan = Na_SePan(*gSoundBanks[bankIndex][index].x, *gSoundBanks[bankIndex][index].z);

								if((gSoundBanks[bankIndex][index].soundBits & SOUNDARGS_MASK_SOUNDID) == (SOUND_MOVING_FLYING & SOUNDARGS_MASK_SOUNDID))
								{
									ret						      = Na_SePitch(bankIndex, index);
									GROUP_TRACK[2].channels[channelIndex]->freqScale = ((f32)ObjSpeed[bankIndex] / US_FLOAT(80.0)) + ret;
								}
								else
								{
									ret						      = Na_SePitch(bankIndex, index);
									GROUP_TRACK[2].channels[channelIndex]->freqScale = ((f32)ObjSpeed[bankIndex] / US_FLOAT(400.0)) + ret;
								}
								GROUP_TRACK[2].channels[channelIndex]->reverb = Na_SeEffect(bankIndex, index, channelIndex);

								break;
							}
							// fallthrough
						case 7:
							GROUP_TRACK[2].channels[channelIndex]->volume    = 1.0f;
							GROUP_TRACK[2].channels[channelIndex]->pan	      = 0.5f;
							GROUP_TRACK[2].channels[channelIndex]->freqScale = 1.0f;
							break;
						case 0:
						case 2:
							GROUP_TRACK[2].channels[channelIndex]->volume    = Na_SeVolume(bankIndex, index, ARG2_VAL1);
							GROUP_TRACK[2].channels[channelIndex]->pan	      = Na_SePan(*gSoundBanks[bankIndex][index].x, *gSoundBanks[bankIndex][index].z);
							GROUP_TRACK[2].channels[channelIndex]->freqScale = Na_SePitch(bankIndex, index);
							GROUP_TRACK[2].channels[channelIndex]->reverb    = Na_SeEffect(bankIndex, index, channelIndex);
							break;
						case 3:
						case 4:
						case 5:
						case 6:
						case 8:
						case 9:
							GROUP_TRACK[2].channels[channelIndex]->reverb    = Na_SeEffect(bankIndex, index, channelIndex);
							GROUP_TRACK[2].channels[channelIndex]->volume    = Na_SeVolume(bankIndex, index, ARG2_VAL2);
							GROUP_TRACK[2].channels[channelIndex]->pan	      = Na_SePan(*gSoundBanks[bankIndex][index].x, *gSoundBanks[bankIndex][index].z);
							GROUP_TRACK[2].channels[channelIndex]->freqScale = Na_SePitch(bankIndex, index);
							break;
					}
				}
			}
			channelIndex++;
		}

		// sUsedChannelsForSoundBank[i] = sMaxChannelsForSoundBank[i] = 1, so this doesn't do anything
		channelIndex += sMaxChannelsForSoundBank[bankIndex] - sUsedChannelsForSoundBank[bankIndex];
	}
}

#undef ARG2_VAL1
#undef ARG2_VAL2

void Na_SeqStart(u8 player, u8 seqId, u16 fadeTimer)
{
	u8 temp_ret;
	u8 i;

	if(player == 0)
	{
		sPlayer0CurSeqId	    = seqId & 0x7f;
		sBackgroundMusicForDynamics = SEQUENCE_NONE;
		sCurrentMusicDynamic	    = 0xff;
		music_num_wait	    = 2;
	}

	for(i = 0; i < 0x10; i++)
	{
		D_80360928[player][i].remDuration = 0;
	}

	GROUP_TRACK[player].seqVariation = seqId & 0x80;
	load_sequence(player, seqId & 0x7f, 0);
	if(player == 0)
	{
		temp_ret = Na_Seq0MuteVolSet(0);
		if(temp_ret != 0xff)
		{
			GROUP_TRACK[0].state      = SEQUENCE_PLAYER_STATE_4;
			GROUP_TRACK[0].fadeVolume = (f32)temp_ret / US_FLOAT(127.0);
		}
	}
	else
	{
		func_8031D690(player, fadeTimer);
	}
}

void Na_BossBgmStop(u8 player, u16 fadeTimer)
{
	if(player == 0)
	{
		sPlayer0CurSeqId = SEQUENCE_NONE;
	}
	Nap_SetS32(player, fadeTimer);
}

void Na_AllSeqSubVolumeTrans(u8 player, u8 targetScale, u16 fadeTimer)
{
	u8 i;
	for(i = 0; i < CHANNELS_MAX; i++)
	{
		Na_SeqSubVolumeTrans(player, i, targetScale, fadeTimer);
	}
}

void Na_SeqSubVolumeTrans(u8 player, u8 channelId, u8 targetScale, u16 fadeTimer)
{
	struct ChannelVolumeScaleFade* temp;

	if(GROUP_TRACK[player].channels[channelId] != &DUMMYSUB)
	{
		temp		  = &D_80360928[player][channelId];
		temp->remDuration = fadeTimer;
		temp->velocity	  = ((f32)(targetScale / US_FLOAT(127.0)) - GROUP_TRACK[player].channels[channelId]->volumeScale) / fadeTimer;
		temp->target	  = targetScale;
		temp->current	  = GROUP_TRACK[player].channels[channelId]->volumeScale;
	}
}

void Na_SeqSubVolumeMove(u8 player)
{
	u8 i;

	// Loop over channels
	for(i = 0; i < CHANNELS_MAX; i++)
	{
		if(GROUP_TRACK[player].channels[i] != &DUMMYSUB && D_80360928[player][i].remDuration != 0)
		{
			D_80360928[player][i].current += D_80360928[player][i].velocity;
			GROUP_TRACK[player].channels[i]->volumeScale = D_80360928[player][i].current;
			D_80360928[player][i].remDuration--;
			if(D_80360928[player][i].remDuration == 0)
			{
				GROUP_TRACK[player].channels[i]->volumeScale = D_80360928[player][i].target / 127.0f;
			}
		}
	}
}

void Na_AllMusicCheck(void)
{
	s32 conditionBits;	// s0
	u8 musicDynIndex;	// sp57 87
	u8 j;			// v0
	s16 conditionValues[8]; // sp44 68
	u8 conditionTypes[8];	// sp3C 60
	s16 dur1;		// sp3A 58
	s16 dur2;		// sp38 56
	u16 bit;		// a1 (in first loop), s0, v1
	u8 i;			// s1
	u8 condIndex;		// a0, v1
	u32 tempBits;		// v1

	Na_SeqSubVolumeMove(0);
	Na_SeqSubVolumeMove(2);
	Na_Seq1PlayingCheck();
	if(music_num_wait != 0)
	{
		music_num_wait--;
	}
	else
	{
		sBackgroundMusicForDynamics = sPlayer0CurSeqId;
	}

	if(activeStageNo >= LEVEL_COUNT || sBackgroundMusicForDynamics != sLevelDynamics[activeStageNo][0])
	{
		return;
	}

	conditionBits = sLevelDynamics[activeStageNo][1] & 0xff00;
	musicDynIndex = sLevelDynamics[activeStageNo][1] & 0xff;
	i	      = 2;
	while(conditionBits & 0xff00)
	{
		for(j = 0, condIndex = 0, bit = 0x8000; j<8; j++, bit = bit >> 1)
		{
			if(conditionBits & bit)
			{
				conditionValues[condIndex] = sLevelDynamics[activeStageNo][i++];
				conditionTypes[condIndex]  = j;
				condIndex++;
			}
		}

		for(j = 0; j < condIndex; j++)
		{
			// (having all 'temp' share a single variable affects regalloc)
			switch(conditionTypes[j])
			{
				case MARIO_X_GE:
				{
					s16 temp = playerWorks[0].pos[0];
					if(temp < conditionValues[j])
						j = condIndex + 1;
					break;
				}
				case MARIO_Y_GE:
				{
					s16 temp = playerWorks[0].pos[1];
					if(temp < conditionValues[j])
						j = condIndex + 1;
					break;
				}
				case MARIO_Z_GE:
				{
					s16 temp = playerWorks[0].pos[2];
					if(temp < conditionValues[j])
						j = condIndex + 1;
					break;
				}
				case MARIO_X_LT:
				{
					s16 temp = playerWorks[0].pos[0];
					if(temp >= conditionValues[j])
						j = condIndex + 1;
					break;
				}
				case MARIO_Y_LT:
				{
					s16 temp = playerWorks[0].pos[1];
					if(temp >= conditionValues[j])
						j = condIndex + 1;
					break;
				}
				case MARIO_Z_LT:
				{
					s16 temp = playerWorks[0].pos[2];
					if(temp >= conditionValues[j])
						j = condIndex + 1;
					break;
				}
				case MARIO_IS_IN_AREA:
				{
					// s16 temp = activeSceneNo;
					if(activeSceneNo != conditionValues[j])
						j = condIndex + 1;
					break;
				}
				case MARIO_IS_IN_ROOM:
				{
					// s16 temp = gMarioCurrentRoom;
					if(gMarioCurrentRoom != conditionValues[j])
						j = condIndex + 1;
					break;
				}
			}
		}

		if(j == condIndex)
		{
			// The area matches. Break out of the loop.
			tempBits = 0;
		}
		else
		{
			tempBits = sLevelDynamics[activeStageNo][i];
			i++;
			musicDynIndex = tempBits & 0xff, tempBits &= 0xff00;
		}

		conditionBits = tempBits;
	}

	if(musicDynIndex != sCurrentMusicDynamic)
	{
		bit = 1;
		if(sCurrentMusicDynamic == 0xff)
		{
			dur1 = 1;
			dur2 = 1;
		}
		else
		{
			dur1 = sMusicDynamics[musicDynIndex].dur1;
			dur2 = sMusicDynamics[musicDynIndex].dur2;
		}

		for(i = 0; i < CHANNELS_MAX; i++)
		{
			if(sMusicDynamics[musicDynIndex].bits1 & bit)
			{
				Na_SeqSubVolumeTrans(0, i, sMusicDynamics[musicDynIndex].volScale1, dur1);
			}
			if(sMusicDynamics[musicDynIndex].bits2 & bit)
			{
				Na_SeqSubVolumeTrans(0, i, sMusicDynamics[musicDynIndex].volScale2, dur2);
			}
			bit <<= 1;
		}

		sCurrentMusicDynamic = musicDynIndex;
	}
}

void Na_SeqMixing(u8 player, u32 bits, s8 arg2)
{
	u8 i;

	if(arg2 < 0)
	{
		arg2 = -arg2;
	}

	for(i = 0; i < CHANNELS_MAX; i++)
	{
		if(GROUP_TRACK[player].channels[i] != &DUMMYSUB)
		{
			if((bits & 3) == 0)
			{
				GROUP_TRACK[player].channels[i]->volumeScale = 1.0f;
			}
			else if((bits & 1) != 0)
			{
				GROUP_TRACK[player].channels[i]->volumeScale = (f32)arg2 / US_FLOAT(127.0);
			}
			else
			{
				GROUP_TRACK[player].channels[i]->volumeScale = US_FLOAT(1.0) - (f32)arg2 / US_FLOAT(127.0);
			}
		}
		bits >>= 2;
	}
}

void Na_SeqVolMute(u8 player, u16 fadeTimer, u8 arg2)
{
	if(player == 0)
	{
		sCapVolumeTo40 = TRUE;
		Na_Seq0MuteVolSet(fadeTimer);
	}
	else if(GROUP_TRACK[player].enabled == TRUE)
	{
		GroupVolMove(player, fadeTimer, arg2);
	}
}

void Na_SeqVolRecover(u8 player, u16 fadeTimer)
{
	sCapVolumeTo40 = FALSE;
	if(player == 0)
	{
		if(GROUP_TRACK[player].state != SEQUENCE_PLAYER_STATE_FADE_OUT)
		{
			Na_Seq0MuteVolSet(fadeTimer);
		}
	}
	else
	{
		if(GROUP_TRACK[player].enabled == TRUE)
		{
			GroupVolRestore(player, fadeTimer);
		}
	}
}

// returns fade volume or 0xff for background music
u8 Na_Seq0MuteVolSet(u16 fadeTimer)
{
	u8 vol = 0xff;
	u8 temp;

	if(sPlayer0CurSeqId == SEQUENCE_NONE || sPlayer0CurSeqId == SEQ_EVENT_CUTSCENE_CREDITS)
	{
		return 0xff;
	}

	if(GROUP_TRACK[0].volume == 0.0f && fadeTimer)
	{
		GROUP_TRACK[0].volume = GROUP_TRACK[0].fadeVolume;
	}

	if(Seq1AddFlag != 0)
	{
		vol = (Seq1AddFlag & 0x7f);
	}

	if(Seq1PlayingMuteCount != 0)
	{
		temp = (Seq1PlayingMuteCount & 0x7f);
		if(vol > temp)
		{
			vol = temp;
		}
	}

	if(sCapVolumeTo40 && vol > 40)
	{
		vol = 40;
	}

	if(D_80332110 != 0 && vol > 20)
	{
		vol = 20;
	}

	if(GROUP_TRACK[0].enabled == TRUE)
	{
		if(vol != 0xff)
		{
			GroupVolMoveDirect(0, fadeTimer, vol);
		}
		else
		{
			GROUP_TRACK[0].volume = sBackgroundMusicDefaultVolume[sPlayer0CurSeqId] / 127.0f;
			GroupVolRestore(0, fadeTimer);
		}
	}
	return vol;
}

void Na_PauseSet(u8 disabled)
{
	u8 i;

	for(i = 0; i < SEQUENCE_PLAYERS; i++)
	{
		GROUP_TRACK[i].muted = disabled;
	}
}

void interface_init(void)
{
	u8 i;
	u8 j;

	for(i = 0; i < SOUND_BANK_COUNT; i++)
	{
		for(j = 0; j < 40; j++)
		{
			gSoundBanks[i][j].soundStatus = SOUND_STATUS_STOPPED;
		}

		for(j = 0; j < MAX_CHANNELS_PER_SOUND; j++)
		{
			sCurrentSound[i][j] = 0xff;
		}

		D_803320A4[i] = 0;
		D_803320B0[i] = 1;
		D_803320BC[i] = 0;
	}

	for(i = 0; i < SOUND_BANK_COUNT; i++)
	{
		gSoundBanks[i][0].prev = 0xff;
		gSoundBanks[i][0].next = 0xff;

		for(j = 1; j < 40 - 1; j++)
		{
			gSoundBanks[i][j].prev = j - 1;
			gSoundBanks[i][j].next = j + 1;
		}

		gSoundBanks[i][j].prev = j - 1;
		gSoundBanks[i][j].next = 0xff;
	}

	for(j = 0; j < SEQUENCE_PLAYERS; j++)
	{
		for(i = 0; i < CHANNELS_MAX; i++)
		{
			D_80360928[j][i].remDuration = 0;
		}
	}

	for(i = 0; i < MAX_BG_MUSIC_QUEUE_SIZE; i++)
	{
		sBackgroundMusicQueue[i].priority = 0;
	}

	Na_PortUnlock(2, 0xffff);
	sUnused80332118		   = 0;
	Seq1AddFlag		   = 0;
	sCapVolumeTo40		   = FALSE;
	D_80332110		   = 0;
	sUnused80332114		   = 0;
	sPlayer0CurSeqId	   = 0xff;
	gSoundMode		   = SOUND_MODE_STEREO;
	entrybgms  = 0;
	Seq1PlayingMuteCount		   = 0;
	Sub1MusicNum		   = 0;
	Sub1MusicVol		   = 0;
	sNumProcessedSoundRequests = 0;
	sSoundRequestCount	   = 0;
}

// (size)
void Na_SeInfoGet(u8 bankIndex, u8* numPlayingSounds, u8* arg2, u8* soundId)
{
	u8 i;
	u8 count = 0;

	for(i = 0; i < sMaxChannelsForSoundBank[bankIndex]; i++)
	{
		if(sCurrentSound[bankIndex][i] != 0xff)
		{
			count++;
		}
	}

	*numPlayingSounds = count;
	*arg2		  = D_803320BC[bankIndex];
	if(sCurrentSound[bankIndex][0] != 0xff)
	{
		*soundId = (u8)(gSoundBanks[bankIndex][sCurrentSound[bankIndex][0]].soundBits >> SOUNDARGS_SHIFT_SOUNDID);
	}
	else
	{
		*soundId = 0xff;
	}
}

void Na_SeEnd(u32 soundBits, f32* vec)
{
	u8 bankIndex;
	u8 item;

	bankIndex = (soundBits & SOUNDARGS_MASK_BANK) >> SOUNDARGS_SHIFT_BANK;
	item	  = gSoundBanks[bankIndex][0].next;
	while(item != 0xff)
	{
		if((u16)(soundBits >> SOUNDARGS_SHIFT_SOUNDID) == (u16)(gSoundBanks[bankIndex][item].soundBits >> SOUNDARGS_SHIFT_SOUNDID) && gSoundBanks[bankIndex][item].x == vec)
		{
			Na_SeBgmMuteOff(bankIndex, item);
			gSoundBanks[bankIndex][item].soundBits = NO_SOUND;
			item				       = 0xff;
		}
		else
		{
			item = gSoundBanks[bankIndex][item].next;
		}
	}
}

void Na_ObjSeEnd(f32* arg0)
{
	u8 bankIndex;
	u8 item;

	for(bankIndex = 0; bankIndex < SOUND_BANK_COUNT; bankIndex++)
	{
		item = gSoundBanks[bankIndex][0].next;
		while(item != 0xff)
		{
			if(gSoundBanks[bankIndex][item].x == arg0)
			{
				Na_SeBgmMuteOff(bankIndex, item);
				gSoundBanks[bankIndex][item].soundBits = NO_SOUND;
			}
			item = gSoundBanks[bankIndex][item].next;
		}
	}
}

static void Na_AllSeStop(u8 bankIndex)
{
	u8 item = gSoundBanks[bankIndex][0].next;

	while(item != 0xff)
	{
		Na_SeBgmMuteOff(bankIndex, item);
		gSoundBanks[bankIndex][item].soundBits = NO_SOUND;
		item				       = gSoundBanks[bankIndex][item].next;
	}
}

void Na_LevelSeAllStop(void)
{
	Na_AllSeStop(1);
	Na_AllSeStop(4);
	Na_AllSeStop(6);
}

void Na_PortLock(UNUSED u8 player, u16 bankMask)
{
	u8 i;

	for(i = 0; i < SOUND_BANK_COUNT; i++)
	{
		if(bankMask & 1)
		{
			lockstat[i] = TRUE;
		}
		bankMask = bankMask >> 1;
	}
}

static void Na_AllSeqStop(void)
{
	u8 i;

	for(i = 0; i < SEQUENCE_PLAYERS; i++)
	{
		Nas_ReleaseGroup(&GROUP_TRACK[i]);
	}
}

void Na_PortUnlock(UNUSED u8 player, u16 bankMask)
{
	u8 i;

	for(i = 0; i < SOUND_BANK_COUNT; i++)
	{
		if(bankMask & 1)
		{
			lockstat[i] = FALSE;
		}
		bankMask = bankMask >> 1;
	}
}

u8 Na_SubTrackMute(u8 player, u8 channelIndex, u8 arg2)
{
	u8 ret = 0;
	if(GROUP_TRACK[player].channels[channelIndex] != &DUMMYSUB)
	{
		GROUP_TRACK[player].channels[channelIndex]->stopSomething2 = arg2;
		ret								= arg2;
	}
	return ret;
}

void Na_SetObjSpeed(u8 bankIndex, u8 arg1)
{
	ObjSpeed[bankIndex] = arg1;
}

void Na_MessageSound(u8 dialogID)
{
	u8 speaker;

	if(dialogID >= 170)
	{
		dialogID = 0;
	}

	speaker = sDialogSpeaker[dialogID];
	if(speaker != 0xff)
	{
		AudStartSound(sDialogSpeakerVoice[speaker], gDefaultSoundArgs);
		if(speaker == 2) // SOUND_OBJ_BOWSER_INTRO_LAUGH
		{
			Na_SeqStart(1, SEQ_EVENT_KOOPA_MESSAGE, 0);
		}
	}

	// "You've stepped on the (Wing|Metal|Vanish) Cap Switch"
	if(dialogID == DIALOG_010 || dialogID == DIALOG_011 || dialogID == DIALOG_012)
	{
		Na_NazoClearBgm();
	}
}

void Na_MusicStart(u8 player, u16 seqArgs, s16 fadeTimer)
{
	u8 seqId    = seqArgs & 0xff;
	u8 priority = seqArgs >> 8;
	u8 i;
	u8 foundIndex = 0;

	// Except for the background music player, we don't support queued
	// sequences. Just play them immediately, stopping any old sequence.
	if(player != 0)
	{
		Na_SeqStart(player, seqId, fadeTimer);
		return;
	}

	// Abort if the queue is already full.
	if(entrybgms == MAX_BG_MUSIC_QUEUE_SIZE)
	{
		return;
	}

	// If already in the queue, abort, after first restarting the sequence if
	// it is first, and handling disabled music somehow.
	// (That handling probably ought to occur even when the queue is full...)
	for(i = 0; i < entrybgms; i++)
	{
		if(sBackgroundMusicQueue[i].seqId == seqId)
		{
			if(i == 0)
			{
				Na_SeqStart(0, seqId, fadeTimer);
			}
			else if(!GROUP_TRACK[0].enabled)
			{
				stop_background_music(sBackgroundMusicQueue[0].seqId);
			}
			return;
		}
	}

	// Find the next sequence slot by priority.
	for(i = 0; i < entrybgms; i++)
	{
		if(sBackgroundMusicQueue[i].priority <= priority)
		{
			foundIndex = i;
			i	   = entrybgms;
		}
	}

	// If the sequence ends up first in the queue, start it, and make space for
	// more entries in the queue.
	if(foundIndex == 0)
	{
		Na_SeqStart(0, seqId, fadeTimer);
		entrybgms++;
	}

	// Move all items up in queue, throwing away the last one if we didn't put
	// the new sequence first.
	for(i = entrybgms - 1; i > foundIndex; i--)
	{
		sBackgroundMusicQueue[i].priority = sBackgroundMusicQueue[i - 1].priority;
		sBackgroundMusicQueue[i].seqId	  = sBackgroundMusicQueue[i - 1].seqId;
	}

	// Insert item into queue.
	sBackgroundMusicQueue[foundIndex].priority = priority;
	sBackgroundMusicQueue[foundIndex].seqId	   = seqId;
}

void stop_background_music(u16 seqId)
{
	u8 foundIndex;
	u8 i;

	if(entrybgms == 0)
	{
		return;
	}

	// If sequence is not found, remove an empty queue item (the next empty
	// queue slot).
	foundIndex = entrybgms;

	// Search for the sequence.
	for(i = 0; i < entrybgms; i++)
	{
		if(sBackgroundMusicQueue[i].seqId == (u8)(seqId & 0xff))
		{
			// Remove sequence from queue. If it was first, play the next one,
			// or fade out the music.
			entrybgms--;
			if(i == 0)
			{
				if(entrybgms != 0)
				{
					Na_SeqStart(0, sBackgroundMusicQueue[1].seqId, 0);
				}
				else
				{
					Na_BossBgmStop(0, 20);
				}
			}
			foundIndex = i;
			i	   = entrybgms; // "break;"
		}
	}

	// Move later slots down.
	for(i = foundIndex; i < entrybgms; i++)
	{
		sBackgroundMusicQueue[i].priority = sBackgroundMusicQueue[i + 1].priority;
		sBackgroundMusicQueue[i].seqId	  = sBackgroundMusicQueue[i + 1].seqId;
	}

	// @bug? If the sequence queue is full and we attempt to stop a sequence
	// that isn't in the queue, this writes out of bounds. Can that happen?
	sBackgroundMusicQueue[i].priority = 0;
}

void fadeout_background_music(u16 seqId, u16 fadeOut)
{
	if(entrybgms != 0 && sBackgroundMusicQueue[0].seqId == (u8)(seqId & 0xff))
	{
		Na_BossBgmStop(0, fadeOut);
	}
}

void Na_AllBgmStackFlagFree(void)
{
	if(entrybgms != 0)
	{
		entrybgms = 1;
	}
}

u16 Na_GetPlayingBgmFlag(void)
{
	if(entrybgms != 0)
	{
		return (sBackgroundMusicQueue[0].priority << 8) + sBackgroundMusicQueue[0].seqId;
	}
	return -1;
}

void Na_Seq1PlayingCheck(void)
{
	if(GROUP_TRACK[1].enabled || Seq1PlayingMuteCount == 0)
	{
		return;
	}

	Seq1PlayingMuteCount = 0;
	Na_Seq0MuteVolSet(50);

	if(Seq1AddFlag != 0 && (Sub1MusicNum == SEQ_EVENT_MERRY_GO_ROUND || Sub1MusicNum == SEQ_EVENT_PIRANHA_PLANT))
	{
		Na_SeqStart(1, Sub1MusicNum, 1);
		if(Sub1MusicVol != 0xff)
		{
			GroupVolMoveDirect(1, 1, Sub1MusicVol);
		}
	}
}

void Na_AddSeq(u8 seqId, u8 bgMusicVolume, u8 volume, u16 fadeTimer)
{
	UNUSED u32 dummy;

	sUnused80332118 = 0;
	if(sPlayer0CurSeqId == 0xff || sPlayer0CurSeqId == SEQ_MENU_TITLE_SCREEN)
	{
		return;
	}

	if(Seq1AddFlag == 0)
	{
		Seq1AddFlag = bgMusicVolume + 0x80;
		Na_Seq0MuteVolSet(fadeTimer);
		Na_SeqStart(1, seqId, fadeTimer >> 1);
		if(volume < 0x80)
		{
			GroupVolMoveDirect(1, fadeTimer, volume);
		}
		Sub1MusicVol = volume;
		Sub1MusicNum = seqId;
	}
	else if(volume != 0xff)
	{
		Seq1AddFlag = bgMusicVolume + 0x80;
		Na_Seq0MuteVolSet(fadeTimer);
		GroupVolMoveDirect(1, fadeTimer, volume);
		Sub1MusicVol = volume;
	}
}

void Na_SubSeq(u16 fadeTimer)
{
	if(Seq1AddFlag != 0)
	{
		Seq1AddFlag = 0;
		Sub1MusicNum = 0;
		Sub1MusicVol = 0;
		Na_Seq0MuteVolSet(fadeTimer);
		Na_BossBgmStop(1, fadeTimer);
	}
}

void Na_SoundFadeOut(u16 fadeOutTime)
{
	u8 i;

	if(SoundFadeFlag)
	{
		return;
	}

	if(GROUP_TRACK[0].enabled == TRUE)
	{
		Nap_SetS32(0, fadeOutTime);
	}
	if(GROUP_TRACK[1].enabled == TRUE)
	{
		Nap_SetS32(1, fadeOutTime);
	}

	for(i = 0; i < SOUND_BANK_COUNT; i++)
	{
		if(i != 7)
		{
			Na_SeqSubVolumeTrans(2, i, 0, fadeOutTime / 16);
		}
	}
	SoundFadeFlag = TRUE;
}

void Na_ClearFanfare(void)
{
	Na_SeqStart(1, SEQ_EVENT_CUTSCENE_COLLECT_STAR, 0);
	Seq1PlayingMuteCount = 0x80 | 0;
	Na_Seq0MuteVolSet(50);
}

void Na_NewDoorMessageBgm(void)
{
	Na_SeqStart(1, SEQ_EVENT_PEACH_MESSAGE, 0);
	Seq1PlayingMuteCount = 0x80 | 0;
	Na_Seq0MuteVolSet(50);
}

/**
 * Plays the puzzle jingle. Plays the dadada dadada *dadada* jingle
 * that usually plays when you solve a "puzzle", like chests, talking to
 * yoshi, releasing chain chomp, opening the pyramid top, etc.
 */
void Na_NazoClearBgm(void)
{
	Na_SeqStart(1, SEQ_EVENT_SOLVE_PUZZLE, 0);
	Seq1PlayingMuteCount = 0x80 | 20;
	Na_Seq0MuteVolSet(50);
}

void Na_HiScoreBgm(void)
{
	Na_SeqStart(1, SEQ_EVENT_HIGH_SCORE, 0);
	Seq1PlayingMuteCount = 0x80 | 20;
	Na_Seq0MuteVolSet(50);
}

void Na_NewStarAppearBgm(u8 arg0)
{
	if(!arg0)
	{
		Seq1AddFlag = 0;
	}
	Na_SeqStart(1, SEQ_EVENT_CUTSCENE_STAR_SPAWN, 0);
	Seq1PlayingMuteCount = 0x80 | 20;
	Na_Seq0MuteVolSet(50);
}

void Na_RaceFanfareBgm(void)
{
	Na_SeqStart(1, SEQ_EVENT_RACE, 0);
	Seq1PlayingMuteCount = 0x80 | 20;
	Na_Seq0MuteVolSet(50);
}

void Na_kinopioAppearBgm(void)
{
	Na_SeqStart(1, SEQ_EVENT_TOAD_MESSAGE, 0);
	Seq1PlayingMuteCount = 0x80 | 20;
	Na_Seq0MuteVolSet(50);
}

void Na_StageChange(u8 arg0)
{
	if(arg0 >= 8)
	{
		arg0		= 0;
		sUnused8033323C = 0;
	}
	sGameLoopTicked = 0;
	Na_AllSeqStop();
	interface_init();
	audio_reset_session(&gAudioSessionPresets[arg0]);
	osWritebackDCacheAll();
	if(arg0 != 7)
	{
		preload_sequence(SEQ_EVENT_SOLVE_PUZZLE, PRELOAD_BANKS | PRELOAD_SEQUENCE);
		preload_sequence(SEQ_EVENT_PEACH_MESSAGE, PRELOAD_BANKS | PRELOAD_SEQUENCE);
		preload_sequence(SEQ_EVENT_CUTSCENE_STAR_SPAWN, PRELOAD_BANKS | PRELOAD_SEQUENCE);
	}
	Na_SeqStart(2, SEQ_SOUND_PLAYER, 0);
	D_80332108	   = (D_80332108 & 0xf0) + arg0;
	gSoundMode	   = D_80332108 >> 4;
	SoundFadeFlag = FALSE;
}

void Na_OutputMode(u8 soundMode)
{
	D_80332108 = (D_80332108 & 0xf) + (soundMode << 4);
	gSoundMode = soundMode;
}

void unused_80321460(UNUSED s32 arg0, UNUSED s32 arg1, UNUSED s32 arg2, UNUSED s32 arg3)
{
}

void unused_80321474(UNUSED s32 arg0)
{
}
