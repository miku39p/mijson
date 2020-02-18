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

#define TEST_ERROR(error, json)                    \
    do                                             \
    {                                              \
        mi_value v;                                \
        v.type = MI_FALSE;                         \
        EXPECT_EQ_INT(error, mi_parser(&v, json)); \
        EXPECT_EQ_INT(MI_NULL, mi_get_type(&v));   \
    } while (0)

#define TEST_TYPE(expect_type, json)                  \
    do                                                \
    {                                                 \
        mi_value v;                                   \
        v.type = MI_NULL;                             \
        EXPECT_EQ_INT(PARSE_OK, mi_parser(&v, json)); \
        EXPECT_EQ_INT(expect_type, mi_get_type(&v));  \
    } while (0)

#define TEST_NUMBER(expect_float, json)                    \
    do                                                     \
    {                                                      \
        mi_value v;                                        \
        v.type = MI_NULL;                                  \
        EXPECT_EQ_INT(PARSE_OK, mi_parser(&v, json));      \
        EXPECT_EQ_INT(MI_NUMBER, mi_get_type(&v));         \
        EXPECT_EQ_DOUBLE(expect_float, mi_get_number(&v)); \
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
    TEST_ERROR(PARSE_ROOT_NOT_SIGNULAR, "0123");
    TEST_ERROR(PARSE_INVALID_VALUE, "+1");
    TEST_ERROR(PARSE_INVALID_VALUE, ".123");
    TEST_ERROR(PARSE_INVALID_VALUE, "1.");
    TEST_ERROR(PARSE_INVALID_VALUE, "INF");
    TEST_ERROR(PARSE_INVALID_VALUE, "inf");
    TEST_ERROR(PARSE_INVALID_VALUE, "NAN");
    TEST_ERROR(PARSE_INVALID_VALUE, "nan");
    TEST_ERROR(PARSE_ROOT_NOT_SIGNULAR, "0x0");
    TEST_ERROR(PARSE_ROOT_NOT_SIGNULAR, "0x123");
    TEST_ERROR(PARSE_NUMBER_TOO_BIG, "1e309");
    TEST_ERROR(PARSE_NUMBER_TOO_BIG, "-1e309");
}

static void test_number()
{
    TEST_NUMBER(0.0, "0");
    TEST_NUMBER(0.0, "-0");
    TEST_NUMBER(-0.1, "-0.1");
    TEST_NUMBER(1.0, "1");
    TEST_NUMBER(-1.0, "-1");
    TEST_NUMBER(1.5, "1.5");
    TEST_NUMBER(-1.5, "-1.5");
    TEST_NUMBER(3.1416, "3.1416");
    TEST_NUMBER(1E10, "1E10");
    TEST_NUMBER(1e10, "1e10");
    TEST_NUMBER(1E+10, "1E+10");
    TEST_NUMBER(1E-10, "1E-10");
    TEST_NUMBER(-1E10, "-1E10");
    TEST_NUMBER(-1e10, "-1e10");
    TEST_NUMBER(-1E+10, "-1E+10");
    TEST_NUMBER(-1E-10, "-1E-10");
    TEST_NUMBER(1.234E+10, "1.234E+10");
    TEST_NUMBER(1.234E-10, "1.234E-10");
    TEST_NUMBER(0.0, "1e-10000");
}

static void test_parse()
{
    test_type();
    test_parse_error();
    test_number();
}

int main()
{
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return 0;
}