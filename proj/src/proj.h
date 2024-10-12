#ifndef PROJ
#define PROJ

#include "graphics/sprite.h"
#include "game/submarine.h"

/**
 * @mainpage 
 * This project consists of a game inspired by “Lovers in a dangerous spacetime”, but with many changes. 
 * The game takes place in an ocean, where a submarine sails. Players can move around inside the submarine and reach different places where they can perform different actions, such as driving the submarine and firing its cannons. 
However, the submarine's oxygen tank is limited and, therefore, it is necessary to come to the surface to renew it. On the surface, there are enemies that players have to defend themselves against.
Players can collect the trash they find in bubbles in the ocean, earning coins, to improve their conditions, namely recovering lost lives, improving the oxygen tank, increasing the firing rate of the cannons and increasing the player's speed. 
The deeper they collect the trash, the more money they make. However, if, when trying to burst the bubbles, they kill any fish, they will lose coins.
*/

/** @file */
/** @defgroup proj Proj
 * @brief Proj handling module.
 * @{
 *
 */

#define NUM_STATES 8 /**< @brief Number of states in Proj */
#define NUM_DEVICES 5 /**< @brief Number of devices in Proj */


/**
 * @brief Enumeration for the devices.
 */
typedef enum {
    TIMER, /**< @brief Timer device. */
    KEYBOARD, /**< @brief Keyboard device. */
    MOUSE, /**< @brief Mouse device. */
    SP, /**< @brief SP device. */
    RTC /**< @brief RTC device. */
} DEVICES;

/**
 * @brief Enumeration for the application states.
 */
typedef enum {
  MAINMENU, /**< @brief Main menu state. */
  GAME, /**< @brief Game state. */
  PLAYERMENU, /**< @brief Player menu state. */
  WAITING, /**< @brief Waiting state. */
  PAUSE, /**< @brief Pause state. */
  GAMEOVER, /**< @brief Game over state. */
  LEADERBOARD, /**< @brief Leaderboard state. */
  SHOP
} APP_STATE;

/**
 * @brief Initializes the game.
 */
void (game_init)();

/**
 * @brief Initializes the player menu.
 */
void (playermenu_init)();

/**
 * @brief Initializes the menu.
 */
void (menu_init)();

/**
 * @brief Handles timer interrupts for the menu.
 */
void (menu_timer_ih)();

/**
 * @brief Handles game timer interrupts.
 */
void (game_timer_ih)();

/**
 * @brief Handles mouse interrupts for the menu.
 */
void (menu_mouse_ih)();

/**
 * @brief Handles mouse interrupts for the game.
 */
void (game_mouse_ih)();

/**
 * @brief Handles SP interrupts for the menu.
 */
void (menu_sp_ih)();

/**
 * @brief Handles keyboard interrupts for the game.
 */
void (game_kbd_ih)();

/**
 * @brief Handles SP interrupts for the game.
 */
void (game_sp_ih)();

/**
 * @brief Handles mouse interrupts for the player menu.
 */
void (playermenu_mouse_ih)();

/**
 * @brief Handles timer interrupts for the player menu.
 */
void (playermenu_timer_ih)();

/**
 * @brief Updates the movement of the game entities.
 */
void update_move();

/**
 * @brief Updates the movement of the background.
 */
void update_move_background();

/**
 * @brief Updates the state of the player.
 */
void update_player_state();

/**
 * @brief Moves the background entity.
 * @param sp Pointer to the background entity.
 */
void move_background(Entity* sp);

/**
 * @brief Moves the player entity.
 * @param sp Pointer to the player entity.
 */
void move_player(MainPlayer* sp);

/**
 * @brief Updates the game scene.
 */
void update_scene();

/**
 * @brief Initializes the player menu.
 */
void (playermenu_init)();

/**
 * @brief Initializes the game.
 */
void (game_init)();

/**
 * @brief Empties the keyboard buffer.
 */
void (kb_empty)();

/**
 * @brief Handles timer interrupts for the waiting state.
 */
void (waiting_timer_ih)();

/**
 * @brief Initializes the waiting state.
 */
void  (waiting_init)();

/**
 * @brief Initializes the game.
 */
void (initialize)();

/**
 * @brief Updates the number of lives.
 */
void(update_number_of_lifes)();

/**
 * @brief Ends the game.
 */
void (end_game)();

/**
 * @brief Handles RTC interrupts for the game.
 */
void (game_rtc_ih)();

/**
 * @brief Handles timer interrupts for the pause state.
 */
void (pause_timer_ih)();

/**
 * @brief Handles mouse interrupts for the pause state.
 */
void (pause_mouse_ih)();

/**
 * @brief Handles keyboard interrupts for the pause state.
 */
void (pause_kb)();

/**
 * @brief Handles keyboard interrupts for the shop state.
 */
void (shop_kb)();

/**
 * @brief Handles timer interrupts for the game over state.
 */
void (gameover_timer_ih)();

/**
 * @brief Handles keyboard interrupts for the game over state.
 */
void (gameover_kb)();


/**
 * @brief Handles empty interrupts.
 */
void (empty_ih)();

/**
 * @brief Initializes the leaderboard.
 */
void (leaderboard_init)();

/**
 * @brief Clears the movements.
 */
void (clear_movements)();

/**
 * @brief Handles timer interrupts for the shop state.
 */
void (shop_timer_ih)();

/**
 * @brief Handles mouse interrupts for the shop state.
 */
void (shop_mouse_ih)();

/**
 * @brief Handles RTC interrupts for the waiting state.
 */
void (waiting_rtc_ih)();

/**
 * @brief Handles SP interrupts for the waiting state.
 */
void (waiting_sp_ih)();

/**
 * @brief Handles RTC interrupts for the pause state.
 */
void (pause_rtc_ih)();

#endif
