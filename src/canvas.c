#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "canvas.h"

typedef struct canvas_t canvas_t;

typedef struct color24_t
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color24_t;

typedef struct __attribute__((packed))
{
    uint16_t bfType;      // "BM"
    uint32_t bfSize;      // Size of the whole file
    uint16_t bfReserved1; // Must be 0
    uint16_t bfReserved2; // Must be 0
    uint32_t bfOffBits;   // Offset to pixel data
} BITMAPFILEHEADER;

typedef struct __attribute__((packed))
{
    uint32_t biSize;         // Size of this header (40 bytes)
    int32_t biWidth;         // Width in pixels
    int32_t biHeight;        // Height in pixels
    uint16_t biPlanes;       // Must be 1
    uint16_t biBitCount;     // Bits per pixel (24 or 32)
    uint32_t biCompression;  // Compression type (0 = none)
    uint32_t biSizeImage;    // Size of pixel data
    int32_t biXPelsPerMeter; // Horizontal resolution
    int32_t biYPelsPerMeter; // Vertical resolution
    uint32_t biClrUsed;      // Number of colors
    uint32_t biClrImportant; // Important colors
} BITMAPINFOHEADER;

struct canvas_t
{
    uint16_t width;
    uint16_t height;
    color_t *frame;
};

color_t color_create(double r, double g, double b)
{
    color_t new_color = {
        .r = r,
        .g = g,
        .b = b,
    };

    return new_color;
}

color_t color_add(color_t *c1, color_t *c2)
{
    return color_create(c1->r + c2->r, c1->g + c2->g, c1->b + c2->b);
}

color_t color_sub(color_t *c1, color_t *c2)
{
    return color_create(c1->r - c2->r, c1->g - c2->g, c1->b - c2->b);
}

color_t color_mul_scal(color_t *c1, double factor)
{
    return color_create(c1->r * factor, c1->g * factor, c1->b * factor);
}

color_t color_mul(color_t *c1, color_t *c2)
{
    return color_create(c1->r * c2->r, c1->g * c2->g, c1->b * c2->b);
}

void canvas_create(canvas_config_t *canvas_config, canvas_handle_t *canvas_handle)
{
    canvas_t *canvas = calloc(1, sizeof(canvas_t));

    canvas->width = canvas_config->width;
    canvas->height = canvas_config->height;
    canvas->frame = calloc(canvas_config->width * canvas_config->height, sizeof(color_t));

    *canvas_handle = canvas;
}

void canvas_write_pixel(canvas_handle_t canvas_handle, uint16_t x, uint16_t y, const color_t *color)
{
    uint16_t width = canvas_handle->width;
    uint16_t height = canvas_handle->height;
    color_t *frame = canvas_handle->frame;

    if (x < 0 || x >= width)
        return;
    if (y < 0 || y >= height)
        return;
    frame[y * width + x] = *color;
}

void canvas_del(canvas_handle_t canvas_handle)
{
    free(canvas_handle->frame);
    free(canvas_handle);
}

void canvas_color_to_color24(const color_t *from, color24_t *to, size_t nitem)
{
    for (int i = 0; i < nitem; i++)
    {
        to->r = (from->r > 1 ? 1 : from->r) * 255;
        to->g = (from->g > 1 ? 1 : from->g) * 255;
        to->b = (from->b > 1 ? 1 : from->b) * 255;
        from++;
        to++;
    }
}

void canvas_export_bmp(canvas_handle_t canvas_handle, char *path)
{
    FILE *file = fopen(path, "wb");
    if (!file)
        return;

    size_t pixels_count = canvas_handle->width * canvas_handle->height;
    size_t row_size = canvas_handle->width * sizeof(color24_t);
    size_t padding_size = (4 - (row_size % 4)) % 4;
    size_t stride_size = row_size + padding_size;
    size_t image_data_size = stride_size * canvas_handle->height;

    uint8_t *color_data = malloc(pixels_count * sizeof(color24_t));
    canvas_color_to_color24(canvas_handle->frame, (color24_t *)color_data, pixels_count);

    BITMAPFILEHEADER file_header = {
        .bfType = 0x4D42,
        .bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + image_data_size, // TODO
        .bfReserved1 = 0,
        .bfReserved2 = 0,
        .bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER),
    };
    BITMAPINFOHEADER info_header = {
        .biSize = sizeof(BITMAPINFOHEADER),
        .biWidth = canvas_handle->width,
        .biHeight = canvas_handle->height,
        .biPlanes = 1,
        .biBitCount = 24,
        .biCompression = 0,
        .biSizeImage = image_data_size,
        .biXPelsPerMeter = 1000, // Didn't think about it much; It won't break anything I guess?
        .biYPelsPerMeter = 1000,
        .biClrUsed = 0,
        .biClrImportant = 0,
    };
    fwrite(&file_header, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite(&info_header, sizeof(BITMAPINFOHEADER), 1, file);

    uint8_t padding_bytes[3] = {0, 0, 0};
    for (int i = canvas_handle->height - 1; i >= 0; i--)
    {
        const uint8_t *row = color_data + row_size * i;
        for (int j = 0; j < canvas_handle->width; j++)
        {
            uint8_t pixel[3] = {row[j * 3 + 2], row[j * 3 + 1], row[j * 3 + 0]};
            fwrite(pixel, 1, 3, file);
        }
        fwrite(padding_bytes, padding_size, 1, file);
    }

    fclose(file);
}

// int main()
// {
//     canvas_handle_t canvas_handle = NULL;
//     canvas_config_t cfg = {
//         .width = 5,
//         .height = 5,
//     };
//     canvas_create(&cfg, &canvas_handle);

//     color_t yellow = {
//         .r = 0.404,
//         .g = 0.161,
//         .b = 0.722,
//     };
//     // color_t black = {

//     // }
//     for (uint16_t i = 1; i < 3; i++)
//     {
//         for (uint16_t j = 1; j < 3; j++)
//         {
//             canvas_write_pixel(canvas_handle, i, j, &yellow);
//         }
//     }
//     canvas_export_bmp(canvas_handle, "./b.bmp");
//     canvas_del(canvas_handle);
// }