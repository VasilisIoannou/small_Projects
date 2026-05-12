#ifndef _ENEMYLIST_H__
#define _ENEMYLIST_H__

#include "enemy.h"

typedef struct enemyList_struct{
    Enemy* e;
    struct enemyList_struct* next;
}EnemyNode;

void enemyList_addEnemy(EnemyNode** head, Enemy* new_e);
void enemyList_cleanup(EnemyNode** head_ref) ;
void enemyList_destroy(EnemyNode* head) ;
void enemyList_draw(EnemyNode* head);

#endif
