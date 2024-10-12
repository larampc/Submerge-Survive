#include "game.h"
#include "../graphics/alphabet.h"
#include "../lib/devices/video_card/video.h"
#include "graphics/spriteCache.h"
#include "../lib/devices/sp/sp.h"

#define SURFACE_LEVEL_Y 320

extern bool multiplayer;
extern int rtc_counter;
uint8_t active_enemies = 0;


void buy_life(void* context){
    Game* game = (Game*)context;
    game->submarine->current_number_of_lifes++;
}

void buy_oxygen_capacity(void* context){
    Game* game = (Game*)context;
    game->submarine->oxygen_counter += MIN(0.25*(game->submarine->oxygen_capacity), 5000 - game->submarine->oxygen_capacity);
    game->submarine->oxygen_capacity += MIN(0.25*(game->submarine->oxygen_capacity), 5000 - game->submarine->oxygen_capacity);
}

void buy_lower_cannon_cooldown(void* context){
    Game* game = (Game*)context;
    game->submarine->fire_cooldown -= 5;
}

void buy_player_speed(void* context){
    Game* game = (Game*)context;
    game->submarine->player_speed += 1;
}

Game* create_game(bool multiplayer, bool player_num) {
    Game* game = (Game*) malloc(sizeof(Game));
    if (!game) return NULL;

    game->submarine = create_sub(400, 300);
    game->background = create_entity(get_sprite(BACKGROUND), 0, 300, 1, 1);
    game->scale = 1;
    game->seed = 0;
    MainPlayer* player0 = create_main_player((player_num) ? -10 : 0, -50, player_num);
    add_player_to_submarine(player0, true, game->submarine);
    if (multiplayer) {
        MainPlayer* player1 = create_main_player((player_num) ? 0 : -10, -50, !player_num);
        add_player_to_submarine(player1, false, game->submarine);
    }
    game->current_money = 30;

    game->fish_spawn_period = 10;
    game->trash_spawn_period = 50;
    for(int i = 0; i < MAX_FISH_NUM; i++){
        game->fishes[i] = NULL;
        game->next_spawn_fishes[i] = -1;
    }
    spawn_fishes(game, 15, 0);
    for(int i = 0; i < MAX_TRASH_NUM; i++){
        game->trash[i] = NULL;
        game->next_spawn_trashes[i] = -1;
    }
    game->round_manager = create_round_manager();
    game->shop = create_shop();
    add_item(game->shop, 518, 188, 5, get_sprite(STONE_LIVES), buy_life, LIFES);
    add_item(game->shop, 690, 150, 5, get_sprite(STONE_OXYGEN), buy_oxygen_capacity, OXYGEN);
    add_item(game->shop, 525, 360, 5, get_sprite(STONE_FIRE), buy_lower_cannon_cooldown, FIRE_RATE);
    add_item(game->shop, 698, 338, 5, get_sprite(STONE_SPEED), buy_player_speed, SPEED);
    return game;
}

void destroy_game(Game* game){
    if(game == NULL) return;
    for(int i = 0; i < MAX_TRASH_NUM; ++i) {
        destroy_trash(game->trash[i]);
        game->trash[i] = NULL;
    }
    for(int i = 0; i < MAX_FISH_NUM; ++i){
        destroy_fish(game->fishes[i]);
        game->fishes[i] = NULL;
    }
    destroy_shop(game->shop);
    game->shop = NULL;
    destroy_submarine(game->submarine);
    game->submarine = NULL;
    destroy_entity(game->background);
    game->background = NULL;
    destroy_round_manager(game->round_manager);
    game->round_manager = NULL;
    free(game);
    game = NULL;
}

