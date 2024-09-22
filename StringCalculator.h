#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBERS 1000

typedef struct {
    int value;
    int isNegative;
} Number;

// Function declarations
int add(const char *input);
static int calculateSum(Number *numbers, int count);
static void parseInput(const char *input, Number *numbers, int *count);
static void handleCustomDelimiter(const char **input, char *delimiter);

/**
 * @brief Processes a string of numbers separated by commas, newlines, or custom delimiters.
 * 
 * @param input The input string containing numbers.
 * @return The sum of the numbers, or -1 if negatives are found.
 */
int add(const char *input) {
    Number numbers[MAX_NUMBERS];
    int count = 0;

    parseInput(input, numbers, &count);
    return calculateSum(numbers, count);
}

/**
 * @brief Calculates the sum of an array of numbers.
 * 
 * @param numbers The array of Number structures to sum.
 * @param count The number of elements in the array.
 * @return The total sum of the numbers, or exits on negatives.
 */
static int calculateSum(Number *numbers, int count) {
    int sum = 0;
    char buffer[256] = "Exception: negatives not allowed: ";
    int negativeCount = 0;

    for (int i = 0; i < count; i++) {
        if (numbers[i].value < 0) {
            if (negativeCount > 0) {
                strcat(buffer, ", ");
            }
            sprintf(buffer + strlen(buffer), "%d", numbers[i].value);
            negativeCount++;
        } else if (numbers[i].value <= 1000) {
            sum += numbers[i].value;
        }
    }

    if (negativeCount > 0) {
        fprintf(stderr, "%s\n", buffer);
        exit(EXIT_FAILURE);
    }

    return sum;
}

/**
 * @brief Parses the input string to extract numbers.
 * 
 * @param input The input string containing numbers.
 * @param numbers The array to store the extracted Number structures.
 * @param count Pointer to the integer that keeps track of the number of extracted numbers.
 */
static void parseInput(const char *input, Number *numbers, int *count) {
    char *token;
    char delimiter[10] = {',', '\0'}; // Default delimiter

    if (input[0] == '/') {
        handleCustomDelimiter(&input, delimiter);
    }

    // Split input by delimiter and convert to Number structures
    token = strtok((char *)input, "\n");
    while (token) {
        char *numToken = strtok(token, delimiter);
        while (numToken) {
            numbers[*count].value = strtol(numToken, NULL, 10);
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
