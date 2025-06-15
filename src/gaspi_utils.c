/**
 * @file gaspi_utils.c
 * @author Wiktor Szewczyk
 * @brief Utility functions for GASPI segment management.
 */

#include "gaspi_utils.h"
#include "msg_consts.h"

void init_gaspi_segments(void)
{
    gaspi_size_t dict_size = sizeof(int) + MAX_KEYWORDS * MAX_WORD_LEN;
    gaspi_size_t work_size = MAX_DOC_SIZE + sizeof(int);

    // Segment dla słownika
    GASPI_CHECK(gaspi_segment_create(DICT_SEGMENT_ID, dict_size, GASPI_GROUP_ALL,
                                     GASPI_BLOCK, GASPI_ALLOC_DEFAULT));

    // Segment dla pracy
    GASPI_CHECK(gaspi_segment_create(WORK_SEGMENT_ID, work_size, GASPI_GROUP_ALL,
                                     GASPI_BLOCK, GASPI_ALLOC_DEFAULT));
}

void cleanup_gaspi_segments(void)
{
    gaspi_segment_delete(DICT_SEGMENT_ID);
    gaspi_segment_delete(WORK_SEGMENT_ID);
}
