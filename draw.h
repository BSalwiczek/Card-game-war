#ifndef DRAW_H
#define DRAW_H

#include "main.h"

#define LEFT_CARD_POSITION_X (COLS-1)/2-COLS/11-12
#define LEFT_CARD_POSITION_Y 5

#define RIGHT_CARD_POSITION_X (COLS-1)/2+COLS/11
#define RIGHT_CARD_POSITION_Y 5

#define PLAYER1_POSITION_X (COLS-1)/2 - COLS/10
#define PLAYER1_POSITION_Y 3
#define PLAYER2_POSITION_X (COLS-1)/2 + COLS/10
#define PLAYER2_POSITION_Y 3

#define CARDS1_QUEUE_POSITION_X (COLS-1)/2 - COLS/5
#define CARDS1_QUEUE_POSITION_Y 3
#define CARDS2_QUEUE_POSITION_X (COLS-1)/2 + COLS/5
#define CARDS2_QUEUE_POSITION_Y 3

#define DEFAULT_COLOR 0
#define WIN_COLOR 1 
#define DEFEAT_COLOR 2

void drawOutput(player_t* player1,player_t* player2, game_t* game);

void drawCardsQueue(int y,int x,int hand[], int DECK_SIZE);

void drawCard(int y,int x,int number, short show_card, short color);

void determineNumber(int number,char *num_display);

void drawMenu();

void drawModesMenu();

#endif