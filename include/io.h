#ifndef IO_H
#define IO_H
#include "../include/types.h"
#include <stdbool.h>
#include <stdio.h>
typedef struct io_res *io_res_ptr;

void io_res_destroy(io_res_ptr input);

index rows_read(io_res_ptr);

index cols_read(io_res_ptr);

index bytes_read(io_res_ptr);

bool eof(io_res_ptr);

char *path(io_res_ptr);

number **values(io_res_ptr);

void io_res_print(io_res_ptr);

io_res_ptr read_batch(char *file_path, char separator, index nrow,
                      index offset);

io_res_ptr read_batch_fp(FILE *fp, char separator, index nrow, char *file_path);
#endif
