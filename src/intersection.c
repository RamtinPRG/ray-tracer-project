#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "intersection.h"
#include "ray.h"
#include "sphere.h"

intersection_t intersection_create(double t, void *object)
{
    return (intersection_t){
        .t = t,
        .object = object,
    };
}

intersection_list_t *intersection_list(unsigned int count, ...)
{
    intersection_list_t *list = malloc(sizeof(intersection_list_t));
    list->count = count;
    list->list = malloc(sizeof(intersection_t) * count);

    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++)
    {
        list->list[i] = va_arg(args, intersection_t);
    }

    va_end(args);

    return list;
}

intersection_list_t *intersection_list_from_array(unsigned int count, intersection_t *items)
{
    if (!items || count == 0)
        return NULL;

    intersection_list_t *list = malloc(sizeof(intersection_list_t));
    list->count = count;
    list->list = malloc(sizeof(intersection_t) * count);
    memcpy(list->list, items, sizeof(intersection_t) * count);

    return list;
}

void intersection_list_del(intersection_list_t *list)
{
    if (list)
    {
        if (list->list)
            free(list->list);
        free(list);
    }
}

intersection_list_t *intersection_list_join(const intersection_list_t *a, const intersection_list_t *b)
{
    if (!a && !b)
        return NULL;
    if (!a)
        return intersection_list_from_array(b->count, b->list);
    if (!b)
        return intersection_list_from_array(a->count, a->list);

    unsigned int total = a->count + b->count;
    intersection_t *combined = malloc(sizeof(intersection_t) * total);
    if (!combined)
        return NULL;

    memcpy(combined, a->list, sizeof(intersection_t) * a->count);
    memcpy(combined + a->count, b->list, sizeof(intersection_t) * b->count);

    intersection_list_t *result = intersection_list_from_array(total, combined);
    free(combined);

    return result;
}

intersection_list_t *intersection_list_join_inplace(intersection_list_t *a, const intersection_list_t *b)
{
    if (!a && !b)
        return NULL;
    if (!a)
        return intersection_list_from_array(b->count, b->list);
    if (!b || b->count == 0)
        return a;

    intersection_t *new_list = realloc(a->list, sizeof(intersection_t) * (a->count + b->count));
    if (!new_list)
        return NULL;

    memcpy(new_list + a->count, b->list, sizeof(intersection_t) * b->count);
    a->list = new_list;
    a->count += b->count;

    return a;
}

intersection_t *intersection_hit(intersection_list_t *list)
{
    intersection_t *hit = NULL;
    for (int i = 0; i < list->count; i++)
    {
        if (list->list[i].t < 0)
            continue;
        if (!hit || list->list[i].t < hit->t)
            hit = &list->list[i];
    }
    return hit;
}

int compare_intersections(const void *a, const void *b)
{
    const intersection_t *ia = (const intersection_t *)a;
    const intersection_t *ib = (const intersection_t *)b;

    if (ia->t < ib->t)
        return -1;
    else if (ia->t > ib->t)
        return 1;
    else
        return 0;
}

void intersection_list_sort(intersection_list_t *list)
{
    if (!list || list->count <= 1)
        return;

    qsort(list->list, list->count, sizeof(intersection_t), compare_intersections);
}

intersection_precomp_t intersection_prepare_computations(intersection_t *i, ray_t r)
{
    intersection_precomp_t comp;
    comp.inside = false;
    comp.t = i->t;
    comp.object = i->object;
    comp.point = ray_position(&r, i->t);
    comp.eyev = tuple_neg(&r.direction);
    comp.normalv = sphere_normal_at(i->object, comp.point);
    if (tuple_vec_dot(&comp.normalv, &comp.eyev) < 0)
    {
        comp.inside = true;
        comp.normalv = tuple_neg(&comp.normalv);
    }
    return comp;
}