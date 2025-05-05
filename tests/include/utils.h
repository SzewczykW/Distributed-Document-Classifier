/**
 * @file utils.h
 * @author Wiktor Szewczyk
 * @brief Header for helper functions used in test suites.
 */

#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Clears the global hash table and inserts provided test keywords.
 *
 * @param keywords Array of keyword strings
 * @param count Number of keywords
 */
void prepare_keywords(char *keywords[], int count);

#endif
