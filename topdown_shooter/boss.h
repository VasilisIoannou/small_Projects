#ifndef _BOSS_H__
#define _BOSS_H__

#include "screen.h"
#include "gun.h"

typedef enum BOSS_STAGE{
    FIRST,
    LAST
}Boss_Stage;

typedef struct boss_struct{
    
    int hp;
    Boss_Stage stage;

    char self_char;
    int size;
    int dead;
    int x,y;

    COLOURS self_colour;

    Gun* g;

}Boss;

Boss* init_Boss(Gun* set_g);
void Boss_destroy(Boss* b);
void Boss_update(Boss* b);
void Boss_draw(Boss* b);

#endif
