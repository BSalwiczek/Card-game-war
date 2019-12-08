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

    game_t* game = malloc(sizeof(game_t));

    #ifndef SIMULATION_MODE
        //inicjalizacja ncurses
        initscr();
        cbreak();
        clear();

        if(startGame(game) == 0)
        {
            clear();
            endwin();
            return 0;
        }
    #else
        game->deck_size = 20;
        game->variant = A;
        game->moves = 0;
        game->war_type = normal;

    #endif

    player_t* player1 = malloc(sizeof(player_t));
    player_t* player2 = malloc(sizeof(player_t));

    if(game->mode == singleplayer){
        initializePlayer(player1, game->deck_size, "Komputer");
        initializePlayer(player2, game->deck_size, "Gracz");
        player1->strategy = furious;
    }else{
        initializePlayer(player2, game->deck_size, "Gracz 2");
        initializePlayer(player1, game->deck_size, "Gracz 1");
    }

    card_t *deck = (card_t *) malloc(game->deck_size * sizeof(card_t));

    initializeDeckWithRandomNumbers(deck, game->deck_size);
    splitIntoTwoHands(deck,player1,player2, game->deck_size);

    #ifdef SIMULATION_MODE
        // player1->strategy = peaceful;
        // player2->strategy = peaceful;

        FILE *file;
        file = fopen("run/test.txt","a");
        if(file == NULL){
            printf("Otwarcie pliku nie powiodło sie\n");
            return 0;
        }
        printf("saving to file...\n");

        fprintf(file, "Deck size,");
        fprintf(file, "Moves,");
        fprintf(file, "Variant,");
        fprintf(file, "Rank1,");
        fprintf(file, "Rank2,");
        fprintf(file, "Player1Strategy,");
        fprintf(file, "Player2Strategy,");
        fprintf(file, "Winner\n");

        for(int i=1;i<100000;i++)
        {
            // printf("%i \n",i);
            clearCards(player1->hand,game->deck_size);
            clearCards(player2->hand,game->deck_size);
            clearCards(player1->stack,game->deck_size);
            clearCards(player2->stack,game->deck_size);

            srand((int) time(NULL)/i);
            initializeDeckWithRandomNumbers(deck, game->deck_size);

            splitIntoTwoHands(deck,player1,player2, game->deck_size);

            determineHandRank(player1,game->deck_size);
            determineHandRank(player2,game->deck_size);

            playGame(player1,player2, game);
            saveResults(player1,player2,game,file);

            game->moves = 0;
            player1->rank = 0;
            player2->rank = 0;
        }
        fclose(file);

    #else
        playGame(player1,player2, game);
        endwin();
    #endif

    free(game);
    free(player1);
    free(player2);
    free(deck);
    return 0;
}

void initializePlayer(player_t* const player, const int deck_size,const char* name)
{
    player->hand = (card_t *) malloc(deck_size * sizeof(card_t));
    player->stack = (card_t *) malloc(deck_size * sizeof(card_t));  
    player->buffor = (card_t *) malloc(BUFFOR_SIZE * sizeof(card_t));
    player->player_status = playing;
    player->name = name;
    player->rank = 0;
    player->strategy = none;

    clearCards(player->hand,deck_size);
    clearCards(player->stack,deck_size);
    clearCards(player->buffor,BUFFOR_SIZE);
}

short startGame(game_t* const game)
{
    drawMenu();
    
    while(true)
    {
        char x = getch();
        if(x == '3')
            return 0;
        
        if(x == '1')
        {
            game->war_type = normal;
            break;
        }
        if(x == '2')
        {
            game->war_type = wise;
            break;
        }
    }

    char deck_size[3];

    while(true)
    {
        clear();
        mvprintw(LINES/2,COLS/2-15,"Podaj rozmiar talii(max: 52): ");
        refresh();
        getnstr(deck_size,sizeof(deck_size)-1);

        if(atoi(deck_size) <= 52)
            break;
    }
    
    game->deck_size = atoi(deck_size);

    if(game->war_type == normal)
    {
        while(true)
        {
            clear();
            mvprintw(LINES/2,COLS/2-15,"Wybierz wariant (A lub B): ");
            refresh();
            char variant = getch();
            if(variant == 'A' || variant == 'a')
            {
                game->variant = A;
                break;
            }else if(variant == 'B' || variant == 'b')
            {
                game->variant = B;
                break;
            }
        }
        game->mode = multiplayer;
    }else
    {
        while(true)
        {
            drawModesMenu();
            char mode = getch();
            if(mode == '1')
            {
                game->mode = multiplayer;
                break;
            }else if(mode == '2')
            {
                game->mode = singleplayer;
                break;
            }
        }
        game->variant = A;
    }
    game->moves = 0;
    return 1;
}

