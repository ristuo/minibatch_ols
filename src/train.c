#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "../include/train.h"
#include "../include/types.h"
#include "../include/dataset.h"
#include "../include/io.h"
#include "../include/model.h"
void train( char* file_path
          , char separator
          , index target_var_column
          , int max_sweeps
          , index batch_size )
{
    stop_cond_ptr stop = stop_cond_create( ITERATIONS, 100 );
    model_ptr model = model_create( stop );
    int i = 0;
    FILE* fp = fopen(file_path, "r");
    if (!fp)
    {
        printf("Error in opening file %s!\n", file_path);
        exit(EXIT_FAILURE);
    }
    while( i < max_sweeps )
    {
        io_res_ptr input = read_batch_fp( fp
                                        , separator
                                        , batch_size
                                        , file_path );
        dataset_ptr ds = dataset_from_io( input 
                                        , target_var_column );
        bool converged = model_update( model, ds );
        if (eof( input ))
        {
            fseek(fp, 0, SEEK_SET);
            i++;
        }
        io_res_destroy( input ); 
        dataset_destroy( ds );
        if (converged)
        {
            break;
        }
    }
    model_print_values(model);
    model_destroy( model );
    fclose(fp);
}
