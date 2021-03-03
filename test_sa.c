#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "sa.h"

int main(void) {
  const char str[] = "abracadabra";
  int sa[sizeof(str)], lcp[sizeof(str) - 1];
  size_t len = strlen(str);

  assert(SA_SUCCESS == sa_init(str, len, sa, lcp));

  for (unsigned i = 0; i <= len; ++i) {
    if (i < len) {
      printf("%-2d: %s\n", lcp[i], str + sa[i]);
    } else {
      printf("--: %s\n", str + sa[i]);
    }
  }

  return 0;
}