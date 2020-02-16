#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mijson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format)                                                           \
    do                                                                                                             \
    {                                                                                                              \
        test_count++;                                                                                              \
        if (equality)                                                                                              \
            test_pass++;                                                                                           \
        else                                                                                                       \
        {                                                                                                          \
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual); \
            main_ret = 1;                                                                                          \
        }                                                                                                          \
    } while (0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

static void test_parse_null()
{
    mi_value v;
    v.type = MI_NULL;
    EXPECT_EQ_INT(PARSE_OK, mi_parser(&v, "null"));
    EXPECT_EQ_INT(MI_NULL, mi_get_type(&v));
}

static void test_parse_true()
{
    mi_value v;
    v.type = MI_TRUE;
    EXPECT_EQ_INT(PARSE_ROOT_NOT_SIGNULAR, mi_parser(&v, "true has"));
    EXPECT_EQ_INT(MI_NULL, mi_get_type(&v));
}

static void test_parse_false()
{
    mi_value v;
    v.type = MI_NULL;
    EXPECT_EQ_INT(PARSE_OK, mi_parser(&v, "false"));
    EXPECT_EQ_INT(MI_FALSE, mi_get_type(&v));
}

static void test_parse()
{
    test_parse_null();
    test_parse_true();
    test_parse_false();
}

int main()
{
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return 0;
}