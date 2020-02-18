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
#define ISDIGIT(ch) ((ch) >= '0' && (ch) <= 9)

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

static void mi_parser_null(mi_context *c, parse_node *v)
{
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
    {
        v->status = PARSE_INVALID_VALUE;
        return;
    }
    c->json += 3;
    if (!context_is_end(c))
    {
        v->status = PARSE_ROOT_NOT_SIGNULAR;
        return;
    }
    v->value.type = MI_NULL;
    return;
}

static void mi_parser_true(mi_context *c, parse_node *v)
{
    EXPECT(c, 't');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
    {
        v->status = PARSE_INVALID_VALUE;
        return;
    }
    c->json += 3;
    if (!context_is_end(c))
    {
        v->status = PARSE_ROOT_NOT_SIGNULAR;
        return;
    }
    v->value.type = MI_TRUE;
    return;
}

static void mi_parser_false(mi_context *c, parse_node *v)
{
    EXPECT(c, 'f');
    if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
    {
        v->status = PARSE_INVALID_VALUE;
        return;
    }
    c->json += 4;
    if (!context_is_end(c))
    {
        v->status = PARSE_ROOT_NOT_SIGNULAR;
        return;
    }
    v->value.type = MI_FALSE;
    return;
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
    v->n = strtod(c->json, &end);
    c->json = end;
    if (!context_is_end(c))
        return PARSE_ROOT_NOT_SIGNULAR;
    if (fabs(v->n) > DBL_MAX)
        return PARSE_NUMBER_TOO_BIG;
    v->type = MI_NUMBER;
    return PARSE_OK;
}

static int mi_parse_value(mi_context *c, mi_value *v)
{
    switch (*(c->json))
    {
    case 'n':
        mi_parser_null(c, v);
    case 't':
        mi_parser_true(c, v);
    case 'f':
        mi_parser_false(c, v);
    case '\0':
        return;
    default:
        return mi_parser_number(c, v);
    }
}

mi_type mi_get_type(const parse_node *node)
{
    return node->value.type;
}

double mi_get_number(const parse_node *node)
{
    assert(v != NULL && v->type == MI_NUMBER);
    return v->n;
}

parse_node *mi_parser(const char *json)
{
    parse_node r;
    mi_context c;
    r.value.type = MI_NULL;
    r.status = PARSE_EXPECT_VALUE;
    c.json = json;
    mi_parser_whitespace(&c);
    mi_parse_value(&c, &r);
    return &r;
}