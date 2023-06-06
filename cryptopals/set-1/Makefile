all: 
	gcc -g3 -o c1 utils.c challenge-1.c
	gcc -g3 -o c2 utils.c challenge-2.c

c1:
	gcc -g3 -o c1 utils.c challenge-1.c

c2:
	gcc -g3 -o c2 utils.c challenge-2.c

clean:
	rm -f c1
	rm -f c2

vg-c1:
	valgrind --leak-check=full --show-leak-kinds=all ./c1

vg-c2:
	valgrind --leak-check=full --show-leak-kinds=all ./c2