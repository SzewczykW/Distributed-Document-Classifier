/**
 * @file mpi_utils.c
 * @brief Implements helper functions for MPI-based task distribution and result communication.
 */

#include "mpi_utils.h"
#include <mpi.h>
#include <string.h>

#define TAG_WORK 1  ///< MPI tag for sending a task to a worker
#define TAG_DONE 2  ///< MPI tag for receiving a result from a worker
#define TAG_STOP 3  ///< MPI tag indicating no more work (terminate)

/**
 * @brief Sends a file path to a worker process.
 *
 * @param dst_rank MPI rank of the worker.
 * @param filepath Path to the file (null-terminated string).
 */
void send_task(int dst_rank, const char *filepath)
{
    MPI_Send(filepath, strlen(filepath) + 1, MPI_CHAR, dst_rank, TAG_WORK,
             MPI_COMM_WORLD);
}

/**
 * @brief Receives a file path task sent by the manager.
 *
 * @param buffer Buffer to store received file path.
 * @param buflen Size of the buffer.
 * @return 1 if task received, 0 if termination signal received.
 */
int receive_task(char *buffer, size_t buflen)
{
    MPI_Status status;
    MPI_Recv(buffer, buflen, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    return status.MPI_TAG != TAG_STOP;
}

/**
 * @brief Sends an integer result (e.g., classification, feature count).
 *
 * @param result Integer value to send.
 */
void send_result(int result)
{
    MPI_Send(&result, 1, MPI_INT, 0, TAG_DONE, MPI_COMM_WORLD);
}

/**
 * @brief Receives a result from any worker.
 *
 * @param from_rank Optional: pointer to store the rank of the sender.
 * @return Received integer result.
 */
int receive_result(int *from_rank)
{
    int result;
    MPI_Status status;
    MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, TAG_DONE, MPI_COMM_WORLD, &status);
    if (from_rank)
        *from_rank = status.MPI_SOURCE;
    return result;
}

/**
 * @brief Sends a termination signal to the worker.
 *
 * @param dst_rank MPI rank of the worker.
 */
void send_stop(int dst_rank)
{
    MPI_Send(NULL, 0, MPI_CHAR, dst_rank, TAG_STOP, MPI_COMM_WORLD);
}

