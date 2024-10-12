#ifndef _GAME_H_
#define _GAME_H_

#include "submarine.h"
#include "graphics/sprite.h"
#include "cannon.h"
#include "moving_entities/enemy.h"
#include "moving_entities/fish.h"
#include "round_manager.h"
#include "bullet.h"
#include "moving_entities/trash.h"
#include "shop.h"

/** @file */
/** @defgroup game Game
 * @brief Game handling module.
 * @{
 *
 */

#define MAX_ENEMY_NUM 100 /**< Max number of Enemy. */
#define MAX_FISH_NUM 30 /**< Max number of Fish. */
#define MAX_TRASH_NUM 5 /**< Max number of Trash */

/**
 * @brief Structure representing the Game environment.
 */
typedef struct {
    RoundManager* round_manager;   /**< Pointer to the RoundManager object. */
    Submarine* submarine;           /**< Pointer to the Submarine object. */
    Entity* background;             /**< Pointer to the background Entity. */
    int current_money;              /**< Current amount of money in the game. */
    float scale;                    /**< Scaling factor for the game environment. */

    Fish* fishes[MAX_FISH_NUM];     /**< Array of pointers to Fish objects. */
    int next_spawn_fishes[MAX_FISH_NUM];     /**< Array of next spawn tick for Fish. */
    Trash* trash[MAX_TRASH_NUM];    /**< Array of pointers to Trash objects. */
    int next_spawn_trashes[MAX_FISH_NUM];     /**< Array of next spawn tick for Trash. */
    uint8_t fish_spawn_period;      /**< Period for Fish spawning.*/
    uint8_t trash_spawn_period;     /**< Period for Trash spawning. */
    uint16_t seed;                  /**< Seed value for random behavior. */
    Shop* shop;                     /**< Pointer to the Shop object. */
} Game;

#include "moving_entities/mainPlayer.h"

/**
 * @brief Creates a new Game environment.
 * 
 * @param multiplayer Flag indicating whether the Game is multiplayer.
 * @param player_num Player number.
 * @return Pointer to the newly created game environment.
 */
Game* create_game(bool multiplayer, bool player_num);

/**
 * @brief Destroys a Game, clearing its memory.
 * 
 * @param game Pointer to the Game to destroy.
 */
void destroy_game(Game* game);

/**
 * @brief Draws the Game environment.
 * 
 * @param game Pointer to the Game environment.
 * @param left Player movement to the left.
 * @param right Player movement to the right.
 * @param up Player movement upwards.
 * @param down Player movement downwards.
 * @param bg_left Background movement to the left.
 * @param bg_right Background movement to the right.
 * @param bg_down Background movement downwards.
 * @param bg_up Background movement upwards.
 * @param cursor Pointer to the cursor Entity.
 */
void draw_game(Game* game, int left, int right, int up, int down, int bg_left, int bg_right, int bg_down, int bg_up, Entity* cursor);

/**
 * @brief Updates the Cannon in the game environment.
 * 
 * @param game Pointer to the Game environment.
 * @param cannon_index Index of the Cannon to update.
 */
void update_cannon(Game* game, uint8_t cannon_index);

/**
 * @brief Draws all fishes in the game environment.
 * 
 * @param game Pointer to the Game environment.
 * @param on_black Flag indicating whether to draw on black background.
 */
void draw_fishes(Game* game, bool on_black);

/**
 * @brief Spawns a new Trash object in the Game environment.
 * 
 * @param game Pointer to the Game environment.
 * @param counter Current counter for spawn.
 */
void spawn_trash(Game* game, int counter);

/**
 * @brief Spawns a new Fish object in the Game environment.
 * 
 * @param game Pointer to the Game environment.
 * @param counter Current counter for spawn.
 */
void spawn_fish(Game* game, int counter);

/**
 * @brief Spawns multiple Fish objects in the Game environment.
 * 
 * @param game Pointer to the Game environment.
 * @param count Number of Fish objects to spawn.
 * @param counter Current counter for spawn.
 */
void spawn_fishes(Game* game, int count, int counter);

/**
 * @brief Moves all Fish objects in the Game environment.
 * 
 * @param game Pointer to the Game environment.
 */
void move_fishes(Game* game);

/**
 * @brief Moves all Trash objects in the Game environment.
 * 
 * @param game Pointer to the Game environment.
 */
void move_trash(Game* game);
void draw_trashes(Game* game, bool on_black);
void draw_fishes(Game* game, bool on_black);

/**
 * @brief Checks if an Entity is out of bounds.
 * 
 * @param background Pointer to the background Entity.
 * @param entity Pointer to the Entity to check.
 * @param padding Padding around the boundary.
 * @return true if the Entity is out of bounds, false otherwise.
 */
bool is_out_of_bounds(Entity* background, Entity* entity, uint16_t padding);

/**
 * @brief Checks if the Submarine Hook has caught any trash objects.
 * 
 * @param game Pointer to the Game environment.
 * @param caught_trash Pointer to the variable to store the index of the caught Trash object.
 * @return true if any Trash object as caught, false otherwise.
 */
bool check_catch_trash(Game* game, uint8_t* caught_trash);

/**
 * @brief Draws all Trash objects in the game environment.
 * 
 * @param game Pointer to the Game environment.
 * @param on_black Flag indicating whether to draw on black background.
 */
void draw_trashes(Game* game, bool on_black);

/**
 * @brief Starts a new round in the Game environment.
 * 
 * @param game Pointer to the Game environment.
 */
void start_round(Game* game);

/**
 * @brief Updates the buyable items in the Shop.
 * 
 * @param game Pointer to the Game environment.
 */
void update_buyable_items(Game* game);

#endif
