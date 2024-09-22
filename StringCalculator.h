#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NUMBERS 1000

// Function declarations
int add(const char *input);
static int calculateSum(const int *numbers, int count);
static void parseInput(const char *input, int *numbers, int *count);
static void handleCustomDelimiter(const char **input, char *delimiter);
static void checkForNegatives(int *numbers, int count);
static void collectNegatives(int *numbers, int count, char *buffer, int *negativeCount);
static void appendNegative(char *buffer, int negativeNumber, int *negativeCount);
static void printNegativeError(char *buffer);
static void processLine(char *line, int *numbers, int *count, char *delimiter);

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

    // Parse input to fill numbers array
    parseInput(input, numbers, &count);

    // Check for negative numbers
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
    char delimiter[2] = {',', '\0'}; // Default delimiter

    if (input[0] == '/') { // Handle custom delimiter
        handleCustomDelimiter(&input, delimiter);
    }

    // Split input by newlines and process each line
    char *line = strtok((char *)input, "\n");
    while (line != NULL) {
        processLine(line, numbers, count, delimiter);
        line = strtok(NULL, "\n");
    }
}

/**
 * @brief Processes each line of the input to extract numbers.
 * 
 * This function splits the line using the specified delimiter and converts 
 * the tokens into integers, storing them in the provided array.
 * 
 * @param line The line of input to process.
 * @param numbers The array to store the extracted numbers.
 * @param count Pointer to the integer that keeps track of the number of extracted numbers.
 * @param delimiter The delimiter used to split the line.
 */
static void processLine(char *line, int *numbers, int *count, char *delimiter) {
    char *numToken = strtok(line, delimiter);
    while (numToken != NULL) {
        numbers[(*count)++] = strtol(numToken, NULL, 10);
        numToken = strtok(NULL, delimiter);
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
    // Skip the custom delimiter line
    (*input) += 2; // Skip "//"
    while (**input != '\n' && **input != '\0') {
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
 * @param numbers The array of integers to check.
 * @param count The number of elements in the array.
 */
static void checkForNegatives(int *numbers, int count) {
    char buffer[256] = "Exception: negatives not allowed: ";
    int negativeCount = 0;

    // Collect negative numbers
    collectNegatives(numbers, count, buffer, &negativeCount);

    // Print error if negatives were found
    if (negativeCount > 0) {
        printNegativeError(buffer);
    }
}

/**
 * @brief Collects negative numbers from the provided array.
 * 
 * This function scans through the numbers and appends any negative numbers 
 * to the provided buffer.
 * 
 * @param numbers The array of integers to check.
 * @param count The number of elements in the array.
 * @param buffer The buffer to store the error message.
 * @param negativeCount Pointer to keep track of the count of negative numbers found.
 */
static void collectNegatives(int *numbers, int count, char *buffer, int *negativeCount) {
    for (int i = 0; i < count; i++) {
        if (numbers[i] < 0) {
            appendNegative(buffer, numbers[i], negativeCount);
        }
    }
}

/**
 * @brief Appends a negative number to the error message buffer.
 * 
 * This function handles formatting when adding a negative number to the buffer.
 * 
 * @param buffer The buffer to store the error message.
 * @param negativeNumber The negative number to append.
 * @param negativeCount Pointer to keep track of the count of negative numbers found.
 */
static void appendNegative(char *buffer, int negativeNumber, int *negativeCount) {
    if (*negativeCount > 0) {
        strcat(buffer, ", "); // Append comma before the next negative number
    }
    sprintf(buffer + strlen(buffer), "%d", negativeNumber); // Add negative number to buffer
    (*negativeCount)++;
}

/**
 * @brief Prints an error message if negative numbers are found.
 * 
 * This function outputs the error message to stderr and exits the program.
 * 
 * @param buffer The error message containing the negative numbers.
 */
static void printNegativeError(char *buffer) {
    fprintf(stderr, "%s\n", buffer);
    exit(EXIT_FAILURE); // Exit on negative number
}
