#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Name: Yuval Anteby
 * ID: 212152896
 * Assigment: 4
 */

#define MAX_ARRAY_SIZE 30
#define MAX_OBJECT_LENGTH 7
#define MAX_USERNAME_LENGTH 20

typedef struct{
    char objectName[MAX_OBJECT_LENGTH];
    int isMatched;
}MemoryObject;

typedef struct{
    char username[MAX_USERNAME_LENGTH];
    int points;
}Player;

int isEnoughPlayers(char combinedNames[], int numOfPlayers);
int isCorrectAmountOfObjects(int argCount, char *argv[], int nDim, int OBJECTS_INDEX);
int isCharacter(char c);
int isPositionsValid(int argCount, char *argv[], int nDim, int OBJECTS_INDEX);
void getNames(int NUM_OF_PLAYERS, Player playerArray[], char combinedNames[]);
void getObjects(int POS_INDEX, int argCount, char *argv[], int DIMENSION, MemoryObject objectsArray[][MAX_ARRAY_SIZE],
                int OBJECTS_INDEX);
void printWelcomeMessage(int DIMENSION, int objectsAmount, char *argv[], int OBJECTS_INDEX);
void printCurrentBoard(int DIMENSION, MemoryObject objectsArray[][MAX_ARRAY_SIZE]);
void printCard(const char *object);
void printRow(MemoryObject objectsArray[], int start, int DIMENSION);
void printScoreboard(const int NUM_OF_PLAYERS, Player playerArray[NUM_OF_PLAYERS]);
void printEndgame(int NUM_OF_PLAYERS, Player playerArray[]);
void runMemoryGame(int NUM_OF_PLAYERS, int DIMENSION, Player playerArray[], int objectsAmount,
                   MemoryObject objectsArray[][MAX_ARRAY_SIZE]);

/* Command line arguments (by order in the array):
 * board dimensions,
 * amount of players,
 * player 1 name#player 2 name etc.(2 to 10),
 * object 1, object 2 etc.(2 to 30),
 * position 1, position 2 etc. (according to the board dimensions (dim*dim/2)) */
int main(int argCount, char *argv[]) {
    const int MIN_ARGUMENTS = 2, BOARD_DIM_INDEX = 1, PLAYERS_NUM_INDEX = 2, PLAYERS_NAMES_INDEX = 3,OBJECTS_INDEX = 4;
    if(argCount <= MIN_ARGUMENTS) {
        printf("INVALID INPUT"); //Empty
    }else{
        //Initialize const for the program
        const int DIMENSION = atoi(argv[BOARD_DIM_INDEX]), NUM_OF_PLAYERS = atoi(argv[PLAYERS_NUM_INDEX]), TWO = 2;
        //Make sure all CLA are valid, if not exit the game
        if (!isEnoughPlayers(argv[PLAYERS_NAMES_INDEX],NUM_OF_PLAYERS)) {
            printf("INVALID INPUT"); //Not enough players
        }else if (!isCorrectAmountOfObjects(argCount, argv, DIMENSION, OBJECTS_INDEX)) {
            printf("INVALID INPUT"); //Incorrect amount of objects
        }else if (!isPositionsValid(argCount, argv, DIMENSION, OBJECTS_INDEX)) {
            printf("INVALID INPUT"); //Invalid positions
        }else{
            //Initialize the structs arrays
            MemoryObject objectsArray[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE];
            Player playerArray[NUM_OF_PLAYERS];
            //[ROW][COLUMN] x,y starts from top left while row is x and column is y
            int objectsAmount = DIMENSION * DIMENSION / TWO;
            const int POS_INDEX = OBJECTS_INDEX + objectsAmount, DEFAULT_ZERO = 0;

            //nullify the arrays, zero the points and make all objects unmatched as default
            memset(playerArray, '\0', NUM_OF_PLAYERS * MAX_USERNAME_LENGTH);
            for (int i = 0; i < NUM_OF_PLAYERS; ++i) {
                playerArray[i].points = DEFAULT_ZERO;
            }
            memset(objectsArray,'\0', MAX_OBJECT_LENGTH * MAX_ARRAY_SIZE * MAX_ARRAY_SIZE);
            for (int i = 0; i < MAX_ARRAY_SIZE; ++i) {
                for (int j = 0; j < MAX_ARRAY_SIZE; ++j) {
                    objectsArray[i][j].isMatched = DEFAULT_ZERO;
                }
            }
            //Get the usernames and objects and populate them in the correct spots
            getNames(NUM_OF_PLAYERS, playerArray, argv[PLAYERS_NAMES_INDEX]);
            getObjects(POS_INDEX, argCount, argv, DIMENSION, objectsArray, OBJECTS_INDEX);
            //Welcome message and introduction
            printWelcomeMessage(DIMENSION, objectsAmount, argv, OBJECTS_INDEX);
            printCurrentBoard(DIMENSION, objectsArray);
            //Start running the turn based part of the game
            runMemoryGame(NUM_OF_PLAYERS, DIMENSION, playerArray, objectsAmount, objectsArray);
        }
        return 0;
    }
}

