#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <time.h>
#include <ncursesw/ncurses.h>



// #define SIMULATION_MODE

#define SHUFFLE_SIZE 1000 //wyznacza ile ma zostać wykonanych zamienień w talii
#define SUIT_SIZE 13
#define EMPTY -1
#define INFINITY -1
#define BUFFOR_SIZE 2

typedef enum {
    playing,
    win,
    defeat,
} status_t;

typedef enum {
    A,
    B
} variant_t;

typedef enum {
    random_choice,
    furious,
    peaceful,
    none
} strategy_t;

typedef enum{
    normal,
    wise
} war_type_t;

typedef enum{
    singleplayer,
    multiplayer
} game_mode_t;

typedef enum{
    empty,
    clubs, //trefl
    diamonds, //karo
    hearts, //kier
    pikes, //pik
} suit_t;

typedef struct{
    int number;
    suit_t suit;
} card_t;

typedef struct {
    int rank;
    const char *name;
    card_t *hand;
    card_t *stack;
    card_t *buffor;
    status_t player_status;
    strategy_t strategy;
} player_t;

typedef struct{
    int deck_size;
    int moves;
    variant_t variant;
    war_type_t war_type;
    game_mode_t mode;
} game_t;

void initializePlayer(player_t* const player, const int deck_size,const char* name);

short startGame(game_t* const game);

void endGame(const player_t* const winner, const int moves);

void initializeDeckWithRandomNumbers(card_t* const deck, const int deck_size);

void splitIntoTwoHands(const card_t* const deck,player_t* const player1,player_t* const player2, const int deck_size);

void playGame(player_t* const player1,player_t* const player2, game_t* const game);

void playWiseTurn(player_t* const you,player_t* const opponent,player_t* const player1,player_t* const player2, game_t* const game);

void shiftCardLeft(card_t* const hand, const int steps, const int deck_size);  //przesunięcie kart jednej talii o steps w lewo

int war(player_t* const player1, player_t* const player2, game_t* const game);

int putCardsOnStackWise(player_t* const chooser, player_t* const other, game_t* const game, const int cards_in_war); //umieszcza karty do wyboru na stacku gracza

void variantBsplitCards(player_t* const player1, player_t* const player2, const int cards_in_war, const int deck_size);

void giveCardsToWinner(player_t* const winner, player_t* const looser, const short cards_in_war, const int deck_size, const war_type_t war_type);

void clearCards(card_t* const cards, const int size);

void clearCard(card_t* const card);

void saveResults(const player_t* const player1, const player_t* const player2, const game_t* const game,FILE* const file);

void determineHandRank(player_t* const player, const int DECK_SIZE);

char chooseCard(const player_t* const you, const card_t* const opponent_stack, const player_t* const player2);

#endif