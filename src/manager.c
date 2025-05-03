#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "manager.h"
#include "file_utils.h"
#include "hash_table.h"
#include "msg_consts.h"

void manager(const char *input_dir, const char *dict_file, const char *output_file, int size) {
    char *keywords[MAX_KEYWORDS];
    int num_keywords = read_dictionary(dict_file, keywords, MAX_KEYWORDS);
    if (num_keywords < 0) {
        perror("Failed to read dictionary");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    broadcast_dictionary(keywords, num_keywords);

    char files[MAX_FILES][MAX_FILENAME];
    int file_count = list_txt_files(input_dir, files, MAX_FILES);
    if (file_count < 0) {
        fprintf(stderr, "Error reading directory: %s\n", input_dir);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    FILE *out = open_output_file(output_file);
    if (!out) {
        perror("Failed to open output file");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    fprintf(out, "file         :");
    for (int i = 0; i < num_keywords; i++) {
        fprintf(out, " %s", keywords[i]);
    }
    fprintf(out, "\n");

    int current_file = 0;
    for (int i = 1; i < size && current_file < file_count; i++) {
        MPI_Send(files[current_file++], MAX_FILENAME, MPI_CHAR, i, FILE_MSG, MPI_COMM_WORLD);
    }

    int done = 0;
    while (done < file_count) {
        int vec[MAX_KEYWORDS];
        char fname[MAX_FILENAME];
        MPI_Status status;

        MPI_Recv(vec, num_keywords, MPI_INT, MPI_ANY_SOURCE, VEC_MSG, MPI_COMM_WORLD, &status);
        MPI_Recv(fname, MAX_FILENAME, MPI_CHAR, status.MPI_SOURCE, FILE_MSG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        fprintf(out, "%-12s:", strrchr(fname, '/') ? strrchr(fname, '/') + 1 : fname);
        for (int i = 0; i < num_keywords; i++)
            fprintf(out, " %d", vec[i]);
        fprintf(out, "\n");
        fflush(out);

        if (current_file < file_count) {
            MPI_Send(files[current_file++], MAX_FILENAME, MPI_CHAR, status.MPI_SOURCE, FILE_MSG, MPI_COMM_WORLD);
        } else {
            MPI_Send(NULL, 0, MPI_CHAR, status.MPI_SOURCE, DONE_MSG, MPI_COMM_WORLD);
        }

        done++;
    }

    fclose(out);
    for (int i = 0; i < num_keywords; i++) free(keywords[i]);
    clear_hash_table();
}
