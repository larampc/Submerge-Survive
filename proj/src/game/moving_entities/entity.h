#ifndef _ENTITY_H_
#define _ENTITY_H_
#include <lcom/lcf.h>

#include "../../graphics/sprite.h"

/** @file */
/** @defgroup entity Entity
 *  @brief Entity handling module.
 * @{
 *
 */

/**
 * @brief Structure representing an Entity.
 */
typedef struct {
  int x, y;               /**< @brief Current sprite position. */
  int xspeed, yspeed;     /**< @brief Current speeds in the x and y direction. */
  Sprite* sprite;         /**< @brief Pointer to the entity sprite. */
  bool facing_left;       /**< @brief Flag indicating whether the entity is facing left. */
} Entity;

/**
 * @brief Creates a new Entity.
 * 
 * @param sp Pointer to the sprite of the entity.
 * @param x Initial x-coordinate of the entity.
 * @param y Initial y-coordinate of the entity.
 * @param xspeed Initial speed in the x-direction.
 * @param yspeed Initial speed in the y-direction.
 * @return Pointer to the newly created Entity.
 */
Entity* create_entity(Sprite* sp, int x, int y, int xspeed, int yspeed);

/**
 * @brief Moves an Entity according to its current speeds.
 * 
 * @param entity Pointer to the Entity to move.
 */
void move_entity(Entity* entity);

/**
 * @brief Destroys an Entity.
 * 
 * @param fig Pointer to the Entity to destroy.
 */
void destroy_entity(Entity* fig);

/**
 * @brief Draws an Entity on it's position on the background, having in count it's rotation (given by the x-speed and y-speed) with a specified scale.
 * 
 * @param e Pointer to the Entity to draw.
 * @param background Pointer to the background Entity.
 * @param scale Scaling factor for the entity.
 * @return 0 on success, non-zero on failure.
 */
int draw_entity_on_background(Entity* e, Entity* background, float scale);

/**
 * @brief Draws an Entity on it's position on the background except pixels already drawn, having in count it's rotation (given by the x-speed and y-speed) with a specified scale.
 * 
 * @param e Pointer to the Entity to draw.
 * @param background Pointer to the background Entity.
 * @param backgroundRot The rotation of the background.
 * @param scale Scaling factor for the entity.
 * @return 0 on success, non-zero on failure.
 */
int draw_entity_on_black_background(Entity* e, Entity* background, float backgroundRot, float scale);

/**
 * @brief Draws an Entity with position relative to screen.
 * 
 * @param e Pointer to the Entity to draw.
 * @return 0 on success, non-zero on failure.
 */
int draw_entity_on_screen(Entity* e);

#endif
