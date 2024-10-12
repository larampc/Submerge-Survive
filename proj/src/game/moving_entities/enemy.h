#ifndef ENEMY_H
#define ENEMY_H

#include "animatedEntity.h"

/** @file */
/** @defgroup enemy Enemy
 * @brief Enemy handling module.
 * @{
 *
 */

/**
 * @brief Structure representing the Enemy.
 */
typedef struct {
  AnimatedEntity* asp;  /**< @brief Pointer to the associated animated entity. */
  bool active;          /**< @brief Status indicating if the enemy is active. */
  uint16_t seed;        /**< @brief Seed value for random behavior of enemy. */
} Enemy;

/**
 * @brief Creates a new Enemy.
 * 
 * @param active Initial status of the enemy.
 * @param seed Seed value for the enemy's random behavior.
 * @return Pointer to the newly created Enemy.
 */
Enemy* create_enemy(bool active, uint16_t seed);

/**
 * @brief Destroys an Enemy.
 * 
 * @param enemy Enemy to destroy.
 */
void destroy_enemy(Enemy* enemy);

/**
 * @brief Handles the death of an enemy.
 * 
 * @param enemy Pointer to the Enemy to be deactivated.
 */
void die(Enemy* Enemy);

#endif
