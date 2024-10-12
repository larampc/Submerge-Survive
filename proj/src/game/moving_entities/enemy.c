#include "enemy.h"
#include <stdlib.h>

Enemy* create_enemy(bool active, uint16_t seed){
    Enemy *enemy = (Enemy*) malloc (sizeof(Enemy));
    srandom(seed);
    enemy->asp = create_animated_entity(-35, random()%50, 0,0, 1, PLANE);
    enemy->active = active;
    enemy->seed = seed;
    return enemy;
}

void destroy_enemy(Enemy* enemy){
    if(enemy == NULL) return;
    destroy_animated_entity(enemy->asp);
    enemy->asp = NULL;
    free(enemy);
    enemy = NULL;
}

void die(Enemy* enemy){
    enemy->asp->e->x = -35;
    enemy->active = false;
}

