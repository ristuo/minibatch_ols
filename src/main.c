#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include "../include/types.h"
#include "../include/matrix.h"

int main()
{
    matrix_ptr x = matrix_from_batch("data/testy.csv", ',', 2, 0);
    matrix_ptr summa = transpose(scale(3.3,x));
    matrix_print(x);
    matrix_print(summa);
}
