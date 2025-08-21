#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include "tuple.h"
#include "matrix.h"
#include "transform.h"

struct matrix_t
{
    uint8_t size;
    double *data;
};

matrix_t *matrix_create(uint8_t size)
{
    matrix_t *matrix = malloc(sizeof(matrix_t));

    matrix->size = size;
    matrix->data = calloc(size * size, sizeof(double));

    return matrix;
}

void matrix_del(matrix_t *matrix)
{
    if (matrix)
    {
        if (matrix->data)
            free(matrix->data);
        free(matrix);
    }
}

void matrix_set(const matrix_t *matrix, uint8_t row, uint8_t col, double value)
{
    if (!matrix)
        return;

    matrix->data[row * matrix->size + col] = value;
}

double matrix_get(const matrix_t *matrix, uint8_t row, uint8_t col)
{
    // if (!matrix)
    //     return 0;

    return matrix->data[row * matrix->size + col];
}

// NOTE: when using this function make sure inputs are double (not integers or anything else)
void matrix_set_values(matrix_t *m, ...)
{
    va_list args;
    va_start(args, m);

    int total = m->size * m->size;
    for (int i = 0; i < total; i++)
    {
        m->data[i] = va_arg(args, double);
    }

    va_end(args);
}

void matrix_set_from_array(const matrix_t *m, const double *values)
{
    int total = m->size * m->size;
    for (int i = 0; i < total; i++)
        m->data[i] = values[i];
}

void matrix_print(const matrix_t *m)
{
    for (int i = 0; i < m->size; i++)
    {
        for (int j = 0; j < m->size; j++)
        {
            printf("%10.5f ", matrix_get(m, i, j));
        }
        printf("\n");
    }
}

matrix_t *matrix_mul(const matrix_t *m1, const matrix_t *m2)
{
    if (!m1 || !m2)
        return NULL;
    if (m1->size != m2->size)
        return NULL;

    uint8_t size = m1->size;
    matrix_t *result = matrix_create(size);

    for (uint8_t row = 0; row < size; row++)
    {
        for (uint8_t col = 0; col < size; col++)
        {
            double val = 0;
            for (uint8_t i = 0; i < size; i++)
                val += matrix_get(m1, row, i) * matrix_get(m2, i, col);
            matrix_set(result, row, col, val);
        }
    }

    return result;
}

matrix_t *matrix_mul_multiple(int count, ...)
{
    if (count < 1)
        return NULL;

    va_list args;
    va_start(args, count);

    matrix_t *matrices[count];
    for (int i = 0; i < count; i++)
        matrices[i] = va_arg(args, matrix_t *);

    va_end(args);

    matrix_t *result = matrix_create(matrices[0]->size);
    matrix_set_from_array(result, matrices[0]->data);
    for (int i = 1; i < count; i++)
    {
        if (matrices[i]->size != result->size)
            return NULL;
        matrix_t *temp = matrix_mul(result, matrices[i]);
        matrix_del(result);
        result = temp;
    }

    return result;
}

tuple_t matrix_mul_tup(const matrix_t *m, tuple_t t)
{
    if (!m)
        return (tuple_t){0, 0, 0, 0};
    if (m->size != 4)
        return (tuple_t){0, 0, 0, 0};

    tuple_t result = tuple_create(0, 0, 0, 0);

    // value of x
    result.x += matrix_get(m, 0, 0) * t.x;
    result.x += matrix_get(m, 0, 1) * t.y;
    result.x += matrix_get(m, 0, 2) * t.z;
    result.x += matrix_get(m, 0, 3) * t.w;

    // value of y
    result.y += matrix_get(m, 1, 0) * t.x;
    result.y += matrix_get(m, 1, 1) * t.y;
    result.y += matrix_get(m, 1, 2) * t.z;
    result.y += matrix_get(m, 1, 3) * t.w;

    // value of z
    result.z += matrix_get(m, 2, 0) * t.x;
    result.z += matrix_get(m, 2, 1) * t.y;
    result.z += matrix_get(m, 2, 2) * t.z;
    result.z += matrix_get(m, 2, 3) * t.w;

    // value of w
    result.w += matrix_get(m, 3, 0) * t.x;
    result.w += matrix_get(m, 3, 1) * t.y;
    result.w += matrix_get(m, 3, 2) * t.z;
    result.w += matrix_get(m, 3, 3) * t.w;

    return result;
}

