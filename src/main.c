#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "./headers/globals.h"
#include "./headers/functions.h"
#include "./headers/structs.h"


int main(){
    running = initialize_window();

    
    while (running)
    {
        if (state == INITIAL_SCREEN) {
            setup_initial_screen();
            process_initial_screen_input();
            render_initial_screen();
        }
        else if(state == PROCESSING){
            setup();
        }
        else if (state == RUNNING) {
            process_input();
            update();
            render();
        }
    }
    
    destroy_window();

    return 0;
}