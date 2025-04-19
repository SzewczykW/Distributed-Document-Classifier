#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "utils.h"

char **list_documents(const char *dir_path, int *count) {
    DIR *dir = opendir(dir_path);
    if (!dir) return NULL;

    struct dirent *entry;
    struct stat st;
    char **files = calloc(sizeof(char *), 1024);
    int idx = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir_path, entry->d_name);
        if (stat(fullpath, &st) == -1) continue;
        if (S_ISREG(st.st_mode)) {
            const char *ext = strrchr(entry->d_name, '.');
            if (!ext) continue;
            if (strcmp(ext, ".txt") == 0 || strcmp(ext, ".html") == 0 || strcmp(ext, ".tex") == 0) {
                files[idx++] = fullpath;
            }
        }
    }

    closedir(dir);
    *count = idx;
    return files;
}

void free_file_list(char **file_list, int count) {
    for (int i = 0; i < count; ++i) {
        free(file_list[i]);
    }
    free(file_list);
}

void print_help(const char *program_name) {
    printf("Usage: %s --input <input_folder> --output <output_file>\n", program_name);
    printf("Options:\n");
    printf("  --help                 Show this help message\n");
    printf("  --input <folder>      Path to folder with .txt files\n");
    printf("  --output <file>       Output file for results\n");}

