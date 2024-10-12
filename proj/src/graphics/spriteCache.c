#include "spriteCache.h"

#include "../assets/play.h"
#include "../assets/sub.h"
#include "../assets/background.h"
#include "../assets/menu.h"
#include "../assets/playerselect.h"
#include "../assets/trash_xpm.h"
#include "../assets/chars.h"
#include "../assets/fishes.h"
#include "../assets/explosions.h"
#include "../assets/heart&gold&oxygen.h"
#include "../assets/shop_xpm.h"

#include "alphabet.h"

#include "../lib/devices/video_card/video.h"
#include <stdarg.h>

static bool letters_loaded = false;
static int game_sprites_load_progress = 0;


static void draw_loading() {
    if(!letters_loaded) return;
    draw_word("loading", 335, 240, 7);
    vg_draw_vline(220, 285, 30, 0x28282B);
    vg_draw_vline(220 + GAME_XPMS_NUM*5 + 2, 285, 30, 0x28282B);
    vg_draw_hline(220, 284, GAME_XPMS_NUM*5 + 2, 0x28282B);
    vg_draw_hline(220, 315, GAME_XPMS_NUM*5 + 2, 0x28282B);
    vg_draw_rectangle( 221, 285, GAME_XPMS_NUM*5, 30, 0x808080);
    vg_draw_rectangle( 221, 285, game_sprites_load_progress*5, 30, 0xADD8E6);
    vg_page_flip();
}


static int load_sprites_(Sprite** array, uint8_t no_pic, xpm_map_t pic1, ...) {
    va_list ap;
    va_start(ap, pic1);
    Sprite* sp;
    if( (sp = create_sprite(pic1)) == NULL) {
        free(sp);
        return 1;
    }
    array[0] = sp;
    for(int  i = 1; i < no_pic; i++ ) {
        draw_loading();
        xpm_row_t* tmp = va_arg(ap, xpm_row_t*);
        Sprite* sptmp;
        if( (sptmp = create_sprite(tmp)) == NULL) {
            for(int j = 0; j < i; j++)
                destroy_sprite(sprites[i]);
                free(sprites[i]);
            va_end(ap);
            return 1;
        }
        array[i] = sptmp;
        game_sprites_load_progress += letters_loaded;
    }
    va_end(ap);
    return 0;
}

Sprite* get_sprite(GAME_XPM_ID id) {
    return sprites[id];
}
Sprite* get_font_sprite(LETTER_XPM id){
    return letters[id];
}

static int load_game_sprites() {
  return load_sprites_(sprites, GAME_XPMS_NUM, pinkidle1, pinkidle2, pinkidle3, pinkidle4, pink1, pink2, pink3, pink4, pink5, pink6, pinkclimb1, pinkclimb2, pinkclimb3, pinkclimb4, submarine, submask, ocean, pewpew, explosion1, explosion2, explosion3, explosion4, explosion5, blueidle1, blueidle2, blueidle3, blueidle4, blue1, blue2, blue3, blue4, blue5, blue6, blueclimb1, blueclimb2, blueclimb3, blueclimb4,
  main_menu, choose_player, waiting, aim, cursorBlue, oxigen, oxigen_bg, full_heart, empty_heart, hook_point, plane, ball_cannon, money, trash, bubble_trash, pause_menu, game_over, leaderboard, fish_1, fish_2, fish_3, fish_4, fish_5, fish_6, trash_pop1, trash_pop2, trash_pop3, trash_pop4, shop, stones_lives, stones_fire, stones_oxygen, stones_speed, cross_stone);
}
static int load_letters_sprites() {
     return load_sprites_(letters, LETTER_XPMS_NUM, char_A, char_B, char_C, char_D, char_E, char_F, char_G, char_H, char_I, char_J, char_K, char_L, char_M, char_N, char_O, char_P, char_Q, char_R, char_S, char_T, char_U, char_V, char_W, char_X, char_Y, char_Z, char_0_gold_font, char_1_gold_font, char_2_gold_font, char_3_gold_font, char_4_gold_font, char_5_gold_font, char_6_gold_font, char_7_gold_font, char_8_gold_font, char_9_gold_font,
     char_A_normal_font, char_B_normal_font, char_C_normal_font, char_D_normal_font, char_E_normal_font, char_F_normal_font, char_G_normal_font, char_H_normal_font, char_I_normal_font, char_J_normal_font, char_K_normal_font, char_L_normal_font, char_M_normal_font, char_N_normal_font, char_O_normal_font, char_P_normal_font, char_Q_normal_font, char_R_normal_font, char_S_normal_font, char_T_normal_font, char_U_normal_font, char_V_normal_font, char_W_normal_font, char_X_normal_font, char_Y_normal_font, char_Z_normal_font, 
     char_0_normal_font, char_1_normal_font, char_2_normal_font, char_3_normal_font, char_4_normal_font, char_5_normal_font, char_6_normal_font, char_7_normal_font, char_8_normal_font, char_9_normal_font, slash, dots);
}

int load_sprites() {
    if(load_letters_sprites() != 0) {
        printf("in %s: failed to load letter sprites\n", __func__);
        return 1;
    }
    letters_loaded = true;
    if(load_game_sprites() != 0) {
        printf("in %s: failed to load game sprites\n", __func__);
        return 1;
    }
    return 0;
}

static void destroy_game_sprites() {
    for(int i = 0; i < GAME_XPMS_NUM; i++) {
        destroy_sprite(sprites[i]);
        free(sprites[i]);
    }
}
static void destroy_letters_sprites() {
    for(int i = 0; i < LETTER_XPMS_NUM; i++) {
        destroy_sprite(letters[i]);
        free(letters[i]);
    }
}

void destroy_sprites(){
    destroy_game_sprites();
    destroy_letters_sprites();
}
