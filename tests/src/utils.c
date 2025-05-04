#include <criterion/criterion.h>
#include "hash_table.h"

void prepare_keywords(char *keywords[], int count)
{
    clear_hash_table();
    for (int i = 0; i < count; i++)
        insert_word(keywords[i], i);
}
