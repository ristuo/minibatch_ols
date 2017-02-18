#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include "../include/dataset.h"
#include "../include/types.h"
#include "../include/io.h"
#include "../include/matrix.h"
struct dataset 
{
    matrix_ptr dependent;
    matrix_ptr independent;
    index nrow;
    index ncol;
};

dataset_ptr dataset_create( number** values
                          , index nrow
                          , index ncol
                          , index target_var_column )
{
    #ifndef RELEASE
        assert(nrow > 1); 
    #endif

    number** dep_values = malloc(sizeof(number*) * nrow);
    number** ind_values = malloc(sizeof(number*) * nrow);
    if (!dep_values || !ind_values)
    {
        printf("Error in allocating memory!\n");
        exit(EXIT_FAILURE);
    }
    index ind_index = 0;
    for (index i = 0; i < nrow; i++)
    {
        number* ind_row = malloc(sizeof(number) * (ncol-1));
        number* dep_row = malloc(sizeof(number));
        if (!ind_row || !dep_row)
        {
            printf("Error in allocating memory!\n");
            exit(EXIT_FAILURE);
        }
        for (index j = 0; j < ncol; j++)
        {
            if (j == target_var_column)
            {
                dep_row[0] = values[i][j];
            }
            else
            {
                ind_row[ind_index] = values[i][j];
                ind_index++;
            }
        }
        dep_values[i] = dep_row;
        ind_values[i] = ind_row;
    }
    matrix_ptr ind = matrix_create( ind_values, nrow, ncol - 1);
    matrix_ptr dep = matrix_create( dep_values, nrow, 1);
    dataset_ptr res = malloc(sizeof(struct dataset));
    if (!res)
    {
        printf("Error in allocating memory!\n");
        exit(EXIT_FAILURE);
    }
    res->dependent = dep;
    res->independent = ind;
    res->nrow = nrow;
    res->ncol = ncol;
    return res;
}

dataset_ptr dataset_from_batch( char* file_path
                              , char separator
                              , index nrow
                              , index offset
                              , index target_var_column )
{
    io_res_ptr io_res = read_batch( file_path
                                  , separator
                                  , nrow
                                  , offset );
    return dataset_create( values(io_res)
                         , rows_read(io_res)
                         , cols_read(io_res)
                         , target_var_column );
}

matrix_ptr dependent( dataset_ptr x )
{
    return x->dependent;
}

matrix_ptr independent( dataset_ptr x )
{
    return x->independent;
}

void dataset_print( dataset_ptr x )
{
    matrix_print(x->independent);
    matrix_print(x->dependent);
}
