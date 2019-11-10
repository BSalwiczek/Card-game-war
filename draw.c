#include "draw.h"

void drawOutput(player_t player1[],player_t player2[], int DECK_SIZE)
{
    clear();

    for(int i=0;i<DECK_SIZE;i++)
    {
        if(player1->stack[i] == EMPTY)
            break;

        mvprintw(PLAYER1_POSITION_Y,PLAYER1_POSITION_X-7,"Gracz 1");
        mvprintw(PLAYER2_POSITION_Y,PLAYER2_POSITION_X,"Gracz 2");
        if(i%2 == 1 && i>0) 
        {
            drawCard(LEFT_CARD_POSITION_Y+2*i,LEFT_CARD_POSITION_X,player1->stack[i],true,DEFAULT_COLOR); //false
            drawCard(RIGHT_CARD_POSITION_Y+2*i,RIGHT_CARD_POSITION_X,player2->stack[i],true,DEFAULT_COLOR); //false
            continue;
        }

        if(player1->stack[i]%SUIT_SIZE == player2->stack[i]%SUIT_SIZE){
            drawCard(LEFT_CARD_POSITION_Y+2*i,LEFT_CARD_POSITION_X,player1->stack[i],true,DEFAULT_COLOR);
            drawCard(RIGHT_CARD_POSITION_Y+2*i,RIGHT_CARD_POSITION_X,player2->stack[i],true,DEFAULT_COLOR);
        }else if(player1->stack[i]%SUIT_SIZE > player2->stack[i]%SUIT_SIZE)
        {
            drawCard(LEFT_CARD_POSITION_Y+2*i,LEFT_CARD_POSITION_X,player1->stack[i],true,WIN_COLOR);
            drawCard(RIGHT_CARD_POSITION_Y+2*i,RIGHT_CARD_POSITION_X,player2->stack[i],true,DEFEAT_COLOR);
        }else{
            drawCard(LEFT_CARD_POSITION_Y+2*i,LEFT_CARD_POSITION_X,player1->stack[i],true,DEFEAT_COLOR);
            drawCard(RIGHT_CARD_POSITION_Y+2*i,RIGHT_CARD_POSITION_X,player2->stack[i],true,WIN_COLOR);
        }
    }

    drawCardsQueue(CARDS1_QUEUE_POSITION_Y,CARDS1_QUEUE_POSITION_X,player1->hand,DECK_SIZE);
    drawCardsQueue(CARDS2_QUEUE_POSITION_Y,CARDS2_QUEUE_POSITION_X,player2->hand,DECK_SIZE);
    

    mvprintw(LINES/2+5,COLS/2 - 22,"Naciśnij dowolny przycisk aby kontynuować");
    refresh();

    getchar();

}

void drawCardsQueue(int y,int x,int hand[], int DECK_SIZE)
{
    for(int i=0;i<DECK_SIZE;i++)
    {
        if(hand[i] == EMPTY)
            return;

        wchar_t *symbols[] = {L"\u2663",L"\u2666",L"\u2665",L"\u2660"}; //kolejno: trefl, karo, kier, pik
        const int symbol_id = (hand[i]/SUIT_SIZE)%4;
        char num_display;
        determineNumber(hand[i],&num_display);
        if(num_display=='X')
            mvprintw(y+i,x,"10 %ls",symbols[symbol_id]);
        else
            mvprintw(y+i,x,"%c %ls",num_display,symbols[symbol_id]);
    }
}

void drawCard(int y,int x,int number, short show_card, short color)
{
    wchar_t *symbols[] = {L"\u2663",L"\u2666",L"\u2665",L"\u2660"}; //kolejno: trefl, karo, kier, pik
    const int symbol_id = (number/SUIT_SIZE)%4;
    char num_display;
    determineNumber(number,&num_display);
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

void determineNumber(int number,char *num_display)
{

    switch(number % SUIT_SIZE)
    {
        case 8: // w przypadku, gdy liczba na karcie ma być równa 10, przypisuję X, dalej tworzę osobny przypadek dla 10
            *num_display = 'X';
            break;
        case 9: //Jopek
            *num_display = 'J';
            break;
        case 10: //Dama
            *num_display = 'D';
            break;
        case 11: //Król
            *num_display = 'K';
            break;
        case 12: //As
            *num_display = 'A';
            break;
        default:
            *num_display = number%SUIT_SIZE + 2 + '0';
    }
}