#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../include/types.h"
#include "../include/matrix.h"
#include "../include/matrix_list.h"

struct matrix_list
{
    matrix_ptr* matrices;
    index size; 
    index capacity;
};

matrix_list_ptr matrix_list_create(void)
{
    matrix_list_ptr res = malloc(sizeof(struct matrix_list));
    if (!res)
    {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE); 
    }
    res->matrices = NULL;
    res->size = 0;
    res->capacity = 0;
    return res;
}

index size( matrix_list_ptr ml )
{
    return ml->size;
}

index capacity( matrix_list_ptr ml )
{
    return ml->capacity;
}

void push( matrix_list_ptr ml, matrix_ptr a )
{
    if (ml->size >= ml->capacity)
    {
        ml->matrices = realloc( ml->matrices
                              , sizeof(matrix_ptr) * (ml->size + 1));
        if (!ml->matrices)
        {
            printf("Memory allocation failed!\n");
            exit(EXIT_FAILURE);
        }
        ml->capacity += 1;
    }
    ml->matrices[ ml->size ] = a;
    ml->size += 1;
}

void reserve( matrix_list_ptr ml, index new_size )
{
    if (ml->capacity < new_size)
    {
        ml->matrices = realloc( ml->matrices
                              , sizeof(matrix_ptr) * ( new_size ));
        if (!ml->matrices)
        {
            printf("Memory allocation failed!\n");
            exit(EXIT_FAILURE);
        }
    }
    ml->capacity = new_size;
}

matrix_ptr matrix_at( matrix_list_ptr ml, index i )
{
    #ifndef RELEASE
        assert(i < ml->size);
    #endif
    return ml->matrices[ ml->size - i - 1 ];
}
