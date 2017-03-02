#include <stdlib.h>
#include <sys/types.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "../include/types.h"
#include "../include/io.h"
#define MAX_CHARS_IN_FLOAT 50
struct io_res
{
    number** values;
    char* path;
    index rows_read;
    index cols_read;
    index bytes_read;
    bool eof;
};

void io_res_destroy( io_res_ptr input )
{
    for (index i = 0; i < input->rows_read; i++)
    {
        free(input->values[i]);
    }
    free(input->values);
    free(input);
}

bool eof( io_res_ptr x )
{
    return x->eof;
}

index rows_read( io_res_ptr res )
{
    return res->rows_read;
}

index cols_read( io_res_ptr res )
{
    return res->cols_read;
}

char* path( io_res_ptr res )
{
    return res->path;
}

index bytes_read( io_res_ptr res )
{
    return res->bytes_read;
}

number** values( io_res_ptr res )
{
    return res->values;
}

index count_numbers(char separator, char* line, ssize_t length)
{
    unsigned long res = 0;
    if (length == 0)
    {
        return res;
    }
    for (int i = 0; i < length; i++)
    {
        if (line[i] == separator)
        {
            res++;
        }
    }
    res++;
    return res;
}

float read_float( index* bytes_read 
                , char separator
                , char* line
                , ssize_t llength
                , int start )
{
    static char float_char[MAX_CHARS_IN_FLOAT];
    for (int i = 0; i < MAX_CHARS_IN_FLOAT; i++) 
    {
        float_char[i] = '\0';
    }
    int i = start;
    int h = 0;
    float res;
    while (i < llength)
    {
        if (line[i] != separator && line[i] != '\n') 
        {
            float_char[h] = line[i]; 
            h++;
        }
        if (line[i] == separator || i == (llength - 1))
        {
            res = strtof(float_char, NULL);
            *bytes_read = h;
            break;
        }
        i++;
    }
    if (*bytes_read == 0)
    {
        printf( "Error at line:\n\t%s\n\t", line );
        for (int k = 0; k < start; k++)
        {
            printf(" ");
        }
        printf("^\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

number* read_row(index* length, char separator, char* line, ssize_t bytes)
{
    *length = count_numbers(separator, line, bytes);
    number* new_row = malloc(sizeof(number) * (*length));
    if (!new_row)
    {
        printf("Failed to allocate memory!");
        exit(EXIT_FAILURE);
    }
    index j = 0;
    ssize_t i = 0;
    while (i < bytes)
    {
        index bytes_read;
        float fin = read_float(&bytes_read, separator, line, bytes, i);
        i += bytes_read + 1;
        new_row[j] = fin; 
        j++;
    }
    return new_row;
}

io_res_ptr read_batch_fp( FILE* fp
                        , char separator
                        , index nrow
                        , char* file_path )
{
    number** values = malloc( sizeof(number*) * nrow ); 
    if (!values)
    {
        printf("Failed to allocate memory!");
        exit(EXIT_FAILURE);
    }
    index bytes_read = 0;
    index ncol;
    index row_length;
    char* line = NULL; 
    size_t line_length = 0;
    ssize_t read = 0;
    index k = 0;
    while (k < nrow && (read = getline( &line, &line_length, fp)) != -1)
    {
        bytes_read += read;
        number* row = read_row( &row_length, separator, line, read );
        if (k == 0)
        {
            ncol = row_length;
        }
        else
        {
            if (ncol != row_length)
            {
                printf( "error: row %lu had %lu cols while first row had %lu!\n"
                      , k
                      , row_length 
                      , ncol );
                exit(EXIT_FAILURE);
            }
        }
        values[k] = row;
        k++;
    }
    io_res_ptr res = malloc(sizeof( struct io_res ));
    res->values = values;
    res->cols_read = ncol;
    res->rows_read = k;
    res->bytes_read = bytes_read;
    res->path = file_path;
    res->eof = k < nrow || getline( &line, &line_length, fp) == -1;
    free(line);
    return res;
}

io_res_ptr read_batch( char* file_path
                     , char separator
                     , index nrow
                     , index offset )
{
    #ifdef DEBUG
        printf( "Starting to read a file at offset %lu\n"
              , offset );
    #endif
    FILE* fp = fopen(file_path, "r");
    if (!fp)
    {
        printf("Failure to open the file %s", file_path);
        exit(EXIT_FAILURE);
    }
    if (fseek(fp, offset, SEEK_SET))
    {
        printf("Failure in reading the file %s", file_path);
        exit(EXIT_FAILURE);
    }
    io_res_ptr res = read_batch_fp( fp, separator, nrow, file_path );
    #ifdef DEBUG
        if (res->eof) 
        {
            printf("Reached end of file while reading!\n");
        }
        else 
        {
            printf("No EOF here\n");
        }
        printf( "Read %lu rows, having %lu bytes\n"
              , k
              , bytes_read );
    #endif
    fclose(fp);
    return res;
}

void io_res_print( io_res_ptr x )
{
    printf( "Read %lu rows and %lu columns from file %s\n"
          , x->rows_read
          , x->cols_read
          , x->path);
}
