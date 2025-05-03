#include "manager.h"
#include <mpi.h>
#include "file_utils.h"
#include "worker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void manager(__attribute__((unused)) int argc, char *argv[], int p)
{
    int i, words_cnt, assign_cnt = 0, dict_size, file_cnt, terminated = 0;
    int *assigned = (int *) malloc(p * sizeof(int));
    unsigned char *buffer;
    unsigned char **vector;
    char **file_name = NULL;
    MPI_Request pending;
    MPI_Status status;
    int src, tag;

    printf("\n\nManager:\n");

    MPI_Irecv(&dict_size, 1, MPI_INT, MPI_ANY_SOURCE, DICT_SIZE_MSG, MPI_COMM_WORLD,
              &pending);
    MPI_Irecv(&words_cnt, 1, MPI_INT, MPI_ANY_SOURCE, 4, MPI_COMM_WORLD, &pending);

    get_names(argv[DIR_ARG], &file_name, &file_cnt);
    MPI_Wait(&pending, &status);

    buffer = (unsigned char *) malloc(words_cnt * sizeof(unsigned char));
    build_2d_array(file_cnt, words_cnt, &vector);

    while (terminated < (p - 1))
    {
        MPI_Recv(buffer, dict_size, MPI_UNSIGNED_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status);
        src = status.MPI_SOURCE;
        tag = status.MPI_TAG;

        if (tag == VECTOR_MSG)
        {
            for (i = 0; i < words_cnt; i++)
            {
                vector[assigned[src]][i] = buffer[i];
            }
        }

        if (assign_cnt < file_cnt)
        {
            MPI_Send(file_name[assign_cnt], strlen(file_name[assign_cnt]) + 1, MPI_CHAR,
                     src, FILE_NAME_MSG, MPI_COMM_WORLD);
            assigned[src] = assign_cnt++;
        }
        else
        {
            MPI_Send(NULL, 0, MPI_CHAR, src, FILE_NAME_MSG, MPI_COMM_WORLD);
            terminated++;
        }
    }

    write_profiles(argv[RES_ARG], file_cnt, words_cnt, file_name, vector);

    for (i = 0; i < file_cnt; i++)
        free(file_name[i]);
    free(file_name);
    free(buffer);
    free(assigned);
}
