#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <term.h>

#define RED "\033[31m"
#define RESET "\033[0m"

typedef struct{
        int rows, cols;
        int center_x,center_y;
	int start_y;
	int current_option;
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

	self->start_y = 5;
	self->current_option = 1;
}

void window_print(window* self){
        //Set Parameters
	 const int number_of_options = 5;
	 char options[][32] = {"Parallel Counter","Add Worker","Remove Worker","Details","Exit"};
	       
	// Clear screen
        printf("\033[2J");
	
        //Print Screen
	int x = self->center_x;
	int y = self->start_y;
	
	for(int i=0;i<number_of_options;i++){
		if(self->current_option == i){
			printf("\033[%d;%dH%s%s%s",y+i*10,x,RED,options[i] ,RESET);
		} else {
			printf("\033[%d;%dH%s",y+i*10,x,options[i]);	
		}
       		fflush(stdout); // Ensure output is displayed immediately
	}
}

void window_handleInput(window* self,int input){
	return;
}
int main() {

        window w;
        window_init(&w);

        printf("\033[?25l");  // Hide cursor
        printf("\033[2K");
       
        window_print(&w);
	
	while(1){
                sleep(5);
        }
        return 0;
}
