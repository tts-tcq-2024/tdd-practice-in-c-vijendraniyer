#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NUMBERS 1000 // Maximum number of integers that can be processed

// Function declarations
int add(const char *input);
static int calculateSum(const int *numbers, int count);
static void parseInput(const char *input, int *numbers, int *count);
static char* sanitizeInput(const char *input, char *delimiter);
static void handleCustomDelimiter(const char **input, char *delimiter);
static void setDefaultDelimiter(char *delimiter);
static void checkForNegatives(int *numbers, int count);
static int collectNegatives(int *numbers, int count, char *buffer);
static void appendNegativeError(char *buffer, int negativeNumber);
static void processTokens(char *line, int *numbers, int *count, const char *delimiter);

// Function definitions

int add(const char *input) {
    int numbers[MAX_NUMBERS];
    int count = 0;

    parseInput(input, numbers, &count);
    checkForNegatives(numbers, count);
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
    char delimiter[10];
    setDefaultDelimiter(delimiter);
    
    char *modifiableInput = sanitizeInput(input, delimiter);
    if (!modifiableInput) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    
    processTokens(modifiableInput, numbers, count, delimiter);
    free(modifiableInput);
}

static char* sanitizeInput(const char *input, char *delimiter) {
    char *modifiableInput = strdup(input);
    if (!modifiableInput) {
        return NULL;
    }

    if (input[0] == '/') {
        handleCustomDelimiter(&input, delimiter);
    }
    
    for (char *p = modifiableInput; *p; p++) {
        if (*p == '\n') {
            *p = ',';
        }
    }

    return modifiableInput;
}

static void setDefaultDelimiter(char *delimiter) {
    delimiter[0] = ',';
    delimiter[1] = '\0';
}

static void handleCustomDelimiter(const char **input, char *delimiter) {
    *input += 2; // Skip "//"
    if (**input == '[') {
        (*input)++;
        while (**input != ']' && **input != '\0') {
            strncat(delimiter, *input, 1);
            (*input)++;
        }
        if (**input == ']') (*input)++;
    } else {
        strncat(delimiter, *input, 1);
        (*input)++;
    }
    if (**input == '\n') (*input)++;
}

static void checkForNegatives(int *numbers, int count) {
    char buffer[256] = "Exception: negatives not allowed: ";
    if (collectNegatives(numbers, count, buffer)) {
        fprintf(stderr, "%s\n", buffer);
        exit(EXIT_FAILURE);
    }
}

static int collectNegatives(int *numbers, int count, char *buffer) {
    int negativeCount = 0;
    for (int i = 0; i < count; i++) {
        if (numbers[i] < 0) {
            if (negativeCount > 0) {
                strcat(buffer, ", ");
            }
            appendNegativeError(buffer, numbers[i]);
            negativeCount++;
        }
    }
    return negativeCount;
}

static void appendNegativeError(char *buffer, int negativeNumber) {
    char numStr[12];
    sprintf(numStr, "%d", negativeNumber);
    strcat(buffer, numStr);
}

static void processTokens(char *line, int *numbers, int *count, const char *delimiter) {
    char *numToken = strtok(line, delimiter);
    while (numToken != NULL) {
        if (*count >= MAX_NUMBERS) {
            fprintf(stderr, "Error: Too many numbers provided.\n");
            exit(EXIT_FAILURE);
        }
        numbers[(*count)++] = strtol(numToken, NULL, 10);
        numToken = strtok(NULL, delimiter);
    }
}
