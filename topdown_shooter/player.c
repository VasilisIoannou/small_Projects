#include <stdlib.h>

#include "player.h"
#include "screen.h"

Player* init_Player(){
    Player* new_p = malloc(sizeof(Player));
    if (new_p == NULL) {
        return NULL; // Or handle the error appropriately
    }

    new_p-> x = START_X;
    new_p->y = START_Y;
    new_p->v = 0;
    new_p->self_char = '@';
    new_p->self_colour = RED;

    return new_p;
}

void Player_destroy(Player* p){
    if (p != NULL) {
        free(p);
    }
}

void Player_draw(Player* p){
    drawCharAt(p->x,p->y,p->self_char,p->self_colour);
}

void Player_move(Player* p,DIRECTION dir){
    if(dir == DIR_RIGHT){
        p->x--;
        return;
    }
    if(dir == DIR_LEFT){
        p->x++;
        return;
    }
    if(dir == DIR_UP){
        p->y--;
        return;
    }
    if(dir == DIR_DOWN){
        p->y++;
        return;
    }
}

void Player_readInput(Player* p,char key){
    if(key == 'w'){
        Player_move(p,DIR_UP); 
        return;
    }
    if(key == 's'){
        Player_move(p,DIR_DOWN);  
        return;
    }
    if(key == 'd'){
        Player_move(p,DIR_LEFT); 
        return;
    }
    if(key == 'a'){
        Player_move(p,DIR_RIGHT); 
        return;
    }
}

