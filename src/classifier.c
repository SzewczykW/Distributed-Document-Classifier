/**
 * @file classifier.c
 * @author Wiktor Szewczyk
 * @brief Implements document classification using tokenization and a hash table.
 */

#include <string.h>

#include "classifier.h"
#include "hash_table.h"
#include "msg_consts.h"

void classify_text(const char *text, int result[], int keyword_count)
{
    memset(result, 0, sizeof(int) * keyword_count);

    char buffer[MAX_DOC_SIZE];
    strncpy(buffer, text, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    char *token = strtok(buffer, CLASSIFY_TOKENS);
    while (token)
    {
        to_lower(token);
        int index = find_word(token);
        if (index != -1)
            result[index]++;
        token = strtok(NULL, CLASSIFY_TOKENS);
    }
}

