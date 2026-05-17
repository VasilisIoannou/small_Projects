#ifndef _CONTROLLER_H__
#define _CONTROLLER_H__

#include <stdlib.h>

#include "gun.h"

typedef enum States_Enum{
    STATE_IDLE,
    STATE_PLAY,
    STATE_END_SCENE,
    STATE_END_GAME
}STATE;

typedef struct Controller_struct{

    STATE current_state;
    Player* p;
    Gun* g;

}Controller;

Controller* init_Controller();

void ctrl_changeState(Controller* ctrl, STATE new_state);

void ctrl_setPlayer(Controller* ctrl,Player* set_p);
void ctrl_setGun(Controller* ctrl,Gun* set_g);

STATE ctrl_getState(Controller* ctrl);
void ctrl_readInput(Controller* ctrl,char c);
void ctrl_Destroy(Controller* ctrl);

#endif
