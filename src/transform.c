#include <stdint.h>
#include <math.h>
#include "tuple.h"
#include "matrix.h"
#include "transform.h"

matrix_t *transform_identity_matrix()
{
    matrix_t *result = matrix_create(4);
    MATRIX_SET_VALUES(result,
                      1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1);
    return result;
}

matrix_t *transform_translation_matrix(double x, double y, double z)
{
    matrix_t *result = matrix_create(4);
    MATRIX_SET_VALUES(result,
                      1, 0, 0, x,
                      0, 1, 0, y,
                      0, 0, 1, z,
                      0, 0, 0, 1);
    return result;
}

matrix_t *transform_scaling_matrix(double x, double y, double z)
{
    matrix_t *result = matrix_create(4);
    MATRIX_SET_VALUES(result,
                      x, 0, 0, 0,
                      0, y, 0, 0,
                      0, 0, z, 0,
                      0, 0, 0, 1);
    return result;
}

matrix_t *transform_rotation_x_matrix(double angle)
{
    matrix_t *result = matrix_create(4);
    double c = cos(angle);
    double s = sin(angle);
    MATRIX_SET_VALUES(result,
                      1, 0, 0, 0,
                      0, c, -s, 0,
                      0, s, c, 0,
                      0, 0, 0, 1);
    return result;
}

matrix_t *transform_rotation_y_matrix(double angle)
{
    matrix_t *result = matrix_create(4);
    double c = cos(angle);
    double s = sin(angle);
    MATRIX_SET_VALUES(result,
                      c, 0, s, 0,
                      0, 1, 0, 0,
                      -s, 0, c, 0,
                      0, 0, 0, 1);
    return result;
}

matrix_t *transform_rotation_z_matrix(double angle)
{
    matrix_t *result = matrix_create(4);
    double c = cos(angle);
    double s = sin(angle);
    MATRIX_SET_VALUES(result,
                      c, -s, 0, 0,
                      s, c, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1);
    return result;
}

// As an example `xy` means "moving in x in proportion to y" and so on
matrix_t *transform_shearing_matrix(double xy, double xz, double yx, double yz, double zx, double zy)
{
    matrix_t *result = matrix_create(4);
    MATRIX_SET_VALUES(result,
                      1, xy, xz, 0,
                      yx, 1, yz, 0,
                      zx, zy, 1, 0,
                      0, 0, 0, 1);
    return result;
}

// int main()
// {

//     matrix_t *transform = transform_translation_matrix(5, -3, 2);
//     matrix_print(transform);
//     point_t p = tuple_create_point(-3, 4, 5);
//     tuple_print(&p);
//     point_t res = matrix_mul_tup(transform, p);
//     tuple_print(&res);

//     matrix_t *inverse = matrix_inverse(transform);
//     point_t res2 = matrix_mul_tup(inverse, p);
//     tuple_print(&res2);
//     matrix_del(inverse);

//     transform = transform_shearing_matrix(1.5, 0, 0, 0, 0, 0);
//     p = tuple_create_point(2, 3, 4);
//     res = matrix_mul_tup(transform, p);
//     tuple_print(&res);

//     matrix_del(transform);

//     return 0;
// }