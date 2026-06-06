#include <stdint.h>
#include "BMPSPC.h"
#include "ArenaAllocation.h"
#include <time.h>
#include <stdio.h>
#include "TrigApprox.h"

//Largo y Ancho de la pantalla
#define WIDTH 400
#define HEIGHT 300
#define CHANNELS 3

// Arena Allocation
Arena arena;

double get_time_ms()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec * 1000.0 +
           ts.tv_nsec / 1000000.0;
}

void make_filename(char *out, int frame)
{
    char base[] = "frame_0000.bmp";

    base[6] = '0' + (frame / 1000) % 10;
    base[7] = '0' + (frame / 100) % 10;
    base[8] = '0' + (frame / 10) % 10;
    base[9] = '0' + (frame % 10);

    for(int i = 0; i < 15; i++)
        out[i] = base[i];

    out[15] = '\0';
}

void render_frame(int frame, int32_t *scratch)
{
    clear(0,0,0);

    int32_t t = frame * 64;
    int phase = frame & 1023;

    int cx = (fast_sin_fp(phase << 6) >> 8);
    int cy = (fast_sin_fp((phase << 6) + 2000) >> 8);

    for(int y = 0; y < HEIGHT; y++)
    {
        for(int x = 0; x < WIDTH; x++)
        {
            int idx = y * WIDTH + x;

            // ANTES: x << 10  →  x * 1024  (muchos ciclos)
            // AHORA: escala reducida para cubrir ~1 ciclo en toda la pantalla
            // SIN_SIZE=1024, queremos que x=400 recorra ~1 período
            // → factor ≈ SIN_SIZE / WIDTH * 64 ≈ 160
            //   usamos 192 (~400*192 / 64 ≈ 1.2 períodos en x)

            int32_t sx = x * 192/4;   // antes: x << 10 = x * 1024
            int32_t sy = y * 256/4;   // antes: y << 10 = y * 1024
                                     // HEIGHT=300, 300*256/64 ≈ 1.2 períodos en y

            /* --- EFFECT 1: gradiente suave --- */
            int32_t g =
                fast_sin_fp(sx + t) +
                fast_sin_fp(sy - (t >> 1));

            /* --- EFFECT 2: plasma suave --- */
            int32_t p =
                fast_sin_fp((sx + sy) + t) +
                fast_sin_fp(sx - t) +
                fast_sin_fp(sy + (t >> 1));

            p /= 3;

            scratch[idx] = g + p;

            /* --- EFFECT 3: círculo --- */
            int dx = x - cx;
            int dy = y - cy;
            int dist2 = dx*dx + dy*dy;
            int circle = (dist2 < 900);

            /* --- COMPOSICIÓN FINAL --- */
            int v = scratch[idx] >> 16;

            int r    = (v + 1) * 127;
            int gcol = (v + 1) * 127;
            int b    = 255 - r;

            if(circle)
            {
                r    = 255;
                gcol = 80;
                b    = 80;
            }

            put_pixel(x, y, r, gcol, b);
        }
    }
}

int main()
{
    arena = arena_create(1024*1024);
    trig_init();

    double total = 0;

    for(int frame = 0; frame < 60; frame++)
    {
        arena_reset(&arena);

        int32_t *scratch =
            (int32_t*)arena_alloc(
                &arena,
                WIDTH * HEIGHT * sizeof(int32_t),
                4
            );

        char name[32];
        make_filename(name, frame);

        double start = get_time_ms();

        render_frame(frame, scratch);
        bmp_write(name);

        double end = get_time_ms();

        total += (end - start);
    }

    double avg = total / 60.0;

    printf("AVG FRAME TIME: %f ms\n", avg);

    arena_destroy(&arena);

    return 0;
}








