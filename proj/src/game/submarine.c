#include <lcom/lcf.h>
#include "submarine.h"
#include "../graphics/spriteCache.h"
#include "../lib/devices/video_card/video.h"
#include <math.h>

Submarine * create_sub(int x, int y) {
    Submarine* sub = (Submarine *) malloc ( sizeof(Submarine));
    sub->submarine = create_entity(get_sprite(SUBMARINE), x, y, 0, 0);
    sub->submask=get_sprite(SUBMASK);
    sub->hook = create_hook(get_sprite(HOOK));
    sub->rotation = 0;
    sub->players[0] = sub->players[1] = NULL;
    sub->cannon[0] = create_default_cannon(240);
    sub->cannon[1] = create_default_cannon(180);
    sub->cannon[2] = create_default_cannon(110);
    sub->cannon[3] = create_default_cannon(20);
    sub->cannon[4] = create_default_cannon(340);
    sub->cannon[0]->cannon->x = -100;
    sub->cannon[0]->cannon->y = 320;
    sub->cannon[1]->cannon->x = 270;
    sub->cannon[1]->cannon->y = 440;
    sub->cannon[2]->cannon->x = 860;
    sub->cannon[2]->cannon->y = 320;
    sub->cannon[3]->cannon->x = 540;
    sub->cannon[3]->cannon->y = 110;
    sub->cannon[4]->cannon->x = 80;
    sub->cannon[4]->cannon->y = 120;

    sub->cannon[0]->firingX = sub->submarine->x - sub->cannon[0]->cannon->x;
    sub->cannon[0]->firingY = sub->submarine->y - sub->cannon[0]->cannon->y;
    
    sub->cannon[1]->firingX = sub->submarine->x - sub->cannon[1]->cannon->x;
    sub->cannon[1]->firingY = sub->submarine->y - sub->cannon[1]->cannon->y;
    
    sub->cannon[2]->firingX = sub->submarine->x - sub->cannon[2]->cannon->x;
    sub->cannon[2]->firingY = sub->submarine->y - sub->cannon[2]->cannon->y;
    
    sub->cannon[3]->firingX = sub->submarine->x - sub->cannon[3]->cannon->x;
    sub->cannon[3]->firingY = sub->submarine->y - sub->cannon[3]->cannon->y;
    
    sub->cannon[4]->firingX = sub->submarine->x - sub->cannon[4]->cannon->x;
    sub->cannon[4]->firingY = sub->submarine->y - sub->cannon[4]->cannon->y;
    
    sub->state = SUBMERGED;
    sub->oxygen_counter = 100;
    sub->oxygen_capacity = 100;
    sub->max_number_of_lifes = 3;
    sub->current_number_of_lifes = 3;
    sub->fire_cooldown = 50;
    sub->player_speed = 5;
    return sub;
}

void destroy_submarine(Submarine* submarine){
    if(submarine == NULL) return;
    destroy_entity(submarine->submarine);
    submarine->submask = NULL;
    destroy_hook(submarine->hook);
    submarine->hook = NULL;
    destroy_main_player(submarine->players[0]);
    destroy_main_player(submarine->players[1]);
    submarine->players[0] = NULL;
    submarine->players[1] = NULL;
    for(int i = 0; i < SUB_CANNON_NUM; i++) {
        destroy_cannon(submarine->cannon[i]);
        submarine->cannon[i] = NULL;
    }
    free(submarine);
    submarine = NULL;
}

bool add_player_to_submarine(MainPlayer* player, bool MainPlayer, Submarine* sub) {
    player->asp->e->x = (sub->submarine->x - player->asp->e->x);
    player->asp->e->y = (sub->submarine->y - player->asp->e->y);
    sub->players[!MainPlayer] = player;
    return 0;
}

bool check_collision(Submarine* sub, int x, int y, MOVEMENT movement) {
    //only when scale = 1
    x = (x - sub->submarine->x) + sub->submarine->sprite->width/2;
    y = (y - sub->submarine->y) + sub->submarine->sprite->height/2;
    if (movement == UP || movement == DOWN) {
        uint32_t up_middle = get_color_of(sub->submask, x, y - sub->players[0]->asp->e->sprite->height/2);
        uint32_t down_middle = get_color_of(sub->submask, x, y + sub->players[0]->asp->e->sprite->height/2);
        if (up_middle != STAIRS || down_middle != STAIRS) return true;
    }
    else {
        uint32_t bottom_left = get_color_of(sub->submask, x - MAX_WIDTH/2, y + sub->players[0]->asp->e->sprite->height/2);
        uint32_t bottom_right = get_color_of(sub->submask, x + MAX_WIDTH/2, y + sub->players[0]->asp->e->sprite->height/2);
        if ((bottom_left != FLOOR && bottom_left != STAIRS) ||bottom_left == WALLS) return true;
        else if ((bottom_right != FLOOR && bottom_right != STAIRS) || bottom_right == WALLS) return true;
    }
    return false;
}

bool check_drive(Submarine* submarine) {
    int x = (submarine->players[0]->asp->e->x - submarine->submarine->x) + submarine->submarine->sprite->width/2;
    int y = (submarine->players[0]->asp->e->y - submarine->submarine->y) + submarine->submarine->sprite->height/2;
    uint32_t middle = get_color_of(submarine->submask, x, y);
    return middle == WHEEL;
}

