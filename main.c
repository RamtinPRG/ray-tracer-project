#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "canvas.h"
#include "intersection.h"
#include "matrix.h"
#include "ray.h"
#include "sphere.h"
#include "transform.h"
#include "tuple.h"

int main()
{
    ///////// Canvas setup ///////////
    int canvas_width = 320;
    int canvas_height = 240;

    canvas_handle_t canvas_handle;
    canvas_config_t canvas_cfg = {
        .width = canvas_width,
        .height = canvas_height,
    };
    canvas_create(&canvas_cfg, &canvas_handle);

    color_t red = {
        .r = 1,
        .g = 0,
        .b = 0,
    };
    ///////////////

    double wall_z = 10;
    double wall_height = 12;
    double pixel_size = wall_height / canvas_height;
    double wall_width = pixel_size * canvas_width;

    point_t ray_origin = tuple_create_point(0, 0, -5);
    sphere_t *s = sphere_create();
    matrix_t *translation = transform_translation_matrix(1.1, 1.1, 0);
    matrix_t *scaling = transform_scaling_matrix(2, 1, 1);
    matrix_t *rotation = transform_rotation_z_matrix(1); // rotate by 1 radian around the z axis
    matrix_t *transform = matrix_mul(rotation, scaling);
    sphere_set_transform(s, transform);
    matrix_del(translation);
    matrix_del(scaling);
    matrix_del(transform);
    matrix_del(rotation);

    for (int i = 0; i < canvas_width; i++)
    {
        double world_x = -wall_width / 2 + pixel_size * i;
        for (int j = 0; j < canvas_height; j++)
        {
            double world_y = wall_height / 2 - pixel_size * j;
            point_t wall_point = tuple_create_point(world_x, world_y, wall_z);
            vector_t ray_direction = tuple_sub(&wall_point, &ray_origin);
            ray_t ray = ray_create(ray_origin, tuple_vec_nor(&ray_direction));
            intersection_list_t *list = sphere_ray_intersect(s, ray);
            if (intersection_hit(list))
                canvas_write_pixel(canvas_handle, i, j, &red);
            intersection_list_del(list);
        }
    }

    canvas_export_bmp(canvas_handle, "./output1.bmp");

    sphere_del(s);
    canvas_del(canvas_handle);
    return 0;
}