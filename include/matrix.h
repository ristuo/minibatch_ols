#ifndef MATRIXH409
#define MATRIXH409
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
typedef float number;
typedef number* vec;
typedef struct 
{
    unsigned long nrow;
    unsigned long ncol;
    vec* vals;

} matrix;

typedef struct
{
    matrix target; 
    matrix data;
} dataset;

typedef struct
{
    unsigned long length; 
    vec vals; 
} vector;

typedef struct
{
    int eof;
    long bytes_read;
    dataset ds;
} ds_io_result;

typedef struct
{
    vector target_value;
    vector var_values;
} dataset_row;

typedef struct
{
    int bytes_read;
    float result;
} float_in;
   

void print_matrix(matrix* a);
ds_io_result read_batch( FILE* fp
                       , char separator
                       , long offset
                       , long nrow
                       , long target_index );
void print_dataset(dataset* ds);
void destroy( matrix* mat );
matrix create_matrix( unsigned long nrow, unsigned long ncol );
matrix create_matrix_rows( unsigned long nrow );
vec create_vec( unsigned long length );
vector read_row(char separator, char* line, ssize_t bytes);
dataset_row read_ds_row( char separator
                       , char* line
                       , ssize_t bytes
                       , unsigned long target_index );
matrix multiply(matrix* a, matrix* b);
matrix transpose(matrix* a);
matrix minus(matrix* a, matrix* b);
matrix from_csv(char* path, long* bytes_read, long offset, long nrow);
dataset data_from_csv( char* path   
                     , long* bytes_read
                     , long offset
                     , long nrow
                     , long target_index );
#endif
