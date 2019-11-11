#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <time.h>
#include <ncursesw/ncurses.h>
#include "main.h"

/*
────────────── Numeracja Kart (przy standardowej talii) ──────────────
indeksy 0 ─ 12: trefl, kolejno starszeństwem, 0 ─ dwójka trefl ... 12 ─ as trefl
indeksy 13 ─ 25: karo, kolejno starszeństwem, 13 ─ dwójka karo ... 25 ─ as karo
indeksy 26 ─ 38: kier, kolejno starszeństwem, 26 ─ dwójka kier ... 38 ─ as kier
indeksy 39 ─ 51: pik, kolejno starszeństwem, 39 ─ dwójka pik ... 51 ─ as pik
*/

int main()
{
    setlocale(LC_ALL, "");

    srand(time(NULL));

    char str[3];

    if(!SIMULATION_MODE)
    {
        //inicjalizacja ncurses
        initscr();
        cbreak();
        clear();

        mvprintw(LINES/2,COLS/2-15,"Podaj rozmiar talii(max: 99): ");
        refresh();
        getnstr(str,sizeof(str)-1);
        mvprintw(LINES/2+1,COLS/2-7,"%s",str);
        refresh();
    }else{
        str[0] = '2';
        str[1] = '0';
        str[2] = '\0';
    }
    

    game_t* game = malloc(sizeof(game_t));
    game->DECK_SIZE = atoi(str);
    game->variant = A;
    game->moves = 0;

    player_t* player1 = malloc(sizeof(player_t));
    player_t* player2 = malloc(sizeof(player_t));

    player1->hand = (int *) malloc(game->DECK_SIZE * sizeof(int));
    player1->stack = (int *) malloc(game->DECK_SIZE * sizeof(int));
    player1->player_status = playing;
    player1->name = "Gracz 1";

    player2->hand = (int *) malloc(game->DECK_SIZE * sizeof(int));
    player2->stack = (int *) malloc(game->DECK_SIZE * sizeof(int));   
    player2->player_status = playing;
    player2->name = "Gracz 2";


    int *deck = (int *) malloc(game->DECK_SIZE * sizeof(int));

    for(int i=0;i<game->DECK_SIZE;i++)
    {
        deck[i] = EMPTY;
        player1->hand[i] = EMPTY;
        player2->hand[i] = EMPTY;
        player1->stack[i] = EMPTY;
        player2->stack[i] = EMPTY;
    }

    initializeDeckWithRandomNumbers(deck, game);

    splitIntoTwoHands(deck,player1,player2, game);

    if(SIMULATION_MODE)
    {

        printf("saving to file...\n");
        FILE *file;
        file = fopen("run/Problem1/A1.txt","a");
        if(file == NULL){
            printf("Otwarcie pliku nie powiodło sie\n");
            return 0;
        }

        fprintf(file, "Deck size,");
        fprintf(file, "Moves,");
        fprintf(file, "Variant,");
        fprintf(file, "Winner\n");

        for(int i=1;i<1001;i++)
        {
            srand((int) time(NULL)/i);
            initializeDeckWithRandomNumbers(deck, game);
            splitIntoTwoHands(deck,player1,player2, game);
            playGame(player1,player2, game);
            saveResults(player1,player2,game,file);
            game->moves = 0;
        }

        fclose(file);
    }else{
        playGame(player1,player2, game);
        getch();
        endwin();
    }

   return 0;
}

void initializeDeckWithRandomNumbers(int deck[], game_t* game)
{
    // for(int i=0;i<game.DECK_SIZE;i++)
    // {
    //     deck[i] = i+1;
    // }

    for(int i=0;i<game->DECK_SIZE;i++)
    {
        if(i<game->DECK_SIZE/4)
            deck[i] = SUIT_SIZE-1-i;
        else if(i<game->DECK_SIZE/2)
            deck[i] = 2*SUIT_SIZE-1-i + game->DECK_SIZE/4;
        else if(i<game->DECK_SIZE*3/4)
            deck[i] = 3*SUIT_SIZE-1-i + game->DECK_SIZE/2;
        else
            deck[i] = 4*SUIT_SIZE-1-i + game->DECK_SIZE*3/4;
    }   

    for(int i=0;i<SHUFFLE_SIZE;i++)
    {
        int index1 = rand() % game->DECK_SIZE;
        int index2 = rand() % game->DECK_SIZE;
        int copy = deck[index1];
        deck[index1] = deck[index2];
        deck[index2] = copy;
    }
}

