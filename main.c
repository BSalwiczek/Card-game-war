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
        game->DECK_SIZE = 52;
        game->variant = A;
        game->moves = 0;
        game->war_type = wise;

    #endif

    player_t* player1 = malloc(sizeof(player_t));
    player_t* player2 = malloc(sizeof(player_t));

    if(game->mode == singleplayer){
        initializePlayer(player1, game, "Komputer");
        initializePlayer(player2, game, "Ty");
        player1->strategy = furious;
    }else{
        initializePlayer(player2, game, "Gracz 2");
        initializePlayer(player1, game, "Gracz 1");
    }

    int *deck = (int *) malloc(game->DECK_SIZE * sizeof(int));

    clearArray(deck,game->DECK_SIZE);

    initializeDeckWithRandomNumbers(deck, game);

    splitIntoTwoHands(deck,player1,player2, game);

    #ifdef SIMULATION_MODE
        player1->strategy = peaceful;
        player2->strategy = peaceful;

        FILE *file;
        file = fopen("run/Problem 2 - madra wojna/pvsp2.txt","a");
        if(file == NULL){
            printf("Otwarcie pliku nie powiodło sie\n");
            return 0;
        }
        printf("saving to file...\n");

        fprintf(file, "Deck size,");
        fprintf(file, "Moves,");
        fprintf(file, "Player1Strategy,");
        fprintf(file, "Player2Strategy,");
        // fprintf(file, "Variant,");
        // fprintf(file, "Rank1,");
        // fprintf(file, "Rank2,");
        fprintf(file, "Winner\n");

        for(int i=1;i<1001;i++)
        {
            printf("%i \n",i);
            clearArray(player1->hand,game->DECK_SIZE);
            clearArray(player2->hand,game->DECK_SIZE);
            clearArray(player1->stack,game->DECK_SIZE);
            clearArray(player2->stack,game->DECK_SIZE);

            srand((int) time(NULL)/i);
            initializeDeckWithRandomNumbers(deck, game);

            splitIntoTwoHands(deck,player1,player2, game);

            determineHandRank(player1,game->DECK_SIZE);
            determineHandRank(player2,game->DECK_SIZE);

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

void initializePlayer(player_t* player,game_t* game,char* name)
{
    player->hand = (int *) malloc(game->DECK_SIZE * sizeof(int));
    player->stack = (int *) malloc(game->DECK_SIZE * sizeof(int));  
    player->buffor = (int *) malloc(BUFFOR_SIZE * sizeof(int));
    player->player_status = playing;
    player->name = name;
    player->rank = 0;
    player->strategy = none;

    clearArray(player->hand,game->DECK_SIZE);
    clearArray(player->stack,game->DECK_SIZE);
    clearArray(player->buffor,BUFFOR_SIZE);
}

short startGame(game_t* game)
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
    
    game->DECK_SIZE = atoi(deck_size);

    

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

void determineHandRank(player_t* player, int DECK_SIZE)
{
    // Simulation 1
    // for(int i=0;i<DECK_SIZE;i++)
    // {
    //     if(player->hand[i] % SUIT_SIZE != 12)
    //         player->rank += (player->hand[i] % SUIT_SIZE);
    //     else
    //         player->rank += 30;

    // }

    // Simulation 2
    // for(int i=0;i<DECK_SIZE;i++)
    // {
    //     if(player->hand[i] % SUIT_SIZE == 12)
    //         player->rank += 16;
    //     else if(player->hand[i] % SUIT_SIZE == 11)
    //         player->rank += 8;
    //     else if(player->hand[i] % SUIT_SIZE == 10)
    //         player->rank += 4;
    //     else if(player->hand[i] % SUIT_SIZE == 9)
    //         player->rank += 2;
    //     else if(player->hand[i] % SUIT_SIZE == 8)
    //         player->rank += 1;
    // }

    // Simulation 3
    short streak_len = 0;
    for(int i=0;i<DECK_SIZE;i++)
    {
        if(player->hand[i] % SUIT_SIZE == 12)
            player->rank += 16;
        else if(player->hand[i] % SUIT_SIZE == 11)
            player->rank += 8;
        else if(player->hand[i] % SUIT_SIZE == 10)
            player->rank += 4;
        
        if(player->hand[i] % SUIT_SIZE >= 9)
        {
            streak_len++;
            if(streak_len > 1)
                player->rank++;
        }else
            streak_len = 0;
        
    }

}

void initializeDeckWithRandomNumbers(int deck[], game_t* game)
{
    static int seed = 1;
    (seed > 10000) ? seed=1 : seed++;
    srand(time(NULL)/seed); 

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
    for(int i=0;i<game->DECK_SIZE;i++)
    {
         if(i<game->DECK_SIZE/2)
            player1->hand[i] = deck[i];
         else
             player2->hand[i%(game->DECK_SIZE/2)] = deck[i];
    }
}

void clearArray(int array[], int size)
{
    for(int i=0;i<size;i++)
    {
        array[i] = EMPTY;
    }
}

void playGame(player_t* player1,player_t* player2, game_t* game)
{
    while(true)
    {

        if(game->war_type == normal)
        {
            player1->stack[0] = player1->hand[0];
            player2->stack[0] = player2->hand[0];
            shiftCardLeft(player1->hand,1,game->DECK_SIZE);
            shiftCardLeft(player2->hand,1,game->DECK_SIZE);
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

        if(player1->stack[0]%SUIT_SIZE > player2->stack[0]%SUIT_SIZE)
            giveCardsToWinner(player1,player2,1,game->DECK_SIZE, game->war_type);
        else if(player1->stack[0]%SUIT_SIZE < player2->stack[0]%SUIT_SIZE)
            giveCardsToWinner(player2,player1,1,game->DECK_SIZE, game->war_type);
        else
            war(player1,player2, game);

        if(player1->hand[0] == EMPTY && player1->buffor[0] == EMPTY && player1->buffor[1] == EMPTY)
        {
            player1->player_status = defeat;
            player2->player_status = win;
            endGame(player2,game->moves);
            break;
        }
        if(player2->hand[0] == EMPTY && player2->buffor[0] == EMPTY && player2->buffor[1] == EMPTY)
        {
            player2->player_status = defeat;
            player1->player_status = win;
            endGame(player1,game->moves);
            break;
        }
        // w przypadku zapętlenia rozgrywki
        if(game->moves > 1000000)
        {
            game->moves = INFINITY;
            break;
        }
    }
}

void endGame(player_t* winner, int moves)
{
    #ifndef SIMULATION_MODE
        clear();
        mvprintw(LINES/2+5,COLS/2 - 12,"%s wygrał! Ilosc ruchow %i",winner->name, moves);    
        refresh();
        getch();
    #endif
}

void playWiseTurn(player_t* you,player_t* opponent,player_t* player1,player_t* player2, game_t* game)
{
    if(opponent->buffor[0] != EMPTY || opponent->buffor[1] != EMPTY)
    {
        if(opponent->buffor[0] != EMPTY)
        {
            opponent->stack[0] = opponent->buffor[0];
            opponent->buffor[0] = EMPTY;
        }else{
            opponent->stack[0] = opponent->buffor[1];
            opponent->buffor[1] = EMPTY;
        }
    }else{
        opponent->stack[0] = opponent->hand[0];
        shiftCardLeft(opponent->hand,1,game->DECK_SIZE);

    }

    drawOutput(player1,player2,game);

    if(you->buffor[0]==EMPTY && you->buffor[1] == EMPTY)
    {
        you->buffor[0] = you->hand[0];
        you->buffor[1] = you->hand[1];
        
        shiftCardLeft(you->hand,2,game->DECK_SIZE);

        drawOutput(player1,player2,game);

        char card = chooseCard(you,opponent->stack,player2);
        if(card == 'l'){
            if(you == player2)
            {
                you->stack[0] = you->buffor[0];
                you->buffor[0] = EMPTY;
            }else{
                you->stack[0] = you->buffor[1];
                you->buffor[1] = EMPTY;
            }
        }
        if(card == 'p'){
            if(you == player2)
            {
                you->stack[0] = you->buffor[1];
                you->buffor[1] = EMPTY;
            }else{
                you->stack[0] = you->buffor[0];
                you->buffor[0] = EMPTY;
            }                 
        }
    }
    // else{
    //     if(you->buffor[0] != EMPTY)
    //     {
    //         you->stack[0] = you->buffor[0];
    //         you->buffor[0] = EMPTY;
    //     }else{
    //         you->stack[0] = you->buffor[1];
    //         you->buffor[1] = EMPTY;
    //     }
    // }
    // drawOutput(opponent,you,game);
}

char chooseCard(player_t* you, int opponent_stack[], player_t* player2)
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

        int opponent_card = opponent_stack[0]%SUIT_SIZE;
        int left_card = you->buffor[0]%SUIT_SIZE;
        int right_card = you->buffor[1]%SUIT_SIZE;

        //random
        if(you->strategy == random_choice)
        {
            if((int)rand()%2 == 0)
                return left;
            else
                return right;
        }else if(you->strategy == furious)
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
            
        }else if(you->strategy == peaceful)
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

char tacticalChose(player_t* you, int opponent_stack[], player_t* player2)
{
    
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

        if(player1->hand[0] == EMPTY || player2->hand[0] == EMPTY || ((player1->hand[1] == EMPTY || player2->hand[1] == EMPTY ) && game->war_type == normal ))
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
                    variantBsplitCards(player1,player2,cards_in_war,game->DECK_SIZE);
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
            if(player1->buffor[0] != EMPTY || player1->buffor[1] != EMPTY)
            {
                if(player1->buffor[0] != EMPTY)
                {
                    player1->stack[cards_in_war - 2] = player1->buffor[0];
                    player1->buffor[0] = EMPTY;
                }else{
                    player1->stack[cards_in_war - 2] = player1->buffor[1];
                    player1->buffor[1] = EMPTY;
                }

                player2->stack[cards_in_war - 2] = player2->hand[0];
                player2->stack[cards_in_war - 1] = player2->hand[1];
                shiftCardLeft(player2->hand,2,game->DECK_SIZE);

                if(player1->hand[0] == EMPTY){
                    drawOutput(player1,player2,game);
                    if(game->war_type == wise)
                        getch();
                    return 0;
                }
                player1->stack[cards_in_war - 1] = player1->hand[0];
                shiftCardLeft(player1->hand,1,game->DECK_SIZE);

            }else if(player2->buffor[0] != EMPTY || player2->buffor[1] != EMPTY)
            {
                if(player2->buffor[0] != EMPTY)
                {
                    player2->stack[cards_in_war - 2] = player2->buffor[0];
                    player2->buffor[0] = EMPTY;
                }else{
                    player2->stack[cards_in_war - 2] = player2->buffor[1];
                    player2->buffor[1] = EMPTY;
                }

                player1->stack[cards_in_war - 2] = player1->hand[0];
                player1->stack[cards_in_war - 1] = player1->hand[1];
                shiftCardLeft(player1->hand,2,game->DECK_SIZE);

                if(player2->hand[0] == EMPTY){
                    drawOutput(player1,player2,game);
                    if(game->war_type == wise)
                        getch();
                    return 0;
                }
                player2->stack[cards_in_war - 1] = player2->hand[0];
                shiftCardLeft(player2->hand,1,game->DECK_SIZE);

            }else{
                player1->stack[cards_in_war - 2] = player1->hand[0];
                player1->stack[cards_in_war - 1] = player1->hand[1];

                player2->stack[cards_in_war - 2] = player2->hand[0];
                player2->stack[cards_in_war - 1] = player2->hand[1];

                shiftCardLeft(player1->hand,2,game->DECK_SIZE);
                shiftCardLeft(player2->hand,2,game->DECK_SIZE);
            }

            game->moves += 2;
            
            drawOutput(player1,player2,game);
            if(game->war_type == wise)
                getch();
        }

        if(player1->stack[cards_in_war - 1] %SUIT_SIZE > player2->stack[cards_in_war - 1] % SUIT_SIZE)
        {
            giveCardsToWinner(player1,player2,cards_in_war,game->DECK_SIZE, game->war_type);
            war_ended = 1;
            break;
        }else if (player1->stack[cards_in_war - 1]%SUIT_SIZE < player2->stack[cards_in_war - 1] % SUIT_SIZE)
        {
            giveCardsToWinner(player2,player1,cards_in_war,game->DECK_SIZE, game->war_type);
            war_ended = 1;
            break;
        }else
            war_count++;
    }
        
}

