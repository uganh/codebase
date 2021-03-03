#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "sa.h"

#define MAX(x, y) ((x) > (y) ? (x) : (y))

int sa_init(const char *str, size_t len, int sa[], int lcp[]) {
  int *mem, m = 256;

  if (!sa || !str || len > INT_MAX / 4) {
    return SA_FAILURE;
  }

  size_t mem_size = len * 2 + MAX(len, m);
  if ((mem = (int *)calloc(mem_size, sizeof(int))) == NULL) {
    return SA_FAILURE;
  }

  sa = sa + 1;

  int *x = mem;
  int *y = mem + len;
  int *c = mem + len * 2;

  for (int j = 0; j < m; ++j) {
    c[j] = 0;
  }
  for (int i = 0; i < len; ++i) {
    ++c[x[i] = (unsigned char)str[i]];
  }
  for (int j = 1; j < m; ++j) {
    c[j] += c[j - 1];
  }
  for (int i = (int)len - 1; i >= 0; --i) {
    sa[--c[x[i]]] = i;
  }

  for (size_t k = 1; k < len; k *= 2) {
    /* Sorting by 2nd part */
    int p = 0;
    for (int i = len - k; i < len; ++i) {
      y[p++] = i;
    }
    for (int i = 0; i < len; ++i) {
      if (sa[i] >= k) {
        y[p++] = sa[i] - k;
      }
    }
    /* Now, `x` holds rank and `y` holds partially ordered suffix offset */

    /* Sorting by 1st part */
    for (int j = 0; j < m; ++j) {
      c[j] = 0;
    }
    for (int i = 0; i < len; ++i) {
      ++c[x[y[i]]];
    }
    for (int j = 1; j < m; ++j) {
      c[j] += c[j - 1];
    }
    for (int i = (int)len - 1; i >= 0; --i) {
      sa[--c[x[y[i]]]] = y[i];
    }

    int *t = x;
    x      = y;
    y      = t;
    /* After exchange, `y` holds (old) rank */

    /* Update rank in `x` */
    x[sa[0]] = 0;
    m        = 1;
    for (int i = 1; i < (int)len; ++i) {
      if (
        y[sa[i - 1]] == y[sa[i]] && sa[i - 1] + k < len && sa[i] + k < len &&
        y[sa[i - 1] + k] == y[sa[i] + k]) {
        x[sa[i]] = m - 1;
      } else {
        x[sa[i]] = m++;
      }
    }
  }

  sa[-1] = len;

  if (lcp) {
    int h = 0;
    for (int i = 0; i < (int)len; ++i) {
      int j = sa[x[i] - 1];
      /* `i + h` cannot be out of bounds */
      while (j + h < len && str[i + h] == str[j + h]) {
        ++h;
      }
      lcp[x[i]] = h;
      if (h) {
        --h;
      }
    }
  }

  free(mem);

  return SA_SUCCESS;
}
