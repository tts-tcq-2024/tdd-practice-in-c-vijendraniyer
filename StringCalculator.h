#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBERS 1000

// Structure to hold number values and their negative status
typedef struct {
    int value;       // The number value
    int isNegative;  // Flag indicating if the number is negative
} Number;

// Function declarations
int add(const char *input);
static int calculateSum(Number *numbers, int count);
static void parseInput(const char *input, Number *numbers, int *count);
static void handleCustomDelimiter(const char **input, char *delimiter);
static void checkForNegatives(Number *numbers, int count);

/**
 * @brief Processes a string of numbers separated by commas, newlines, or custom delimiters.
 * 
 * This function returns the sum of the numbers, ignoring those greater than 1000. 
 * If negative numbers are present, it reports them and exits with an error.
 * 
 * @param input The input string containing numbers.
 * @return The sum of the numbers, or -1 if negatives are found.
 */
int add(const char *input) {
    Number numbers[MAX_NUMBERS];
    int count = 0;

    // Parse input to fill numbers array
    parseInput(input, numbers, &count);
    checkForNegatives(numbers, count);

    // Calculate and return the sum
    return calculateSum(numbers, count);
}

/**
 * @brief Calculates the sum of an array of numbers.
 * 
 * This function iterates through the provided array and sums up all numbers 
 * that are less than or equal to 1000.
 * 
 * @param numbers The array of Number structures to sum.
 * @param count The number of elements in the array.
 * @return The total sum of the numbers.
 */
static int calculateSum(Number *numbers, int count) {
    int sum = 0;
    for (int i = 0; i < count; i++) {
        if (!numbers[i].isNegative && numbers[i].value <= 1000) {
            sum += numbers[i].value;
        }
    }
    return sum;
}

/**
 * @brief Parses the input string to extract numbers.
 * 
 * This function splits the input string using the specified delimiters and converts 
 * the tokens into Number structures, storing them in the provided array.
 * 
 * @param input The input string containing numbers.
 * @param numbers The array to store the extracted Number structures.
 * @param count Pointer to the integer that keeps track of the number of extracted numbers.
 */
static void parseInput(const char *input, Number *numbers, int *count) {
    char *token;
    char *end;
    char delimiter[10] = {',', '\0'}; // Default delimiter

    if (input[0] == '/') { // Handle custom delimiter
        handleCustomDelimiter(&input, delimiter);
    }

    // Split input by delimiter and convert to Number structures
    token = strtok((char *)input, "\n");
    while (token) {
        char *numToken = strtok(token, delimiter);
        while (numToken) {
            numbers[*count].value = strtol(numToken, &end, 10);
            numbers[*count].isNegative = (numbers[*count].value < 0) ? 1 : 0;
            (*count)++;
            numToken = strtok(NULL, delimiter);
        }
        token = strtok(NULL, "\n");
    }
}

/**
 * @brief Handles custom delimiters found in the input string.
 * 
 * This function extracts the custom delimiter from the input string and updates the 
 * pointer to skip past the delimiter definition.
 * 
 * @param input Pointer to the input string to parse.
 * @param delimiter Buffer to store the custom delimiter.
 */
static void handleCustomDelimiter(const char **input, char *delimiter) {
    (*input) += 2; // Skip "//"
    while (**input != '\n' && **input) {
        strncat(delimiter, *input, 1); // Append custom delimiter character
        (*input)++;
    }
    (*input)++; // Skip the newline character
}

/**
 * @brief Checks for negative numbers in the provided array.
 * 
 * This function scans the numbers and if any negative numbers are found, 
 * it prints an error message and exits the program.
 * 
 * @param numbers The array of Number structures to check.
 * @param count The number of elements in the array.
 */
static void checkForNegatives(Number *numbers, int count) {
    char buffer[256] = "Exception: negatives not allowed: ";
    int negativeCount = 0;

    for (int i = 0; i < count; i++) {
        if (numbers[i].isNegative) {
            if (negativeCount > 0) {
                strcat(buffer, ", ");
            }
            sprintf(buffer + strlen(buffer), "%d", numbers[i].value);
            negativeCount++;
        }
    }

    if (negativeCount > 0) {
        fprintf(stderr, "%s\n", buffer);
        exit(EXIT_FAILURE); // Exit on negative number
    }
}
