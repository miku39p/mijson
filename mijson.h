#ifndef MIJSON_H__
#define MIJSON_H__
typedef enum
{
    MI_NULL,
    MI_FALSE,
    MI_TRUE,
    MI_NUMBER,
    MI_STRING,
    MI_ARRAY,
    MI_OBJ
} mi_type;
typedef struct
{
    /* data */
    union {
        struct
        {
            char *c;
            size_t len;
        } s;
        double n;
    } u;

    mi_type type;

} mi_value;
typedef enum
{
    PARSE_OK,
    PARSE_INVALID_VALUE,
    PARSE_EXPECT_VALUE,
    PARSE_ROOT_NOT_SIGNULAR,
    PARSE_NUMBER_TOO_BIG
} parse_return_type;
typedef struct
{
    /* data */
    const char *json;
    char* stack;
    size_t size;
    size_t top;
} mi_context;
int mi_parser(mi_value *v, const char *json);
mi_type mi_get_type(const mi_value *v);
double mi_get_number(const mi_value *v);
const char* mi_get_char(const mi_value *v);
size_t mi_get_string_length(const mi_value *v);
void mi_set_boolen(mi_value *v, int b);
void mi_set_number(mi_value *v, double n);
void mi_set_string(mi_value *v, const char *s, size_t len);
#endif
