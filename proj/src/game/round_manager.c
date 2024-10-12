#include "round_manager.h"
#include "../lib/devices/sp/sp.h"

extern bool multiplayer;

RoundManager* create_round_manager(){
  RoundManager* r = (RoundManager*)malloc(sizeof(RoundManager));
  if(r == NULL) return NULL;
  r->appearence_rate = 100;
  r->active_enemies = 0;
  r->round_enemies = 5;
  r->under_attack = false;
  r->created_enemies = 0;
  r->bulletInfo = get_bullet_info(NORMAL);
  for(int i = 0; i < MAX_ENEMY_NUM; i++){
    r->enemies[i] = create_enemy(false, i);
  }
  for(int i = 0; i < MAX_BULLET_NUM; i++){
    r->enemy_bullets[i] = NULL;
    r->explosions[i] = NULL;
  }
  return r;
}

void destroy_round_manager(RoundManager* r){
  if(r == NULL) return;
  for(int i = 0; i < MAX_BULLET_NUM; i++){
    free(r->enemy_bullets[i]);
    r->enemy_bullets[i] = NULL;
    destroy_animated_entity(r->explosions[i]);
    r->explosions[i] = NULL;
  }
  for(int i = 0; i < MAX_ENEMY_NUM; i++){
    destroy_enemy(r->enemies[i]);
    r->enemies[i] = NULL;
  }
  destroy_animated_entity(r->bulletInfo->bulletExplosion);
  r->bulletInfo->bulletExplosion = NULL;
  r->bulletInfo->bulletSprite = NULL;
  free(r->bulletInfo);
  r->bulletInfo = NULL;
  free(r);
}

void finish_round(RoundManager* r) {
  for(int i = 0; i < MAX_BULLET_NUM; ++i){
    if(r->enemy_bullets[i] == NULL) continue;
    expload_bullet(r->explosions, r->enemy_bullets[i], r->bulletInfo);
    r->enemy_bullets[i] = NULL;
  }
  r->under_attack = false;
  if (r->round_enemies < MAX_ENEMY_NUM) r->round_enemies += 5;
  if (r->appearence_rate >= 2) r->appearence_rate -= 10;
  r->created_enemies = 0;
}

void update_round(RoundManager* r){
  if(r->under_attack && r->created_enemies < r->round_enemies){
    r->enemies[r->created_enemies]->active = true;
    r->created_enemies++;
    r->active_enemies++;
  }
}

void update_enemies(RoundManager* r) {
  for (int i = 0; i < MAX_ENEMY_NUM; i++) {
      if (r->enemies[i]->active) {
        srandom(r->enemies[i]->seed);
        fire_enemy(r, r->enemies[i]);
        r->enemies[i]->asp->e->xspeed = random()%10;
      }
  }
}

void fire_enemy(RoundManager* r, Enemy* enemy) {
  Bullet *bullet = (Bullet*) malloc(sizeof(Bullet));
  bullet->x = (enemy->asp->e->x + enemy->asp->e->sprite->width/2);
  bullet->y = (enemy->asp->e->y + enemy->asp->e->sprite->height/2);
  bullet->xspeed = 0;
  bullet->yspeed = 5;
  for(int i = 0; i < MAX_BULLET_NUM; ++i){
    if(r->enemy_bullets[i] == NULL){
      r->enemy_bullets[i] = bullet;
      break;
    }
  }
}

