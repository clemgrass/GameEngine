#ifndef BMPSPC_H
#define BMPSPC_H

#include <stdint.h>

#define WIDTH 400
#define HEIGHT 300
#define CHANNELS 3

extern uint8_t pixel_buffer[WIDTH * HEIGHT * CHANNELS];

void bmp_write(const char *filename);

void put_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
void clear(uint8_t r, uint8_t g, uint8_t b);

#endif