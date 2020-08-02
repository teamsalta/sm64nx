#ifndef _GAME_H_
#define _GAME_H_

#include "memory.h"

struct DemoInput
{
	u8 timer; // time until next input. if this value is 0, it means the demo is over
	s8 rawStickX;
	s8 rawStickY;
	u8 buttonMask;
};

// extern OSMesgQueue gGameVblankQueue;
extern OSMesgQueue D_80339CB8;
extern OSMesg D_80339CD0;
extern OSMesg D_80339CD4;
extern struct VblankHandler gGameVblankHandler;
extern uintptr_t gPhysicalFrameBuffers[3];
extern uintptr_t gPhysicalZBuffer;
extern void* D_80339CF0;
extern void* D_80339CF4;
extern struct SPTask* gGfxSPTask;
extern Gfx* gDisplayListHead;
extern u8* gGfxPoolEnd;
extern struct GfxPool* gGfxPool;

extern void (*D_8032C6A0)(void);
extern struct DemoInput* autoDemoPtr;
extern u16 gDemoInputListID;
extern struct DemoInput gRecordedDemoInput;

extern void setup_game_memory(void);
extern void game_init(void*);
extern u16 func_802495B0(u32);

// this area is the demo input + the header. when the demo is loaded in, there is a header the size
// of a single word next to the input list. this word is the current ID count.
extern struct MarioAnimation playerAnimation;
extern struct MarioAnimation gDemo;

// extern const void* marioAnims();
const void* demoInputs();

#endif
