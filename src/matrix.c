#include "../include/matrix.h"
#include "../include/io.h"
#include "../include/types.h"
#include "../include/util.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
struct matrix {
  index nrow;
  index ncol;
  number **values;
};

number **get_mem(index nrow, index ncol) {
  number **res = malloc(sizeof(number *) * nrow);
  if (!res) {
    printf("Memory allocation failed at matrix operation!\n");
    exit(EXIT_FAILURE);
  }
  for (index i = 0; i < nrow; i++) {
    number *x = malloc(sizeof(number) * ncol);
    if (!x) {
      printf("Memory allocation failed!");
      exit(EXIT_FAILURE);
    }
    res[i] = x;
  }
  return res;
}

matrix_ptr matrix_create(number **values, index nrow, index ncol) {
  matrix_ptr res = malloc(sizeof(struct matrix));
  res->nrow = nrow;
  res->ncol = ncol;
  res->values = values;
  return res;
}

void matrix_destroy(matrix_ptr x) {
  for (index i = 0; i < x->nrow; i++) {
    free(x->values[i]);
  }
  free(x->values);
  free(x);
}

matrix_ptr cbind(matrix_ptr a, matrix_ptr b) {
#ifndef RELEASE
  assert(a->nrow == b->nrow);
#endif
  index cols = a->ncol + b->ncol;
  number **res = get_mem(a->nrow, cols);
  for (index i = 0; i < a->nrow; i++) {
    for (index j = 0; j < cols; j++) {
      if (j < a->ncol) {
        res[i][j] = a->values[i][j];
      } else {
        res[i][j] = b->values[i][j - a->ncol];
      }
    }
  }
  return matrix_create(res, a->nrow, cols);
}

matrix_ptr matrix_ones(index nrow, index ncol) {
  number **res = get_mem(nrow, ncol);
  for (index i = 0; i < nrow; i++) {
    for (index j = 0; j < ncol; j++) {
      res[i][j] = 1;
    }
  }
  return matrix_create(res, nrow, ncol);
}

index nrow(matrix_ptr x) { return x->nrow; }

index ncol(matrix_ptr x) { return x->ncol; }

matrix_ptr matrix_from_batch(char *file_path, char separator, index nrow,
                             index offset) {
  io_res_ptr input = read_batch(file_path, separator, nrow, offset);
  return matrix_create(values(input), rows_read(input), cols_read(input));
}

matrix_ptr copy(matrix_ptr x) {
  number **values = get_mem(x->nrow, x->ncol);
  for (index i = 0; i < x->nrow; i++) {
    for (index j = 0; j < x->ncol; j++) {
      values[i][j] = x->values[i][j];
    }
  }
  return matrix_create(values, x->nrow, x->ncol);
}

matrix_ptr scale(number c, matrix_ptr x) {
  number **values = get_mem(x->nrow, x->ncol);
  for (index i = 0; i < x->nrow; i++) {
    for (index j = 0; j < x->ncol; j++) {
      values[i][j] = c * x->values[i][j];
    }
  }
  return matrix_create(values, x->nrow, x->ncol);
}

void matrix_print(matrix_ptr x) {
  printf("%lux%lu Matrix\n", x->nrow, x->ncol);
  index rows_to_print = ind_min(10, x->nrow);
  index cols_to_print = ind_min(5, x->ncol);
  for (index i = 0; i < rows_to_print; i++) {
    for (index j = 0; j < cols_to_print; j++) {
      printf("%14.3f", x->values[i][j]);
    }
    if (cols_to_print < x->ncol) {
      printf("%15s", "...");
    }
    printf("\n");
  }
  if (rows_to_print < x->nrow) {
    if (x->ncol > cols_to_print) {
      cols_to_print++;
    }
    for (index i = 0; i < cols_to_print; i++) {
      printf("%15s", "...");
    }
    printf("\n");
  }
}

number element_at(matrix_ptr a, index i, index j) { return a->values[i][j]; }

matrix_ptr multiply(matrix_ptr a, matrix_ptr b) {
#ifndef RELEASE
  assert(a->ncol == b->nrow);
#endif
  index nrow = a->nrow;
  index ncol = b->ncol;
  number **res = get_mem(nrow, ncol);
  for (index i = 0; i < nrow; i++) {
    for (index j = 0; j < ncol; j++) {
      number accu = 0;
      for (index k = 0; k < b->nrow; k++) {
        accu += a->values[i][k] * b->values[k][j];
      }
      res[i][j] = accu;
    }
  }
  return matrix_create(res, nrow, ncol);
}

matrix_ptr sum(matrix_ptr a, matrix_ptr b) {
#ifndef RELEASE
  assert(a->nrow == b->nrow);
  assert(a->ncol == b->ncol);
#endif
  number **res = get_mem(a->nrow, a->ncol);
  for (index i = 0; i < a->nrow; i++) {
    for (index j = 0; j < a->ncol; j++) {
      res[i][j] = a->values[i][j] + b->values[i][j];
    }
  }
  return matrix_create(res, a->nrow, a->ncol);
}

matrix_ptr transpose(matrix_ptr a) {
  number **res = get_mem(a->ncol, a->nrow);
  for (index j = 0; j < a->ncol; j++) {
    for (index i = 0; i < a->nrow; i++) {
      res[j][i] = a->values[i][j];
    }
  }
  return matrix_create(res, a->ncol, a->nrow);
}

matrix_ptr matrix_id(index size) {
  number **res = get_mem(size, size);
  for (index i = 0; i < size; i++) {
    for (index j = 0; j < size; j++) {
      number x;
      if (i == j) {
        x = 1;
      } else {
        x = 0;
      }
      res[i][j] = x;
    }
  }
  return matrix_create(res, size, size);
}

matrix_ptr minus(matrix_ptr a, matrix_ptr b) {
#ifndef RELEASE
  assert(a->ncol == b->ncol);
  assert(a->nrow == b->nrow);
#endif
  number **res = get_mem(a->nrow, a->ncol);
  for (index i = 0; i < a->nrow; i++) {
    for (index j = 0; j < a->ncol; j++) {
      res[i][j] = a->values[i][j] - b->values[i][j];
    }
  }
  return matrix_create(res, a->nrow, a->ncol);
}

matrix_ptr matrix_zeros(index nrow, index ncol) {
  number **res = get_mem(nrow, ncol);
  for (index i = 0; i < nrow; i++) {
    for (index j = 0; j < ncol; j++) {
      res[i][j] = 0;
    }
  }
  return matrix_create(res, nrow, ncol);
}

matrix_ptr matrix_random(index nrow, index ncol) {
  number **res = get_mem(nrow, ncol);
  for (index i = 0; i < nrow; i++) {
    for (index j = 0; j < ncol; j++) {
      number r = (number)(rand() % 1300) / 1300;
      res[i][j] = r;
    }
  }
  return matrix_create(res, nrow, ncol);
}
