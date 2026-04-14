#include <stdlib.h>
#include <stdio.h>

#include "gun.h"
#include "player.h"

Gun* init_Gun(Player* p){
    Gun* new_gun = malloc(sizeof(Gun));

    new_gun->bullets = NULL;

    if(!new_gun){
        fprintf(stderr, "Error: Gun pointer is NULL in Gun_readInput\n");
        return NULL;
    }

    new_gun -> my_p = p;
    new_gun-> x = p->x;
    new_gun-> y = p->y-1; // is always up for now
    new_gun-> pos = DIR_UP;

    new_gun -> self_char = '*';
    new_gun -> self_colour = YELLOW;

    return new_gun;
}

void Gun_draw(Gun* g){
    drawCharAt(g->x,g->y,g->self_char,g->self_colour);
}

void Gun_destroy(Gun* g) {
    if (g == NULL) return;

    struct BulletNode* curr = g->bullets;
    struct BulletNode* next_node = NULL;

    while (curr != NULL) {
        next_node = curr->next;
        Bullet_destroy(curr->bullet);
        free(curr);
        curr = next_node;
    }

    free(g);
}

void Gun_move(Gun* g){

    if (g == NULL || g->my_p == NULL) return;

    if(g->pos == DIR_UP){
        g-> x = g->my_p->x;    
        g-> y = g->my_p->y-1; 
        return;
    }

    if(g->pos == DIR_DOWN){
        g-> x = g->my_p->x;    
        g-> y = g->my_p->y+1;    
        return;
    }

    if(g->pos == DIR_LEFT){
        g-> x = g->my_p->x-1;    
        g-> y = g->my_p->y;    
        return;
    }

    if(g->pos == DIR_RIGHT){
        g-> x = g->my_p->x+1;    
        g-> y = g->my_p->y;    
        return;
    }
}

void AddBullet(Gun* g,Bullet* b){

    BulletNode* newNode = malloc(sizeof(BulletNode));

    if (newNode == NULL) {
        fprintf(stderr, "Failed to allocate memory for bullet node\n");
        return;
    }

    newNode->bullet = b;
    newNode->next = g->bullets;
    
    g->bullets = newNode;
}

void RemoveBullets(Gun* g){

    if (g == NULL || g->bullets == NULL) return;

    struct BulletNode* curr = g->bullets;
    struct BulletNode* prev = NULL;
    
    while (curr != NULL) {
        if (curr->bullet->remove == 1) {
            struct BulletNode* to_delete = curr;

            if (prev == NULL) {
                g->bullets = curr->next;
                curr = g->bullets; // Move curr to the new head
            } else {
                prev->next = curr->next;
                curr = curr->next; // Move curr forward
            }

            Bullet_destroy(to_delete->bullet);
            free(to_delete);
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

void Gun_updateBullets(Gun* g){
    if (g == NULL || g->bullets == NULL) return;
    
    //Move bullets;
    struct BulletNode* current_bullet = g->bullets;

    while(current_bullet != NULL){
       Bullet_move(current_bullet->bullet);
       current_bullet = current_bullet->next;
    }
    
    RemoveBullets(g);
}

void Gun_drawBullets(Gun* g){
    if (g == NULL || g->bullets == NULL) return;
    
    struct BulletNode* current_bullet = g->bullets;

    while(current_bullet != NULL){
       Bullet_draw(current_bullet->bullet);
       current_bullet = current_bullet->next;
    }
    
}

void Gun_shoot(Gun* g){
    //create bullet
    Bullet* new_bullet = init_Bullet(g);
    AddBullet(g,new_bullet);
}   

void Gun_readInput(Gun* g,char key){
    if(key == 'w'){
        g->pos = DIR_UP;
        return;
    }
    if(key == 's'){
        g->pos = DIR_DOWN;
        return;
    }
    if(key == 'd'){
        g->pos = DIR_RIGHT;
        return;
    }
    if(key == 'a'){
        g->pos = DIR_LEFT;
        return;
    }
    if(key == ' '){
        Gun_shoot(g);
    }
}
