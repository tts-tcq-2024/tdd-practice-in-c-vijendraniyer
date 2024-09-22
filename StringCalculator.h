#ifndef STRING_CALCULATOR_H
#define STRING_CALCULATOR_H

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
static void collectNegatives(int *numbers, int count, char *buffer);

// Function to add numbers from a string
int add(const char *input) {
    int numbers[MAX_NUMBERS];
    int count = 0;

    parseInput(input, numbers, &count);
    checkForNegatives(numbers, count);
    return calculateSum(numbers, count);
}

// Function to calculate the sum of an array of numbers
static int calculateSum(const int *numbers, int count) {
    int sum = 0;
    for (int i = 0; i < count; i++) {
        if (numbers[i] <= 1000) {
            sum += numbers[i];
        }
    }
    return sum;
}

// Function to parse the input string and extract numbers
static void parseInput(const char *input, int *numbers, int *count) {
    char *token;
    char *end;
    char delimiter[2] = {',', '\0'}; // Default delimiter

    if (input[0] == '/') {
        handleCustomDelimiter(&input, delimiter);
    }

    token = strtok((char *)input, "\n");
    while (token != NULL) {
        char *numToken = strtok(token, delimiter);
        while (numToken != NULL) {
            if (*count < MAX_NUMBERS) {
                numbers[(*count)++] = strtol(numToken, &end, 10);
            }
            numToken = strtok(NULL, delimiter);
        }
        token = strtok(NULL, "\n");
    }
}

// Function to handle custom delimiters in the input string
static void handleCustomDelimiter(const char **input, char *delimiter) {
    (*input) += 2; // Skip "//"
    delimiter[0] = '\0'; // Initialize delimiter as empty string

    while (**input != '\n' && **input != '\0') {
        strncat(delimiter, *input, 1);
        (*input)++;
    }
    (*input)++; // Skip the newline character
}

// Function to check for negative numbers
static void checkForNegatives(int *numbers, int count) {
    char buffer[256] = "Exception: negatives not allowed: ";
    collectNegatives(numbers, count, buffer);
}

// Helper function to collect negative numbers
static void collectNegatives(int *numbers, int count, char *buffer) {
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
        exit(EXIT_FAILURE);
    }
}

#endif // STRING_CALCULATOR_H
