#ifndef _ROUND_MANAGER_
#define _ROUND_MANAGER_

#include <lcom/lcf.h>
#include "moving_entities/enemy.h"
#include "bullet.h"
#include "submarine.h"

/** @file */
/** @defgroup roundManager RoundManager
 * @brief Round management module.
 * @{
 */

/** @brief Maximum number of enemies in a round */
#define MAX_ENEMY_NUM 100

/**
 * @brief Structure representing the round manager.
 */
typedef struct {
  Enemy* enemies[MAX_ENEMY_NUM]; /**< Array of pointers to enemies */
  Bullet* enemy_bullets[MAX_BULLET_NUM]; /**< Array of pointers to enemy bullets */
  AnimatedEntity* explosions[MAX_BULLET_NUM]; /**< Array of pointers to explosion animations */
  BulletInfo* bulletInfo; /**< Pointer to the bullet graphical information used by enemies */
  uint8_t appearence_rate; /**< Rate of enemy appearance */
  uint8_t round_enemies; /**< Total number of enemies in the round */
  uint8_t active_enemies; /**< Number of active enemies in the round */
  uint8_t created_enemies; /**< Number of created enemies in the round */
  bool under_attack; /**< Flag indicating if the round is ongoing */
} RoundManager;

/**
 * @brief Creates a round manager.
 * 
 * @return Pointer to the created round manager.
 */
RoundManager* create_round_manager();

/**
 * @brief Destroys the roundManager, clearing its memory.
 * 
 * @param r Pointer to the round manager.
 */
void destroy_round_manager(RoundManager* r);

/**
 * @brief Updates the state of the round.
 * 
 * @param r Pointer to the round manager.
 */
void update_round(RoundManager* r);

/**
 * @brief Updates the state of the enemies in the round.
 * 
 * @param r Pointer to the round manager.
 */
void update_enemies(RoundManager* r);

/**
 * @brief Checks if a bullet collided with enemies.
 * 
 * @param bullet_info Pointer to the bullet information.
 * @param bullet Pointer to the bullet.
 * @param r Pointer to the round manager.
 * @param background Pointer to the background entity.
 * @param cannon_index Index of the Cannon the bullet is from.
 * @param bullet_index Index of the Bullet to check collision with.
 * @return true if a collision is detected, false otherwise.
 */
bool check_bullet_collisions_with_enemies(BulletInfo* bullet_info, Bullet* bullet, RoundManager* r, Entity* background, uint8_t cannon_index, uint8_t bullet_index);

/**
 * @brief Moves the enemies.
 * 
 * @param r Pointer to the round manager.
 * @param submarine Pointer to the submarine entity.
 */
void move_enemies(RoundManager* r, Submarine* submarine);

/**
 * @brief Draws the enemies.
 * 
 * @param r Pointer to the round manager.
 */
void draw_enemies(RoundManager* r);

/**
 * @brief Fires a bullet from an enemy.
 * 
 * @param r Pointer to the round manager.
 * @param enemy Pointer to the enemy that fired.
 */
void fire_enemy(RoundManager* r, Enemy* enemy);

/**
 * @brief Finishes the current round.
 * 
 * @param r Pointer to the round manager.
 */
void finish_round(RoundManager* r);

/**
 * @brief Resets the round manager to its initial state.
 * 
 * @param r Pointer to the round manager.
 */
void reset(RoundManager* r);

/**
 * @brief Moves the active enemy bullets checking if they hit the submarine.
 * 
 * @param r Pointer to the round manager.
 * @param submarine Pointer to the submarine entity.
 */
void move_bullets(RoundManager* r, Submarine* submarine);

/**
 * @brief Draws the bullets fired by enemies.
 * 
 * @param r Pointer to the round manager.
 */
void draw_bullets(RoundManager* r);

/** @} end of round_manager */

#endif /* _ROUND_MANAGER_ */
