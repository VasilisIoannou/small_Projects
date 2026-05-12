#include <stddef.h>
#include <stdlib.h>

#include "enemy.h"

Enemy* init_Enemy(int set_x,int set_y,Player* set_p){
    Enemy* new_e = malloc(sizeof(Enemy));
    if (new_e == NULL){
        return NULL;
    }

    new_e->x = set_x;
    new_e->y = set_y;
    new_e->v = 0;
    new_e->dead = 0;
    new_e->self_char = 'x';
    new_e->self_colour = RED;
    new_e->p = set_p;

    return new_e;
}

void Enemy_destroy(Enemy* e){
    if(e != NULL){
        free(e);
    }
}

void Enemy_draw(Enemy* e){
    drawCharAt(e->x,e->y,e->self_char,e->self_colour);
}

void Enemy_move(Enemy* e){
    //Do something with the player's position
    // A* ?
    return;
}