matrix_t *matrix_transpose(const matrix_t *m)
{
    if (!m)
        return NULL;

    matrix_t *result = matrix_create(m->size);

    for (uint8_t row = 0; row < m->size; row++)
        for (uint8_t col = 0; col < m->size; col++)
            matrix_set(result, row, col, matrix_get(m, col, row));

    return result;
}

double matrix_det(const matrix_t *m)
{
    // if (!m)
    //     return 0;

    // if (m->size == 2)
    //     return matrix_get(m, 0, 0) * matrix_get(m, 1, 1) - matrix_get(m, 0, 1) * matrix_get(m, 1, 0);

    if (m->size == 1)
        return matrix_get(m, 0, 0);

    double det = 0;
    for (int col = 0; col < m->size; col++)
        det += matrix_cofactor(m, 0, col) * matrix_get(m, 0, col);
    return det;
}

matrix_t *matrix_submatrix(const matrix_t *m, uint8_t rm_row, uint8_t rm_col)
{
    if (!m)
        return NULL;

    matrix_t *result = matrix_create(m->size - 1);
    for (int row = 0; row < result->size; row++)
    {
        for (int col = 0; col < result->size; col++)
        {
            double val = matrix_get(m, (row < rm_row) ? row : (row + 1), (col < rm_col) ? col : (col + 1));
            matrix_set(result, row, col, val);
        }
    }

    return result;
}

double matrix_minor(const matrix_t *m, uint8_t row, uint8_t col)
{
    if (!m)
        return 0;

    matrix_t *sub = matrix_submatrix(m, row, col);
    double det = matrix_det(sub);
    matrix_del(sub);

    return det;
}

double matrix_cofactor(const matrix_t *m, uint8_t row, uint8_t col)
{
    if (!m)
        return 0;

    double minor = matrix_minor(m, row, col);
    return ((row + col) % 2 == 0) ? minor : -minor;
}

bool matrix_invertible(const matrix_t *m)
{
    return matrix_det(m) != 0;
}

matrix_t *matrix_inverse(const matrix_t *m)
{
    if (!m)
        return NULL;
    if (!matrix_invertible(m))
        return NULL;

    matrix_t *result = matrix_create(m->size);
    double det = matrix_det(m);
    for (int row = 0; row < m->size; row++)
    {
        for (int col = 0; col < m->size; col++)
        {
            double val = matrix_cofactor(m, row, col) / det;
            matrix_set(result, col, row, val);
        }
    }

    return result;
}

// int main()
// {
//     matrix_t *m = matrix_create(2);
//     matrix_print(m);
//     printf("------------\n\n");

//     matrix_set_values(m, 1., 2., 3., 4.);
//     matrix_print(m);
//     printf("------------\n\n");

//     matrix_set_from_array(m, (double[]){5, 6, 7, 8});
//     matrix_print(m);
//     printf("------------\n\n");

//     MATRIX_SET_VALUES(m, 1, 5, -3, 2);
//     matrix_print(m);
//     printf("------------\n\n");

//     printf("%6.2f\n\n", matrix_det(m));

//     matrix_del(m);

//     matrix_t *m3 = matrix_create(3);

//     MATRIX_SET_VALUES(m3,
//                       1, 5, 0,
//                       -3, 2, 7,
//                       0, 6, -3);
//     matrix_t *sub = matrix_submatrix(m3, 0, 2);
//     matrix_print(sub);
//     printf("------------\n\n");
//     matrix_del(sub);

//     matrix_del(m3);

//     matrix_t *m4 = matrix_create(4);

