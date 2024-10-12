#include "sprite.h"
#include "../lib/devices/video_card/video.h"
#include <lcom/lcf.h>
#include <math.h>


Sprite *create_sprite(xpm_map_t pic) {
    Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
    xpm_image_t img;
    if( sp == NULL )
        return NULL;
    sp->map = xpm_load(pic, XPM_8_8_8, &img);
    if( sp->map == NULL ) {
        free(sp);
        return NULL;
    }
    sp->width = img.width;
    sp->height = img.height;
    return sp;
}

void destroy_sprite(Sprite *sp) {
    if( sp == NULL )
        return;
    if( sp ->map )
        free(sp->map);
    free(sp);
    sp = NULL;
}

int draw_sprite_mirrored(Sprite* sp, uint16_t x, uint16_t y){
    uint16_t height = sp->height, width = sp->width;
    for (uint16_t i = 0; i < height; i++) {
        for (uint16_t j = 0; j < width; j++) {
            vg_draw_pixel(x + (width-j-1), y + i, get_color_of(sp, j, i));
        }
    }
    return 0;
}

int draw_sprite(Sprite* sp, uint16_t x, uint16_t y){
    uint16_t height = sp->height, width = sp->width;
    for (uint16_t i = 0; i < height; i++) {
        for (uint16_t j = 0; j < width; j++) {
            vg_draw_pixel(x + j, y + i, get_color_of(sp, j, i));
        }
    }
    return 0;
}

inline int draw_sprite_centered(Sprite* sp, uint16_t x, uint16_t y){
    return draw_sprite(sp, x - (sp->width/2), y - (sp->height/2));
}

int draw_sprite_color(Sprite* sp, uint16_t x, uint16_t y, uint32_t color){
    uint16_t height = sp->height, width = sp->width;
    for (uint16_t i = 0; i < height; i++) {
        for (uint16_t j = 0; j < width; j++) {
            vg_draw_pixel(x + j, y + i, get_color_of(sp, j, i) > 0xA4A4A4? color: get_color_of(sp, j, i));
        }
    }
    return 0;
}

int (draw_sprite_on_background)(Sprite *sp, int x_, int y_, float scale, float rotation, int backgrounX, int backgrounY, uint16_t backgroundW, bool mirrored) {
    x_ -= backgrounX;
    y_ = (((y_ - backgrounY) - 300) * (scale * 2)) + 300;
    x_ = (((x_) - 400) * (scale * 2)) + 400;
    return draw_sprite_t(sp, x_, y_ , scale, rotation, 0, 0, mirrored);
}

int (draw_sprite_on_black_background)(Sprite *sp, int x_, int y_, float scale, float rotation, int backgrounX, int backgrounY, uint16_t backgroundW, float backgroundRot, bool mirrored) {
    x_ -= backgrounX;
    y_ = (((y_ - backgrounY) - 300) * (scale * 2)) + 300;
    x_ = (((x_) - 400) * (scale * 2)) + 400;
    if (backgroundRot != 0) {
        float rot = backgroundRot * 0.01745329251;
        float coss = cos(rot);
        float sinn = sin(rot);
        int x = ((x_ - 400) * coss - (y_ - 300) * sinn) + 400;
        int y = ((x_ - 400) * sinn + (y_ - 300) * coss) + 300;
        x_ = x;
        y_ = y;
        rotation += backgroundRot;
    }
    return draw_sprite_black_t(sp, x_, y_ , scale, rotation, 0, 0, mirrored);
}

