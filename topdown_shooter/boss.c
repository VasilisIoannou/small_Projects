#include <stdlib.h>

#include "boss.h"
#include "bullet.h"
#include "gun.h"
#include "screen.h"
#include "log.h"

Boss* init_Boss(Gun* set_g){
    Boss* new_b = malloc(sizeof(Boss));

    if(new_b == NULL){
        return NULL;
    }

    new_b->hp = 100;
    new_b->stage = FIRST;
    new_b->size = 5;
    new_b->x = 20;
    new_b->x = 5;
    new_b->self_char = '+';
    new_b->dead = 0;
    new_b->self_colour = RED;
    new_b->g = set_g;

    return new_b;
}

void bullet_collision_boss(Boss* b) {
    BulletNode* current = b->g->bullets;

    while (current != NULL) {
        Bullet* bul = current->bullet;

        if ((bul->x >= b->x && bul->x < (b->x + b->size) && bul->y >= b->y && bul->y < b->y+b->size)) {
            b->hp--;
            //mak
            char bossHp_log[100];
            sprintf(bossHp_log, "Boss Hp = %d", b->hp);
            write_log(bossHp_log);
            bul->remove = 1;
            return; 
        }
        current = current->next;
    }
}

void next_move(Boss* b){
    return;
}

void Boss_draw(Boss* b){
    for(int i=0;i<b->size;i++){
        for(int j=0;j<b->size;j++){
            drawCharAt(b->x+i,b->y+j,b->self_char,b->self_colour);
        }
    }
}

void Boss_destroy(Boss* b){
    if(b != NULL){
        free(b);
    }
}

void Boss_update(Boss* b){
    bullet_collision_boss(b);

    next_move(b);

    if(b->hp <= 0){
        b->dead = 1;
        Boss_destroy(b);
    }
}