void splitIntoTwoHands(int deck[],player_t* player1,player_t* player2, game_t* game)
{
    // player1->hand[0] = 10;
    // player1->hand[1] = 11;
    // player1->hand[2] = 9;
    // player1->hand[3] = 5;
    // player1->hand[4] = 9;
    // player1->hand[5] = 12;
    // player1->hand[6] = 13;
    // player1->hand[7] = 13;
    // player1->hand[8] = 13;
    // player1->hand[9] = 13;
    // player1->hand[10] = 13;
    // player1->hand[11] = 13;

    // player2->hand[0] = 10;

    for(int i=0;i<game->DECK_SIZE;i++)
    {
         if(i<game->DECK_SIZE/2)
            player1->hand[i] = deck[i];
         else
             player2->hand[i%(game->DECK_SIZE/2)] = deck[i];
    }
}

void clearStacks(player_t* player1, player_t* player2, int DECK_SIZE)
{
    for(int i=0;i<DECK_SIZE;i++)
    {
        player1->stack[i] = EMPTY;
        player2->stack[i] = EMPTY;
    }
}

// void printHands(int hand1[],int hand2[])
// {
//     for(int i=0;i<DECK_SIZE;i++)
//     {
//         wprintf(L"%i",hand1[i]);
//         wprintf(L" ");
//     }
//         wprintf(L"\n");
//     for(int i=0;i<DECK_SIZE;i++)
//     {
//         wprintf(L"%i",hand2[i]);
//         wprintf(L" ");
//     }
//         wprintf(L"\n");
// }

void playGame(player_t* player1,player_t* player2, game_t* game)
{
    while(true)
    {
        player1->stack[0] = player1->hand[0];
        player2->stack[0] = player2->hand[0];
        game->moves++;
        shiftCardLeft(player1->hand,1,game->DECK_SIZE);
        shiftCardLeft(player2->hand,1,game->DECK_SIZE);
        drawOutput(player1,player2,game->DECK_SIZE);

        if(player1->stack[0]%SUIT_SIZE > player2->stack[0]%SUIT_SIZE)
        {
            giveCardsToWinner(player1,player2,1,game->DECK_SIZE);
        }else if(player1->stack[0]%SUIT_SIZE < player2->stack[0]%SUIT_SIZE)
        {
            giveCardsToWinner(player2,player1,1,game->DECK_SIZE);
        }else{
            war(player1,player2, game);
        }
        if(player1->hand[0] == EMPTY)
        {
            player1->player_status = defeat;
            player2->player_status = win;
            endGame(player2,game->moves);
            break;
        }
        if(player2->hand[0] == EMPTY)
        {
            player2->player_status = defeat;
            player1->player_status = win;
            endGame(player1,game->moves);
            break;
        }
        //zapętlenie
        if(game->moves > 100000)
        {
            game->moves = -1;
            break;
        }
    }
}

void endGame(player_t* winner, int moves)
{
    if(!SIMULATION_MODE)
    {
        clear();
        mvprintw(LINES/2+5,COLS/2 - 8,"%s wygrał! Ilosc ruchow %i",winner->name, moves);    
        refresh();
        getchar();
    }
}



void shiftCardLeft(int hand[], int steps, int DECK_SIZE)  //przesunięcie kart talii o steps w lewo
{
    for(int i=0;i<DECK_SIZE-steps;i++)
    {
        hand[i] = hand[i+steps];
    }
    for(int i=1;i<=steps;i++)
    {
        hand[DECK_SIZE-i] = EMPTY;
    }
    
}

