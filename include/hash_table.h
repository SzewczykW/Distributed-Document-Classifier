#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define HASH_SIZE 101

typedef struct node {
    char *word;
    int index;
    struct node *next;
} Node;

extern Node *hash_table[HASH_SIZE];

int hash_func(const char *s);
void insert_word(const char *word, int index);
int find_word(const char *word);
void clear_hash_table(void);
void to_lower(char *str);

#endif

