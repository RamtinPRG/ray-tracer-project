#include <stdio.h>
#include "transform.h"
#include "tuple.h"
#include "matrix.h"

int main()
{
    /***************** View Transform Tests ****************/

    /* Test 1 */
    // point_t from = {0, 0, 0, 1};
    // point_t to = {0, 0, 1, 1};
    // vector_t up = {0, 1, 0, 0};
    // matrix_t *t = transform_view(from, to, up);
    // matrix_t *trans = transform_scaling_matrix(-1, 1, -1);
    // matrix_print(t);
    // matrix_print(trans);
    // matrix_del(t);
    // matrix_del(trans);

    /* Test 2 */
    // point_t from = {0, 0, 8, 1};
    // point_t to = {0, 0, 0, 1};
    // vector_t up = {0, 1, 0, 0};
    // matrix_t *t = transform_view(from, to, up);
    // matrix_t *trans = transform_translation_matrix(0, 0, -8);
    // matrix_print(t);
    // matrix_print(trans);
    // matrix_del(t);
    // matrix_del(trans);

    /* Test 3 */
    point_t from = {1, 3, 2, 1};
    point_t to = {4, -2, 8, 1};
    vector_t up = {1, 1, 0, 0};
    matrix_t *t = transform_view(from, to, up);
    matrix_print(t);
    matrix_del(t);

    return 0;
}