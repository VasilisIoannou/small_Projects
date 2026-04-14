#include "controller.h"

Controller* init_Controller(){
    Controller* new_ctrl = malloc(sizeof(Controller));
    if (new_ctrl == NULL) {
        return NULL; // Or handle the error appropriately
    }

    new_ctrl->current_state = STATE_PLAY;
    return new_ctrl;
}  

void ctrl_Destroy(Controller* ctrl) {
    if (ctrl != NULL) {
        free(ctrl);
    }
}

void ctrl_changeState(Controller* ctrl, STATE new_state){
    ctrl->current_state = new_state;
}

STATE ctrl_getState(Controller* ctrl){
    return ctrl->current_state;
}

void ctrl_setPlayer(Controller* ctrl,Player* set_p){
    ctrl->p = set_p;
}

void ctrl_setGun(Controller* ctrl,Gun* set_g){
    ctrl->g = set_g;
}

void ctrl_readInput(Controller* ctrl,char key){
    if(key == 'q'){
        ctrl_changeState(ctrl,STATE_END);
    }

    if(ctrl_getState(ctrl) == STATE_PLAY){
        Player_readInput(ctrl->p,key);
        Gun_readInput(ctrl->g,key);
    }
}
