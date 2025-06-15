/**
 * @file test_classifier.c
 * @author Wiktor Szewczyk
 * @brief Unit tests for the classify_text function using Criterion.
 *
 * Covers keyword detection, case insensitivity, punctuation stripping,
 * and handling of multiple/zero occurrences.
 */

#include <criterion/criterion.h>
#include <string.h>

#include "classifier.h"
#include "utils.h"

Test(classify, detects_keywords)
{
    char *keywords[] = {strdup("manager"), strdup("worker"), strdup("mpi")};
    prepare_keywords(keywords, 3);
    int result[3] = {0};
    int expected[3] = {1, 1, 1};

    classify_text("The Manager oversees MPI tasks. Each worker runs independently.",
                  result, 3);
    cr_assert_arr_eq(result, expected, 3);

    for (int i = 0; i < 3; i++)
        free(keywords[i]);
}

Test(classify, no_keywords_found)
{
    char *keywords[] = {strdup("thread"), strdup("mpi"), strdup("distributed")};
    prepare_keywords(keywords, 3);
    int result[3] = {0};
    int expected[3] = {0};

    classify_text("Nothing to see here.", result, 3);
    cr_assert_arr_eq(result, expected, 3);

    for (int i = 0; i < 3; i++)
        free(keywords[i]);
}

Test(classify, multiple_occurrences)
{
    char *keywords[] = {strdup("task"), strdup("system")};
    prepare_keywords(keywords, 2);
    int result[2] = {0};
    int expected[2] = {3, 3};

    classify_text("Task system Task SYSTEM system taSk.", result, 2);
    cr_assert_arr_eq(result, expected, 2);

    for (int i = 0; i < 2; i++)
        free(keywords[i]);
}

Test(classify, ignores_case)
{
    char *keywords[] = {strdup("mpi")};
    prepare_keywords(keywords, 1);
    int result[1] = {0};

    classify_text("MPI mpi MpI mPi", result, 1);
    cr_assert_eq(result[0], 4);

    free(keywords[0]);
}

Test(classify, strips_punctuation)
{
    char *keywords[] = {strdup("task")};
    prepare_keywords(keywords, 1);
    int result[1] = {0};

    classify_text("task, task. task! task? task;", result, 1);
    cr_assert_eq(result[0], 5);

    free(keywords[0]);
}
