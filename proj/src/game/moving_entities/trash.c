#include "trash.h"
#include "../../graphics/spriteCache.h"

Trash* create_trash(uint8_t money, int x, int y, int xspeed, int yspeed){
  Trash *trash = (Trash*) malloc (sizeof(Trash));
  trash->trash = create_animated_entity(x, y, xspeed, yspeed, 6, BUBBLE_TRASH, TRASH_POP1, TRASH_POP2, TRASH_POP3, TRASH_POP4, NO_BUBBLE_TRASH);
  trash->money = money;
  trash->state = IN_BUBBLE;
  return trash;
}


void trash_pop_bubble(Trash* trash) {
  trash->state = POPPING_BUBBLE;
  trash->trash->e->xspeed = 0;
  trash->trash->e->yspeed = 2;
}

void destroy_trash(Trash* trash) {
  if(trash == NULL) return;
  destroy_animated_entity(trash->trash);
  trash->trash = NULL;
  free(trash);
}

void draw_trash(Trash* trash, Entity* background, float backgroundRot, float scale, bool on_black) {
  if (trash->state == POPPING_BUBBLE) {
      animate_entity(trash->trash);
      if (trash->trash->cur_fig == trash->trash->num_fig-1) trash->state = OUT_BUBBLE;
  }
  if (on_black) draw_sprite_on_black_background(trash->trash->e->sprite, trash->trash->e->x, trash->trash->e->y, scale, 0, background->x, background->y, background->sprite->width, backgroundRot, false);
  else draw_sprite_on_background(trash->trash->e->sprite, trash->trash->e->x, trash->trash->e->y, scale, 0, background->x, background->y, background->sprite->width, false);
}
