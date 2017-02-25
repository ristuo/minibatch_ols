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
    index reps = 10;
    for (index i = 0; i < reps; i++)
    {
        train( "data/iris.csv"
             , ','
             , 1
             , 10000
             , 100 );
    }
}
