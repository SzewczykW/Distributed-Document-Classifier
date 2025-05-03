#include "worker.h"
#include <mpi.h>
#include "file_utils.h"
#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void worker(__attribute__((unused)) int argc, char *argv[], MPI_Comm worker_comm)
{
    int words_cnt, name_len;
    long dict_size;
    char *buffer = NULL, *name;
    unsigned char *profile;
    hash_table hash_tbl = *init_hash_table();
    MPI_Status status;
    MPI_Request pending;
    int worker_id;

    MPI_Comm_rank(worker_comm, &worker_id);

    printf("\n\nWorker %d:\n", worker_id);

    if (!worker_id)
    {
        read_dictionary(argv[DICT_ARG], &buffer, &dict_size);
        MPI_Isend(&dict_size, 1, MPI_INT, 0, DICT_SIZE_MSG, MPI_COMM_WORLD, &pending);
        MPI_Wait(&pending, &status);
    }

    MPI_Bcast(&dict_size, 1, MPI_LONG, 0, worker_comm);

    if (worker_id)
        buffer = (char *) malloc(dict_size);

    MPI_Bcast(buffer, dict_size, MPI_CHAR, 0, worker_comm);
    build_hash_table(buffer, &hash_tbl, &words_cnt);

    if (!worker_id)
    {
        MPI_Isend(&words_cnt, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &pending);
        MPI_Wait(&pending, &status);
    }

    profile = (unsigned char *) calloc(words_cnt, sizeof(unsigned char));
    MPI_Send(NULL, 0, MPI_UNSIGNED_CHAR, 0, EMPTY_MSG, MPI_COMM_WORLD);

    for (;;)
    {
        MPI_Probe(0, FILE_NAME_MSG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_CHAR, &name_len);
        if (!name_len)
            break;

        name = (char *) malloc(name_len);
        MPI_Recv(name, name_len, MPI_CHAR, 0, FILE_NAME_MSG, MPI_COMM_WORLD, &status);
        make_profile(name, hash_tbl, &profile);
        free(name);

        MPI_Send(profile, dict_size, MPI_UNSIGNED_CHAR, 0, VECTOR_MSG, MPI_COMM_WORLD);
        memset(profile, 0, words_cnt * sizeof(unsigned char));
    }

    free(buffer);
    free(profile);
}