int (draw_sprite_t)(Sprite *sp, uint16_t x_, uint16_t y_, float scale, float rotation, int pivotX, int pivotY, bool mirrored) {
    //if (pivotX > 1 || pivotX  < 0 || pivotY > 1 || pivotY < 0 || scale == 0) return 1;
    const float angle_radians = rotation * 0.01745329251;

    const float sin_angle_radians = (rotation) ? sin(angle_radians) : 0;
    const float sin_inverted_angle_radians = -sin_angle_radians;
    const float cos_angle_radians = (rotation) ? cos(angle_radians) : 1;
    const float scaledH = scale*sp->height, scaledW = scale*sp->width;
    const uint32_t width = (abs(scaledH*sin_angle_radians) + abs(scaledW*cos_angle_radians));
    const uint32_t height = (abs(scaledW*sin_angle_radians) + abs(scaledH*cos_angle_radians));
    const float height2 = height/2, width2 = width/2;
    int pivot_height = 0;
    int pivot_width = 0;
    if (pivotX || pivotY) 
    {
        const float hipotuse = sqrt((pivotY*scale)*(pivotY*scale) + (pivotX*scale)*(pivotX*scale));
        const float angle = angle_radians - atan((float)(pivotX*scale)/(float)(pivotY*scale));

        pivot_height = cos(angle)*(hipotuse);
        pivot_width= sin(angle)*(hipotuse);
    }
    if (pivotY >= 0) {
        pivot_width = -pivot_width;
        pivot_height = -pivot_height;
    }

    const uint32_t x = x_-width2 - pivot_width, y = y_-height2 + pivot_height;
    const float rx = (width)*cos_angle_radians;
    const float ry = (width)*sin_inverted_angle_radians;
    float fx = (scaledW/2) - (width2*cos_angle_radians) + (height2*sin_inverted_angle_radians) - cos_angle_radians + sin_inverted_angle_radians;
    float fy = (scaledH/2) - (height2*cos_angle_radians) - (width2*sin_inverted_angle_radians) - cos_angle_radians - sin_inverted_angle_radians;
    for (uint32_t v = 0; v < height; v++)
    {
        fx -= sin_inverted_angle_radians;
        fy += cos_angle_radians;
        for (uint32_t h = 0; h < width; h++)
        {
            fx += cos_angle_radians;
            fy += sin_inverted_angle_radians;
            if(*(int*)&fx < 0 || *(int*)&fy < 0) continue;
            const int sx = fx/scale, sy = fy/scale;
            if (sx < sp->width && sy < sp->height)
            {
                vg_draw_pixel(x+h, y+v, get_color_of(sp, mirrored? sp->width-1-sx : sx, sy));
        
                //vg_draw_pixel(sp->x+h-(width/2)-pivot_width, sp->y+v-(height/2)+pivot_height, get_color_of(sp, (fx/scale), (fy/scale)));
            }
            
        }
        fx -= rx;
        fy -= ry;
    }
    return 0;
}

int (draw_sprite_black_t)(Sprite *sp, uint16_t x_, uint16_t y_, float scale, float rotation, int pivotX, int pivotY, bool mirrored) {
    //if (pivotX > 1 || pivotX  < 0 || pivotY > 1 || pivotY < 0 || scale == 0) return 1;
    const float angle_radians = rotation * 0.01745329251;

    const float sin_angle_radians = (rotation) ? sin(angle_radians) : 0;
    const float sin_inverted_angle_radians = -sin_angle_radians;
    const float cos_angle_radians = (rotation) ? cos(angle_radians) : 1;
    const float scaledH = scale*sp->height, scaledW = scale*sp->width;
    const uint32_t width = (abs(scaledH*sin_angle_radians) + abs(scaledW*cos_angle_radians));
    const uint32_t height = (abs(scaledW*sin_angle_radians) + abs(scaledH*cos_angle_radians));
    const float height2 = height/2, width2 = width/2;
    int pivot_height = 0;
    int pivot_width = 0;
    if (pivotX || pivotY) 
    {
        const float hipotuse = sqrt((pivotY*scale)*(pivotY*scale) + (pivotX*scale)*(pivotX*scale));
        const float angle = angle_radians - atan((float)(pivotX*scale)/(float)(pivotY*scale));

        pivot_height = cos(angle)*(hipotuse);
        pivot_width= sin(angle)*(hipotuse);
    }
    if (pivotY >= 0) {
        pivot_width = -pivot_width;
        pivot_height = -pivot_height;
    }

    const uint32_t x = x_-width2 - pivot_width, y = y_-height2 + pivot_height;
    const float rx = (width)*cos_angle_radians;
    const float ry = (width)*sin_inverted_angle_radians;
    float fx = (scaledW/2) - (width2*cos_angle_radians) + (height2*sin_inverted_angle_radians) - cos_angle_radians + sin_inverted_angle_radians;
    float fy = (scaledH/2) - (height2*cos_angle_radians) - (width2*sin_inverted_angle_radians) - cos_angle_radians - sin_inverted_angle_radians;
    for (uint32_t v = 0; v < height; v++)
    {
        fx -= sin_inverted_angle_radians;
        fy += cos_angle_radians;
        for (uint32_t h = 0; h < width; h++)
        {
            fx += cos_angle_radians;
            fy += sin_inverted_angle_radians;
            if(*(int*)&fx < 0 || *(int*)&fy < 0) continue;
            if (!vg_is_black_pixel(x+h, y+v)) continue;
            const int sx = fx/scale, sy = fy/scale;
            if (sx < sp->width && sy < sp->height)
            {
                vg_draw_pixel(x+h, y+v, get_color_of(sp, mirrored? sp->width-1-sx : sx, sy));
        
                //vg_draw_pixel(sp->x+h-(width/2)-pivot_width, sp->y+v-(height/2)+pivot_height, get_color_of(sp, (fx/scale), (fy/scale)));
            }
            
        }
        fx -= rx;
        fy -= ry;
    }
    return 0;
}

