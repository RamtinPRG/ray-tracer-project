#ifndef CANVAS_H_
#define CANVAS_H_

typedef struct Color Color;
typedef struct Canvas Canvas;

Color color_create(double r, double g, double b);
Color color_add(Color *c1, Color *c2);
Color color_sub(Color *c1, Color *c2);
Color color_mul_scal(Color *c1, double factor);
Color color_mul(Color *c1, Color *c2);
Canvas canvas_create(uint16_t width, uint16_t height);
void canvas_write_pixel(Canvas *canvas, uint16_t x, uint16_t y, Color *color);

#endif