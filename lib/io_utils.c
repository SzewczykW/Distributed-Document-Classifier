/**
 * @file io_utils.c
 * @brief Provides CLI and I/O helper functions such as help text and output formatting.
 */

#include "io_utils.h"
#include <stdio.h>

/**
 * @brief Displays CLI usage instructions on stdout.
 *
 * @param progname The name of the program (typically argv[0]).
 */
void print_help(const char *progname)
{
    printf("Usage: %s --input <folder> --output <file>\n", progname);
    printf("Options:\n");
    printf("  --help               Show this help message\n");
    printf("  --input <folder>     Input folder with .txt/.html/.tex files\n");
    printf("  --output <file>      Output file path\n");
}

/**
 * @brief Writes a 2D feature matrix to a file in plain text format.
 *
 * Each line represents one document, formatted as:
 *   Document N: 0 1 2 ...
 *
 * @param output_file File path to write the matrix to.
 * @param matrix 2D array of size [rows][cols], representing the feature matrix.
 * @param rows Number of documents (rows).
 * @param cols Number of features (columns).
 */
void write_feature_matrix(const char *output_file, unsigned char **matrix, int rows,
                          int cols)
{
    FILE *out = fopen(output_file, "w");
    if (!out)
        return;

    for (int i = 0; i < rows; ++i)
    {
        fprintf(out, "Document %d: ", i);
        for (int j = 0; j < cols; ++j)
        {
            fprintf(out, "%d ", matrix[i][j]);
        }
        fprintf(out, "\n");
    }

    fclose(out);
}

