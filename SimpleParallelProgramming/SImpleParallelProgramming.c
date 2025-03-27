#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

typedef struct{
        int rows, cols;
        int center_x,center_y;
}window;

void window_init(window* self){
        if (self == NULL) {
                fprintf(stderr, "Error: NULL window pointer passed to window_init\n");
                exit(EXIT_FAILURE);
        }

        struct winsize w;

        if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1 ){
                perror("ioctl error");
                // Set default values if ioctl fails
                self->rows = 24;
                self->cols = 80;
        } else {
                self->rows = w.ws_row;
                self->cols = w.ws_col;
        }

        self->center_x = w.ws_row / 2;
        self->center_y = w.ws_col / 2;
}

void window_print(window* self,int x,int y,char* text){
        // Clear screen
        printf("\033[2J");

        //Print Text
        printf("\033[%d;%dH%s", y, x, text);
        fflush(stdout); // Ensure output is displayed immediately
}

int main() {

        window w;
        window_init(&w);

        printf("\033[?25l");  // Hide cursor
        printf("\033[2K");
        window_print(&w,10,10,"Hello World\n");

        while(1){
                sleep(5);
        }
        return 0;
}