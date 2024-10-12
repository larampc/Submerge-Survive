#include "../lib/devices/sp/sp.h"
#include "handler.h"
#include "proj.h"
#include "./lib/devices/rtc/rtc.h"
#include "moving_entities/trash.h"
#include "round_manager.h"

int player_num = -1;
bool multiplayer = false;
extern APP_STATE app_state;
uint16_t x = 0;
uint16_t y = 0;
int object_index = -1;
int spawn_rate = -1;
int bullet = -1;
extern uint16_t more_bytes;
extern Entity* cursor;
bool end = false;
extern int rtc_counter;
int ping;
bool wait_ping = false;
bool wait = false;

void connect_handler(Game* game, uint8_t byte) {
    switch (byte) {
        case CONNECT_PLAYER_0:
            if (!wait_ping && ((app_state == WAITING) || (app_state == GAME)) && player_num == 1) {
                sp_write_wq(CONNECT_HEADER|CONNECT_START_GAME);
                sp_write_fifo();
            }
            break;
        case CONNECT_PLAYER_1:
            if (!wait_ping && ((app_state == WAITING) || (app_state == GAME)) && player_num == 0) {
                sp_write_wq(CONNECT_HEADER|CONNECT_START_GAME);
                sp_write_fifo();
            }
            break;
        case CONNECT_START_GAME:
            if (!wait_ping && player_num == 0 && (app_state == WAITING)) {
                rtc_subscribe_periodic_interrupts(PERIODIC_1024);
                sp_write_wq(CONNECT_HEADER|PING_CODE);
                sp_write_fifo();
                wait_ping = true;
            }
            break;
        case QUIT_GAME:
            end = true;
            break;
        case PING_CODE:
            if (player_num == 1 && (app_state == WAITING)) {
                sp_write_wq(CONNECT_HEADER|RECEIVED_PING);
                sp_write_fifo();
            }
            break;
        case RECEIVED_PING:
            if (player_num == 0 && (app_state == WAITING)) {
                ping = rtc_counter/2;
                rtc_unsubscribe_periodic_interrupts();
                rtc_subscribe_periodic_interrupts(PERIODIC_1024);
                sp_write_wq(CONNECT_HEADER|START_GAME);
                sp_write_fifo();
                wait = true;
                wait_ping = false;
            }
            break;
        case START_GAME:
            if (player_num == 1) {
                multiplayer = true;
            }
            break;
        case GAME_OVER:  
            app_state = GAMEOVER;
            break;
        default:
            break;
    }
}

void player_handler(Game* game, uint8_t byte) {
    if (game == NULL) return;
    switch (byte) {
        case PLAYER_MOVE_UP:
            move_main_player(game->submarine->players[1], UP, game->submarine->player_speed);
            break;
        case PLAYER_MOVE_DOWN:
            move_main_player(game->submarine->players[1], DOWN, game->submarine->player_speed);
            break;
        case PLAYER_MOVE_LEFT:
            move_main_player(game->submarine->players[1], LEFT, game->submarine->player_speed);
            break;
        case PLAYER_MOVE_RIGHT:
            move_main_player(game->submarine->players[1], RIGHT, game->submarine->player_speed);
            break;
        case PLAYER_STOPPED:
            move_main_player(game->submarine->players[1], STOP, game->submarine->player_speed);
            break;
        case PLAYER_NONE:
            game->submarine->players[1]->state = WALK;
            break;
        case PLAYER_DRIVE:
            game->submarine->players[1]->state = DRIVE;
            break;
        case PLAYER_CANNON_1:
            game->submarine->players[1]->state = INCANNON1;
            break;
        case PLAYER_CANNON_2:
            game->submarine->players[1]->state = INCANNON2;
            break;
        case PLAYER_CANNON_3:
            game->submarine->players[1]->state = INCANNON3;
            break;
        case PLAYER_CANNON_4:
            game->submarine->players[1]->state = INCANNON4;
            break;
        case PLAYER_CANNON_5:
            game->submarine->players[1]->state = INCANNON5;
            break;

        default:
            break;
    }
}

