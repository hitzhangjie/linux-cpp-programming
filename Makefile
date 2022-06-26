all:
	@gcc -g -o main -O0 *.c
	@./main

clean:
	@rm -rf main

PHONY: clean