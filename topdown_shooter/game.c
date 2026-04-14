#define _DEFAULT_SOURCE

#include <unistd.h>
#include <stdio.h>

#include "screen.h"
#include "player.h"
#include "gun.h"
#include "controller.h"
#include "log.h"

#define FPS 30
#define FRAME_DELAY (1000000 / FPS)


void drawNextFrame(Player* p, Gun* g){
    //update screen
    clearScreen();
    drawBorders();
    Player_draw(p);
    Gun_draw(g);
    Gun_drawBullets(g);

    return;
}

int main(){

    create_log();

    Controller* ctrl = init_Controller();
    Player* p = init_Player();
    Gun* g = init_Gun(p);

    ctrl_setPlayer(ctrl,p);
    ctrl_setGun(ctrl, g);

    //setEcho(0);
    enableRawMode();
    clearScreen();
    hideCursor();

    while(ctrl_getState(ctrl) != STATE_END){
       
        //input
        if(kbhit()){
            char key = getchar();
            ctrl_readInput(ctrl,key);
        }

        //Update One Cycle after Input
        Gun_move(g);
        Gun_updateBullets(g);

        drawNextFrame(p,g);
        usleep(FRAME_DELAY);
    }

    //Delete structs
    close_log();

    Player_destroy(p);
    Gun_destroy(g);

    //Exit
    //setEcho(1);
    disableRawMode();
    showCursor();    
    clearScreen();


}
