#include <lcom/lcf.h>
#include <lcom/proj.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "proj.h"
#include "lib/devices/video_card/video.h"
#include "lib/devices/keyboard/keyboard.h"
#include "lib/devices/timer/timer.h"
#include "lib/devices/mouse/mouse.h"
#include "lib/devices/kbc/i8042.h"
#include "lib/devices/sp/sp.h"
#include "lib/devices/RTC/rtc.h"
#include "game/cannon.h"
#include "graphics/alphabet.h"
#include "menu/leaderboard.h"
#include "game/game.h"
#include "game/moving_entities/entity.h"
#include "game/moving_entities/animatedEntity.h"
#include "game/moving_entities/mainPlayer.h"
#include "game/shop.h"


extern uint8_t scancode;
bool two_b = false;
bool move = false;
extern int byte;
extern bool multiplayer;
extern int player_num;
uint8_t bit_no_sp = 4;
extern bool scroll;
int bad_sp = 1;
bool quit = 0;
extern uint8_t hours;
extern uint8_t minutes;
extern uint8_t seconds;
extern bool end;
extern int rtc_counter;
extern int elapsed_time;
extern bool wait;
extern int ping;
char player_name[8] = {0};

APP_STATE app_state = MAINMENU;

extern int count;

Entity* cursor;
AnimatedEntity* player_select[2];
Game* game;

bool up = false, down = false, left = false, right = false, bg_up = false, bg_down = false, bg_left = false, bg_right = false;
MOVEMENT movement[4] = {0,0,0,0};

static void (*ih[NUM_DEVICES][NUM_STATES])() =
{{menu_timer_ih, game_timer_ih, playermenu_timer_ih, waiting_timer_ih, pause_timer_ih, gameover_timer_ih, empty_ih, shop_timer_ih},
{empty_ih, game_kbd_ih, kb_empty, kb_empty, pause_kb, gameover_kb, kb_empty, shop_kb},
{menu_mouse_ih, game_mouse_ih, playermenu_mouse_ih, kb_empty, pause_mouse_ih, empty_ih, empty_ih, shop_mouse_ih},
{menu_sp_ih, game_sp_ih, menu_sp_ih, waiting_sp_ih, game_sp_ih, empty_ih, game_sp_ih, game_sp_ih},
{empty_ih, game_rtc_ih, empty_ih, waiting_rtc_ih, pause_rtc_ih, empty_ih, empty_ih, pause_rtc_ih}};


void (empty_ih)() {
    return;
}

void (menu_timer_ih)() {
    if (count % 4 == 0) {
        draw_sprite_full_screen(get_sprite(MENU));
        if (draw_sprite(cursor->sprite, cursor->x, cursor->y) != 0) return;
        vg_page_flip();
    }
}

void (shop_timer_ih)() {
    if (count % 4 == 0) {
        draw_sprite_full_screen(get_sprite(SHOP_BG));
        draw_sprite(get_sprite(MONEY), 30, 550);
        draw_number(game->current_money, 67, 556);
        draw_shop(game->shop, game->current_money);
        if (draw_sprite(cursor->sprite, cursor->x, cursor->y) != 0) return;
        vg_page_flip();
    }
}

void (gameover_timer_ih)() {
    static uint8_t draw_placer = 0;
    if (count % 4 == 0) {
        draw_sprite_full_screen(get_sprite(GAMEOVER_BG));
        draw_string(player_name, 10, 320, 423, false, 0xffffff);
        if (draw_placer == 10) draw_placer = 0;
        if (draw_placer < 5) {
            vg_draw_rectangle(325 + strlen(player_name)*22 , 423, 2, 30, 0xffffff);
            draw_placer++;
        }
        else draw_placer++;
        vg_page_flip(); 
    }
}

void (pause_timer_ih)() {
    if (count % 4 == 0) {
        draw_sprite_full_screen(get_sprite(PAUSE_BG));
        if (draw_sprite(cursor->sprite, cursor->x, cursor->y) != 0) return;
        vg_page_flip(); 
    }
}

