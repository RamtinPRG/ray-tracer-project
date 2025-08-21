#ifndef WORLD_H_
#define WORLD_H_

#include "light.h"
#include "sphere.h"

typedef struct world_t
{
    uint16_t max_light_source_count; //
    uint16_t light_source_count;     //
    point_light_t **light_source;    //
    uint16_t max_objects_count;      //
    uint16_t objects_count;          //
    sphere_t **objects;              // TODO: turn it into a generalized object type
} world_t;                           // TODO: it may need to be opaque

typedef struct world_config_t
{
    uint16_t light_source_count;
    uint16_t objects_count;
} world_config_t;

world_t *world_create(world_config_t *config);
void world_del(world_t *world);
void world_add_light_source(world_t *world, point_light_t *light_source);
void world_add_object(world_t *world, sphere_t *object);
intersection_list_t *world_ray_intersect(world_t *world, ray_t r);
color_t world_shade_hit(world_t *world, intersection_precomp_t *comps);
color_t world_color_at(world_t *world, ray_t r);

#endif