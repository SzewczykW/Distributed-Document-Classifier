#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include "classifier.h"
#include "worker.h"

#define TAG_WORK 1
#define TAG_DONE 2
#define TAG_STOP 3

void run_worker(int world_rank) {
    char filepath[1024];
    MPI_Status status;

    while (1) {
        MPI_Recv(filepath, sizeof(filepath), MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == TAG_STOP)
            break;

        DocType type = classify_file(filepath);
        int result = (int) type;

        MPI_Send(&result, 1, MPI_INT, 0, TAG_DONE, MPI_COMM_WORLD);
    }
}