void (game_timer_ih)() {
    if (count % 4 == 0) {
        update_scene();
    }
}

void (game_rtc_ih)() {
    if (game->round_manager->under_attack) {
        move_enemies(game->round_manager, game->submarine);
        if (rtc_counter % game->round_manager->appearence_rate == 0) {
            update_enemies(game->round_manager);
            update_round(game->round_manager);
        }
    }
    move_fishes(game);
    move_trash(game);
    if (rtc_counter % 20 == 0) {
        for(uint8_t i = 0; i < MAX_FISH_NUM; i++ ) {
            if (game->fishes[i] == NULL) continue;
            change_fish_direction(game->fishes[i]);
        }
        update_oxygen(game->submarine);
        if(game->submarine->oxygen_counter == 0){
            app_state = GAMEOVER;
            if(multiplayer){
                sp_write_wq(CONNECT_HEADER|GAME_OVER);
                sp_write_fifo();
            }
        } 
    }
    if (game->submarine->state == SUBMERGED && rtc_counter % game->fish_spawn_period == 0) spawn_fish(game, rtc_counter);
    if (game->submarine->state == SUBMERGED && rtc_counter % game->trash_spawn_period == 0) spawn_trash(game, rtc_counter);
}

void (pause_rtc_ih)() {
    if (multiplayer) {
        game_rtc_ih();
    }
}

void (waiting_rtc_ih)() {
    if (wait && ping == rtc_counter) {
        rtc_unsubscribe_periodic_interrupts();
        multiplayer = true;
        wait = false;
        game_init();
    }
}


void (kb_empty)() {
    if (scancode == ESC) {
        menu_init();
    }
    return;
}

void (gameover_kb)() {
    if (update_player_name(player_name,scancode)) {
        char playertime[10];
        rtc_get_date(playertime);
        write_file_leaderboard(player_name, game->current_money, playertime);
        memset(player_name, 0, sizeof(player_name));
        end_game();
        app_state = LEADERBOARD;
        leaderboard_init();
    }
    return;
}

void (pause_kb)() {
    if (scancode == ESC) {
        app_state = GAME;
        cursor->sprite = get_sprite(AIM);
    }
    return;
}

void (shop_kb)() {
    if (scancode == ESC || scancode == I_KEY) {
        app_state = GAME;
        cursor->sprite = get_sprite(AIM);
    }
    return;
}

void (menu_mouse_ih)() {
    mouse_packet pp;
    assemble_packet(&pp);
    if (cursor->x + pp.delta_x < 800 - cursor->sprite->width && cursor->x + pp.delta_x > 0) cursor->x += pp.delta_x;
    if (cursor->y - pp.delta_y < 600 - cursor->sprite->height && cursor->y - pp.delta_y > 0) cursor->y -= pp.delta_y;
    int cursor_x = cursor->x - cursor->sprite->width/2;
    int cursor_y = cursor->y - cursor->sprite->height/2;
    if (pp.lb &&  cursor_x > 270 && cursor_x < 476 && cursor_y >312 && cursor_y < 349) {
        game_init();
        return;
    }
    else if (pp.lb && cursor_x > 270 && cursor_x < 476
    && cursor_y >361 && cursor_y < 398) {
        playermenu_init();
        return;
    }
    else if (pp.lb && cursor_x > 270 && cursor_x < 476 && cursor_y > 410 && cursor_y < 446) {
        app_state = LEADERBOARD;
        leaderboard_init();
        return;
    }
    else if (pp.lb && cursor_x > 270 && cursor_x < 476 && cursor_y > 457 && cursor_y < 493) {
        quit = true;
        return;
    }
}

