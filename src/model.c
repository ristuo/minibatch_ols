#include "../include/model.h"
#include "../include/dataset.h"
#include "../include/list.h"
#include "../include/matrix.h"
#include "../include/util.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
struct stop_cond {
  stop_type type;
  number treshold;
};

struct model {
  matrix_ptr beta;
  stop_cond_ptr stop;
  LIST(beta_history, matrix_ptr)
  LIST(g_history, matrix_ptr)
  LIST(value_history, float)
};

stop_cond_ptr stop_cond_create(stop_type type, number treshold) {
  stop_cond_ptr res = malloc(sizeof(struct stop_cond));
  res->type = type;
  res->treshold = treshold;
  return res;
}

bool should_stop(model_ptr model) {
  stop_cond_ptr stop = model->stop;
  if (stop->type == ITERATIONS) {
    return stop->treshold < iterations_run(model);
  }
  if (stop->type == CONVERGENCE) {
    if (model->value_history.size < 2) {
      return false;
    }
    number cost1 = AT(model->value_history, 1);
    number cost2 = AT(model->value_history, 0);
    number cost_change = cost1 - cost2;
    return (num_abs(cost_change) < stop->treshold);
  }
  assert(false);
  return true;
}

void stop_cond_destroy(stop_cond_ptr x) { free(x); }

index iterations_run(model_ptr model) { return model->value_history.size; }

void model_destroy(model_ptr model) {
  stop_cond_destroy(model->stop);
  if (model->beta) {
    matrix_destroy(model->beta);
  }
  for (index i = 0; i < model->beta_history.size; i++) {
    matrix_destroy(AT(model->beta_history, i));
  }
  free(model->beta_history.values);
  for (index i = 0; i < model->g_history.size; i++) {
    matrix_destroy(AT(model->g_history, i));
  }
  free(model->g_history.values);
  free(model->value_history.values);
  free(model);
}

model_ptr model_create(stop_cond_ptr x) {
  model_ptr res = malloc(sizeof(struct model));
  if (!res) {
    printf("Memory allocation failed!\n");
    exit(EXIT_FAILURE);
  }
  res->beta = NULL;
  res->stop = x;
  INIT(res->beta_history)
  INIT(res->g_history)
  INIT(res->value_history)
  return res;
}

number bb_stepsize(model_ptr model) {
  if (model->beta_history.size <= 2) {
    return 0.0000001;
  }

  matrix_ptr g_change = minus(AT(model->g_history, 0), AT(model->g_history, 1));
  matrix_ptr beta_change =
      minus(AT(model->beta_history, 0), AT(model->beta_history, 1));
  matrix_ptr beta_change_t = transpose(beta_change);
  matrix_ptr btb = multiply(beta_change_t, beta_change);
  matrix_ptr bts = multiply(beta_change_t, g_change);
  matrix_destroy(beta_change_t);
  matrix_destroy(g_change);
  matrix_destroy(beta_change);
  number res = element_at(btb, 0, 0) / element_at(bts, 0, 0);
  matrix_destroy(btb);
  matrix_destroy(bts);
  if (isnan(res)) {
    return 0.00001;
  }
  return res;
}

matrix_ptr gradient(dataset_ptr ds, model_ptr model) {
  matrix_ptr beta = model->beta;
  matrix_ptr x = independent(ds);
  matrix_ptr y = dependent(ds);
  matrix_ptr x_transpose = transpose(x);
  matrix_ptr x_transpose_y = multiply(x_transpose, y);
  matrix_ptr x_transpose_x = multiply(x_transpose, x);
  matrix_ptr xtx_beta = multiply(x_transpose_x, beta);
  matrix_ptr res = minus(xtx_beta, x_transpose_y);
  matrix_destroy(x_transpose);
  matrix_destroy(x_transpose_y);
  matrix_destroy(x_transpose_x);
  matrix_destroy(xtx_beta);
  return res;
}

number cost(dataset_ptr ds, model_ptr model) {
  matrix_ptr x = independent(ds);
  matrix_ptr y = dependent(ds);
  matrix_ptr beta = model->beta;
  matrix_ptr xtb = multiply(x, beta);
  matrix_ptr y_minus_xtb = minus(y, xtb);
  matrix_ptr y_minus_xtb_t = transpose(y_minus_xtb);
  matrix_ptr res = multiply(y_minus_xtb_t, y_minus_xtb);
  number result = element_at(res, 0, 0);
  matrix_destroy(res);
  matrix_destroy(y_minus_xtb_t);
  matrix_destroy(y_minus_xtb);
  matrix_destroy(xtb);
  return result;
}

dataset_ptr append_ones(dataset_ptr ds) {
  matrix_ptr ind = independent(ds);
  matrix_ptr ones = matrix_ones(nrow(ind), 1);
  matrix_ptr new_ind = cbind(ones, ind);
  matrix_ptr new_dep = copy(dependent(ds));
  matrix_destroy(ones);
  return dataset_create_mat(new_ind, new_dep);
}

bool model_update(model_ptr model, dataset_ptr dset) {
  dataset_ptr ds = append_ones(dset);
  if (!model->beta) {
    model->beta = matrix_random(ncol(independent(ds)), 1);
    number cost_at_beta = cost(ds, model);
    PUSH(model->value_history, number, cost_at_beta);
    dataset_destroy(ds);
    return false;
  } else {
    PUSH(model->beta_history, matrix_ptr, model->beta);
    matrix_ptr g = gradient(ds, model);
    PUSH(model->g_history, matrix_ptr, g);
    // number stepsize = bb_stepsize( model );
    number stepsize = 0.000001;
    matrix_ptr scaled = scale(stepsize, g);
    model->beta = minus(model->beta, scaled);
    number cost_at_beta = cost(ds, model);
    PUSH(model->value_history, number, cost_at_beta);
    matrix_destroy(scaled);
    dataset_destroy(ds);
    return (should_stop(model));
  }
}

void model_print(model_ptr model) {
  printf("After %lu iterations, cost is %f\n", model->value_history.size,
         AT(model->value_history, 0));
}

void model_print_coeff(model_ptr model) { matrix_print(model->beta); }

void model_print_values(model_ptr model) {
  index size = model->value_history.size;
  for (index i = 0; i < size - 1; i++) {
    printf("%f,", model->value_history.values[i]);
  }
  printf("%f\n", model->value_history.values[size - 1]);
}
