#include "../include/matrix.h"
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#define MAX_CHARS_IN_FLOAT 25
int min(int a, int b)
{
    if (a < b)
    {
        return a;
    }
    return b;
}

void print_matrix(matrix* a)
{
    printf("%lux%lu Matrix\n", a->nrow, a->ncol);
    unsigned int rows_to_print = min(10, a->nrow);
    unsigned int cols_to_print = min(5, a->ncol);
    for (unsigned int i = 0; i < rows_to_print; i++) 
    {
        for (unsigned int j = 0; j < cols_to_print; j++)
        {
            printf("%15.2f", a->vals[i][j]);
        }
        if (cols_to_print < a->ncol)
        {
            printf("%15s", "...");
        }
        printf("\n");
    }
    if (rows_to_print < a->nrow)
    {
        if (a->ncol > cols_to_print)
        {
            cols_to_print++;
        }
        for (unsigned int i = 0; i < cols_to_print; i++)
        {
            printf("%15s", "...");
        }
        printf("\n");
    }
}

matrix multiply(matrix* a, matrix* b)
{
    #ifndef RELEASE
        assert(a->ncol == b->nrow); 
    #endif
    unsigned long nrow = b->ncol;
    unsigned long ncol = a->nrow;
    vec* res = malloc(sizeof(number*)*nrow);
    if (!res)
    {
        printf("Memory allocation failed!");
        exit(EXIT_FAILURE);
    }
    for (unsigned int i = 0; i < nrow; i++) 
    {
        vec new_row = malloc(sizeof(number) * ncol);
        res[i] = new_row;
        if (!new_row)
        {
            printf("Memory allocation failed!");
            exit(EXIT_FAILURE);
        }
        for (unsigned int j = 0; j < ncol; j++)
        {
            number accu = 0;
            for (unsigned int k = 0; k < b->nrow; k++)
            {
                accu += a->vals[i][k] * b->vals[k][j];
            }
            res[i][j] = accu; 
        }
    }
    matrix m;
    m.nrow = nrow;
    m.ncol = ncol;
    m.vals = res;
    return m;
}

matrix transpose(matrix* a) 
{
    unsigned long nrow = a->ncol;
    unsigned long ncol = a->nrow;
    vec* vals = malloc(sizeof(number*) * nrow);
    for (unsigned int i = 0; i < nrow; i++) 
    {
        number* new_row = malloc(sizeof(number)*ncol);
        vals[i] = new_row;
        for (unsigned int j = 0; j < ncol; j++) 
        {
            vals[i][j] = a->vals[j][i];
        }
    }
    matrix m;
    m.nrow = nrow;
    m.ncol = ncol;
    m.vals = vals;
    return m;
}

unsigned long laske_numerot(char separator, char* line, ssize_t length)
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



vec create_vec( unsigned long length )
{
    vec res = malloc( sizeof(number) * length);
    if (!res) 
    {
        printf("Error in allocating memory\n");
        exit(EXIT_FAILURE);
    }
    for (unsigned int i = 0; i < length; i++)
    {
        res[i] = 0;
    }
    return res;
}

float_in read_float( char separator, char* line, ssize_t llength, int start )
{
    static char float_char[MAX_CHARS_IN_FLOAT];
    for (int ii = 0; ii < MAX_CHARS_IN_FLOAT; ii++) 
    {
        float_char[ii] = '\0';
    }
    int i = start;
    int h = 0;
    float_in res;
    while (i < llength)
    {
        if (line[i] != separator && line[i] != '\n') 
        {
            float_char[h] = line[i]; 
            h++;
        }
        if (line[i] == separator || i == (llength - 1))
        {
            res.result = strtof(float_char, NULL);
            res.bytes_read = h;
            break;
        }
        i++;
    }
    if (res.bytes_read == 0)
    {
        printf( "Error at line:\n\t%sposition %d, character \"%c\"\n"
              , line
              , start
              , line[start] );
        exit(EXIT_FAILURE);
    }
    return res;
}

vector read_row(char separator, char* line, ssize_t bytes)
{
    unsigned long numeroita = laske_numerot(',', line, bytes);
    vector res;
    res.length = numeroita;
    vec new_row = create_vec( numeroita );
    int j = 0;
    int i = 0;
    while (i < bytes)
    {
        float_in fin = read_float(separator, line, bytes, i);
        i += fin.bytes_read + 1;
        float value = fin.result;
        new_row[j] = value; 
        j++;
    }
    res.vals = new_row;
    return res;
}