void determineHandRank(player_t* const player, const int deck_size)
{
    // Simulation 1
    // for(int i=0;i<deck_size;i++)
    // {
    //     if(player->hand[i].number != 12)
    //         player->rank += (player->hand[i].number);
    //     else
    //         player->rank += 30;

    // }

    // Simulation 2
    // for(int i=0;i<deck_size;i++)
    // {
    //     if(player->hand[i].number == 12)
    //         player->rank += 16;
    //     else if(player->hand[i].number == 11)
    //         player->rank += 8;
    //     else if(player->hand[i].number == 10)
    //         player->rank += 4;
    //     else if(player->hand[i].number == 9)
    //         player->rank += 2;
    //     else if(player->hand[i].number == 8)
    //         player->rank += 1;
    // }

    // Simulation 3
    short streak_len = 0;
    for(int i=0;i<deck_size;i++)
    {
        if(player->hand[i].number == 12)
            player->rank += 16;
        else if(player->hand[i].number == 11)
            player->rank += 8;
        else if(player->hand[i].number == 10)
            player->rank += 4;
        
        if(player->hand[i].number >= 9)
        {
            streak_len++;
            if(streak_len > 1)
                player->rank++;
        }else
            streak_len = 0;
        
    }

}

void initializeDeckWithRandomNumbers(card_t* const deck, const int deck_size)
{
    static int seed = 1;
    (seed > 10000) ? seed=1 : seed++;
    srand(time(NULL)/seed); 

    for(int i=0;i<deck_size;i++)
    {
        if(i<deck_size/4){
            deck[i].number = (SUIT_SIZE-1-i)%13;
            deck[i].suit = clubs;
        }
        else if(i<deck_size/2)
        {
            deck[i].number  = (2*SUIT_SIZE-1-i + deck_size/4)%13;
            deck[i].suit = diamonds;
        }
        else if(i<deck_size*3/4)
        {
            deck[i].number  = (3*SUIT_SIZE-1-i + deck_size/2)%13;
            deck[i].suit = hearts;
        }
        else
        {
            deck[i].number  = (4*SUIT_SIZE-1-i + deck_size*3/4)%13;
            deck[i].suit = pikes;
        }
            
    }

    for(int i=0;i<SHUFFLE_SIZE;i++)
    {
        short index1 = rand() % deck_size;
        short index2 = rand() % deck_size;
        card_t copy = deck[index1];
        deck[index1] = deck[index2];
        deck[index2] = copy;
    }

}

void splitIntoTwoHands(const card_t* const deck,player_t* const player1,player_t* const player2, const int deck_size)
{
    for(int i=0;i<deck_size;i++)
    {
        if(i<deck_size/2)
            player1->hand[i] = deck[i];
        else
            player2->hand[i%(deck_size/2)] = deck[i];
    }
}

void clearCards(card_t* const cards, const int size)
{
    for(int i=0;i<size;i++)
    {
        cards[i].number = EMPTY;
        cards[i].suit = empty;
    }
}

void playGame(player_t* const player1,player_t* const player2, game_t* const game)
{
    while(true)
    {
        if(game->war_type == normal)
        {
            player1->stack[0] = player1->hand[0];
            player2->stack[0] = player2->hand[0];
            shiftCardLeft(player1->hand,1,game->deck_size);
            shiftCardLeft(player2->hand,1,game->deck_size);
            drawOutput(player1,player2,game);
        }else{
            if(game->moves % 2 == 0)
                playWiseTurn(player1,player2,player1,player2,game);
            else
                playWiseTurn(player2,player1,player1,player2,game);
            drawOutput(player1,player2,game);
            #ifndef SIMULATION_MODE
                getch();
            #endif            
        }

        game->moves++;

        if(player1->stack[0].number > player2->stack[0].number)
            giveCardsToWinner(player1,player2,1,game->deck_size, game->war_type);
        else if(player1->stack[0].number < player2->stack[0].number)
            giveCardsToWinner(player2,player1,1,game->deck_size, game->war_type);
        else
            war(player1,player2, game);

        short player2win = player1->hand[0].number == EMPTY && player1->buffor[0].number == EMPTY && player1->buffor[1].number == EMPTY;
        short player1win = player2->hand[0].number == EMPTY && player2->buffor[0].number == EMPTY && player2->buffor[1].number == EMPTY;
        if(player1win && player2win)
        {
            player1->player_status = win;
            player2->player_status = win;
            endGame(NULL,game->moves);
            break;
        }
        if(player2win)
        {
            player1->player_status = defeat;
            player2->player_status = win;
            endGame(player2,game->moves);
            break;
        }
        if(player1win)
        {
            player2->player_status = defeat;
            player1->player_status = win;
            endGame(player1,game->moves);
            break;
        }
        
        if(game->moves > 1000000) // w przypadku zapętlenia rozgrywki
        {
            game->moves = INFINITY;
            break;
        }
    }
}

