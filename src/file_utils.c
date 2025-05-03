#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>

#include "file_utils.h"
#include "hash_table.h"

void get_names(char *path, char ***file_name, int *cnt)
{
    int count = 0;
    struct dirent *entry;
    DIR *dp;
    char fullpath[1024];
    char **name = (char **) malloc(100 * sizeof(char *));

    dp = opendir(path);
    if (!dp)
    {
        perror("opendir failed");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dp)) != NULL)
    {
        // Build full path to check file type
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(fullpath, &st) != 0)
            continue;

        if (S_ISREG(st.st_mode))
        {
            const char *ext = strrchr(entry->d_name, '.');
            if (ext && strcmp(ext, ".txt") == 0)
            {
                name[count] = strdup(fullpath); // copy full path
                count++;
            }
        }
    }

    closedir(dp);
    *file_name = name;
    *cnt = count;
}

void read_dictionary(char *path, char **content, long *len)
{
    FILE *f = fopen(path, "r");
    if (!f)
    {
        perror("Cannot open dictionary file");
        exit(EXIT_FAILURE);
    }

    fseek(f, 0, SEEK_END);
    *len = ftell(f);
    rewind(f);

    *content = (char *) malloc(*len + 1);
    fread(*content, 1, *len, f);
    (*content)[*len] = '\0';

    fclose(f);
}

void make_profile(char *path, hash_table hash_tbl, unsigned char **profile)
{
    FILE *f = fopen(path, "r");
    if (!f)
    {
        perror("Cannot open document file");
        return;
    }

    char string[50] = {0};
    int i = 0, num;
    char c;

    while ((c = fgetc(f)) != EOF)
    {
        if (ispunct(c))
            continue;
        if (isspace(c))
        {
            if (i > 0)
            {
                string[i] = '\0';
                num = hash_exist(hash_tbl, string);
                if (num > 0)
                {
                    (*profile)[num - 1]++;
                }
                i = 0;
                memset(string, 0, sizeof(string));
            }
        }
        else
        {
            if (i < (int) (sizeof(string) - 1))
            {
                string[i++] = c;
            }
        }
    }

    fclose(f);
}

void build_2d_array(int file_cnt, int words_cnt, unsigned char ***vector)
{
    *vector = (unsigned char **) malloc(file_cnt * sizeof(unsigned char *));
    for (int i = 0; i < file_cnt; i++)
    {
        (*vector)[i] = (unsigned char *) calloc(words_cnt, sizeof(unsigned char));
    }
}

void write_profiles(char *path, int file_cnt, int words_cnt, __attribute__((unused)) char **file_name,
                    unsigned char **vector)
{
    // Utwórz katalog nadrzędny jeśli nie istnieje
    char path_copy[1024];
    strncpy(path_copy, path, sizeof(path_copy));
    path_copy[sizeof(path_copy) - 1] = '\0';

    char *dir = dirname(path_copy);
    struct stat st = {0};
    if (stat(dir, &st) == -1) {
        if (mkdir(dir, 0755) != 0 && errno != EEXIST) {
            perror("Cannot create directory for result file");
            return;
        }
    }

    FILE *f = fopen(path, "w");
    if (!f) {
        perror("Cannot open result file for writing");
        return;
    }

    for (int i = 0; i < file_cnt; i++) {
        fprintf(f, "%s :", file_name[i]);
        printf("%s :", file_name[i]);
        for (int j = 0; j < words_cnt; j++) {
            fprintf(f, "%d ", vector[i][j]);
            printf("%d ", vector[i][j]);
        }
        fprintf(f, "\n");
        printf("\n");
    }

    fclose(f);
}
