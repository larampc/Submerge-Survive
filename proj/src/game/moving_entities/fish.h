#ifndef FISH_H
#define FISH_H

#include "animatedEntity.h"

/** @file */
/** @defgroup fish Fish
 * @brief Fish handling module.
 * @{
 *
 */

/**
 * @brief Structure representing a fish entity.
 */
typedef struct {
    AnimatedEntity* asp;  /**< @brief Pointer to the associated animated entity. */
    bool facing_left;     /**< @brief Flag indicating whether the fish is facing left or right. */
    uint8_t money;        /**< @brief Amount of money associated with the fish. */
    uint16_t seed;        /**< @brief Seed value for random behavior or attributes. */
} Fish;

/**
 * @brief Creates a new Fish entity.
 * 
 * @param money Amount of money associated with the fish.
 * @param x Initial x-coordinate of the fish.
 * @param y Initial y-coordinate of the fish.
 * @param xspeed Initial speed in the x-direction.
 * @param yspeed Initial speed in the y-direction.
 * @param seed Seed value for the fish's random behavior.
 * @return Pointer to the newly created Fish.
 */
Fish* create_fish(uint8_t money, int x, int y, int xspeed, int yspeed, uint16_t seed);

/**
 * @brief Destroys a Fish.
 * 
 * @param fish Pointer to the Fish to destroy.
 */
void destroy_fish(Fish* fish);

/**
 * @brief Changes the direction of movement for a Fish.
 * 
 * @param fish Pointer to the Fish to change direction.
 */
void change_fish_direction(Fish* fish);

#endif
