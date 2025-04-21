/**
 * @file worker.h
 * @brief Entry point for MPI worker process responsible for processing documents.
 */

#ifndef WORKER_H
#define WORKER_H

/**
 * @brief Starts the worker loop that receives tasks and sends back results.
 */
void run_worker();

#endif

