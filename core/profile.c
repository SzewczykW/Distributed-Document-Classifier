/**
 * @file profile.c
 * @brief Builds feature vectors for documents using a hash table dictionary.
 */

#include "profile.h"
#include <ctype.h>
#include <stdio.h>

/**
 * @brief Parses the content of a document and updates its feature vector.
 *
 * The function reads the file at the given `filepath`, tokenizes words using
 * whitespace as delimiters, filters non-alphabetic characters, and checks each
 * word against the dictionary. If found, the corresponding entry in the profile
 * vector is incremented.
 *
 * @param filepath Path to the document file.
 * @param dict Dictionary represented as a hash table.
 * @param profile Output array (feature vector) updated in-place.
 * @param dict_size Total number of features in the dictionary.
 */
void build_profile_from_file(const char *filepath, HashTable *dict,
                             unsigned char *profile, int dict_size)
{
    FILE *file = fopen(filepath, "r");
    if (!file)
        return;

    char word[128];
    int c, i = 0;

    while ((c = fgetc(file)) != EOF)
    {
        if (isspace(c))
        {
            // Word boundary detected
            if (i > 0)
            {
                word[i] = '\0';
                int idx = hash_exist(dict, word);
                if (idx > 0 && idx <= dict_size)
                    profile[idx - 1]++;
                i = 0;
            }
        }
        else if (isalpha(c))
        {
            // Build lowercase word from alphabetic characters
            if (i < (int)(sizeof(word) - 1))
                word[i++] = tolower(c);
        }
    }

    // Handle trailing word (if file doesn't end with whitespace)
    if (i > 0)
    {
        word[i] = '\0';
        int idx = hash_exist(dict, word);
        if (idx > 0 && idx <= dict_size)
            profile[idx - 1]++;
    }

    fclose(file);
}