/* Checks if the user gave the same amount of players as specified. Gets the char array from argv and number of players
 * Returns 1 if more than one player and same amount of players as specified (valid)
 * Returns 0 if one or fewer players or if not enough username given (invalid)*/
int isEnoughPlayers(char combinedNames[], int numOfPlayers) {
    const int INVALID = 0, VALID = 1, MIN_PLAYERS = 2;
    char namesCopy[strlen(combinedNames)];
    strcpy(namesCopy, combinedNames);
    const char delimiter[] = "#";
    int usernameCount = 0;

    //use strtok to go to the next time # appear, if it does add one since there is a username there. Loop until null.
    char *tokenPointer = strtok(namesCopy, delimiter);
    if (tokenPointer != NULL){
        while (tokenPointer != NULL){
            usernameCount++;
            tokenPointer = strtok(NULL, delimiter);
        }
    }else{
        //If there wasn't even one name then the CLA are invalid
        return INVALID;
    }
    //If the number of desired players doesn't match or didn't reach min players - return invalid.
    if ((numOfPlayers != usernameCount) || (usernameCount < MIN_PLAYERS))
        return INVALID;
    return VALID;
}

/* Checks if the user given the correct amount of objects.
 * Returns 1 if indeed the same amount (valid)
 * Returns 0 if not the same amount was given (invalid)*/
int isCorrectAmountOfObjects(int argCount, char *argv[], int nDim, const int OBJECTS_INDEX){
    const int  FIRST_CHAR_INDEX = 0, INVALID = 0, VALID = 1, TWO = 2, OBJECTS_AMOUNT = nDim * nDim / TWO;
    int objectCount = 0;
    //Loop from the start index of the objects (assuming CLA were entered correctly). start index is 3.
    for (int i = OBJECTS_INDEX; i < argCount; ++i) {
        if (isCharacter(argv[i][FIRST_CHAR_INDEX])) {
            objectCount++;
        } else{
            break;
        }
    }
    //If the amount of object doesn't match the real amount in the input-invalid input
    if (OBJECTS_AMOUNT == objectCount) {
        return VALID;
    }
    return INVALID;
}

// Function to check if a character is alphabetic. Input is a char, returns 1 if the char is alphabetic
int isCharacter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

/* Function checks if there is any position appearing more than once
 * Receives the arguments amount and the CLA
 * Returns 0 if there is a position more than once (invalid), returns 1 if there isn't (valid) */
int isPositionsValid(int argCount, char *argv[], int nDim, const int OBJECTS_INDEX){
    const int ONE = 1, TWO = 2, INVALID = 0, VALID = 1, MAX_INSTANCES = 1, OBJECTS_AMOUNT = nDim * nDim,
            POS_INDEX = OBJECTS_INDEX + OBJECTS_AMOUNT / TWO, ZERO = 0;
    int totalPosCounter = 0;
    //Loop through the CLA by checking if there are any position that appearing again add to counter
    for (int i = POS_INDEX; i < argCount; ++i) {
        totalPosCounter++;
        int posCounter = 1;
        //Check if the position is in the correct boundaries. if not-invalid input
        if ((atoi(argv[i]) >= ZERO) || (atoi(argv[i]) <= OBJECTS_AMOUNT/TWO - ONE)){
            //Compare current position to all others
            for (int j = i; j < argCount; ++j) {
                if ((i != j) && (atoi(argv[i]) == atoi(argv[j])))
                    posCounter++;
            }
            //If the position appeared more than once - invalid input
            if (posCounter > MAX_INSTANCES) {
                return INVALID;
            }
        } else{
            return INVALID;
        }
    }
    //Not enough positions given
    if (totalPosCounter < OBJECTS_AMOUNT - ONE){
        return INVALID;
    }
    return VALID;
}

/* Function to parse the names from argv's copy to the players array.
 * Gets the number of players, the players array and the copy of the combined names (combined by #) */
void getNames(const int NUM_OF_PLAYERS, Player playerArray[NUM_OF_PLAYERS], char combinedNames[]){
    const char delimiter1[] = "#";
    char *tokenPointer = strtok(combinedNames, delimiter1), *tempTokenPointer = NULL;
    int namesCounter = 0;
    //keep looping through the string until no more '#' can be found
    while (tokenPointer != NULL) {
        if (namesCounter == NUM_OF_PLAYERS) {
            // Last token, store the pointer
            tempTokenPointer = tokenPointer;
        }
        //copy the player's name into the player array
        strcpy(playerArray[namesCounter].username, tokenPointer);
        namesCounter++;
        tokenPointer = strtok(NULL, delimiter1);
    }
    // Handle the last token if it exists
    if (tempTokenPointer != NULL) {
        strcpy(playerArray[namesCounter].username, tempTokenPointer);
    }
}