void draw_game(Game* game, int left, int right, int up, int down, int bg_left, int bg_right, int bg_down, int bg_up, Entity* cursor) {
    float rot = game->submarine->rotation;
    if (((bg_right && bg_up) || (bg_left && bg_down)) && rot >= -45) rot -= 1;
    else if (((bg_right && bg_down) || (bg_left && bg_up)) && rot < 45) rot += 1;
    else if (!((bg_right && bg_down) || (bg_left && bg_up) || (bg_right && bg_up) || (bg_left && bg_down))) {
        if (rot < 0) (rot)++;
        else if (rot > 0) (rot)--;
    }
    game->submarine->rotation = rot;
    if (game->scale == 1) {
        clear_buffer();
        draw_smaller_sprite_line(game->submarine->submarine->sprite, 100, (600-game->submarine->submarine->sprite->height)/2);
        draw_sprite_bottom(game->submarine->hook->hook, game->submarine->submarine->x, game->submarine->submarine->y, game->scale, -game->submarine->rotation, game->submarine->submarine->x - 400, game->submarine->submarine->y -470, game->submarine->hook->height + 74, true);
        draw_fishes(game, true);
        draw_trashes(game, true);
        draw_sprite_background_t(game->background->sprite, game->background->x, game->background->y, 2, rot);
    }
    else {
        draw_larger_sprite_line(game->background->sprite, game->background->x%game->background->sprite->width, game->background->y);
        draw_fishes(game, false);
        draw_trashes(game, false);
    }
    draw_enemies(game->round_manager);
    draw_submarine_and_childs(game->submarine, game->background, cursor, game->scale);
    draw_word("oxygen", 30, 10, 6);
    draw_oxygen_bar(game->submarine);
    draw_hearts(game->submarine);
    draw_sprite(get_sprite(MONEY), 30, 550);
    draw_number(game->current_money, 67, 556);
}

void update_cannon(Game* game, uint8_t cannon_index) {
    Cannon* cannon = game->submarine->cannon[cannon_index];
  for(int i = 0; i < MAX_BULLET_NUM; ++i) {
    if(cannon->activeBullets[i] == NULL) continue;
    Bullet* active_bullet = cannon->activeBullets[i];

    bool collided = false;

    move_bullet(active_bullet);
    if(active_bullet->x - game->background->x > 800 || active_bullet->y - game->background->y > 600 || active_bullet->x - game->background->x < 0 || active_bullet->y - game->background->y < 0 ){
        free(active_bullet);
        cannon->activeBullets[i] = NULL;
        continue;
    }
    if (check_bullet_collisions_with_enemies(cannon->bulletInfo,active_bullet,game->round_manager, game->background, cannon_index, i)) {
        expload_bullet(cannon->explosions, active_bullet, cannon->bulletInfo);
        cannon->activeBullets[i] = NULL;
        game->current_money += 1;
        continue;
    };
    for(int j = 0; j < MAX_FISH_NUM; j++){
        if(game->fishes[j] == NULL) continue;
        int collision_x, collision_y;
        if(check_bullet_collision(cannon->bulletInfo, active_bullet, game->fishes[j]->asp->e, &collision_x, &collision_y)){
            collided = true;
            game->current_money -= game->fishes[j]->money;
            if (game->current_money < 0) game->current_money = 0;
            if (multiplayer) {
                sp_write_wq(DESTROY_HEADER|KILL_FISH);
                sp_write_wq(j);
                uint8_t counter = (rtc_counter + (game->fish_spawn_period - (rtc_counter % game->fish_spawn_period)) +  game->fish_spawn_period*2) % 249;
                sp_write_wq(counter);
                sp_write_wq((cannon_index << 5)|i);
                sp_write_fifo();
                game->next_spawn_fishes[j] = counter; 
            }
            destroy_fish(game->fishes[j]);
            game->fishes[j] = NULL;
            active_bullet->x = collision_x;
            active_bullet->y = collision_y;
            expload_bullet(cannon->explosions, active_bullet, cannon->bulletInfo);
            cannon->activeBullets[i] = NULL;
            break;
        }
    }
    if(collided) continue;
    for (int j = 0; j < MAX_TRASH_NUM; j++) {
        Trash* current_trash = game->trash[j];
        if (current_trash == NULL || current_trash->state == OUT_BUBBLE) continue;
        int collision_x, collision_y;
        if (check_bullet_collision(cannon->bulletInfo, active_bullet, current_trash->trash->e, &collision_x, &collision_y)) {
            collided = true;
            if (multiplayer) {
                    sp_write_wq(DESTROY_HEADER|POP_TRASH);
                    sp_write_wq(j);
                    sp_write_wq((cannon_index << 5) | i);
                    sp_write_fifo();
            }
            trash_pop_bubble(current_trash);
            active_bullet->x = collision_x;
            active_bullet->y = collision_y;
            expload_bullet(cannon->explosions, active_bullet, cannon->bulletInfo);
            cannon->activeBullets[i] = NULL;
            break;
        }
    }
  }
}

