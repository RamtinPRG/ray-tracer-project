#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "intersection.h"
#include "light.h"
#include "material.h"
#include "sphere.h"
#include "transform.h"
#include "world.h"

/* Unlike assert(), these checks also run when CMake builds with NDEBUG. */
#define CHECK(condition)                                                     \
    do                                                                       \
    {                                                                        \
        if (!(condition))                                                    \
        {                                                                    \
            fprintf(stderr, "%s:%d: failed: %s\n", __FILE__, __LINE__,         \
                    #condition);                                             \
            exit(EXIT_FAILURE);                                              \
        }                                                                    \
    } while (0)

static void check_color(color_t actual, color_t expected)
{
    CHECK(fabs(actual.r - expected.r) < 0.00001);
    CHECK(fabs(actual.g - expected.g) < 0.00001);
    CHECK(fabs(actual.b - expected.b) < 0.00001);
}

static void test_lighting_in_shadow(void)
{
    material_t material = material_create();
    point_t position = tuple_create_point(0, 0, 0);
    vector_t eyev = tuple_create_vector(0, 0, -1);
    vector_t normalv = tuple_create_vector(0, 0, -1);
    point_light_t light = light_create(tuple_create_point(0, 0, -10),
                                       color_create(1, 1, 1));

    color_t result = light_lighting(&material, &light, position, eyev, normalv, true);
    check_color(result, color_create(0.1, 0.1, 0.1));
    puts("PASS: lighting with the surface in shadow");
}

static void test_shadow_visibility(void)
{
    world_config_t config = {.light_source_count = 1, .objects_count = 2};
    world_t *world = world_create(&config);
    point_light_t light = light_create(tuple_create_point(-10, 10, -10),
                                       color_create(1, 1, 1));
    sphere_t *outer = sphere_create();
    material_t *material = sphere_get_material(outer);
    material->color = color_create(0.8, 1, 0.6);
    material->diffuse = 0.7;
    material->specular = 0.2;
    sphere_t *inner = sphere_create();
    matrix_t *scaling = transform_scaling_matrix(0.5, 0.5, 0.5);
    sphere_set_transform(inner, scaling);
    matrix_del(scaling);
    world_add_light_source(world, &light);
    world_add_object(world, outer);
    world_add_object(world, inner);

    CHECK(!world_is_shadowed(world, tuple_create_point(0, 10, 0), &light));
    puts("PASS: nothing is collinear with the point and light");

    CHECK(world_is_shadowed(world, tuple_create_point(10, -10, 10), &light));
    puts("PASS: an object is between the point and light");

    CHECK(!world_is_shadowed(world, tuple_create_point(-20, 20, -20), &light));
    puts("PASS: an object is behind the light");

    CHECK(!world_is_shadowed(world, tuple_create_point(-2, 2, -2), &light));
    puts("PASS: an object is behind the point");

    world_del(world);
    sphere_del(outer);
    sphere_del(inner);
}

static void test_shade_hit_in_shadow(void)
{
    world_config_t config = {.light_source_count = 1, .objects_count = 2};
    world_t *world = world_create(&config);
    point_light_t light = light_create(tuple_create_point(0, 0, -10),
                                       color_create(1, 1, 1));
    sphere_t *first = sphere_create();
    sphere_t *second = sphere_create();
    matrix_t *translation = transform_translation_matrix(0, 0, 10);
    sphere_set_transform(second, translation);
    matrix_del(translation);
    world_add_light_source(world, &light);
    world_add_object(world, first);
    world_add_object(world, second);

    ray_t ray = ray_create(tuple_create_point(0, 0, 5), tuple_create_vector(0, 0, 1));
    intersection_t hit = intersection_create(4, second);
    intersection_precomp_t comps = intersection_prepare_computations(&hit, ray);
    check_color(world_shade_hit(world, &comps), color_create(0.1, 0.1, 0.1));
    puts("PASS: shade_hit receives an intersection in shadow");

    world_del(world);
    sphere_del(first);
    sphere_del(second);
}

static void test_hit_offsets_point(void)
{
    ray_t ray = ray_create(tuple_create_point(0, 0, -5), tuple_create_vector(0, 0, 1));
    sphere_t *shape = sphere_create();
    matrix_t *translation = transform_translation_matrix(0, 0, 1);
    sphere_set_transform(shape, translation);
    matrix_del(translation);
    intersection_t hit = intersection_create(5, shape);
    intersection_precomp_t comps = intersection_prepare_computations(&hit, ray);

    CHECK(comps.over_point.z < -INTERSECTION_EPSILON / 2);
    CHECK(comps.point.z > comps.over_point.z);
    puts("PASS: the hit offsets the point above the surface");
    sphere_del(shape);
}

int main(void)
{
    test_lighting_in_shadow();
    test_shadow_visibility();
    test_shade_hit_in_shadow();
    test_hit_offsets_point();
    puts("All seven Chapter 8 scenarios passed.");
    return EXIT_SUCCESS;
}
