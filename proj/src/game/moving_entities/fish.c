#include "fish.h"
#include <stdlib.h>

Fish* create_fish(uint8_t money, int x, int y, int xspeed, int yspeed, uint16_t seed){
    Fish *fish = (Fish*) malloc (sizeof(Fish));
    fish->seed = seed;
    fish->asp = create_animated_entity(x, y, xspeed, yspeed, 6, FISH_1, FISH_2, FISH_3, FISH_4, FISH_5, FISH_6);
    fish->facing_left = false;
    fish->money = money;

    return fish;
}

void change_fish_direction(Fish* fish){
  srandom(fish->seed++);
  uint8_t tmp = (random() % 10);
  Entity* e = fish->asp->e;
  //randomly change movement mode
  if(tmp == 1 ){
    e->yspeed = (random() % 5);
    e->xspeed = (random() % 5);
  }

  tmp = random() % 10;
  if(tmp == 0) {
    e->xspeed *= -1;
  }
  if(tmp == 1) {
    e->yspeed *= -1;
  }
}

void destroy_fish(Fish* fish){
  if(fish == NULL) return;
   destroy_animated_entity(fish->asp);
   free(fish);
}


