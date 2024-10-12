#ifndef _ANIMATED_ENTITY_H_
#define _ANIMATED_ENTITY_H_

#include <stdarg.h>
#include "../../graphics/sprite.h"
#include "entity.h"
#include "../../graphics/spriteCache.h"

/** @file */
/** @defgroup animatedEntity AnimatedEntity
 * @brief Animated entity handling module.
 * @{
 *
 */

/**
 * @brief Structure representing the Animated entity.
 */
typedef struct {
	Entity* e;           /**< @brief Pointer to the base entity. */
	int cur_aspeed;      /**< @brief Animation speed. */
	int frame_number;    /**< @brief Number of frames after last change */
	int num_fig;         /**< @brief Total number of sprites. */
	int cur_fig;         /**< @brief Current sprite. */
	int start_fig;       /**< @brief Starting sprite. */
	Sprite** sprites;    /**< @brief Array of pointers to sprites. */
} AnimatedEntity;


/**
 * @brief Clones an existing AnimatedEntity.
 * 
 * @param original Pointer to the AnimatedEntity to clone.
 * @return Pointer to the newly created AnimatedEntity.
 */
AnimatedEntity* clone(AnimatedEntity* original);

/**
 * @brief Creates a new AnimatedEntity.
 * 
 * @param x Initial x-coordinate.
 * @param y Initial y-coordinate.
 * @param xspeed Initial speed in the x-direction.
 * @param yspeed Initial speed in the y-direction.
 * @param no_pic Number of pictures.
 * @param pic1 First picture ID.
 * @param ... Additional picture IDs.
 * @return Pointer to the newly created AnimatedEntity.
 */
AnimatedEntity * create_animated_entity(int x, int y, int xspeed, int yspeed,uint8_t no_pic, GAME_XPM_ID pic1, ...);

/**
 * @brief Animates an AnimatedEntity.
 * 
 * @param asp Pointer to the AnimatedEntity to animate.
 */
void animate_entity(AnimatedEntity* asp);

/**
 * @brief Destroys an AnimatedEntity.
 * 
 * @param asp Pointer to the AnimatedEntity to destroy.
 */
void destroy_animated_entity(AnimatedEntity *asp);

#endif
