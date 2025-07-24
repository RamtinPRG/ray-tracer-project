#define _USE_MATH_DEFINES

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "sphere.h"
#include "matrix.h"
#include "transform.h"
#include "intersection.h"
#include "ray.h"
#include "material.h"
#include "light.h"

struct sphere_t
{
    matrix_t *transform;
    material_t material;
};

sphere_t *sphere_create()
{
    sphere_t *sphere = malloc(sizeof(sphere_t));
    sphere->transform = transform_identity_matrix();
    sphere->material = material_create();
    return sphere;
}

void sphere_del(sphere_t *s)
{
    if (s)
    {
        if (s->transform)
            free(s->transform);
        free(s);
    }
}

void sphere_set_transform(sphere_t *s, matrix_t *m)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            matrix_set(s->transform, i, j, matrix_get(m, i, j));
        }
    }
}

material_t *sphere_get_material(sphere_t *s)
{
    return &s->material;
}

intersection_list_t *sphere_ray_intersect(sphere_t *s, ray_t r)
{
    matrix_t *transform_inverse = matrix_inverse(s->transform);
    r = ray_transform(&r, transform_inverse);
    matrix_del(transform_inverse);

    point_t origin = tuple_create_point(0, 0, 0);
    vector_t sphere_ray_vec = tuple_sub(&r.origin, &origin);

    double a = tuple_vec_dot(&r.direction, &r.direction);
    double b = 2 * tuple_vec_dot(&r.direction, &sphere_ray_vec);
    double c = tuple_vec_dot(&sphere_ray_vec, &sphere_ray_vec) - 1;
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return intersection_list(0);

    double t1 = (-b - sqrt(discriminant)) / (2 * a);
    double t2 = (-b + sqrt(discriminant)) / (2 * a);

    intersection_t i1 = intersection_create(t1, s);
    intersection_t i2 = intersection_create(t2, s);

    return intersection_list(2, i1, i2);
}

vector_t sphere_normal_at(sphere_t *s, point_t p)
{
    matrix_t *inverse = matrix_inverse(s->transform);
    matrix_t *inverse_transpose = matrix_transpose(inverse);

    point_t converted_p = matrix_mul_tup(inverse, p);
    vector_t normal_vec = {
        .x = converted_p.x,
        .y = converted_p.y,
        .z = converted_p.z,
        .w = 0,
    };
    normal_vec = matrix_mul_tup(inverse_transpose, normal_vec);
    normal_vec.w = 0;

    matrix_del(inverse);
    matrix_del(inverse_transpose);

    return tuple_vec_nor(&normal_vec);
}

// int main()
// {
//     // ray_t r = ray_create((point_t){0, 0, -5, 1}, (vector_t){0, 0, 1, 0});
//     // sphere_t *s = sphere_create();
//     // matrix_t *scaling = transform_translation_matrix(5, 0, 0);
//     // sphere_set_transform(s, scaling);
//     // matrix_del(scaling);
//     // intersection_list_t *list = sphere_ray_intersect(s, r);

//     // for (int i = 0; i < list->count; i++)
//     //     printf("%10.5f\n", list->list[i].t);

//     // sphere_t *s = sphere_create();
//     // intersection_t i1 = intersection_create(5, s);
//     // intersection_t i2 = intersection_create(7, s);
//     // intersection_t i3 = intersection_create(-3, s);
//     // intersection_t i4 = intersection_create(2, s);
//     // intersection_list_t *list = intersection_list(4, i1, i2, i3, i4);
//     // intersection_t *i = intersection_hit(list);
//     // printf("%p\n", i);
//     // printf("%f\n%d\n%d\n", i->t, i->t == i4.t, i->object == i4.object);

//     // ray_t r = ray_create(tuple_create_point(1, 2, 3), tuple_create_vector(0, 1, 0));
//     // matrix_t *translation = transform_scaling_matrix(2, 3, 4);
//     // ray_t r2 = ray_transform(&r, translation);
//     // tuple_print(&r2.origin);
//     // tuple_print(&r2.direction);

