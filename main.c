#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <time.h>
#include <ncursesw/ncurses.h>
#include "main.h"

// #define DECK_SIZE 10
#define SHUFFLE_SIZE 1000 //wyznacza ile ma zostać wykonanych zamienień w talii
//colored console
#define GREEN  "\x1B[32m"
#define RESET "\x1B[0m"

#define LEFT_CARD_POSITION_X (COLS-1)/2-COLS/11-12
#define LEFT_CARD_POSITION_Y 5

#define RIGHT_CARD_POSITION_X (COLS-1)/2+COLS/11
#define RIGHT_CARD_POSITION_Y 5

#define PLAYER1_POSITION_X (COLS-1)/2 - COLS/10
#define PLAYER1_POSITION_Y 3
#define PLAYER2_POSITION_X (COLS-1)/2 + COLS/10
#define PLAYER2_POSITION_Y 3

#define PLAYER_1_WIN 0
#define PLAYER_2_WIN 1

#define DEFAULT_COLOR 0
#define WIN_COLOR 1 
#define DEFEAT_COLOR 2

#define CARDS1_QUEUE_POSITION_X (COLS-1)/2 - COLS/5
#define CARDS1_QUEUE_POSITION_Y 3
#define CARDS2_QUEUE_POSITION_X (COLS-1)/2 + COLS/5
#define CARDS2_QUEUE_POSITION_Y 3

/*
────────────── Numeracja Kart (przy standardowej talii) ──────────────
numery 1 ─ 13: trefl, kolejno starszeństwem, 1 ─ dwójka trefl ... 14 ─ as trefl
numery 14 ─ 26: karo, kolejno starszeństwem, 14 ─ dwójka karo ... 26 ─ as karo
numery 27 ─ 39: kier, kolejno starszeństwem, 27 ─ dwójka kier ... 39 ─ as kier
numery 40 ─ 52: pik, kolejno starszeństwem, 40 ─ dwójka pik ... 52 ─ as pik
*/

int DECK_SIZE; //ilość kart w talii

int main()
{
    setlocale(LC_ALL, "");

    //inicjalizacja ncurses
    initscr();
    cbreak();
    // noecho();
    clear();

    srand(time(NULL));

    mvprintw(LINES/2,COLS/2-15,"Podaj rozmiar talii max(99): ");
    refresh();
    char str[3];
    getnstr(str,sizeof(str)-1);
    mvprintw(LINES/2+1,COLS/2-7,"%s",str);
    refresh();

    DECK_SIZE = atoi(str);

    int *deck = (int *) malloc(DECK_SIZE * sizeof(int));
    int *hand1 = (int *) malloc(DECK_SIZE * sizeof(int));
    int *hand2 = (int *) malloc(DECK_SIZE * sizeof(int));

    for(int i=0;i<DECK_SIZE;i++)
    {
        deck[i] = 0;
        hand1[i] = 0;
        hand2[i] = 0;
    }

    initializeDeckWithRandomNumbers(deck);
    splitIntoTwoHands(deck,hand1,hand2);

    playGame(hand1,hand2);

    getch();
    endwin();

   return 0;
}

void drawOutput(int stack1[],int stack2[], int stacks_len, int hand1[], int hand2[])
{
    clear();

    for(int i=0;i<stacks_len;i++)
    {
        mvprintw(PLAYER1_POSITION_Y,PLAYER1_POSITION_X-7,"Gracz 1");
        mvprintw(PLAYER2_POSITION_Y,PLAYER2_POSITION_X,"Gracz 2");
        if(i%2 == 1 && i>0) 
        {
            drawCard(LEFT_CARD_POSITION_Y+2*i,LEFT_CARD_POSITION_X,stack1[i],false,DEFAULT_COLOR);
            drawCard(RIGHT_CARD_POSITION_Y+2*i,RIGHT_CARD_POSITION_X,stack2[i],false,DEFAULT_COLOR);
            continue;
        }

        if(stack1[i]%14 == stack2[i]%14){
            drawCard(LEFT_CARD_POSITION_Y+2*i,LEFT_CARD_POSITION_X,stack1[i],true,DEFAULT_COLOR);
            drawCard(RIGHT_CARD_POSITION_Y+2*i,RIGHT_CARD_POSITION_X,stack2[i],true,DEFAULT_COLOR);
        }else if(stack1[i]%14 > stack2[i]%14)
        {
            drawCard(LEFT_CARD_POSITION_Y+2*i,LEFT_CARD_POSITION_X,stack1[i],true,WIN_COLOR);
            drawCard(RIGHT_CARD_POSITION_Y+2*i,RIGHT_CARD_POSITION_X,stack2[i],true,DEFEAT_COLOR);
        }else{
            drawCard(LEFT_CARD_POSITION_Y+2*i,LEFT_CARD_POSITION_X,stack1[i],true,DEFEAT_COLOR);
            drawCard(RIGHT_CARD_POSITION_Y+2*i,RIGHT_CARD_POSITION_X,stack2[i],true,WIN_COLOR);
        }
    }

    drawCardsQueue(CARDS1_QUEUE_POSITION_Y,CARDS1_QUEUE_POSITION_X,hand1);
    drawCardsQueue(CARDS2_QUEUE_POSITION_Y,CARDS2_QUEUE_POSITION_X,hand2);
    

    mvprintw(LINES/2+5,COLS/2 - 22,"Naciśnij dowolny przycisk aby kontynuować");
    refresh();

    getchar();

}

