#include <ultra64.h>

#include "sm64.h"
#include "seq_ids.h"
#include "level_update.h"
#include "main.h"
#include "engine/math_util.h"
#include "area.h"
#include "audio/external.h"
#include "print.h"
#include "save_file.h"
#include "sound_init.h"
#include "engine/graph_node.h"
#include "paintings.h"
#include "level_table.h"
#include "game/motor.h"

#define MUSIC_NONE 0xFFFF

static Vec3f unused80339DC0;
static OSMesgQueue sSoundMesgQueue;
static OSMesg sSoundMesgBuf[1];
static struct VblankHandler sSoundVblankHandler;

static u8 D_8032C6C0		       = 0;
static u8 D_8032C6C4		       = 0;
static u16 sCurrentMusic	       = MUSIC_NONE;
static u16 sCurrentShellMusic	       = MUSIC_NONE;
static u16 sCurrentCapMusic	       = MUSIC_NONE;
static u8 sPlayingInfiniteStairs       = FALSE;
static u8 unused8032C6D8[16]	       = {0};
static s16 sSoundMenuModeToSoundMode[] = {SOUND_MODE_STEREO, SOUND_MODE_MONO, SOUND_MODE_HEADSET};
// Only the 20th array element is used.
static u32 menuSoundsExtra[] = {
    SOUND_MOVING_TERRAIN_SLIDE + (0 << 16),
    SOUND_MOVING_TERRAIN_SLIDE + (1 << 16),
    SOUND_MOVING_TERRAIN_SLIDE + (2 << 16),
    SOUND_MOVING_TERRAIN_SLIDE + (3 << 16),
    SOUND_MOVING_TERRAIN_SLIDE + (4 << 16),
    SOUND_MOVING_TERRAIN_SLIDE + (5 << 16),
    SOUND_MOVING_TERRAIN_SLIDE + (6 << 16),
    SOUND_MOVING_LAVA_BURN,
    SOUND_MOVING_SLIDE_DOWN_TREE,
    SOUND_MOVING_SLIDE_DOWN_POLE,
    SOUND_MOVING_QUICKSAND_DEATH,
    SOUND_MOVING_TERRAIN_RIDING_SHELL,
    NO_SOUND,
    SOUND_ENV_BOAT_ROCKING1,
    SOUND_ENV_ELEVATOR3,
    SOUND_ENV_UNKNOWN2,
    SOUND_ENV_WATERFALL1,
    SOUND_ENV_WATERFALL2,
    SOUND_ENV_ELEVATOR1,
    SOUND_ENV_DRONING1,
    SOUND_ENV_DRONING2,
    SOUND_ENV_ELEVATOR2,
    SOUND_ENV_WIND1,
    SOUND_ENV_WATER,
    SOUND_AIR_BOWSER_SPIT_FIRE,
    SOUND_MOVING_AIM_CANNON,
    SOUND_AIR_LAKITU_FLY,
    SOUND_AIR_AMP_BUZZ,
    SOUND_AIR_CASTLE_OUTDOORS_AMBIENT,
    SOUND_OBJ2_BIRD_CHIRP1,
    SOUND_GENERAL2_BIRD_CHIRP2,
    SOUND_OBJ_BIRD_CHIRP3,
    SOUND_ENV_ELEVATOR2,
    SOUND_AIR_BLOW_WIND,
    SOUND_AIR_BLOW_FIRE,
    SOUND_ENV_ELEVATOR4,
};
static s8 paintingEjectSoundPlayed = FALSE;

static void play_menu_sounds_extra(int a, void* b);

void AudResetMute(void)
{
	D_8032C6C0 = 0;
}

void AudStartMute(s32 a) // Soften volume
{
	switch(a)
	{
		case 1:
			set_sound_disabled(TRUE);
			break;
		case 2:
			func_8031FFB4(0, 60, 40); // soften music
			break;
	}
	D_8032C6C0 |= a;
}

void AudEndMute(s32 a) // harden volume
{
	switch(a)
	{
		case 1:
			set_sound_disabled(FALSE);
			break;
		case 2:
			Na_SeqVolRecover(0, 60);
			break;
	}
	D_8032C6C0 &= ~a;
}

void func_80248D48(void)
{
	if(D_8032C6C4 == 0)
	{
		D_8032C6C4 = 1;
		sound_banks_disable(2, 0x037A);
	}
}

void AudUnlockSound(void)
{
	if(D_8032C6C4 == 1)
	{
		D_8032C6C4 = 0;
		sound_banks_enable(2, 0x037A);
	}
}

/**
 * Sets the sound mode
 */
void set_sound_mode(u16 soundMode)
{
	if(soundMode < 3)
	{
		audio_set_sound_mode(sSoundMenuModeToSoundMode[soundMode]);
	}
}

/**
 * Wrapper method by menu used to set the sound via flags.
 */
