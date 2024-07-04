#include "./headers/macros.h"
#include "./headers/globals.h"
#include "./headers/functions.h"
#include "./headers/structs.h"


int main(int argc, char *argv[]){
    running = initialize();

    
    while (running)
    {
        if(state == PROCESSING){
            setup_initial_screen();
            setup();
        }
        if (state == INITIAL_SCREEN){
            process_initial_screen_input();
            update();
            render();
        }
        else if (state == RUNNING || state == PAUSED) {
            process_input();
            update();
            render();
        }
    }
    destroy_window();

    return 0;
}