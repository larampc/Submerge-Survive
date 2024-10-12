#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/devices/kbc/i8042.h"
#include "graphics/alphabet.h"

#define MAX_NAME_LEN 100
#define MAX_TIME_LEN 100

typedef struct {
    char name[MAX_NAME_LEN];
    int money;
    char time[MAX_TIME_LEN];
} Leaderboard;

int compare(const void *a, const void *b) {
    Leaderboard *leaderboardA = (Leaderboard *)a;
    Leaderboard *leaderboardB = (Leaderboard *)b;
    return leaderboardB->money - leaderboardA->money;
}

void write_file_leaderboard(char* name, int money, char* time) {
    Leaderboard leaderboards[6];
    int count = 0;

    FILE *f = fopen("/home/lcom/labs/proj/src/leaderboard.txt", "r");
    if (f != NULL) {
        while (fscanf(f, "%99[^,], %d, %99[^\n]\n", leaderboards[count].name, &leaderboards[count].money, leaderboards[count].time) != EOF && count < 5) {
            count++;
        }
        fclose(f);
    }

    strncpy(leaderboards[count].name, name, MAX_NAME_LEN - 1);
    leaderboards[count].name[MAX_NAME_LEN - 1] = '\0';
    leaderboards[count].money = money;
    strncpy(leaderboards[count].time, time, MAX_TIME_LEN - 1);
    leaderboards[count].time[MAX_TIME_LEN - 1] = '\0';
    count++;

    qsort(leaderboards, count, sizeof(Leaderboard), compare);

    f = fopen("/home/lcom/labs/proj/src/leaderboard.txt", "w");
    if (f != NULL) {
        for (int i = 0; i < count && i < 5; i++) {
            fprintf(f, "%s, %d, %s\n", leaderboards[i].name, leaderboards[i].money, leaderboards[i].time);
        }
        fclose(f);
    }

    // remover linhas vazias
    
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    char* temp_file = "/home/lcom/labs/proj/src/temp.txt";

    f = fopen("/home/lcom/labs/proj/src/leaderboard.txt", "r");
    FILE *temp = fopen(temp_file, "w");
    if (f != NULL && temp != NULL) {
        while ((read = getline(&line, &len, f)) != -1) {
            if (strlen(line) > 1) {
                fprintf(temp, "%s", line);
            }
        }
        fclose(f);
        fclose(temp);
        remove("/home/lcom/labs/proj/src/leaderboard.txt");
        rename(temp_file, "/home/lcom/labs/proj/src/leaderboard.txt");
    }
    if (line)
        free(line);
}

bool update_player_name(char* player_name, uint8_t scancode) {
if (((scancode >= A_KEY && scancode <= L_KEY) || (scancode >= Q_KEY && scancode <= P_KEY) || (scancode >= Z_KEY && scancode <= M_KEY)) && strlen(player_name) < 7) {
        char letter;
        switch(scancode) {
            case A_KEY: letter = 'a'; break;
            case B_KEY: letter = 'b'; break;
            case C_KEY: letter = 'c'; break;
            case D_KEY: letter = 'd'; break;
            case E_KEY: letter = 'e'; break;
            case F_KEY: letter = 'f'; break;
            case G_KEY: letter = 'g'; break;
            case H_KEY: letter = 'h'; break;
            case I_KEY: letter = 'i'; break;
            case J_KEY: letter = 'j'; break;
            case K_KEY: letter = 'k'; break;
            case L_KEY: letter = 'l'; break;
            case M_KEY: letter = 'm'; break;
            case N_KEY: letter = 'n'; break;
            case O_KEY: letter = 'o'; break;
            case P_KEY: letter = 'p'; break;
            case Q_KEY: letter = 'q'; break;
            case R_KEY: letter = 'r'; break;
            case S_KEY: letter = 's'; break;
            case T_KEY: letter = 't'; break;
            case U_KEY: letter = 'u'; break;
            case V_KEY: letter = 'v'; break;
            case W_KEY: letter = 'w'; break;
            case X_KEY: letter = 'x'; break;
            case Y_KEY: letter = 'y'; break;
            case Z_KEY: letter = 'z'; break;
        }
        strncat(player_name, &letter, 1);

    }
    else if (scancode == BACKSPACE_KEY && strlen(player_name) > 0) {

        player_name[strlen(player_name) - 1] = '\0';

    }
    else if (scancode == ENTER_KEY) {
        if (strlen(player_name) > 0) {
            return true;
        }
    }
    return false;
}

void display_leaderboard() {
    FILE *f = fopen("/home/lcom/labs/proj/src/leaderboard.txt", "a+");
    if (f == NULL) return;
    rewind(f);

    fseek(f, 0, SEEK_END);
    if (ftell(f) == 0) {
        draw_string("Go play", 7 , 300, 250, false, 0x7df9ff);
    } else {
        rewind(f);
        char line[256];
        int y = 190;
        size_t max_name_len = 7, max_money_len = 5, max_time_len = 8;

        /*
        size_t max_name_len = strlen("name"), max_money_len = strlen("money"), max_time_len = strlen("time");

        while (fgets(line, sizeof(line), f)) {
            char name[256], money[256], time[256];
            sscanf(line, "%s %s %s", name, money, time);
            if (strlen(name) > max_name_len) max_name_len = strlen(name);
            if (strlen(money) > max_money_len) max_money_len = strlen(money);
            if (strlen(time) > max_time_len) max_time_len = strlen(time);
        }

        rewind(f);
        */

        draw_string("name", strlen("name"), 155 + ((max_name_len-4)/2) * 22, y - 40, false, 0x7df9ff);
        draw_string("money", strlen("money"), 155 + max_name_len * 22 + 15, y - 40, false, 0x7df9ff);
        draw_string("time", strlen("time"), 155 + (max_name_len + max_money_len + (max_time_len-4)/2) * 22 + 35, y - 40, false, 0x7df9ff);

        while (fgets(line, sizeof(line), f)) {
            char name[256], money[256], time[256];
            sscanf(line, "%s %s %s", name, money, time);

            char formatted_name[max_name_len + 1], formatted_money[max_money_len + 1], formatted_time[max_time_len + 1];

            memset(formatted_name, ' ', max_name_len);
            formatted_name[max_name_len] = '\0';
            memcpy(formatted_name, name, strlen(name));

            memset(formatted_money, ' ', max_money_len);
            formatted_money[max_money_len] = '\0';
            memcpy(formatted_money, money, strlen(money));

            memset(formatted_time, ' ', max_time_len);
            formatted_time[max_time_len] = '\0';
            memcpy(formatted_time, time, strlen(time));
            draw_string(formatted_name, max_name_len, 155, y, false, 0xffffff);
            draw_string(formatted_money, max_money_len, 155 + max_name_len * 22 + 15, y, false, 0xffffff);
            draw_string(formatted_time, max_time_len, 155 + (max_name_len + max_money_len) * 22 + 37, y, false, 0xffffff);

            y+=40;
        }
    }
    fclose(f);
}
