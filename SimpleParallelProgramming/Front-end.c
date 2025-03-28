#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <term.h>

#define RED "\033[31m"
#define RESET "\033[0m"

// Set terminal to raw mode
void enable_raw_mode() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO | ICANON); // Disable echo and canonical mode
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// Restore default terminal settings
void disable_raw_mode() {
    struct termios normal;
    tcgetattr(STDIN_FILENO, &normal);
    normal.c_lflag |= (ECHO | ICANON); // Re-enable echo and canonical mode
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &normal);
}

enum STATE{
	RUNNING,
	END
} current_state = RUNNING;

typedef struct{
	int rows, cols;
	int center_x,center_y;
	int start_y;
	int current_option,number_of_options;
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
	self->number_of_options = 5;
}

void window_print(window* self){
	//Set Parameters
	 char options[][32] = {"Parallel Counter","Add Worker","Remove Worker","Details","Exit"};
	       
	// Clear screen
	printf("\033[2J");
	
	//Print Screen
	int x = self->center_x;
	int y = self->start_y;
	
	printf("\033[%d;%dH%s",y,x,options[0]);

	for(int i=0;i<self->cols-1;i++){
			printf("\033[%d;%dH%c",y + 3,i,'-');
	}
	
	for(int i=0;i<self->number_of_options;i++){
		if(self->current_option == i){
			printf("\033[%d;%dH%s%s%s",y+i*10,x,RED,options[i] ,RESET);
		} else {
			printf("\033[%d;%dH%s",y+i*10,x,options[i]);	
		}
       		fflush(stdout); // Ensure output is displayed immediately
	}
	
	//Print Help Section
	const int number_of_help_options = 4;
	char help_options[][32] = {"Help","Move Up: 'w' and 'k'","Move down: 's', 'j'", "Choose Option: 'e' 'l'"};
	const int help_offset = 3;
	const int constant_offset = offset*self->number_of_options;

	for(int i=0;i<self->cols-1;i++){
			printf("\033[%d;%dH%c",y+constant_offset - 3,i,'-');
	}

	for(int i=0;i<number_of_help_options;i++){
			printf("\033[%d;%dH%s",y+constant_offset+help_offset*i,x,help_options[i]);

			fflush(stdout); // Ensure output is displayed immediately
	}
}
void window_executeInput(window* self,char input){

	if(self->current_option == 1) {
		return;
	}
	if(self->current_option == 2) {
		return;
	}
	if(self->current_option == 3) {
		return;
	}
	if(self->current_option == 4) {
		current_state = END;
		return;
	}
}

void window_handleInput(window* self,char input){
	if(input == 'j' || input == 's'){
		if(self->current_option < self->number_of_options - 1) self->current_option++;
	}
	if(input == 'k' || input == 'w'){
		if(self->current_option > 1) self->current_option--;
	}
	if(input == 'e' || input == 'l'){
		window_executeInput(self,input);
	}
	return;
}
int main() {
	
	//Initialide Window
	window w;
	window_init(&w);

	printf("\033[?25l");  // Hide cursor
	printf("\033[2K");
       
	enable_raw_mode(); // Enter raw mode
		
	window_print(&w);
	char c = 'a';
	while( current_state != END && read(STDIN_FILENO, &c, 1) == 1){
		window_handleInput(&w,c);
		window_print(&w);
	}
	
	disable_raw_mode();
	
	fclose(inputFile);

	printf("\033[2J");
	printf("\033[%d;%dH%s",1,1,"goodbye...\n");	

        return 0;
}
