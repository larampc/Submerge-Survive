#ifndef _SPRITE_H_
#define _SPRITE_H_
#include <lcom/lcf.h>
/** @file */
/** @defgroup sprite Sprite
 *
 *
 * @brief Sprite handling module.
 * @{
 */

/**
 * @brief Structure representing a Sprite.
 */
typedef struct {
  uint16_t width; /**< @brief Sprite dimensions */
  uint16_t height; /**< @brief Sprite dimensions */
  unsigned char *map; /**< @brief The sprite pixmap */
} Sprite;

/**
 * @brief Creates a new Sprite.
 *
 * @param pic The picture to be used for the sprite.
 * @return Pointer to the newly created Sprite.
 */
Sprite * create_sprite(xpm_map_t pic);

/**
 * @brief Destroys a Sprite.
 *
 * @param fig Pointer to the Sprite to destroy.
 */
void destroy_sprite(Sprite *fig);

/**
 * @brief Draws a sprite with transformations.
 *
 * @param sp Pointer to the Sprite to draw.
 * @param x The x-coordinate where the sprite will be drawn.
 * @param y The y-coordinate where the sprite will be drawn.
 * @param scale The scale of the sprite.
 * @param rotation The rotation of the sprite.
 * @param pivotX The x-coordinate of the pivot point for rotation.
 * @param pivotY The y-coordinate of the pivot point for rotation.
 * @param mirrored Whether the sprite should be mirrored.
 * @return Zero if successful, non-zero otherwise.
 */
int draw_sprite_t(Sprite *sp, uint16_t x, uint16_t y,float scale, float rotation, int pivotX, int pivotY, bool mirrored);

/**
 * @brief Draws a sprite with transformations on a black background.
 *
 * @param sp Pointer to the Sprite to draw.
 * @param x The x-coordinate where the sprite will be drawn.
 * @param y The y-coordinate where the sprite will be drawn.
 * @param scale The scale of the sprite.
 * @param rotation The rotation of the sprite.
 * @param pivotX The x-coordinate of the pivot point for rotation.
 * @param pivotY The y-coordinate of the pivot point for rotation.
 * @param mirrored Whether the sprite should be mirrored.
 * @return Zero if successful, non-zero otherwise.
 */
int draw_sprite_black_t(Sprite *sp, uint16_t x, uint16_t y,float scale, float rotation, int pivotX, int pivotY, bool mirrored);

/**
 * @brief Draws a sprite with transformations on a background.
 *
 * @param sp Pointer to the Sprite to draw.
 * @param x The x-coordinate where the sprite will be drawn.
 * @param y The y-coordinate where the sprite will be drawn.
 * @param scale The scale of the sprite.
 * @param rotation The rotation of the sprite.
 * @return Zero if successful, non-zero otherwise.
 */
int (draw_sprite_background_t)(Sprite *sp, int x_, int y_, float scale, float rotation);

/**
 * @brief Draws a sprite with transformations for a cannon.
 *
 * @param sp Pointer to the Sprite to draw.
 * @param x The x-coordinate where the sprite will be drawn.
 * @param y The y-coordinate where the sprite will be drawn.
 * @param scale The scale of the sprite.
 * @param rotation The rotation of the sprite.
 * @param pivotX The x-coordinate of the pivot point for rotation.
 * @param pivotY The y-coordinate of the pivot point for rotation.
 * @param cursorX The x-coordinate of the cursor.
 * @param cursorY The y-coordinate of the cursor.
 * @param facing The direction the cannon is facing.
 * @return Zero if successful, non-zero otherwise.
 */
int (draw_sprite_cannon_t)(Sprite *sp, uint16_t x_, uint16_t y_, float scale, float rotation, int pivotX, int pivotY, int cursorX, int cursorY, uint16_t facing);

/**
 * @brief Draws a sprite at the bottom of the screen.
 *
 * @param sp Pointer to the Sprite to draw.
 * @param x The x-coordinate where the sprite will be drawn.
 * @param y The y-coordinate where the sprite will be drawn.
 * @param scale The scale of the sprite.
 * @param rotation The rotation of the sprite.
 * @param pivotX The x-coordinate of the pivot point for rotation.
 * @param pivotY The y-coordinate of the pivot point for rotation.
 * @param height The height of the sprite.
 * @param on_black Whether the sprite should be drawn on a black background.
 * @return Zero if successful, non-zero otherwise.
 */
