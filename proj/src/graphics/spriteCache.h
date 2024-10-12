#ifndef _SPRITE_CACHE_H
#define _SPRITE_CACHE_H
#include "sprite.h"

/** @file */
/** @defgroup spritecache SpriteCache
 *
 *
 * @brief Sprite cache module.
 * @{
 */

/**
 * @brief Enumeration representing different game sprite IDs.
 */
typedef enum XPM_ID {
  PLAYER_PINK_IDLE_1, 
  PLAYER_PINK_IDLE_2, 
  PLAYER_PINK_IDLE_3, 
  PLAYER_PINK_IDLE_4, 
  PLAYER_PINK_WALKING_1,
  PLAYER_PINK_WALKING_2,
  PLAYER_PINK_WALKING_3,
  PLAYER_PINK_WALKING_4,
  PLAYER_PINK_WALKING_5,
  PLAYER_PINK_WALKING_6,
  PLAYER_PINK_CLIMB_1,
  PLAYER_PINK_CLIMB_2,
  PLAYER_PINK_CLIMB_3,
  PLAYER_PINK_CLIMB_4,
  SUBMARINE,
  SUBMASK,
  BACKGROUND,
  CANNON,
  EXPLOSION1,
  EXPLOSION2,
  EXPLOSION3,
  EXPLOSION4,
  EXPLOSION5,
  PLAYER_BLUE_IDLE_1, 
  PLAYER_BLUE_IDLE_2, 
  PLAYER_BLUE_IDLE_3, 
  PLAYER_BLUE_IDLE_4, 
  PLAYER_BLUE_WALKING_1,
  PLAYER_BLUE_WALKING_2,
  PLAYER_BLUE_WALKING_3,
  PLAYER_BLUE_WALKING_4,
  PLAYER_BLUE_WALKING_5,
  PLAYER_BLUE_WALKING_6,
  PLAYER_BLUE_CLIMB_1,
  PLAYER_BLUE_CLIMB_2,
  PLAYER_BLUE_CLIMB_3,
  PLAYER_BLUE_CLIMB_4,
  MENU,
  CHOOSEPLAYER,
  LOADING,
  AIM,
  CURSOR,
  OXIGEN,
  OXIGEN_BG,
  FULL_HEART,
  EMPTY_HEART,
  HOOK,
  PLANE,
  MINE,
  MONEY,
  NO_BUBBLE_TRASH,
  BUBBLE_TRASH,
  PAUSE_BG,
  GAMEOVER_BG,
  LEADERBOARD_BG,
  FISH_1,
  FISH_2,
  FISH_3,
  FISH_4,
  FISH_5,
  FISH_6,
  TRASH_POP1,
  TRASH_POP2,
  TRASH_POP3,
  TRASH_POP4,
  SHOP_BG,
  STONE_LIVES,
  STONE_FIRE,
  STONE_OXYGEN,
  STONE_SPEED,
  CROSS_STONE
} GAME_XPM_ID;

/**
 * @brief Enumeration representing different font sprite IDs.
 */
typedef enum {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N, 
    O, 
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W, 
    X,
    Y,
    Z,
    NUM_0,
    NUM_1,
    NUM_2,
    NUM_3,
    NUM_4,
    NUM_5,
    NUM_6,
    NUM_7,
    NUM_8,
    NUM_9,
    AN,
    BN,
    CN,
    DN,
    EN,
    FN,
    GN,
    HN,
    IN,
    JN,
    KN,
    LN,
    MN,
    NN, 
    ON, 
    PN,
    QN,
    RN,
    SN,
    TN,
    UN,
    VN,
    WN, 
    XN,
    YN,
    ZN,
    NUM_0N,
    NUM_1N,
    NUM_2N,
    NUM_3N,
    NUM_4N,
    NUM_5N,
    NUM_6N,
    NUM_7N,
    NUM_8N,
    NUM_9N,
    SLASH,
    DOTS
} LETTER_XPM;
#define GAME_XPMS_NUM 71 /**< @brief Number of game sprites. */
#define LETTER_XPMS_NUM 74 /**< @brief Number of font sprites. */

static Sprite* sprites[GAME_XPMS_NUM]; /**< @brief Array of game sprite pointers. */
static Sprite* letters[LETTER_XPMS_NUM]; /**< @brief Array of font sprite pointers. */

/**
 * @brief Retrieves a sprite given its ID.
 * @param id The ID of the sprite to retrieve.
 * @return A pointer to the sprite.
 */
Sprite* get_sprite(GAME_XPM_ID id);

/**
 * @brief Retrieves a font sprite given its ID.
 * @param id The ID of the font sprite to retrieve.
 * @return A pointer to the font sprite.
 */
Sprite* get_font_sprite(LETTER_XPM id);

/**
 * @brief Loads all the sprites into memory.
 * @return Zero on success, non-zero on failure.
 */
int load_sprites();

/**
 * @brief Frees all the loaded sprites from memory.
 */
void destroy_sprites();

#endif
