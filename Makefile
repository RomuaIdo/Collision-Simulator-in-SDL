compile:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lSDL2_mixer -lSDL2_ttf -lm -o ./build/linux/main
	
compile_win32bit:
	i686-w64-mingw32-gcc -I src/include/w32-bit -L src/lib/w32-bit -Wall -std=c99 ./src/*.c -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf -lm -o ./build/windows-32bit/main.exe

compile_win64bit:
	x86_64-w64-mingw32-gcc -I src/include/w64-bit -L src/lib/w64-bit -Wall -std=c99 ./src/*.c -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf -lm -o ./build/windows-64bit/main.exe

run:
	./main
	
clean:
	rm main
