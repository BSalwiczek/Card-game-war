#include "draw.h"

void drawOutput(const player_t* const player1,const player_t* const player2, const game_t* const game)
{
    #ifndef SIMULATION_MODE
        clear();

        for(int i=0;i<game->deck_size;i++)
        {
            if(player1->stack[i].number == EMPTY && player2->stack[i].number == EMPTY)
                break;

            mvprintw(PLAYER1_POSITION_Y,PLAYER1_POSITION_X-9,player1->name);
            mvprintw(PLAYER2_POSITION_Y,PLAYER2_POSITION_X+1,player2->name);
            if(i%2 == 1 && i>0) 
            {
                drawCard(LEFT_CARD_POSITION_Y+2*i,LEFT_CARD_POSITION_X,player1->stack[i],false,DEFAULT_COLOR);
                drawCard(RIGHT_CARD_POSITION_Y+2*i,RIGHT_CARD_POSITION_X,player2->stack[i],false,DEFAULT_COLOR);
                continue;
            }

            if(player1->stack[i].number == player2->stack[i].number){
                drawCard(LEFT_CARD_POSITION_Y+2*i,LEFT_CARD_POSITION_X,player1->stack[i],true,DEFAULT_COLOR);
                drawCard(RIGHT_CARD_POSITION_Y+2*i,RIGHT_CARD_POSITION_X,player2->stack[i],true,DEFAULT_COLOR);
            }else if(player1->stack[i].number > player2->stack[i].number)
            {
                drawCard(LEFT_CARD_POSITION_Y+2*i,LEFT_CARD_POSITION_X,player1->stack[i],true,WIN_COLOR);
                drawCard(RIGHT_CARD_POSITION_Y+2*i,RIGHT_CARD_POSITION_X,player2->stack[i],true,DEFEAT_COLOR);
            }else{
                drawCard(LEFT_CARD_POSITION_Y+2*i,LEFT_CARD_POSITION_X,player1->stack[i],true,DEFEAT_COLOR);
                drawCard(RIGHT_CARD_POSITION_Y+2*i,RIGHT_CARD_POSITION_X,player2->stack[i],true,WIN_COLOR);
            }
        }

        if(player1->strategy == none)
            drawCardsQueue(CARDS1_QUEUE_POSITION_Y,CARDS1_QUEUE_POSITION_X,player1->hand,game->deck_size);
        if(player2->strategy == none)
            drawCardsQueue(CARDS2_QUEUE_POSITION_Y,CARDS2_QUEUE_POSITION_X,player2->hand,game->deck_size);
        
        if(game->war_type == wise)
        {
            short showPlayer1Card = (player1->strategy == none ? true : false);
            short showPlayer2Card = (player2->strategy == none ? true : false);

            if(player2->buffor[0].number != EMPTY)
                drawCard((int) LINES* 2/4,COLS/2+10, player2->buffor[0],showPlayer2Card, DEFAULT_COLOR);
            if(player2->buffor[1].number != EMPTY)
                drawCard((int) LINES* 2/4,COLS/2+30, player2->buffor[1],showPlayer2Card, DEFAULT_COLOR);
            if(player2->buffor[0].number != EMPTY && player2->buffor[1].number != EMPTY)
                mvprintw((int) LINES* 2/4 + 10,COLS/2 + 9,"Wybierz kartę (l – lewa, p – prawa)");

            if(player1->buffor[0].number != EMPTY)
                drawCard((int) LINES* 2/4,COLS/2-10, player1->buffor[0],showPlayer1Card, DEFAULT_COLOR);
            if(player1->buffor[1].number != EMPTY)
                drawCard((int) LINES* 2/4,COLS/2-30, player1->buffor[1],showPlayer1Card, DEFAULT_COLOR);
            if(player1->buffor[0].number != EMPTY && player1->buffor[1].number != EMPTY)
                mvprintw((int) LINES* 2/4 + 10,COLS/2 - 31,"Wybierz kartę (l – lewa, p – prawa)");

            if(player1->stack[0].number != EMPTY && player2->stack[0].number != EMPTY)
                mvprintw(LINES/2+15,COLS/2 - 22,"Naciśnij dowolny przycisk aby kontynuować");
            
        }else{
            mvprintw(LINES/2+5,COLS/2 - 22,"Naciśnij dowolny przycisk aby kontynuować");
            getchar();
        }
        
        refresh();
        
    #endif
}

