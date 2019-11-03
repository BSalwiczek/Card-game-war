
void drawOutput(int stack1[],int stack2[], int stacks_len, int hand1[], int hand2[]);

void drawCardsQueue(int y,int x,int hand[]);

void endGame(short winner);

void drawCard(int y,int x,int number, short show_card, short color);

void initializeDeckWithRandomNumbers(int deck[]);

void splitIntoTwoHands(int deck[],int hand1[],int hand2[]);

void battle(int from[], int to[]);

void playGame(int hand1[],int hand2[]);

void shiftCardLeft(int hand[], int steps);  //przesunięcie kart jednej talii o steps w lewo

int war(int hand1[], int hand2[]);

void printHands(int hand1[],int hand2[]);