//     // sphere_t *sphere = sphere_create();
//     // matrix_t *translation = transform_translation_matrix(2, 3, 4);
//     // sphere_set_transform(sphere, translation);
//     // matrix_print(translation);
//     // matrix_print(sphere->transform);
//     // sphere_del(sphere);
//     // matrix_del(translation);

//     /////////// normal vector tests ///////////
//     // sphere_t *s = sphere_create();
//     // vector_t n = sphere_normal_at(s, tuple_create_point(1, 0, 0));
//     // tuple_print(&n);

//     // n = sphere_normal_at(s, tuple_create_point(0, 1, 0));
//     // tuple_print(&n);

//     // n = sphere_normal_at(s, tuple_create_point(0, 0, 1));
//     // tuple_print(&n);

//     // n = sphere_normal_at(s, tuple_create_point(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3));
//     // tuple_print(&n);

//     // matrix_t *translation = transform_translation_matrix(0, 1, 0);
//     // sphere_set_transform(s, translation);
//     // n = sphere_normal_at(s, tuple_create_point(0, 1.70711, -0.70711));
//     // tuple_print(&n);
//     // matrix_del(translation);

//     // matrix_t *scaling = transform_scaling_matrix(1, 0.5, 1);
//     // matrix_t *rotation = transform_rotation_z_matrix(M_PI / 5);
//     // matrix_t *transform = matrix_mul(scaling, rotation);
//     // sphere_set_transform(s, transform);
//     // n = sphere_normal_at(s, tuple_create_point(0, M_SQRT1_2, -M_SQRT1_2));
//     // tuple_print(&n);
//     // matrix_del(scaling);
//     // matrix_del(rotation);
//     // matrix_del(transform);

//     // sphere_del(s);

//     // vector_t v = tuple_create_vector(0, -1, 0);
//     // n = tuple_create_vector(M_SQRT1_2, M_SQRT1_2, 0);
//     // vector_t r = tuple_vec_reflect(&v, &n);
//     // tuple_print(&r);

//     //////////// lighting tests ////////////
//     material_t m = material_create();
//     point_t position = tuple_create_point(0, 0, 0);
//     vector_t eyev;
//     vector_t normalv;
//     point_light_t light;
//     color_t result;

//     eyev = tuple_create_vector(0, 0, -1);
//     normalv = tuple_create_vector(0, 0, -1);
//     light = light_create(tuple_create_point(0, 0, -10), color_create(1, 1, 1));
//     result = light_lighting(&m, &light, position, eyev, normalv);
//     printf("%10.5f, %10.5f, %10.5f\n", result.r, result.g, result.b);

//     eyev = tuple_create_vector(0, M_SQRT1_2, -M_SQRT1_2);
//     normalv = tuple_create_vector(0, 0, -1);
//     light = light_create(tuple_create_point(0, 0, -10), color_create(1, 1, 1));
//     result = light_lighting(&m, &light, position, eyev, normalv);
//     printf("%10.5f, %10.5f, %10.5f\n", result.r, result.g, result.b);

//     eyev = tuple_create_vector(0, 0, -1);
//     normalv = tuple_create_vector(0, 0, -1);
//     light = light_create(tuple_create_point(0, 10, -10), color_create(1, 1, 1));
//     result = light_lighting(&m, &light, position, eyev, normalv);
//     printf("%10.5f, %10.5f, %10.5f\n", result.r, result.g, result.b);

//     eyev = tuple_create_vector(0, -M_SQRT1_2, -M_SQRT1_2);
//     normalv = tuple_create_vector(0, 0, -1);
//     light = light_create(tuple_create_point(0, 10, -10), color_create(1, 1, 1));
//     result = light_lighting(&m, &light, position, eyev, normalv);
//     printf("%10.5f, %10.5f, %10.5f\n", result.r, result.g, result.b);

//     eyev = tuple_create_vector(0, 0, -1);
//     normalv = tuple_create_vector(0, 0, -1);
//     light = light_create(tuple_create_point(0, 0, 10), color_create(1, 1, 1));
//     result = light_lighting(&m, &light, position, eyev, normalv);
//     printf("%10.5f, %10.5f, %10.5f\n", result.r, result.g, result.b);
// }