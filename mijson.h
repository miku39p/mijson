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
    double n;
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
} mi_context;
typedef struct 
{
    /* data */
    int status;
    mi_value value;
}parse_node;

parse_node* mi_parser(const char *json);
mi_type mi_get_type(const parse_node *n);

double mi_get_number(const parse_node *n);
#endif
