#include "../include/dataset.h"
#include "../include/matrix.h"
#include "../include/model.h"
#include "../include/train.h"
#include "../include/types.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

int main() {
  srand(time(NULL));
  index reps = 1;
  for (index i = 0; i < reps; i++) {
    train2("data/iris.csv", ',', 1, 5, 100);
  }
}
