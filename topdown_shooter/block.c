#include <stdlib.h>

#include "block.h"
#include "screen.h"

void blocklist_addBlock(BlockNode* head,Block* new_block){
    if(new_block == NULL || head == NULL){
        return;
    }

    BlockNode* new_node = malloc(sizeof(BlockNode));

    if(new_node == NULL){
        return;
    }

    new_node->block = new_block;
    new_node->next = head->next;

    head->next = new_node;
}

Block* init_Block(int set_x,int set_y){
    Block* new_block = malloc(sizeof(Block));

    new_block->x = set_x;
    new_block->y = set_y;
    new_block->self_colour = BG_BLUE;
    new_block->self_char=' ';

    return new_block;
}

void Block_destroy(Block* block){
    if(block != NULL){
        free(block);
    }
}

void Block_draw(Block* block){
    drawCharAt(block->x,block->y,block->self_char,block->self_colour);
}

void blocklist_free(BlockNode** head) {
    if (head == NULL || *head == NULL) {
        return;
    }

    BlockNode* current = *head;
    BlockNode* next_node = NULL;

    while (current != NULL) {
        next_node = current->next;

        if (current->block != NULL) {
            Block_destroy(current->block);
        }

        free(current);

        current = next_node;
    }

    *head = NULL;
}

void blocklist_drawBlocks(BlockNode* head){
    if (head == NULL) {
        return;
    }

    BlockNode* current = head;

    while (current != NULL) {
        Block_draw(current->block);
        current = current->next;
    }

}

