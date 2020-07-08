#ifndef _ULTRA64_LIBAUDIO_H_
#define _ULTRA64_LIBAUDIO_H_

#include "abi.h"

typedef struct
{
    u8 *offset;
    s32 len;
} ALSeqData;

typedef struct
{
    s16 revision;
    s16 seqCount;
    ALSeqData seqArray[1];
} ALSeqFile;

#ifdef __cplusplus
extern "C" {
#endif

	void alSeqFileNew(ALSeqFile *f, u8 *base);

#ifdef __cplusplus
}
#endif

#endif
