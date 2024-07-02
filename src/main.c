#include "./headers/globals.h"
#include "./headers/macros.h"
#include "./headers/functions.h"
#include "./headers/structs.h"


int main(){
    running = initialize();

    
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
        else if (state == RUNNING || state == PAUSED) {
            process_input();
            update();
            render();
        }
    }
    free_alocatedmemory();
    destroy_window();

    return 0;
}