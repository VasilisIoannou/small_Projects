#ifndef _ENEMY_H__
#define _ENEMY_H__

#include "screen.h"
#include "player.h"
#include "gun.h"

typedef struct Enemy_struct{
    COLOURS self_colour;
    char self_char;
    
    int x,y;
    int v;

    int dead;
    int counter;

    Player* p;
    Gun* g;
}Enemy;

Enemy* init_Enemy(int set_x,int set_y, Player* set_p,Gun* set_g);
void Enemy_draw(Enemy* e);
void Enemy_destroy(Enemy* e);
void Enemy_update(Enemy* e);

#endif