ds_io_result read_batch( FILE* fp
                       , char separator
                       , long offset
                       , long nrow
                       , long target_index )
{
    if (fseek(fp, offset, SEEK_SET))
    {
        printf("Problem in reading the file");
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    ds_io_result res;
    matrix mat = create_matrix_rows( nrow );
    matrix target = create_matrix( nrow, 1 );
    long ncol = 0;
    long k = 0;
    long bytes_read = 0;
    ssize_t read;
    char* line = NULL; 
    size_t line_length = 0;
    while (k < nrow && (read = getline(&line, &line_length, fp)) != -1) 
    {
        bytes_read += read;
        dataset_row new_row = read_ds_row(separator, line, read, target_index);
        unsigned long numeroita = new_row.var_values.length;
        if (k == 0)
        {
            ncol = numeroita - 1;
            mat.ncol = ncol;
        }
        else 
        {
            if (mat.ncol != numeroita - 1)
            {
                printf("differing numbers of columns! ");
                printf("First row had ");
                printf("%lu cols, row %lu had %lu!\n", ncol, k, numeroita);
                exit(EXIT_FAILURE);
            }
        }
        #if DEBUGLVL > 5
            printf( "reading line %lu which has %lu values\n"
                  , k
                  , numeroita);
        #endif
        mat.vals[k] = new_row.var_values.vals;
        target.vals[k] = new_row.target_value.vals;
        k++;
    }
    mat.nrow = k;
    target.nrow = k;
    dataset ds;
    ds.target = target;
    ds.data = mat;
    res.ds = ds;
    if (read == -1)
    {
        res.eof = 1;
    }
    else
    {
        res.eof = 0;
    }
    res.bytes_read = bytes_read;
    return res;
}

dataset_row read_ds_row( char separator
                       , char* line
                       , ssize_t bytes
                       , unsigned long target_index )
{
    dataset_row res;
    unsigned long numeroita = laske_numerot(',', line, bytes);
    vector var_values;
    var_values.length = numeroita;
    var_values.vals = create_vec( numeroita );
    vector target;
    target.length = 1;
    target.vals = create_vec( 1 );
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int index_of_values = 0;
    while (i < bytes)
    {
        float_in fin = read_float(separator, line, bytes, i);
        i += fin.bytes_read + 1;
        if (j == target_index)
        {
            target.vals[0] = fin.result;
        }
        else
        {
            var_values.vals[index_of_values] = fin.result;
            index_of_values++;
        }
        j++;
    }
    res.target_value = target;
    res.var_values = var_values;
    return res;
}

matrix create_matrix( unsigned long nrow, unsigned long ncol)
{
    matrix res;
    res.nrow = nrow;
    res.ncol = ncol;
    res.vals = malloc( sizeof(number) * nrow * ncol ); 
    if (!res.vals)
    {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

void destroy( matrix* mat ) 
{
    for (unsigned long i = 0; i < mat->nrow; i++)
    {
        free(mat->vals[i]);
    }
    free(mat->vals);
}

matrix create_matrix_rows( unsigned long nrow )
{
    matrix res;
    res.nrow = nrow;
    res.ncol = 0;
    res.vals = malloc( sizeof(number*) * nrow );
    if (!res.vals)
    {
        printf("Memory allocation failed!");
        exit(EXIT_FAILURE);
    }
    return res;
}

dataset data_from_csv( char* path
                     , long* bytes_read
                     , long offset
                     , long nrow
                     , long target_index )
{
    FILE* fp = fopen(path, "r");       
    if (!fp)
    {
        printf("reading file %s failed", path);
        exit(EXIT_FAILURE);
    }
    ds_io_result res = read_batch( fp
                                 , ','
                                 , offset
                                 , nrow
                                 , target_index );
    *bytes_read += res.bytes_read;
    return res.ds;
}

void print_dataset(dataset* ds)
{
    printf("Independent variables:\n");
    print_matrix(&(ds->data));
    printf("Dependent variable:\n");
    print_matrix(&(ds->target));
}

matrix from_csv(char* path, long* bytes_read, long offset, long nrow)
{
    char* line = NULL; 
    size_t line_length = 0;
    ssize_t read = 0;
    FILE* fp = fopen(path, "r"); 
    if (fseek(fp, offset, SEEK_SET))
    {
        printf("Problem in reading the file");
        exit(EXIT_FAILURE);
    }
    if (!fp)
    {
        printf("reading file %s failed", path);
        exit(EXIT_FAILURE);
    }
    vec* vals = malloc(sizeof(number*) * nrow);
    if (!vals)
    {
        printf("Memory allocation failed!");
        exit(EXIT_FAILURE);
    }
    long k = 0;
    matrix mat;
    unsigned long ncol = 0;
    while (k < nrow && (read = getline(&line, &line_length, fp)) != -1) 
    {
        *bytes_read += read;
        vector new_row = read_row(',', line, read);
        unsigned long numeroita = new_row.length;
        if (k == 0)
        {
            ncol = numeroita;
        }
        else 
        {
            if (ncol != numeroita)
            {
                printf("differing numbers of columns! ");
                printf("First row had ");
                printf("%lu cols, row %lu had %lu!\n", ncol, k, numeroita);
                exit(EXIT_FAILURE);
            }
        }
        #if DEBUGLVL > 5
            printf( "reading line %lu which has %lu values\n"
                  , k
                  , numeroita);
        #endif
        vals[k] = new_row.vals;
        mat.ncol = ncol;
        k++;
    }
    fclose(fp);
    mat.nrow = k;
    mat.vals = vals;
    return mat;
}

matrix minus( matrix* a, matrix* b)
{
    #ifndef RELEASE
        assert(a->nrow == b->nrow);
        assert(b->ncol == b->ncol);
    #endif
    matrix res = create_matrix(a->nrow, a->ncol);
    for (unsigned long i = 0; i < a->nrow; i++)
    {
        for (unsigned long j = 0; j < a->ncol; j++) 
        {
            res.vals[i][j] = a->vals[i][j] - b->vals[i][j]; 
        }
    }
    return res;
}
