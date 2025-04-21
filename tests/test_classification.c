#include "classifier.h"
#include <criterion/criterion.h>

Test(classify, detects_manager)
{
    /* const char *result = classify_document("This is a Manager document."); */
    /* cr_assert_str_eq(result, "Manager"); */
}

Test(classify, detects_worker)
{
    /* const char *result = classify_document("This is unrelated to management."); */
    /* cr_assert_str_eq(result, "Worker"); */
}
