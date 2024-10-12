#ifndef _ALPHABET_H_
#define _ALPHABET_H_
#include <lcom/lcf.h>
/** @file */
/** @defgroup alphabet Alphabet
 *
 *
 * @brief Draw text module.
 * @{
 */

/**
 * @brief Draws a word on the screen.
 *
 * @param word The word to be drawn.
 * @param x The x-coordinate where the word will be drawn.
 * @param y The y-coordinate where the word will be drawn.
 * @param size The size of the word.
 */
 void draw_word(char word[], int x, int y, int size);

/**
 * @brief Draws a number on the screen.
 *
 * @param number The number to be drawn.
 * @param x The x-coordinate where the number will be drawn.
 * @param y The y-coordinate where the number will be drawn.
 */
 void draw_number(int number, int x, int y);

/**
 * @brief Draws a string on the screen.
 *
 * @param str The string to be drawn.
 * @param size The size of the string.
 * @param x The x-coordinate where the string will be drawn.
 * @param y The y-coordinate where the string will be drawn.
 * @param font The font to be used.
 * @param color The color of the string.
 */
void draw_string(const char str[], int size, int x, int y, bool font, uint32_t color);

/**
 * @brief Draws the current time on the screen.
 *
 * @param hours The current hours.
 * @param minutes The current minutes.
 * @param seconds The current seconds.
 * @param x The x-coordinate where the time will be drawn.
 * @param y The y-coordinate where the time will be drawn.
 */
void draw_current_time(uint8_t hours, uint8_t minutes, uint8_t seconds, int x, int y);


#endif
