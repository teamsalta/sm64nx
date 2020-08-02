#ifndef _MARIO_ACTIONS_CUTSCENE_H
#define _MARIO_ACTIONS_CUTSCENE_H

#include "types.h"

extern void DrawStaffRoll(void);
extern void DoPeachProcess(void);
extern void DoKinopioProcess(void);
extern s32 geo_switch_peach_eyes(s32 run, struct GraphNode* node, UNUSED s32 a2);
extern s32 mario_ready_to_speak(void);
extern s32 CtrlPlayerDialog(s32);

#endif /* _MARIO_ACTIONS_CUTSCENE_H */