void (shop_mouse_ih)() {
    mouse_packet pp;
    assemble_packet(&pp);
    if (cursor->x + pp.delta_x < 800 - cursor->sprite->width && cursor->x + pp.delta_x > 0) cursor->x += pp.delta_x;
    if (cursor->y - pp.delta_y < 600 - cursor->sprite->height && cursor->y - pp.delta_y > 0) cursor->y -= pp.delta_y;
    int cursor_x = cursor->x - cursor->sprite->width/2;
    int cursor_y = cursor->y - cursor->sprite->height/2;
    if (pp.lb) {
        ITEMS selected_item;
        Item* item = get_clicked_item(game->shop, cursor_x, cursor_y, &selected_item);
        if(item != NULL && item->buyable && item->price <= game->current_money) {
            item->on_click_effect(game);
            if (multiplayer) {
                sp_write_wq(SHOP_HEADER|selected_item);
                sp_write_fifo();
            }
            game->current_money -= item->price;
            update_buyable_items(game);
        }
    }
}

void (pause_mouse_ih)() {
    mouse_packet pp;
    assemble_packet(&pp);
    if (cursor->x + pp.delta_x < 800 - cursor->sprite->width && cursor->x + pp.delta_x > 0) cursor->x += pp.delta_x;
    if (cursor->y - pp.delta_y < 600 - cursor->sprite->height && cursor->y - pp.delta_y > 0) cursor->y -= pp.delta_y;
    int cursor_x = cursor->x - cursor->sprite->width/2;
    int cursor_y = cursor->y - cursor->sprite->height/2;
    if (pp.lb && cursor_x > 270 && cursor_x < 476 && cursor_y > 312 && cursor_y < 349) {
        app_state = GAME;
        cursor->sprite = get_sprite(AIM);
        return;
    }
    else if (pp.lb && cursor_x > 270 && cursor_x < 476 && cursor_y > 361 && cursor_y < 398) {
        if (multiplayer) {
            sp_write_wq(CONNECT_HEADER|QUIT_GAME);
            sp_write_fifo();
        }
        end_game();
        menu_init();
        return;
    }
    else if (pp.lb && cursor_x > 270 && cursor_x < 476 && cursor_y > 410 && cursor_y < 446) {
        if (multiplayer) {
            sp_write_wq(CONNECT_HEADER|QUIT_GAME);
            sp_write_fifo();   
        }
        quit = true;
        return;
    }
}

void (game_mouse_ih)() {
    mouse_packet pp;
    assemble_packet(&pp);
    if (cursor->x + pp.delta_x < 800 - cursor->sprite->width && cursor->x + pp.delta_x > 0) cursor->x += pp.delta_x;
    if (cursor->y - pp.delta_y < 600 - cursor->sprite->height && cursor->y - pp.delta_y > 0) cursor->y -= pp.delta_y;
    if(pp.lb && game->submarine->players[0]->state >= INCANNON1){
        if (fire(game->submarine->cannon[game->submarine->players[0]->state-INCANNON1], game->background, game->scale, game->submarine->fire_cooldown, elapsed_time) == 0) {
            if (multiplayer) {
            sp_write_wq(CANNON_HEADER|CANNON_FIRE);
            sp_write_fifo();   
            }
        };
    }
    if (game->submarine->players[0]->state == DRIVE) {
        if (game->submarine->hook->height + 10*pp.delta_z > 0 && game->submarine->hook->height+ 10*pp.delta_z < game->submarine->hook->hook->height) {
            game->submarine->hook->height += 10 * pp.delta_z;
            if (multiplayer) {
                for (uint8_t i = 0; i < abs(pp.delta_z); i++) {
                    sp_write_wq(HOOK_HEADER | (pp.delta_z < 0? BIT(0): 0));
                }
                sp_write_fifo();
            }
        }
        if (pp.lb) {
            uint8_t caught_trash;
            if (check_catch_trash(game, &caught_trash)) {
                if (multiplayer) {
                    sp_write_wq(DESTROY_HEADER|CATCH_TRASH);
                    sp_write_wq(caught_trash);
                    sp_write_fifo();
                }
            }
        }
    }
}

void (menu_sp_ih)() {
    sp_ih(NULL);
}

void (waiting_sp_ih)() {
    sp_ih(NULL);
    if (multiplayer) {
        game_init();
    }
}