bool check_bullet_collisions_with_enemies(BulletInfo* bullet_info, Bullet* bullet, RoundManager* r, Entity* background, uint8_t cannon_index, uint8_t bullet_index){
  for(int j = 0; j < MAX_ENEMY_NUM; j++){
    if(!r->enemies[j]->active) continue;
    if(check_cannon_bullet_collision(bullet_info, bullet, r->enemies[j]->asp->e, background)) {
        if (multiplayer) {
            sp_write_wq(DESTROY_HEADER|KILL_ENEMY);
            sp_write_wq(j);
            sp_write_wq((cannon_index << 5) | bullet_index);
            sp_write_fifo();
        }
        die(r->enemies[j]);
        r->active_enemies--;
        if(r->active_enemies == 0 && r->created_enemies == r->round_enemies) finish_round(r);
        return true;
    }
  }
  for(int j = 0; j < MAX_BULLET_NUM; j++){
      if(r->enemy_bullets[j] == NULL) continue;
      int collisionX, collisionY;
      int x_ = bullet->x;
      if (x_ - background->x + bullet_info->bulletSprite->width < 0) x_ += background->sprite->width;
      else if (x_ - background->x - bullet_info->bulletSprite->width > 800) x_ -= background->sprite->width;
      x_ -= background->x;
      int y_ = bullet->y - background->y;
      if(check_sprite_collision(bullet_info->bulletSprite, 0.5, x_, y_, r->bulletInfo->bulletSprite, 1, r->enemy_bullets[j]->x, r->enemy_bullets[j]->y, &collisionX, &collisionY)){
          r->enemy_bullets[j]->x = collisionX;
          r->enemy_bullets[j]->y = collisionY;
          if (multiplayer) {
              sp_write_wq(DESTROY_HEADER|DESTROY_ENEMY_BULLET);
              sp_write_wq(j);
              sp_write_fifo();
          }
          expload_bullet(r->explosions, r->enemy_bullets[j], r->bulletInfo);
          r->enemy_bullets[j] = NULL;
          return false;
      }
  }
  return false;
}

void move_enemies(RoundManager* r, Submarine* submarine) {
    for (int i = 0; i < MAX_ENEMY_NUM; i++) {
      Enemy* enemy = r->enemies[i];
      if(!enemy->active) continue;

      Entity* e = enemy->asp->e;
      if (e->x + e->xspeed > 800) e->facing_left = true;
      else if (e->x - e->xspeed < 0) e->facing_left = false;

      if (e->facing_left) e->x -= e->xspeed;
      else e->x += e->xspeed;
    }
    move_bullets(r, submarine);
}

void draw_enemies(RoundManager* r) {
  draw_bullets(r);
  for (int i = 0; i < MAX_ENEMY_NUM; i++) {
        if (r->enemies[i]->active) {
            draw_entity_on_screen(r->enemies[i]->asp->e);
        }
    }
}

void reset(RoundManager* r){
  r->under_attack = false;
  r->round_enemies = 5;
  r->active_enemies = 0;
  r->created_enemies = 0;
  r->appearence_rate = 100;
}

void draw_bullets(RoundManager* r) {
  for(int i = 0; i < MAX_BULLET_NUM; ++i) {
    Bullet* bullet = r->enemy_bullets[i];
    if(bullet != NULL) {
      draw_sprite_t(r->bulletInfo->bulletSprite, (int)bullet->x, (int)bullet->y,1,0,0,0,0);
    }
  }
  for(int i = 0; i < MAX_BULLET_NUM; ++i) {
    if(r->explosions[i] == NULL) continue;
    draw_sprite_t(r->explosions[i]->e->sprite, r->explosions[i]->e->x, r->explosions[i]->e->y, 1, 0, 0, 0, 0);
    animate_entity(r->explosions[i]);
    if(r->explosions[i]->cur_fig == r->explosions[i]->start_fig && r->explosions[i]->frame_number == 0) {
      //animation ended
      destroy_animated_entity(r->explosions[i]);
      r->explosions[i] = NULL;
    }
  }
}

void move_bullets(RoundManager* r, Submarine* submarine) {
    for(int i = 0; i < MAX_BULLET_NUM; ++i) {
      Bullet* bullet = r->enemy_bullets[i];
      if(bullet == NULL) continue;
      move_bullet(bullet);
      if(bullet->x > 800 || bullet->y > 600 || bullet->x < 0 || bullet->y < 0 ){
          free(bullet);
          r->enemy_bullets[i] = NULL;
          continue;
      }
      int collision_x, collision_y;
      if(check_bullet_collision(r->bulletInfo, bullet, submarine->submarine, &collision_x, &collision_y)) {
          if (submarine->current_number_of_lifes > 0) submarine->current_number_of_lifes--;
          bullet->x = collision_x;
          bullet->y = collision_y;
          expload_bullet(r->explosions, bullet, r->bulletInfo);
          r->enemy_bullets[i] = NULL;
      }
  }
}

