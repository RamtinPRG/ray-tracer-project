#ifndef LIGHT_H_
#define LIGHT_H_

#include "canvas.h"
#include "tuple.h"
#include "material.h"
#include "light.h"

typedef struct point_light_t
{
    color_t intensity;
    point_t position;
} point_light_t;

point_light_t light_create(point_t position, color_t intensity);
color_t light_lighting(material_t *material, point_light_t *light, point_t point, vector_t eye_vec, vector_t normal_vec);

#endif