/**
 * @file profile.h
 * @brief Functions for building feature vectors from text documents using a dictionary.
 */

#ifndef PROFILE_H
#define PROFILE_H

#include "hash_table.h"

/**
 * @brief Parses a document and builds its feature vector based on dictionary words.
 *
 * @param filepath Path to the text file.
 * @param dict Hash table containing dictionary words.
 * @param profile Output vector to fill with word occurrence counts.
 * @param dict_size Size of the dictionary (feature vector length).
 */
void build_profile_from_file(const char *filepath, HashTable *dict,
                             unsigned char *profile, int dict_size);

#endif

