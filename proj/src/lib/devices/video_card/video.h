#ifndef _VIDEO_H_
#define _VIDEO_H_

#include "vbe.h"
#include <lcom/lcf.h>
#include "../../../graphics/sprite.h"

/** @file */
/** @defgroup video Video
 * @brief Video card management module
 * @{
 *
 */

/**
 * @brief Initializes the video card in a specific mode.
 * 
 * @param mode The VBE mode to be set.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (vg_initialize)(uint16_t mode);

/**
 * @brief Retrieves information about a specific VBE mode.
 * 
 * @param mode The VBE mode to get information from.
 * @param vmi_p Pointer to a vbe_mode_info_t structure to store the mode information.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (vbe_get_mode_inf)(uint16_t mode, vbe_mode_info_t* vmi_p);

/**
 * @brief Clears the video buffer.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (clear_buffer)();

/**
 * @brief Draws a rectangle on the screen.
 * 
 * @param x The x-coordinate of the rectangle's top-left corner.
 * @param y The y-coordinate of the rectangle's top-left corner.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws a vertical line on the screen.
 * 
 * @param x The x-coordinate of the line's starting point.
 * @param y The y-coordinate of the line's starting point.
 * @param len The length of the line.
 * @param color The color of the line.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a horizontal line on the screen.
 * 
 * @param x The x-coordinate of the line's starting point.
 * @param y The y-coordinate of the line's starting point.
 * @param len The length of the line.
 * @param color The color of the line.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a pixel on the screen.
 * 
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Performs page flipping to update the screen.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (vg_page_flip)();

/**
 * @brief Draws a sprite to fill the entire screen.
 * 
 * @param sp Pointer to the sprite to be drawn.
 */
void (draw_sprite_full_screen)(Sprite* sp);

/**
 * @brief Draws a larger sprite line.
 * 
 * @param sp Pointer to the sprite to be drawn.
 * @param x The x-coordinate of the sprite's position.
 * @param y The y-coordinate of the sprite's position.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (draw_larger_sprite_line)(Sprite *sp, int x, int y);

/**
 * @brief Checks if a pixel at a specific position is black.
 * 
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * 
 * @return 1 if the pixel is black, 0 otherwise.
 */
int (vg_is_black_pixel)(uint16_t x, uint16_t y);

/**
 * @brief Draws a smaller sprite line.
 * 
 * @param sp Pointer to the sprite to be drawn.
 * @param x The x-coordinate of the sprite's position.
 * @param y The y-coordinate of the sprite's position.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (draw_smaller_sprite_line)(Sprite *sp, uint16_t x, uint16_t y);

/**
 * @brief Draws a line by splitting two sprites.
 * 
 * @param sp1 Pointer to the first sprite.
 * @param sp2 Pointer to the second sprite.
 * @param x The x-coordinate of the line's starting point.
 * @param y The y-coordinate of the line's starting point.
 * @param width The width of the line.
 * @param height The height of the line.
 * @param line The line to be drawn.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (draw_splitted_sprites_line)(Sprite *sp1, Sprite *sp2, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t line);

#endif /* _VIDEO_H_ */
