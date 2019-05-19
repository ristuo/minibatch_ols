#ifndef MODEL_H
#define MODEL_H
#include "../include/dataset.h"
#include "../include/matrix.h"
#include <stdbool.h>
typedef struct model *model_ptr;

typedef struct stop_cond *stop_cond_ptr;

typedef enum { ITERATIONS, CONVERGENCE } stop_type;

stop_cond_ptr stop_cond_create(stop_type type, number treshold);

bool should_stop(model_ptr model);

void stop_cond_destroy(stop_cond_ptr x);

model_ptr model_create(stop_cond_ptr stop);

void model_destroy(model_ptr);

bool model_update(model_ptr model, dataset_ptr ds);

void model_print(model_ptr mod);

void model_print_coeff(model_ptr model);

void model_print_values(model_ptr model);

index iterations_run(model_ptr model);

matrix_ptr model_predict(model_ptr model, matrix_ptr mat);
#endif
