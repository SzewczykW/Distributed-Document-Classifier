#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_utils.h"
#include "hash_table.h"
#include "msg_consts.h"
#include "worker.h"

void worker(void) {
    int num_keywords;
    char *keywords[MAX_KEYWORDS];
    receive_dictionary(keywords, &num_keywords);

    char filename[PATH_MAX];
    char content[MAX_DOC_SIZE];
    int vec[MAX_KEYWORDS];

    while (1) {
        int dummy = 0;
        MPI_Send(&dummy, 1, MPI_INT, 0, REQUEST_MSG, MPI_COMM_WORLD);

        MPI_Status status;
        MPI_Recv(filename, PATH_MAX, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if (status.MPI_TAG == DONE_MSG)
            break;

        if (read_file_content(filename, content, sizeof(content)) != 0) {
            memset(vec, 0, sizeof(vec));
        } else {
            memset(vec, 0, sizeof(vec));
            char *token = strtok(content, " \n\t.,;:!?()[]{}\"'-");
            while (token) {
                to_lower(token);
                int idx = find_word(token);
                if (idx != -1)
                    vec[idx]++;
                token = strtok(NULL, " \n\t.,;:!?()[]{}\"'-");
            }
        }

        MPI_Request reqs[2];
        MPI_Isend(vec, num_keywords, MPI_INT, 0, VEC_MSG, MPI_COMM_WORLD, &reqs[0]);
        MPI_Isend(filename, PATH_MAX, MPI_CHAR, 0, FILE_MSG, MPI_COMM_WORLD, &reqs[1]);
        MPI_Waitall(2, reqs, MPI_STATUSES_IGNORE);
    }

    for (int i = 0; i < num_keywords; i++) free(keywords[i]);
}