int war(player_t* player1, player_t* player2, game_t* game)
{
    int war_count = 0;
    int war_ended = 0;

    int player_helped = false;

    while(war_ended == false)
    {
        int cards_in_war = 3+war_count*2;
        int player_helped_in_this_turn = false;

        if(cards_in_war-1 >= game->DECK_SIZE)
            return 0;

        if(player1->hand[0] == EMPTY || player2->hand[0] == EMPTY || player1->hand[1] == EMPTY || player2->hand[1] == EMPTY)
        {
            //wariant A
            //return 0;

            //wariant B
            if(player_helped == false) //jeden z graczy może wspomóc drugiego tylko raz podczas wojnie
            {
                player_helped = true;
                player_helped_in_this_turn = true;
                variantBsplitCards(player1,player2,cards_in_war,game->DECK_SIZE);
                game->moves += 2;
                drawOutput(player1,player2,game->DECK_SIZE);
            }else{
                return 0;
            }
            
        }

        if(player_helped_in_this_turn == false)
        {
            //umieszczenie po 2 kart każdego gracza na stosie
            player1->stack[cards_in_war - 2] = player1->hand[0];
            player1->stack[cards_in_war - 1] = player1->hand[1];

            player2->stack[cards_in_war - 2] = player2->hand[0];
            player2->stack[cards_in_war - 1] = player2->hand[1];

            game->moves += 2;

            shiftCardLeft(player1->hand,2,game->DECK_SIZE);
            shiftCardLeft(player2->hand,2,game->DECK_SIZE);
            drawOutput(player1,player2,game->DECK_SIZE);
        }

        if(player1->stack[cards_in_war - 1] %SUIT_SIZE > player2->stack[cards_in_war - 1] % SUIT_SIZE)
        {
            giveCardsToWinner(player1,player2,cards_in_war,game->DECK_SIZE);
            war_ended = 1;
            break;
        }else if (player1->stack[cards_in_war - 1]%SUIT_SIZE < player2->stack[cards_in_war - 1] % SUIT_SIZE)
        {
            giveCardsToWinner(player2,player1,cards_in_war,game->DECK_SIZE);
            war_ended = 1;
            break;
        }else{
            war_count++;

        }
    }
        
}

void giveCardsToWinner(player_t* winner, player_t* looser, short cards_in_war, int DECK_SIZE)
{
    for(int i=0;;i++)
    {
        if(winner->hand[i] == EMPTY)
        {
            //przekazanie kart graczowi który wygrał
            for(int j=0;j<cards_in_war;j++)
            {
                winner->hand[i+j] = winner->stack[j];
            }
            for(int j=0;j<cards_in_war;j++)
            {
                winner->hand[i+cards_in_war+j] = looser->stack[j];
            }
            clearStacks(looser,winner,DECK_SIZE);
            break;
        }
    }
}

void variantBsplitCards(player_t* player1, player_t* player2, int cards_in_war,int DECK_SIZE)
{
    if(player1->hand[0] == EMPTY)
    {   
        player1->stack[cards_in_war - 2] = player2->hand[0];
        player1->stack[cards_in_war - 1] = player2->hand[1];

        player2->stack[cards_in_war - 2] = player2->hand[2];
        player2->stack[cards_in_war - 1] = player2->hand[3];

        shiftCardLeft(player2->hand,4,DECK_SIZE);
    }else if(player1->hand[1] == EMPTY)
    {
        player1->stack[cards_in_war - 2] = player1->hand[0];
        player1->stack[cards_in_war - 1] = player2->hand[1];

        player2->stack[cards_in_war - 2] = player2->hand[0];
        player2->stack[cards_in_war - 1] = player2->hand[2];
        shiftCardLeft(player1->hand,1,DECK_SIZE);
        shiftCardLeft(player2->hand,3,DECK_SIZE);
    }

    if(player2->hand[0] == EMPTY)
    {
        player2->stack[cards_in_war - 2] = player1->hand[0];
        player2->stack[cards_in_war - 1] = player1->hand[1];

        player1->stack[cards_in_war - 2] = player1->hand[2];
        player1->stack[cards_in_war - 1] = player1->hand[3];

        shiftCardLeft(player1->hand,4,DECK_SIZE);
    }else if(player2->hand[1] == EMPTY)
    {
        player2->stack[cards_in_war - 2] = player2->hand[0];
        player2->stack[cards_in_war - 1] = player1->hand[1];
        
        player1->stack[cards_in_war - 2] = player1->hand[0];
        player1->stack[cards_in_war - 1] = player1->hand[2];
        shiftCardLeft(player2->hand,1,DECK_SIZE);
        shiftCardLeft(player1->hand,3,DECK_SIZE);
    }
}

void saveResults(player_t* player1, player_t* player2, game_t* game,FILE *file)
{

    fprintf(file, "%i,",game->DECK_SIZE);
    fprintf(file, "%i,",game->moves);
    fprintf(file, "%i,",game->variant);
    fprintf(file, "%i",player1->player_status == win);
    fprintf(file,"\n");

}