int (draw_sprite_bottom)(Sprite *sp, uint16_t x_, uint16_t y_, float scale, float rotation, int pivotX, int pivotY, uint16_t height, bool on_black);

/**
 * @brief Gets the color of a pixel in a sprite.
 *
 * @param sp Pointer to the Sprite.
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @return The color of the pixel.
 */
uint32_t get_color_of(Sprite *sp, uint32_t x, uint32_t y);

/**
 * @brief Draws a mirrored sprite.
 *
 * @param sp Pointer to the Sprite to draw.
 * @param x The x-coordinate where the sprite will be drawn.
 * @param y The y-coordinate where the sprite will be drawn.
 * @return Zero if successful, non-zero otherwise.
 */
int draw_sprite_mirrored(Sprite* sp, uint16_t x, uint16_t y);

/**
 * @brief Draws a sprite.
 *
 * @param sp Pointer to the Sprite to draw.
 * @param x The x-coordinate where the sprite will be drawn.
 * @param y The y-coordinate where the sprite will be drawn.
 * @return Zero if successful, non-zero otherwise.
 */
int draw_sprite(Sprite* sp, uint16_t x, uint16_t y);

/**
 * @brief Draws a sprite centered.
 *
 * @param sp Pointer to the Sprite to draw.
 * @param x The x-coordinate of the center where the sprite will be drawn.
 * @param y The y-coordinate of the center where the sprite will be drawn.
 * @return Zero if successful, non-zero otherwise
 */
int draw_sprite_centered(Sprite* sp, uint16_t x, uint16_t y);

/**
 * @brief Draws a sprite on a background.
 *
 * @param sp Pointer to the Sprite to draw.
 * @param x The x-coordinate where the sprite will be drawn.
 * @param y The y-coordinate where the sprite will be drawn.
 * @param scale The scale of the sprite.
 * @param rotation The rotation of the sprite.
 * @param backgrounX The x-coordinate of the background.
 * @param backgrounY The y-coordinate of the background.
 * @param backgroundW The width of the background.
 * @param mirrored Whether the sprite should be mirrored.
 * @return Zero if successful, non-zero otherwise.
 */
int (draw_sprite_on_background)(Sprite *sp, int x_, int y_, float scale, float rotation, int backgrounX, int backgrounY, uint16_t backgroundW, bool mirrored);

/**
 * @brief Draws a sprite on a black background.
 *
 * @param sp Pointer to the Sprite to draw.
 * @param x The x-coordinate where the sprite will be drawn.
 * @param y The y-coordinate where the sprite will be drawn.
 * @param scale The scale of the sprite.
 * @param rotation The rotation of the sprite.
 * @param backgrounX The x-coordinate of the background.
 * @param backgrounY The y-coordinate of the background.
 * @param backgroundW The width of the background.
 * @param backgroundRot The rotation of the background.
 * @param mirrored Whether the sprite should be mirrored.
 * @return Zero if successful, non-zero otherwise.
 */
int (draw_sprite_on_black_background)(Sprite *sp, int x_, int y_, float scale, float rotation, int backgrounX, int backgrounY, uint16_t backgroundW, float backgroundRot, bool mirrored);

/**
 * @brief Draws a sprite with a specific color.
 *
 * @param sp Pointer to the Sprite to draw.
 * @param x The x-coordinate where the sprite will be drawn.
 * @param y The y-coordinate where the sprite will be drawn.
 * @param color The color to use for the sprite.
 * @return Zero if successful, non-zero otherwise.
 */
int draw_sprite_color(Sprite* sp, uint16_t x, uint16_t y, uint32_t color);
/**
 * @brief Checks if two sprites are colliding.
 *
 *
 * @param sp1 Pointer to the first Sprite.
 * @param scale1 The scale of the first sprite.
 * @param x1 The x-coordinate of the first sprite.
 * @param y1 The y-coordinate of the first sprite.
 * @param sp2 Pointer to the second Sprite.
 * @param scale2 The scale of the second sprite.
 * @param x2 The x-coordinate of the second sprite.
 * @param y2 The y-coordinate of the second sprite.
 * @param collisionX Pointer to an integer where the x-coordinate of the collision point will be stored.
 * @param collisionY Pointer to an integer where the y-coordinate of the collision point will be stored.
 * @return True if the sprites are colliding, false otherwise.
 */
bool check_sprite_collision(Sprite* sp1, float scale1, int x1, int y1, Sprite* sp2, float scale2, int x2, int y2, int* collisionX, int* collisionY);


#endif
