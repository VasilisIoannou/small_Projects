#include "bullet.h"
#include "gun.h"
#include "player.h"
#include "globals.h"
#include "log.h"

#include <stdlib.h>
#include <stdio.h>

Bullet* init_Bullet(Gun* g){
    Bullet* new_bullet = malloc(sizeof(Bullet));

    if(!new_bullet){
        fprintf(stderr, "Error: Bullet pointer is NULL in Bullet_readInput\n");
        return NULL;
    }

    new_bullet -> my_g = g;
    new_bullet->remove = 0;


    if(g->pos == DIR_UP){
        new_bullet-> x = g->x;
        new_bullet-> y = g->y-1; 
        new_bullet-> dir = DIR_UP;
    } else if(g->pos == DIR_DOWN){
        new_bullet-> x = g->x;
        new_bullet-> y = g->y+1; 
        new_bullet-> dir = DIR_DOWN;
    } else if(g->pos == DIR_LEFT){
        new_bullet-> x = g->x-1;
        new_bullet-> y = g->y; 
        new_bullet-> dir = DIR_LEFT;
    } else if(g->pos == DIR_RIGHT){
        new_bullet-> x = g->x+1;
        new_bullet-> y = g->y; 
        new_bullet-> dir = DIR_RIGHT;
    }

    new_bullet -> self_char = '.';
    new_bullet -> self_colour = BLUE;

    return new_bullet;
}

void Bullet_draw(Bullet* b){
    if(b->x < 0 || b->x > width-1 || b->y<0 || b->y>height-1) return;

    drawCharAt(b->x,b->y,b->self_char,b->self_colour);
}

void Bullet_destroy(Bullet* b){
    if(b != NULL){
        write_log("Bullet destroyed!");
        free(b);
    }
}

void Bullet_move(Bullet* b){

    if (b == NULL) return;

    if(b->dir == DIR_UP){    
        b-> y--; 
        return;
    }

    if(b->dir == DIR_DOWN){
        b-> y++; 
        return;
    }

    if(b->dir == DIR_LEFT){
        b-> x--;    
        return;
    }

    if(b->dir == DIR_RIGHT){
        b-> x++; 
        return;
    }

    if(b->x < 10 || b->x > width-2 || b->y < 10 || b->y > height-2){
        b->remove = 1;
    }
}

