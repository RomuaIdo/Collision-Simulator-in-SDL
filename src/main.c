#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "./headers/globals.h"
#include "./headers/functions.h"
#include "./headers/structs.h"


int main(){
    running = initialize_window();

    setup();
    while (running)
    {
        process_input();
        update();
        render();
    }
    
    destroy_window();

    return 0;
}