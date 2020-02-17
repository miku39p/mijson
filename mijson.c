#include "mijson.h"
#include <assert.h>
#include <stdlib.h>
#define EXPECT(c, ch)               \
    do                              \
    {                               \
        assert(*(c->json) == (ch)); \
        c->json++;                  \
    } while (0)

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

static int mi_parser_true(mi_context *c, parse_node *v)
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

static int mi_parser_false(mi_context *c, parse_node *v)
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
    v->n = strtod(c->json, &end);
    if (c->json == end)
        return PARSE_INVALID_VALUE;
    c->json = end;
    v->type = MI_NUMBER;
    return PARSE_OK;
}
static void mi_parse_value(mi_context *c, parse_node *v)
{
    switch (*(c->json))
    {
    case 'n':
        return mi_parser_null(c, v);
    case 't':
        return mi_parser_true(c, v);
    case 'f':
        return mi_parser_false(c, v);
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
    /*assert(v != NULL && v->type == MI_NUMBER)*/;
    return node->value.n;
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