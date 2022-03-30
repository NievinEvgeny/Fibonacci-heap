all: Fheap.c main.c
		gcc -Wall -o Fheap $^ -lm
clean:
		rm Fheap