void (game_kbd_ih)() {
    if (scancode == ESC) {
        app_state = PAUSE;
        clear_movements();
        cursor->sprite = get_sprite(CURSOR);
        return;
    }
    else if (scancode == I_KEY) {
        app_state = SHOP;
        update_buyable_items(game);
        clear_movements();
        cursor->sprite = get_sprite(CURSOR);
        return;
    }
    if (game->submarine->players[0]->state == WALK) update_move();
    else if (game->submarine->players[0]->state == DRIVE) update_move_background();
    update_player_state();
}

void (game_sp_ih)() {
    if (multiplayer) {
        sp_ih(game);
        if (end) {
            end = false;
            end_game();
            menu_init();
        }
    }
}

void (playermenu_mouse_ih)() {
    mouse_packet pp;
    assemble_packet(&pp);
    if (cursor->x + pp.delta_x < 800 - cursor->sprite->width && cursor->x + pp.delta_x > 0) cursor->x += pp.delta_x;
    if (cursor->y - pp.delta_y < 600 - cursor->sprite->height && cursor->y - pp.delta_y > 0) cursor->y -= pp.delta_y;
    if (pp.lb && cursor->x > 0 && cursor->x < 400) {
        player_num = 0;
        waiting_init();
    }
    else if (pp.lb) {
        player_num = 1;
        waiting_init();
    }
}

void (playermenu_timer_ih)() {
    if (count % 4 == 0) {
        draw_sprite_full_screen(get_sprite(CHOOSEPLAYER));
        draw_sprite_t(player_select[0]->e->sprite, 250, 315, 2.5, 0, 0, 0, 0);
        draw_sprite_t(player_select[1]->e->sprite, 555, 315, 2.5, 0, 0, 0, 1);
        if (draw_sprite(cursor->sprite, cursor->x, cursor->y) != 0) return;
        vg_page_flip();
    }
    if (count % 5 == 0) {
        animate_entity(player_select[0]);
        animate_entity(player_select[1]);
    }
}

void (waiting_timer_ih)() {
    if (count % 120 == 0) {
        sp_write_wq(CONNECT_HEADER|CONNECT_PLAYER_N(player_num));
        sp_write_fifo();
        if (bad_sp == 2) scancode = ESC;
        else if (bad_sp == 1) bad_sp = 2;
    }
}

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
    if (vg_initialize(DIRECT_24) != 0) return 1;
    clear_buffer();
    int ipc_status;
    message msg;
    uint8_t bit_no_mouse = 1;
    uint8_t bit_no_kb = 2;
    uint8_t bit_no_timer = 3;
    uint8_t bit_no_rtc = 5;
    if (mouse_enable_scroll() != 0) return 1;
    if (mouse_enable_reporting() != 0) return 1;
    if (mouse_subscribe_int(&bit_no_mouse) != 0) return 1;
    if (keyboard_subscribe_int(&bit_no_kb) != 0) return 1;
    if (timer_subscribe_int(&bit_no_timer) != 0) return 1;
    if(rtc_subscribe_int(&bit_no_rtc) != 0) return 1;
    rtc_init();
    if (sp_init() != 0) return 1;
    if (sp_subscribe_int(&bit_no_sp) != 0) return 1;
    initialize();
    menu_init();
    while(!quit) {
        int r;
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & bit_no_timer) {
                        timer_ih();
                        (*ih[TIMER][app_state]) ();
                    } 				
                    if (msg.m_notify.interrupts & bit_no_kb) {
                        kbd_ih();
                        if (scancode == TWOB_SCAN) two_b = true;
                        else {
                            (*ih[KEYBOARD][app_state]) ();
                            two_b = false;
                        }
                    }
                    if (msg.m_notify.interrupts & bit_no_mouse) {
                        mouse_ih();
                        if (byte == (scroll? 4: 3)) {
                            (*ih[MOUSE][app_state]) ();
                            byte = 0;
                        }
                    }
                    if (msg.m_notify.interrupts & bit_no_sp) {
                        if (bad_sp) bad_sp = 0;
                        if (!(app_state == GAME && !multiplayer)) {
                            (*ih[SP][app_state]) ();
                        }
                    }
                    if(msg.m_notify.interrupts & bit_no_rtc) {
                        if (rtc_ih())(*ih[RTC][app_state])();
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */	
            }
        } else { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }
    destroy_sprites();
    if(rtc_unsubscribe_int()!= 0) return 1;
    // delete_sp();
    // if (sp_unsubscribe_int()!=0) return 1;
    if (mouse_unsubscribe_int()!=0) return 1;
    if (mouse_disable_reporting()!=0) return 1;
    if (keyboard_unsubscribe_int()!=0) return 1;
    if (timer_unsubscribe_int()!=0) return 1;
    if (vg_exit() != 0) return 1;
    return 0;
}

