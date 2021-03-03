#include <assert.h>
#include <stddef.h>
#include <string.h>

void kmp_init(const char *P, size_t pi[], size_t m) {
  if (!m) {
    return;
  }

  size_t k = 0;
  pi[0]    = 0;
  for (size_t i = 1; i < m; ++i) {
    while (k > 0 && P[k] != P[i]) {
      k = pi[k - 1];
    }
    if (P[k] == P[i]) {
      ++k;
    }
    pi[i] = k;
  }
}

char *
kmp_find(const char *T, size_t n, const char *P, const size_t pi[], size_t m) {
  if (m == 0) {
    return (char *)T;
  }

  size_t q = 0;
  for (size_t i = 0; i < n; ++i) {
    while (q > 0 && P[q] != T[i]) {
      q = pi[q - 1];
    }
    if (P[q] == T[i]) {
      ++q;
    }
    if (q == m) {
      return (char *)T + i - m + 1;
    }
  }

  return NULL;
}

#include <stdio.h>

int main(void) {
  char P[] = "abba";
  char *T  = "abaabababba";
  size_t pi[sizeof(P) - 1];

  size_t n = strlen(T);
  size_t m = sizeof(P) - 1;

  kmp_init(P, pi, m);
  const char *ptr = kmp_find(T, n, P, pi, m);
  assert(ptr && ptr - T == 7);

  return 0;
}