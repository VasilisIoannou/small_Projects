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
    uint8_t addr = p->mem[p->pc];  
    p->acc = p->mem[addr];
    printf("LOAD: acc = %x\n",p->acc);
    p->pc++;
}

void ins_add(proc* p){
    uint8_t addr = p->mem[p->pc];
    p->acc += p->mem[addr];
    printf("ADD: acc = %x\n",p->acc);
    p->pc++;
}

void ins_store(proc* p){
    uint8_t addr = p->mem[p->pc];
    p->mem[addr] = p->acc;
    printf("STORE: stored %x at %x\n",p->acc,p->pc);
    p->pc++;
}

void ins_goto(proc* p){
    uint8_t target_addr = p->mem[p->pc];
    p->pc = target_addr;
    printf("GOTO: %x\n",p->pc);
}

void ins_breq(proc* p){
    uint8_t cmp_val = p->mem[p->pc];
    uint8_t target_addr = p->mem[p->pc+1];
    
    printf("BREQ: if %x == %x goto %x \n", p->acc, cmp_val, target_addr);
    
    if(cmp_val == p->acc){
        p->pc = target_addr;         
    } else {
        p->pc += 2;                 
    }
}

void ins_halt(){
    current_state = END;
    printf("HALT\n");
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
        case 0x05:
            ins_goto(p);
            break;
        case 0x06:
            ins_breq(p);
            break;
        default: break;
    }
}

void test_program(proc* p){
    /*  The program is a simple loop:
    *   [0x00] LOAD 0x20   // Load starting value (5) from addr 0x20
    *   [0x02] ADD 0x21    // Add increment value (1) from addr 0x21
    *   [0x04] STORE 0x23  // Store the current accumulator in addr 0x23
    *   [0x06] BREQ 0x07, 0x0B // If acc == 7, branch to HALT at 0x0B
    *   [0x09] GOTO 0x02   // Otherwise, loop back to the ADD instruction
    *   [0x0B] HALT        // End of program
    */

    // 0x00: LOAD 0x20
    p->mem[0x00] = 0x01; // Opcode: LOAD
    p->mem[0x01] = 0x20; // Operand: Address 0x20

    // 0x02: ADD 0x21
    p->mem[0x02] = 0x02; // Opcode: ADD
    p->mem[0x03] = 0x21; // Operand: Address 0x21

    // 0x04: STORE 0x23
    p->mem[0x04] = 0x03; // Opcode: STORE
    p->mem[0x05] = 0x23; // Operand: Address 0x23

    // 0x06: BREQ 0x07, 0x0B
    p->mem[0x06] = 0x06; // Opcode: BREQ
    p->mem[0x07] = 0x07; // Operand 1: Immediate value to compare (7)
    p->mem[0x08] = 0x0B; // Operand 2: Target address to jump to (0x0B)

    // 0x09: GOTO 0x02
    p->mem[0x09] = 0x05; // Opcode: GOTO
    p->mem[0x0A] = 0x02; // Operand: Address 0x02 (Loops back to ADD)

    // 0x0B: HALT
    p->mem[0x0B] = 0x04; // Opcode: HALT

    // --- DATA SEGMENT ---
    
    // Starting data
    p->mem[0x20] = 0x05; // Starting value for ACC
    p->mem[0x21] = 0x01; // Number to add each loop
    
    // 0x23 is left at 0x00 by default. 
    // By the end of the simulation, if you look at your memory dump, 
    // p->mem[0x23] should contain 0x07!
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