void draw_fishes(Game* game, bool on_black) {
    for(int i = 0; i < MAX_FISH_NUM; i++){
        if(game->fishes[i] == NULL) continue;
        animate_entity(game->fishes[i]->asp);
        if (on_black) draw_entity_on_black_background(game->fishes[i]->asp->e, game->background, -game->submarine->rotation, game->scale);
        else draw_entity_on_background(game->fishes[i]->asp->e, game->background, game->scale);
    }
}

void draw_trashes(Game* game, bool on_black) {
    for(int i = 0; i < MAX_TRASH_NUM; i++){
        if(game->trash[i] == NULL) continue;
        draw_trash(game->trash[i], game->background, -game->submarine->rotation, game->scale, on_black);
    }
}

void spawn_trash(Game* game, int counter){
    for(uint8_t i = 0; i < MAX_TRASH_NUM; i++){
        if (game->next_spawn_trashes[i] == counter) {
            game->next_spawn_trashes[i] = -1;
        }
    }
    srandom(game->seed++);
    for(uint8_t i = 0; i < MAX_TRASH_NUM; i++){
        if(game->trash[i] != NULL) continue;
        if (game->next_spawn_trashes[i] != -1) continue;
        int money = game->background->y / 600 + 1;

        uint8_t posX = random() % 50;
        uint8_t posY = random() % 50;
        int x = (posX < 25) ? game->background->x - posX : game->background->x + 800 + 50 - posX;
        int y = (posY < 25) ? game->background->y - posY : game->background->y + 600 + 50 - posY;
        y = MAX(y, 800);
        y = MIN(y, game->background->sprite->height - 100);

        int yspeed = (random() % 2);
        int xspeed = 1 + (random() % 5);
        if(x > game->background->x) xspeed *= -1;
        game->trash[i] = create_trash(money, x, y, xspeed, yspeed);
        return;
    }
}
void spawn_fishes(Game* game, int count, int counter){
    for(uint8_t i = 0; i < MAX_FISH_NUM && count; i++){
        if(game->fishes[i] != NULL) continue;
        if (game->next_spawn_fishes[i] != -1) continue;
        srandom(i);
        uint8_t money = (game->background->y / 600 + 1);

        uint8_t posX = random() % 50;
        uint8_t posY = random() % 50;
        int x = (posX < 25) ? game->background->x - posX : game->background->x + 800 + 50 - posX;
        int y = (posY < 25) ? game->background->y - posY : game->background->y + 600 + 50 - posY;
        y = MAX(y, 600);
        y = MIN(y, game->background->sprite->height - 100);

        int yspeed = 1 + (random() % 5);
        int xspeed = 1 + (random() % 5);

        if(random() % 2 == 0) xspeed *= -1;
        if(random() % 2 == 0) yspeed *= -1;

        game->fishes[i] = create_fish(money, x, y, xspeed, yspeed, i);
        count--;
    }
}

inline void spawn_fish(Game* game, int counter){
    for(uint8_t i = 0; i < MAX_FISH_NUM; i++){
        if (game->next_spawn_fishes[i] == counter) {
            game->next_spawn_fishes[i] = -1;
        }
    }
    spawn_fishes(game, 1, counter);
}

