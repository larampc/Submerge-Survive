#ifndef _BULLET_H_
#define _BULLET_H_

#include <lcom/lcf.h>
#include "moving_entities/animatedEntity.h"

/** @file */
/** @defgroup bullet Bullet
 * @brief Bullet handling module.
 * @{
 *
 */

/** @brief Maximum number of bullets shot at once by the player */
#define MAX_BULLET_NUM 20 

/** 
 * @brief Types of bullets.
 */
typedef enum {
  NORMAL /**< Normal bullet type */
} BULLET_TYPES;

/**
 * @brief Structure containing graphical information for bullets.
 */
typedef struct {
  AnimatedEntity* bulletExplosion; /**< Pointer to the bullet explosion animation */
  Sprite* bulletSprite; /**< Pointer to the bullet sprite */
} BulletInfo;

/**
 * @brief Structure representing a bullet with position and speed.
 */
typedef struct {
  float x, y; /**< X and Y position of the bullet */
  float xspeed, yspeed; /**< X and Y speed of the bullet */
} Bullet;

/**
 * @brief Gets graphical information of a given bullet type.
 * 
 * @param type The type of bullet.
 * @return Pointer to the bullet's graphical information struct.
 */
BulletInfo* get_bullet_info(BULLET_TYPES type);

/**
 * @brief Moves the bullet based on its speed.
 * 
 * @param bullet Pointer to the bullet to be moved.
 */
void move_bullet(Bullet* bullet);

/**
 * @brief Checks if a bullet has collided with an entity.
 * 
 * @param bulletInfo Pointer to the bullet's graphical information.
 * @param bullet Pointer to the bullet to be checked.
 * @param entity Pointer to the entity.
 * @param collision_x Pointer to store the X coordinate of the collision point.
 * @param collision_y Pointer to store the Y coordinate of the collision point.
 * @return true if there is a collision, false otherwise.
 */
bool check_bullet_collision(BulletInfo* bulletInfo, Bullet* bullet, Entity* entity, int* collision_x, int* collision_y);

/**
 * @brief Handles the explosion of a bullet.
 * 
 * @param explosions Array of pointers to animated entities for explosions.
 * @param bullet Pointer to the bullet that exploded.
 * @param bulletInfo Pointer to the bullet's graphical information.
 */
void expload_bullet(AnimatedEntity* explosions[], Bullet* bullet, BulletInfo* bulletInfo);

/**
 * @brief Checks if a bullet fired by a cannon has collided with a given entity.
 * 
 * @param bulletInfo Pointer to the bullet's graphical information.
 * @param bullet Pointer to the bullet to be checked.
 * @param e Pointer to the entity (cannon or background) to check collision with.
 * @param background Pointer to the background entity.
 * @return true if there is a collision, false otherwise.
 */
bool check_cannon_bullet_collision(BulletInfo* bulletInfo, Bullet* bullet, Entity* e, Entity* background);

/** @} end of bullet */

#endif /* _BULLET_H_ */
