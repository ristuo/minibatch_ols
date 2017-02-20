#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include "../include/train.h"
#include "../include/types.h"
#include "../include/dataset.h"
#include "../include/io.h"
#include "../include/model.h"
#include <stdbool.h>
void train( char* file_path
          , char separator
          , index target_var_column
          , int max_sweeps
          , index batch_size )
{
    model_ptr model = model_create();
    index offset = 0; 
    int i = 0;
    while( i < max_sweeps )
    {
        io_res_ptr input = read_batch( file_path
                                      , separator
                                      , batch_size
                                      , offset );
        dataset_ptr ds = dataset_from_io( input 
                                        , target_var_column );
        bool converged = model_update( model, ds );
        if (eof( input ))
        {
            offset=0;
            i++;
        }
        else
        {
            offset += bytes_read( input );
        }
        io_res_destroy( input ); 
        dataset_destroy( ds );
        if (converged)
        {
            printf("reached convergence!\n");
            break;
        }
    }
    model_print(model);
    model_destroy( model );
}
