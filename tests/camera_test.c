#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>
#include "camera.h"
#include "ray.h"
#include "tuple.h"

int main()
{
    /* Test 1 */
    // camera_t c = camera_create(160, 120, M_PI_2);
    // printf("%d\n%d\n%10.5f\n", c.hsize, c.vsize, c.field_of_view);
    // matrix_print(c.transform);
    // camera_del(&c);

    /* Test 2 */
    // camera_t c = camera_create(200, 125, M_PI_2);
    // printf("%10.5f %10.5f %10.5f\n", c.half_width, c.half_height, c.pxsize);
    // camera_del(&c);
    // camera_t c1 = camera_create(125, 200, M_PI_2);
    // printf("%10.5f %10.5f %10.5f\n", c1.half_width, c1.half_height, c1.pxsize);
    // camera_del(&c1);

    /* Test 3 */
    // camera_t c = camera_create(201, 101, M_PI_2);
    // ray_t r = camera_ray_for_pixel(&c, 100, 50);
    // tuple_print(&r.origin);
    // tuple_print(&r.direction);
    // camera_del(&c);

    /* Test 4 */
    // camera_t c = camera_create(201, 101, M_PI_2);
    // ray_t r = camera_ray_for_pixel(&c, 0, 0);
    // tuple_print(&r.origin);
    // tuple_print(&r.direction);
    // camera_del(&c);

    /* Test 5 */
    // camera_t c = camera_create(201, 101, M_PI_2);

    // matrix_t *rotation = transform_rotation_y_matrix(M_PI_4);
    // matrix_t *translation = transform_translation_matrix(0, -2, 5);
    // matrix_t *transform = matrix_mul(rotation, translation);
    // matrix_del(rotation);
    // matrix_del(translation);
    // matrix_del(c.transform);
    // c.transform = transform;

    // ray_t r = camera_ray_for_pixel(&c, 100, 50);
    // tuple_print(&r.origin);
    // tuple_print(&r.direction);
    // camera_del(&c);

    return 0;
}