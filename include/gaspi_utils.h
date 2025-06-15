/**
 * @file gaspi_utils.h
 * @author Wiktor Szewczyk
 * @brief Utility macros and functions for GASPI error handling.
 */

#ifndef GASPI_UTILS_H
#define GASPI_UTILS_H

#include <GASPI.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Check if GASPI call returned success, exit on error.
 * @param call The GASPI function call to check
 */
#define GASPI_CHECK(call)                                                              \
    do                                                                                 \
    {                                                                                  \
        gaspi_return_t ret = (call);                                                   \
        if (ret != GASPI_SUCCESS)                                                      \
        {                                                                              \
            fprintf(stderr, "GASPI error at %s:%d: %s returned %d\n", __FILE__,        \
                    __LINE__, #call, ret);                                             \
            gaspi_proc_term(GASPI_BLOCK);                                              \
            exit(EXIT_FAILURE);                                                        \
        }                                                                              \
    } while (0)

// Constants for GASPI segments
#define DICT_SEGMENT_ID 0
#define WORK_SEGMENT_ID 1
#define RESULT_SEGMENT_ID 2

// Per worker segment size
#define WORKER_DATA_SIZE (PATH_MAX + MAX_KEYWORDS * sizeof(int))

// Per worker work segment offset
#define WORK_OFFSET(rank) ((rank - 1) * PATH_MAX)
// Per worker result segment offset
#define RESULT_OFFSET(rank) ((rank - 1) * WORKER_DATA_SIZE)

/**
 * @brief Initializes GASPI segments for dictionary and work data.
 */
void init_gaspi_segments(void);

/**
 * @brief Cleans up GASPI segments.
 */
void cleanup_gaspi_segments(void);

#endif /* GASPI_UTILS_H */
