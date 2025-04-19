#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "classifier.h"
#include "manager.h"

#define TAG_WORK 1
#define TAG_DONE 2
#define TAG_STOP 3

void run_manager(const char *input_dir, const char *output_path, int world_size) {
    int file_count = 0;
    char **files = list_documents(input_dir, &file_count);
    if (!files || file_count == 0) {
        fprintf(stderr, "No documents found in %s\n", input_dir);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    FILE *output = fopen(output_path, "w");
    if (!output) {
        perror("fopen");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int next_file = 0;

    // Initial distribution
    for (int i = 1; i < world_size && next_file < file_count; ++i) {
        MPI_Send(files[next_file], strlen(files[next_file]) + 1, MPI_CHAR, i, TAG_WORK, MPI_COMM_WORLD);
        next_file++;
    }

    // Receive results and send more
    for (int completed = 0; completed < file_count; ++completed) {
        int result;
        MPI_Status status;
        MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, TAG_DONE, MPI_COMM_WORLD, &status);

        fprintf(output, "Worker %d: %s -> %s\n", status.MPI_SOURCE,
                files[completed], doc_type_to_string((DocType) result));

        if (next_file < file_count) {
            MPI_Send(files[next_file], strlen(files[next_file]) + 1, MPI_CHAR,
                     status.MPI_SOURCE, TAG_WORK, MPI_COMM_WORLD);
            next_file++;
        } else {
            MPI_Send(NULL, 0, MPI_CHAR, status.MPI_SOURCE, TAG_STOP, MPI_COMM_WORLD);
        }
    }

    fclose(output);
    free_file_list(files, file_count);
}

