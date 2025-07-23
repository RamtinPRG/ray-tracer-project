#ifndef SPHERE_H_
#define SPHERE_H_

#include "ray.h"
#include "matrix.h"
#include "intersection.h"

typedef struct sphere_t sphere_t;

sphere_t *sphere_create();
void sphere_del(sphere_t *s);
void sphere_set_transform(sphere_t *s, matrix_t *m);
intersection_list_t *sphere_ray_intersect(sphere_t *s, ray_t r);

#endif