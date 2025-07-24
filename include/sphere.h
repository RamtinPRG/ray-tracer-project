#ifndef SPHERE_H_
#define SPHERE_H_

#include "ray.h"
#include "matrix.h"
#include "intersection.h"
#include "material.h"

typedef struct sphere_t sphere_t;

sphere_t *sphere_create();
void sphere_del(sphere_t *s);
void sphere_set_transform(sphere_t *s, matrix_t *m);
material_t *sphere_get_material(sphere_t *s);
intersection_list_t *sphere_ray_intersect(sphere_t *s, ray_t r);
vector_t sphere_normal_at(sphere_t *s, point_t p);

#endif