/* Function to parse the objects from the argv array into the correct row and column in the 2d objects array.
 * gets the index of positions in argv, the number of arguments in argv, argv itself, the dimension of the board,
 * the 2d array of the objects and the index of the objects in argv */
void getObjects(const int POS_INDEX, int argCount, char *argv[], const int DIMENSION,
                MemoryObject objectsArray[][MAX_ARRAY_SIZE], const int OBJECTS_INDEX){
    int objectIndex = 0, TWO = 2;
    for (int i = POS_INDEX; i < argCount ; i++) {
        int posIndex = atoi(argv[i]), rowIndex = (posIndex) / DIMENSION, colIndex = (posIndex) % DIMENSION;
        //position of the name in argv array, the col and row it'll populate in the struct array
        strcpy(objectsArray[rowIndex][colIndex].objectName, argv[OBJECTS_INDEX + objectIndex / TWO]);
        objectIndex++;
    }
}

/* Function to print the welcome message. Including instructions and the objects that we will be playing with.
 * Gets the dimension, amount of objects, the index for the objects in argv and argv itself (to get the objects) */
void printWelcomeMessage(const int DIMENSION, int objectsAmount, char *argv[], const int OBJECTS_INDEX){
    const int STARTING_ROW = -1, TEN = 10, FIRST_COL = 0, ONE = 1;
    printf("Welcome to the Memory Card Game!\nThe game will be played on the following board:\n\n  |");
    //Outer loop for each row
    for (int i = -1; i < DIMENSION; ++i) {
        //-1 is the first row of specifying the column's numbers
        if (i == STARTING_ROW){
            for (int j = 0; j < DIMENSION; ++j) {
                //If the number is smaller than 10, it can be centered, if not indent to the right by 1 char
                if (j < TEN)
                    printf("  %d  |", j);
                else
                    printf("  %d |", j);
            }
            printf("\n--");
            //Print the border
            for (int j = 0; j < DIMENSION; ++j) {
                printf("------");
            }
            printf("-\n");
        } else{
            for (int j = 0; j < DIMENSION; ++j) {
                //Print the cells. if the first column then add the number of row before the first cell
                if (j == FIRST_COL)
                    printf("%d |(%d,%d)|", i, i, j);
                else
                    printf("(%d,%d)|", i, j);
            }
            printf("\n--");
            //Print the lower border
            for (int j = 0; j < DIMENSION; ++j) {
                printf("------");
            }
            printf("-\n");
        }
    }
    //Print a list of the names of the objects that will be played with
    printf("\nAnd the following objects:\n\n");
    for (int i = 0; i < objectsAmount; ++i) {
        printf("%d. %s\n", (i + ONE), argv[OBJECTS_INDEX + i]);
    }
    printf("\nLet the game begin!\n");
}

/* Function prints the current state of the board.
 * Gets the dimension and the 2d array of the objects*/
void printCurrentBoard(const int DIMENSION, MemoryObject objectsArray[][MAX_ARRAY_SIZE]) {
    const int PRINT_INDEX_START = 0;
    printf("\nCurrent board state:\n");
    //Upper border
    printf("-");
    for (int i = 0; i < DIMENSION; i++) {
        printf("----------------");
    }
    //Loop each row and print the values from the
    for (int i = 0; i < DIMENSION; ++i) {
        printRow(objectsArray[i], PRINT_INDEX_START, DIMENSION);
    }
    printf("\n");
}

/* Function to print a row of cards
 * Gets the 2d array of the objects, the start index and dimension */
void printRow(MemoryObject objectsArray[], int start, const int DIMENSION) {
    const int MATCHED = 1;
    printf("\n");
    for (int j = start; j < start + DIMENSION; j++) {
        if (j == start)
            printf("|");
        //Print the object if indeed a user managed to match this object. if not - print X
        if (objectsArray[j].isMatched == MATCHED)
            printCard(objectsArray[j].objectName);
        else
            printCard("X");
    }
    printf("\n");
    //Lower border
    for (int i = 0; i < DIMENSION; i++) {
        printf("----------------");
    }
    printf("-");
}

