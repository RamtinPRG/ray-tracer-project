#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "world.h"
#include "light.h"
#include "sphere.h"
#include "canvas.h"
#include "transform.h"
#include "camera.h"
#include "tuple.h"

int main()
{
    world_config_t world_config = {
        .light_source_count = 1,
        .objects_count = 2,
    };
    world_t *world = world_create(&world_config);

    point_light_t light_source = light_create(tuple_create_point(-10, 10, -10), color_create(1, 1, 1));

    sphere_t *s1 = sphere_create();
    material_t *s1_material = sphere_get_material(s1);
    s1_material->color = color_create(0.8, 1, 0.6);
    s1_material->diffuse = 0.7;
    s1_material->specular = 0.2;

    sphere_t *s2 = sphere_create();
    matrix_t *scaling = transform_scaling_matrix(0.5, 0.5, 0.5);
    sphere_set_transform(s2, scaling);
    matrix_del(scaling);

    world_add_light_source(world, &light_source);
    world_add_object(world, s1);
    world_add_object(world, s2);

    // Main code here

    /* Test 1 */
    // ray_t ray = ray_create(tuple_create_point(0, 0, -5), tuple_create_vector(0, 0, 1));
    // intersection_list_t *intersections = world_ray_intersect(world, ray);
    // printf("%d\n", intersections->count);
    // for (int i = 0; i < intersections->count; i++)
    // {
    //     printf("%10.5f\n", intersections->list[i].t);
    // }
    // intersection_list_del(intersections);

    /* Test 2 */
    // ray_t ray = ray_create(tuple_create_point(0, 0, -5), tuple_create_vector(0, 0, 1));
    // intersection_t i = intersection_create(4, s1);
    // intersection_precomp_t comps = intersection_prepare_computations(&i, ray);
    // color_t c = world_shade_hit(world, &comps);
    // color_print(&c);

    /* Test 3 */
    // light_source = light_create(tuple_create_point(0, 0.25, 0), color_create(1, 1, 1));
    // ray_t r = ray_create(tuple_create_point(0, 0, 0), tuple_create_vector(0, 0, 1));
    // intersection_t i = intersection_create(0.5, s2);
    // intersection_precomp_t comps = intersection_prepare_computations(&i, r);
    // color_t c = world_shade_hit(world, &comps);
    // color_print(&c);

    /* Test 4 */
    // ray_t r = ray_create(tuple_create_point(0, 0, -5), tuple_create_vector(0, 0, 1));
    // color_t c = world_color_at(world, r);
    // color_print(&c);

    /* Test 5 */
    // material_t *material = sphere_get_material(s1);
    // material->ambient = 1;
    // material = sphere_get_material(s2);
    // material->ambient = 1;
    // material->color = (color_t){0.4, 0.6, 0.7};
    // ray_t r = ray_create(tuple_create_point(0, 0, 0.75), tuple_create_vector(0, 0, -1));
    // color_t c = world_color_at(world, r);
    // color_print(&material->color);
    // color_print(&c);

    /* Test 6 */
    camera_t c = camera_create(11, 11, M_PI_2);
    point_t from = tuple_create_point(0, 0, -5);
    point_t to = tuple_create_point(0, 0, 0);
    vector_t up = tuple_create_vector(0, 1, 0);
    matrix_del(c.transform);
    c.transform = transform_view(from, to, up);
    canvas_handle_t canvas_handle = camera_render(&c, world);
    color_t color = canvas_pixel_at(canvas_handle, 5, 5);
    color_print(&color);
    canvas_del(canvas_handle);
    camera_del(&c);

    world_del(world);
    sphere_del(s1);
    sphere_del(s2);

    return 0;
}