#ifndef _MAIN_PLAYER_H_
#define _MAIN_PLAYER_H_

#include <lcom/lcf.h>
#include "animatedEntity.h"

/** @file */
/** @defgroup mainPlayer MainPlayer
 * @brief Main player handling module.
 * @{
 *
 */

#define STATES_NUM 4 /**< @brief Number of player states. */
#define MAX_WIDTH 35 /**< @brief Max width of the player sprites */

/**
 * @brief Enumeration representing movement states of the MainPlayer.
 */
typedef enum {
    STOPPED_HORIZONTAL, /**< @brief Player is stopped horizontally. */
    WALKING,            /**< @brief Player is walking. */
    STOPPED_VERTICAL,   /**< @brief Player is stopped climbing. */
    CLIMBING            /**< @brief Player is climbing. */
} MOVEMENT_STATE;

/**
 * @brief Enumeration representing movement directions.
 */
typedef enum {
    STOP,   /**< @brief Stop movement. */
    UP,     /**< @brief Move upwards. */
    DOWN,   /**< @brief Move downwards. */
    LEFT,   /**< @brief Move leftwards. */
    RIGHT   /**< @brief Move rightwards. */
} MOVEMENT;

/**
 * @brief Enumeration representing states of the MainPlayer.
 */
typedef enum {
    WALK,       /**< @brief Player is walking. */
    DRIVE,      /**< @brief Player is driving. */
    INCANNON1,  /**< @brief Player is controlling cannon 1. */
    INCANNON2,  /**< @brief Player is controlling cannon 2. */
    INCANNON3,  /**< @brief Player is controlling cannon 3. */
    INCANNON4,  /**< @brief Player is controlling cannon 4. */
    INCANNON5   /**< @brief Player is controlling cannon 5. */
} PLAYER_STATE;

/**
 * @brief Structure representing the MainPlayer entity.
 */
typedef struct MainPlayer {
    AnimatedEntity* asp;            /**< @brief Pointer to the associated animated entity. */
    PLAYER_STATE state;             /**< @brief State of the MainPlayer. */
    MOVEMENT_STATE move_state;      /**< @brief Movement state of the MainPlayer. */
    uint8_t no_pics_per_state[STATES_NUM];    /**< @brief Number of pictures per state. */
    uint8_t start_pics_per_state[STATES_NUM]; /**< @brief Starting picture index per state. */
} MainPlayer;

/**
 * @brief Creates a new MainPlayer entity.
 * 
 * @param x Initial x-coordinate of the MainPlayer.
 * @param y Initial y-coordinate of the MainPlayer.
 * @param player_num Specifies the player number.
 * @return Pointer to the newly created MainPlayer entity.
 */
MainPlayer* create_main_player(int x, int y, bool player_num);

/**
 * @brief Destroys a MainPlayer entity.
 * 
 * @param fig Pointer to the MainPlayer entity to destroy.
 */
void destroy_main_player(MainPlayer* fig);

/**
 * @brief Moves the MainPlayer entity according to the specified movement and speed.
 * 
 * @param sp Pointer to the MainPlayer entity.
 * @param movement The movement direction.
 * @param player_speed The speed of the MainPlayer entity.
 */
void move_main_player(MainPlayer* sp, MOVEMENT movement, uint8_t player_speed);

/**
 * @brief Transitions the main player entity to the specified movement state:
 * 
 * @param p Pointer to the MainPlayer entity.
 * @param state The movement state to transition to.
 */
void transition_to_state(MainPlayer* p, MOVEMENT_STATE state);

#endif
