#include "../include/macros.h"
#include "../include/globals.h"
#include "../include/functions.h"
#include "../include/structs.h"


int main(int argc, char *argv[]){
    printf("Digite o coeficiente de restituição: ");
    scanf("%f", &CR);
    running = initialize();
    setup();
    setup_initial_screen();
    while (running)
    {
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
