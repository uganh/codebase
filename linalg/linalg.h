#ifndef LINALG_H
#define LINALG_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

double dot(double *u, double *v, size_t n);

void matmul(
  double *restrict C,
  double *A,
  double *B,
  size_t m,
  size_t n,
  size_t p);

void inplace_transpose(double *restrict A, size_t m, size_t n);

void transpose(double *restrict T, double *restrict A, size_t m, size_t n);

void gram_schimidt(double *restrict A, size_t m, size_t n);

void qr(
  double *restrict Q,
  double *restrict R,
  double *restrict A,
  size_t m,
  size_t n);

#ifdef __cplusplus
}
#endif

#endif