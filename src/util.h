#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef vector_def
#define vector_def

#define DECLARE_VEC(type)                                                      \
   typedef struct {                                                            \
      type *ptr;                                                               \
      size_t capacity;                                                         \
      size_t size;                                                             \
   } vec_##type;                                                               \
                                                                               \
   vec_##type new_vec_##type(const size_t capacity);                           \
   void vec_push_##type(vec_##type *vec, const type elem);                     \
   type vec_pop_##type(vec_##type *vec);                                       \
   type *vec_last_##type(vec_##type *vec);                                     \
   type vec_get_##type(vec_##type *vec, const size_t idx);                     \
   void vec_set_##type(vec_##type *vec, const size_t idx, const type elem);

#define DEFINE_VEC(type)                                                       \
   vec_##type new_vec_##type(const size_t capacity) {                          \
      type *ptr = malloc(sizeof(type) * capacity);                             \
      if (ptr == NULL) {                                                       \
         printf("ERROR: malloc returned null pointer in file %s at line "      \
                "%d.\n",                                                       \
                __FILE__, __LINE__);                                           \
         exit(EXIT_FAILURE);                                                   \
      }                                                                        \
      vec_##type vec = {ptr, capacity, 0};                                     \
      return vec;                                                              \
   }                                                                           \
                                                                               \
   void vec_push_##type(vec_##type *vec, const type elem) {                    \
      if (vec->size >= vec->capacity) {                                        \
         printf("ERROR: vector of type %s with capacity %zu and max size "     \
                "tried to push element. File %s and line %d.\n",               \
                #type, vec->capacity, __FILE__, __LINE__);                     \
         exit(EXIT_FAILURE);                                                   \
      }                                                                        \
      (vec->ptr)[vec->size] = elem;                                            \
      (vec->size)++;                                                           \
   }                                                                           \
                                                                               \
   type vec_pop_##type(vec_##type *vec) {                                      \
      if (vec->size == 0) {                                                    \
         printf(                                                               \
             "ERROR: vector of type %s with capacity %zu tried to pop when "   \
             "empty! File %s and line %d.\n",                                  \
             #type, vec->capacity, __FILE__, __LINE__);                        \
         exit(EXIT_FAILURE);                                                   \
      }                                                                        \
      type elem = (vec->ptr)[vec->size - 1];                                   \
      (vec->ptr)[vec->size - 1] = (type){};                                    \
      (vec->size)--;                                                           \
      return elem;                                                             \
   }                                                                           \
                                                                               \
   type *vec_last_##type(vec_##type *vec) {                                    \
      if (vec->size == 0) {                                                    \
         printf("ERROR: vector of type %s with capacity %zu tried to pop"      \
                "when empty! File %s and line %d.\n",                          \
                #type, vec->capacity, __FILE__, __LINE__);                     \
         exit(EXIT_FAILURE);                                                   \
      }                                                                        \
      type *elem = &((vec->ptr)[vec->size - 1]);                               \
      return elem;                                                             \
   }                                                                           \
                                                                               \
   type vec_get_##type(vec_##type *vec, const size_t idx) {                    \
      if (idx >= (vec->size)) {                                                \
         printf("ERROR: vector of type %s with size %zu tried to get at "      \
                "index %zu. File %s and line %d.\n",                           \
                #type, vec->size, idx, __FILE__, __LINE__);                    \
         exit(EXIT_FAILURE);                                                   \
      }                                                                        \
      type elem = (vec->ptr)[idx];                                             \
      return elem;                                                             \
   }                                                                           \
                                                                               \
   void vec_set_##type(vec_##type *vec, const size_t idx, const type elem) {   \
      if (idx >= (vec->size)) {                                                \
         printf("ERROR: vector of type %s with size %zu tried to set at "      \
                "index %zu. File %s and line %d.\n",                           \
                #type, vec->size, idx, __FILE__, __LINE__);                    \
         exit(EXIT_FAILURE);                                                   \
      }                                                                        \
      (vec->ptr)[idx] = elem;                                                  \
   }

#endif
