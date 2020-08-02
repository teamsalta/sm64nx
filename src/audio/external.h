#ifndef AUDIO_EXTERNAL_H
#define AUDIO_EXTERNAL_H

#include "types.h"

// Sequence arguments, passed to Na_SeqStart. seqId may be bit-OR'ed with
// SEQ_VARIATION; this will load the same sequence, but set a variation
// bit which may be read by the sequence script.
#define SEQUENCE_ARGS(priority, seqId) ((priority << 8) | seqId)

#define SOUND_MODE_STEREO 0
#define SOUND_MODE_MONO 3
#define SOUND_MODE_HEADSET 1

extern s32 gAudioErrorFlags;
extern f32 gDefaultSoundArgs[3];

extern u8 gAudioSPTaskYieldBuffer[]; // ucode yield data ptr; only used in JP

struct SPTask* create_next_audio_frame_task(void);
void AudStartSound(s32 soundBits, f32* pos);
void audio_signal_game_loop_tick(void);
void Na_BossBgmStop(u8 player, u16 fadeTimer);
void Na_AllSeqSubVolumeTrans(u8 player, u8 targetScale, u16 fadeTimer);
void Na_SeqVolMute(u8 player, u16 fadeTimer, u8 arg2);
void Na_SeqVolRecover(u8 player, u16 fadeTimer);
void Na_PauseSet(u8 disabled);
void interface_init(void);
void Na_SeEnd(u32 soundBits, f32* vec);
void Na_ObjSeEnd(f32* arg0);
void Na_LevelSeAllStop(void);
void Na_PortLock(u8 player, u16 bankMask);
void Na_PortUnlock(u8 player, u16 bankMask);
void Na_SetObjSpeed(u8 bankIndex, u8 arg1);
void Na_MessageSound(u8 dialogID);
void Na_MusicStart(u8 player, u16 seqArgs, s16 fadeTimer);
void stop_background_music(u16 seqId);
void fadeout_background_music(u16 arg0, u16 fadeOut);
void Na_AllBgmStackFlagFree(void);
u16 Na_GetPlayingBgmFlag(void);
void Na_AddSeq(u8 seqId, u8 bgMusicVolume, u8 volume, u16 fadeTimer);
void Na_SubSeq(u16 fadeTimer);
void Na_SoundFadeOut(u16 fadeOutTime);
void Na_ClearFanfare(void);
void Na_NewDoorMessageBgm(void);
void Na_NazoClearBgm(void);
void Na_HiScoreBgm(void);
void Na_NewStarAppearBgm(u8 arg0);
void Na_RaceFanfareBgm(void);
void Na_kinopioAppearBgm(void);
void Na_StageChange(u8 arg0);
void Na_OutputMode(u8 arg0);

void audio_init(void); // in load.c

#endif /* AUDIO_EXTERNAL_H */
