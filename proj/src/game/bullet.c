#include "bullet.h"
#include<math.h>

BulletInfo* get_bullet_info(BULLET_TYPES type){
  BulletInfo* bullet_info = (BulletInfo*) malloc(sizeof(BulletInfo));
  switch (type)
  {
  case NORMAL:
    bullet_info->bulletExplosion = create_animated_entity(0, 0, 0, 0, 5, EXPLOSION1, EXPLOSION2, EXPLOSION3, EXPLOSION4, EXPLOSION5);
    bullet_info->bulletSprite = get_sprite(MINE);
    break;
  default:
    break;
  }
  return bullet_info;
}

void move_bullet(Bullet* bullet){
    bullet->x += bullet->xspeed;
    bullet->y += bullet->yspeed;
}

bool check_bullet_collision(BulletInfo* bulletInfo, Bullet* bullet, Entity* e, int* collision_x, int* collision_y){
  if(e == NULL || bullet == NULL) return false;
  float leftBulletEdge = bullet->x - bulletInfo->bulletSprite->width/2;
  float rigthBulletEdge = bullet->x + bulletInfo->bulletSprite->width/2;
  float topBulletEdge = bullet->y - bulletInfo->bulletSprite->height/2;
  float bottomBulletEdge = bullet->y + bulletInfo->bulletSprite->height/2;
  float leftEntityEdge = e->x - e->sprite->width/4;
  float rigthEntityEdge = e->x + e->sprite->width/4;
  float topEntityEdge = e->y - e->sprite->height/4;
  float bottomEntityEdge = e->y + e->sprite->height/4;

  if (rigthBulletEdge < leftEntityEdge || leftBulletEdge > rigthEntityEdge || bottomBulletEdge < topEntityEdge || topBulletEdge > bottomEntityEdge) {
        return false;
  }

  float overlapLeft = fmax(leftBulletEdge, leftEntityEdge);
  float overlapRight = fmin(rigthBulletEdge, rigthEntityEdge);
  float overlapTop = fmax(topBulletEdge, topEntityEdge);
  float overlapBottom = fmin(bottomBulletEdge, bottomEntityEdge);
  for (float x = overlapLeft; x < overlapRight; x++) {
    for (float y = overlapTop; y < overlapBottom; y++) {
        int bulletX = (int)(x - leftBulletEdge);
        int bulletY = (int)(y - topBulletEdge);
        int entityX = (int)(x - leftEntityEdge);
        int entityY = (int)(y - topEntityEdge);

        if (get_color_of(bulletInfo->bulletSprite, bulletX, bulletY) != 0
          && get_color_of(e->sprite, entityX*2, entityY*2) != 0) {
            *collision_x = (int) x;
            *collision_y = (int) y;
            return true;
        }
    }
  }
  return false;
}

bool check_cannon_bullet_collision(BulletInfo* bulletInfo, Bullet* bullet, Entity* e, Entity* background){
  if(e == NULL || bullet == NULL) return false;
  Sprite* bulletSprite = bulletInfo->bulletSprite;
  int x_ = bullet->x;
  if (x_ - background->x + bulletInfo->bulletSprite->width < 0) x_ += background->sprite->width;
  else if (x_ - background->x - bulletInfo->bulletSprite->width > 800) x_ -= background->sprite->width;
  x_ -= background->x;
  int y_ = bullet->y - background->y;
  if (x_ - bulletSprite->width/2 >= e->x &&
        x_ + bulletSprite->width/2 <= e->x + e->sprite->width &&
        y_ + bulletSprite->height/2 >= e->y &&
        y_ - bulletSprite->height/2 <= e->y + e->sprite->height) {
    return true;
  }
  return false;
}

void expload_bullet(AnimatedEntity* explosions[], Bullet* bullet, BulletInfo* bulletInfo) {
  for(int i = 0; i < MAX_BULLET_NUM; ++i){
    if(explosions[i] != NULL) continue;
    explosions[i] = clone(bulletInfo->bulletExplosion);
    explosions[i]->e->x = (int)bullet->x;
    explosions[i]->e->y = (int)bullet->y;
    free(bullet);
    break;
  }
}

