#ifndef TRANSFORM_H_
#define TRANSFORM_H_

matrix_t *transform_identity_matrix();
matrix_t *transform_translation_matrix(double x, double y, double z);
matrix_t *transform_scaling_matrix(double x, double y, double z);
matrix_t *transform_rotation_x_matrix(double angle);
matrix_t *transform_rotation_y_matrix(double angle);
matrix_t *transform_rotation_z_matrix(double angle);
matrix_t *transform_shearing_matrix(double xy, double xz, double yx, double yz, double zx, double zy);

#endif