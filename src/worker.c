#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "hash_table.h"
#include "worker.h"
#include "file_utils.h"
#include "msg_consts.h"

void worker(void) {
    int num_keywords;
    char *keywords[MAX_KEYWORDS];
    receive_dictionary(keywords, &num_keywords);

    char filename[MAX_FILENAME];
    char content[MAX_DOC_SIZE];
    int vec[MAX_KEYWORDS];

    while (1) {
        MPI_Status status;
        MPI_Recv(filename, MAX_FILENAME, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

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
                    vec[idx] = 1;
                token = strtok(NULL, " \n\t.,;:!?()[]{}\"'-");
            }
        }

        MPI_Send(vec, num_keywords, MPI_INT, 0, VEC_MSG, MPI_COMM_WORLD);
        MPI_Send(filename, MAX_FILENAME, MPI_CHAR, 0, FILE_MSG, MPI_COMM_WORLD);
    }

    for (int i = 0; i < num_keywords; i++) free(keywords[i]);
}
