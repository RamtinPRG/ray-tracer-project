#ifndef CAMERA_H_
#define CAMERA_H_

#include <stdint.h>
#include "matrix.h"
#include "ray.h"
#include "transform.h"
#include "world.h"
#include "canvas.h"

typedef struct camera_t
{
    uint16_t hsize;       // horizontal size of the camera (in pixels)
    uint16_t vsize;       // vertical size of the camera (in pixels)
    double field_of_view; // angle of field of view (in radians)
    matrix_t *transform;
    double pxsize;
    double half_width;
    double half_height;
} camera_t; // TODO: might need to be opaque

camera_t camera_create(uint16_t hsize, uint16_t vsize, double field_of_view);
void camera_del(camera_t *camera);
ray_t camera_ray_for_pixel(camera_t *camera, uint16_t x, uint16_t y);
canvas_handle_t camera_render(camera_t *camera, world_t *world);

#endif