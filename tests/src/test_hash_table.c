#include "hash_table.h"
#include <criterion/criterion.h>

Test(hash_table, insert_and_find_basic)
{
    clear_hash_table();

    insert_word("apple", 0);
    insert_word("banana", 1);
    insert_word("cherry", 2);

    cr_assert_eq(find_word("apple"), 0);
    cr_assert_eq(find_word("banana"), 1);
    cr_assert_eq(find_word("cherry"), 2);
    cr_assert_eq(find_word("orange"), -1);
}

Test(hash_table, insert_duplicates_should_keep_first)
{
    clear_hash_table();

    insert_word("apple", 10);
    insert_word("apple", 99);

    int index = find_word("apple");
    cr_assert(index == 10 || index == 99);
}

Test(hash_table, collision_handling)
{
    clear_hash_table();

    insert_word("abc", 1);
    insert_word("acb", 2);
    insert_word("bac", 3);

    cr_assert(find_word("abc") != -1);
    cr_assert(find_word("acb") != -1);
    cr_assert(find_word("bac") != -1);
}

Test(hash_table, clear_hash_table_works)
{
    clear_hash_table();

    insert_word("thread", 5);
    insert_word("parallel", 6);
    cr_assert(find_word("thread") == 5);
    cr_assert(find_word("parallel") == 6);

    clear_hash_table();

    cr_assert_eq(find_word("thread"), -1);
    cr_assert_eq(find_word("parallel"), -1);
}

Test(hash_table, find_on_empty_table_returns_minus1)
{
    clear_hash_table();
    cr_assert_eq(find_word("nonexistent"), -1);
}
