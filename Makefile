all:	Gready_snake


Gready_snake:
	gcc -g  main.c Greedy_snake.c -l curses -o Gready_snake

clean:
	rm -rf *.o Gready_snake
