/**
 * @file test_file_utils.c
 * @author Wiktor Szewczyk
 * @brief Unit tests for file I/O and dictionary utilities.
 *
 * Validates reading file content, reading dictionaries, handling limits,
 * detecting duplicates, and scanning directories for .txt files.
 */


#include <criterion/criterion.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "file_utils.h"
#include "hash_table.h"
#include "msg_consts.h"

Test(file_utils, read_file_success)
{
    const char *filename = "tmp_test_file.txt";
    FILE *f = fopen(filename, "w");
    fputs("This is test content.", f);
    fclose(f);

    char buffer[MAX_DOC_SIZE] = {0};
    int ret = read_file_content(filename, buffer, sizeof(buffer));
    cr_assert_eq(ret, 0);
    cr_assert(strstr(buffer, "test content"));

    remove(filename);
}

Test(file_utils, read_file_fail)
{
    char buffer[MAX_DOC_SIZE] = {0};
    int ret = read_file_content("non_existent_file.txt", buffer, sizeof(buffer));
    cr_assert_eq(ret, -1);
}

Test(file_utils, list_txt_files_empty_dir)
{
    mkdir("tmp_empty_dir", 0700);
    char files[MAX_FILES][PATH_MAX] = {{0}};
    int count = list_txt_files("tmp_empty_dir", files, MAX_FILES);
    cr_assert_eq(count, 0);
    rmdir("tmp_empty_dir");
}

Test(file_utils, list_txt_files_respects_limit)
{
    const char *dir = "tmp_limited_dir";
    mkdir(dir, 0700);

    for (int i = 0; i < MAX_FILES + 10; i++)
    {
        char filename[PATH_MAX];
        snprintf(filename, sizeof(filename), "%s/file%d.txt", dir, i);
        FILE *f = fopen(filename, "w");
        fprintf(f, "file %d", i);
        fclose(f);
    }

    char files[MAX_FILES][PATH_MAX] = {{0}};
    int count = list_txt_files(dir, files, MAX_FILES);
    cr_assert_eq(count, MAX_FILES);

    // cleanup
    for (int i = 0; i < MAX_FILES + 10; i++)
    {
        char filename[PATH_MAX];
        snprintf(filename, sizeof(filename), "%s/file%d.txt", dir, i);
        remove(filename);
    }
    rmdir(dir);
}

Test(file_utils, read_dictionary_normal)
{
    const char *filename = "tmp_dict.txt";
    FILE *f = fopen(filename, "w");
    fputs("manager\nworker\nmpi\n", f);
    fclose(f);

    char *keywords[MAX_KEYWORDS] = {0};
    int count = read_dictionary(filename, keywords, MAX_KEYWORDS);
    cr_assert_eq(count, 3);
    cr_assert_str_eq(keywords[0], "manager");
    cr_assert_str_eq(keywords[1], "worker");
    cr_assert_str_eq(keywords[2], "mpi");

    for (int i = 0; i < count; i++)
        free(keywords[i]);
    remove(filename);
    clear_hash_table();
}

Test(file_utils, read_dictionary_empty)
{
    const char *filename = "tmp_empty.txt";
    FILE *f = fopen(filename, "w");
    fclose(f);

    char *keywords[MAX_KEYWORDS] = {0};
    int count = read_dictionary(filename, keywords, MAX_KEYWORDS);
    cr_assert_eq(count, 0);
    remove(filename);
}

Test(file_utils, read_dictionary_file_not_found)
{
    char *keywords[MAX_KEYWORDS] = {0};
    int count = read_dictionary("no_such_file.txt", keywords, MAX_KEYWORDS);
    cr_assert_eq(count, -1);
}

Test(file_utils, read_dictionary_limit_enforced)
{
    const char *filename = "tmp_bigdict.txt";
    FILE *f = fopen(filename, "w");
    for (int i = 0; i < MAX_KEYWORDS + 10; i++)
        fprintf(f, "word%d\n", i);
    fclose(f);

    char *keywords[MAX_KEYWORDS] = {0};
    clear_hash_table();
    int count = read_dictionary(filename, keywords, MAX_KEYWORDS);
    cr_assert_eq(count, MAX_KEYWORDS);

    for (int i = 0; i < count; i++)
        free(keywords[i]);
    remove(filename);
}

Test(file_utils, read_dictionary_with_duplicates)
{
    const char *filename = "tmp_dupdict.txt";
    FILE *f = fopen(filename, "w");
    fputs("thread\nthread\nparallel\nthread\n", f);
    fclose(f);

    char *keywords[MAX_KEYWORDS] = {0};
    clear_hash_table();
    int count = read_dictionary(filename, keywords, MAX_KEYWORDS);
    cr_assert_eq(count, 4);

    for (int i = 0; i < count; i++)
        free(keywords[i]);
    remove(filename);
}
