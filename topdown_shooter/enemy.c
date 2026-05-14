#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "enemy.h"

#define COUNTER_LIMIT 10

Enemy* init_Enemy(int set_x,int set_y,Player* set_p,Gun* set_g){
    Enemy* new_e = malloc(sizeof(Enemy));
    if (new_e == NULL){
        return NULL;
    }

    new_e->x = set_x;
    new_e->y = set_y;
    new_e->v = 0;
    new_e->dead = 0;
    new_e->counter = 0;
    new_e->self_char = 'x';
    new_e->self_colour = RED;
    new_e->p = set_p;
    new_e->g = set_g;

    return new_e;
}

void Enemy_destroy(Enemy* e){
    if(e != NULL){
        free(e);
    }
}

void bullet_collision(Enemy* e) {
    BulletNode* current = e->g->bullets;

    while (current != NULL) {
        Bullet* b = current->bullet;

        if (b->x == e->x && b->y == e->y) {
            e->dead = 1; 
            b->remove = 1;
            return; 
        }
        current = current->next;
    }
}

void move_to_player_random(Enemy* e) {
    e->counter = (e->counter + 1) % COUNTER_LIMIT;

    if(e->counter > 1){
        return;
    }

    int random_num = (rand() % 10) + 1;

    int diff_x = e->x - e->p->x;
    int diff_y = e->y - e->p->y;

    int dir_x = (diff_x > 0) ? -1 : (diff_x < 0) ? 1 : 0;
    int dir_y = (diff_y > 0) ? -1 : (diff_y < 0) ? 1 : 0;

    if (random_num < 10) { 
        if (abs(diff_x) > abs(diff_y)) {
            e->x += dir_x;
        } else {
            e->y += dir_y;
        }
    }
}

void Enemy_move(Enemy* e){
    move_to_player_random(e);
    return;
}

void Enemy_update(Enemy* e){
    bullet_collision(e);
    Enemy_move(e);
}

void Enemy_draw(Enemy* e){
    drawCharAt(e->x,e->y,e->self_char,e->self_colour);
}



