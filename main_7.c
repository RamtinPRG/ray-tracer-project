#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>
#include <time.h>
#include "world.h"
#include "camera.h"
#include "canvas.h"
#include "light.h"
#include "sphere.h"
#include "transform.h"
#include "material.h"

int main()
{
    matrix_t *transform, *scaling, *translation, *rotation_y, *rotation_x;

    sphere_t *floor = sphere_create();
    scaling = transform_scaling_matrix(10, 0.01, 10);
    sphere_set_transform(floor, scaling);
    matrix_del(scaling);
    material_t *floor_material = sphere_get_material(floor);
    floor_material->color = color_create(1, 0.9, 0.9);
    floor_material->specular = 0;

    sphere_t *left_wall = sphere_create();
    translation = transform_translation_matrix(0, 0, 5);
    rotation_y = transform_rotation_y_matrix(-M_PI_4);
    rotation_x = transform_rotation_x_matrix(M_PI_2);
    scaling = transform_scaling_matrix(10, 0.01, 10);
    transform = matrix_mul_multiple(4, translation, rotation_y, rotation_x, scaling);
    sphere_set_transform(left_wall, transform);
    matrix_del(translation);
    matrix_del(rotation_x);
    matrix_del(rotation_y);
    matrix_del(scaling);
    matrix_del(transform);
    material_t *left_wall_material = sphere_get_material(left_wall);
    *left_wall_material = *floor_material;

    sphere_t *right_wall = sphere_create();
    translation = transform_translation_matrix(0, 0, 5);
    rotation_y = transform_rotation_y_matrix(M_PI_4);
    rotation_x = transform_rotation_x_matrix(M_PI_2);
    scaling = transform_scaling_matrix(10, 0.01, 10);
    transform = matrix_mul_multiple(4, translation, rotation_y, rotation_x, scaling);
    sphere_set_transform(right_wall, transform);
    matrix_del(translation);
    matrix_del(rotation_x);
    matrix_del(rotation_y);
    matrix_del(scaling);
    matrix_del(transform);
    material_t *right_wall_material = sphere_get_material(right_wall);
    *right_wall_material = *floor_material;

    sphere_t *middle = sphere_create();
    translation = transform_translation_matrix(-0.5, 1, 0.5);
    transform = matrix_mul_multiple(1, translation);
    sphere_set_transform(middle, transform);
    matrix_del(translation);
    matrix_del(transform);
    material_t *middle_material = sphere_get_material(middle);
    middle_material->color = color_create(0.1, 1, 0.5);
    middle_material->diffuse = 0.7;
    middle_material->specular = 0.3;

    sphere_t *right = sphere_create();
    translation = transform_translation_matrix(1.5, 0.5, -0.5);
    scaling = transform_scaling_matrix(0.5, 0.5, 0.5);
    transform = matrix_mul_multiple(2, translation, scaling);
    sphere_set_transform(right, transform);
    matrix_del(translation);
    matrix_del(scaling);
    matrix_del(transform);
    material_t *right_material = sphere_get_material(right);
    right_material->color = color_create(0.5, 1, 0.1);
    right_material->diffuse = 0.7;
    right_material->specular = 0.3;

    sphere_t *left = sphere_create();
    translation = transform_translation_matrix(-1.5, 0.33, -0.75);
    scaling = transform_scaling_matrix(0.33, 0.33, 0.33);
    transform = matrix_mul_multiple(2, translation, scaling);
    sphere_set_transform(left, transform);
    matrix_del(translation);
    matrix_del(scaling);
    matrix_del(transform);
    material_t *left_material = sphere_get_material(left);
    left_material->color = color_create(1, 0.8, 0.1);
    left_material->diffuse = 0.7;
    left_material->specular = 0.3;

    point_light_t light_source_1 = light_create(tuple_create_point(-10, 10, -10), color_create(1, 1, 1));
    // point_light_t light_source_2 = light_create(tuple_create_point(0, 10, -10), color_create(1, 0, 0));

    world_config_t world_config = {
        .light_source_count = 2,
        .objects_count = 3,
    };
    world_t *world = world_create(&world_config);
    world_add_light_source(world, &light_source_1);
    // world_add_light_source(world, &light_source_2);
    world_add_object(world, floor);
    world_add_object(world, left_wall);
    world_add_object(world, right_wall);
    world_add_object(world, middle);
    world_add_object(world, right);
    world_add_object(world, left);

    camera_t camera = camera_create(800, 400, M_PI / 3);
    matrix_del(camera.transform);
    camera.transform = transform_view(
        tuple_create_point(0, 1.5, -5),
        tuple_create_point(0, 1, 0),
        tuple_create_vector(0, 1, 0));

    printf("\n******** Starting to render ********\n\n");
    clock_t start_time = clock();
    canvas_handle_t canvas_handle = camera_render(&camera, world);
    clock_t end_time = clock();
    printf("\n******** Rendering finished ********\n\n");
    printf("Render time: %.5f seconds\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    canvas_export_bmp(canvas_handle, "./chapter7.bmp");

    sphere_del(floor);
    sphere_del(left_wall);
    sphere_del(right_wall);
    sphere_del(middle);
    sphere_del(right);
    sphere_del(left);
    world_del(world);
    camera_del(&camera);
    canvas_del(canvas_handle);

    return 0;
}