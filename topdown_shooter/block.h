#ifndef _BLOCK_H__
#define _BLOCK_H__

#include "screen.h"

typedef struct Block_struct{
    int x,y;
    COLOURS self_colour;
    char self_char;
}Block;

typedef struct Block_node_struct{
    Block* block;
    struct Block_node_struct* next;
}BlockNode;


void Block_draw(Block* block);
void Block_destroy(Block* block);
Block* init_Block(int set_x,int set_y);
void blockList_drawBlocks(BlockNode* head);
void blockList_free(BlockNode** head) ;
void blockList_addBlock(BlockNode* head,Block* new_block);

#endif