int (draw_sprite_bottom)(Sprite *sp, uint16_t x_, uint16_t y_, float scale, float rotation, int pivotX, int pivotY, uint16_t height, bool on_black) {
    //if (pivotX > 1 || pivotX  < 0 || pivotY > 1 || pivotY < 0 || scale == 0) return 1;
    const float angle_radians = rotation * 0.01745329251;

    const float sin_angle_radians = (rotation) ? sin(angle_radians) : 0;
    const float sin_inverted_angle_radians = -sin_angle_radians;
    const float cos_angle_radians = (rotation) ? cos(angle_radians) : 1;
    const float scaledH = scale*sp->height, scaledW = scale*sp->width;
    const uint32_t width = (abs(scaledH*sin_angle_radians) + abs(scaledW*cos_angle_radians));
    const uint32_t heightr = (abs(scaledW*sin_angle_radians) + abs(scaledH*cos_angle_radians));
    const float height2 = heightr/2, width2 = width/2;
    const int margin_top = ((sp->height - height) < 0) ? 0 : (sp->height - height);
    const int pivot_minus_width = pivotX - (sp->width)/2;
    const int pivot_minus_height = pivotY - (sp->height)/2;
    int pivot_height = 0;
    int pivot_width = 0;
    if (pivot_minus_width || pivot_minus_height) 
    {
        const float hipotuse = sqrt((pivot_minus_height*scale)*(pivot_minus_height*scale) + (pivot_minus_width*scale)*(pivot_minus_width*scale));
        const float angle = angle_radians - atan((float)(pivot_minus_width*scale)/(float)(pivot_minus_height*scale));

        pivot_height = cos(angle)*(hipotuse);
        pivot_width= sin(angle)*(hipotuse);
    }
    if (pivot_minus_height >= 0) {
        pivot_width = -pivot_width;
        pivot_height = -pivot_height;
    }

    const uint32_t x = x_-width2 - pivot_width, y = y_-height2 + pivot_height;
    const float rx = (width)*cos_angle_radians;
    const float ry = (width)*sin_inverted_angle_radians;
    float fx = (scaledW/2) - (width2*cos_angle_radians) + (height2*sin_inverted_angle_radians) - cos_angle_radians + sin_inverted_angle_radians;
    float fy = (scaledH/2) - (height2*cos_angle_radians) - (width2*sin_inverted_angle_radians) - cos_angle_radians - sin_inverted_angle_radians;
    for (uint32_t v = 0; v < heightr; v++)
    {
        fx -= sin_inverted_angle_radians;
        fy += cos_angle_radians;
        for (uint32_t h = 0; h < width; h++)
        {
            fx += cos_angle_radians;
            fy += sin_inverted_angle_radians;
            if(*(int*)&fx < 0 || *(int*)&fy < 0) continue;
            const int sx = fx/scale, sy = fy/scale;
            if (sx < sp->width && sy + margin_top < sp->height && (!on_black || vg_is_black_pixel(x+h, y+v)))
            {
                vg_draw_pixel(x+h, y+v, get_color_of(sp, sx, sy + margin_top));
            }
            
        }
        fx -= rx;
        fy -= ry;
    }
    return 0;
}


