#include "../include/util.h"
#include "../include/types.h"
index ind_min(index a, index b) {
  if (a < b) {
    return a;
  }
  return b;
}
number num_abs(number a) {
  if (a < 0) {
    return -a;
  }
  return a;
}
