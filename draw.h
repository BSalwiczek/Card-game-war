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

#define CARDS1_QUEUE_POSITION_X (COLS-1)/2 - COLS/4
#define CARDS1_QUEUE_POSITION_Y 3
#define CARDS2_QUEUE_POSITION_X (COLS-1)/2 + COLS/4
#define CARDS2_QUEUE_POSITION_Y 3

#define DEFAULT_COLOR 0
#define WIN_COLOR 1 
#define DEFEAT_COLOR 2

void drawOutput(const player_t* const player1,const player_t* const player2, const game_t* const game);

void drawCardsQueue(const int y,const int x,const card_t* const hand, const int deck_size);

void drawCard(const int y,const int x,const card_t card, const short show_card, const short color);

char determineSymbol(const card_t card);

int determineSuit(const card_t card);

void drawMenu();

void drawModesMenu();

#endif