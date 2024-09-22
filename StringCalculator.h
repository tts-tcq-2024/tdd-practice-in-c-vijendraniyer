#ifndef STRING_CALCULATOR_H
#define STRING_CALCULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NUMBERS 1000

// Function declarations
int add(const char *input);
static int calculateSum(const int *numbers, int count);
static void parseInput(const char *input, int *numbers, int *count);
static void handleCustomDelimiter(const char **input, char *delimiter);
static int checkForNegatives(const int *numbers, int count);

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
    int numbers[MAX_NUMBERS];
    int count = 0;

    parseInput(input, numbers, &count);
    if (checkForNegatives(numbers, count)) {
        return -1; // Early return if negatives found
    }

    return calculateSum(numbers, count);
}

/**
 * @brief Calculates the sum of an array of numbers.
 * 
 * This function iterates through the provided array and sums up all numbers 
 * that are less than or equal to 1000.
 * 
 * @param numbers The array of integers to sum.
 * @param count The number of elements in the array.
 * @return The total sum of the numbers.
 */
static int calculateSum(const int *numbers, int count) {
    int sum = 0;
    for (int i = 0; i < count; i++) {
        if (numbers[i] <= 1000) {
            sum += numbers[i];
        }
    }
    return sum;
}

/**
 * @brief Parses the input string to extract numbers.
 * 
 * This function splits the input string using the specified delimiters and converts 
 * the tokens into integers, storing them in the provided array.
 * 
 * @param input The input string containing numbers.
 * @param numbers The array to store the extracted numbers.
 * @param count Pointer to the integer that keeps track of the number of extracted numbers.
 */
static void parseInput(const char *input, int *numbers, int *count) {
    char *token;
    char delimiter[10] = {',', '\0'}; // Default delimiter

    if (input[0] == '/') { // Handle custom delimiter
        handleCustomDelimiter(&input, delimiter);
    }

    token = strtok((char *)input, "\n");
    while (token != NULL) {
        char *numToken = strtok(token, delimiter);
        while (numToken != NULL) {
            long number = strtol(numToken, NULL, 10);
            // Check for valid number conversion and store
            if (number >= INT_MIN && number <= INT_MAX) {
                numbers[(*count)++] = (int)number;
            }
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
    while (**input && **input != '\n') {
        strncat(delimiter, *input, 1); // Append custom delimiter character
        (*input)++;
    }
    (*input)++; // Skip the newline character
}

/**
 * @brief Checks for negative numbers in the provided array.
 * 
 * This function scans the numbers and if any negative numbers are found, 
 * it prints an error message and returns a flag.
 * 
 * @param numbers The array of integers to check.
 * @param count The number of elements in the array.
 * @return 1 if negatives are found, otherwise 0.
 */
static int checkForNegatives(const int *numbers, int count) {
    char buffer[256] = "Exception: negatives not allowed: ";
    int negativeCount = 0;

    for (int i = 0; i < count; i++) {
        if (numbers[i] < 0) {
            if (negativeCount > 0) {
                strcat(buffer, ", ");
            }
            sprintf(buffer + strlen(buffer), "%d", numbers[i]);
            negativeCount++;
        }
    }

    if (negativeCount > 0) {
        fprintf(stderr, "%s\n", buffer);
        return 1; // Indicate that negatives were found
    }

    return 0; // No negatives found
}

#endif // STRING_CALCULATOR_H
