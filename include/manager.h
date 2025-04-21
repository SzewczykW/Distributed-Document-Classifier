/**
 * @file manager.h
 * @brief Declares the entry point for the MPI manager process.
 */

#ifndef MANAGER_H
#define MANAGER_H

/**
 * @brief Executes the MPI manager, which distributes tasks to workers and gathers results.
 * @param input_dir Path to directory containing input files.
 * @param output_file Path to output results file.
 * @param world_size Number of MPI processes.
 */
void run_manager(const char *input_dir, const char *output_file, int world_size);

#endif

