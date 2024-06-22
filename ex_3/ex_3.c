/*
 * Name: Yuval Anteby
 * ID: 212152896
 * Assigment: 3
 */

#include <stdio.h>

#define MERGE_ORDER_INCOME 1
#define DIS_FROM_SYMMETRY 2
#define GANGS_BALANCED 3
#define TIME_FOR_ESCAPING 4
#define WORKERS_EFFICIENCY 5
#define EXIT 6
#define ARRAY_SIZE 30

void mainMenu();
void mergePrint(int a1[], int a2[], int start1, int start2, int end1, int end2);
void mergeOrderIncome();
void enterIntegerToArray(int a1[], int index, int length);
void distanceFromSymmetry();
int paliDistance(char str[], int start, int end);
void gangsBalanced();
int checkBalance(char* str, int n);
void optimalTimeForEscaping();
int escapingTime(int n);
void workersEfficiency();
int heistOpt(int maxW, int optVal, int w[], int v[], int start, int len);

int main() {
    mainMenu();
    return 0;
}
//Function requests a number from user. return value is double that was input from user
double getUserDouble(){
    double userDouble;
    scanf("%lf", &userDouble);
    return userDouble;
}

//Function checks if the number is integer. returns 1- integer, returns 0- not integer
int isNumberInteger(double inputDouble){
    const int NON_INTEGER_RETURN_VALUE = 0, INTEGER_RETURN_VALUE = 1;
    int integerNumber = (int) inputDouble;
    if ((double) integerNumber != inputDouble)
        return NON_INTEGER_RETURN_VALUE;
    return INTEGER_RETURN_VALUE;
}

//Function requests a number from user. return value is integer that was input from user
int getUserInt() {
    int userInteger;
    scanf("%d", &userInteger);
    return userInteger;
}

//Shows Tony's tasks. Will keep running the menu if incorrect input was detected or when finished a task.
void mainMenu() {
    const int DEFAULT_SELECTED = 0;
    int selectedTask = DEFAULT_SELECTED;
    while(selectedTask != EXIT) {
        printf("What is your desired task boss?\n1. merge order incomes\n2. find distance from symmetry\n"
               "3. check if the gangs are balanced\n4. find optimal time for escaping\n"
               "5. check workers efficiency\n6. exit\n");
        double selectedDouble = getUserDouble();
        if (isNumberInteger(selectedDouble)){
            selectedTask = (int) selectedDouble;
            switch (selectedTask) {
                case MERGE_ORDER_INCOME:
                    mergeOrderIncome();
                    break;
                case DIS_FROM_SYMMETRY:
                    distanceFromSymmetry();
                    break;
                case GANGS_BALANCED:
                    gangsBalanced();
                    break;
                case TIME_FOR_ESCAPING:
                    optimalTimeForEscaping();
                    break;
                case WORKERS_EFFICIENCY:
                    workersEfficiency();
                    break;
                case EXIT:
                    printf("goodbye boss!");
                    break;
                default:
                    printf("Sorry Tony, I don't understand...\n");
                    selectedTask = DEFAULT_SELECTED;
                    break;
            }
        } else{
            printf("Sorry Tony, I don't understand...\n");
        }
    }

}

/*Recursive function for entering integers to 1 array. Function will get the array, start index and end index (length)
 * Function will stop running once reached array's end */
void enterIntegerToArray(int a1[], int index, int length) {
    const int ONE = 1;
    //Check if the index didn't reach the end and indeed user entered a value (while saving in array)
    if (index < length && scanf("%d", &a1[index]) == ONE)
        enterIntegerToArray(a1, (index + ONE), length);
}

/* Recursive function for entering integers to 2 arrays. Function will get the arrays, start index and end index
 * (length). Function will stop running once reached arrays' ends*/
void enterIntegerTo2Arrays(int a1[], int a2[], int index, int length) {
    const int TWO = 2;
    //Check if didn't reach arrays' ends and if the user indeed entered 2 integers (while saving them to arrays)
    if (index < length && scanf("%d %d", &a1[index], &a2[index]) == TWO)
        enterIntegerTo2Arrays(a1, a2, (++index), length);
}

/*Recursive function for entering chars to 1 array. Function will get the array, start index and end index (length).
 *Function will stop running once reached array's end.*/
void enterCharsToArray(char a1[], int index, int length) {
    const int ONE = 1;
    //Check if didn't reach array's end and indeed user entered a char while saving to array
    if ((index < length) && (scanf(" %c", &a1[index]) == ONE))
        enterCharsToArray(a1, (index + ONE), length);
}

/*Task 1- recursive function for getting input of 2 arrays and their length from the user. Will call mergePrint in
 * order to print the arrays in ascending order. */
