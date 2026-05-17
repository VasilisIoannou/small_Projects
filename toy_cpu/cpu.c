#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MEM_SIZE 256

//This is a toy vertion of a CPU emulator
//My goal is to create a simple CPU before moving to something harder like CHIP-8

typedef enum STATE{
    RUNNING,
    END
}state;

state current_state = RUNNING;

typedef struct PROC{
    uint8_t mem[MEM_SIZE];
    uint8_t pc;
    uint8_t acc;

    uint8_t curr_opcode;
}proc;

proc* init_proc(){
    proc* new_proc = malloc(sizeof(proc));
    new_proc->pc = 0;
    new_proc->acc = 0;

    for(int i=0;i<MEM_SIZE;i++){
        new_proc->mem[i] = 0;
    }

    new_proc->curr_opcode = 0;

    return new_proc;
}

void proc_printMem(proc* p){
    for(int i=0;i<MEM_SIZE;i++){
        if(p->mem[i] != 0){
            printf("%x: %x\n",i,p->mem[i]);
        }
    }
}

void proc_destroy(proc* p){
    if(p != NULL){
        free(p);
    }
}

void proc_fetch(proc* p){
    p->curr_opcode = p->mem[p->pc];
    p->pc++;
}

void ins_load(proc* p){
    p->acc = p->mem[p->pc];
    printf("LOAD: acc = %x\n",p->acc);
}

void ins_add(proc* p){
    p->acc += p->mem[p->pc];
    printf("ADD: acc = %x\n",p->acc);
}

void ins_store(proc* p){
    uint8_t addr = p->mem[p->pc];
    p->mem[addr] = p->acc;
    printf("STORE: stored %x at %x\n",p->acc,p->pc);
}
void ins_halt(){
    current_state = END;
}

void proc_decAndExec(proc* p){
    switch(p->curr_opcode){
        case 0x01: // LOAD
            ins_load(p);
            break;
        case 0x02: //ADD
            ins_add(p);
            break;
        case 0x03: // STORE
            ins_store(p);
            break;
        case 0x04:
            ins_halt();
            break;
        default: break;
    }
    p->pc++;
}

void test_program(proc* p){
    /*  The program is the following
    *   LOAD 0x10 = 5
    *   ADD 0x11 = 7
    *   STORE 0x12
    *   END
    */

    //LOAD 0x10
    p->mem[0x00] = 0x01;
    p->mem[0x01] = 0x10;

    //ADD 0x11
    p->mem[0x02] = 0x02;
    p->mem[0x03] = 0x11;

    //STORE 0x12
    p->mem[0x04] = 0x03;
    p->mem[0x05] = 0x12;

    //END
    p->mem[0x06] = 0x04;

    //Data
    p->mem[0x10] = 0x05;
    p->mem[0x11] = 0x07;

}

int main(int argc, char argv[]){
    //I will later load the memory from the file (as a program)

    proc* p;

    p = init_proc();
    if(p == NULL){
        printf("There was an error with initializing the processor\n");
        return 1;
    }

    //load a test program
    test_program(p);

    printf("CPU simulation starting ... \n");
    while(current_state == RUNNING){
        //fetch
        proc_fetch(p);

        //decode & execute
        proc_decAndExec(p);
    }

    proc_printMem(p);

    proc_destroy(p);
}
