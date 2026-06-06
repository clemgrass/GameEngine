#ifndef TRIG_APPROX_H
#define TRIG_APPROX_H
#include <stdint.h>

#define SIN_SIZE 1024

#define PI 3.14159265358979323846f
#define TWO_PI 6.28318530717958647692f

#define FP_SHIFT 16
#define FP_ONE (1 << FP_SHIFT)

#define TO_FP(x) ((int32_t)((x) * FP_ONE))
#define FROM_FP(x) ((float)(x) / FP_ONE)

void trig_init();

int32_t fast_sin_fp(int32_t x);
int32_t fast_cos_fp(int32_t x);

float sin_approx(float x);

#endif