void endGame(const player_t* const winner, const int moves)
{
    #ifndef SIMULATION_MODE
        getch();
        clear();
        if(winner == NULL)
            mvprintw(LINES/2+5,COLS/2 - 12,"Remis! Ilosc ruchow %i", moves);    
        else
            mvprintw(LINES/2+5,COLS/2 - 12,"%s wygrał! Ilosc ruchow %i",winner->name, moves);    
        refresh();
        getch();
    #endif
}

void playWiseTurn(player_t* const you,player_t* const opponent,player_t* const player1,player_t* const player2, game_t* const game)
{
    if(opponent->buffor[0].number != EMPTY || opponent->buffor[1].number != EMPTY)
    {
        if(opponent->buffor[0].number != EMPTY)
        {
            opponent->stack[0] = opponent->buffor[0];
            clearCard(&opponent->buffor[0]);
        }else{
            opponent->stack[0] = opponent->buffor[1];
            clearCard(&opponent->buffor[1]);
        }
    }else{
        opponent->stack[0] = opponent->hand[0];
        shiftCardLeft(opponent->hand,1,game->deck_size);

    }

    drawOutput(player1,player2,game);

    if(you->buffor[0].number==EMPTY && you->buffor[1].number == EMPTY)
    {
        you->buffor[0] = you->hand[0];
        you->buffor[1] = you->hand[1];
        
        shiftCardLeft(you->hand,2,game->deck_size);

        drawOutput(player1,player2,game);

        char card = chooseCard(you,opponent->stack,player2);
        if(card == 'l'){
            if(you == player2)
            {
                you->stack[0] = you->buffor[0];
                clearCard(&you->buffor[0]);
            }else{
                you->stack[0] = you->buffor[1];
                clearCard(&you->buffor[1]);
            }
        }
        if(card == 'p'){
            if(you == player2)
            {
                you->stack[0] = you->buffor[1];
                clearCard(&you->buffor[1]);
            }else{
                you->stack[0] = you->buffor[0];
                clearCard(&you->buffor[0]);
            }                 
        }
    }
}

char chooseCard(const player_t* const you, const card_t* const opponent_stack, const player_t* const player2)
{
    if(you->strategy == none)
    {
        while(true)
        {
            char card;
            card = getch();
            if(card == 'l' || card=='p'){
                return card;
            }
        }
    }else{
        static int seed = 1;
        (seed > 10000) ? seed=1 : seed++;
        srand(time(NULL)/seed);

        char left, right;
        if(you == player2)
        {
            left = 'l';
            right = 'p';
        }else{ //przeciwnie dla gracza po lewej 
            left = 'p';
            right = 'l';
        }

        int opponent_card = opponent_stack[0].number;
        int left_card = you->buffor[0].number;
        int right_card = you->buffor[1].number;

        if(you->strategy == random_choice) //strategia losowa
        {
            if((int)rand()%2 == 0)
                return left;
            else
                return right;
        }else if(you->strategy == furious) //strategia wściekła
        {
            //jeśli może być wojna to trzeba do niej doprowadzić
            if(opponent_card == left_card)
                return left;
            if(opponent_card == right_card)
                return right;
            //skoro nie może być wojny postaraj się przebić młodszą kartą
            if(opponent_card < left_card)
            {
                if(left_card < right_card)
                    return left;
                if(opponent_card > right_card)
                    return left;
            }
            if(opponent_card < right_card)
            {
                if(right_card < left_card)
                    return right;
                if(opponent_card > left_card)
                    return right;
            }
            //jeśli obie karty są zbyt słabe by wygrać połóż młodszą 
            if(left_card > right_card)
                return right;
            else
                return left;
            
        }else if(you->strategy == peaceful) //strategia pokojowa
        {
            //jeśli może być wojna to to do niej nie doprowadź
            if(opponent_card == left_card)
                return right;
            if(opponent_card == right_card)
                return left;
            //skoro nie może być wojny postaraj nie przebijać
            if(opponent_card > left_card && opponent_card > right_card)
            {
                if((int)rand()%2 == 0)
                    return left;
                else
                    return right;
            }else if(opponent_card > left_card)
                return left;
            else if(opponent_card > right_card)
                return right;
            //jeśli musisz przebić zrób to słabszą kartą
            if(left_card < right_card)
                return left;
            else
                return right;
        }
    }
}

