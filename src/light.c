#include <math.h>
#include "light.h"
#include "tuple.h"
#include "canvas.h"
#include "material.h"

inline point_light_t light_create(point_t position, color_t intensity)
{
    return (point_light_t){
        .position = position,
        .intensity = intensity,
    };
}

color_t light_lighting(material_t *material, point_light_t *light, point_t point, vector_t eye_vec, vector_t normal_vec)
{
    color_t ambient = color_create(0, 0, 0);
    color_t diffuse = color_create(0, 0, 0);
    color_t specular = color_create(0, 0, 0);

    color_t effective_color = color_mul(&material->color, &light->intensity);
    vector_t light_vec = tuple_sub(&light->position, &point);
    light_vec = tuple_vec_nor(&light_vec);
    double light_dot_nor = tuple_vec_dot(&light_vec, &normal_vec);

    ambient = color_mul_scal(&effective_color, material->ambient);
    if (light_dot_nor >= 0)
    {
        diffuse = color_mul_scal(&effective_color, material->diffuse * light_dot_nor);

        vector_t light_vec_neg = tuple_neg(&light_vec);
        vector_t reflect_vec = tuple_vec_reflect(&light_vec_neg, &normal_vec);
        double reflect_dot_eye = tuple_vec_dot(&reflect_vec, &eye_vec);

        if (reflect_dot_eye > 0)
        {
            double factor = pow(reflect_dot_eye, material->shininess);
            specular = color_mul_scal(&light->intensity, material->specular * factor);
        }
    }

    color_t output = color_add(&ambient, &diffuse);
    output = color_add(&output, &specular);

    return output;
}