//     MATRIX_SET_VALUES(m4,
//                       -6, 1, 1, 6,
//                       -8, 5, 8, 6,
//                       -1, 0, 8, 2,
//                       -7, 1, -1, 1);
//     sub = matrix_submatrix(m4, 2, 1);
//     matrix_print(sub);
//     printf("------------\n\n");
//     matrix_del(sub);

//     matrix_del(m4);

//     m3 = matrix_create(3);
//     MATRIX_SET_VALUES(m3,
//                       1, 2, 6,
//                       -5, 8, -4,
//                       2, 6, 4);
//     printf("%6.2f\n\n", matrix_cofactor(m3, 0, 0));
//     printf("%6.2f\n\n", matrix_cofactor(m3, 0, 1));
//     printf("%6.2f\n\n", matrix_cofactor(m3, 0, 2));
//     printf("%6.2f\n\n", matrix_det(m3));
//     matrix_del(m3);

//     m4 = matrix_create(4);
//     MATRIX_SET_VALUES(m4,
//                       -2, -8, 3, 5,
//                       -3, 1, 7, 3,
//                       1, 2, -9, 6,
//                       -6, 7, 7, -9);
//     printf("%6.2f\n\n", matrix_cofactor(m4, 0, 0));
//     printf("%6.2f\n\n", matrix_cofactor(m4, 0, 1));
//     printf("%6.2f\n\n", matrix_cofactor(m4, 0, 2));
//     printf("%6.2f\n\n", matrix_cofactor(m4, 0, 3));
//     printf("%6.2f\n\n", matrix_det(m4));
//     matrix_del(m4);

//     m4 = matrix_create(4);
//     MATRIX_SET_VALUES(m4,
//                       6, 4, 4, 4,
//                       5, 5, 7, 6,
//                       4, -9, 3, -7,
//                       9, 1, 7, -6);
//     printf("%d\n\n", matrix_invertible(m4));
//     matrix_del(m4);

//     m4 = matrix_create(4);
//     MATRIX_SET_VALUES(m4,
//                       0, 4, 4, 4,
//                       0, 5, 7, 6,
//                       0, -9, 3, -7,
//                       0, 1, 7, -6);
//     printf("%d\n\n", matrix_invertible(m4));
//     matrix_del(m4);

//     m4 = matrix_create(4);
//     MATRIX_SET_VALUES(m4,
//                       -5, 2, 6, -8,
//                       1, -5, 1, 8,
//                       7, 7, -6, -7,
//                       1, -3, 7, 4);
//     matrix_t *inverse = matrix_inverse(m4);
//     matrix_print(inverse);
//     matrix_del(inverse);
//     matrix_del(m4);

//     m = matrix_create(2);
//     MATRIX_SET_VALUES(m,
//                       2, 5,
//                       3, 8);
//     inverse = matrix_inverse(m);
//     matrix_print(inverse);
//     matrix_del(inverse);
//     matrix_del(m);

//     m4 = matrix_create(4);
//     MATRIX_SET_VALUES(m4,
//                       8, -5, 9, 2,
//                       7, 5, 6, 1,
//                       -6, 0, 9, 6,
//                       -3, 0, -9, -4);
//     inverse = matrix_inverse(m4);
//     matrix_print(inverse);
//     matrix_del(inverse);
//     matrix_del(m4);

//     matrix_t *a = matrix_create(4);
//     matrix_t *b = matrix_create(4);
//     MATRIX_SET_VALUES(a,
//                       3, -9, 7, 3,
//                       3, -8, 2, -9,
//                       -4, 4, 4, 1,
//                       -6, 5, -1, 1);
//     MATRIX_SET_VALUES(b,
//                       8, 2, 2, 2,
//                       3, -1, 7, 0,
//                       7, 0, 5, 4,
//                       6, -2, 0, 5);
//     matrix_t *c = matrix_mul(a, b);
//     matrix_t *b_inverse = matrix_inverse(b);
//     matrix_t *result = matrix_mul(c, b_inverse);
//     matrix_print(c);
//     matrix_print(result);
//     matrix_del(a);
//     matrix_del(b);
//     matrix_del(c);
//     matrix_del(b_inverse);
//     matrix_del(result);

//     return 0;
// }