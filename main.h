void drawCards(const int number1, const int number2, const int show_cards);

void initializeDeckWithRandomNumbers(int deck[]);

void splitIntoTwoHands(int deck[],int hand1[],int hand2[]);

void battle(int from[], int to[]);

void playGame(int hand1[],int hand2[]);

void shiftCardsLeft(int hand1[], int hand2[], int steps);  //przesunięcie kart obu talii o steps w lewo

int war(int hand1[], int hand2[]);

void printHands(int hand1[],int hand2[]);