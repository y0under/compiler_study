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
extern Vector *new_vec();
extern void vec_push(Vector *v, void *elem);
extern void *vec_pop(Vector *v);
extern void *vec_pop(Vector *v);
extern void *vector_last(Vector *v);

#endif
