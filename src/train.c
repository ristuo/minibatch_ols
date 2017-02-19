#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include "../include/train.h"
#include "../include/types.h"
#include "../include/dataset.h"
#include "../include/io.h"

void train( char* file_path
          , char separator
          , index target_var_column
          , int max_sweeps
          , index batch_size )
{
    index offset = 0; 
    int i = 0;
    while( i < max_sweeps )
    {
        io_res_ptr input = read_batch( file_path
                                     , separator
                                     , batch_size
                                     , offset );
        if (eof( input ))
        {
            offset=0;
            i++;
        }
        else
        {
            offset += bytes_read( input );
        }
        dataset_ptr ds = dataset_from_io( input, target_var_column );
        io_res_destroy( input );
        //do something
        dataset_destroy( ds );
    }
}
