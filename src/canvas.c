#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "canvas.h"

struct Color
{
    double r;
    double g;
    double b;
};

struct Canvas
{
    uint16_t width;
    uint16_t height;
    Color *frame;
};

Color color_create(double r, double g, double b)
{
    Color new_color = {
        .r = r,
        .g = g,
        .b = b,
    };

    return new_color;
}

Color color_add(Color *c1, Color *c2)
{
    return color_create(c1->r + c2->r, c1->g + c2->g, c1->b + c2->b);
}

Color color_sub(Color *c1, Color *c2)
{
    return color_create(c1->r - c2->r, c1->g - c2->g, c1->b - c2->b);
}

Color color_mul_scal(Color *c1, double factor)
{
    return color_create(c1->r * factor, c1->g * factor, c1->b * factor);
}

Color color_mul(Color *c1, Color *c2)
{
    return color_create(c1->r * c2->r, c1->g * c2->g, c1->b * c2->b);
}

Canvas canvas_create(uint16_t width, uint16_t height)
{
    Canvas new_canvas = {
        .width = width,
        .height = height,
        .frame = calloc(width * height, sizeof(Color)),
    };

    return new_canvas;
}

void canvas_write_pixel(Canvas *canvas, uint16_t x, uint16_t y, Color *color)
{
    uint16_t width = canvas->width;
    uint16_t height = canvas->height;
    Color *frame = canvas->frame;

    if (x < 0 || x >= width)
        return;
    if (y < 0 || y >= height)
        return;
    frame[y * width + x] = *color;
}