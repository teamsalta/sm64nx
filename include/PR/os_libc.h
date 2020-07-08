#ifndef _OS_LIBC_H_
#define _OS_LIBC_H_

#include "ultratypes.h"
#include <string.h>
// Old deprecated functions from strings.h, replaced by memcpy/memset.
//extern void bcopy(const void *, void *, size_t);
//extern void bzero(void *, size_t);
#define bcopy(b1,b2,len) (memmove((b2), (b1), (len)), (void) 0)
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

#endif /* !_OS_LIBC_H_ */
