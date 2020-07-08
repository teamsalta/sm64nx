#ifndef _ULTRA64_OS_MISC_H_
#define _ULTRA64_OS_MISC_H_

/* Miscellaneous OS functions */

#ifdef __cplusplus
extern "C" {
#endif
	u32 osGetCount(void);

	uintptr_t osVirtualToPhysical(void *);

#ifdef __cplusplus
}
#endif

#endif
