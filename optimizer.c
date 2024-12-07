#include "optimizer.h"
#include "parser.h"

void optimize(Line *lines) { 
  Line* line;
  size_t index = 0;
  while ( ( line = *(lines + index) ) != NULL ) {
    printf("%s", line.var );
  }
  return; 
}
