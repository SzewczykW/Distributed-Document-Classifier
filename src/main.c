#include <mpi.h>
#include "manager.h"
#include "worker.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    int id, p;
    MPI_Comm worker_comm = MPI_COMM_WORLD;
    MPI_Comm manager_comm = MPI_COMM_WORLD;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (argc != 4)
    {
        if (!id)
        {
            printf("Program needs three arguments:\n");
            printf("%s <dir> <dict> <results>\n", argv[0]);
        }
    }
    else if (p < 2)
    {
        printf("Program needs at least two processes\n");
    }
    else
    {
        if (!id)
        {
            MPI_Comm_split(MPI_COMM_WORLD, MPI_UNDEFINED, id, &manager_comm);
            manager(argc, argv, p);
        }
        else
        {
            MPI_Comm_split(MPI_COMM_WORLD, 0, id, &worker_comm);
            worker(argc, argv, worker_comm);
        }
    }

    MPI_Finalize();
    return 0;
}
