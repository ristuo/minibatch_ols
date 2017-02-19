#ifndef DATASET_H
#define DATASET_H
#include "../include/matrix.h"
#include "../include/io.h"
#include "../include/types.h"
typedef struct dataset* dataset_ptr;

matrix_ptr dependent(dataset_ptr);

matrix_ptr independent(dataset_ptr);

void dataset_print( dataset_ptr );

dataset_ptr dataset_from_batch( char* file_path
                              , char separator
                              , index nrow
                              , index offset
                              , index target_var_column );

dataset_ptr dataset_from_io( io_res_ptr io_res, index target_var_column );

dataset_ptr dataset_create( number** values
                          , index nrow
                          , index ncol
                          , index target_var_column );
#endif
