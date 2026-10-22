#include <stdlib.h>
#include "world.h"
#include "light.h"
#include "sphere.h"
#include "intersection.h"

world_t *world_create(world_config_t *config)
{
    world_t *world = malloc(sizeof(world_t));
    world->max_light_source_count = config->light_source_count;
    world->max_objects_count = config->objects_count;
    world->light_source_count = 0;
    world->objects_count = 0;

    world->light_source = malloc(sizeof(point_light_t *) * config->light_source_count);
    world->objects = malloc(sizeof(sphere_t *) * config->objects_count);
    return world;
}

void world_del(world_t *world)
{
    if (world)
    {
        if (world->light_source)
            free(world->light_source);
        if (world->objects)
            free(world->objects);
        free(world);
    }
}

void world_add_light_source(world_t *world, point_light_t *light_source)
{
    world->light_source[world->light_source_count] = light_source;
    world->light_source_count++;
}

void world_add_object(world_t *world, sphere_t *object)
{
    world->objects[world->objects_count] = object;
    world->objects_count++;
}

intersection_list_t *world_ray_intersect(world_t *world, ray_t r)
{
    intersection_list_t *intersections = intersection_list(0);

    for (int i = 0; i < world->objects_count; i++)
    {
        intersection_list_t *object_intersections = sphere_ray_intersect(world->objects[i], r);
        intersection_list_t *updated_intersections = intersection_list_join_inplace(intersections, object_intersections);
        if (!updated_intersections)
        {
            intersection_list_del(object_intersections);
            intersection_list_del(intersections);
            return NULL;
        }
        intersections = updated_intersections;
        intersection_list_del(object_intersections);
    }

    intersection_list_sort(intersections);
    return intersections;
}

bool world_is_shadowed(world_t *world, point_t point, point_light_t *light)
{
    vector_t to_light = tuple_sub(&light->position, &point);
    double distance = tuple_vec_len(&to_light);
    if (distance == 0)
        return false;

    vector_t direction = tuple_div_scal(&to_light, distance);
    ray_t shadow_ray = ray_create(point, direction);
    intersection_list_t *xs = world_ray_intersect(world, shadow_ray);
    if (!xs)
        return false;

    intersection_t *hit = intersection_hit(xs);
    bool shadowed = hit && hit->t < distance;
    intersection_list_del(xs);
    return shadowed;
}

color_t world_shade_hit(world_t *world, intersection_precomp_t *comps)
{
    color_t result = color_create(0, 0, 0);
    for (int i = 0; i < world->light_source_count; i++)
    {
        point_light_t *light = world->light_source[i];
        bool in_shadow = world_is_shadowed(world, comps->over_point, light);
        color_t color = light_lighting(sphere_get_material((sphere_t *)comps->object), light,
                                       comps->over_point, comps->eyev, comps->normalv, in_shadow);
        result = color_add(&result, &color);
    }
    return result;
}

color_t world_color_at(world_t *world, ray_t r)
{
    intersection_list_t *xs = world_ray_intersect(world, r);
    if (!xs)
        return (color_t){0, 0, 0};

    color_t color = {0, 0, 0};
    intersection_t *hit = intersection_hit(xs);

    if (hit)
    {
        intersection_precomp_t comps =
            intersection_prepare_computations(hit, r);
        color = world_shade_hit(world, &comps);
    }

    intersection_list_del(xs);
    return color;
}
