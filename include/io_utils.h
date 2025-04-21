/**
 * @file io_utils.h
 * @brief Utility functions for CLI usage help and feature matrix output.
 */

#ifndef IO_UTILS_H
#define IO_UTILS_H

/**
 * @brief Prints CLI usage instructions to stdout.
 * @param progname The program's name (typically argv[0]).
 */
void print_help(const char *progname);

/**
 * @brief Writes a matrix of document feature vectors to a file.
 * @param output_file Path to the output file.
 * @param matrix 2D array representing feature vectors.
 * @param rows Number of documents.
 * @param cols Number of features (words in dictionary).
 */
void write_feature_matrix(const char *output_file, unsigned char **matrix, int rows,
                          int cols);

#endif

