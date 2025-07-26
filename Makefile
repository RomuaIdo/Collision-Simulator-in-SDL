build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lSDL2_mixer -lSDL2_ttf -lm -o ./bin/main

run:
	./bin/main
	
clean:
	rm main
