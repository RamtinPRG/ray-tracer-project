#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "canvas.h"

typedef struct canvas_t canvas_t;

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

void canvas_write_pixel(canvas_handle_t canvas_handle, uint16_t x, uint16_t y, color_t *color)
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