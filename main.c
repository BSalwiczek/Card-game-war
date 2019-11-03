#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <time.h>
// #include <ncursesw/ncurses.h>
#include "main.h"

#define DECK_SIZE 52
#define SHUFFLE_SIZE 1000 //wyznacza ile ma zostać wykonanych zamienień w talii
//colored console
#define GREEN  "\x1B[32m"
#define RESET "\x1B[0m"

/*
────────────── Numeracja Kart (przy standardowej talii) ──────────────
numery 1 ─ 13: trefl, kolejno starszeństwem, 1 ─ dwójka trefl ... 14 ─ as trefl
numery 14 ─ 26: karo, kolejno starszeństwem, 14 ─ dwójka karo ... 26 ─ as karo
numery 27 ─ 39: kier, kolejno starszeństwem, 27 ─ dwójka kier ... 39 ─ as kier
numery 40 ─ 52: pik, kolejno starszeństwem, 40 ─ dwójka pik ... 52 ─ as pik
*/


int main()
{
    setlocale(LC_ALL, "");

    //inicjalizacja ncurses
    // initscr();
    // cbreak();
    // noecho();
    // clear();

    srand(time(NULL));
    int deck[DECK_SIZE] = {0};
    int hand1[DECK_SIZE] = {0};
    int hand2[DECK_SIZE] = {0};

    initializeDeckWithRandomNumbers(deck);
    splitIntoTwoHands(deck,hand1,hand2);

    drawCards(9,9,1);
    drawCards(9,9,1);
    //playGame(hand1,hand2);



   return 0;
}

void drawCards(int number1, int number2, const int show_cards)
{
    wchar_t *cards[] = {L"\u2663",L"\u2666",L"\u2665",L"\u2660"}; //kolejno: trefl, karo, kier, pik
    const int symbol_id1 = number1/14; 
    const int symbol_id2 = number2/14;


    number1 %= 14;
    number2 %= 14;

    char num1_display, num2_display;
    switch(number1)
    {
        case 9: // w przypadku, gdy liczba na karcie ma być równa 10, przypisuję X, dalej tworzę osobny przypadek dla 10
            num1_display = 'X';
            break;
        case 10:
            num1_display = 'J';
            break;
        case 11:
            num1_display = 'D';
            break;
        case 12:
            num1_display = 'K';
            break;
        case 13:
            num1_display = 'A';
            break;
        default:
            num1_display = number1 + 1 + '0';
    }
    switch(number2)
    {
        case 9: // w przypadku, gdy liczba na karcie ma być równa 10, przypisuję X, dalej tworzę osobny przypadek dla 10
            num2_display = 'X';
            break;
        case 10:
            num2_display = 'J';
            break;
        case 11:
            num2_display = 'D';
            break;
        case 12:
            num2_display = 'K';
            break;
        case 13:
            num2_display = 'A';
            break;
        default:
            num2_display = number2 + 1 + '0';
    }
    // int maxlines = LINES - 1;
    // int maxcols = COLS - 1;
    // mvaddnwstr(10,10,cards[1],1);
    // refresh();
    // getch();
    // endwin();
    // if(show_cards == 0)
    // {
    //     wprintf(L"       ┌─────────┐");wprintf(L"       ┌─────────┐\n"); 
    //     wprintf(L"       │#########│");wprintf(L"       │#########│\n");
    //     wprintf(L"       │#########|");wprintf(L"       │#########│\n");
    //     wprintf(L"       │#########│");wprintf(L"       │#########│\n");
    //     wprintf(L"       │#########│");wprintf(L"       │#########│\n");
    //     wprintf(L"       │#########│");wprintf(L"       │#########│\n");
    //     wprintf(L"       └─────────┘");wprintf(L"       └─────────┘\n");
    // }else{
    //     wprintf(L""GREEN"       ┌─────────┐");
    //     wprintf(L"       ┌─────────┐\n");
    //     num1_display == 'X' ?  wprintf(L"       │ 10      │") : wprintf(L"       │ %c       │", num1_display);
    //     num2_display == 'X' ?  wprintf(L"       │ 10      │\n") : wprintf(L"       │ %c       │\n", num2_display);
    //     wprintf(L"       │         │");
    //     wprintf(L"       │         │\n");
    //     wprintf(L"\033[F");
    //     wprintf(L"       │    %ls    │",cards[symbol_id1]);
    //     wprintf(L"       │    %ls    │\n",cards[symbol_id2]);
    //     wprintf(L"       │         │");
    //     wprintf(L"       │         │\n");
    //     num1_display == 'X' ?  wprintf(L"       │      10 │") : wprintf(L"       │       %c │", num1_display);
    //     num2_display == 'X'?   wprintf(L"       │      10 │\n") : wprintf(L"       │       %c │\n", num2_display);
    //     wprintf(L"       └─────────┘");                      wprintf(L"       └─────────┘\n" RESET);
    // }
    

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
    for(int i=0;i<DECK_SIZE;i++)
    {
        if(i<DECK_SIZE/2)
            hand1[i] = deck[i];
        else
            hand2[i%(DECK_SIZE/2)] = deck[i];
    }
}