void mergeOrderIncome() {
    const int ARRAY_START_INDEX = 0, ONE = 1, DEFAULT_ARRAY_VALUE = 0;
    int workerArray1[ARRAY_SIZE] = {DEFAULT_ARRAY_VALUE}, workerArray2[ARRAY_SIZE] = {DEFAULT_ARRAY_VALUE};
    //Receive the input for the lengths and arrays using a recursive function - enterIntegerToArray
    printf("Enter the length of the first worker array:");
    int firstWorkerLength = getUserInt();
    printf("\nNow enter the first array's elements:");
    enterIntegerToArray(workerArray1, ARRAY_START_INDEX, firstWorkerLength);
    printf("\nEnter the length of the second worker array:");
    int secondWorkerLength = getUserInt();
    printf("\nNow enter the second array's elements:");
    enterIntegerToArray(workerArray2, ARRAY_START_INDEX, secondWorkerLength);
    /*Start the recursive printing function from index zero, end the printing at the length provided by user
    * -1 to prevent exiting array boundary */
    mergePrint(workerArray1, workerArray2, ARRAY_START_INDEX, ARRAY_START_INDEX,
               (firstWorkerLength - ONE), (secondWorkerLength - ONE));
}

/*Function to print the arrays from the user in ascending order.
 * the function receives two integer arrays sorted in ascending order, the start & end index they should print.
 * the function will return to the main menu for more tasks when finished printing. */
void mergePrint(int a1[], int a2[], int start1, int start2, int end1, int end2) {
    const int ZERO_FOR_START = 0;
    //Checks if it's the start of the arrays and prints start if it.
    if ((start1 == ZERO_FOR_START) && (start2 == ZERO_FOR_START)) {
        printf("\nstart ");
    }
    //Checks if a1 & a2 haven't reached their end index, if they didn't will print the smaller value and add 1 to index
    if ((start1 <= end1) && (start2 <= end2)) {
        if ((a1[start1] <= a2[start2])) {
            printf("%d ", a1[start1]);
            mergePrint(a1, a2, ++start1, start2, end1, end2);
        } else if ((a2[start2] < a1[start1])) {
            printf("%d ", a2[start2]);
            mergePrint(a1, a2, start1, ++start2, end1, end2);
        }
    } else if(start1 <= end1){
        //In case a2 reached the end index, print a1 from the current index to the end
        printf("%d ", a1[start1]);
        mergePrint(a1, a2, ++start1, start2, end1, end2);
    }else if(start2 <= end2){
        //In case a1 reached the end index, print a2 from the current index to the end
        printf("%d ", a2[start2]);
        mergePrint(a1, a2, start1, ++start2, end1, end2);
    } else{
        //In case both reached their end index, print end and return to the main menu for more tasks from Tony.
        printf("end\n");
    }
}

/*Function to print how many chars are different in the array in order to be a palindrome.
 * the function receives char array, the start & end index.
 * Return value is the 'distance from palindrome' */
int paliDistance(char str[], int start, int end){
    const int DEFAULT_ZERO = 0, ONE = 1;
    int distanceCount = DEFAULT_ZERO;
    //If started a new palindrome, return the starting value of the distance to 0
    if(start == DEFAULT_ZERO){
        distanceCount = DEFAULT_ZERO;
    }
    if(start < end){
        /*Check if the chars are equal taking the first and last, if they aren't add 1. keeps going until reaching the
        *center of the char or if the 2 middle chars have been reached. then change the indexes accordingly */
        if(str[start] != str[end]){
            distanceCount = ONE + paliDistance(str, ++start, --end);
        } else
            distanceCount = paliDistance(str, ++start, --end);
    }
    return distanceCount;
}

/*Task 2-find the amount of different chars preventing array from being palindrome.
 * Will call paliDistance to calculate */
void distanceFromSymmetry() {
    char str1[ARRAY_SIZE];
    const int ARRAY_START_INDEX = 0, ONE = 1;
    //Get the needed info about the length and array values from the user
    printf("Enter the length of the string: ");
    int length = getUserInt();
    printf("\nNow enter the string:");
    enterCharsToArray(str1, ARRAY_START_INDEX, length);
    //Calculate the distance using the recursive function
    int distance = paliDistance(str1, ARRAY_START_INDEX, (length - ONE));
    printf("\nThe palindrome distance is %d.\n", distance);
}

/*Function to check if gangs are balanced by checking 'a' & 'b' in the array
 * the function receives char array and the 'end' index of the array, end index decrementing by 1 each time
 *Return value is 1 if the gangs are balanced, 0 if they aren't */
int checkBalance(char* str, int n){
    const int DEFAULT_ZERO = 0;
    int balanceCount = DEFAULT_ZERO, result;
    //Check the array value, add the member to the correct gang count. keeps checking till reaching the end (n value)
    if(n<DEFAULT_ZERO) {
        return balanceCount;
    }
    if (str[n] == 'a' || str[n] == 'A') {
        balanceCount++;
    } else if (str[n] == 'b' || str[n] == 'B') {
        balanceCount--;
    }
    result = balanceCount + checkBalance(str, --n);
    return result;
}

