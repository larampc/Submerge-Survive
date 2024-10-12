#include "alphabet.h"
#include <stdarg.h>
#include "graphics/spriteCache.h"
#include <stdio.h>

void draw_word(char word[], int x, int y, int size) {
    for (int i = 0; i < size; i++) {
        if (word[i] == ' ') {x+= 20; continue;}
        draw_sprite(get_font_sprite(word[i]-'a'), x,y);
        x+=get_font_sprite(word[i]-'a')->width + 1;
    }
}

void draw_number(int number, int x, int y) {
    char str[12];
    sprintf(str, "%d", number);

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '0') {
            draw_sprite(get_font_sprite(NUM_0), x, y);
            x += get_font_sprite(NUM_0)->width + 1;
        } else {
            draw_sprite(get_font_sprite((str[i] - '0') + NUM_1 - 1), x, y);
            x += get_font_sprite((str[i] - '0') + NUM_1 - 1)->width + 1;
        }
    }
}


void draw_string(const char str[], int size, int x, int y, bool font, uint32_t color) {
    for (int i = 0; i < size && str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            x += 20;
            continue;
        }
        int index = -1;
        if (str[i] >= '0' && str[i] <= '9') {
            index = (str[i] - '0') + (font? NUM_0 : NUM_0N);
            
        } else if ((str[i] >= 'a' && str[i] <= 'z')) {
            index = str[i]-'a' + (font? 0: AN);
        }
        else if ( (str[i] >= 'A' && str[i] <= 'Z')) {
            index = str[i]-'A'+ (font? 0: AN);
        }
        else if (str[i] == ':') {
            index = DOTS;
        }
        else if (str[i] == '/') {
            index = SLASH;
        }
        else if (str[i] == '\n') {
            break;
        }
        if(index == -1){
            //invalid char
            continue;
        }
        Sprite* letter = get_font_sprite(index);
        font? draw_sprite(letter, x, y) : draw_sprite_color(letter, x, y, color);
        x += letter->width + 1;
    }
}

void draw_current_time(uint8_t hours, uint8_t minutes, uint8_t seconds, int x, int y){
    char date[10];
    sprintf(date, "%02d:%02d:%02d", hours, minutes, seconds);
    draw_string(date, 8, x, y, true, 0);
}
