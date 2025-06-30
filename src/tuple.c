#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "tuple.h"

struct Tuple
{
    double x;
    double y;
    double z;
    uint8_t w;
};

static bool float_equal(double a, double b, double epsilon)
{
    return fabs(a - b) < epsilon;
}

Tuple tuple_create(double x, double y, double z, uint8_t w)
{
    Tuple new_tuple = {
        .x = x,
        .y = y,
        .z = z,
        .w = w,
    };

    return new_tuple;
}

Vector tuple_create_vector(double x, double y, double z)
{
    return tuple_create(x, y, z, 0);
}

Point tuple_create_point(double x, double y, double z)
{
    return tuple_create(x, y, z, 1);
}

// Checks the equality of 2 tuples
bool tuple_equal(Tuple *a, Tuple *b, double epsilon)
{
    return ((a->w == b->w) &&
            float_equal(a->x, b->x, epsilon) &&
            float_equal(a->y, b->y, epsilon) &&
            float_equal(a->z, b->z, epsilon));
}

// Adds 2 tuples together
Tuple tuple_add(Tuple *a, Tuple *b)
{
    return tuple_create(a->x + b->x, a->y + b->y, a->z + b->z, a->w + b->w);
}

// Subtracts tuple b from tuple a
Tuple tuple_sub(Tuple *a, Tuple *b)
{
    return tuple_create(a->x - b->x, a->y - b->y, a->z - b->z, a->w - b->w);
}

// Negates a tuple
Tuple tuple_neg(Tuple *tuple)
{
    return tuple_create(-tuple->x, -tuple->y, -tuple->z, -tuple->w);
}

// Multiplies a tuple by a scalar factor
Tuple tuple_mul_scal(Tuple *tuple, double factor)
{
    return tuple_create(tuple->x * factor, tuple->y * factor, tuple->z * factor, tuple->w * factor);
}

// Divides a tuple by a scalar factor
Tuple tuple_div_scal(Tuple *tuple, double factor)
{
    return tuple_create(tuple->x / factor, tuple->y / factor, tuple->z / factor, tuple->w / factor);
}

double tuple_vec_len(Vector *vector)
{
    return sqrt(vector->x * vector->x +
                vector->y * vector->y +
                vector->z * vector->z +
                vector->w * vector->w);
}

Vector tuple_vec_nor(Vector *vector)
{
    double len = tuple_vec_len(vector);
    return tuple_div_scalar(vector, len);
}

double tuple_vec_dot(Vector *a, Vector *b)
{
    return (a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w);
}

Vector tuple_vec_cross(Vector *a, Vector *b)
{
    return tuple_create_vector(a->y * b->z - a->z * b->y,
                               a->z * b->x - a->x * b->z,
                               a->x * b->y - a->y * b->x);
}