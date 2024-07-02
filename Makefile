build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lSDL2_mixer -lm -o main

run:
	./main


clean:
	rm main