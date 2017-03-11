#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "../include/train.h"
#include "../include/types.h"
#include "../include/dataset.h"
#include "../include/io.h"
#include "../include/model.h"

typedef struct
{
    sem_t* producer_sem;
    sem_t* consumer_sem;
    dataset_ptr data;
    bool done;
    bool producer_done;
} shared_data;

void* produce( void* args )
{
    void** arguments = (void**)(args);
    FILE* fp = (FILE*)(arguments[0]);
    char* separator = (char*)(arguments[1]);
    index target_var_column = *( (index*)(arguments[2]) );
    index max_sweeps = *( (index*)(arguments[3]) );
    index* batch_size = (index*)(arguments[4]);
    char* file_path = *((char**)(arguments[5]));
    shared_data* shared = (shared_data*)(arguments[6]);

    sem_t* producer_sem = shared->producer_sem;
    sem_t* consumer_sem = shared->consumer_sem;

    io_res_ptr io_res = read_batch_fp( fp, *separator, *batch_size, file_path );
    dataset_ptr ds = dataset_from_io( io_res 
                                    , target_var_column );
    io_res_destroy( io_res );
    shared->data = ds; 
    sem_post( producer_sem );
    index i = 0;
    while( i < max_sweeps ) 
    {
        io_res = read_batch_fp( fp, *separator, *batch_size, file_path );
        ds = dataset_from_io( io_res, target_var_column );
        sem_wait( consumer_sem );
        dataset_destroy( shared->data );
        shared->data = ds; 
        sem_post( producer_sem );
        if (eof( io_res ))
        {
            fseek(fp, 0, SEEK_SET);
            i++;
            printf("completed sweep %lu\n", i);
        }
        io_res_destroy( io_res );
        if ( shared->done )
        {
            break;
        }
    }
    shared->producer_done = true;
    sem_post(producer_sem);
    return NULL;
}

void* consume( void* args )
{
    void** arguments = (void**)(args);
    shared_data* shared = (shared_data*)(arguments[0]);
    sem_t* producer_sem = shared->producer_sem;
    sem_t* consumer_sem = shared->consumer_sem;
    model_ptr model = (model_ptr)(arguments[1]);
    while( true )
    {
        if (shared->producer_done)
        {
            break;
        }
        sem_wait( producer_sem );
        model_update( model, shared->data );
        shared->done = should_stop( model );
        sem_post( consumer_sem );
    }
    return NULL;
}

void train2( char* file_path
           , char separator
           , index target_var_column
           , index max_sweeps
           , index batch_size )
{
    stop_cond_ptr stop = stop_cond_create( CONVERGENCE, 0.00000001 );
    model_ptr model = model_create( stop );
    FILE* fp = fopen(file_path, "r");
    if (!fp)
    {
        printf("Error in opening file %s!\n", file_path);
        exit(EXIT_FAILURE);
    }
    shared_data shared;
    shared.consumer_sem = malloc( sizeof(sem_t) );
    shared.producer_sem = malloc( sizeof(sem_t) );
    sem_init( shared.consumer_sem, 0, 0 );
    sem_init( shared.producer_sem, 0, 0 );
    shared.done = false; 
    shared.producer_done = false;
    shared.data = NULL;
    pthread_t consumer;
    pthread_t producer;
    void* consumer_args[2];
    consumer_args[0] = (void*)(&shared);
    consumer_args[1] = (void*)(model);

    void* producer_args[7];
    producer_args[0] = (void*)fp;  
    producer_args[1] = (void*)(&separator);
    producer_args[2] = (void*)(&target_var_column);
    producer_args[3] = (void*)(&max_sweeps);
    producer_args[4] = (void*)(&batch_size);
    producer_args[5] = (void*)(&file_path);
    producer_args[6] = (void*)(&shared);

    int cons_start = pthread_create( &consumer
                                   , NULL
                                   , consume
                                   , (void*)(&consumer_args) );
    if (cons_start)
    {
        printf("Error in starting a thread!\n");
        exit(EXIT_FAILURE);
    }
    int prod_start = pthread_create( &producer
                                   , NULL
                                   , produce
                                   , (void*)(producer_args) );
    if (prod_start)
    {
        printf("Error in starting a thread!\n");
        exit(EXIT_FAILURE);
    }
    void** results = malloc(sizeof(NULL));
    pthread_join( producer, results );
    pthread_join( consumer, results );
    //model_print_values(model);
    model_print_coeff(model);
    model_destroy( model );
    fclose(fp);
    free(shared.producer_sem);
    free(shared.consumer_sem);
    dataset_destroy(shared.data);
    free(results);
}

void train( char* file_path
          , char separator
          , index target_var_column
          , int max_sweeps
          , index batch_size )
{
    stop_cond_ptr stop = stop_cond_create( CONVERGENCE, 0.00000001 );
    model_ptr model = model_create( stop );
    int i = 0;
    FILE* fp = fopen(file_path, "r");
    if (!fp)
    {
        printf("Error in opening file %s!\n", file_path);
        exit(EXIT_FAILURE);
    }
    while( i < max_sweeps )
    {
        io_res_ptr input = read_batch_fp( fp
                                        , separator
                                        , batch_size
                                        , file_path );
        dataset_ptr ds = dataset_from_io( input 
                                        , target_var_column );
        bool converged = model_update( model, ds );
        if (eof( input ))
        {
            fseek(fp, 0, SEEK_SET);
            i++;
        }
        io_res_destroy( input ); 
        dataset_destroy( ds );
        if (converged)
        {
            break;
        }
    }
    model_print_values(model);
    //model_print_coeff(model);
    model_destroy( model );
    fclose(fp);
}
