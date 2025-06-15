/**
 * @file worker.h
 * @author Wiktor Szewczyk
 * @brief Interface for worker process.
 */

#ifndef WORKER_H
#define WORKER_H

/**
 * @brief Entry point for each GASPI worker process.
 *
 * Each worker:
 * - Receives the dictionary from the manager
 * - Processes assigned documents
 * - Sends back feature vectors to the manager
 */
void worker(void);

#endif
