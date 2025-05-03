#include <mpi.h>
#include <stdio.h>

#include "manager.h"
#include "worker.h"

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 4)
    {
        if (rank == 0)
            fprintf(stderr, "Usage: %s <input_dir> <dict_file> <output_file>\n",
                    argv[0]);
        MPI_Finalize();
        return 1;
    }

    if (rank == 0)
        manager(argv[1], argv[2], argv[3], size);
    else
        worker();

    MPI_Finalize();
    return 0;
}
