#ifndef MODEL_H
#define MODEL_H
#include "../include/dataset.h"
#include "../include/matrix.h"
#include <stdbool.h>
typedef struct model* model_ptr;

model_ptr model_create(void);

void model_destroy( model_ptr );

bool model_update( model_ptr model, dataset_ptr ds );

void model_print( model_ptr mod );

matrix_ptr model_predict( model_ptr model, matrix_ptr mat );
#endif
