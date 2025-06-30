#ifndef CANVAS_H_
#define CANVAS_H_

typedef struct
{
    double r;
    double g;
    double b;
} color_t;
typedef struct canvas_t *canvas_handle_t;
typedef struct
{
    uint16_t width;
    uint16_t height;
} canvas_config_t;

color_t color_create(double r, double g, double b);
color_t color_add(color_t *c1, color_t *c2);
color_t color_sub(color_t *c1, color_t *c2);
color_t color_mul_scal(color_t *c1, double factor);
color_t color_mul(color_t *c1, color_t *c2);
void canvas_create(canvas_config_t *canvas_config, canvas_handle_t *canvas_handle);
void canvas_write_pixel(canvas_handle_t canvas_handle, uint16_t x, uint16_t y, color_t *color);
void canvas_del(canvas_handle_t canvas_handle);

#endif