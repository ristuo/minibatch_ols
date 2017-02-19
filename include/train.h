#ifndef TRAIN_H
#define TRAIN_H
#include "../include/types.h"
void train( char* file_path
          , char separator
          , index target_var_column
          , int max_sweeps
          , index batch_size );
#endif
