# Collision-Simulator-in-SDL
Um simulador de colisoes simples em C utilizando a biblioteca SDL para um projeto de física

Como executar/compilar este projeto em várias plataformas:

Windows 64 bits:
Execute o main.exe na pasta build/windows-64bit
Compilar: digite make compile_win64bit na pasta de trabalho

Windows 32 bits:
Execute o main.exe na pasta build/windows-32bit
Compilar: digite make compile_win32bit na pasta de trabalho

Linux
Distros principais:

Instale as dependências:
SDL2
SDL2_mixer
SDL_ttf

Ubuntu/Debian:
sudo apt update
sudo apt install libsdl2-dev libsdl2-mixer-dev libsdl2-ttf-dev

Fedora:
sudo dnf check-update
sudo dnf install SDL2-devel SDL2_mixer-devel SDL2_ttf-devel

Arch Linux:
sudo pacman -Syu
sudo pacman -S sdl2 sdl2_mixer sdl2_ttf

OpenSUSE:
sudo zypper refresh
sudo zypper install libSDL2-devel libSDL2_mixer-devel libSDL2_ttf-devel

Gentoo:
sudo emerge --sync
sudo emerge media-libs/libsdl2 media-libs/sdl2-mixer media-libs/sdl2-ttf

Depois disso execute o arquivo main na pasta build/linux


A simples collision simulator in SDL for a physics project


How run/compile this project in multiple plataforms:

Windows 64 bit:
Execute the main.exe in the build/windows-64bit folder
Compile: type make compile_win64bit in the workspace folder

Windows 32 bit:
Execute the main.exe in the build/windows-32bit folder
Compile: type make compile_win32bit in the workspace folder

Linux
Main distros:
Install the dependencies:
SDL2
SDL2_mixer
SDL_ttf

Ubuntu/Debian:
sudo apt update
sudo apt install libsdl2-dev libsdl2-mixer-dev libsdl2-ttf-dev

Fedora:
sudo dnf check-update
sudo dnf install SDL2-devel SDL2_mixer-devel SDL2_ttf-devel

Arch Linux:
sudo pacman -Syu
sudo pacman -S sdl2 sdl2_mixer sdl2_ttf

OpenSUSE:
sudo zypper refresh
sudo zypper install libSDL2-devel libSDL2_mixer-devel libSDL2_ttf-devel

Gentoo:
sudo emerge --sync
sudo emerge media-libs/libsdl2 media-libs/sdl2-mixer media-libs/sdl2-ttf

After that execute the main in the build/linux folder
