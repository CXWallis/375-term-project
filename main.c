#include "optimizer.h"
#include "parser.h"
#include <time.h>

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

  clock_t start = clock();

  lines = optimize(lines);

  clock_t end = clock();

  printf("Optimization time: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

  FILE *output = fopen(argv[2], "w");
  if (!output) {
    printf("Error: cannot open file %s\n", argv[2]);
    return 1;
  }

  fprintLine(output, lines);

  fclose(input);
  fclose(output);

  // No memory deallocation, OS is the best Garbage Collector
  // -- i laughed so fucking hard at this this is so real
  return 0;
}
