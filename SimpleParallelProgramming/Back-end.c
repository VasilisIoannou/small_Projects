#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <term.h>

typedef struct{
	pid_t* worker_list;
	int current_count;
	FILE* inputFile;
}controller;

void controller_init(controller* self,FILE* file){
	self->current_count = 0;
	self->inpitFile = file;
}

void controller_addWorker(controller* self){
	return;
}

typedef struct{
	int count;
	pid_t pid;
	FILE* inputFile;
	long start,end;
}worker;

void worker_init(worker* self,pid_t setPid,FILE* file){
	self-> pid = setPid;
	self-> inputFile = file;
}

int main(){
	//Manage Input File
	char fileName[32] = "text.txt";
	FILE* inputFile = fopen(fileName,"r");
	
	//Initialise Controller
	controller c;
	controller_init(&c,inputFile);
	
	return 0;
}