void drawCardsQueue(int y,int x,int hand[])
{
    for(int i=1;i<DECK_SIZE;i++)
    {
        if(hand[i] == 0)
            return;

        wchar_t *symbols[] = {L"\u2663",L"\u2666",L"\u2665",L"\u2660"}; //kolejno: trefl, karo, kier, pik
        const int symbol_id = (hand[i]/14)%4;
        char num_display;
        switch(hand[i] % 14)
        {
            case 9: // w przypadku, gdy liczba na karcie ma być równa 10, przypisuję X, dalej tworzę osobny przypadek dla 10
                num_display = 'X';
                break;
            case 10:
                num_display = 'J';
                break;
            case 11:
                num_display = 'D';
                break;
            case 12:
                num_display = 'K';
                break;
            case 13:
                num_display = 'A';
                break;
            default:
                num_display = hand[i]%14 + 1 + '0';
        }
        if(num_display=='X')
            mvprintw(y+i,x,"10 %ls",symbols[symbol_id]);
        else
            mvprintw(y+i,x,"%c %ls",num_display,symbols[symbol_id]);
    }
}

void drawCard(int y,int x,int number, short show_card, short color)
{
    wchar_t *symbols[] = {L"\u2663",L"\u2666",L"\u2665",L"\u2660"}; //kolejno: trefl, karo, kier, pik
    const int symbol_id = (number/14)%4;
    
    char num_display;
    switch(number % 14)
    {
        case 9: // w przypadku, gdy liczba na karcie ma być równa 10, przypisuję X, dalej tworzę osobny przypadek dla 10
            num_display = 'X';
            break;
        case 10:
            num_display = 'J';
            break;
        case 11:
            num_display = 'D';
            break;
        case 12:
            num_display = 'K';
            break;
        case 13:
            num_display = 'A';
            break;
        default:
            num_display = number%14 + 1 + '0';
    }
    if(show_card == 0)
    {
        mvaddwstr(y,x,L"┌──────────┐");
        mvaddwstr(y+1,x,L"│##########│");
        mvaddwstr(y+1,x,L"│##########│");
        mvaddwstr(y+1,x,L"│##########│");
        mvaddwstr(y+2,x,L"│##########│");
        mvaddwstr(y+3,x,L"│##########│");
        mvaddwstr(y+4,x,L"│##########│");
        mvaddwstr(y+5,x,L"│##########│");
        mvaddwstr(y+6,x,L"└──────────┘");
    }else{
        start_color();   
        init_pair(DEFEAT_COLOR,COLOR_RED,COLOR_BLACK);
        init_pair(WIN_COLOR,COLOR_GREEN,COLOR_BLACK);
        init_pair(DEFAULT_COLOR,COLOR_WHITE,COLOR_BLACK);
        attron(COLOR_PAIR(color));

        mvaddwstr(y,x,L"┌──────────┐");
        if(num_display == 'X')
            mvprintw(y+1,x,"│ 10       │");
        else
            mvprintw(y+1,x,"│ %c        │",num_display);
        mvaddwstr(y+2,x,L"│          │");
        mvaddwstr(y+3,x,L"│          │");
        mvprintw(y+4,x,"│    %ls     │",symbols[symbol_id]);
        mvaddwstr(y+5,x,L"│          │");
        mvaddwstr(y+6,x,L"│          │");
        if(num_display == 'X')
            mvprintw(y+7,x,"│       10 │");
        else
            mvprintw(y+7,x,"│        %c │",num_display);
        mvaddwstr(y+8,x,L"└──────────┘");

        attroff(COLOR_PAIR(color));
    }
}

