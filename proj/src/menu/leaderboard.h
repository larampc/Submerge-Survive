#ifndef _LEADERBOARD_H_
#define _LEADERBOARD_H_
#include <lcom/lcf.h>

/** @file */
/** @defgroup leaderboard Leaderboard
 *
 * @brief Leaderboard handling module.
 * @{
 */

/**
 * @brief Structure representing a leaderboard entry.
 */
typedef struct {
    char name[50]; /**< @brief The name of the player. */
    int money; /**< @brief The amount of money the player has. */
    char time[10]; /**< @brief The time associated with the player's score. */
} Leaderboard;

/**
 * @brief Writes a new entry to the leaderboard file.
 *
 * This function adds a new player's score to the leaderboard file.
 *
 * @param name The name of the player.
 * @param money The amount of money the player has.
 * @param time The time associated with the player's score.
 */
void write_file_leaderboard(char* name, int money, char* time);

/**
 * @brief Compares two leaderboard entries.
 *
 * This function is used for sorting the leaderboard. It compares two entries
 * based on their scores.
 *
 * @param a Pointer to the first entry.
 * @param b Pointer to the second entry.
 * @return An integer less than, equal to, or greater than zero if the first entry is
 * considered to be respectively less than, equal to, or greater than the second.
 */
int compare(const void *a, const void *b);

/**
 * @brief Displays the leaderboard.
 *
 * This function shows the current leaderboard, usually by printing it to the screen.
 */
void display_leaderboard();

/**
 * @brief Updates the player's name based on keyboard input.
 *
 * This function modifies the player's name according to the provided scancode.
 *
 * @param name The current name of the player.
 * @param scancode The scancode of the key that was pressed.
 * @return True if the name was successfully updated, false otherwise.
 */
bool update_player_name(char* name, uint8_t scancode);

#endif
