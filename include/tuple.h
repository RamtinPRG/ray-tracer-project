#ifndef TUPLE_H_
#define TUPLE_H_

#include <stdint.h>

typedef struct
{
    double x;
    double y;
    double z;
    uint8_t w;
} tuple_t;
typedef tuple_t vector_t;
typedef tuple_t point_t;

tuple_t tuple_create(double x, double y, double z, uint8_t w);
vector_t tuple_create_vector(double x, double y, double z);
point_t tuple_create_point(double x, double y, double z);
void tuple_print(tuple_t *a);
bool tuple_equal(tuple_t *a, tuple_t *b, double epsilon);
tuple_t tuple_add(tuple_t *a, tuple_t *b);
tuple_t tuple_sub(tuple_t *a, tuple_t *b);
tuple_t tuple_neg(tuple_t *tuple);
tuple_t tuple_mul_scal(tuple_t *tuple, double factor);
tuple_t tuple_div_scal(tuple_t *tuple, double factor);
double tuple_vec_len(vector_t *vector);
vector_t tuple_vec_nor(vector_t *vector);
double tuple_vec_dot(vector_t *a, vector_t *b);
vector_t tuple_vec_cross(vector_t *a, vector_t *b);
vector_t tuple_vec_reflect(vector_t *in, vector_t *nor);

#endif