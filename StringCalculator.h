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
static int collectNegatives(int *numbers, int count, char *buffer);
static void appendNegativeError(char *buffer, int negativeNumber);
static void processTokens(char *input, int *numbers, int *count, const char *delimiter);
static void replaceNewlinesWithCommas(char *input);
static void extractMultiCharDelimiter(const char **input, char *delimiter);
static void appendToDelimiter(const char **input, char *delimiter);
static void skipClosingBracket(const char **input);
static void extractSingleCharDelimiter(const char **input, char *delimiter);
static void checkForInvalidEnding(const char *input);

// Main add function to process the input string
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

// Calculate the sum of the numbers array
static int calculateSum(const int *numbers, int count) {
    int sum = 0;
    for (int i = 0; i < count; i++) {
        if (numbers[i] <= 1000) {
            sum += numbers[i];
        }
    }
    return sum;
}

// Parse input for custom delimiters and split into numbers
static void parseInput(const char *input, int *numbers, int *count) {
    char delimiter[10] = {',', '\0'}; // Default delimiter

    // Handle custom delimiter if present
    if (input[0] == '/') {
        handleCustomDelimiter(&input, delimiter);
    }

    // Replace newlines with commas for tokenization
    char *modifiableInput = strdup(input); // Create a mutable copy
    if (!modifiableInput) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    replaceNewlinesWithCommas(modifiableInput);

    // Check for invalid ending
    checkForInvalidEnding(modifiableInput);

    // Process tokens using the specified delimiter
    processTokens(modifiableInput, numbers, count, delimiter);

    free(modifiableInput); // Free the allocated memory
}

// Replace newlines with commas for consistent tokenization
static void replaceNewlinesWithCommas(char *input) {
    for (char *ptr = input; *ptr; ptr++) {
        if (*ptr == '\n') {
            *ptr = ','; // Replace newline with comma
        }
    }
}

// Check for invalid ending of the input
static void checkForInvalidEnding(const char *input) {
    size_t len = strlen(input);
    // Check if the last character is a delimiter or the string is empty
    if (len == 0 || input[len - 1] == ',' || input[len - 1] == '\n') {
        fprintf(stderr, "Invalid input\n");
        exit(EXIT_FAILURE); // Exit on invalid input
    }
}

// Process tokens based on the delimiter
static void processTokens(char *input, int *numbers, int *count, const char *delimiter) {
    char *numToken = strtok(input, delimiter); // Tokenize using the specified delimiter
    while (numToken != NULL) {
        // Check if count exceeds MAX_NUMBERS to prevent overflow
        if (*count >= MAX_NUMBERS) {
            fprintf(stderr, "Error: Too many numbers provided.\n");
            exit(EXIT_FAILURE);
        }
        numbers[(*count)++] = strtol(numToken, NULL, 10); // Convert to int and store
        numToken = strtok(NULL, delimiter); // Get next token
    }
}

// Handle custom delimiters defined in the input
static void handleCustomDelimiter(const char **input, char *delimiter) {
    (*input) += 2; // Move past "//"

    if (**input == '[') {
        extractMultiCharDelimiter(input, delimiter);
    } else {
        extractSingleCharDelimiter(input, delimiter);
    }

    // Skip the newline character after the delimiter definition
    if (**input == '\n') {
        (*input)++;
    }
}

// Extract a multi-character delimiter from the input
static void extractMultiCharDelimiter(const char **input, char *delimiter) {
    (*input)++; // Skip the opening bracket
    appendToDelimiter(input, delimiter);
    skipClosingBracket(input);
}

// Append characters to the delimiter until the closing bracket is found
static void appendToDelimiter(const char **input, char *delimiter) {
    while (**input != ']' && **input != '\0') {
        strncat(delimiter, *input, 1);
        (*input)++;
    }
}

// Skip the closing bracket of the custom delimiter
static void skipClosingBracket(const char **input) {
    if (**input == ']') {
        (*input)++; // Skip the closing bracket
    }
}

// Extract a single-character delimiter from the input
static void extractSingleCharDelimiter(const char **input, char *delimiter) {
    while (**input != '\n' && **input != '\0') {
        strncat(delimiter, *input, 1);
        (*input)++;
    }
}

// Check for negative numbers in the array
static void checkForNegatives(int *numbers, int count) {
    char buffer[256] = "Exception: negatives not allowed: "; // Buffer for error message
    int negativeCount = collectNegatives(numbers, count, buffer);

    // Print error if negatives were found
    if (negativeCount > 0) {
        fprintf(stderr, "%s\n", buffer);
        exit(EXIT_FAILURE); // Exit on negative number
    }
}

// Collect negative numbers and prepare error message
static int collectNegatives(int *numbers, int count, char *buffer) {
    int negativeCount = 0;
    int buffer_length = strlen(buffer); // Get the initial length of the buffer

    for (int i = 0; i < count; i++) {
        if (numbers[i] < 0) {
            if (negativeCount > 0) {
                strcat(buffer, ", "); // Add comma for subsequent negatives
            }
            appendNegativeError(buffer, numbers[i]); // Append negative number
            negativeCount++;
        }
    }
    return negativeCount;
}

// Append a negative number to the error message buffer
static void appendNegativeError(char *buffer, int negativeNumber) {
    char numStr[12];
    sprintf(numStr, "%d", negativeNumber); // Convert negative number to string
    strcat(buffer, numStr); // Append number to buffer
}