void battle(int from[], int to[])
{
    int own_card = to[0];
    int enemy_card = from[0];
    // printf("%i",own_card);
    // printf(" ");
    // printf("%i",enemy_card);
    // printf("\n");

    shiftCardsLeft(from,to,1);

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

    wprintf(L"\n");
    for(int i=0;i<DECK_SIZE;i++)
    {
        wprintf(L"%i", from[i]);
        wprintf(L" ");
    }
    wprintf(L"\n");
    for(int i=0;i<DECK_SIZE;i++)
    {
        wprintf(L"%i", to[i]);
        wprintf(L" ");
    }
    wprintf(L"\n");
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
        if(hand1[0]%14 > hand2[0]%14)
        {
            drawCards(hand1[0],hand2[0],1);
            battle(hand2,hand1);
        }else if(hand1[0]%14 < hand2[0]%14)
        {
            drawCards(hand1[0],hand2[0],1);
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
            game_over = 1;
            break;
        }
    }
}



void shiftCardsLeft(int hand1[], int hand2[], int steps)  //przesunięcie kart obu talii o steps w lewo
{
    for(int i=0;i<DECK_SIZE-1;i++)
    {
        hand1[i] = hand1[i+steps];
        hand2[i] = hand2[i+steps];
    }
    for(int i=1;i<=steps;i++)
    {
        hand1[DECK_SIZE-i] = 0;
        hand2[DECK_SIZE-i] = 0;
    }
    
}

int war(int hand1[], int hand2[])
{
    //wariant A
    int war_count = 0;
    int war_ended = 0;
    int stack1[DECK_SIZE] = {0};
    int stack2[DECK_SIZE] = {0};
    while(war_ended == 0)
    {
        // if(hand1[0] % 14 != hand2[0] % 14){
        //     war_ended = 1;
        //     break;
        // }

        wprintf(L"Przed \n");
        printHands(hand1,hand2);
        int card_in_war = 3+war_count*2;
        if(card_in_war-1 >= DECK_SIZE)
            return 0;
        if(hand1[card_in_war-1] == 0 || hand2[card_in_war-1] == 0)
            return 0;

        if(card_in_war == 3){
            //umieszczenie po 3 kart każdego gracza na stosie
            stack1[card_in_war - 3] = hand1[0];
            stack1[card_in_war - 2] = hand1[1];
            stack1[card_in_war - 1] = hand1[2];

            stack2[card_in_war - 3] = hand2[0];
            stack2[card_in_war - 2] = hand2[1];
            stack2[card_in_war - 1] = hand2[2];
            shiftCardsLeft(hand1,hand2,3);
        }else
        {
            //umieszczenie po 2 kart każdego gracza na stosie
            stack1[card_in_war - 2] = hand1[0];
            stack1[card_in_war - 1] = hand1[1];

            stack2[card_in_war - 2] = hand2[0];
            stack2[card_in_war - 1] = hand2[1];
            shiftCardsLeft(hand1,hand2,2);
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
        wprintf(L"\n");
        wprintf(L"Po \n");
        printHands(hand1,hand2);
    }
        
}