void add_movement(MOVEMENT move) {
    for (int i = 3; i > 0; i--) {
        movement[i] = movement[i-1];
    }
    movement[0] = move;
}

void remove_movement(MOVEMENT move) {
    for (int i = 0; i < 4; i++) {
        if (movement[i] == move) {
            for (int j = i; j < 3; j++) {
                movement[j] = movement[j+1];
            }        
            movement[3] = 0;
            return; 
        }
    }
}

void update_move() {
    switch (scancode) {
    case S_KEY:
        if (!down) {
            add_movement(DOWN);
            down = true;
        }
        break;
    case W_KEY:
        if (!up) {
            add_movement(UP);
            up = true;
        }
        break;
    case A_KEY:
        if (!left) {
            add_movement(LEFT);
            left = true;
        }
        break;
    case D_KEY:
        if (!right) {
            add_movement(RIGHT);
            right = true;
        }
        break;
    case BREAK(S_KEY):
        remove_movement(DOWN);
        down = false;
        break;
    case BREAK(W_KEY):
        remove_movement(UP);
        up = false;
        break;
    case BREAK(A_KEY):
        remove_movement(LEFT);
        left = false;
        break;
    case BREAK(D_KEY):
        remove_movement(RIGHT);
        right = false;
        break;
    default:
        break;
    }
    if (!left && !right && !up && !down) {
        move_main_player(game->submarine->players[0], STOP, game->submarine->player_speed);
        if (multiplayer) {
            sp_write_wq(PLAYER_HEADER|PLAYER_STOPPED);
            sp_write_fifo();
        }
    }
}

void update_move_background() {
    if (scancode == W_KEY) bg_up = true;
    if (scancode == S_KEY) bg_down = true;
    if (scancode == A_KEY) bg_left = true;
    if (scancode == D_KEY) bg_right = true;
    if (scancode == BREAK(S_KEY)) bg_down = false;
    if (scancode == BREAK(W_KEY)) bg_up = false;
    if (scancode == BREAK(A_KEY)) bg_left = false;
    if (scancode == BREAK(D_KEY)) bg_right = false;
}

void update_player_state() {
    if (scancode == BREAK(E_KEY)) {
        if (game->submarine->players[0]->state >= INCANNON1)  {
            deactivate_cannon(game->submarine);
        }
        if (game->submarine->players[0]->state != WALK) {
            game->submarine->players[0]->state = WALK;
            bg_down = false;
            bg_up = false;
            bg_left = false;
            bg_right = false;
            if (multiplayer) {
                sp_write_wq(PLAYER_HEADER|PLAYER_NONE);
                sp_write_fifo();
            }
        }
        else if ((game->submarine->state != SURFACE || (!game->round_manager->under_attack)) && check_drive(game->submarine) && (!multiplayer || game->submarine->players[1]->state != DRIVE)) {
            remove_movement(DOWN);
            down = false;
            remove_movement(UP);
            up = false;
            remove_movement(LEFT);
            left = false;
            remove_movement(RIGHT);
            right = false;
            game->submarine->players[0]->state = DRIVE;
            move_main_player(game->submarine->players[0], STOP, game->submarine->player_speed);
            if (multiplayer) {
                sp_write_wq(PLAYER_HEADER|PLAYER_STOPPED);
                sp_write_wq(PLAYER_HEADER|PLAYER_DRIVE);
                sp_write_fifo();
            }
        }
        else if (activate_cannon(game->submarine)) {
            remove_movement(DOWN);
            down = false;
            remove_movement(UP);
            up = false;
            remove_movement(LEFT);
            left = false;
            remove_movement(RIGHT);
            right = false;
            move_main_player(game->submarine->players[0], STOP, game->submarine->player_speed);
            if (multiplayer) {
                sp_write_wq(PLAYER_HEADER|PLAYER_STOPPED);
                sp_write_wq(PLAYER_HEADER|PLAYER_CANNON_N(game->submarine->players[0]->state - INCANNON1));
                sp_write_fifo();
            }
        }
    }
    if (game->submarine->state == SURFACE && game->round_manager->active_enemies == 0 && game->round_manager->created_enemies == game->round_manager->round_enemies) {
       finish_round(game->round_manager);
    }
    if (game->submarine->players[0]->state == WALK && !game->round_manager->under_attack) game->scale = 1;
    else game->scale = 0.5;
}

