#include <assert.h>
#include <math.h>
#include <string.h>

#include "linalg.h"

double dot(double *u, double *v, size_t n) {
  double res = 0.0;
  for (size_t i = 0; i < n; ++i) {
    res += u[i] * v[i];
  }
  return res;
}

void matmul(
  double *restrict C,
  double *A,
  double *B,
  size_t m,
  size_t n,
  size_t p) {
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < p; ++j) {
      double cij = 0.0;
      for (size_t k = 0; k < n; ++k) {
        cij += A[i * n + k] * B[k * p + j];
      }
      C[i * p + j] = cij;
    }
  }
}

void inplace_transpose(double *restrict A, size_t m, size_t n) {
  size_t mn = m * n;
  assert(mn >= m && mn >= n && "Overflow");
  assert(mn && mn * mn >= mn && "Overflow");

  /* TODO: Optimization reference to ppopp2014 */

  /* Note that A[0] and A[mn - 1] won't move */
  for (size_t i = 1; i < mn - 1; ++i) {
    size_t j = i;
    do {
      j = j * m % (mn - 1);
    } while (j > i);
    /* Cycle following */
    if (j == i) {
      size_t j = i;
      double t = A[i], u;
      do {
        j    = j * m % (mn - 1);
        u    = A[j];
        A[j] = t;
        t    = u;
      } while (j != i);
    }
  }
}

void transpose(double *restrict T, double *restrict A, size_t m, size_t n) {
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < n; ++j) {
      T[j * m + i] = A[i * n + j];
    }
  }
}

/*
 * Gram-Schimidt
 * */
void gram_schimidt(double *restrict A, size_t m, size_t n) {
  for (size_t i = 0; i < m; ++i) {
    double *restrict vi = A + i * n;
    for (size_t j = 0; j < i; ++j) {
      double *restrict vj = A + j * n;
      double prj          = dot(vi, vj, n) / dot(vj, vj, n);
      for (size_t k = 0; k < n; ++k) {
        vi[k] -= prj * vj[k];
      }
    }
    double rjj = sqrt(dot(vi, vi, n));
    if (rjj) {
      for (size_t k = 0; k < n; ++k) {
        vi[k] /= rjj;
      }
    }
  }
}

/*
 * The QR Factorization
 *
 * If A is an m*n matrix with linearly independent columns, then A can be
 * factored as A=QR, where Q is an m*n matrix whose columns form an orthonormal
 * basis for Col A and R is an n*n upper triangular invertible matrix with
 * positive entries on its diagonal.
 * */
void qr(
  double *restrict Q,
  double *restrict R,
  double *restrict A,
  size_t m,
  size_t n) {
  transpose(Q, A, m, n);
  /* Q.T */
  gram_schimidt(Q, n, m);
  matmul(R, Q, A, n, m, n);
  inplace_transpose(Q, n, m);
}
