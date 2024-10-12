#ifndef _CANNON_H_
#define _CANNON_H_

#include <lcom/lcf.h>
#include "moving_entities/animatedEntity.h"
#include "bullet.h"

/** @file */
/** @defgroup cannon Cannon
 * @brief Cannon handling module.
 * @{
 *
 */

/**
 * @brief Structure representing a cannon.
 */
typedef struct {
  BulletInfo* bulletInfo; /**< Pointer to the bullet information used by the cannon */
  Entity* cannon; /**< Pointer to the cannon entity */
  uint16_t facing; /**< Direction the cannon is facing */
  int pointX, pointY; /**< Coordinates where the cannon is pointing */
  int firingX, firingY; /**< Coordinates from where the cannon fires bullets */
  Bullet* activeBullets[MAX_BULLET_NUM]; /**< Array of active bullets fired by the cannon */
  AnimatedEntity* explosions[MAX_BULLET_NUM]; /**< Array of explosion animations */
  uint32_t lastFired; /**< Timestamp of the last fired bullet */
} Cannon;

/**
 * @brief Creates a cannon with the specified facing direction and bullet type.
 * 
 * @param facing The direction the cannon is facing.
 * @param bullet_type The type of bullet the cannon fires.
 * @return Pointer to the created cannon.
 */
Cannon* create_cannon(uint16_t facing, BULLET_TYPES bullet_type);

/**
 * @brief Creates a default cannon with the specified facing direction.
 * 
 * @param facing The direction the cannon is facing.
 * @return Pointer to the created default cannon.
 */
Cannon* create_default_cannon(uint16_t facing);

/**
 * @brief Fires a bullet from a cannon.
 * 
 * @param cannon Pointer to the cannon to be fired.
 * @param background Pointer to the background entity.
 * @param scale The scale factor for the cannon's position and firing calculations.
 * @param fire_cooldown The cooldown period between successive fires.
 * @param currentTime The current game timestamp to control fire rate.
 * 
 * @return Zero if it can fire, one otherwise.
 */
int fire(Cannon* cannon, Entity* background, float scale, uint32_t fire_cooldown, int currentTime);

/**
 * @brief Draws the cannon on the screen.
 * 
 * @param cannon Pointer to the cannon to be drawn.
 * @param background Pointer to the background entity.
 * @param scale The scale factor for the cannon's position.
 */
void draw_cannon(Cannon* cannon, Entity* background, float scale);

/**
 * @brief Destroys the cannon and frees associated resources.
 * 
 * @param cannon Pointer to the cannon to be destroyed.
 */
void destroy_cannon(Cannon *cannon);

/** @} end of cannon */

#endif /* _CANNON_H_ */
