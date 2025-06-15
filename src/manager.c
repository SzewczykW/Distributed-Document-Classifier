/**
 * @file manager.c
 * @author Wiktor Szewczyk
 * @brief Orchestrates the classification by managing worker coordination and output
 * collection.
 */

#include <GASPI.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_utils.h"
#include "gaspi_utils.h"
#include "hash_table.h"
#include "manager.h"
#include "msg_consts.h"

void manager(const char *input_dir, const char *dict_file, const char *output_file,
             int processes_num)
{
    printf("[MANAGER] Starting with %d processes\n", processes_num);

    gaspi_size_t dict_size = sizeof(int) + MAX_KEYWORDS * MAX_WORD_LEN;
    gaspi_size_t work_size = processes_num * PATH_MAX;
    gaspi_size_t result_size = processes_num * WORKER_DATA_SIZE;

    printf("[MANAGER] Creating GASPI segments...\n");
    GASPI_CHECK(gaspi_segment_create(DICT_SEGMENT_ID, dict_size, GASPI_GROUP_ALL,
                                     GASPI_BLOCK, GASPI_ALLOC_DEFAULT));

    GASPI_CHECK(gaspi_segment_create(WORK_SEGMENT_ID, work_size, GASPI_GROUP_ALL,
                                     GASPI_BLOCK, GASPI_ALLOC_DEFAULT));

    GASPI_CHECK(gaspi_segment_create(RESULT_SEGMENT_ID, result_size, GASPI_GROUP_ALL,
                                     GASPI_BLOCK, GASPI_ALLOC_DEFAULT));
    printf("[MANAGER] GASPI segments created successfully\n");

    char *keywords[MAX_KEYWORDS] = {0};
    int num_keywords = read_dictionary(dict_file, keywords, MAX_KEYWORDS);
    if (num_keywords < 0)
    {
        perror("Failed to read dictionary");
        gaspi_proc_term(GASPI_BLOCK);
        exit(1);
    }
    printf("[MANAGER] Dictionary loaded: %d keywords\n", num_keywords);

    printf("[MANAGER] Broadcasting dictionary to workers...\n");
    broadcast_dictionary(keywords, num_keywords);

    char files[MAX_FILES][PATH_MAX] = {0};
    int file_count = list_txt_files(input_dir, files, MAX_FILES);
    if (file_count < 0)
    {
        fprintf(stderr, "Error reading directory: %s\n", input_dir);
        gaspi_proc_term(GASPI_BLOCK);
        exit(1);
    }
    printf("[MANAGER] Found %d files to process\n", file_count);

    FILE *out = open_output_file(output_file);
    if (!out)
    {
        perror("Failed to open output file");
        gaspi_proc_term(GASPI_BLOCK);
        exit(1);
    }

    fprintf(out, "%-12s:", "dictionary");
    for (int i = 0; i < num_keywords; i++)
        fprintf(out, " %s", keywords[i]);
    fprintf(out, "\n");

    int current_file = 0;
    int done_workers = 0;

    gaspi_pointer_t work_seg_ptr;
    gaspi_pointer_t result_seg_ptr;
    GASPI_CHECK(gaspi_segment_ptr(WORK_SEGMENT_ID, &work_seg_ptr));
    GASPI_CHECK(gaspi_segment_ptr(RESULT_SEGMENT_ID, &result_seg_ptr));

    printf("[MANAGER] Sending initial tasks to workers...\n");
    for (gaspi_rank_t rank = 1; rank < processes_num; rank++)
    {
        if (current_file < file_count)
        {
            gaspi_offset_t work_offset = WORK_OFFSET(rank);
            strcpy((char *) work_seg_ptr + work_offset, files[current_file]);
            printf("[MANAGER] Sending file '%s' to worker %d\n", files[current_file],
                   rank);
            current_file++;
            GASPI_CHECK(gaspi_write_notify(
                WORK_SEGMENT_ID, work_offset, rank, WORK_SEGMENT_ID, work_offset,
                PATH_MAX, WORK_NOTIF_ID, WORK_AVAILABLE_NOTIF, 1, GASPI_BLOCK));
        }
    }

    printf("[MANAGER] Waiting for worker results...\n");
    while (done_workers < processes_num - 1)
    {
        gaspi_notification_id_t notif_id;
        gaspi_return_t ret = gaspi_notify_waitsome(
            RESULT_SEGMENT_ID, 1, processes_num - 1, &notif_id, GASPI_TEST);
        if (ret == GASPI_SUCCESS)
        {
            gaspi_notification_t notif_val;
            GASPI_CHECK(gaspi_notify_reset(RESULT_SEGMENT_ID, notif_id, &notif_val));

            if (notif_val == WORK_DONE_NOTIF)
            {
                gaspi_rank_t rank = notif_id;
                gaspi_offset_t result_offset = RESULT_OFFSET(rank);

                int *vec = (int *) ((char *) result_seg_ptr + result_offset);
                char *fname = (char *) ((char *) result_seg_ptr + result_offset +
                                        MAX_KEYWORDS * sizeof(int));

                printf("[MANAGER] Received results from worker %d for file '%s'\n",
                       rank, strrchr(fname, '/') ? strrchr(fname, '/') + 1 : fname);

                fprintf(out, "%-12s:",
                        strrchr(fname, '/') ? strrchr(fname, '/') + 1 : fname);
                for (int i = 0; i < num_keywords; i++)
                    fprintf(out, " %d", vec[i]);
                fprintf(out, "\n");

                if (current_file < file_count)
                {
                    gaspi_offset_t work_offset = WORK_OFFSET(rank);
                    strcpy((char *) work_seg_ptr + work_offset, files[current_file]);
                    printf("[MANAGER] Sending next file '%s' to worker %d\n",
                           files[current_file], rank);
                    current_file++;
                    GASPI_CHECK(gaspi_write_notify(
                        WORK_SEGMENT_ID, work_offset, rank, WORK_SEGMENT_ID,
                        work_offset, PATH_MAX, WORK_NOTIF_ID, WORK_AVAILABLE_NOTIF, 1,
                        GASPI_BLOCK));
                }
                else
                {
                    gaspi_offset_t work_offset = WORK_OFFSET(rank);
                    strcpy((char *) work_seg_ptr + work_offset, "");
                    printf("[MANAGER] Sending shutdown signal to worker %d\n", rank);
                    GASPI_CHECK(gaspi_write_notify(
                        WORK_SEGMENT_ID, work_offset, rank, WORK_SEGMENT_ID,
                        work_offset, 1, WORK_NOTIF_ID, SHUTDOWN_NOTIF, 1, GASPI_BLOCK));
                    done_workers++;
                }
            }
        }
    }

    printf("[MANAGER] All workers finished, cleaning up...\n");
    fclose(out);
    for (int i = 0; i < num_keywords; i++)
        free(keywords[i]);
    clear_hash_table();

    gaspi_segment_delete(DICT_SEGMENT_ID);
    gaspi_segment_delete(WORK_SEGMENT_ID);
    gaspi_segment_delete(RESULT_SEGMENT_ID);
    printf("[MANAGER] Cleanup complete\n");
}
