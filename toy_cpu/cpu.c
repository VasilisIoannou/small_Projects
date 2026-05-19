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

void ins_LDA(proc* p){
    uint8_t addr = p->mem[p->pc];  
    p->acc = p->mem[addr];
    printf("LDA: acc = %x\n",p->acc);
    p->pc++;
}

void ins_LDI(proc* p){ // load immidiate to acc
    p->acc = p->mem[p->pc];
    printf("LDI: acc = %x\n",p->acc);
    p->pc++;
}

void ins_ADDA(proc* p){
    uint8_t addr = p->mem[p->pc];
    p->acc += p->mem[addr];
    printf("ADDA: acc = %x\n",p->acc);
    p->pc++;
}

void ins_ADDI(proc* p){
    p->acc += p->mem[p->pc];
    printf("ADDI: acc = %x\n",p->acc);
    p->pc++;
}

void ins_STA(proc* p){
    uint8_t addr = p->mem[p->pc];
    p->mem[addr] = p->acc;
    printf("STORE: stored %x at %x\n",p->acc,p->pc);
    p->pc++;
}

void ins_GOTO(proc* p){
    uint8_t target_addr = p->mem[p->pc];
    p->pc = target_addr;
    printf("GOTO: %x\n",p->pc);
}

void ins_BREQ(proc* p){
    uint8_t cmp_val = p->mem[p->pc];
    uint8_t target_addr = p->mem[p->pc+1];
    
    printf("BREQ: if %x == %x goto %x \n", p->acc, cmp_val, target_addr);
    
    if(cmp_val == p->acc){
        p->pc = target_addr;         
    } else {
        p->pc += 2;                 
    }
}

void ins_END(){
    current_state = END;
    printf("HALT\n");
}

void proc_decAndExec(proc* p){
    switch(p->curr_opcode){
        case 0x01: // LDA
            ins_LDA(p);
            break;
        case 0x02:
            ins_LDI(p);
            break;
        case 0x03: //ADDA
            ins_ADDA(p);
            break;
        case 0x04: //ADDA
            ins_ADDI(p);
            break;
        case 0x05: // STORE
            ins_STA(p);
            break;
        case 0x06:
            ins_END();
            break;
        case 0x07:
            ins_GOTO(p);
            break;
        case 0x08:
            ins_BREQ(p);
            break;
        default: break;
    }
}


void load_program(proc* p, char* filename){
    FILE* load = fopen(filename, "r");

    if(load == NULL){
        printf("There was an error with reading the file\n");
        return;
    }

    char line[10]; 
    int address = 0; 

    while (fgets(line, sizeof(line), load) != NULL){
        uint8_t value = (uint8_t)strtol(line, NULL, 16);

        if (address < MEM_SIZE) {
            p->mem[address] = value;
            address++;
        } else {
            printf("Warning: Program file is larger than CPU memory!\n");
            break;
        }
    }

    fclose(load); 
}

int main(int argc, char *argv[]){
    //I will later load the memory from the file (as a program)

    proc* p;

    p = init_proc();
    if(p == NULL){
        printf("There was an error with initializing the processor\n");
        return 1;
    }

    //load a test program
    load_program(p,argv[1]);

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