int (draw_sprite_background_t)(Sprite *sp, int x_, int y_, float scale, float rotation) {
    x_ %= sp->width;
    const float angle_radians = rotation * 0.01745329251;

    const float sin_angle_radians = rotation? sin(angle_radians) : 0;
    const float cos_angle_radians = rotation? cos(angle_radians) : 1;
    const int pivot_minus_width = x_ - (sp->width)/2;
    const int pivot_minus_height = y_ - (sp->height)/2;
    int pivot_height = 0;
    int pivot_width = 0;
    if (pivot_minus_width || pivot_minus_height) 
    {
        const float hipotuse = sqrt((pivot_minus_height*scale)*(pivot_minus_height*scale) + (pivot_minus_width*scale)*(pivot_minus_width*scale));
        const float angle = angle_radians - atan((float)(pivot_minus_width*scale)/(float)(pivot_minus_height*scale));

        pivot_height = cos(angle)*(hipotuse);
        pivot_width= sin(angle)*(hipotuse);
    }
    if (pivot_minus_height >= 0) {
        pivot_width = -pivot_width;
        pivot_height = -pivot_height;
    }

    const float cos_s = cos_angle_radians/scale;
    const float sin_s = sin_angle_radians/scale;
    const float increment_x = 800*cos_s + sin_s;
    const float increment_y = 800*sin_s - cos_s;
    float fx = (float)300*sin_s - (float)400*cos_s;
    float fy = (float)300*cos_s + (float)400*sin_s;
    for (uint32_t v = 0; v < 600; v++)
    {
        for (uint32_t h = 0; h < 800; h++)
        {
            if (!vg_is_black_pixel(h, v)) {
                fx += cos_s;
                fy += sin_s;
                continue;
            }
            int x = (((int)h-400)/scale)*cos_angle_radians - (((int)v-300)/scale)*sin_angle_radians;
            int y = (((int)h-400)/scale)*sin_angle_radians + (((int)v-300)/scale)*cos_angle_radians;
            vg_draw_pixel(h, v, get_color_of(sp, x_+400+x, y_+300+y));
            fx += cos_s;
            fy += sin_s;
        }
        fx -= increment_x;
        fy += increment_y;
    }
    return 0;
}

