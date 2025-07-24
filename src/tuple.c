#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "tuple.h"

static bool float_equal(double a, double b, double epsilon)
{
    return fabs(a - b) < epsilon;
}

tuple_t tuple_create(double x, double y, double z, uint8_t w)
{
    return (tuple_t){
        .x = x,
        .y = y,
        .z = z,
        .w = w,
    };
}

vector_t tuple_create_vector(double x, double y, double z)
{
    return tuple_create(x, y, z, 0);
}

point_t tuple_create_point(double x, double y, double z)
{
    return tuple_create(x, y, z, 1);
}

void tuple_print(tuple_t *a)
{
    printf("%10.5f, %10.5f, %10.5f, %d\n", a->x, a->y, a->z, a->w);
}

// Checks the equality of 2 tuples
bool tuple_equal(tuple_t *a, tuple_t *b, double epsilon)
{
    return ((a->w == b->w) &&
            float_equal(a->x, b->x, epsilon) &&
            float_equal(a->y, b->y, epsilon) &&
            float_equal(a->z, b->z, epsilon));
}

// Adds 2 tuples together
tuple_t tuple_add(tuple_t *a, tuple_t *b)
{
    return tuple_create(a->x + b->x, a->y + b->y, a->z + b->z, a->w + b->w);
}

// Subtracts tuple b from tuple a
tuple_t tuple_sub(tuple_t *a, tuple_t *b)
{
    return tuple_create(a->x - b->x, a->y - b->y, a->z - b->z, a->w - b->w);
}

// Negates a tuple
tuple_t tuple_neg(tuple_t *tuple)
{
    return tuple_create(-tuple->x, -tuple->y, -tuple->z, -tuple->w);
}

// Multiplies a tuple by a scalar factor
tuple_t tuple_mul_scal(tuple_t *tuple, double factor)
{
    return tuple_create(tuple->x * factor, tuple->y * factor, tuple->z * factor, tuple->w * factor);
}

// Divides a tuple by a scalar factor
tuple_t tuple_div_scal(tuple_t *tuple, double factor)
{
    return tuple_create(tuple->x / factor, tuple->y / factor, tuple->z / factor, tuple->w / factor);
}

double tuple_vec_len(vector_t *vector)
{
    return sqrt(vector->x * vector->x +
                vector->y * vector->y +
                vector->z * vector->z +
                vector->w * vector->w);
}

vector_t tuple_vec_nor(vector_t *vector)
{
    double len = tuple_vec_len(vector);
    return tuple_div_scal(vector, len);
}

double tuple_vec_dot(vector_t *a, vector_t *b)
{
    return (a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w);
}

vector_t tuple_vec_cross(vector_t *a, vector_t *b)
{
    return tuple_create_vector(a->y * b->z - a->z * b->y,
                               a->z * b->x - a->x * b->z,
                               a->x * b->y - a->y * b->x);
}

vector_t tuple_vec_reflect(vector_t *in, vector_t *nor)
{
    vector_t vec_proj = tuple_mul_scal(nor, 2 * tuple_vec_dot(in, nor));
    return tuple_sub(in, &vec_proj);
}