bool activate_cannon(Submarine* submarine) {
    int x = (submarine->players[0]->asp->e->x - submarine->submarine->x) + submarine->submarine->sprite->width/2;
    int y = (submarine->players[0]->asp->e->y - submarine->submarine->y) + submarine->submarine->sprite->height/2;
    bool multiplayer = submarine->players[1] != NULL;
    uint32_t middle = get_color_of(submarine->submask, x, y);
    if (middle == CANNON1 && (!multiplayer || submarine->players[1]->state != INCANNON1)) submarine->players[0]->state = INCANNON1;
    else if (middle == CANNON2 && (!multiplayer || submarine->players[1]->state != INCANNON2)) submarine->players[0]->state = INCANNON2;
    else if (middle == CANNON3 && (!multiplayer || submarine->players[1]->state != INCANNON3)) submarine->players[0]->state = INCANNON3;
    else if (middle == CANNON4 && (!multiplayer ||submarine->players[1]->state != INCANNON4)) submarine->players[0]->state = INCANNON4;
    else if (middle == CANNON5 && (!multiplayer ||submarine->players[1]->state != INCANNON5)) submarine->players[0]->state = INCANNON5;
    else return false;
    return true;
}

void deactivate_cannon(Submarine* submarine) {
    //only when scale = 0.5
    submarine->cannon[submarine->players[0]->state-INCANNON1]->pointX = (submarine->cannon[submarine->players[0]->state-INCANNON1]->pointX - 400)*(2) + 400;
    submarine->cannon[submarine->players[0]->state - INCANNON1]->pointY = (submarine->cannon[submarine->players[0]->state - INCANNON1]->pointY - 300)*(2) + 300;
}

int draw_submarine_and_childs(Submarine* sub, Entity* background, Entity* cursor, float scale) {
    float rot = 0;
    if (scale == 0.5) {
        draw_sprite_bottom(sub->hook->hook, sub->submarine->x, sub->submarine->y, scale, 0, sub->submarine->x - 400, sub->submarine->y -470, sub->hook->height + 74, false);
        if (draw_sprite_t(sub->submarine->sprite, sub->submarine->x, sub->submarine->y, scale, sub->rotation, 0, 0, false) != 0) return 1;
        rot = sub->rotation;
    }
    if (draw_sprite_t(sub->players[0]->asp->e->sprite, sub->submarine->x, sub->submarine->y, scale, rot, sub->submarine->x - sub->players[0]->asp->e->x, sub->submarine->y - sub->players[0]->asp->e->y, sub->players[0]->asp->e->facing_left) != 0) return 1;

    if (sub->players[1] != NULL) {
        if (draw_sprite_t(sub->players[1]->asp->e->sprite, sub->submarine->x, sub->submarine->y, scale, rot, sub->submarine->x - sub->players[1]->asp->e->x, sub->submarine->y - sub->players[1]->asp->e->y, sub->players[1]->asp->e->facing_left) != 0) return 1;
    }
    if (sub->players[0]->state >= INCANNON1) {
        draw_sprite(cursor->sprite, cursor->x - cursor->sprite->width/2, cursor->y - cursor->sprite->height/2);
        sub->cannon[sub->players[0]->state - INCANNON1]->pointX = cursor->x;
        sub->cannon[sub->players[0]->state - INCANNON1]->pointY = cursor->y;
    }
    for (uint8_t j = 0; j < 5; j++) {
        if (draw_sprite_cannon_t(sub->cannon[j]->cannon->sprite, sub->submarine->x, sub->submarine->y, scale, rot, sub->submarine->x - sub->cannon[j]->cannon->x, sub->submarine->y - sub->cannon[j]->cannon->y, sub->cannon[j]->pointX - cursor->sprite->width/2, sub->cannon[j]->pointY - cursor->sprite->height/2, sub->cannon[j]->facing) != 0) return 1;
        draw_cannon(sub->cannon[j], background, scale);
    }
    return 0;
}


int draw_oxygen_bar(Submarine* sub){
    vg_draw_vline(29, 40, 30, 0x28282B);
    vg_draw_vline(230, 40, 30, 0x28282B);
    vg_draw_hline(29, 39, (1000 / 5) + 2, 0x28282B);
    vg_draw_hline(29, 70, (1000 / 5) + 2, 0x28282B);
    uint8_t line = 0;
    int oxygen = (1000*sub->oxygen_counter)/sub->oxygen_capacity;
    if (oxygen < 260) line = (oxygen - 250 > 0) ? 10 - (oxygen - 250) + 9 : 19;
    else if (oxygen < 510) line = (oxygen - 500 > 0) ? 10 - (oxygen - 500) : 10;
    draw_splitted_sprites_line(get_sprite(OXIGEN), get_sprite(OXIGEN_BG), 30, 40, oxygen / 5, 30, line);
    return 0;
}

int draw_hearts(Submarine* sub){
    Sprite* full_heart = get_sprite(FULL_HEART);

    int num_full_hearts = sub->current_number_of_lifes;

    if(num_full_hearts == 3){
        return draw_sprite(full_heart, 660, 40) != 0 || draw_sprite(full_heart, 700, 40) != 0 || draw_sprite(full_heart, 740, 40) != 0;
    }

    Sprite* empty_heart = get_sprite(EMPTY_HEART);

    if(num_full_hearts == 2){
        return draw_sprite(empty_heart, 660, 40) != 0 || draw_sprite(full_heart, 700, 40) != 0 || draw_sprite(full_heart, 740, 40) != 0;
    }
    if(num_full_hearts == 1){
        return draw_sprite(empty_heart, 660, 40) != 0 || draw_sprite(empty_heart, 700, 40) != 0 || draw_sprite(full_heart, 740, 40) != 0;
    }

    return 1;
}

void (update_oxygen)(Submarine* sub) {
    if (sub->state == SUBMERGED) sub->oxygen_counter--;
    else if (sub->oxygen_counter < sub->oxygen_capacity) sub->oxygen_counter += MIN(2, sub->oxygen_capacity - sub->oxygen_counter);
}