void move_background(Entity* sp) {
    if (bg_up && sp->y-5 >= 0) sp->y -= 5;
    if (bg_down && sp->y+5 <= sp->sprite->height-600) sp->y += 5;
    if (bg_left) sp->x -= 5;
    if (bg_right) sp->x += 5;
    // if(sp->x >= sp->sprite->width) sp->x -= sp->sprite->width;
    // if (sp->x <= 0) sp->x += sp->sprite->width;
    if (multiplayer) {
        if (bg_up && bg_left && !bg_down && !bg_right) {
            sp_write_wq(SUB_HEADER|BG_UP_LEFT);
            sp_write_fifo();
        }
        else if (bg_up && !bg_left && !bg_down && bg_right) {
            sp_write_wq(SUB_HEADER|BG_UP_RIGHT);
            sp_write_fifo();
        }
        else if (!bg_up && bg_left && bg_down && !bg_right) {
            sp_write_wq(SUB_HEADER|BG_DOWN_LEFT);
            sp_write_fifo();
        }
        else if (!bg_up && !bg_left && bg_down && bg_right) {
            sp_write_wq(SUB_HEADER|BG_DOWN_RIGHT);
            sp_write_fifo();
        }
        else if (bg_up && !bg_down) {
            sp_write_wq(SUB_HEADER|BG_UP);
            sp_write_fifo();
        }
        else if (!bg_up && bg_down) {
            sp_write_wq(SUB_HEADER|BG_DOWN);
            sp_write_fifo();
        }
        else if (bg_left && !bg_right) {
            sp_write_wq(SUB_HEADER|BG_LEFT);
            sp_write_fifo();
        }
        else if (!bg_left && bg_right) {
            sp_write_wq(SUB_HEADER|BG_RIGHT);
            sp_write_fifo();
        }
    }
    if (game->background->y > 50) game->submarine->state = SUBMERGED;
    else if (game->submarine->oxygen_counter < game->submarine->oxygen_capacity && game->submarine->state == SUBMERGED) {
        bg_down = false;
        bg_up = false;
        bg_right = false;
        bg_left = false;
        game->submarine->state = SURFACE;
        count = 1;
        game->round_manager->under_attack = true;
        if (game->submarine->players[0]->state == DRIVE) {
            game->submarine->players[0]->state = WALK;
            if (multiplayer) {
                sp_write_wq(PLAYER_HEADER|PLAYER_NONE);
                sp_write_fifo();
            }
        }
        game->scale = 0.5;
    } 
}

void move_player(MainPlayer* sp) {
    if (check_collision(game->submarine, game->submarine->players[0]->asp->e->x + game->submarine->player_speed*((movement[0] == RIGHT) - (movement[0] == LEFT)),
    game->submarine->players[0]->asp->e->y + game->submarine->player_speed*((movement[0] == DOWN) - (movement[0] == UP)), movement[0])) return;
    move_main_player(game->submarine->players[0], movement[0], game->submarine->player_speed);
    if (multiplayer && movement[0] != 0) {
        sp_write_wq(PLAYER_HEADER|movement[0]);
        sp_write_fifo();
    }
    return;
}