int (draw_sprite_cannon_t)(Sprite *sp, uint16_t x_, uint16_t y_, float scale, float rotation, int pivotX, int pivotY, int cursorX, int cursorY, uint16_t facing) {
    //if (pivotX > 1 || pivotX  < 0 || pivotY > 1 || pivotY < 0 || scale == 0) return 1;
    const float angle_radians = rotation * 0.01745329251;

    const float scaledH = scale*sp->height, scaledW = scale*sp->width;
    const int pivot_minus_width = pivotX - (sp->width)/2;
    const int pivot_minus_height = pivotY - (sp->height)/2;
    const uint32_t width = (abs(scaledH*sin(angle_radians)) + abs(scaledW*cos(angle_radians)));
    const uint32_t height = (abs(scaledW*sin(angle_radians)) + abs(scaledH*cos(angle_radians)));
    const float height2 = height/2, width2 = width/2;
    int pivot_height = 0;
    int pivot_width = 0;
    if (pivot_minus_width || pivot_minus_height) 
    {
        const float hipotuse = sqrt((pivot_minus_height*scale)*(pivot_minus_height*scale) + (pivot_minus_width*scale)*(pivot_minus_width*scale));
        const float angle = angle_radians - atan((float)(pivot_minus_width*scale)/(float)(pivot_minus_height*scale));

        pivot_height = cos(angle)*(hipotuse);
        pivot_width= sin(angle)*(hipotuse);
    }
    if (pivot_minus_height >= 0) {
        pivot_width = -pivot_width;
        pivot_height = -pivot_height;
    }
    const int x = x_-width2 - pivot_width, y = y_-height2 + pivot_height;
    int rot = (atan2(cursorY - y, cursorX - x)) * (180.0 / M_PI) + 90;
    if (rot < 0) rot += 360;
    facing += ((int)facing+rotation < 0) ? 360+rotation : rotation;
    if (facing > 360) facing -= 360;
    rot -= facing;
    if (rot <= -180) rot += 360;
    if (rot > 180) rot -= 360;
    rot = (rot <= 90) ? rot : 90;
    rot = (rot >= -90) ? rot : -90;
    float cannon_rotation = rot + facing;
    const float sin_angle_radians = sin(cannon_rotation * 0.01745329251);
    const float sin_inverted_angle_radians = -sin_angle_radians;
    const float cos_angle_radians = cos(cannon_rotation * 0.01745329251);

    const float rx = (width)*cos_angle_radians;
    const float ry = (width)*sin_inverted_angle_radians;
    float fx = (scaledW/2) - (width2*cos_angle_radians) + (height2*sin_inverted_angle_radians) - cos_angle_radians + sin_inverted_angle_radians;
    float fy = (scaledH/2) - (height2*cos_angle_radians) - (width2*sin_inverted_angle_radians) - cos_angle_radians - sin_inverted_angle_radians;
    for (uint32_t v = 0; v < height; v++)
    {
        fx -= sin_inverted_angle_radians;
        fy += cos_angle_radians;
        for (uint32_t h = 0; h < width; h++)
        {
            fx += cos_angle_radians;
            fy += sin_inverted_angle_radians;
            if(*(int*)&fx < 0 || *(int*)&fy < 0) continue;
            const int sx = fx/scale, sy = fy/scale;
            if (sx < sp->width && sy < sp->height)
            {
                vg_draw_pixel(x+h, y+v, get_color_of(sp, sx, sy));
            }
            
        }
        fx -= rx;
        fy -= ry;
    }
    return 0;
}

inline uint32_t get_color_of(Sprite *sp, uint32_t x, uint32_t y) {
    return (*(uint32_t*)(sp->map + (x + (sp->width * y))*3)) & 0xffffff;
}


bool check_sprite_collision(Sprite* sp1, float scale1, int x1, int y1, Sprite* sp2, float scale2, int x2, int y2, int* collisionX, int* collisionY){
  if(sp1 == NULL || sp2 == NULL) return false;
  float leftBulletEdge = x1 - ((sp1->width/2)*scale1);
  float rigthBulletEdge = x1 + ((sp1->width/2)*scale1);
  float topBulletEdge = y1 - ((sp1->height/2)*scale1);
  float bottomBulletEdge = y1 + ((sp1->height/2)*scale1);
  float leftEntityEdge = x2 - (sp2->width/2.0)*scale2;
  float rigthEntityEdge = x2 + (sp2->width/2.0)*scale2;
  float topEntityEdge = y2 - (sp2->height/2.0)*scale2;
  float bottomEntityEdge = y2 + (sp2->height/2.0)*scale2;

  if (rigthBulletEdge < leftEntityEdge || leftBulletEdge > rigthEntityEdge || bottomBulletEdge < topEntityEdge || topBulletEdge > bottomEntityEdge) {
        return false;
  }

  float overlapLeft = fmax(leftBulletEdge, leftEntityEdge);
  float overlapRight = fmin(rigthBulletEdge, rigthEntityEdge);
  float overlapTop = fmax(topBulletEdge, topEntityEdge);
  float overlapBottom = fmin(bottomBulletEdge, bottomEntityEdge);
  for (float x = overlapLeft; x < overlapRight; x++) {
    for (float y = overlapTop; y < overlapBottom; y++) {
        int bulletX = (int)(x - leftBulletEdge);
        int bulletY = (int)(y - topBulletEdge);
        int entityX = (int)(x - leftEntityEdge);
        int entityY = (int)(y - topEntityEdge);

        if (get_color_of(sp1, bulletX/scale1, bulletY/scale1) != 0
          && get_color_of(sp2, entityX/scale2, entityY/scale2) != 0) {
            *collisionX = (int) x;
            *collisionY = (int) y;
            return true;
        }
    }
  }
  return false;
}

