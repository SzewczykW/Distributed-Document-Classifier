/**
 * @file main.c
 * @author Wiktor Szewczyk
 * @brief Entry point for the GASPI-based document classifier.
 */

#include <GASPI.h>
#include <stdio.h>

#include "gaspi_utils.h"
#include "manager.h"
#include "worker.h"

int main(int argc, char *argv[])
{
    gaspi_rank_t rank = 0;
    gaspi_rank_t size = 0;

    printf("[MAIN] Initializing GASPI...\n");
    GASPI_CHECK(gaspi_proc_init(GASPI_BLOCK));
    GASPI_CHECK(gaspi_proc_rank(&rank));
    GASPI_CHECK(gaspi_proc_num(&size));

    printf("[MAIN] Process %d of %d started\n", rank, size);

    if (argc != 4)
    {
        if (rank == 0)
        {
            fprintf(stderr, "Usage: %s <input_dir> <dict_file> <output_file>\n",
                    argv[0]);
            printf("[MAIN] Manager: Invalid arguments provided\n");
        }
        gaspi_proc_term(GASPI_BLOCK);
        return 1;
    }

    if (rank == 0)
    {
        printf("[MAIN] Starting as MANAGER\n");
        printf("[MAIN] Input dir: %s\n", argv[1]);
        printf("[MAIN] Dictionary: %s\n", argv[2]);
        printf("[MAIN] Output file: %s\n", argv[3]);
        manager(argv[1], argv[2], argv[3], size);
    }
    else
    {
        printf("[MAIN] Starting as WORKER %d\n", rank);
        worker();
    }

    printf("[MAIN] Process %d terminating...\n", rank);
    gaspi_proc_term(GASPI_BLOCK);
    printf("[MAIN] Process %d terminated successfully\n", rank);
    return 0;
}
