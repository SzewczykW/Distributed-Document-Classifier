#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define HASH_SIZE 100

typedef struct _node
{
    char *data;
    int num;
    struct _node *next;
} node;

typedef struct _hash_table
{
    node *value;
} hash_table;

int hash_func(char *string);
void hash_insert(char *string, hash_table *hash_tbl, int num);
hash_table *init_hash_table();
char hash_exist(hash_table hash_tbl, char *string);
void build_hash_table(char *buffer, hash_table *hash_tbl, int *words_cnt);

#endif
