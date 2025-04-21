/**
 * @file utils.c
 * @brief File utilities for scanning directories and releasing memory.
 */

#include "utils.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/**
 * @brief Lists all supported documents in a given directory.
 *
 * Scans the directory `dir_path` and returns paths to regular files
 * that have extensions: `.txt`, `.html`, or `.tex`.
 *
 * @param dir_path Path to directory to scan.
 * @param count Output parameter: number of documents found.
 * @return Dynamically allocated array of file paths. Must be freed with free_file_list().
 */
char **list_documents(const char *dir_path, int *count)
{
    DIR *dir = opendir(dir_path);
    if (!dir)
        return NULL;

    struct dirent *entry;
    struct stat st;
    char **files = malloc(sizeof(char *) * 1024);
    int idx = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir_path, entry->d_name);
        if (stat(fullpath, &st) == -1)
            continue;
        if (S_ISREG(st.st_mode))
        {
            const char *ext = strrchr(entry->d_name, '.');
            if (!ext)
                continue;
            if (strcmp(ext, ".txt") == 0 || strcmp(ext, ".html") == 0 ||
                strcmp(ext, ".tex") == 0)
            {
                files[idx++] = strdup(fullpath);
            }
        }
    }

    closedir(dir);
    *count = idx;
    return files;
}

/**
 * @brief Frees the memory allocated by list_documents().
 *
 * @param file_list Array of strings.
 * @param count Number of strings in the array.
 */
void free_file_list(char **file_list, int count)
{
    for (int i = 0; i < count; ++i)
    {
        free(file_list[i]);
    }
    free(file_list);
}

