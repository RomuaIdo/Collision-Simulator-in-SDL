build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lm -o main

run:
	./main


clean:
	rm main