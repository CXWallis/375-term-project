#include "optimizer.h"
#include "parser.h"

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: %s <input_filename> <output_filename>\n", argv[0]);
    return 1;
  }

  FILE *input = fopen(argv[1], "r");
  if (!input) {
    printf("Error: cannot open file %s\n", argv[1]);
    return 1;
  }

  Line *lines = parse(input);

  lines = optimize(lines);

  FILE *output = fopen(argv[2], "w");
  if (!output) {
    printf("Error: cannot open file %s\n", argv[2]);
    return 1;
  }

  fprintLine(output, lines);

  fclose(input);
  fclose(output);

  // No memory deallocation, OS is the best Garbage Collector

  return 0;
}
