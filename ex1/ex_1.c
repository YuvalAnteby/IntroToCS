#include <stdio.h>

/*
 * Name: Yuval Anteby
 * ID: 212152896
 * Assigment: 1
 */

int main() {

    unsigned int firstDigit1, secondDigit1, thirdDigit1, fourthDigit1, fifthDigit1;
    unsigned int firstDigit2, secondDigit2, thirdDigit2, fourthDigit2, fifthDigit2;
    unsigned int firstDigitXor, secondDigitXor, thirdDigitXor, fourthDigitXor;
    unsigned int hex1, hex2, hexSum, lsbBinary, baseChosen, decimalValue;
    unsigned int numberEntered, bitFromRight, shiftedNumber, binaryNumber;
    const int BINARY_BASE = 2, MASK_F = 0XF, MASK_1 = 1, NUMBER_ONE = 1;

    //Exercise 1
    printf("Exercise 1:\n");
    printf("Please enter 2 binary strings with 4 digits:\n");
    printf("Binary 1: ");
    scanf("%1d%1d%1d%1d", &fourthDigit1, &thirdDigit1, &secondDigit1, &firstDigit1);
    printf("Binary 2: ");
    scanf("%1d%1d%1d%1d", &fourthDigit2, &thirdDigit2, &secondDigit2, &firstDigit2);
    firstDigitXor = firstDigit1 ^ firstDigit2;
    secondDigitXor = secondDigit1 ^ secondDigit2;
    thirdDigitXor = thirdDigit1 ^ thirdDigit2;
    fourthDigitXor = fourthDigit1 ^ fourthDigit2;
    printf("\n%d%d%d%d\n^\n%d%d%d%d\n--------\n%d%d%d%d\n", fourthDigit1, thirdDigit1, secondDigit1, firstDigit1,
           fourthDigit2, thirdDigit2, secondDigit2, firstDigit2, fourthDigitXor, thirdDigitXor, secondDigitXor,
           firstDigitXor);

    //Exercise 2
    printf("\nExercise 2:\n");
    printf("Enter 2 hexadecimal numbers:\n");
    printf("Hex 1: ");
    scanf("%X", &hex1);
    printf("Hex 2: ");
    scanf("%X", &hex2);
    hexSum = hex1 + hex2;
    printf("\n%X + %X = %X\n", hex1, hex2, hexSum);

    //Use mask in order to keep only the rightmost digit, then find it in binary
    lsbBinary = hexSum & MASK_F;
    firstDigit1 = lsbBinary % BINARY_BASE;
    lsbBinary = lsbBinary / BINARY_BASE;
    secondDigit1 = lsbBinary % BINARY_BASE;
    lsbBinary = lsbBinary / BINARY_BASE;
    thirdDigit1 = lsbBinary % BINARY_BASE;
    lsbBinary = lsbBinary / BINARY_BASE;
    fourthDigit1 = lsbBinary % BINARY_BASE;
    printf("The last 4 binary digits of the sum are %1d%1d%1d%1d\n",fourthDigit1, thirdDigit1, secondDigit1, firstDigit1);

    //Exercise 3
    printf("\nExercise 3:\n");
    printf("Choose a base between 2 to 9: ");
    scanf("%d", &baseChosen);
    printf(" Enter a 5 digit number using that base: ");
    scanf("%1d%1d%1d%1d%1d", &fifthDigit1, &fourthDigit1, &thirdDigit1, &secondDigit1,
          &firstDigit1);

    //Convert the number to the correct base
    secondDigit2 = secondDigit1 * baseChosen;
    thirdDigit2 = thirdDigit1 * baseChosen * baseChosen;
    fourthDigit2 = fourthDigit1 * baseChosen * baseChosen * baseChosen;
    fifthDigit2 = fifthDigit1 * baseChosen *baseChosen * baseChosen * baseChosen;
    decimalValue = firstDigit1 + secondDigit2 + thirdDigit2 + fourthDigit2 + fifthDigit2;
    printf(" The decimal value of %1d%1d%1d%1d%1d in base %d is %d\n", fifthDigit1, fourthDigit1, thirdDigit1,
           secondDigit1, firstDigit1, baseChosen, decimalValue);

    //Exercise 4
    printf("\nExercise 4:\n");
    printf("Enter a number: ");
    scanf("%d", &numberEntered);
    printf(" I want to know the value of bit number: ");
    scanf("%d", &bitFromRight);

    //Right shift the number, then print the correct value
    shiftedNumber = numberEntered >> (bitFromRight - NUMBER_ONE);
    shiftedNumber = shiftedNumber & MASK_1;
    binaryNumber = shiftedNumber % BINARY_BASE;
    printf("\nThe value of the %d bit in %d is %d\n", bitFromRight, numberEntered, binaryNumber);

    printf("Congrats! You've found the philosopher's stone!");
    return 0;
}