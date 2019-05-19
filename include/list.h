#ifndef LIST_H
#define LIST_H
#define CREATE_LIST(var, type)                                                 \
  struct {                                                                     \
    type *values;                                                              \
    index size;                                                                \
  } var;                                                                       \
  var.values = NULL;

#define LIST(var, type)                                                        \
  struct {                                                                     \
    type *values;                                                              \
    index size;                                                                \
  } var;

#define INIT(list)                                                             \
  list.values = NULL;                                                          \
  list.size = 0;

#define PUSH(list, type, value)                                                \
  list.values = realloc(list.values, sizeof(type) * (list.size + 1));          \
  if (!list.values) {                                                          \
    printf("Memory allocation failed\n!");                                     \
    exit(EXIT_FAILURE);                                                        \
  }                                                                            \
  list.values[list.size] = value;                                              \
  list.size += 1;

#define AT(list, index) list.values[(list.size - 1) - index]
#endif