/*Task 3- find if there amount of gangs members in char array 'a' and 'b' is equal.
 * Calls checkBalance to calculate */
void gangsBalanced() {
    char membersArray[ARRAY_SIZE];
    const int ARRAY_START_INDEX = 0, BALANCED_VALUE = 0, ONE = 1;
    //Get the needed info about the amount of members and array values
    printf("Enter the number of gang members: ");
    int membersAmount = getUserInt();
    printf("\nEnter the gang associations: ");
    enterCharsToArray(membersArray, ARRAY_START_INDEX, membersAmount);
    //Checks the amount of gang members. if return value equal 1 it's balanced, if not then it's not balanced
    if(checkBalance(membersArray, membersAmount - ONE) == BALANCED_VALUE)
        printf("\nThe army is balanced.\n");
    else
        printf("\nThe army is not balanced.\n");
}

/*Recursive function to check the 'time to get to the safe room' by checking how many times you need to divide/subtract
 * the number given n till reaching 1 (safe room). The function receives integer n for the original room.
 *the function returns the amount of times needed to divide by 2,3 (default divisors) or subtracting 1 until reaching 1
 * */
int escapingTime(int n) {
    const int DEF_ZERO = 0, SAFE_ROOM = 1, DESIRED_REMAINDER = 0, FIRST_DIVISOR = 3, SECOND_DIVISOR = 2, INCREMENT= 1;
    //Given a starting value that is extremely large in order to make sure result won't be smaller than the calculation
    int result = 1000000;
    if(n == SAFE_ROOM) {
        return DEF_ZERO;
    }
    //Check if n is divisible by 3 and keep recursion in this path. Return this path value if indeed smaller
    if (n % FIRST_DIVISOR == DESIRED_REMAINDER) {
        int divideBy3 = INCREMENT + escapingTime(n / FIRST_DIVISOR);
        if(divideBy3 < result)
            result = divideBy3;
    }
    //Check if n is divisible by 2 and keep the recursion in this path. Return this path value if indeed smaller
    if (n % SECOND_DIVISOR == DESIRED_REMAINDER) {
        int divideBy2 = INCREMENT + escapingTime(n / SECOND_DIVISOR);
        if(divideBy2 < result)
            result = divideBy2;
    }
    //Subtract 1 and keep the recursion going on again in this path. Return this path if indeed smaller
    int subtractBy1 = INCREMENT + escapingTime(n - INCREMENT);
    if(subtractBy1 < result)
        result = subtractBy1;
    return result;
}

/*Task 4- find the amount of times needed to divide by 2,3 or subtracting 1 to reach the safe room (number 1)
 * The function will call the recursive function escaping time for calculations. */
void optimalTimeForEscaping() {
    printf("What room are you in boss?\n");
    int originalRoom = getUserInt();
    int escapeTime = escapingTime(originalRoom);
    printf("Your escaping time is: %d\n", escapeTime);
}

/*Recursive function to check the optimal value a thief can get. The function receives the max weight that can be
 *carried, the current optimal value (default 0), the arrays of weights and values, the start index and end index.
 * The function returns the optimal value a thief can get. */
int heistOpt(int maxW, int optVal, int w[], int v[], int start, int len){
    const int INCREMENT = 1, DEFAULT_ZERO = 0;
    if(start == len || maxW == DEFAULT_ZERO) {
        return DEFAULT_ZERO;
    }
    //If current object exceeding weight - skip it
    if(w[start] > maxW){
        return heistOpt(maxW, optVal, w, v, INCREMENT+start, len);
    }
    // Consider two cases: including the current object or excluding it
    int include = v[start] + heistOpt(maxW - w[start], optVal, w, v,(start + INCREMENT), len);
    int exclude = heistOpt(maxW, optVal, w, v, (start + INCREMENT), len);
    // Return the max value
    if(include > exclude){
        return include;
    } else{
        return exclude;
    }
}

/*Task 5- find what is the optimal value a thief can get Tony.
 *the function will call the recursive function heistOpt for calculations. */
void workersEfficiency() {
    int weightArr[ARRAY_SIZE] = {0}, valueArr[ARRAY_SIZE] = {0};
    int objectAmount, maxWeight, optimalValue;
    const int START_INDEX = 0, DEFAULT_ZERO = 0;
    printf("Enter the number of objects and the maximum weight:");
    scanf("%d %d", &objectAmount, &maxWeight);
    printf("\nNow enter the objects' values and weights:");
    //Entering the values for both arrays
    enterIntegerTo2Arrays(valueArr, weightArr, START_INDEX, objectAmount);
    optimalValue = heistOpt(maxWeight, DEFAULT_ZERO, weightArr, valueArr, START_INDEX,
                            objectAmount);
    printf("\nThe optimal heist income value is %d\n", optimalValue);
}




