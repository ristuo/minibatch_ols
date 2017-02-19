#ifndef MATRIX_H
#define MATRIX_H
#include "../include/types.h"
typedef struct matrix* matrix_ptr;

void matrix_destroy( matrix_ptr x);

index nrow( matrix_ptr );

index ncol( matrix_ptr );

number element_at( matrix_ptr, index i, index j );

matrix_ptr matrix_create( number **, index, index );

matrix_ptr matrix_from_batch( char* file_path
                            , char sep
                            , index
                            , index );

matrix_ptr scale( number, matrix_ptr );

matrix_ptr copy( matrix_ptr );

matrix_ptr matrix_random( index nrow, index ncol );

matrix_ptr matrix_zeros( index nrow, index ncol );

matrix_ptr matrix_id( index size );

matrix_ptr multiply( matrix_ptr, matrix_ptr );

matrix_ptr sum( matrix_ptr, matrix_ptr );

matrix_ptr minus( matrix_ptr, matrix_ptr );

matrix_ptr transpose( matrix_ptr );

void matrix_print( matrix_ptr );
#endif
