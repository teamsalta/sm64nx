#ifndef _SOUND_INIT_H
#define _SOUND_INIT_H

#include "types.h"

#define AUD_MUTE_PAUSE 1
#define AUD_MUTE_VIEWING 2

#define AUD_FACE_APPEAR 0x01
#define AUD_FACE_DISAPPEAR 0x02
#define AUD_FACE_SPARK 0x04
#define AUD_FACE_PINCH 0x08
#define AUD_FACE_PULL 0x10
#define AUD_FACE_REFORM 0x20
#define AUD_FACE_ZOOMUP 0x40
#define AUD_FACE_ZOOMDOWN 0x80
#define AUD_FACE_TWINKLE 0x100

#if 0
#define AUD_LSE1_SLIP 0
#define AUD_LSE1_SLIP_GRND 0
#define AUD_LSE1_SLIP_GRAS 1
#define AUD_LSE1_SLIP_WATRSIDE 2
#define AUD_LSE1_SLIP_CONC 3
#define AUD_LSE1_SLIP_FLOOR 4
#define AUD_LSE1_SLIP_SNOW 5
#define AUD_LSE1_SLIP_ICE 6
#define AUD_LSE1_SMOKE 7
#define AUD_LSE1_TREEDOWN 8
#define AUD_LSE1_POLEDOWN 9
#define AUD_LSE1_SANDSINK 10
#define AUD_LSE1_KOURA_SLIP 11
#endif

#define AUD_LSE2_BRIDGESLOPE 13
#define AUD_LSE2_LIFTROLLER 14
#define AUD_LSE2_ROCKROLL 15
#define AUD_LSE2_RIVER 16
#define AUD_LSE2_FALL 17
#define AUD_LSE2_MOVE 18
#define AUD_LSE2_MAGMA 19
#define AUD_LSE2_KIRAKIRA 20
#define AUD_LSE2_CONVEYOR 21
#define AUD_LSE2_SANDSTORM 22
#define AUD_LSE2_WATERTORNADO 23

#define AUD_LSE3_KUPAFIRE 24
#define AUD_LSE3_CANNONAIM 25
#define AUD_LSE3_JUGEMU_FLY 26
#define AUD_LSE3_SPARK 27
#define AUD_ENV_BIRD0 28
#define AUD_ENV_BIRD1 29
#define AUD_ENV_BIRD2 30
#define AUD_ENV_BIRD3 31

#define AUD_LSE2_STAND_APPEAR 32
#define AUD_LSE3_SNOWMAN_BREATH 33
#define AUD_LSE3_BURNER 34
#define AUD_LSE2_WOODLIFT 35

#define SOUND_MENU_MODE_STEREO 0
#define SOUND_MENU_MODE_MONO 1
#define SOUND_MENU_MODE_HEADSET 2

extern void AudResetMute(void);
extern void AudEndMute(s32);
extern void AudStartMute(s32);
extern void func_80248D48(void);
extern void AudUnlockSound(void);
extern void set_sound_mode(u16);
extern void play_menu_sounds(s16 a);
extern void AudPictWaveSound(void);
extern void play_infinite_stairs_music(void);
extern void AudPlayMusic(u16, u16, s16);
extern void AudStopMusic(s16);
extern void func_8024924C(s16 fadeTimer);
extern void play_cutscene_music(u16);
extern void play_shell_music(void);
extern void stop_shell_music(void);
extern void AudPlaySpecialMusic(u16);
extern void fadeout_cap_music(void);
extern void stop_cap_music(void);
extern void audio_game_loop_tick(void);
extern void thread4_sound(void*);

#endif /* _SOUND_INIT_H */
