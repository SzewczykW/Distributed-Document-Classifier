#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

Node *hash_table[HASH_SIZE];

int hash_func(const char *s)
{
    int h = 0;
    for (int i = 0; s[i]; i++)
    {
        h = (h * 31 + s[i]) % HASH_SIZE;
    }
    return h;
}

void insert_word(const char *word, int index)
{
    int h = hash_func(word);
    Node *new_node = malloc(sizeof(Node));
    new_node->word = strdup(word);
    new_node->index = index;
    new_node->next = hash_table[h];
    hash_table[h] = new_node;
}

int find_word(const char *word)
{
    int h = hash_func(word);
    Node *cur = hash_table[h];
    while (cur)
    {
        if (strcmp(cur->word, word) == 0)
            return cur->index;
        cur = cur->next;
    }
    return -1;
}

void clear_hash_table(void)
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        Node *cur = hash_table[i];
        while (cur)
        {
            Node *tmp = cur;
            cur = cur->next;
            free(tmp->word);
            free(tmp);
        }
        hash_table[i] = NULL;
    }
}

void to_lower(char *str)
{
    for (; *str; ++str)
        *str = tolower(*str);
}
