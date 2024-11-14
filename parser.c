#include "parser.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *parseVar(Parser *parser) {
  while (isspace(parser->line[parser->pos])) {
    parser->pos += 1;
  }
  size_t initial = parser->pos;
  if (isalpha(parser->line[parser->pos]) || parser->line[parser->pos] == '_') {
    parser->pos += 1;
  } else {
    printf("Error: invalid variable\n");
    return 0;
  }
  while (isalnum(parser->line[parser->pos]) ||
         parser->line[parser->pos] == '_') {
    parser->pos += 1;
  }
  char *allocated = malloc(parser->pos - initial + 1);
  memcpy(allocated, parser->line + initial, parser->pos - initial);
  return allocated;
}

ssize_t parseInt(Parser *parser) {
  ssize_t value = 0;
  while (isdigit(parser->line[parser->pos])) {
    value = value * 10 + parser->line[parser->pos] - '0';
    parser->pos += 1;
  }
  return value;
}

Expr *parseSimpleExpr(Parser *parser) {
  while (isspace(parser->line[parser->pos])) {
    parser->pos += 1;
  }

  Expr *expr = malloc(sizeof(*expr));

  if (isdigit(parser->line[parser->pos])) {
    expr->type = Int;
    expr->value.i = parseInt(parser);
    return expr;
  }

  if (isalpha(parser->line[parser->pos]) || parser->line[parser->pos] == '_') {
    expr->type = Var;
    expr->value.v = parseVar(parser);
    return expr;
  }

  printf("Error: invalid simple expression\n");
  return 0;
}

Expr *parseExpr(Parser *parser) {
  while (isspace(parser->line[parser->pos])) {
    parser->pos += 1;
  }

  Expr *expr = malloc(sizeof(*expr));

  if (isdigit(parser->line[parser->pos])) {
    expr->type = Int;
    expr->value.i = parseInt(parser);
    return expr;
  }

  if (isalpha(parser->line[parser->pos]) || parser->line[parser->pos] == '_') {
    expr->type = Var;
    expr->value.v = parseVar(parser);
    return expr;
  }

  switch (parser->line[parser->pos]) {
  case '+':
  case '-':
  case '*':
  case '/':
    expr->type = Op;
    expr->value.op = malloc(sizeof(*expr->value.op));
    expr->value.op->op = parser->line[parser->pos];
    parser->pos += 1;
    expr->value.op->lhs = parseSimpleExpr(parser);
    expr->value.op->rhs = parseSimpleExpr(parser);
    if (!expr->value.op->lhs || !expr->value.op->rhs) {
      return 0;
    }
    return expr;
  default:
    printf("Error: invalid expression\n");
    return 0;
  }
}

Line *parse(FILE *input) {
  Line *head = 0;
  Line **tail = &head;
  char *line = 0;
  while ((getline(&line, &(size_t){0}, input)) != -1) {
    Parser parser = {line, 0};
    Line *l = malloc(sizeof(*l));
    l->next = 0;
    l->var = parseVar(&parser);
    if (!l->var) {
      printf("Error: failed to parse variable\n");
      return 0;
    }
    l->expr = parseExpr(&parser);
    if (!l->expr) {
      printf("Error: failed to parse expression\n");
      return 0;
    }
    *tail = l;
    tail = &l->next;
  }
  return head;
}

void fprintExpr(FILE *out, Expr *expr) {
  switch (expr->type) {
  case Int:
    fprintf(out, "%ld", expr->value.i);
    break;
  case Var:
    fprintf(out, "%s", expr->value.v);
    break;
  case Op:
    fprintf(out, "%c ", expr->value.op->op);
    fprintExpr(out, expr->value.op->lhs);
    fprintf(out, " ");
    fprintExpr(out, expr->value.op->rhs);
    break;
  }
}

void fprintLine(FILE *out, Line *line) {
  fprintf(out, "%s ", line->var);
  fprintExpr(out, line->expr);
  fprintf(out, "\n");
  if (line->next) {
    fprintLine(out, line->next);
  }
}