// Function to print a single card with border. Gets the name of the object
void printCard(const char *object) {
    const int TWO = 2, CARD_WIDTH = 15;
    int len = strlen(object), padding = (CARD_WIDTH - len) / TWO;
    //printf("|");
    //Print the padding of the left side of the card
    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    //print the name of the object
    printf("%s", object);
    //Print the padding of the right side of the card
    for (int i = 0; i < CARD_WIDTH - len - padding; i++) {
        printf(" ");
    }
    printf("|");
}

/* Function to print the scores of the users and their names.
 * Gets the number of players and the array of players. */
void printScoreboard(const int NUM_OF_PLAYERS, Player playerArray[NUM_OF_PLAYERS]){
    const int FIRST_USER = 0;
    printf("\nThe scores are:\n%s: %d", playerArray[FIRST_USER].username, playerArray[FIRST_USER].points);
    //Print all users from the second to the last
    for (int i = 1; i < NUM_OF_PLAYERS; ++i) {
        printf(", %s: %d", playerArray[i].username, playerArray[i].points);
    }
}

/* Function running the turn based part of the game. It will ask the user for (x,y) of a cell in the table (2d array)
 * if the player guessed correctly add 1 point, if not continue the game until all cards were matched (per the
 * instructions of the memory card game).
 * Gets the number of players, dimension, array of players, amount of objects and 2d array of objects. */
void runMemoryGame(const int NUM_OF_PLAYERS, const int DIMENSION, Player playerArray[NUM_OF_PLAYERS], int objectsAmount,
                   MemoryObject objectsArray[][MAX_ARRAY_SIZE]){
    const int GAME_OVER = -1, MATCHED = 1, DEFAULT_ZERO = 0, EQUAL = 0;
    int playerTurn = 0, newTurn = 1, matchCounter = 0, row1, col1, row2, col2;
    //Continue playing until all cards were matched
    while (playerTurn != GAME_OVER) {
        //For each new turn, tell the players whose turn it is
        if (newTurn == 1) {
            printf("\nIt is %s's turn.", playerArray[playerTurn].username);
        }
        newTurn = DEFAULT_ZERO;
        printf("\nEnter the coordinates of the first card (row col): ");
        scanf("%d %d", &row1, &col1);
        printf("Enter the coordinates of the second card (row col): ");
        scanf("%d %d", &row2, &col2);
        //Check if the x,y are different and if weren't matched already (validate the input)
        if ((row1 >= DIMENSION) || (col1 >= DIMENSION) || (row2 >= DIMENSION) || (col2 >= DIMENSION) ||
            (row1 == row2 && col1 == col2) || (objectsArray[row1][col1].isMatched == MATCHED) ||
            (objectsArray[row2][col2].isMatched == MATCHED)){
            printf("Invalid move. Try again.");
        } else{
            //If the objects names are equal the user guessed correctly. add point and change their status to matched
            if (strcmp(objectsArray[row1][col1].objectName, objectsArray[row2][col2].objectName) == EQUAL){
                playerArray[playerTurn].points++;
                objectsArray[row1][col1].isMatched = MATCHED;
                objectsArray[row2][col2].isMatched = MATCHED;
                printf("Match!\n");
                matchCounter++;
            } else{
                printf("No match. Try again.\n");
                playerTurn++;
            }
            //Continue to the next player and print the current leaderboard and current board

            newTurn = 1;
            printScoreboard(NUM_OF_PLAYERS, playerArray);
            if (matchCounter != objectsAmount)
                printCurrentBoard(DIMENSION, objectsArray);
            //If reached a higher number than there are players return the turn to the first player
            if (playerTurn >= NUM_OF_PLAYERS)
                playerTurn = DEFAULT_ZERO;
            //If all cards were matched - the game is over. summarize the game in print endgame
            if (matchCounter == objectsAmount) {
                playerTurn = GAME_OVER;
                printEndgame(NUM_OF_PLAYERS, playerArray);
            }
        }
    }
}

/* Function to summarize the end of the game. Show the leaderboard and decide if there is a winner or a tie.
 * Gets the number of players and array of players */
void printEndgame(const int NUM_OF_PLAYERS, Player playerArray[NUM_OF_PLAYERS]){
    printf("\n");
    int maxPlayerIndex = -1;
    //Check each player against each other to make sure one has the highest point (if there is a tie stop the loop)
    for (int i = 0; i < NUM_OF_PLAYERS; ++i) {
        for (int j = 0; j < NUM_OF_PLAYERS; ++j) {
            //Check if there is tie, if there is then break out of the loop and print it
            if ((playerArray[i].points == playerArray[j].points) && (i != j)){
                printf("It's a tie!");
                return;
            }
            //If the player has more points, save their index and point count for future checks
            if ((playerArray[i].points > playerArray[j].points) && (i != j)){
                maxPlayerIndex = i;
            }
        }
    }
    printf("Congratulations %s! You won!", playerArray[maxPlayerIndex].username);
}