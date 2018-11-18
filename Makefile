all:
	gcc -o irCbot main.c connection.c -I. -lpthread
