#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "linalg.h"

void matrix_print(FILE *fp, double *A, size_t m, size_t n) {
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < n; ++j) {
      fprintf(fp, "%8.2f%c", A[i * n + j], (j + 1 == n ? '\n' : ' '));
    }
  }
}

int main(void) {
  double A[][3] = {{1, 0, 0}, {1, 1, 0}, {1, 1, 1}, {1, 1, 1}};
  double Q[12], R[9];
  qr(Q, R, A[0], 4, 3);
  matrix_print(stdout, Q, 4, 3);
  printf("\n");
  matrix_print(stdout, R, 3, 3);

  memcpy(Q, A[0], sizeof(Q));
  inplace_transpose(Q, 4, 3);
  for (size_t i = 0; i < 4; ++i) {
    for (size_t j = 0; j < 3; ++j) {
      assert(A[i][j] == Q[j * 4 + i]);
    }
  }

  return 0;
}
