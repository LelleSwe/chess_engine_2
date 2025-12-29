#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef arrvec_def
#define arrvec_def
#define arr_unsafe_ops false

#define DECLARE_ARR(type, max)                                                 \
   typedef struct {                                                            \
      type data[max];                                                          \
      size_t size;                                                             \
   } arr_##type##max;                                                          \
                                                                               \
   arr_##type##max new_arr_##type##max();                                      \
   void arr_push_##type##max(arr_##type##max *arr, const type elem);           \
   type arr_pop_##type##max(arr_##type##max *arr);                             \
   void arr_drop_##type##max(arr_##type##max *arr);                            \
   type *arr_last_##type##max(const arr_##type##max *arr);                     \
   type arr_get_##type##max(const arr_##type##max *arr, const size_t idx);     \
   void arr_set_##type##max(arr_##type##max *arr, const size_t idx,            \
                            const type elem);

#define DEFINE_ARR(type, max)                                                  \
   inline arr_##type##max new_arr_##type##max() {                              \
      type data[max];                                                          \
      arr_##type##max arr = {{*data}, 0};                                      \
      return arr;                                                              \
   }                                                                           \
                                                                               \
   inline void arr_push_##type##max(arr_##type##max *arr, const type elem) {   \
      if (!arr_unsafe_ops && (arr->size >= max)) {                             \
         printf("ERROR: array of type %s with capacity %zu and max size "      \
                "tried to push element. File %s and line %d.\n",               \
                #type, (size_t)max, __FILE__, __LINE__);                       \
         exit(EXIT_FAILURE);                                                   \
      }                                                                        \
      (arr->data)[arr->size] = elem;                                           \
      (arr->size)++;                                                           \
   }                                                                           \
                                                                               \
   inline type arr_pop_##type##max(arr_##type##max *arr) {                     \
      if (!arr_unsafe_ops && (arr->size == 0)) {                               \
         printf("ERROR: array of type %s with capacity %zu tried to pop when " \
                "empty! File %s and line %d.\n",                               \
                #type, (size_t)max, __FILE__, __LINE__);                       \
         exit(EXIT_FAILURE);                                                   \
      }                                                                        \
      type elem = (arr->data)[arr->size - 1];                                  \
      (arr->size)--;                                                           \
      return elem;                                                             \
   }                                                                           \
                                                                               \
   inline void arr_drop_##type##max(arr_##type##max *arr) {                    \
      if (!arr_unsafe_ops && (arr->size == 0)) {                               \
         printf(                                                               \
             "ERROR: array of type %s with capacity %zu tried to drop when "   \
             "empty! File %s and line %d.\n",                                  \
             #type, (size_t)max, __FILE__, __LINE__);                          \
         exit(EXIT_FAILURE);                                                   \
      }                                                                        \
      (arr->size)--;                                                           \
   }                                                                           \
                                                                               \
   inline type *arr_last_##type##max(const arr_##type##max *arr) {             \
      if (!arr_unsafe_ops && (arr->size == 0)) {                               \
         printf("ERROR: array of type %s with capacity %zu tried to peek last" \
                "when empty! File %s and line %d.\n",                          \
                #type, (size_t)max, __FILE__, __LINE__);                       \
         exit(EXIT_FAILURE);                                                   \
      }                                                                        \
      type *elem = &((arr->data)[arr->size - 1]);                              \
      return elem;                                                             \
   }                                                                           \
                                                                               \
   inline type arr_get_##type##max(const arr_##type##max *arr,                 \
                                   const size_t idx) {                         \
      if (!arr_unsafe_ops && (idx >= (arr->size))) {                           \
         printf("ERROR: array of type %s with size %zu tried to get at "       \
                "index %zu. File %s and line %d.\n",                           \
                #type, arr->size, idx, __FILE__, __LINE__);                    \
         exit(EXIT_FAILURE);                                                   \
      }                                                                        \
      type elem = (arr->data)[idx];                                            \
      return elem;                                                             \
   }                                                                           \
                                                                               \
   inline void arr_set_##type##max(arr_##type##max *arr, const size_t idx,     \
                                   const type elem) {                          \
      if (!arr_unsafe_ops && (idx >= (arr->size))) {                           \
         printf("ERROR: array of type %s with size %zu tried to set at "       \
                "index %zu. File %s and line %d.\n",                           \
                #type, arr->size, idx, __FILE__, __LINE__);                    \
         exit(EXIT_FAILURE);                                                   \
      }                                                                        \
      (arr->data)[idx] = elem;                                                 \
   }

#endif

#include <time.h>
#define CLOCK_START()                                                          \
   struct timespec START_TIME_dorigh, END_TIME_dorigh;                         \
                                                                               \
   timespec_get(&START_TIME_dorigh, TIME_UTC)

#define CLOCK_END()                                                            \
   timespec_get(&END_TIME_dorigh, TIME_UTC);                                   \
   double TIME_TAKEN =                                                         \
       (END_TIME_dorigh.tv_sec - START_TIME_dorigh.tv_sec) +                   \
       (END_TIME_dorigh.tv_nsec - START_TIME_dorigh.tv_nsec) / 1e9;            \
   printf("Time taken for %s: %.9f seconds\n", __func__, TIME_TAKEN)\
