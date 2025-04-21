/**
 * @file utils.h
 * @brief General utility functions for file scanning and memory cleanup.
 */

#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Scans a directory for .txt, .html, and .tex files.
 *
 * @param dir_path Path to the directory to scan.
 * @param count Output: number of files found.
 * @return Dynamically allocated array of file paths.
 */
char **list_documents(const char *dir_path, int *count);

/**
 * @brief Frees memory allocated by list_documents().
 *
 * @param file_list Array of file paths.
 * @param count Number of entries in the array.
 */
void free_file_list(char **file_list, int count);

#endif