void sub_handler(Game* game, uint8_t byte) {
    if (game == NULL) return;
    switch (byte) {
        case BG_UP:
            if (game->background->y-5 >= 0) game->background->y -= 5;
            break;
        case BG_DOWN:
            if (game->background->y+5 <= game->background->sprite->height-600) game->background->y += 5;
            break;
        case BG_LEFT:
            game->background->x -= 5;
            break;
        case BG_RIGHT:
            game->background->x += 5;
            break;
        case BG_UP_LEFT:
            if (game->background->y-5 >= 0) game->background->y -= 5;
            game->background->x -= 5;
            if (game->submarine->rotation < 45) game->submarine->rotation += 1;
            if (game->submarine->rotation != 45) {
                if (game->submarine->rotation > 0) (game->submarine->rotation)++;
                if (game->submarine->rotation < 0) (game->submarine->rotation)--;
            }
            break;
        case BG_UP_RIGHT:
            if (game->background->y-5 >= 0) game->background->y -= 5;
            game->background->x += 5;
            if (game->submarine->rotation > -45) game->submarine->rotation -= 1;
            if (game->submarine->rotation != -45) {
                if (game->submarine->rotation > 0) (game->submarine->rotation)++;
                if (game->submarine->rotation < 0) (game->submarine->rotation)--;
            }
            break;
        case BG_DOWN_LEFT:
            if (game->background->y+5 <= game->background->sprite->height-600) game->background->y += 5;
            game->background->x -= 5;
            if (game->submarine->rotation > -45) game->submarine->rotation -= 1;
            if (game->submarine->rotation != -45) {
                if (game->submarine->rotation > 0) (game->submarine->rotation)++;
                if (game->submarine->rotation < 0) (game->submarine->rotation)--;
            }
            break;
        case BG_DOWN_RIGHT:
            if (game->background->y+5 <= game->background->sprite->height-600) game->background->y += 5;
            game->background->x += 5;
            if (game->submarine->rotation < 45) game->submarine->rotation += 1;
            if (game->submarine->rotation != 45) {
                if (game->submarine->rotation > 0) (game->submarine->rotation)++;
                if (game->submarine->rotation < 0) (game->submarine->rotation)--;
            }
            break;

        default:
            break;
    }
}

void cannon_handler(Game* game, uint8_t byte, queue_t *read_queue) {
    if (game == NULL) return;
    switch (byte) {
        case CANNON_ROTATE:
            if ((more_bytes & 0xFF) == 0) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((CANNON_HEADER|CANNON_ROTATE) << 8);
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                x = (byte << 8);
            }
            if ((more_bytes & 0xFF) <= 1) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((CANNON_HEADER|CANNON_ROTATE) << 8) | 1;
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                x |= byte;
            }
            if ((more_bytes & 0xFF) <= 2) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((CANNON_HEADER|CANNON_ROTATE) << 8) | 2;
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                y = (byte << 8);
            }
            if ((more_bytes & 0xFF) <= 3) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((CANNON_HEADER|CANNON_ROTATE) << 8) | 3;
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                y |= byte;
            }
            game->submarine->cannon[game->submarine->players[1]->state - INCANNON1]->pointX = (x - 400)*(1/0.5) + 400;
            game->submarine->cannon[game->submarine->players[1]->state - INCANNON1]->pointY = (y - 300)*(1/0.5) + 300;
            x = 0;
            y = 0;
            more_bytes = 0x0;
            break;
        case CANNON_FIRE:
            game->submarine->cannon[game->submarine->players[1]->state - INCANNON1]->pointX = (game->submarine->cannon[game->submarine->players[1]->state - INCANNON1]->pointX - 400)/(1/0.5) + 400;
            game->submarine->cannon[game->submarine->players[1]->state - INCANNON1]->pointY = (game->submarine->cannon[game->submarine->players[1]->state - INCANNON1]->pointY - 300)/(1/0.5) + 300;
            fire(game->submarine->cannon[game->submarine->players[1]->state - INCANNON1], game->background, game->scale, game->submarine->fire_cooldown, rtc_counter);
            game->submarine->cannon[game->submarine->players[1]->state - INCANNON1]->pointX = (game->submarine->cannon[game->submarine->players[1]->state - INCANNON1]->pointX - 400)*(1/0.5) + 400;
            game->submarine->cannon[game->submarine->players[1]->state - INCANNON1]->pointY = (game->submarine->cannon[game->submarine->players[1]->state - INCANNON1]->pointY - 300)*(1/0.5) + 300;
            break;

        default:
            break;
    }
}

