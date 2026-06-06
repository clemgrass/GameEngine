#include "BMPSPC.h"

long syscall(long number, ...);

#define SYS_open 2
#define SYS_write 1
#define SYS_close 3
#define O_WRONLY 1
#define O_CREAT 64
#define O_TRUNC 512

uint8_t pixel_buffer[WIDTH * HEIGHT * CHANNELS];

#pragma pack(push, 1)
typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} BMPHeader;

typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bpp;
    uint32_t compression;
    uint32_t image_size;
    int32_t xppm;
    int32_t yppm;
    uint32_t colors_used;
    uint32_t colors_important;
} DIBHeader;
#pragma pack(pop)

void put_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    if(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;

    int i = (y * WIDTH + x) * 3;

    pixel_buffer[i + 0] = b;
    pixel_buffer[i + 1] = g;
    pixel_buffer[i + 2] = r;
}

void clear(uint8_t r, uint8_t g, uint8_t b)
{
    for(int i = 0; i < WIDTH * HEIGHT; i++)
    {
        pixel_buffer[i*3+0] = b;
        pixel_buffer[i*3+1] = g;
        pixel_buffer[i*3+2] = r;
    }
}

void bmp_write(const char *filename)
{
    int fd = syscall(SYS_open, filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    int row_size = WIDTH * 3;
    int padding = (4 - (row_size % 4)) % 4;
    int image_size = (row_size + padding) * HEIGHT;

    BMPHeader bmp = {
        .type = 0x4D42,
        .size = 54 + image_size,
        .reserved1 = 0,
        .reserved2 = 0,
        .offset = 54
    };

    DIBHeader dib = {
        .size = 40,
        .width = WIDTH,
        .height = HEIGHT,
        .planes = 1,
        .bpp = 24,
        .compression = 0,
        .image_size = image_size,
        .xppm = 2835,
        .yppm = 2835,
        .colors_used = 0,
        .colors_important = 0
    };

    syscall(SYS_write, fd, &bmp, 14);
    syscall(SYS_write, fd, &dib, 40);

    uint8_t pad[3] = {0,0,0};

    for(int y = HEIGHT - 1; y >= 0; y--)
    {
        for(int x = 0; x < WIDTH; x++)
        {
            int i = (y * WIDTH + x) * 3;
            syscall(SYS_write, fd, &pixel_buffer[i], 3);
        }
        syscall(SYS_write, fd, pad, padding);
    }

    syscall(SYS_close, fd);
}