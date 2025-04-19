#include <mpi.h>
#include <stdlib.h>
#include <string.h>
#include "manager.h"
#include "worker.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        print_help(argv[0]);
        return (argc == 3) ? EXIT_FAILURE : EXIT_SUCCESS;
    }

    const char *input_dir = argv[1];
    const char *output_path = argv[2];

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        run_manager(input_dir, output_path, size);
    } else {
        run_worker(rank);
    }

    MPI_Finalize();
    return 0;
}

