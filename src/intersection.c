#include <stdarg.h>
#include <stdlib.h>
#include "intersection.h"

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

void intersection_list_del(intersection_list_t *list)
{
    if (list)
    {
        if (list->list)
            free(list->list);
        free(list);
    }
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