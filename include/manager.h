/**
 * @file manager.h
 * @author Wiktor Szewczyk
 * @brief Interface for the manager process.
 */

#ifndef MANAGER_H
#define MANAGER_H

/**
 * @brief Entry point for the manager process.
 *
 * The manager is responsible for:
 * - Reading and broadcasting the dictionary
 * - Scanning the input directory for text files
 * - Distributing work to worker processes
 * - Collecting and writing classification results
 *
 * @param input_dir Path to directory containing .txt files
 * @param dict_file Path to the dictionary file
 * @param output_file Path where results will be written
 * @param processes_num Number of GASPI processes
 */
void manager(const char *input_dir, const char *dict_file, const char *output_file,
             int processes_num);

#endif
