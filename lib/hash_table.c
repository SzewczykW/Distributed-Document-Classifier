/**
 * @file hash_table.c
 * @brief Implements a chained hash table for word indexing.
 */

#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Computes a hash value for a string using a simple accumulator strategy.
 *
 * This function uses position-weighted character accumulation and mixing to
 * create a pseudo-unique integer, then maps it to a hash table index.
 *
 * @param string Null-terminated input string.
 * @return Index in the hash table (range: 0 to HASH_SIZE-1).
 */
int hash_func(const char *string)
{
    int len = strlen(string);
    int sum = 0, h = 0, a = 0;
    const char *ptr = string;

    while (ptr - string < len)
    {
        int offset = ptr - string;
        a = (*ptr) * offset;
        ptr++;
        sum += sum ^ a;
        h += a;
    }

    return (((sum) << 16) | h) % HASH_SIZE;
}

/**
 * @brief Initializes a hash table with empty chains.
 *
 * @return Pointer to newly allocated and initialized HashTable.
 */
HashTable *init_hash_table(void)
{
    HashTable *table = malloc(sizeof(HashTable));
    table->value = calloc(HASH_SIZE, sizeof(HashNode));
    return table;
}

/**
 * @brief Frees all memory associated with a hash table, including all nodes.
 *
 * @param table Pointer to the hash table to be freed.
 */
void free_hash_table(HashTable *table)
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        HashNode *node = &table->value[i];
        while (node && node->data)
        {
            HashNode *next = node->next;
            free(node->data);
            if (node != &table->value[i])
            {
                free(node);
            }
            node = next;
        }
    }
    free(table->value);
    free(table);
}

/**
 * @brief Inserts a new word and its dictionary index into the hash table.
 *
 * @param string Word to be inserted.
 * @param table HashTable pointer.
 * @param num Dictionary index assigned to the word.
 */
void hash_insert(const char *string, HashTable *table, int num)
{
    int key = hash_func(string);
    HashNode *node = &table->value[key];

    while (node->data != NULL)
    {
        if (node->next == NULL)
        {
            node->next = calloc(1, sizeof(HashNode));
        }
        node = node->next;
    }

    node->data = strdup(string);
    node->num = num;
    node->next = NULL;
}

/**
 * @brief Checks if a word exists in the hash table.
 *
 * @param table HashTable pointer.
 * @param string Word to search for.
 * @return Dictionary index if found, or 0 if not found.
 */
int hash_exist(HashTable *table, const char *string)
{
    int key = hash_func(string);
    HashNode *node = &table->value[key];

    while (node != NULL && node->data != NULL)
    {
        if (strcmp(node->data, string) == 0)
            return node->num;
        node = node->next;
    }

    return 0;
}

