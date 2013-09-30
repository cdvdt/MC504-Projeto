all:
	gcc -c anim.c
	gcc -c sprites.c
	gcc -o anim anim.o sprites.o -lpthread
	
