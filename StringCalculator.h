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
static void processLine(char *line, int *numbers, int *count, const char *delimiter);
static void splitLines(char *input, int *numbers, int *count, const char *delimiter);
static void processTokens(char *line, int *numbers, int *count, const char *delimiter);
static void extractMultiCharDelimiter(const char **input, char *delimiter);
static void appendToDelimiter(const char **input, char *delimiter);
static void skipClosingBracket(const char **input);
static void extractSingleCharDelimiter(const char **input, char *delimiter);

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
    char delimiter[10] = {',', '\0'}; // Default delimiter, allowing for longer delimiters

    // Handle custom delimiter if present
    if (input[0] == '/') {
        handleCustomDelimiter(&input, delimiter);
    }

    // Make a mutable copy of the input for strtok
    char *modifiableInput = strdup(input); // Create a mutable copy
    if (!modifiableInput) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Split input by lines and process each line
    splitLines(modifiableInput, numbers, count, delimiter);

    free(modifiableInput); // Free the allocated memory
}

static void splitLines(char *input, int *numbers, int *count, const char *delimiter) {
    char *line = strtok(input, "\n");
    while (line != NULL) {
        processTokens(line, numbers, count, delimiter); // Process each line's tokens
        line = strtok(NULL, "\n"); // Get next line
    }
}

static void processTokens(char *line, int *numbers, int *count, const char *delimiter) {
    char *numToken = strtok(line, delimiter); // Tokenize using the specified delimiter
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

static void extractMultiCharDelimiter(const char **input, char *delimiter) {
    (*input)++; // Skip the opening bracket
    appendToDelimiter(input, delimiter);
    skipClosingBracket(input);
}

static void appendToDelimiter(const char **input, char *delimiter) {
    while (**input != ']' && **input != '\0') {
        strncat(delimiter, *input, 1);
        (*input)++;
    }
}

static void skipClosingBracket(const char **input) {
    if (**input == ']') {
        (*input)++; // Skip the closing bracket
    }
}

static void extractSingleCharDelimiter(const char **input, char *delimiter) {
    while (**input != '\n' && **input != '\0') {
        strncat(delimiter, *input, 1);
        (*input)++;
    }
}

static void checkForNegatives(int *numbers, int count) {
    char buffer[256] = "Exception: negatives not allowed: "; // Buffer for error message
    int negativeCount = collectNegatives(numbers, count, buffer);

    // Print error if negatives were found
    if (negativeCount > 0) {
        // Ensure the message is correctly formatted
        if (negativeCount == 1) {
            // Remove the trailing comma and space for a single negative
            buffer[strlen(buffer) - 2] = '\0'; // Remove the last comma and space
        }
        fprintf(stderr, "%s\n", buffer);
        exit(EXIT_FAILURE); // Exit on negative number
    }
}

static int collectNegatives(int *numbers, int count, char *buffer) {
    int negativeCount = 0;
    buffer[strlen(buffer)] = '\0'; // Ensure buffer is null-terminated
    for (int i = 0; i < count; i++) {
        if (numbers[i] < 0) {
            appendNegativeError(buffer, numbers[i]);
            negativeCount++;
        }
    }
    return negativeCount;
}

static void appendNegativeError(char *buffer, int negativeNumber) {
    if (strstr(buffer, "negatives not allowed:") == NULL) {
        // First negative, add the number
        strcat(buffer, "-"); // Start with a hyphen for the first negative
    } else {
        strcat(buffer, ", "); // Add a comma for subsequent negatives
    }
    char numStr[12];
    sprintf(numStr, "%d", negativeNumber);
    strcat(buffer, numStr);
}
