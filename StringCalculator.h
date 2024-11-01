#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NUMBERS 1000 // Maximum number of integers that can be processed

// Function declarations
int add(const char *input);
static int calculateSum(const int *numbers, int count);
static void parseInput(const char *input, int *numbers, int *count);
static void handleCustomDelimiter(const char **input, char *delimiter);
static void checkForNegatives(int *numbers, int count);
static void collectNegatives(int *numbers, int count, char *buffer, int *negativeCount);
static void appendNegative(char *buffer, int negativeNumber, int *negativeCount);
static void printNegativeError(char *buffer);
static void processTokens(char *line, int *numbers, int *count, const char *delimiter);

// Function definitions

int add(const char *input) {
    int numbers[MAX_NUMBERS]; // Array to hold extracted numbers
    int count = 0; // Count of numbers extracted

    // Parse input to fill numbers array
    parseInput(input, numbers, &count);

    // Check for negative numbers
    checkForNegatives(numbers, count);

    // Calculate and return the sum of valid numbers
    return calculateSum(numbers, count);
}

static int calculateSum(const int *numbers, int count) {
    int sum = 0;
    for (int i = 0; i < count; i++) {
        if (numbers[i] <= 1000) {
            sum += numbers[i];
        }
    }
    return sum;
}

static void parseInput(const char *input, int *numbers, int *count) {
    char delimiter[10] = ","; // Default delimiter is comma

    // Handle custom delimiter if present
    if (input[0] == '/') {
        handleCustomDelimiter(&input, delimiter);
    }

    // Make a mutable copy of the input for processing
    char *modifiableInput = strdup(input);
    if (!modifiableInput) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Replace newlines with commas for consistent processing
    for (char *p = modifiableInput; *p; p++) {
        if (*p == '\n') {
            *p = ','; // Replace newline with comma
        }
    }

    // Process tokens with the modified delimiter
    processTokens(modifiableInput, numbers, count, delimiter);

    free(modifiableInput); // Free the allocated memory
}

static void processTokens(char *line, int *numbers, int *count, const char *delimiter) {
    char *numToken = strtok(line, delimiter); // Tokenize using the specified delimiter
    while (numToken != NULL) {
        if (*count >= MAX_NUMBERS) {
            fprintf(stderr, "Error: Too many numbers provided.\n");
            exit(EXIT_FAILURE);
        }
        numbers[(*count)++] = strtol(numToken, NULL, 10); // Convert to int and store
        numToken = strtok(NULL, delimiter); // Get next token
    }
}

// Custom delimiter handling and negative number checking functions remain unchanged.

static void handleCustomDelimiter(const char **input, char *delimiter) {
    (*input) += 2; // Move past "//"
    if (**input == '[') {
        (*input)++; // Skip the opening bracket
        while (**input != ']' && **input != '\0') {
            strncat(delimiter, *input, 1);
            (*input)++;
        }
        if (**input == ']') {
            (*input)++; // Skip the closing bracket
        }
    } else {
        strncat(delimiter, *input, 1);
        (*input)++;
    }
    if (**input == '\n') {
        (*input)++;
    }
}

static void checkForNegatives(int *numbers, int count) {
    char buffer[256] = "Exception: negatives not allowed: "; // Buffer for error message
    int negativeCount = 0;
    for (int i = 0; i < count; i++) {
        if (numbers[i] < 0) {
            if (negativeCount > 0) {
                strcat(buffer, ", ");
            }
            char numStr[12];
            sprintf(numStr, "%d", numbers[i]);
            strcat(buffer, numStr);
            negativeCount++;
        }
    }
    if (negativeCount > 0) {
        fprintf(stderr, "%s\n", buffer);
        exit(EXIT_FAILURE);
    }
}
