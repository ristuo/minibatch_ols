#ifndef TRAIN_H
#define TRAIN_H
#include "../include/types.h"
void train( char* file_path
          , char separator
          , index target_var_column
          , int max_sweeps
          , index batch_size );

void train2( char* file_path
          , char separator
          , index target_var_column
          , index max_sweeps
          , index batch_size );
#endif
