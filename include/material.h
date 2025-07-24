#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "canvas.h"

typedef struct material_t
{
    color_t color;
    double ambient;
    double diffuse;
    double specular;
    double shininess;
} material_t;

material_t material_create();

#endif