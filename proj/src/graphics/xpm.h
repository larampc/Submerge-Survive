#ifndef _XPM_H_
#define _XPM_H_

/** @file */
/** @defgroup xpm Xpm
 *
 *
 * @brief Reading XPM module.
 * @{
 */

/**
 * @brief Reads an XPM image from a map and retrieves its width and height.
 *
 * This function takes an XPM map and extracts the image data, storing the width
 * and height of the image in the provided pointers.
 *
 * @param map The XPM map containing the image data.
 * @param wd Pointer to an integer where the width of the image will be stored.
 * @param ht Pointer to an integer where the height of the image will be stored.
 * @return A pointer to the pixel data of the image.
 */
char *read_xpm(char *map[], int *wd, int *ht);


#endif