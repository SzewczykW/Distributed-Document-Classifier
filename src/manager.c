/**
 * @file manager.c
 * @brief Manager (rank 0) implementation responsible for task distribution and result aggregation.
 */

#include "manager.h"
#include <mpi.h>
#include "io_utils.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Runs the manager process.
 *
 * Loads documents, distributes tasks to workers,
 * receives feature vectors, and writes the final output matrix.
 *
 * @param input_dir Path to the input directory containing documents.
 * @param output_file Path to the output file for results.
 * @param world_size Number of processes in the MPI world.
 */
void run_manager(const char *input_dir, const char *output_file, int world_size)
{
    int file_count = 0;
    char **files = list_documents(input_dir, &file_count);
    if (!files || file_count == 0)
    {
        fprintf(stderr, "No documents found in %s\n", input_dir);
        return;
    }

    int dict_size;
    MPI_Recv(&dict_size, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    unsigned char **matrix = malloc(file_count * sizeof(unsigned char *));
    for (int i = 0; i < file_count; ++i)
        matrix[i] = calloc(dict_size, sizeof(unsigned char));

    int assigned = 0, completed = 0;

    for (int i = 1; i < world_size && assigned < file_count; ++i)
    {
        const char *path = files[assigned];
        MPI_Send(path, strlen(path) + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        assigned++;
    }

    while (completed < file_count)
    {
        int sender;
        MPI_Status status;
        MPI_Probe(MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
        sender = status.MPI_SOURCE;

        MPI_Recv(matrix[completed], dict_size, MPI_UNSIGNED_CHAR, sender, 1,
                 MPI_COMM_WORLD, &status);
        completed++;

        if (assigned < file_count)
        {
            MPI_Send(files[assigned], strlen(files[assigned]) + 1, MPI_CHAR, sender, 0,
                     MPI_COMM_WORLD);
            assigned++;
        }
        else
        {
            MPI_Send(NULL, 0, MPI_CHAR, sender, 2, MPI_COMM_WORLD);
        }
    }

    write_feature_matrix(output_file, matrix, file_count, dict_size);

    for (int i = 0; i < file_count; ++i)
        free(matrix[i]);
    free(matrix);
    free_file_list(files, file_count);
}

