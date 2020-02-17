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
#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g")

#define TEST_ERROR(error, json)                     \
    do                                              \
    {                                               \
        parse_node node;                            \
        node = mi_parser(json);                     \
        EXPECT_EQ_INT(error, node.status);          \
        EXPECT_EQ_INT(MI_NULL, mi_get_type(&node)); \
    } while (0)

#define TEST_TYPE(expect_type, json)                    \
    do                                                  \
    {                                                   \
        parse_node node;                                \
        node = mi_parser(json);                         \
        EXPECT_EQ_INT(PARSE_OK, node.status);           \
        EXPECT_EQ_INT(expect_type, mi_get_type(&node)); \
    } while (0)

#define TEST_NUMBER(expect_float, json)                       \
    do                                                        \
    {                                                         \
        parse_node node;                                      \
        node = mi_parser(json);                               \
        EXPECT_EQ_INT(PARSE_OK, node.status);                 \
        EXPECT_EQ_INT(MI_NUMBER, mi_get_type(&node));         \
        EXPECT_EQ_DOUBLE(expect_float, mi_get_number(&node)); \
    } while (0)

static void test_type()
{
    TEST_TYPE(MI_NULL, "null");
    TEST_TYPE(MI_TRUE, " true ");
    TEST_TYPE(MI_FALSE, "false ");
}

static void test_parse_error()
{
    TEST_ERROR(PARSE_INVALID_VALUE, "none");
    TEST_ERROR(PARSE_EXPECT_VALUE, "  ");
    TEST_ERROR(PARSE_ROOT_NOT_SIGNULAR, " false x");
    TEST_ERROR(PARSE_INVALID_VALUE, "+0");
}

static void test_number()
{
    TEST_NUMBER(1.0, "This");
    TEST_NUMBER(-1.2, "-1.2");
    TEST_NUMBER(1.2, "+1.2");
    TEST_NUMBER(0.0, "0");
    TEST_NUMBER(0.0, "-0");
}

static void test_parse()
{
    test_type();
    test_parse_error();
}

int main()
{
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return 0;
}