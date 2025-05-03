#ifndef WORKER_H
#define WORKER_H

#include <mpi.h>

#define DICT_SIZE_MSG 0
#define FILE_NAME_MSG 1
#define VECTOR_MSG 2
#define EMPTY_MSG 3

#define DIR_ARG 1
#define DICT_ARG 2
#define RES_ARG 3

void worker(int argc, char *argv[], MPI_Comm worker_comm);

#endif
