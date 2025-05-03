#include <dirent.h>
#include <limits.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_utils.h"
#include "hash_table.h"

int list_txt_files(const char *dir_path, char files[][PATH_MAX], int max_files) {
    DIR *dir = opendir(dir_path);
    if (!dir) return -1;

    struct dirent *entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL && count < max_files) {
        if (strstr(entry->d_name, ".txt")) {
            snprintf(files[count], PATH_MAX, "%s/%s", dir_path, entry->d_name);
            count++;
        }
    }

    closedir(dir);
    return count;
}

int read_file_content(const char *filename, char *buffer, size_t bufsize) {
    FILE *f = fopen(filename, "r");
    if (!f) return -1;

    size_t n = fread(buffer, 1, bufsize - 1, f);
    buffer[n] = '\0';
    fclose(f);
    return 0;
}

FILE *open_output_file(const char *output_path) {
    return fopen(output_path, "w");
}

int read_dictionary(const char *dict_path, char *keywords[], int max_keywords) {
    FILE *df = fopen(dict_path, "r");
    if (!df) return -1;

    int count = 0;
    char word[MAX_WORD_LEN];

    while (fscanf(df, "%63s", word) == 1 && count < max_keywords) {
        to_lower(word);
        keywords[count] = strdup(word);
        if (!keywords[count]) {
            fclose(df);
            return -2;
        }
        insert_word(word, count);
        count++;
    }

    fclose(df);
    return count;
}

void broadcast_dictionary(char *keywords[], int num_keywords) {
    MPI_Bcast(&num_keywords, 1, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < num_keywords; i++) {
        MPI_Bcast(keywords[i], MAX_WORD_LEN, MPI_CHAR, 0, MPI_COMM_WORLD);
    }
}

void receive_dictionary(char *keywords[], int *num_keywords) {
    MPI_Bcast(num_keywords, 1, MPI_INT, 0, MPI_COMM_WORLD);
    char word[MAX_WORD_LEN];
    for (int i = 0; i < *num_keywords; i++) {
        MPI_Bcast(word, MAX_WORD_LEN, MPI_CHAR, 0, MPI_COMM_WORLD);
        keywords[i] = strdup(word);
        if (!keywords[i]) exit(EXIT_FAILURE);
        insert_word(word, i);
    }
}
