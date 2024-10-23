.DEFAULT_GOAL:=run

GCC:=gcc

main: main.c ../src/Thready.c
	$(GCC) -I . -I .. main.c ../src/Thready.c -o main

.PHONY: run
run: main
	./main

