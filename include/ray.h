#ifndef RAY_H_
#define RAY_H_

#include "tuple.h"
#include "matrix.h"

typedef struct ray_t
{
    point_t origin;
    vector_t direction;
} ray_t;

ray_t ray_create(point_t origin, vector_t direction);
point_t ray_position(ray_t *ray, double t);
ray_t ray_transform(ray_t *ray, matrix_t *m);

#endif