#ifndef TRASH_H
#define TRASH_H

#include "animatedEntity.h"

/** @file */
/** @defgroup trash Trash
 * @brief Trash handling module.
 * @{
 *
 */

/**
 * @brief Enumeration representing states of the Trash entity.
 */
typedef enum {
    IN_BUBBLE,          /**< @brief Trash is inside the bubble. */
    POPPING_BUBBLE,     /**< @brief Bubble of Trash popping. */
    OUT_BUBBLE          /**< @brief Trash is outside the bubble. */
} TRASH_STATE;

/**
 * @brief Structure representing a Trash entity.
 */
typedef struct {
    TRASH_STATE state;          /**< @brief State of the Trash entity. */
    AnimatedEntity* trash;      /**< @brief Pointer to the associated animated entity. */
    uint8_t money;              /**< @brief Amount of money associated with the trash. */
} Trash;

/**
 * @brief Creates a new Trash entity.
 * 
 * @param money Amount of money associated with the Trash.
 * @param x Initial x-coordinate of the Trash.
 * @param y Initial y-coordinate of the Trash.
 * @param xspeed Initial speed in the x-direction.
 * @param yspeed Initial speed in the y-direction.
 * @return Pointer to the newly created Trash entity.
 */
Trash* create_trash(uint8_t money, int x, int y, int xspeed, int yspeed);

/**
 * @brief Pops the bubble of a Trash entity.
 * 
 * @param trash Pointer to the Trash entity.
 */
void trash_pop_bubble(Trash* trash);

/**
 * @brief Destroys a Trash entity.
 * 
 * @param trash Pointer to the Trash entity to destroy.
 */
void destroy_trash(Trash* trash);

/**
 * @brief Draws a Trash entity on a background.
 * 
 * @param trash Pointer to the trash Entity to draw.
 * @param background Pointer to the background Entity.
 * @param scale Scaling factor for the trash Entity.
 * @param backgroundRot The rotation of the background.
 * @param on_black Flag indicating whether the background is black.~
 */
void draw_trash(Trash* trash, Entity* background, float scale, float backgroundRot, bool on_black);

#endif