void hook_handler(Game* game, uint8_t byte) {
    if (game == NULL) return;
    game->submarine->hook->height += ((byte & BIT(0)) ? -10 : 10);
}

void (shop_handler)(Game* game, uint8_t byte) {
    if (game == NULL) return;
    switch (byte) {
        case LIFES: 
            game->current_money -= game->shop->items[LIFES]->price;
            game->shop->items[LIFES]->on_click_effect(game);
            update_buyable_items(game);
            break;
        case OXYGEN:
            game->current_money -= game->shop->items[OXYGEN]->price;
            game->shop->items[OXYGEN]->on_click_effect(game);
            update_buyable_items(game);
            break;
        case FIRE_RATE:
            game->current_money -= game->shop->items[FIRE_RATE]->price;
            game->shop->items[FIRE_RATE]->on_click_effect(game);
            update_buyable_items(game);
            break;
        case SPEED:
            game->current_money -= game->shop->items[SPEED]->price;
            game->shop->items[SPEED]->on_click_effect(game);
            update_buyable_items(game);
            break;
    }
}

void destroy_handler(Game* game, uint8_t byte, queue_t *read_queue) {
    if (game == NULL) return;
    Cannon* cannon;
    Bullet* active_bullet;
    switch (byte) {
        case DESTROY_FISH:
            if ((more_bytes & 0xFF) == 0) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((DESTROY_HEADER|DESTROY_FISH) << 8);
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                object_index = (int)byte;
            }
            if ((more_bytes & 0xFF) <= 1) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((DESTROY_HEADER|DESTROY_FISH) << 8) | 1;
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                spawn_rate = (int)byte;
            }
            game->next_spawn_fishes[object_index] = MAX(game->next_spawn_fishes[object_index], spawn_rate);
            destroy_fish(game->fishes[object_index]);
            game->fishes[object_index] = NULL;
            object_index = -1;
            spawn_rate = -1;
            more_bytes = 0x0;
            break;
        case DESTROY_TRASH:
            if ((more_bytes & 0xFF) == 0) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((DESTROY_HEADER|DESTROY_TRASH) << 8);
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                object_index = (int)byte;
            }
            if ((more_bytes & 0xFF) <= 1) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((DESTROY_HEADER|DESTROY_TRASH) << 8) | 1;
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                spawn_rate = (int)byte;
            }
            game->next_spawn_trashes[object_index] = MAX(game->next_spawn_trashes[object_index], spawn_rate);
            destroy_trash(game->trash[object_index]);
            game->trash[object_index] = NULL;
            object_index = -1;
            spawn_rate = -1;
            more_bytes = 0x0;
            break;
        case KILL_FISH:
            if ((more_bytes & 0xFF) == 0) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((DESTROY_HEADER|KILL_FISH) << 8);
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                object_index = (int)byte;
            }
            if ((more_bytes & 0xFF) <= 1) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((DESTROY_HEADER|KILL_FISH) << 8) | 1;
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                spawn_rate = (int)byte;
            }
            if ((more_bytes & 0xFF) <= 2) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((CANNON_HEADER|CANNON_ROTATE) << 8) | 2;
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                bullet = (int)byte;
            }
            game->next_spawn_fishes[object_index] = MAX(game->next_spawn_fishes[object_index], spawn_rate);
            if (game->fishes[object_index] != NULL) {
                if (game->current_money - game->fishes[object_index]->money > 0) game->current_money -= game->fishes[object_index]->money;
                destroy_fish(game->fishes[object_index]);
                game->fishes[object_index] = NULL;
            }
            cannon = game->submarine->cannon[bullet >> 5];
            active_bullet = cannon->activeBullets[bullet & 0x1f];
            if (active_bullet != NULL) {
                expload_bullet(cannon->explosions, active_bullet, cannon->bulletInfo);
                cannon->activeBullets[bullet & 0x1f] = NULL;
            }            
            object_index = -1;
            spawn_rate = -1;
            bullet = -1;
            more_bytes = 0x0;
            break;
        case CATCH_TRASH:
            if ((more_bytes & 0xFF) == 0) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((DESTROY_HEADER|CATCH_TRASH) << 8);
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                object_index = (int)byte;
            }
            if ((more_bytes & 0xFF) <= 1) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((DESTROY_HEADER|CATCH_TRASH) << 8) | 1;
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                spawn_rate = (int)byte;
            }
            game->next_spawn_trashes[object_index] = MAX(game->next_spawn_trashes[object_index], spawn_rate);
            if (game->trash[object_index] != NULL) {
                game->current_money += game->trash[object_index]->money;
                destroy_trash(game->trash[object_index]);
                game->trash[object_index] = NULL;
            }
            object_index = -1;
            spawn_rate = -1;
            more_bytes = 0x0;
            break;
        case POP_TRASH:
            if ((more_bytes & 0xFF) == 0) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((DESTROY_HEADER|POP_TRASH) << 8);
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                object_index = (int)byte;
            }
            if ((more_bytes & 0xFF) <= 1) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((DESTROY_HEADER|CATCH_TRASH) << 8) | 1;
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                bullet = (int)byte;
            }
            trash_pop_bubble(game->trash[object_index]);
            cannon = game->submarine->cannon[bullet >> 5];
            active_bullet = cannon->activeBullets[bullet & 0x1f];
            if (active_bullet != NULL) {
                expload_bullet(cannon->explosions, active_bullet, cannon->bulletInfo);
                cannon->activeBullets[bullet & 0x1f] = NULL;
            }
            object_index = -1;
            bullet = -1;
            more_bytes = 0x0;
            break;
        case KILL_ENEMY:
            if ((more_bytes & 0xFF) == 0) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((DESTROY_HEADER|KILL_ENEMY) << 8);
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                object_index = (int)byte;
            }
            if ((more_bytes & 0xFF) <= 1) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((DESTROY_HEADER|KILL_ENEMY) << 8) | 1;
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                bullet = (int)byte;
            }
            if (game->round_manager->enemies[object_index]->active) {
                die(game->round_manager->enemies[object_index]);
                game->round_manager->active_enemies--;
                if(game->round_manager->active_enemies == 0 && game->round_manager->created_enemies == game->round_manager->round_enemies)  
                    finish_round(game->round_manager);
                game->current_money += 1;
                cannon = game->submarine->cannon[bullet >> 5];
                active_bullet = cannon->activeBullets[bullet & 0x1f];
                if (active_bullet != NULL) {
                    expload_bullet(cannon->explosions, active_bullet, cannon->bulletInfo);
                    cannon->activeBullets[bullet & 0x1f] = NULL;
                }
            }
            object_index = -1;
            bullet = -1;
            more_bytes = 0x0;
            break;
        case DESTROY_ENEMY_BULLET:
            if ((more_bytes & 0xFF) == 0) {
                if (pop_queue(read_queue, &byte)) {
                    more_bytes = ((DESTROY_HEADER|DESTROY_ENEMY_BULLET) << 8);
                    //printf("ERROR: unable to pop read queue inside %s\n", __func__);
                    return;
                }
                if (game->round_manager->enemy_bullets[byte] != NULL) {
                    expload_bullet(game->round_manager->explosions, game->round_manager->enemy_bullets[byte], game->round_manager->bulletInfo);
                    game->round_manager->enemy_bullets[byte] = NULL;
                }
            }
            more_bytes = 0x0;
            break;
        default:
            break;
    }
}
