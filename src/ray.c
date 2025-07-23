#include "tuple.h"
#include "matrix.h"
#include "ray.h"

ray_t ray_create(point_t origin, vector_t direction)
{
    return (ray_t){
        .origin = origin,
        .direction = direction,
    };
}

point_t ray_position(ray_t *ray, double t)
{
    tuple_t distance = tuple_mul_scal(&ray->direction, t);
    return tuple_add(&ray->origin, &distance);
}

ray_t ray_transform(ray_t *ray, matrix_t *m)
{
    point_t origin = matrix_mul_tup(m, ray->origin);
    vector_t direction = matrix_mul_tup(m, ray->direction);
    return (ray_t){
        .origin = origin,
        .direction = direction,
    };
}

// int main()
// {
//     ray_t ray = ray_create((point_t){2, 3, 4, 1}, (vector_t){1, 0, 0, 0});
//     point_t p1 = ray_position(&ray, 0);
//     point_t p2 = ray_position(&ray, 1);
//     point_t p3 = ray_position(&ray, -1);
//     point_t p4 = ray_position(&ray, 2.5);
//     tuple_print(&p1);
//     tuple_print(&p2);
//     tuple_print(&p3);
//     tuple_print(&p4);

//     return 0;
// }