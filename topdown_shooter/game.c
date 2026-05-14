#define _DEFAULT_SOURCE

#include <unistd.h>
#include <stdio.h>

#include "screen.h"
#include "player.h"
#include "gun.h"
#include "controller.h"
#include "log.h"
#include "enemy.h"
#include "enemyList.h"
#include "boss.h"

#define FPS 30
#define FRAME_DELAY (1000000 / FPS)


void drawNextFrame(Player* p, Gun* g,EnemyNode* enemyList,Boss* boss){
    //update screen
    clearScreen();
    drawBorders();
    Player_draw(p);
    Gun_draw(g);
    Gun_drawBullets(g);

    enemyList_draw(enemyList);

    if(boss != NULL){
        Boss_draw(boss);
    }

    return;
}

int main(){

    create_log();

    Controller* ctrl = init_Controller();
    Player* p = init_Player();
    Gun* g = init_Gun(p);
    
    EnemyNode* enemyList = NULL;


    ctrl_setPlayer(ctrl,p);
    ctrl_setGun(ctrl, g);

    //temporary create 1 enemy here
    Enemy* e = init_Enemy(10,10,p,g);
    Enemy* e2 = init_Enemy(20,20,p,g);
    enemyList_addEnemy(&enemyList,e);
    enemyList_addEnemy(&enemyList,e2);

    //temporary crete boss here
    //After I add spawn of enemies in rounds and points I will contnue with the boss
    Boss* boss = init_Boss(g);

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

        //Update Enemies
        enemyList_update(enemyList);
        enemyList_cleanup(&enemyList);

        //Update Boss
        if(boss != NULL){
            Boss_update(boss);
        }

        drawNextFrame(p,g,enemyList,boss);
        usleep(FRAME_DELAY);
    }

    //Delete structs
    close_log();

    Player_destroy(p);
    Gun_destroy(g);
    enemyList_destroy(enemyList);
    enemyList = NULL;

    //Exit
    //setEcho(1);
    disableRawMode();
    showCursor();    
    clearScreen();


}