void initializeDeckWithRandomNumbers(int deck[])
{
    for(int i=0;i<DECK_SIZE;i++)
    {
        deck[i] = i+1;
    }

    for(int i=0;i<SHUFFLE_SIZE;i++)
    {
        int index1 = rand() % DECK_SIZE;
        int index2 = rand() % DECK_SIZE;
        int copy = deck[index1];
        deck[index1] = deck[index2];
        deck[index2] = copy;
    }
}

void splitIntoTwoHands(int deck[],int hand1[],int hand2[])
{
    hand1[0] = 11;
    hand1[1] = 12;
    hand1[2] = 10;
    hand1[3] = 7;
    hand1[4] = 12;
    hand1[5] = 12;
    hand1[6] = 2;

    hand2[0] = 11;
    hand2[1] = 9;
    hand2[2] = 10;
    hand2[3] = 2;
    // for(int i=0;i<DECK_SIZE;i++)
    // {
    //     if(i<DECK_SIZE/2)
    //         hand1[i] = deck[i];
    //     else
    //         hand2[i%(DECK_SIZE/2)] = deck[i];
    // }
}

void battle(int from[], int to[])
{
    int own_card = to[0];
    int enemy_card = from[0];

    shiftCardLeft(from,1);
    shiftCardLeft(to,1);

    //dodanie kart do tali zwycięskiego gracza
    for(int i=0;i<DECK_SIZE-1;i++)
    {
        if(to[i] == 0)
        {
            to[i] = own_card;
            to[i+1] = enemy_card;
            break;
        }
    }
}

void printHands(int hand1[],int hand2[])
{
    for(int i=0;i<DECK_SIZE;i++)
    {
        wprintf(L"%i",hand1[i]);
        wprintf(L" ");
    }
        wprintf(L"\n");
    for(int i=0;i<DECK_SIZE;i++)
    {
        wprintf(L"%i",hand2[i]);
        wprintf(L" ");
    }
        wprintf(L"\n");
}

void playGame(int hand1[],int hand2[])
{
    short game_over = 0;
    
    while(game_over == 0)
    { 
        drawOutput(hand1,hand2,1,hand1,hand2);
        if(hand1[0]%14 > hand2[0]%14)
        {
            battle(hand2,hand1);
        }else if(hand1[0]%14 < hand2[0]%14)
        {
            battle(hand1,hand2);
        }else{
            if(war(hand1,hand2) == 0)
            {
                game_over = 1;
                break;
            }
        }
        if(hand1[0] == 0 || hand2[0] == 0)
        {
            endGame(hand1[0]==0 ? PLAYER_2_WIN : PLAYER_1_WIN);
            game_over = 1;
            break;
        }
    }
}

void endGame(short winner)
{
    clear();
    if(winner == PLAYER_1_WIN)
    {
        mvprintw(LINES/2+5,COLS/2 - 8,"Gracz 1 wygrał!");
    }else
    {
        mvprintw(LINES/2+5,COLS/2 - 8,"Gracz 2 wygrał!");        
    }
    refresh();
    getchar();
}



void shiftCardLeft(int hand[], int steps)  //przesunięcie kart obu talii o steps w lewo
{
    for(int i=0;i<DECK_SIZE-steps;i++)
    {
        hand[i] = hand[i+steps];
    }
    for(int i=1;i<=steps;i++)
    {
        hand[DECK_SIZE-i] = 0;
    }
    
}

