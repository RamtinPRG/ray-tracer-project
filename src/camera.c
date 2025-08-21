#include <math.h>
#include "camera.h"
#include "ray.h"
#include "canvas.h"
#include "world.h"

camera_t camera_create(uint16_t hsize, uint16_t vsize, double field_of_view)
{
    double pxsize;
    double half_width;
    double half_height;

    double half_view = tan(field_of_view / 2);
    double aspect_ratio = (double)hsize / (double)vsize;
    if (aspect_ratio >= 1)
    {
        half_width = half_view;
        half_height = half_view / aspect_ratio;
    }
    else
    {
        half_width = half_view * aspect_ratio;
        half_height = half_view;
    }
    pxsize = half_width * 2 / hsize;

    return (camera_t){
        .hsize = hsize,
        .vsize = vsize,
        .field_of_view = field_of_view,
        .transform = transform_identity_matrix(),
        .pxsize = pxsize,
        .half_width = half_width,
        .half_height = half_height,
    };
}

void camera_del(camera_t *camera)
{
    if (camera->transform)
        matrix_del(camera->transform);
}

ray_t camera_ray_for_pixel(camera_t *camera, uint16_t x, uint16_t y)
{
    double xoffset = (0.5 + x) * camera->pxsize;
    double yoffset = (0.5 + y) * camera->pxsize;
    double world_x = camera->half_width - xoffset;
    double world_y = camera->half_height - yoffset;

    matrix_t *inverse_transform = matrix_inverse(camera->transform);
    point_t pixel = matrix_mul_tup(inverse_transform, tuple_create_point(world_x, world_y, -1));
    point_t origin = matrix_mul_tup(inverse_transform, tuple_create_point(0, 0, 0));
    matrix_del(inverse_transform);

    vector_t direction = tuple_sub(&pixel, &origin);
    direction = tuple_vec_nor(&direction);

    return ray_create(origin, direction);
}

canvas_handle_t camera_render(camera_t *camera, world_t *world)
{
    canvas_handle_t canvas_handle = NULL;
    canvas_config_t canvas_config = {
        .width = camera->hsize,
        .height = camera->vsize,
    };
    canvas_create(&canvas_config, &canvas_handle);

    for (int y = 0; y < camera->vsize; y++)
    {
        for (int x = 0; x < camera->hsize; x++)
        {
            ray_t r = camera_ray_for_pixel(camera, x, y);
            color_t color = world_color_at(world, r);
            canvas_write_pixel(canvas_handle, x, y, &color);
        }
    }

    return canvas_handle;
}