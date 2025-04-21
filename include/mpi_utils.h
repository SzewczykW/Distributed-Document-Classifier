/**
 * @file mpi_utils.h
 * @brief MPI helper functions for task and result communication in a manager-worker model.
 */

#ifndef MPI_UTILS_H
#define MPI_UTILS_H

#include <stdlib.h>

/**
 * @brief Sends a file path (task) to a worker process.
 * @param dst_rank Destination rank (worker).
 * @param filepath Path to file to be processed.
 */
void send_task(int dst_rank, const char *filepath);

/**
 * @brief Receives a file path (task) sent to this process.
 * @param buffer Destination buffer for the file path.
 * @param buflen Size of the buffer.
 * @return 1 if task was received, 0 if termination signal.
 */
int receive_task(char *buffer, size_t buflen);

/**
 * @brief Sends a result (e.g. classification or feature vector) to the manager.
 * @param result Integer result to send.
 */
void send_result(int result);

/**
 * @brief Receives a result from a worker.
 * @param from_rank Optional: store the sender's rank.
 * @return Integer result.
 */
int receive_result(int *from_rank);

/**
 * @brief Sends a termination signal to a worker.
 * @param dst_rank Destination rank (worker).
 */
void send_stop(int dst_rank);

#endif

