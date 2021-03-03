#ifndef SA_H_
#define SA_H_

#include <stddef.h>

#define SA_SUCCESS 0
#define SA_FAILURE -1

#define SA_ARRAY_SIZE(n) ((size_t)(n) + 1)

#ifdef __cplusplus
extern "C" {
#endif

int sa_init(const char *str, size_t len, int sa[], int lcp[]);

#ifdef __cplusplus
}
#endif

#endif
