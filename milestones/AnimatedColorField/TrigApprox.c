#include "TrigApprox.h"

static int32_t sin_table[SIN_SIZE];


static float wrap_pi(float x)
{
    while(x > PI)  x -= TWO_PI;
    while(x < -PI) x += TWO_PI;
    return x;
}

float sin_approx(float x)
{
    x = wrap_pi(x);

    float x2 = x * x;

    return x - (x * x2) / 6.0f;
}

void trig_init()
{
    for(int i = 0; i < SIN_SIZE; i++)
    {
        float t = (float)i / (float)SIN_SIZE;
        float angle = t * TWO_PI - PI;

        sin_table[i] =
            (int32_t)(sin_approx(angle) * FP_ONE);
    }
}

int32_t fast_sin_fp(int32_t x)
{
    int i = (x >> 6) & (SIN_SIZE - 1);
    return sin_table[i];
}

int32_t fast_cos_fp(int32_t x)
{
    return fast_sin_fp(x + (PI * FP_ONE / 2));
}