void drawCardsQueue(const int y,const int x,const card_t* const hand, const int deck_size)
{
    for(int i=0;i<deck_size;i++)
    {
        if(hand[i].number == EMPTY)
            return;

        wchar_t *suits[] = {L"\u2663",L"\u2666",L"\u2665",L"\u2660"}; //kolejno: trefl, karo, kier, pik
        int suit_id = determineSuit(hand[i]);
        char num_display = determineSymbol(hand[i]);

        if(num_display=='X')
            mvprintw(y+i,x,"10 %ls",suits[suit_id]);
        else
            mvprintw(y+i,x,"%c %ls",num_display,suits[suit_id]);
    }
}

void drawCard(const int y,const int x,const card_t card, const short show_card, const short color)
{
    if(card.number == EMPTY)
        return ;
    wchar_t *suits[] = {L"\u2663",L"\u2666",L"\u2665",L"\u2660"}; //kolejno: trefl, karo, kier, pik
    int suit_id = determineSuit(card);
    char num_display = determineSymbol(card);
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
        mvaddwstr(y+6,x,L"│##########│");
        mvaddwstr(y+7,x,L"│##########│");
        mvaddwstr(y+8,x,L"└──────────┘");
    }else{
        start_color();   
        init_pair(DEFEAT_COLOR,COLOR_RED,COLOR_BLACK);
        init_pair(WIN_COLOR,COLOR_GREEN,COLOR_BLACK);
        init_pair(DEFAULT_COLOR,COLOR_WHITE,COLOR_BLACK);
        attron(COLOR_PAIR(color));

        mvaddwstr(y,x,L"┌──────────┐");
        num_display == 'X' ? mvprintw(y+1,x,"│ 10       │") : mvprintw(y+1,x,"│ %c        │",num_display);
        mvaddwstr(y+2,x,L"│          │");
        mvaddwstr(y+3,x,L"│          │");
        mvprintw(y+4,x,"│    %ls     │",suits[suit_id]);
        mvaddwstr(y+5,x,L"│          │");
        mvaddwstr(y+6,x,L"│          │");
        num_display == 'X' ? mvprintw(y+7,x,"│       10 │") : mvprintw(y+7,x,"│        %c │",num_display);
        mvaddwstr(y+8,x,L"└──────────┘");

        attroff(COLOR_PAIR(color));
    }
}

char determineSymbol(const card_t card)
{
    switch(card.number)
    {
        case 8: // w przypadku, gdy liczba na karcie ma być równa 10, przypisuję X, dalej tworzę osobny przypadek dla 10
            return 'X';
        case 9: //Jopek
            return 'J';
        case 10: //Dama
            return 'Q';
        case 11: //Król
            return 'K';
        case 12: //As
            return 'A';
        default:
            return card.number + 2 + '0';
    }
}

int determineSuit(const card_t card)
{
    switch(card.suit)
    {
        case clubs:
            return 0;
        case diamonds:
            return 1;
        case hearts:
            return 2;
        case pikes:
            return 3;
    }
}

void drawMenu()
{
    #ifndef SIMULATION_MODE
        clear();
        mvprintw(LINES/2-3,COLS/2-10,"[1] Gra w wojnę");
        mvprintw(LINES/2,COLS/2-10,"[2] Gra w mądrą wojnę");
        mvprintw(LINES/2+3,COLS/2-10,"[3] Wyjście");
        refresh();
    #endif
}

void drawModesMenu()
{
    #ifndef SIMULATION_MODE
        clear();
        mvprintw(LINES/2-3,COLS/2-10,"Wybierz tryb");
        mvprintw(LINES/2,COLS/2-10,"[1] 2 graczy");
        mvprintw(LINES/2+3,COLS/2-10,"[2] gracz vs komputer");
        refresh();
    #endif
}