void giveCardsToWinner(player_t* winner, player_t* looser, short cards_in_war, int DECK_SIZE, war_type_t war_type)
{
    //losowo dla gry w madra wojne
    if(war_type == wise)
    {
        int index1,index2,copy;
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
            clearArray(winner->stack,DECK_SIZE);
            clearArray(looser->stack,DECK_SIZE);
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
        player1->stack[cards_in_war - 2] = player1->hand[2];
        player1->stack[cards_in_war - 1] = player1->hand[3];

        player2->stack[cards_in_war - 2] = player1->hand[0];
        player2->stack[cards_in_war - 1] = player1->hand[1];
        shiftCardLeft(player1->hand,4,DECK_SIZE);
    }else if(player2->hand[1] == EMPTY)
    {
        player1->stack[cards_in_war - 2] = player1->hand[0];
        player1->stack[cards_in_war - 1] = player1->hand[2];
        
        player2->stack[cards_in_war - 2] = player2->hand[0];
        player2->stack[cards_in_war - 1] = player1->hand[1];
    
        shiftCardLeft(player2->hand,1,DECK_SIZE);
        shiftCardLeft(player1->hand,3,DECK_SIZE);
    }
}

void saveResults(player_t* player1, player_t* player2, game_t* game,FILE *file)
{
    fprintf(file, "%i,",game->DECK_SIZE);
    fprintf(file, "%i,",game->moves);
    // fprintf(file, "%i,",game->variant);
    // fprintf(file, "%i,",player1->rank);
    // fprintf(file, "%i,",player2->rank);
    fprintf(file, "%i,",player1->strategy);
    fprintf(file, "%i,",player2->strategy);
    fprintf(file, "%i",player1->player_status == win);
    fprintf(file,"\n");

}
