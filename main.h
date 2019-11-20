#define SHUFFLE_SIZE 1000 //wyznacza ile ma zostać wykonanych zamienień w talii
#define SUIT_SIZE 13
#define EMPTY -1
#define SIMULATION_MODE 0
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
    peaceful
} strategy_t;

typedef enum{
    normal,
    wise
} war_type_t;

typedef struct {
    int *hand;
    int rank;
    int *stack;
    int *buffor;
    char *name;
    status_t player_status;
    strategy_t strategy;
} player_t;

typedef struct{
    int DECK_SIZE;
    int moves;
    variant_t variant;
    war_type_t war_type;
} game_t;

void endGame(player_t* winner, int moves);

void initializePlayer(player_t* player,game_t* game,char* name);

void initializeDeckWithRandomNumbers(int deck[], game_t* game);

void splitIntoTwoHands(int deck[],player_t* player1,player_t* player2, game_t* game);

void playGame(player_t* player1,player_t* player2, game_t* game);

void playWiseTurn(player_t* you,player_t* opponent,player_t* player1,player_t* player2, game_t* game);

void shiftCardLeft(int hand[], int steps, int DECK_SIZE);  //przesunięcie kart jednej talii o steps w lewo

int war(player_t* player1, player_t* player2, game_t* game);

void variantBsplitCards(player_t* player1, player_t* player2, int cards_in_war,int DECK_SIZE);

void giveCardsToWinner(player_t* winner, player_t* looser, short cards_in_war, int DECK_SIZE, war_type_t war_type);

void clearArray(int array[], int size);

void saveResults(player_t* player1, player_t* player2, game_t* game,FILE *file);

void determineHandRank(player_t* player, int DECK_SIZE);

char chooseCard(player_t* you, int opponent_stack[], player_t* player2);

short startGame(game_t* game);