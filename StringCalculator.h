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
    char delimiter[10] = {',', '\n', '\0'}; // Default delimiter includes newline

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
    int negativeCount = 0; // Counter for negative numbers

    // Collect negative numbers
    collectNegatives(numbers, count, buffer, &negativeCount);

    // Print error if negatives were found
    if (negativeCount > 0) {
        printNegativeError(buffer); // Report negative numbers
    }
}

static void collectNegatives(int *numbers, int count, char *buffer, int *negativeCount) {
    for (int i = 0; i < count; i++) {
        if (numbers[i] < 0) {
            appendNegative(buffer, numbers[i], negativeCount); // Append negative number to buffer
        }
    }
}

static void appendNegative(char *buffer, int negativeNumber, int *negativeCount) {
    if (*negativeCount > 0) {
        strcat(buffer, ", "); // Append comma before the next negative number
    }
    sprintf(buffer + strlen(buffer), "%d", negativeNumber); // Add negative number to buffer
    (*negativeCount)++;
}

static void printNegativeError(char *buffer) {
    fprintf(stderr, "%s\n", buffer);
    exit(EXIT_FAILURE); // Exit on negative number
}
