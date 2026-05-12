#include <stddef.h>  
#include <stdlib.h>

#include "enemy.h"
#include "enemyList.h"

void enemyList_addEnemy(EnemyNode** head, Enemy* new_e){
    EnemyNode* new_node = malloc(sizeof(Enemy));
    new_node->e = new_e;
    new_node->next = *head;

    *head = new_node;
}

void enemyList_cleanup(EnemyNode** head_ref) {
    EnemyNode* curr = *head_ref;
    EnemyNode* prev = NULL;

    while (curr != NULL) {
        if (curr->e->dead == 1) {
            EnemyNode* nodeToDelete = curr;

            if (prev == NULL) {
                *head_ref = curr->next;
                curr = *head_ref; 
            } else {
                prev->next = curr->next;
                curr = prev->next;
            }
            Enemy_destroy(nodeToDelete->e);
            free(nodeToDelete);
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

void enemyList_destroy(EnemyNode* head) {
    EnemyNode* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        // Option: free(temp->enemyData); // Only if you malloc'd the Enemy too!
        free(temp);
    }
}

void enemyList_draw(EnemyNode* head) {
    EnemyNode* curr = head; 
    while (curr != NULL) {
        Enemy_draw(curr->e);
        curr = curr->next;
    }
}
