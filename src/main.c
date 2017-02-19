#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include "../include/types.h"
#include "../include/dataset.h"
#include "../include/matrix.h"
#include "../include/train.h"

int main()
{
    matrix_ptr x = matrix_random(5,5);
    matrix_print(x);
//    train( "data/testy.csv", ',', 1, 2, 3 );
}
