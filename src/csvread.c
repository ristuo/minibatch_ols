#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include "../include/matrix.h"
#include "../include/optimize.h"
#define DEBUGLVL 10
int main( int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Give filepath\n");
        return 1;     
    }
    else 
    {
        char* path = argv[1];
        #if DEBUGLVL > 0
            printf("data file %s\n", path);
        #endif
        FILE* fp = fopen(path, "r");
        if (!fp)
        {
            printf("failed to read the file %s", path);
            exit(EXIT_FAILURE);
        }
        long batch_size = 4;
        unsigned long target_index = 1;
        char separator = ',';
        int sweeps = 2;
        unsigned long offset = 0;
        int i = 0;
        int j = 0;
        while( i < sweeps )
        {
            ds_io_result res = read_batch( fp
                                         , separator
                                         , offset
                                         , batch_size
                                         , target_index );
            if (res.eof)
            {
                offset = 0;
                i++;
            }
            else
            {
                offset += res.bytes_read;
            }
            if (res.ds.data.nrow != 0)
            {
                print_dataset( &(res.ds) );
            }
            j++;
        }
        fclose(fp);
    }
}
