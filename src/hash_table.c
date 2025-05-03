#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hash_func(char *string) {
    int len = strlen(string);
    int sum = 0, h = 0, a = 0;
    char *ptr = string;

    while (ptr - string < len) {
        int offset = ptr - string;
        a = (*ptr) * offset;
        sum += sum ^ a;
        h += a;
        ptr++;
    }

    return (((sum) << 16) | h) % HASH_SIZE;
}

void hash_insert(char *string, hash_table *hash_tbl, int num)
{
    int key_value = hash_func(string);
    node *nde = &(hash_tbl->value[key_value]);

    while (nde->data != NULL)
    {
        nde = nde->next;
        printf("Position was used, turn to next.\n");
    }

    nde->data = (char *) malloc(strlen(string) + 1);
    strcpy(nde->data, string);
    nde->num = num;
    nde->next = (node *) malloc(sizeof(node));
    nde->next->data = NULL;
    nde->next->next = NULL;

    printf("\"%s\" Insert success! Key value: %d\n", string, key_value);
}

hash_table *init_hash_table()
{
    int i;
    hash_table *hash_tbl = (hash_table *) malloc(sizeof(hash_table));
    hash_tbl->value = (node *) malloc(HASH_SIZE * sizeof(node));
    for (i = 0; i < HASH_SIZE; i++)
    {
        hash_tbl->value[i].data = NULL;
        hash_tbl->value[i].next = NULL;
    }
    return hash_tbl;
}

char hash_exist(hash_table hash_tbl, char *string)
{
    int key_value = hash_func(string);
    node *nde = &(hash_tbl.value[key_value]);

    if (nde->data == NULL)
        return 0;

    while (nde->next != NULL)
    {
        if (strcmp(nde->data, string) == 0)
            return nde->num;
        nde = nde->next;
    }

    return 0;
}

void build_hash_table(char *buffer, hash_table *hash_tbl, int *words_cnt)
{
    char *string;
    char *ptr = buffer;
    int i = 0;
    *words_cnt = 0;

    while (*ptr != '\0')
    {
        i = 0;
        string = (char *) malloc(50);
        memset(string, 0, 50);

        while (*ptr != ' ' && *ptr != '\0')
        {
            string[i++] = *ptr++;
        }

        *words_cnt += 1;
        hash_insert(string, hash_tbl, *words_cnt);

        if (*ptr == ' ')
            ptr++;
    }

    printf("End building hash table.\n");
}
