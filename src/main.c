#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <time.h>
#include "../include/types.h"
#include "../include/dataset.h"
#include "../include/matrix.h"
#include "../include/train.h"
#include "../include/model.h"

int main()
{

/*
    matrix_ptr x = matrix_random(2,2);
    matrix_print(x);
    matrix_ptr xt = transpose(x);
    printf("xt\n");
    matrix_print(xt);
    matrix_ptr xtx = multiply(xt, x);
    matrix_print(xtx);
*/
    srand(time(NULL));            
    srand(1);
    model_ptr mod = model_create();
    dataset_ptr ds = dataset_from_batch( "data/iris.csv"
                                       , ','
                                       , 200
                                       , 0
                                       , 1 );
    for (int i = 0; i < 15; i++)
    {
        model_update( mod, ds );
    }
    model_print( mod );
    model_destroy( mod );
    dataset_destroy( ds );
}