int war(int hand1[], int hand2[])
{
    int war_count = 0;
    int war_ended = 0;
    int *stack1 = (int *) malloc(DECK_SIZE * sizeof(int));
    int *stack2 = (int *) malloc(DECK_SIZE * sizeof(int));
    for(int i=0;i<DECK_SIZE;i++)
    {
        stack1[i] = 0;
        stack2[i] = 0;
    }

    int player_helped = false;
    
    stack1[0] = hand1[0];
    stack2[0] = hand2[0];
    shiftCardLeft(hand1,1);
    shiftCardLeft(hand2,1);

    while(war_ended == false)
    {
        // if(hand1[0] % 14 != hand2[0] % 14){
        //     war_ended = 1;
        //     break;
        // }

        // wprintf(L"Przed \n");
        // printHands(hand1,hand2);
        int card_in_war = 3+war_count*2;
        int player_helped_in_this_turn = false;
        if(card_in_war-1 >= DECK_SIZE)
            return 0;

        if(hand1[card_in_war-1] == 0 || hand2[card_in_war-1] == 0)
        {
            //wariant A
            //return 0;

            //wariant B
            if(player_helped == false) //jeden z graczy może wspomóc drugiego tylko raz podczas wojnie
            {
                player_helped = true;
                player_helped_in_this_turn = true;
                if(hand1[1] == 0)
                {   
                    stack1[card_in_war - 2] = hand2[1];
                    stack1[card_in_war - 1] = hand2[2];
    
                    stack2[card_in_war - 2] = hand2[3];
                    stack2[card_in_war - 1] = hand2[4];

                    shiftCardLeft(hand2,4);
                }else if(hand1[2] == 0)
                {
                    stack1[card_in_war - 2] = hand1[1];
                    stack1[card_in_war - 1] = hand2[1];
    
                    stack2[card_in_war - 2] = hand2[2];
                    stack2[card_in_war - 1] = hand2[3];
                    shiftCardLeft(hand1,1);
                    shiftCardLeft(hand2,3);
                }

                if(hand2[1] == 0)
                {
                    stack2[card_in_war - 2] = hand1[1];
                    stack2[card_in_war - 1] = hand1[2];

                    stack1[card_in_war - 2] = hand1[3];
                    stack1[card_in_war - 1] = hand1[4];

                    shiftCardLeft(hand1,4);
                }else if(hand2[2] == 0)
                {
                    stack2[card_in_war - 2] = hand2[1];
                    stack2[card_in_war - 1] = hand1[1];
                    
                    stack1[card_in_war - 2] = hand1[2];
                    stack1[card_in_war - 1] = hand1[3];
                    shiftCardLeft(hand2,1);
                    shiftCardLeft(hand1,3);
                }
                drawOutput(stack1,stack2,card_in_war,hand1,hand2);
            }else{
                return 0;
            }
            
        }

        if(player_helped_in_this_turn == false)
        {
            //umieszczenie po 2 kart każdego gracza na stosie
            stack1[card_in_war - 2] = hand1[0];
            stack1[card_in_war - 1] = hand1[1];

            stack2[card_in_war - 2] = hand2[0];
            stack2[card_in_war - 1] = hand2[1];

            shiftCardLeft(hand1,1);
            shiftCardLeft(hand2,1);
            drawOutput(stack1,stack2,card_in_war,hand1,hand2);
            shiftCardLeft(hand1,1);
            shiftCardLeft(hand2,1);
        }
        
            
        

        if(stack1[card_in_war - 1] %14 > stack2[card_in_war - 1] % 14)
        {
            for(int i=0;i<DECK_SIZE-1;i++)
            {
                if(hand1[i] == 0)
                {
                    //przekazanie kart graczowi który wygrał
                    for(int j=0;j<card_in_war;j++)
                    {
                        hand1[i+j] = stack1[j];
                    }
                    for(int j=0;j<card_in_war;j++)
                    {
                        hand1[i+card_in_war+j] = stack2[j];
                    }
                    war_ended = 1;
                    break;
                }
            }
        }else if (stack1[card_in_war - 1]%14 < stack2[card_in_war - 1] % 14)
        { 
            for(int i=0;i<DECK_SIZE-1;i++)
            {
                if(hand2[i] == 0)
                {
                    //przekazanie kart graczowi który wygrał
                    for(int j=0;j<card_in_war;j++)
                    {
                        hand2[i+j] = stack2[j];
                    }
                    for(int j=0;j<card_in_war;j++)
                    {
                        hand2[i+card_in_war+j] = stack1[j];
                    }
                    war_ended = 1;
                    break;
                }
            }
        }else{
            war_count++;

        }
        // wprintf(L"\n");
        // wprintf(L"Po \n");
        // printHands(hand1,hand2);
    }
        
}