void update_scene() {
    move_background(game->background);
    if (game->submarine->players[0]->state == WALK) move_player(game->submarine->players[0]);    
    update_number_of_lifes();
    animate_entity(game->submarine->players[0]->asp);
    for (uint8_t j = 0; j < 5; j++) {
        update_cannon(game, j);
    }
    if (multiplayer && game->submarine->players[0]->state >= INCANNON1 && (game->submarine->cannon[game->submarine->players[0]->state-INCANNON1]->pointX != cursor->x || game->submarine->cannon[game->submarine->players[0]->state-INCANNON1]->pointY != cursor->y)){
        sp_write_wq(CANNON_HEADER|CANNON_ROTATE);
        uint8_t msb = 0;
        uint8_t lsb = 0;
        util_get_MSB(cursor->x, &msb);
        util_get_LSB(cursor->x, &lsb);
        sp_write_wq(msb);
        sp_write_wq(lsb);
        util_get_MSB(cursor->y, &msb);
        util_get_LSB(cursor->y, &lsb);
        sp_write_wq(msb);
        sp_write_wq(lsb);
        sp_write_fifo();
    }
    if (multiplayer) animate_entity(game->submarine->players[1]->asp);
    draw_game(game, left, right, up, down, bg_left, bg_right, bg_down, bg_up, cursor);
    draw_current_time(hours, minutes, seconds, 630, 550);
    vg_page_flip(); 
}

void (game_init)() {
    rtc_subscribe_periodic_interrupts(PERIODIC_16);
    app_state = GAME;
    cursor->sprite = get_sprite(AIM);
    game = create_game(multiplayer, (player_num == -1) ? 0 : player_num);
    clear_movements();
    game->submarine->oxygen_counter = 100;
}

void (playermenu_init)() {
    app_state = PLAYERMENU;
    player_select[0] = create_animated_entity(0, 0, 0, 0, 4, PLAYER_PINK_IDLE_1, PLAYER_PINK_IDLE_2, PLAYER_PINK_IDLE_3, PLAYER_PINK_IDLE_4);
    player_select[1] = create_animated_entity(0, 0, 0, 0, 4, PLAYER_BLUE_IDLE_1, PLAYER_BLUE_IDLE_2, PLAYER_BLUE_IDLE_3, PLAYER_BLUE_IDLE_4);
}

void (menu_init)() {
    app_state = MAINMENU;
    multiplayer = false;
    player_num = -1;
    sp_disable_irq_interrupts();

    cursor->sprite = get_sprite(CURSOR);
}

void (waiting_init)() {
    count = 1;
    app_state = WAITING;
    draw_sprite_full_screen(get_sprite(LOADING));
    vg_page_flip();
    sp_enable_irq_interrupts();
}

void (initialize)() {
    if(load_sprites() != 0) return;
    cursor = create_entity(get_sprite(CURSOR), 300,320,1,1);
    // rtc_init();
}

void(decrement_number_of_lifes)(){
    game->submarine->current_number_of_lifes--;
}

void(increment_number_of_lifes)(){
    game->submarine->current_number_of_lifes++;
}

void(update_number_of_lifes)(){
    //if (scancode == S_KEY) decrement_number_of_lifes();
    //else if ((game->submarine->current_number_of_lifes < game->submarine->max_number_of_lifes) &&(scancode == W_KEY)) increment_number_of_lifes();
    if (game->submarine->current_number_of_lifes == 0){
        app_state = GAMEOVER;
        if(multiplayer){
            sp_write_wq(CONNECT_HEADER|GAME_OVER);
            sp_write_fifo();
        }
    }
}

void (end_game)() {
    destroy_game(game);
    rtc_unsubscribe_periodic_interrupts();
}

void (leaderboard_init)() {
    draw_sprite_full_screen(get_sprite(LEADERBOARD_BG));
    display_leaderboard();
    vg_page_flip();
}

void (clear_movements)() {
    memset(movement, 0, 4 * sizeof(MOVEMENT));
    bg_down = bg_up = bg_right = bg_left = up = down = left = right = false;
}
