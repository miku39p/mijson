#include "mijson.h"
#include <assert.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#define EXPECT(c, ch)               \
    do                              \
    {                               \
        assert(*(c->json) == (ch)); \
        c->json++;                  \
    } while (0)
#define ISDIGIT(ch) ((ch) >= '0' && (ch) <= '9')

static void mi_parser_whitespace(mi_context *c)
{
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
    {
        p++;
    }
    c->json = p;
}

static int context_is_end(mi_context *c)
{
    mi_parser_whitespace(c);
    if (c->json[0] == '\0')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static int mi_parser_null(mi_context *c, mi_value *v)
{
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
    {
        return PARSE_INVALID_VALUE;
    }
    c->json += 3;
    if (!context_is_end(c))
    {
        return PARSE_ROOT_NOT_SIGNULAR;
    }
    v->type = MI_NULL;
    return PARSE_OK;
}

static int mi_parser_true(mi_context *c, mi_value *v)
{
    EXPECT(c, 't');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
    {
        return PARSE_INVALID_VALUE;
    }
    c->json += 3;
    if (!context_is_end(c))
    {
        return PARSE_ROOT_NOT_SIGNULAR;
    }
    v->type = MI_TRUE;
    return PARSE_OK;
}

static int mi_parser_false(mi_context *c, mi_value *v)
{
    EXPECT(c, 'f');
    if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
    {
        return PARSE_INVALID_VALUE;
    }
    c->json += 4;
    if (!context_is_end(c))
    {
        return PARSE_ROOT_NOT_SIGNULAR;
    }
    v->type = MI_FALSE;
    return PARSE_OK;
}
static int mi_parser_number(mi_context *c, mi_value *v)
{
    char *end;
    const char *start = c->json;
    if (*(c->json) != '-' && (!(ISDIGIT(*(c->json)))))
    {
        return PARSE_INVALID_VALUE;
    }
    while (*start != '\0')
    {
        if (*start == '.' && !ISDIGIT(*(start+1)))
        {
            return PARSE_INVALID_VALUE;
        }
        start++;
    }
    if (*(c->json) == '0')
    {
        if (c->json[1] != '.' && c->json[1] != 'e' && c->json[1] != 'E' && c->json[1] != '\0' && c->json[1] != ' ')
        {
            return PARSE_ROOT_NOT_SIGNULAR;
        }
    }
    v->u.n = strtod(c->json, &end);
    c->json = end;
    if (!context_is_end(c))
        return PARSE_ROOT_NOT_SIGNULAR;
    if (fabs(v->u.n) > DBL_MAX)
        return PARSE_NUMBER_TOO_BIG;
    v->type = MI_NUMBER;
    return PARSE_OK;
}

 void mi_set_string(mi_value*v, const char *s, size_t len){
     assert(v != NULL &&(s!=NULL || len == 0));
     
 }

static int mi_parse_value(mi_context *c, mi_value *v)
{
    v->type = MI_NULL;
    switch (*(c->json))
    {
    case 'n':
        return mi_parser_null(c, v);
    case 't':
        return mi_parser_true(c, v);
    case 'f':
        return mi_parser_false(c, v);
    case '\0':
        return PARSE_EXPECT_VALUE;
    default:
        return mi_parser_number(c, v);
    }
}

mi_type mi_get_type(const mi_value *v)
{
    return v->type;
}

double mi_get_number(const mi_value *v)
{
    assert(v != NULL && v->type == MI_NUMBER);
    return v->u.n;
}
int mi_parser(mi_value *v, const char *json)
{
    mi_context c;
    /*assert(&v != NULL)*/;
    c.json = json;
    mi_parser_whitespace(&c);
    return mi_parse_value(&c, v);
}