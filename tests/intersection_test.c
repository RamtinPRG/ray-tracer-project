#include <stdio.h>
#include "intersection.h"
#include "sphere.h"
#include "ray.h"

int main()
{
    ray_t r = ray_create(tuple_create_point(0, 0, -5), tuple_create_vector(0, 0, 1));
    sphere_t *shape = sphere_create();
    intersection_t i = intersection_create(4, shape);
    intersection_precomp_t precomp = intersection_prepare_computations(&i, r);
    tuple_print(&precomp.eyev);
    tuple_print(&precomp.normalv);
    tuple_print(&precomp.point);
    printf("%10.5f\n", precomp.t);
    printf("%d\n", precomp.object == shape);
    sphere_del(shape);

    return 0;
}