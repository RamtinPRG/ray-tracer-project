#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdint.h>
#include "tuple.h"

#define MATRIX_SET_VALUES(mtx, ...)         \
    do                                      \
    {                                       \
        double values[] = {__VA_ARGS__};    \
        matrix_set_from_array(mtx, values); \
    } while (0)

typedef struct matrix_t matrix_t;

matrix_t *matrix_create(uint8_t size);
void matrix_del(matrix_t *matrix);
void matrix_set(const matrix_t *matrix, uint8_t row, uint8_t col, double value);
double matrix_get(const matrix_t *matrix, uint8_t row, uint8_t col);
void matrix_set_values(matrix_t *m, ...);
void matrix_set_from_array(const matrix_t *m, const double *values);
void matrix_print(const matrix_t *m);
matrix_t *matrix_mul(const matrix_t *m1, const matrix_t *m2);
matrix_t *matrix_mul_multiple(int count, ...);
tuple_t matrix_mul_tup(const matrix_t *m, tuple_t t);
matrix_t *matrix_transpose(const matrix_t *m);
double matrix_det(const matrix_t *m);
matrix_t *matrix_submatrix(const matrix_t *m, uint8_t rm_row, uint8_t rm_col);
double matrix_minor(const matrix_t *m, uint8_t row, uint8_t col);
double matrix_cofactor(const matrix_t *m, uint8_t row, uint8_t col);
bool matrix_invertible(const matrix_t *m);
matrix_t *matrix_inverse(const matrix_t *m);

#endif