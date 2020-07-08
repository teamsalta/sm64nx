#include <math.h>

#ifndef _MSC_VER
#ifndef __GNUC__
#pragma intrinsic(sqrtf)
#define __builtin_sqrtf sqrtf
#endif

float sqrtf(float f) {
    return __builtin_sqrtf(f);
}
#endif
