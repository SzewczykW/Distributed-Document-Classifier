#include "manager.h"
#include <mpi.h>
#include "file_utils.h"
#include "hash_table.h"
#include "msg_consts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void manager(const char *input_dir, const char *dict_file, const char *output_file,
             int size)
{
    char *keywords[MAX_KEYWORDS] = {0};
    int num_keywords = read_dictionary(dict_file, keywords, MAX_KEYWORDS);
    if (num_keywords < 0)
    {
        perror("Failed to read dictionary");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    broadcast_dictionary(keywords, num_keywords);

    char files[MAX_FILES][PATH_MAX] = {0};
    int file_count = list_txt_files(input_dir, files, MAX_FILES);
    if (file_count < 0)
    {
        fprintf(stderr, "Error reading directory: %s\n", input_dir);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    FILE *out = open_output_file(output_file);
    if (!out)
    {
        perror("Failed to open output file");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    fprintf(out, "%-12s:", "dictionary");
    for (int i = 0; i < num_keywords; i++)
        fprintf(out, " %s", keywords[i]);
    fprintf(out, "\n");

    int current_file = 0;
    int done_workers = 0;

    MPI_Request req;
    MPI_Status status;
    MPI_Irecv(NULL, 0, MPI_CHAR, MPI_ANY_SOURCE, REQUEST_MSG, MPI_COMM_WORLD, &req);

    while (done_workers < size - 1)
    {
        int flag = 0;
        MPI_Test(&req, &flag, &status);
        if (!flag)
            continue;

        int source = status.MPI_SOURCE;

        if (current_file < file_count)
        {
            MPI_Send(files[current_file++], PATH_MAX, MPI_CHAR, source, FILE_MSG,
                     MPI_COMM_WORLD);
        }
        else
        {
            MPI_Send(NULL, 0, MPI_CHAR, source, DONE_MSG, MPI_COMM_WORLD);
            done_workers++;
            MPI_Irecv(NULL, 0, MPI_CHAR, MPI_ANY_SOURCE, REQUEST_MSG, MPI_COMM_WORLD,
                      &req);
            continue;
        }

        int vec[MAX_KEYWORDS];
        char fname[PATH_MAX];
        MPI_Recv(vec, num_keywords, MPI_INT, source, VEC_MSG, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        MPI_Recv(fname, PATH_MAX, MPI_CHAR, source, FILE_MSG, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        fprintf(out, "%-12s:", strrchr(fname, '/') ? strrchr(fname, '/') + 1 : fname);
        for (int i = 0; i < num_keywords; i++)
            fprintf(out, " %d", vec[i]);
        fprintf(out, "\n");

        MPI_Irecv(NULL, 0, MPI_CHAR, MPI_ANY_SOURCE, REQUEST_MSG, MPI_COMM_WORLD, &req);
    }

    int flag = 0;
    MPI_Test(&req, &flag, MPI_STATUS_IGNORE);
    if (!flag)
    {
        MPI_Cancel(&req);
        MPI_Wait(&req, MPI_STATUS_IGNORE);
    }

    fclose(out);
    for (int i = 0; i < num_keywords; i++)
        free(keywords[i]);
    clear_hash_table();
}
