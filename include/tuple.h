#ifndef TUPLE_H_
#define TUPLE_H_

typedef struct Tuple Tuple;
typedef Tuple Vector;
typedef Tuple Point;

Tuple tuple_create(double x, double y, double z, uint8_t w);
Vector tuple_create_vector(double x, double y, double z);
Point tuple_create_point(double x, double y, double z);
bool tuple_equal(Tuple *a, Tuple *b, double epsilon);
Tuple tuple_add(Tuple *a, Tuple *b);
Tuple tuple_sub(Tuple *a, Tuple *b);
Tuple tuple_neg(Tuple *tuple);
Tuple tuple_mul_scal(Tuple *tuple, double factor);
Tuple tuple_div_scal(Tuple *tuple, double factor);
double tuple_vec_len(Vector *vector);
double tuple_vec_dot(Vector *a, Vector *b);
Vector tuple_vec_cross(Vector *a, Vector *b);

#endif