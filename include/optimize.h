#ifndef OPTIMIZE1321
#define OPTIMIZE1321
#include "../include/matrix.h"
#include <assert.h>
typedef number (*cost_function)(matrix x);
typedef matrix (*gradient)(matrix x, matrix y, matrix beta);
typedef number (*stepsize_function)(matrix xk, matrix xk_minus_1, gradient g);

matrix ols_gradient(matrix *x, matrix *y, matrix *beta) {
#ifndef RELEASE
  assert(x->ncol == beta->nrow);
  assert(y->nrow == x->nrow);
  assert(y->ncol == 1);
  assert(beta->ncol == 1);
#endif
  matrix xt = transpose(x);
  matrix xtx = multiply(&xt, &xt);
  matrix xtxb = multiply(&xtx, beta);
  matrix xty = multiply(&xt, y);
  for (unsigned long i = 0; i < xtxb.nrow; i++) {
    xtxb.vals[i][0] = xtxb.vals[i][0] - xty.vals[i][0];
  }
  return xtxb;
}
#endif

number const_stepsize(matrix xk, matrix xk_minus_1, gradient g) { return 0.01; }

matrix update(stepsize_function mu, matrix xk, gradient g) {}
