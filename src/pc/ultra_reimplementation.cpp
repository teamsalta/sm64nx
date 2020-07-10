#include <stdio.h>
#include <string.h>
#include "lib/src/libultra_internal.h"
#include "macros.h"

#ifdef TARGET_WEB
#include <emscripten.h>
#endif

#if defined(__SWITCH__) && !defined(_MSC_VER)
#include "nx.h"
#endif

extern OSMgrArgs piMgrArgs;

u64 osClockRate = 62500000;

s32 osPiStartDma(UNUSED OSIoMesg *mb, UNUSED s32 priority, UNUSED s32 direction,
                 uintptr_t devAddr, void *vAddr, size_t nbytes,
                 UNUSED OSMesgQueue *mq) {
    memcpy(vAddr, (const void *) devAddr, nbytes);
    return 0;
}

void osSetEventMesg(UNUSED OSEvent e, UNUSED OSMesgQueue *mq, UNUSED OSMesg msg) {
}
s32 osJamMesg(UNUSED OSMesgQueue *mq, UNUSED OSMesg msg, UNUSED s32 flag) {
    return 0;
}
s32 osSendMesg(UNUSED OSMesgQueue *mq, UNUSED OSMesg msg, UNUSED s32 flag) {
    return 0;
}
s32 osRecvMesg(UNUSED OSMesgQueue *mq, UNUSED OSMesg *msg, UNUSED s32 flag) {
    return 0;
}

uintptr_t osVirtualToPhysical(void *addr) {
    return (uintptr_t) addr;
}

void osCreateViManager(UNUSED OSPri pri) {
}
void osViSetMode(UNUSED OSViMode *mode) {
}
void osViSetEvent(UNUSED OSMesgQueue *mq, UNUSED OSMesg msg, UNUSED u32 retraceCount) {
}
void osViBlack(UNUSED u8 active) {
}
void osViSetSpecialFeatures(UNUSED u32 func) {
}
void osViSwapBuffer(UNUSED void *vaddr) {
}

OSTime osGetTime(void) {
    return 0;
}

void osWritebackDCacheAll(void) {
}

void osWritebackDCache(UNUSED void *a, UNUSED size_t b) {
}

void osInvalDCache(UNUSED void *a, UNUSED size_t b) {
}

u32 osGetCount(void) {
    static u32 counter;
    return counter++;
}

s32 osAiSetFrequency(u32 freq) {
    u32 a1;
    s32 a2;
    u32 D_8033491C;

#ifdef VERSION_EU
    D_8033491C = 0x02E6025C;
#else
    D_8033491C = 0x02E6D354;
#endif

    a1 = D_8033491C / (float) freq + .5f;

    if (a1 < 0x84) {
        return -1;
    }

    a2 = (a1 / 66) & 0xff;
    if (a2 > 16) {
        a2 = 16;
    }

    return D_8033491C / (s32) a1;
}

s32 osEepromProbe(UNUSED OSMesgQueue *mq) {
    return 1;
}



s32 osEepromLongRead(UNUSED OSMesgQueue *mq, u8 address, u8 *buffer, int nbytes) {
    u8 content[512];
    s32 ret = -1;

#if defined(__SWITCH__) && !defined(_MSC_VER) && !defined(BUILD_NRO)
	mountSaveData();

	FILE *fp = fopen("sv:/sm64.sav", "rb");
	if (fp == NULL)
	{
		fp = fopen("sdmc:/sm64.sav", "rb");

		if (fp == NULL)
		{
			unmountSaveData();
			return -1;
		}
	}

	if (fread(content, 1, 512, fp) == 512) {
		memcpy(buffer, content + address * 8, nbytes);
		ret = 0;
	}
	fclose(fp);
	unmountSaveData();
#else
    FILE *fp = fopen("sm64.sav", "rb");
    if (fp == NULL) {
        return -1;
    }
    if (fread(content, 1, 512, fp) == 512) {
        memcpy(buffer, content + address * 8, nbytes);
        ret = 0;
    }
    fclose(fp);
#endif
    return ret;
}

s32 osEepromLongWrite(UNUSED OSMesgQueue *mq, u8 address, u8 *buffer, int nbytes) {
    u8 content[512] = {0};
    if (address != 0 || nbytes != 512) {
        osEepromLongRead(mq, 0, content, 512);
    }
    memcpy(content + address * 8, buffer, nbytes);

#if defined(__SWITCH__) && !defined(_MSC_VER) && !defined(BUILD_NRO)
	mountSaveData();
	FILE *fp = fopen("sv:/sm64.sav", "wb");

	if (fp == NULL)
	{
		fp = fopen("sdmc:/sm64.sav", "wb");

		if (fp == NULL)
		{
			return -1;
		}
	}

    s32 ret = fwrite(content, 1, 512, fp) == 512 ? 0 : -1;
    fclose(fp);
	commitSave();
	unmountSaveData();
#else
	FILE *fp = fopen("sm64.sav", "wb");

	if (fp == NULL)
	{
		return -1;
	}

	s32 ret = fwrite(content, 1, 512, fp) == 512 ? 0 : -1;
	fclose(fp);
#endif
    return ret;
}

