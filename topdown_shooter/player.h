#ifndef _PLAYER_H__
#define _PLAYER_H__

#include "screen.h"
#include "player.h"

#define START_X 5
#define START_Y 5

typedef enum Direction_Enum{
    DIR_RIGHT,
    DIR_LEFT,
    DIR_UP,
    DIR_DOWN
}DIRECTION;

typedef struct Player_struct{
 
    char self_char;
    COLOURS self_colour;

    int x,y;
    int v;

}Player;

Player* init_Player();
void Player_draw(Player* p);
void Player_destroy(Player* p);
void Player_move(Player* p,DIRECTION dir);
void Player_readInput(Player* p,char key);

#endif
