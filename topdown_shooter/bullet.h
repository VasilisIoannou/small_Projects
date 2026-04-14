#ifndef _BULLET_H__
#define _BULLET_H__

#include "player.h"
#include "screen.h"

typedef struct Gun_struct Gun;

typedef struct Bullet_struct{

    int x,y;
    int v;
    DIRECTION dir;

    int remove;

    char self_char;
    COLOURS self_colour;

    Gun* my_g;

}Bullet;

Bullet* init_Bullet(Gun* set_g);
void Bullet_draw(Bullet* b);
void Bullet_destroy(Bullet* b);
void Bullet_move(Bullet* b);


#endif
