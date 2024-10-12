#ifndef _SUBMARINE_H_
#define _SUBMARINE_H_

#include <lcom/lcf.h>
#include "moving_entities/entity.h"
#include "cannon.h"
#include "moving_entities/mainPlayer.h"
#include "hook.h"




/** @file */
/** @defgroup submarine Submarine
 * @brief Submarine handling module.
 * @{
 *
 */
#define SUB_CANNON_NUM 5 /**< Number of cannons in Submarine */
#define WALLS 0x000000 /**< Color of walls in Submarine mask */
#define FLOOR 0xff0000 /**< Color of floor in Submarine mask */
#define STAIRS 0x0000ff /**< Color of stais in Submarine mask */
#define CANNON3 0xf000f0 /**< Color of cannon 3 in Submarine mask */
#define CANNON4 0xf00f00 /**< Color of cannon 4 in Submarine mask */
#define CANNON2 0x00ff00 /**< Color of cannon 2 in Submarine mask */
#define CANNON1 0x0ff000 /**< Color of cannon 1 in Submarine mask */
#define CANNON5 0xf0000f /**< Color of cannon 5 in Submarine mask */
#define WHEEL 0xf0f000 /**< Color of wheel in Submarine mask */
/**
 * @brief Enumeration representing the state of the ubmarine.
 */
typedef enum {
    SUBMERGED,  /**< Submarine is submerged underwater. */
    SURFACE     /**< Submarine is on the surface. */
} SUBMARINE_STATE;

/**
 * @brief Structure representing the Submarine entity.
 */
typedef struct {
    Entity* submarine;                      /**< Pointer to the Submarine entity. */
    Sprite* submask;                        /**< Pointer to the Sprite representing the submarine mask. */
    Hook* hook;                             /**< Pointer to the Hook object. */
    float rotation;                         /**< Rotation angle of the submarine. */
    MainPlayer* players[2];                 /**< Array of pointers to players objects. */
    Cannon* cannon[5];                      /**< Array of pointers to Cannon objects. */
    SUBMARINE_STATE state;                  /**< State of the Submarine. */
    int oxygen_capacity;                    /**< Capacity of oxygen of the submarine. */
    int oxygen_counter;                     /**< Counter for tracking remaining oxygen. */
    int max_number_of_lifes;                /**< Maximum number of lives of the Submarine. */
    int current_number_of_lifes;            /**< Current number of lives of the Submarine. */
    uint32_t fire_cooldown;                 /**< Cooldown period for firing cannons. */
    uint8_t player_speed;                   /**< Speed of the Submarine players. */
} Submarine;

/**
 * @brief Creates a new Submarine entity.
 * 
 * @param x Initial x-coordinate of the Submarine.
 * @param y Initial y-coordinate of the Submarine.
 * @return Pointer to the newly created Submarine entity.
 */
Submarine* create_sub(int x, int y);

/**
 * @brief Adds a player to the Submarine.
 * 
 * @param player Pointer to the player entity.
 * @param MainPlayer Flag indicating if the player is the main player.
 * @param sub Pointer to the Submarine.
 * @return true if the player was successfully added, false otherwise.
 */
bool add_player_to_submarine(MainPlayer* player, bool MainPlayer, Submarine* sub);

/**
 * @brief Checks collision for the player movement in Submarine.
 * 
 * @param sub Pointer to the Submarine.
 * @param x X-coordinate to check collision.
 * @param y Y-coordinate to check collision.
 * @param movement Movement direction.
 * @return true if collision is detected, false otherwise.
 */
bool check_collision(Submarine* sub, int x, int y, MOVEMENT movement);

/**
 * @brief Checks if the player can start driving the Submarine.
 * 
 * @param submarine Pointer to the Submarine.
 * @return true if the Submarine can be driven, false otherwise.
 */
bool check_drive(Submarine* submarine);

/**
 * @brief Activates the Cannon the player's in on the submarine.
 * 
 * @param submarine Pointer to the Submarine.
 * @return true if the cannon was successfully activated, false otherwise.
 */
bool activate_cannon(Submarine* submarine);

/**
 * @brief Deactivates the Cannon the player's on the submarine.
 * 
 * @param submarine Pointer to the submarine.
 */
void deactivate_cannon(Submarine* submarine);

/**
 * @brief Draws the Submarine and its components.
 * 
 * @param sub Pointer to the Submarine.
 * @param background Pointer to the background Entity.
 * @param cursor Pointer to the cursor Entity.
 * @param scale Scaling factor.
 * @return 0 on success, non-zero otherwise.
 */
int draw_submarine_and_childs(Submarine* sub, Entity* background, Entity* cursor, float scale);

/**
 * @brief Draws the oxygen bar.
 * 
 * @param sub Pointer to the Submarine.
 * @return 0 on success, non-zero otherwise.
 */
int draw_oxygen_bar(Submarine* sub);

/**
 * @brief Draws the heart icons representing the current lives.
 * 
 * @param sub Pointer to the Submarine.
 * @return 0 on success, non-zero otherwise.
 */
int draw_hearts(Submarine* sub);

/**
 * @brief Updates the oxygen level for the Submarine.
 * 
 * @param sub Pointer to the Submarine.
 */
void update_oxygen(Submarine* sub);

/**
 * @brief Destroy Submarine, clearing its memory.
 * 
 * @param submarine Pointer to the Submarine to destroy.
 */
void destroy_submarine(Submarine* submarine);

#endif /* _SUBMARINE_H_ */
