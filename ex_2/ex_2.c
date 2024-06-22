#include <stdio.h>

/*
 * Name: Yuval Anteby
 * ID: 212152896
 * Assigment: 2
 */

//Function requests a number from user. return value is double that was input from user
double getUserInput(){
    double userDouble;
    printf("\nEnter a number: ");
    scanf("%lf", &userDouble);
    return userDouble;
}

//Function checks if the number is integer. returns 1- integer, returns 0- not integer
int isNumberInteger(double inputDouble){
    const int NON_INTEGER_RETURN_VALUE = 0, INTEGER_RETURN_VALUE = 1;
    int integerNumber = (int) inputDouble;
    if ((double) integerNumber != inputDouble){
        printf(" Foolish mortal! You cannot solve Thanos's quest with an input that isn't an Integer\n");
        return NON_INTEGER_RETURN_VALUE;
    }
    return INTEGER_RETURN_VALUE;
}

//Functions checks if the number is bigger than 0. returns 1- bigger, returns 0- smaller than 0.
int isGreaterThan0(int input){
    const int SMALLER_RETURN_VALUE = 0, BIGGER_RETURN_VALUE = 1;
    if (input < SMALLER_RETURN_VALUE){
        printf(" Foolish mortal! You cannot solve Thanos's quest with an input that isn't a number bigger"
               " than 0\n");
        return SMALLER_RETURN_VALUE;
    }
    return BIGGER_RETURN_VALUE;
}

//Functions checks if the number is bigger than 1. returns 1- bigger, returns 0- smaller than 1.
int isGreaterThan1(int input){
    const int ONE = 1, SMALLER_RETURN_VALUE = 0, BIGGER_RETURN_VALUE = 1;
    if (input < ONE) {
        printf(" Foolish mortal! You cannot solve Thanos's quest with an input that isn't a number bigger"
               " than 1\n");
    } else
        return BIGGER_RETURN_VALUE;
    return SMALLER_RETURN_VALUE;
}

//Adventure number 1 - FizzBuzz
void hiddenCode(){
    const int DESIRED_REMAINDER = 0, THREE = 3, FIVE = 5, FIFTEEN = 15;
    double userDouble = getUserInput();
    int userInteger;
    //Adventure will play if the number is both int and greater than 1
    if (!isNumberInteger(userDouble) || !isGreaterThan1((int)userDouble)) {
        printf("\n");
        return;
    } else {
        userInteger = (int) userDouble;
        //Loop checks if the current number is divisible by 3,5,15 and show if divisible:FizzBuzz- 15, Buzz- 5, Fizz- 3
        for (int i = 1; i <= userInteger; i++) {
            if ((i == FIFTEEN) || (i % FIFTEEN == DESIRED_REMAINDER)) {
                printf("FizzBuzz\n");
            } else if ((i == FIVE) || (i % FIVE == DESIRED_REMAINDER)) {
                printf("Buzz\n");
            } else if ((i == THREE) || (i % THREE == DESIRED_REMAINDER)) {
                printf("Fizz\n");
            } else
                printf("%d\n", i);
        }
        printf("\n");
    }
}

//Adventure number 2 - Fibonacci
void numericMayhem(){
    const int FIRST_FIBONACCI = 0, SECOND_FIBONACCI = 1, ZERO = 0, ONE = 1;
    double userDouble;
    int userInput;
    unsigned long long lastFibonacci = SECOND_FIBONACCI, secondLastFibonacci = FIRST_FIBONACCI, newFibonacci;
    userDouble = getUserInput();
    //Adventure will play if the number is both int and greater than 0
    if (!isNumberInteger(userDouble) || !isGreaterThan0((int) userDouble)) {
        printf("\n\n");
        return;
    } else {
        userInput = (int) userDouble;
        for (int i = 0; i < userInput; i++) {
            //Print the first number in Fibonacci sequence
            if (i == ZERO)
                printf("%llu ", secondLastFibonacci);
            //Print the second number in Fibonacci sequence
            else if (i == ONE)
                printf("%llu ", lastFibonacci);
            //Print the current number of the Fibonacci sequence (the sum of the 2 last numbers)
            else {
                newFibonacci = lastFibonacci + secondLastFibonacci;
                printf("%llu ", newFibonacci);
                secondLastFibonacci = lastFibonacci;
                lastFibonacci = newFibonacci;
            }
        }
        printf("\n\n");
    }
}

//Adventure number 3 - Sum of prime
void mazeSystem(){
    const int DEFAULT_ZERO = 0, MIN_DIVIDERS_FOR_PRIME = 2;
    double userDouble = getUserInput();
    unsigned long primeCount, sumInteger, userInput;
    //Adventure will play if the number is both int and greater than 1
    if (!isNumberInteger(userDouble) || !isGreaterThan1((int) userDouble)){
        printf("\n");
        return;
    } else {
        userInput = (int) userDouble;
        sumInteger = (unsigned long) DEFAULT_ZERO;
        //Loop from one to the number chosen by user, will check each number if prime or not
        for (int i = 1; i <= userInput; i++) {
            primeCount = (unsigned long) DEFAULT_ZERO;
            //Check if the number is prime by checking if there are equal/less than 2 natural nums to divide
            for (int j = 1; j <= i; j++) {
                if ((i % j == DEFAULT_ZERO))
                    primeCount++;
            }
            //Check if the amount of dividers is equal to 2. if it is than its prime and add it to the sum
            if (primeCount == (unsigned long) MIN_DIVIDERS_FOR_PRIME)
                sumInteger = sumInteger + (unsigned long) i;
        }
        printf("%lu\n", sumInteger);
        printf("\n");
    }
}

//Adventure number 4 - Perfect numbers
void thanosSorcery(){
    const int DEFAULT_SUM = 0, DESIRED_REMAINDER = 0;
    double userDouble = getUserInput();
    unsigned int sumInteger, userInput;
    //Adventure will play if the number is both int and greater than 0
    if (!isNumberInteger(userDouble) || !isGreaterThan0((int) userDouble)) {
        printf("\n");
        return;
    } else {
        userInput = (int) userDouble;
        sumInteger = DEFAULT_SUM;
        //Loop will add the new potential dividers to the sum if it indeed matched (needs to have remainder = 0)
        for (int i = 1; i < userInput; i++) {
            if (userInput % i == DESIRED_REMAINDER)
                sumInteger = sumInteger + i;
        }
        //Check if the number is equal to the sum of the dividers. if equal it's perfect
        if (userInput == sumInteger)
            printf("Perfect!\n");
        else
            printf("Not Perfect!\n");
        printf("\n");
    }
}

//Starts the menu
int main() {
    const int DEFAULT_SELECTED = 0;
    int selectedAdventure = DEFAULT_SELECTED;
    while (selectedAdventure != 5) {
        printf("Welcome to Thanos's playground.\nChoose your adventure:\n1. Revealing the Hidden Code\n"
               "2. Thanos' numeric mayhem\n3. Thanos' Maze System\n4. The Sorcery of Thanos\n5. Quit the quest\n");
        scanf("%d", &selectedAdventure);
        switch (selectedAdventure) {
            case 1:
                hiddenCode();
                break;

            case 2:
                numericMayhem();
                break;

            case 3:
                mazeSystem();
                break;

            case 4:
                thanosSorcery();
                break;

            case 5:
                printf("\nCongratulations! You finished the quest and managed to defeat Thanos");
                break;

            default:
                printf("\nThanos is mad! You are playing his game and this is not an option. Choose again, wisely.\n"
                       "\n");
                selectedAdventure = DEFAULT_SELECTED;
                break;

        }
    }
    return 0;
}