void shiftCardLeft(card_t* const hand, const int steps, const int deck_size)  //przesunięcie kart talii o steps w lewo
{
    for(int i=0;i<deck_size-steps;i++)
    {
        hand[i] = hand[i+steps];
    }
    for(int i=1;i<=steps;i++)
    {
        clearCard(&hand[deck_size-i]);
    }
    
}

int war(player_t* const player1, player_t* const player2, game_t* const game)
{
    int war_count = 0;
    short war_ended = false;
    short player_helped = false;

    while(war_ended == false)
    {
        int cards_in_war = 3+war_count*2;
        int player_helped_in_this_turn = false;

        if(cards_in_war-1 >= game->deck_size)
            return 0;

        if(player1->hand[0].number == EMPTY || player2->hand[0].number == EMPTY || ((player1->hand[1].number == EMPTY || player2->hand[1].number == EMPTY ) && game->war_type == normal ))
        {
            //wariant A
            if(game->variant == A)
                return 0;
            else{
                //wariant B
                if(player_helped == false) //jeden z graczy może wspomóc drugiego tylko raz podczas wojnie
                {
                    player_helped = true;
                    player_helped_in_this_turn = true;
                    variantBsplitCards(player1,player2,cards_in_war,game->deck_size);
                    game->moves += 2;
                    drawOutput(player1,player2,game);
                }else{
                    return 0;
                }
            }
        }

        if(player_helped_in_this_turn == false)
        {
            //umieszczenie po 2 kart każdego gracza na stosie
            if(player1->buffor[0].number != EMPTY || player1->buffor[1].number != EMPTY) //mądra wojna
            {
                if(putCardsOnStackWise(player1, player2, game, cards_in_war)==0)
                    return 0;
            }else if(player2->buffor[0].number != EMPTY || player2->buffor[1].number != EMPTY) //mądra wojna
            {
                if(putCardsOnStackWise(player2, player1, game, cards_in_war)==0)
                    return 0;
            }else{ // normalna wojna
                player1->stack[cards_in_war - 2] = player1->hand[0];
                player1->stack[cards_in_war - 1] = player1->hand[1];

                player2->stack[cards_in_war - 2] = player2->hand[0];
                player2->stack[cards_in_war - 1] = player2->hand[1];

                shiftCardLeft(player1->hand,2,game->deck_size);
                shiftCardLeft(player2->hand,2,game->deck_size);
            }

            game->moves += 2;
            
            drawOutput(player1,player2,game);
            if(game->war_type == wise)
                getch();
        }

        if(player1->stack[cards_in_war - 1].number > player2->stack[cards_in_war - 1].number % SUIT_SIZE)
        {
            giveCardsToWinner(player1,player2,cards_in_war,game->deck_size, game->war_type);
            war_ended = 1;
            break;
        }else if (player1->stack[cards_in_war - 1].number < player2->stack[cards_in_war - 1].number % SUIT_SIZE)
        {
            giveCardsToWinner(player2,player1,cards_in_war,game->deck_size, game->war_type);
            war_ended = 1;
            break;
        }else
            war_count++;
    }
        
}

int putCardsOnStackWise(player_t* const chooser, player_t* const other, game_t* const game, const int cards_in_war)
{
    if(chooser->buffor[0].number != EMPTY)
    {
        chooser->stack[cards_in_war - 2] = chooser->buffor[0];
        clearCard(&chooser->buffor[0]);
    }else{
        chooser->stack[cards_in_war - 2] = chooser->buffor[1];
        clearCard(&chooser->buffor[1]);
    }

    other->stack[cards_in_war - 2] = other->hand[0];
    other->stack[cards_in_war - 1] = other->hand[1];
    shiftCardLeft(other->hand,2,game->deck_size);

    if(chooser->hand[0].number == EMPTY){
        drawOutput(chooser,other,game);
        if(game->war_type == wise)
            getch();
        return 0;
    }
    chooser->stack[cards_in_war - 1] = chooser->hand[0];
    shiftCardLeft(chooser->hand,1,game->deck_size);
    return 1;
}

