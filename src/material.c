#include "material.h"

inline material_t material_create()
{
    return (material_t){
        .color = {1., 1., 1.},
        .ambient = .1,
        .diffuse = .9,
        .specular = .9,
        .shininess = 200.,
    };
}