#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include <stdint.h>
#include <stdbool.h>
#include "tuple.h"
#include "ray.h"

typedef struct intersection_t
{
    double t;
    void *object;
} intersection_t;

typedef struct intersection_list_t
{
    uint8_t count;
    intersection_t *list;
} intersection_list_t;

typedef struct intersection_precomp_t
{
    double t;
    void *object;
    point_t point;
    vector_t eyev;
    vector_t normalv;
    bool inside;
} intersection_precomp_t;

intersection_t intersection_create(double t, void *object);
intersection_list_t *intersection_list(unsigned int count, ...);
intersection_list_t *intersection_list_from_array(unsigned int count, intersection_t *items);
void intersection_list_del(intersection_list_t *list);
intersection_list_t *intersection_list_join(const intersection_list_t *a, const intersection_list_t *b);
intersection_list_t *intersection_list_join_inplace(intersection_list_t *a, const intersection_list_t *b);
intersection_t *intersection_hit(intersection_list_t *list);
void intersection_list_sort(intersection_list_t *list);
intersection_precomp_t intersection_prepare_computations(intersection_t *i, ray_t r);

#endif