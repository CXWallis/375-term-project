#include "parser.h"
#include "string.h"

// definitely totally doesnt clash indexes for sure
int dumb_hash( char* var ) {
  int hash = 0;
  for ( int i = 0; i < strlen( var ); ++i ) {
    hash += ((int)*(var + i) - 65) * 10 / (i + 1);
  }
  printf("accessing: %d", hash);
  return hash;
}

int bruteforce( Line* lines ) {
  
  dumb_hash("ZZ");
  int items[1000];
  Line* new_lines = 0;
  Line* line = lines;
  for (; line->next; line = line->next) {
    if ( !line->expr || line->expr->type != Op ) {
      items[dumb_hash(line->var)] = line->expr->value.i;
      continue;
    }
    if ( line->expr && line->expr->type == Op ) {
      int value = 0;
      switch (line->expr->value.op->op) {
        case '+': 
          items[dumb_hash(line->var)] = items[dumb_hash(line->expr->value.op->larg->value.v)] + items[dumb_hash(line->expr->value.op->rarg->value.v)];
          break;
        case '-': 
          items[dumb_hash(line->var)] = items[dumb_hash(line->expr->value.op->larg->value.v)] + items[dumb_hash(line->expr->value.op->rarg->value.v)];
          break;
        case '*':
          items[dumb_hash(line->var)] = items[dumb_hash(line->expr->value.op->larg->value.v)] + items[dumb_hash(line->expr->value.op->rarg->value.v)];
          break;
        case '/': 
          items[dumb_hash(line->var)] = items[dumb_hash(line->expr->value.op->larg->value.v)] + items[dumb_hash(line->expr->value.op->rarg->value.v)];
          break;
      }
    }
  }
  return 0;
}
