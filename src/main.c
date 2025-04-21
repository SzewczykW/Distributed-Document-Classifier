/**
 * @file main.c
 * @brief Entry point of the distributed document classification application.
 */

#include <mpi.h>
#include "io_utils.h"
#include "manager.h"
#include "worker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Parses CLI arguments and launches the MPI application.
 *
 * Initializes MPI, checks for required input/output arguments,
 * and dispatches to either the manager or a worker based on rank.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 on success, non-zero on error.
 */
int main(int argc, char *argv[])
{
    const char *input = NULL, *output = NULL;

    for (int i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "--help"))
        {
            print_help(argv[0]);
            return 0;
        }
        else if (!strcmp(argv[i], "--input") && i + 1 < argc)
        {
            input = argv[++i];
        }
        else if (!strcmp(argv[i], "--output") && i + 1 < argc)
        {
            output = argv[++i];
        }
    }

    if (!input || !output)
    {
        print_help(argv[0]);
        return 1;
    }

    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
        run_manager(input, output, argc);
    else
        run_worker();

    MPI_Finalize();
    return 0;
}

