#ifndef _SHOP_H_
#define _SHOP_H_
#include <lcom/lcf.h>

#include "graphics/sprite.h"

/** @file */
/** @defgroup shop Shop
 * @brief Shop handling module.
 * @{
 *
 */

#define MAX_ITEMS_SIZE 4 /**< Max number of items in Shop. */
#define YELLOW 0xFDE101 /**< Hex code for yellow color. */
#define RED 0xFF0000 /**< Hex code for red color. */

/**
 * @brief Enumeration representing different Shop items.
 */
typedef enum {
    LIFES,      /**< Item representing additional lives. */
    OXYGEN,     /**< Item representing additional oxygen supply. */
    FIRE_RATE,  /**< Item representing increased fire rate. */
    SPEED       /**< Item representing increased speed. */
} ITEMS;

/**
 * @brief Structure representing an Item in the Shop.
 */
typedef struct {
    Sprite* sprite;                         /**< Pointer to the sprite representing the Item. */
    uint16_t price;                         /**< Price of the Item. */
    uint16_t x, y;                          /**< Coordinates of the Item on the screen. */
    void (*on_click_effect)(void*);         /**< Function pointer to the function to execute when clicked. */
    bool buyable;                           /**< Flag indicating if the Item is buyable. */
} Item;

/**
 * @brief Structure representing the Shop.
 */
typedef struct {
    Item* items[MAX_ITEMS_SIZE];            /**< Array of pointers to Shop items. */
} Shop;

/**
 * @brief Creates a new Shop.
 * 
 * @return Pointer to the newly created Shop.
 */
Shop* create_shop();

/**
 * @brief Adds an Item to the Shop.
 * 
 * @param shop Pointer to the Shop.
 * @param x_ X-coordinate of the Item.
 * @param y_ Y-coordinate of the Item.
 * @param price Price of the Item.
 * @param sprite Pointer to the sprite representing the Item.
 * @param on_click Function pointer to the function to execute when clicked.
 * @param position Index of the Item in the Shop.
 */
void add_item(Shop* shop, uint16_t x_, uint16_t y_, uint16_t price, Sprite* sprite, void (*on_click)(void*), uint8_t position);

/**
 * @brief Retrieves the clicked Item in the Shop.
 * 
 * @param shop Pointer to the Shop.
 * @param x X-coordinate of the click.
 * @param y Y-coordinate of the click.
 * @param selected_item Pointer to store the index of the selected item.
 * @return Pointer to the clicked item.
 */
Item* get_clicked_item(Shop* shop, uint16_t x, uint16_t y, ITEMS* selected_item);

/**
 * @brief Draws the Shop on the screen.
 * 
 * @param shop Pointer to the Shop.
 * @param budget Amount of money available for purchasing.
 */
void draw_shop(Shop* shop, uint16_t budget);
void destroy_shop(Shop* shop);
void destroy_item(Item* item);

#endif /* _SHOP_H_ */
