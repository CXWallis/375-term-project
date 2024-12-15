#include "optimizer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Expr *find(Expr *expr, Line *lines) {
  if (!expr)
    return 0;
  switch (expr->type) {
  case Int:
    return expr;
  case Var:
    if (expr->forward)
      return find(expr->forward, lines);
    for (Line *line = lines; line; line = line->next) {
      if (line->var && line->expr && !strcmp(line->var, expr->value.v)) {
        expr->forward = find(line->expr, lines);
        return expr->forward;
      }
    }
  case Op:
    while (expr->forward) {
      expr = expr->forward;
    }
    return expr;
  }
  return 0;
}

void unionize(Expr *expr, Expr *forward, Line *lines) {
  if (!expr)
    return;
  switch (expr->type) {
  case Int:
    return;
  case Var:
    for (Line *line = lines; line; line = line->next) {
      if (line->var && line->expr && !strcmp(line->var, expr->value.v)) {
        line->expr = forward;
      }
    }
    return;
  case Op:
    find(expr, lines)->forward = forward;
  }
}

int equalVal(Expr *lhs, Expr *rhs) {
  if (!lhs || !rhs)
    return 0;
  if (lhs->type != rhs->type)
    return 0;
  switch (lhs->type) {
  case Int:
    return lhs->value.i == rhs->value.i;
  case Var:
    return !strcmp(lhs->value.v, rhs->value.v);
  case Op:
    return lhs == rhs;
  }
  return 0;
}

Expr *findPrevOp(Expr *l, Expr *r, char op, Line *lines) {
  for (Line *line = lines; line; line = line->next) {
    if (line->expr && line->expr->type == Op) {
      if (equalVal(line->expr->value.op->larg, l) &&
          equalVal(line->expr->value.op->rarg, r) &&
          line->expr->value.op->op == op) {
        return find(line->expr, lines);
      }
    }
  }
  return 0;
}

Line *optimize(Line *lines) {
  Line *new_lines = 0;
  Line *line = lines;
  for (; line->next; line = line->next) {
    if (line->expr && line->expr->type == Op) {
      Expr *argl = find(line->expr->value.op->larg, lines);
      Expr *argr = find(line->expr->value.op->rarg, lines);
      if (argl->type == Int && argr->type == Int) {
        ssize_t left = argl->value.i;
        ssize_t right = argr->value.i;
        Expr *i = malloc(sizeof(*i));
        i->type = Int;
        switch (line->expr->value.op->op) {
        case '+':
          i->value.i = left + right;
          unionize(line->expr, i, lines);
          break;
        case '-':
          i->value.i = left - right;
          unionize(line->expr, i, lines);
          break;
        case '*':
          i->value.i = left * right;
          unionize(line->expr, i, lines);
          break;
        case '/':
          i->value.i = left / right;
          unionize(line->expr, i, lines);
          break;
        }
        continue;
      }
      Expr *prev = findPrevOp(argl, argr, line->expr->value.op->op, new_lines);
      if (prev) {
        unionize(line->expr, prev, lines);
        continue;
      }
      if (equalVal(argl, &(Expr){.type = Int, .value.i = 0}) &&
          (line->expr->value.op->op == '+' ||
           line->expr->value.op->op == '*')) {
        switch (line->expr->value.op->op) {
        case '+':
          unionize(line->expr, argr, lines);
          break;
        case '*': {
          Expr *zero = malloc(sizeof(*zero));
          zero->type = Int;
          zero->value.i = 0;
          unionize(line->expr, zero, lines);
          break;
        }
        }
        continue;
      }
      if (equalVal(argr, &(Expr){.type = Int, .value.i = 0}) &&
          (line->expr->value.op->op == '+' ||
           line->expr->value.op->op == '*')) {
        switch (line->expr->value.op->op) {
        case '+':
          unionize(line->expr, argl, lines);
          break;
        case '*': {
          Expr *zero = malloc(sizeof(*zero));
          zero->type = Int;
          zero->value.i = 0;
          unionize(line->expr, zero, lines);
          break;
        }
        }
        continue;
      }
      Line *new_line = malloc(sizeof(*new_line));
      new_line->var = line->var;
      new_line->expr = line->expr;
      new_line->next = new_lines;
      new_lines = new_line;
    }
  }
  Line *new_line = malloc(sizeof(*new_line));
  new_line->var = line->var;
  new_line->expr = find(line->expr, lines);
  new_line->next = new_lines;
  new_lines = new_line;
  return new_lines;
}
