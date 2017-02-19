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
    srand(time(NULL));            
    model_ptr mod = model_create();
    dataset_ptr ds = dataset_from_batch( "data/iris.csv"
                                       , ','
                                       , 10
                                       , 0
                                       , 1 );
    model_update( mod, ds );
    model_destroy( mod );
//train( "data/testy.csv", ',', 1, 2, 3 );
}
