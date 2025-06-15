/**
 * @file worker.c
 * @author Wiktor Szewczyk
 * @brief Logic for a worker process participating in distributed document
 * classification.
 */

#include <GASPI.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "classifier.h"
#include "file_utils.h"
#include "gaspi_utils.h"
#include "hash_table.h"
#include "msg_consts.h"
#include "worker.h"

void worker(void)
{
    gaspi_rank_t my_rank;
    GASPI_CHECK(gaspi_proc_rank(&my_rank));

    printf("[WORKER %d] Starting up...\n", my_rank);

    char *keywords[MAX_KEYWORDS] = {0};
    int num_keywords;
    printf("[WORKER %d] Waiting for dictionary...\n", my_rank);
    receive_dictionary(keywords, &num_keywords);
    printf("[WORKER %d] Received dictionary with %d keywords\n", my_rank, num_keywords);

    int vec[MAX_KEYWORDS] = {0};
    char fname[PATH_MAX] = {0};
    char buffer[MAX_DOC_SIZE] = {0};

    gaspi_pointer_t work_seg_ptr;
    gaspi_pointer_t result_seg_ptr;
    GASPI_CHECK(gaspi_segment_ptr(WORK_SEGMENT_ID, &work_seg_ptr));
    GASPI_CHECK(gaspi_segment_ptr(RESULT_SEGMENT_ID, &result_seg_ptr));

    gaspi_offset_t my_work_offset = WORK_OFFSET(my_rank);
    gaspi_offset_t my_result_offset = RESULT_OFFSET(my_rank);

    printf("[WORKER %d] Ready for work (work_offset=%lu, result_offset=%lu)\n", my_rank,
           my_work_offset, my_result_offset);

    int files_processed = 0;
    while (1)
    {
        printf("[WORKER %d] Waiting for task...\n", my_rank);
        gaspi_notification_id_t notif_id;
        GASPI_CHECK(gaspi_notify_waitsome(WORK_SEGMENT_ID, WORK_NOTIF_ID, 1, &notif_id,
                                          GASPI_BLOCK));

        gaspi_notification_t notif_val;
        GASPI_CHECK(gaspi_notify_reset(WORK_SEGMENT_ID, notif_id, &notif_val));

        if (notif_val == SHUTDOWN_NOTIF)
        {
            printf("[WORKER %d] Received shutdown signal, processed %d files total\n",
                   my_rank, files_processed);
            break;
        }

        if (notif_val == WORK_AVAILABLE_NOTIF)
        {
            strcpy(fname, (char *) work_seg_ptr + my_work_offset);
            printf("[WORKER %d] Processing file: %s\n", my_rank, fname);

            if (read_file_content(fname, buffer, sizeof(buffer)) < 0)
            {
                printf("[WORKER %d] ERROR: Failed to read file %s\n", my_rank, fname);
                memset(vec, 0, sizeof(vec));
            }
            else
            {
                classify_text(buffer, vec, num_keywords);
                printf("[WORKER %d] Classification complete for %s\n", my_rank, fname);
            }

            int *vec_ptr = (int *) ((char *) result_seg_ptr + my_result_offset);
            char *fname_ptr = (char *) ((char *) result_seg_ptr + my_result_offset +
                                        MAX_KEYWORDS * sizeof(int));

            memcpy(vec_ptr, vec, MAX_KEYWORDS * sizeof(int));
            strcpy(fname_ptr, fname);

            printf("[WORKER %d] Sending results back to manager\n", my_rank);
            GASPI_CHECK(gaspi_write_notify(RESULT_SEGMENT_ID, my_result_offset, 0,
                                           RESULT_SEGMENT_ID, my_result_offset,
                                           MAX_KEYWORDS * sizeof(int) + PATH_MAX,
                                           my_rank, WORK_DONE_NOTIF, 1, GASPI_BLOCK));
            files_processed++;
        }
    }

    printf("[WORKER %d] Cleaning up...\n", my_rank);
    for (int i = 0; i < num_keywords; i++)
        free(keywords[i]);
    clear_hash_table();
    printf("[WORKER %d] Shutdown complete\n", my_rank);
}