void clearCard(card_t* const card)
{
    card->number = EMPTY;
    card->suit = empty;
}

void giveCardsToWinner(player_t* const winner, player_t* const looser, const short cards_in_war, const int deck_size, const war_type_t war_type)
{
    //losowo dla gry w madra wojne
    if(war_type == wise)
    {
        int index1,index2;
        card_t copy;
        static int seed = 1;
        (seed > 10000) ? seed=1 : seed++;
        srand(time(NULL)/seed);
        if(cards_in_war == 1)
        {
            if(rand() % 2 == 0)
            {
                copy = winner->stack[0];
                winner->stack[0] = looser->stack[0];
                looser->stack[0] = copy;
            }
        }else{
            for(int i=0;i<SHUFFLE_SIZE;i++)
            {
                index1 = rand() % cards_in_war;
                index2 = rand() % cards_in_war;
                copy = winner->stack[index2];
                winner->stack[index2] = looser->stack[index1];
                looser->stack[index1] = copy;
            }
        }
    }

    for(int i=0;;i++)
    {
        if(winner->hand[i].number == EMPTY)
        {
            for(int j=0;j<cards_in_war;j++) //przekazanie kart graczowi który wygrał
            {
                winner->hand[i+j] = winner->stack[j];
                winner->hand[i+cards_in_war+j] = looser->stack[j];
            }
            clearCards(winner->stack,deck_size);
            clearCards(looser->stack,deck_size);
            break;
        }
    }
}

void variantBsplitCards(player_t* const player1, player_t* const player2, const int cards_in_war, const int deck_size)
{
    if(player1->hand[0].number == EMPTY)
    {   
        player1->stack[cards_in_war - 2] = player2->hand[0];
        player1->stack[cards_in_war - 1] = player2->hand[1];

        player2->stack[cards_in_war - 2] = player2->hand[2];
        player2->stack[cards_in_war - 1] = player2->hand[3];

        shiftCardLeft(player2->hand,4,deck_size);
    }else if(player1->hand[1].number == EMPTY)
    {
        player1->stack[cards_in_war - 2] = player1->hand[0];
        player1->stack[cards_in_war - 1] = player2->hand[1];

        player2->stack[cards_in_war - 2] = player2->hand[0];
        player2->stack[cards_in_war - 1] = player2->hand[2];
        shiftCardLeft(player1->hand,1,deck_size);
        shiftCardLeft(player2->hand,3,deck_size);
    }

    if(player2->hand[0].number == EMPTY)
    {
        player1->stack[cards_in_war - 2] = player1->hand[2];
        player1->stack[cards_in_war - 1] = player1->hand[3];

        player2->stack[cards_in_war - 2] = player1->hand[0];
        player2->stack[cards_in_war - 1] = player1->hand[1];
        shiftCardLeft(player1->hand,4,deck_size);
    }else if(player2->hand[1].number == EMPTY)
    {
        player1->stack[cards_in_war - 2] = player1->hand[0];
        player1->stack[cards_in_war - 1] = player1->hand[2];
        
        player2->stack[cards_in_war - 2] = player2->hand[0];
        player2->stack[cards_in_war - 1] = player1->hand[1];
    
        shiftCardLeft(player2->hand,1,deck_size);
        shiftCardLeft(player1->hand,3,deck_size);
    }
}

void saveResults(const player_t* const player1, const player_t* const player2, const game_t* const game,FILE* const file)
{
    fprintf(file, "%i,",game->deck_size);
    fprintf(file, "%i,",game->moves);
    fprintf(file, "%i,",game->variant);
    fprintf(file, "%i,",player1->rank);
    fprintf(file, "%i,",player2->rank);
    fprintf(file, "%i,",player1->strategy);
    fprintf(file, "%i,",player2->strategy);
    if(player1->player_status == win && player2->player_status == win)
        fprintf(file, "%i",-1);
    else
        fprintf(file, "%i",player1->player_status == win);
    fprintf(file,"\n");

}
