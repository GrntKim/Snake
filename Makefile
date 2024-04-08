snake: snake.o
	gcc -o snake snake.o -lncurses
	rm snake.o

snake.o:
	gcc -c snake.c
clean: 
	rm snake