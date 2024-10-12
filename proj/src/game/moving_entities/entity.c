#include "entity.h"
#include "math.h"
Entity* create_entity(Sprite* sp, int x, int y, int xspeed, int yspeed){
    Entity *e = (Entity*) malloc (sizeof(Entity));
    e->sprite = sp;
    e->x = x;
    e->y = y;
    e->xspeed = xspeed;
    e->yspeed = yspeed;
    e->facing_left = false;
    return e;
}

void destroy_entity(Entity *e) {
  if( e == NULL )
        return;
  free(e);
}

void move_entity(Entity* e) {
    e->facing_left = e->xspeed < 0;
    e->x += e->xspeed;
    e->y += e->yspeed;
}

int draw_entity_on_background(Entity* e, Entity* background, float scale){
    int rotation = (int)(e->yspeed != 0 || e->xspeed != 0) ? -(atan2(-e->yspeed, abs(e->xspeed))) * (180.0 / M_PI) : 0;
    if (rotation < 0) rotation += 360;
    if (e->facing_left) rotation *= -1;
    return draw_sprite_on_background(e->sprite, e->x, e->y, scale, rotation, background->x, background->y, background->sprite->width, e->facing_left);
}
int draw_entity_on_black_background(Entity* e, Entity* background, float backgroundRot, float scale){
    int rotation = (int)(e->yspeed != 0 || e->xspeed != 0) ? -(atan2(-e->yspeed, abs(e->xspeed))) * (180.0 / M_PI) : 0;
    if (rotation < 0) rotation += 360;
    if (e->facing_left) rotation *= -1;
    return draw_sprite_on_black_background(e->sprite, e->x, e->y, scale, rotation, background->x, background->y, background->sprite->width, backgroundRot, e->facing_left);
}
inline int draw_entity_on_screen(Entity* e){
   return e->facing_left ? draw_sprite_mirrored(e->sprite, e->x, e->y) : draw_sprite(e->sprite, e->x, e->y);
}
