#ifndef COMPILER_SRC_VECTOR_H
#define COMPILER_SRC_VECTOR_H

#include <assert.h>
#include <stdlib.h>

// prototype
typedef struct Vector
{
  void **data;
  int  capacity;
  int  len;
} Vector;


// function
Vector *new_vec();
void vec_push(Vector *v, void *elem);
void *vec_pop(Vector *v);
void *vec_pop(Vector *v);
void *vector_last(Vector *v);

#endif
