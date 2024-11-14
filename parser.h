#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

typedef struct expr {
  enum { Int, Var, Op } type;
  union {
    ssize_t i;
    char *v;
    struct {
      char op;
      struct expr *lhs;
      struct expr *rhs;
    } *op;
  } value;
} Expr;

typedef struct line {
  char *var;
  Expr *expr;
  struct line *next;
} Line;

typedef struct parser {
  char *line;
  size_t pos;
} Parser;

char *parseVar(Parser *parser);
Expr *parseExpr(Parser *parser);
Line *parse(FILE *input);
void fprintExpr(FILE *out, Expr *expr);
void fprintLine(FILE *out, Line *line);
#endif // PARSER_H
