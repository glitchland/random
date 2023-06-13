SUB_TARGETS := c1 c2 c3 c4 c5 c6
SRCS := base64.c frequency.c hamming.c slice.c utils.c xor.c 
VALGRIND := valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all

all: $(SUB_TARGETS)

c1:
	gcc -g3 -o c1 $(SRCS) challenge-1.c

c2:
	gcc -g3 -o c2 $(SRCS) challenge-2.c

c3:
	gcc -g3 -o c3 $(SRCS) challenge-3.c

c4:
	gcc -g3 -o c4 $(SRCS) challenge-4.c

c5:
	gcc -g3 -o c5 $(SRCS) challenge-5.c

c6:
	gcc -g3 -o c6 $(SRCS) challenge-6.c

clean:
	rm -f $(SUB_TARGETS)

vg-c1:
	$(VALGRIND) ./c1

vg-c2:
	$(VALGRIND) ./c2

vg-c3:
	$(VALGRIND) ./c3

vg-c4:
	$(VALGRIND) ./c4

vg-c5:
	$(VALGRIND) ./c5

vg-c6:
	$(VALGRIND) ./c6