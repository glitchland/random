all: 
	gcc -g3 -o c1 xor.c utils.c frequency.c challenge-1.c
	gcc -g3 -o c2 xor.c utils.c frequency.c challenge-2.c
	gcc -g3 -o c3 xor.c utils.c frequency.c challenge-3.c

c1:
	gcc -g3 -o c1 xor.c utils.c frequency.c challenge-1.c

c2:
	gcc -g3 -o c2 xor.c utils.c frequency.c challenge-2.c

c3:
	gcc -g3 -o c3 xor.c utils.c frequency.c challenge-3.c

c4:
	gcc -g3 -o c4 xor.c utils.c frequency.c challenge-4.c

clean:
	rm -f c1
	rm -f c2
	rm -f c3
	rm -f c4

vg-c1:
	valgrind --leak-check=full --show-leak-kinds=all ./c1

vg-c2:
	valgrind --leak-check=full --show-leak-kinds=all ./c2

vg-c3:
	valgrind --leak-check=full --show-leak-kinds=all ./c3

vg-c4:
	valgrind --leak-check=full --show-leak-kinds=all ./c4