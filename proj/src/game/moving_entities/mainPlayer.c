

#include "mainPlayer.h"
#include <lcom/lcf.h>
#include <math.h>


MainPlayer *create_main_player(int x, int y, bool player_num) {
    MainPlayer *sp = (MainPlayer *) malloc ( sizeof(MainPlayer));
    if (!player_num) {
      sp->asp = create_animated_entity(x, y, 0, 0, 15, PLAYER_PINK_IDLE_1, PLAYER_PINK_IDLE_2, PLAYER_PINK_IDLE_3, PLAYER_PINK_IDLE_4, 
      PLAYER_PINK_WALKING_1, PLAYER_PINK_WALKING_2, PLAYER_PINK_WALKING_3, PLAYER_PINK_WALKING_4, PLAYER_PINK_WALKING_5, PLAYER_PINK_WALKING_6, 
      PLAYER_PINK_CLIMB_1, 
      PLAYER_PINK_CLIMB_1, PLAYER_PINK_CLIMB_2, PLAYER_PINK_CLIMB_3, PLAYER_PINK_CLIMB_4);
    } else {
      sp->asp = create_animated_entity(x, y, 0, 0, 15, PLAYER_BLUE_IDLE_1, PLAYER_BLUE_IDLE_2, PLAYER_BLUE_IDLE_3, PLAYER_BLUE_IDLE_4, 
      PLAYER_BLUE_WALKING_1, PLAYER_BLUE_WALKING_2, PLAYER_BLUE_WALKING_3, PLAYER_BLUE_WALKING_4, PLAYER_BLUE_WALKING_5, PLAYER_BLUE_WALKING_6, 
      PLAYER_BLUE_CLIMB_1, 
      PLAYER_BLUE_CLIMB_1, PLAYER_BLUE_CLIMB_2, PLAYER_BLUE_CLIMB_3, PLAYER_BLUE_CLIMB_4);
    }
    sp->move_state = STOPPED_HORIZONTAL;
    sp->state = WALK;
    sp->no_pics_per_state[0] = 4;
    sp->no_pics_per_state[1] = 6;
    sp->no_pics_per_state[2] = 1;
    sp->no_pics_per_state[3] = 4;
    sp->start_pics_per_state[0] = 0;
    sp->start_pics_per_state[1] = 4;
    sp->start_pics_per_state[2] = 10;
    sp->start_pics_per_state[3] = 11;
    transition_to_state(sp, STOPPED_HORIZONTAL);
    return sp;
}

void destroy_main_player(MainPlayer *sp) {
    if( sp == NULL )
        return;
    destroy_animated_entity(sp->asp);
    free(sp->asp);
    sp->asp = NULL;
    free(sp);
    sp = NULL;
}

void transition_to_state(MainPlayer* p, MOVEMENT_STATE state){
  p->move_state = state;

  AnimatedEntity* asp = p->asp;

  int start = p->start_pics_per_state[state];
  asp->start_fig = start;
  asp->num_fig = p->no_pics_per_state[state];
  asp->cur_fig = start;
  asp->e->sprite = asp->sprites[start];
  asp->frame_number = 0;
}

void move_main_player(MainPlayer *mp, MOVEMENT movement, uint8_t player_speed){
  switch(mp->move_state){
    case STOPPED_HORIZONTAL:
    case STOPPED_VERTICAL:
      if(movement == LEFT || movement == RIGHT){
        transition_to_state(mp, WALKING);
      }
      else if (movement == UP || movement == DOWN){
        transition_to_state(mp, CLIMBING);
      }
      break;
    case WALKING:
      if( movement == STOP ){
        transition_to_state(mp, STOPPED_HORIZONTAL);
        break;
      }
      else if (movement == UP || movement == DOWN) {
        transition_to_state(mp, CLIMBING);
      }
      break;
    case CLIMBING:
      if(movement == STOP){
        transition_to_state(mp, STOPPED_VERTICAL);
        break;
      }
      else if (movement == LEFT || movement == RIGHT) {
        transition_to_state(mp, WALKING);
      }
      break;
  }
  if(mp->move_state == WALKING || mp->move_state == CLIMBING) {
    mp->asp->e->xspeed = player_speed*((movement == RIGHT) - (movement == LEFT));
    mp->asp->e->yspeed = player_speed*((movement == DOWN) - (movement == UP));
    move_entity(mp->asp->e);
  }
}

