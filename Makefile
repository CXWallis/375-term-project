CC = gcc
CFLAGS = -O2

EXE = main

release: $(EXE)

debug: CFLAGS += -Wall -Wextra -Werror -g
debug: $(EXE)

test: main
	./main input output
	cat output

main: 
	$(CC) $(CFLAGS) main.c parser.c optimizer.c -o main


clean:
	rm -f $(EXE) $(OBJ)

.PHONY: release debug clean
