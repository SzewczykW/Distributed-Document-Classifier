/**
 * @file worker.c
 * @brief Worker process implementation for processing and classifying documents.
 */

#include "worker.h"
#include <mpi.h>
#include "hash_table.h"
#include "profile.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Entry point for a worker process.
 *
 * Loads the dictionary from a file, builds the hash table,
 * receives file paths via MPI, computes feature vectors, and
 * sends results back to the manager.
 */
void run_worker(void)
{
    char dict_path[] = "dictionary.txt";

    FILE *f = fopen(dict_path, "r");
    if (!f)
        exit(EXIT_FAILURE);

    char word[128];
    int word_count = 0;
    HashTable *dict = init_hash_table();

    while (fscanf(f, "%127s", word) == 1)
        hash_insert(word, dict, ++word_count);
    fclose(f);

    MPI_Send(&word_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    char filepath[1024];
    MPI_Status status;

    while (1)
    {
        MPI_Recv(filepath, sizeof(filepath), MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD,
                 &status);
        if (status.MPI_TAG == 2)
            break;

        unsigned char *profile = calloc(word_count, sizeof(unsigned char));
        build_profile_from_file(filepath, dict, profile, word_count);
        MPI_Send(profile, word_count, MPI_UNSIGNED_CHAR, 0, 1, MPI_COMM_WORLD);
        free(profile);
    }

    free_hash_table(dict);
}

