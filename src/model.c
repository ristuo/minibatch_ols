#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/matrix.h"
#include "../include/dataset.h"
#include "../include/model.h"
#include "../include/list.h"

struct model
{
    matrix_ptr beta;
    LIST(beta_history, matrix_ptr)
    LIST(g_history, matrix_ptr)
    LIST(value_history, float)
};

void model_destroy( model_ptr model )
{
    if (model->beta)
    {
        matrix_destroy( model->beta );
    }
    for (index i = 0; i < model->beta_history.size; i++)
    {
        matrix_destroy( AT(model->beta_history,i) );
    }
    free(model->beta_history.values);
    for (index i = 0; i < model->g_history.size; i++)
    {
        matrix_destroy( AT(model->g_history,i) );
    }
    free(model->g_history.values);
    free(model->value_history.values);
    free(model);
}

model_ptr model_create(void)
{
    model_ptr res = malloc( sizeof(struct model) );
    if (!res)
    {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    res->beta = NULL;
    INIT(res->beta_history)
    INIT(res->g_history)
    INIT(res->value_history)
    return res;
}

number bb_stepsize( model_ptr model )
{
    if (model->beta_history.size <= 2)
    {
        return 0.0000001;
    }

    matrix_ptr g_change = minus( AT(model->g_history,0)
                               , AT(model->g_history, 1));
    matrix_ptr beta_change = minus( AT(model->beta_history, 0)
                                  , AT(model->beta_history, 1) );
    matrix_ptr beta_change_t = transpose( beta_change );
    matrix_ptr btb = multiply( beta_change_t, beta_change );
    matrix_ptr bts = multiply( beta_change_t, g_change );
    matrix_destroy( beta_change_t );
    matrix_destroy( g_change );
    matrix_destroy( beta_change );
    printf("btb:\n");
    matrix_print(btb);
    number res = element_at(btb, 0, 0) / element_at(bts, 0, 0);
    matrix_destroy(btb);
    matrix_destroy(bts);
    return res;
}

matrix_ptr gradient( dataset_ptr ds, model_ptr model )
{
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

void model_update( model_ptr model, dataset_ptr ds )
{
    if (!model->beta)
    {
        model->beta = matrix_random( ncol(independent(ds)), 1 );
    }
    else
    {
        PUSH(model->beta_history, matrix_ptr, model->beta);
        matrix_ptr g = gradient( ds, model );
        PUSH(model->g_history, matrix_ptr, g);
        number stepsize = bb_stepsize( model );
        matrix_ptr scaled = scale(stepsize, g);
        model->beta = minus(model->beta, scaled);
        matrix_destroy(scaled);
    }
}

void model_print( model_ptr model )
{
    matrix_print(model->beta);
    for (index i = 0; i < model->beta_history.size; i++)
    {
        printf("beta_%lu\n", i+1);
        matrix_print(model->beta_history.values[i]);
    }
}
