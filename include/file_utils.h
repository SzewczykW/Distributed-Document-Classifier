/**
 * @file file_utils.h
 * @author Wiktor Szewczyk
 * @brief Utility functions for files operations.
 */

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <limits.h>
#include <stdio.h>

/**
 * @brief Lists all .txt files in a directory.
 *
 * @param dir_path Path to the directory
 * @param files Output array of full file paths
 * @param max_files Maximum number of files to find
 * @return Number of files found, or -1 on error
 */
int list_txt_files(const char *dir_path, char files[][PATH_MAX], int max_files);

/**
 * @brief Reads entire content of a file into buffer.
 *
 * @param filename File to read
 * @param buffer Destination buffer
 * @param bufsize Maximum size of buffer
 * @return 0 on success, -1 on error
 */
int read_file_content(const char *filename, char *buffer, size_t bufsize);

/**
 * @brief Opens the result output file for writing.
 *
 * @param output_path Output file path
 * @return FILE* handle or NULL on failure
 */
FILE *open_output_file(const char *output_path);

/**
 * @brief Reads dictionary and builds global keyword list and hash table.
 *
 * @param dict_path Path to dictionary file
 * @param keywords Output array of allocated keyword strings
 * @param max_keywords Maximum allowed keyword count
 * @return Number of keywords read, or -1 on error
 */
int read_dictionary(const char *dict_path, char *keywords[], int max_keywords);

/**
 * @brief Broadcasts dictionary from manager to workers.
 *
 * @param keywords Array of keyword strings
 * @param num_keywords Number of keywords
 */
void broadcast_dictionary(char *keywords[], int num_keywords);

/**
 * @brief Receives dictionary in a worker and builds local keyword array and hash table.
 *
 * @param keywords Output array of keyword strings
 * @param num_keywords Output number of keywords
 */
void receive_dictionary(char *keywords[], int *num_keywords);

#endif
