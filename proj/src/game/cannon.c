#include "cannon.h"

#include <math.h>
#include "game.h"
#include "../lib/devices/RTC/rtc.h"

void normalize(float* x, float* y) {
    float length = sqrt((*x)*(*x) + (*y)*(*y));
    *x = *x / length;
    *y = *y / length;
}

Cannon* create_cannon(uint16_t facing, BULLET_TYPES bullet_type){
    Cannon *cannon = (Cannon*) malloc(sizeof(Cannon));
    cannon->cannon = create_entity(get_sprite(CANNON), 100, 100, 0, 0);
    cannon->facing = facing;
    cannon->pointX = 0;
    cannon->pointY = 0;
    cannon->firingX = 0;
    cannon->firingY = 0;
    cannon->bulletInfo = get_bullet_info(bullet_type);
    for(int i = 0; i < MAX_BULLET_NUM; ++i){
      cannon->activeBullets[i] = NULL;
      cannon->explosions[i] = NULL;
    }
    cannon->lastFired = 0;
    return cannon;
}

//Facing is between 0 and 360 where 0 is up and 90 is right
Cannon* create_default_cannon(uint16_t facing) {
  return create_cannon(facing, NORMAL);
}

int fire(Cannon* cannon, Entity* background, float scale, uint32_t fire_cooldown, int currentTime) {
  if (currentTime - cannon->lastFired < fire_cooldown) {
      return 1;
  }
  cannon->lastFired = currentTime;

  Bullet *bullet = (Bullet*) malloc(sizeof(Bullet));
  bullet->x = ((((cannon->cannon->x + cannon->cannon->sprite->width/2) - 400) / 2) + 400 + background->x);
  bullet->y = ((((cannon->cannon->y + cannon->cannon->sprite->height/2) - 300) / 2) + 300 + background->y);
  
  float dx = (cannon->pointX - (bullet->x - background->x));
  float dy = (cannon->pointY - (bullet->y - background->y));

  int rot = (atan2(dy, dx)) * (180.0 / M_PI) + 90;
  if (rot < 0) rot += 360;
  rot -= cannon->facing;
  if (rot <= -180) rot += 360;
  if (rot > 180) rot -= 360;
  if (rot > 90) {
    rot = cannon->facing;
    dx = cos(rot * 0.01745329251);
    dy = sin(rot * 0.01745329251);
  } else if (rot < -90) {
    rot = cannon->facing - 180;
    dx = cos(rot * 0.01745329251);
    dy = sin(rot * 0.01745329251);
  } else normalize(&dx, &dy);
  dx *= 20;
  dy *= 20;
  bullet->xspeed = dx;
  bullet->yspeed = dy;

  for(int i = 0; i < MAX_BULLET_NUM; ++i){
    if(cannon->activeBullets[i] == NULL){
      cannon->activeBullets[i] = bullet;
      break;
    }
  }
  return 0;
}

void draw_cannon(Cannon* cannon, Entity* background, float scale) {
  for(int i = 0; i < MAX_BULLET_NUM; ++i) {
    Bullet* bullet = cannon->activeBullets[i];
    if(bullet != NULL) {
      draw_sprite_on_background(cannon->bulletInfo->bulletSprite, (int)bullet->x, (int)bullet->y, scale, 1, background->x, background->y, background->sprite->width, 0);
      //draw_sprite_t(cannon->bulletInfo->bulletSprite, (int)bullet->x, (int)bullet->y,1,1,0,0,0);
    }
  }
  for(int i = 0; i < MAX_BULLET_NUM; ++i) {
    if(cannon->explosions[i] == NULL) continue;

    draw_entity_on_background(cannon->explosions[i]->e, background, scale);
    animate_entity(cannon->explosions[i]);
    if(cannon->explosions[i]->cur_fig == cannon->explosions[i]->start_fig && cannon->explosions[i]->frame_number == 0) {
      //animation ended
      destroy_animated_entity(cannon->explosions[i]);
      cannon->explosions[i] = NULL;
    }
  }
}

void destroy_cannon(Cannon *sp) {
    if( sp == NULL ) return;
    destroy_animated_entity(sp->bulletInfo->bulletExplosion);
    sp->bulletInfo->bulletExplosion = NULL;
    sp->bulletInfo->bulletSprite = NULL;
    free(sp->bulletInfo);
    sp->bulletInfo = NULL;
    for(int i = 0; i < MAX_BULLET_NUM; ++i){
      free(sp->activeBullets[i]);
      sp->activeBullets[i] = NULL;
    }
    destroy_entity(sp->cannon);
    sp->cannon = NULL;
    free(sp);
    sp = NULL;
}
