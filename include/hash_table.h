/**
 * @file hash_table.h
 * @brief Hash table for dictionary word indexing.
 */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define HASH_SIZE 100

/**
 * @brief A single node in the hash table chain.
 */
typedef struct node
{
    char *data;           /**< Stored word */
    int num;              /**< Dictionary index */
    struct node *next;    /**< Next node in the chain */
} HashNode;

/**
 * @brief Hash table structure.
 */
typedef struct
{
    HashNode *value; /**< Array of hash slots (buckets) */
} HashTable;

/**
 * @brief Computes a hash for a given string.
 *
 * @param string Input string.
 * @return Hash index in range [0, HASH_SIZE).
 */
int hash_func(const char *string);

/**
 * @brief Allocates and initializes a new hash table.
 * @return Pointer to a new hash table.
 */
HashTable *init_hash_table(void);

/**
 * @brief Frees all memory associated with the hash table.
 * @param table Hash table to free.
 */
void free_hash_table(HashTable *table);

/**
 * @brief Inserts a string into the hash table.
 *
 * @param string Word to insert.
 * @param table Target hash table.
 * @param num Index to assign to the word.
 */
void hash_insert(const char *string, HashTable *table, int num);

/**
 * @brief Checks whether a word exists in the hash table.
 *
 * @param table Hash table to search.
 * @param string Word to find.
 * @return Index of the word if found, otherwise 0.
 */
int hash_exist(HashTable *table, const char *string);

#endif

