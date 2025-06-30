#ifndef TUPLE_H_
#define TUPLE_H_

typedef struct Tuple Tuple;
typedef Tuple Vector;
typedef Tuple Point;

Tuple *create_tuple(double x, double y, double z, double w);
Tuple *create_vector(double x, double y, double z);

#endif