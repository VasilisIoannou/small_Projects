#ifndef _GUN_H__
#define _GUN_H__

#include "screen.h"
#include "player.h"
#include "bullet.h"

typedef struct BulletNode {
    Bullet* bullet;
    struct BulletNode* next;
} BulletNode;

typedef struct Gun_struct{
 
    char self_char;
    COLOURS self_colour;
    DIRECTION pos;
    
    Player* my_p;

    int x,y;
    //int v;
    
    BulletNode* bullets;//head of Linked List

}Gun;

Gun* init_Gun();
void Gun_draw(Gun* g);
void Gun_destroy(Gun* g);
void Gun_move(Gun* g);
void Gun_shoot(Gun* g);
void Gun_readInput(Gun*g,char key);
void Gun_drawBullets(Gun* g);
void Gun_updateBullets(Gun* g);

#endif
