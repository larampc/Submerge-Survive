#ifndef _HOOK_H_
#define _HOOK_H_

#include <lcom/lcf.h>
#include "graphics/Sprite.h"

/** @defgroup hook Hook
 * @brief Hook handling module.
 * @{
 */

/**
 * @brief Structure representing the fishing hook.
 */
typedef struct {
  Sprite* hook; /**< Pointer to the sprite representing the hook */
  int height; /**< Height of the hook */
} Hook;

/**
 * @brief Creates a fishing hook with the given sprite.
 * 
 * @param sp Pointer to the sprite to be used for the hook.
 * @return Pointer to the created hook.
 */
Hook* create_hook(Sprite* sp);

/**
 * @brief Destroy a fishing hook, clearing its memory.
 * 
 * @param hook Pointer to the hook to destroy.
 */
void destroy_hook(Hook* hook);

/** @} end of hook */

#endif /* _HOOK_H_ */
