#include "libultra_internal.h"
#include "hardware.h"

extern u32 osTvType;

OSViContext D_803348B0[2] = { 0 };
OSViContext *D_80334910 = &D_803348B0[0];
OSViContext *D_80334914 = &D_803348B0[1];
u32 D_80334918 = TV_TYPE_NTSC;
u32 D_8033491C = 0x02E6D354;

extern OSViMode D_80334990;
extern OSViMode D_803349E0;
void __osViInit(void) {
//#ifdef VERSION_JP
    D_80334918 = osTvType;
    bzero(D_803348B0, sizeof(D_803348B0));
    D_80334910 = &D_803348B0[0];
    D_80334914 = &D_803348B0[1];
    D_80334914->retraceCount = 1;
    D_80334910->retraceCount = 1;
    if (D_80334918 == TV_TYPE_NTSC)
    {
        D_80334914->unk08 = &D_80334990;
        D_8033491C = 0x02E6D354;
    } else {
        D_80334914->unk08 = &D_803349E0;
        D_8033491C = 0x02E6025C;
    }
    D_80334914->unk00 = 32;
    D_80334914->features = D_80334914->unk08->comRegs.ctrl;
    while (HW_REG(VI_CURRENT_REG, u32) > 0xa) {
        ;
    }
    HW_REG(VI_STATUS_REG, u32) = 0;
    __osViSwapContext();
}