void move_fishes(Game* game){
    for(uint8_t i = 0; i < MAX_FISH_NUM; i++ ) {
        if(game->fishes[i] == NULL) continue;
        Entity* fish = game->fishes[i]->asp->e;
        move_entity(fish);
        if(is_out_of_bounds(game->background, fish, 300)) {
            if (multiplayer) {
                sp_write_wq(DESTROY_HEADER|DESTROY_FISH);
                sp_write_wq(i);
                uint8_t counter = (rtc_counter + (game->fish_spawn_period - (rtc_counter % game->fish_spawn_period)) +  game->fish_spawn_period*2) % 249;
                sp_write_wq(counter);
                sp_write_fifo();
                game->next_spawn_fishes[i] = counter; 
            }
            destroy_fish(game->fishes[i]);
            game->fishes[i] = NULL;
            continue;
        }
        if(fish->y <= SURFACE_LEVEL_Y) {
            fish->yspeed *= -1;
        }
        else if(fish->y >= game->background->sprite->height - fish->sprite->height) fish->yspeed *= -1;
    }
}
void move_trash(Game* game){
    for(uint8_t i = 0; i < MAX_TRASH_NUM; i++ ) {
        if(game->trash[i] == NULL) continue;
        Entity* trash = game->trash[i]->trash->e;
        move_entity(trash);
        if(is_out_of_bounds(game->background, trash, 1000)){
            if (multiplayer) {
                sp_write_wq(DESTROY_HEADER|DESTROY_TRASH);
                sp_write_wq(i);
                uint8_t counter = (rtc_counter + (game->trash_spawn_period - (rtc_counter % game->trash_spawn_period)) +  game->trash_spawn_period*2) % 249;
                sp_write_wq(counter);
                sp_write_fifo();
                game->next_spawn_trashes[i] = counter; 
            }
            destroy_trash(game->trash[i]);
            game->trash[i] = NULL;
            continue;
        }
        if (trash->y >= game->background->sprite->height - trash->sprite->height) trash->yspeed = 0;
    }
}

bool is_out_of_bounds(Entity* background, Entity* entity, uint16_t padding) {
    return (background->y > entity->y + entity->sprite->height/2 + padding ||
        background->y + 600 + padding < entity->y - entity->sprite->height/2) ||
        (background->x > entity->x + entity->sprite->width/2 + padding ||
            background->x + 800 + padding < entity->x - entity->sprite->width/2);
   
}

bool check_catch_trash(Game* game, uint8_t* caught_trash) {
    for (int i = 0; i < MAX_TRASH_NUM; i++) {
        if (game->trash[i] == NULL || game->trash[i]->state != OUT_BUBBLE) continue;
        int hook_y = game->background->y + game->submarine->submarine->y + game->submarine->hook->height/2 + (game->submarine->submarine->sprite->height/4);
        int hook_x = game->background->x + game->submarine->submarine->x;
        if (game->trash[i]->trash->e->x <= hook_x + 20 
        && game->trash[i]->trash->e->x >= hook_x - 20 
        && game->trash[i]->trash->e->y <= hook_y + 20 &&
         game->trash[i]->trash->e->y >= hook_y - 35) {
            game->current_money += game->trash[i]->money;
            if (multiplayer) {
                sp_write_wq(DESTROY_HEADER|CATCH_TRASH);
                sp_write_wq(i);
                uint8_t counter = (rtc_counter + (game->trash_spawn_period - (rtc_counter % game->trash_spawn_period)) +  game->trash_spawn_period*2) % 249;
                sp_write_wq(counter);
                sp_write_fifo();
                game->next_spawn_trashes[i] = counter; 
            }
            destroy_trash(game->trash[i]);
            game->trash[i] = NULL;
            *caught_trash = i;
            return true;
        }
    }
    return false;
}

void update_buyable_items(Game* game) {
    for (int i = 0; i < MAX_ITEMS_SIZE; i++) {
        if (game->shop->items[i] != NULL) {
            switch (i) {
                case LIFES:
                     game->shop->items[i]->buyable = game->submarine->current_number_of_lifes < game->submarine->max_number_of_lifes;
                    break;
                case OXYGEN:
                    game->shop->items[i]->buyable = game->submarine->oxygen_capacity < 5000;
                    break;
                case SPEED:
                    game->shop->items[i]->buyable = game->submarine->player_speed < 15;
                    break;
                case FIRE_RATE:
                    game->shop->items[i]->buyable = game->submarine->fire_cooldown > 5;
                    break;
            }
        }
    }
}
