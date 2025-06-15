/**
 * @file hash_table.h
 * @author Wiktor Szewczyk
 * @brief Simple fixed-size chained hash table for word lookup.
 */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/**
 * @brief Size of the hash table (number of buckets).
 *
 * The hash table uses fixed-size separate chaining with linked lists.
 * A prime number is chosen to reduce the likelihood of collisions
 * and distribute keys uniformly.
 */
#define HASH_SIZE 101

/**
 * @struct Node
 * @brief Represents a dictionary word in a linked list at a hash slot.
 */
typedef struct node
{
    char *word;        ///< Keyword string
    int index;         ///< Position in feature vector
    struct node *next; ///< Next node in chain
} Node;

/// Global hash table structure
extern Node *hash_table[HASH_SIZE];

/**
 * @brief Hash function for a word.
 *
 * @param s Word to hash
 * @return Index in hash table
 */
int hash_func(const char *s);

/**
 * @brief Inserts a keyword into the hash table.
 *
 * @param word Keyword string
 * @param index Index in the dictionary
 */
void insert_word(const char *word, int index);

/**
 * @brief Finds a word in the hash table.
 *
 * @param word Word to find
 * @return Index in dictionary, or -1 if not found
 */
int find_word(const char *word);

/**
 * @brief Frees all allocated hash table entries.
 */
void clear_hash_table(void);

/**
 * @brief Converts a string to lowercase in-place.
 *
 * @param str String to lowercase
 */
void to_lower(char *str);

#endif
