/**
 * @file file_utils.c
 * @author Wiktor Szewczyk
 * @brief File reading, dictionary parsing, and dictionary broadcasting over GASPI.
 */

#include <GASPI.h>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_utils.h"
#include "gaspi_utils.h"
#include "hash_table.h"
#include "msg_consts.h"

int list_txt_files(const char *dir_path, char files[][PATH_MAX], int max_files)
{
    DIR *dir = opendir(dir_path);
    if (!dir)
        return -1;

    struct dirent *entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL && count < max_files)
    {
        if (strstr(entry->d_name, ".txt"))
        {
            snprintf(files[count], PATH_MAX, "%s/%s", dir_path, entry->d_name);
            count++;
        }
    }

    closedir(dir);
    return count;
}

int read_file_content(const char *filename, char *buffer, size_t bufsize)
{
    FILE *f = fopen(filename, "r");
    if (!f)
        return -1;

    size_t n = fread(buffer, 1, bufsize - 1, f);
    buffer[n] = '\0';
    fclose(f);
    return 0;
}

FILE *open_output_file(const char *output_path)
{
    return fopen(output_path, "w");
}

int read_dictionary(const char *dict_path, char *keywords[], int max_keywords)
{
    FILE *df = fopen(dict_path, "r");
    if (!df)
        return -1;

    int count = 0;
    char word[MAX_WORD_LEN] = {0};

    while (fscanf(df, "%63s", word) == 1 && count < max_keywords)
    {
        to_lower(word);
        keywords[count] = strdup(word);
        if (!keywords[count])
        {
            fclose(df);
            return -2;
        }
        insert_word(word, count);
        count++;
    }

    fclose(df);
    return count;
}

void broadcast_dictionary(char *keywords[], int num_keywords)
{
    gaspi_rank_t size;
    GASPI_CHECK(gaspi_proc_num(&size));

    gaspi_pointer_t dict_seg_ptr;
    GASPI_CHECK(gaspi_segment_ptr(DICT_SEGMENT_ID, &dict_seg_ptr));

    // Zapisz liczbę słów kluczowych
    memcpy(dict_seg_ptr, &num_keywords, sizeof(int));

    // Zapisz słowa kluczowe
    for (int i = 0; i < num_keywords; i++)
    {
        strncpy((char *) dict_seg_ptr + sizeof(int) + i * MAX_WORD_LEN, keywords[i],
                MAX_WORD_LEN - 1);
    }

    // Wyślij do wszystkich workerów
    for (gaspi_rank_t rank = 1; rank < size; rank++)
    {
        GASPI_CHECK(gaspi_write(DICT_SEGMENT_ID, 0, rank, DICT_SEGMENT_ID, 0,
                                sizeof(int) + num_keywords * MAX_WORD_LEN, 0,
                                GASPI_BLOCK));
    }

    GASPI_CHECK(gaspi_wait(0, GASPI_BLOCK));
}

void receive_dictionary(char *keywords[], int *num_keywords)
{
    gaspi_pointer_t dict_seg_ptr;
    GASPI_CHECK(gaspi_segment_ptr(DICT_SEGMENT_ID, &dict_seg_ptr));

    // Odbierz liczbę słów kluczowych
    memcpy(num_keywords, dict_seg_ptr, sizeof(int));

    // Odbierz słowa kluczowe
    char word[MAX_WORD_LEN] = {0};
    for (int i = 0; i < *num_keywords; i++)
    {
        strncpy(word, (char *) dict_seg_ptr + sizeof(int) + i * MAX_WORD_LEN,
                MAX_WORD_LEN - 1);
        word[MAX_WORD_LEN - 1] = '\0'; // Zabezpieczenie

        keywords[i] = strdup(word);
        if (!keywords[i])
            exit(EXIT_FAILURE);
        insert_word(word, i);
    }
}
