#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include <stdint.h>

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

intersection_t intersection_create(double t, void *object);
intersection_list_t *intersection_list(unsigned int count, ...);
void intersection_list_del(intersection_list_t *list);
intersection_t *intersection_hit(intersection_list_t *list);

#endif