void play_menu_sounds(s16 soundMenuFlags)
{
	if(soundMenuFlags & AUD_FACE_APPEAR)
	{
		AudStartSound(SOUND_MENU_HAND_APPEAR, gDefaultSoundArgs);
	}
	else if(soundMenuFlags & AUD_FACE_DISAPPEAR)
	{
		AudStartSound(SOUND_MENU_HAND_DISAPPEAR, gDefaultSoundArgs);
	}
	else if(soundMenuFlags & AUD_FACE_SPARK)
	{
		AudStartSound(SOUND_MENU_UNK0C, gDefaultSoundArgs);
	}
	else if(soundMenuFlags & AUD_FACE_PINCH)
	{
		AudStartSound(SOUND_MENU_PINCH_MARIO_FACE, gDefaultSoundArgs);
	}
	else if(soundMenuFlags & AUD_FACE_PULL)
	{
		AudStartSound(SOUND_MENU_PINCH_MARIO_FACE, gDefaultSoundArgs);
	}
	else if(soundMenuFlags & AUD_FACE_REFORM)
	{
		AudStartSound(SOUND_MENU_LET_GO_MARIO_FACE, gDefaultSoundArgs);
	}
	else if(soundMenuFlags & AUD_FACE_ZOOMUP)
	{
		AudStartSound(SOUND_MENU_CAMERA_ZOOM_IN, gDefaultSoundArgs);
	}
	else if(soundMenuFlags & AUD_FACE_ZOOMDOWN)
	{
		AudStartSound(SOUND_MENU_CAMERA_ZOOM_OUT, gDefaultSoundArgs);
	}

	if(soundMenuFlags & 0x100)
	{
		play_menu_sounds_extra(20, NULL);
	}

	if((soundMenuFlags & 0x20) != 0)
	{
		SendMotorEvent(5, 80);
	}
}

/**
 * Plays the painting eject sound effect if it has not already been played
 */
void AudPictWaveSound(void)
{
	if(ripplingPainting != NULL && ripplingPainting->rippleStatus == 2) // ripple when Mario enters painting
	{
		if(paintingEjectSoundPlayed == FALSE)
		{
			AudStartSound(SOUND_GENERAL_PAINTING_EJECT, playerWorks[0].marioObj->header.gfx.cameraToObject);
		}
		paintingEjectSoundPlayed = TRUE;
	}
	else
	{
		paintingEjectSoundPlayed = FALSE;
	}
}

void play_infinite_stairs_music(void)
{
	u8 shouldPlay = FALSE;

	/* Infinite stairs? */
	if(activeStageNo == LEVEL_CASTLE && activeSceneNo == 2 && marioWorks->numStars < 70)
	{
		if(marioWorks->floor != NULL && marioWorks->floor->room == 6)
		{
			if(marioWorks->pos[2] < 2540.0f)
			{
				shouldPlay = TRUE;
			}
		}
	}

	if(sPlayingInfiniteStairs ^ shouldPlay)
	{
		sPlayingInfiniteStairs = shouldPlay;
		if(shouldPlay)
		{
			play_secondary_music(SEQ_EVENT_ENDLESS_STAIRS, 0, 255, 1000);
		}
		else
		{
			func_80321080(500);
		}
	}
}

void AudPlayMusic(u16 a, u16 seqArgs, s16 fadeTimer)
{
	if(gResetTimer == 0 && seqArgs != sCurrentMusic)
	{
		if(snEndingScene != 0)
		{
			sound_reset(7);
		}
		else
		{
			sound_reset(a);
		}

		if(!(mesgCastle && seqArgs == SEQ_LEVEL_INSIDE_CASTLE))
		{
			Na_MusicStart(0, seqArgs, fadeTimer);
			sCurrentMusic = seqArgs;
		}
	}
}

void AudStopMusic(s16 fadeOutTime)
{
	func_803210D4(fadeOutTime);
	sCurrentMusic	   = MUSIC_NONE;
	sCurrentShellMusic = MUSIC_NONE;
	sCurrentCapMusic   = MUSIC_NONE;
}

void func_8024924C(s16 fadeTimer)
{
	Na_BossBgmStop(0, fadeTimer);
	sCurrentMusic	   = MUSIC_NONE;
	sCurrentShellMusic = MUSIC_NONE;
	sCurrentCapMusic   = MUSIC_NONE;
}

void play_cutscene_music(u16 seqArgs)
{
	Na_MusicStart(0, seqArgs, 0);
	sCurrentMusic = seqArgs;
}

void play_shell_music(void)
{
	Na_MusicStart(0, SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP | SEQ_VARIATION), 0);
	sCurrentShellMusic = SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP | SEQ_VARIATION);
}

void stop_shell_music(void)
{
	if(sCurrentShellMusic != MUSIC_NONE)
	{
		stop_background_music(sCurrentShellMusic);
		sCurrentShellMusic = MUSIC_NONE;
	}
}

void AudPlaySpecialMusic(u16 seqArgs)
{
	Na_MusicStart(0, seqArgs, 0);
	if(sCurrentCapMusic != MUSIC_NONE && sCurrentCapMusic != seqArgs)
	{
		stop_background_music(sCurrentCapMusic);
	}
	sCurrentCapMusic = seqArgs;
}

void fadeout_cap_music(void)
{
	if(sCurrentCapMusic != MUSIC_NONE)
	{
		fadeout_background_music(sCurrentCapMusic, 600);
	}
}

void stop_cap_music(void)
{
	if(sCurrentCapMusic != MUSIC_NONE)
	{
		stop_background_music(sCurrentCapMusic);
		sCurrentCapMusic = MUSIC_NONE;
	}
}

void play_menu_sounds_extra(s32 a, void* b)
{
	AudStartSound(menuSoundsExtra[a], (f32*)b);
}

void audio_game_loop_tick(void)
{
	audio_signal_game_loop_tick();
}

/**
 * Sound processing thread. Runs at 60 FPS.
 */
void thread4_sound(UNUSED void* arg)
{
	audio_init();
	sound_init();

	// Zero-out size vector
	vec3f_copy(unused80339DC0, gVec3fZero);

	osCreateMesgQueue(&sSoundMesgQueue, sSoundMesgBuf, ARRAY_COUNT(sSoundMesgBuf));

	while(TRUE)
	{
		OSMesg msg;

		osRecvMesg(&sSoundMesgQueue, &msg, OS_MESG_BLOCK);
		if(gResetTimer < 25)
		{
			SPTask* spTask;

			spTask = create_next_audio_frame_task();
			if(spTask != NULL)
			{
				dispatch_audio_sptask(spTask);
			}
		}
	}
}
