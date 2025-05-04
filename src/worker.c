/**
 * @file worker.c
 * @author Wiktor Szewczyk
 * @brief Logic for a worker process participating in distributed document classification.
 */


#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "classifier.h"
#include "file_utils.h"
#include "hash_table.h"
#include "msg_consts.h"
#include "worker.h"

void worker(void)
{
    char *keywords[MAX_KEYWORDS] = {0};
    int num_keywords;
    receive_dictionary(keywords, &num_keywords);

    int vec[MAX_KEYWORDS] = {0};
    char fname[PATH_MAX] = {0};
    char buffer[MAX_DOC_SIZE] = {0};

    while (1)
    {
        MPI_Send(NULL, 0, MPI_CHAR, 0, REQUEST_MSG, MPI_COMM_WORLD);

        MPI_Status status;
        MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if (status.MPI_TAG == DONE_MSG)
        {
            MPI_Recv(NULL, 0, MPI_CHAR, 0, DONE_MSG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            break;
        }

        MPI_Recv(fname, PATH_MAX, MPI_CHAR, 0, FILE_MSG, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        if (read_file_content(fname, buffer, sizeof(buffer)) < 0)
        {
            memset(vec, 0, sizeof(vec));
        }
        else
        {
            classify_text(buffer, vec, num_keywords);
        }

        MPI_Request reqs[2];
        MPI_Isend(vec, num_keywords, MPI_INT, 0, VEC_MSG, MPI_COMM_WORLD, &reqs[0]);
        MPI_Isend(fname, PATH_MAX, MPI_CHAR, 0, FILE_MSG, MPI_COMM_WORLD, &reqs[1]);
        MPI_Waitall(2, reqs, MPI_STATUSES_IGNORE);
    }

    for (int i = 0; i < num_keywords; i++)
        free(keywords[i]);
    clear_hash_table();
}
