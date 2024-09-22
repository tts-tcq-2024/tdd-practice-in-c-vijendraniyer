#ifndef STRING_CALCULATOR_H
#define STRING_CALCULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NUMBERS 1024    // Maximum numbers we might handle
#define MAX_DELIMITER 16    // Maximum length for custom delimiters
#define EMPTY_STRING ""      // Represents an empty string

// Function Declarations
int add(const char *numbers);
void process_custom_delimiter(char **input, char *delimiters);
int parse_and_sum(const char *input, const char *delimiters, int *sum, char *negatives);
void init_negatives_buffer(char *negatives);

/**
 * @brief Sums numbers from a string with various delimiters.
 *
 * This function processes a string containing numbers separated by 
 * commas, newlines, or custom delimiters. It returns the sum of 
 * numbers less than or equal to 1000. If negative numbers are 
 * present, it returns -1 along with a message listing those numbers.
 *
 * @param numbers A string containing the numbers to be summed.
 * @return 
 *   - The sum of the numbers (<= 1000) 
 *   - -1 if any negative numbers are found, with details provided.
 */
int add(const char *numbers) {
    char delimiters[MAX_DELIMITER] = ",";  // Default delimiter is a comma
    char *input = NULL;                     // Pointer for the input string
    int sum = 0;                            // Variable to store the cumulative sum
    char negatives[MAX_NUMBERS];             // Buffer to store negative numbers

    // If the input string is empty, return 0
    if (strcmp(numbers, EMPTY_STRING) == 0) {
        return 0;
    }

    input = strdup(numbers);  // Duplicate input string for tokenization
    if (input == NULL) {
        return -1; // Handle memory allocation failure (simplified)
    }

    // Check for invalid trailing comma or newline
    if (input[strlen(input) - 1] == ',' || input[strlen(input) - 1] == '\n') {
        free(input);
        printf("Exception: Invalid input\n");
        return -1; // Return an error for invalid input
    }

    process_custom_delimiter(&input, delimiters); // Check for custom delimiters
    int negative_found = parse_and_sum(input, delimiters, &sum, negatives); // Parse and calculate sum

    free(input);               // Free memory allocated for the input copy
    if (negative_found) {
        printf("Exception: %s\n", negatives); // Print negative numbers if found
        return -1; // Return -1 if negatives found
    }
    return sum; // Return the sum if no negatives found
}

/**
 * @brief Checks for custom delimiters and modifies input string.
 *
 * This function checks if the input string specifies a custom delimiter 
 * and updates the delimiters and input accordingly.
 *
 * @param input A pointer to the input string to process.
 * @param delimiters A string to store the custom delimiters.
 */
void process_custom_delimiter(char **input, char *delimiters) {
    // Check for custom delimiter in the format "//[delimiter]\n"
    if (strncmp(*input, "//", 2) == 0) {
        char *delimiter_end = strchr(*input, '\n');  // Locate the newline character
        if (delimiter_end != NULL) {
            *delimiter_end = '\0';                  // Terminate delimiter string
            strncpy(delimiters, *input + 2, MAX_DELIMITER - 1); // Copy the custom delimiter
            delimiters[MAX_DELIMITER - 1] = '\0';  // Ensure null termination
            *input = delimiter_end + 1;            // Move input pointer to start of numbers
        }
    }
}

/**
 * @brief Parses the input string and calculates the sum.
 *
 * This function tokenizes the input string based on specified delimiters,
 * calculates the sum, and checks for negative numbers.
 *
 * @param input The input string to parse.
 * @param delimiters The delimiters used for tokenization.
 * @param sum Pointer to an integer to store the cumulative sum.
 * @param negatives Buffer to store any negative numbers found.
 * @return 
 *   - 0 if successful,
 *   - 1 if negative numbers are found.
 */
int parse_and_sum(const char *input, const char *delimiters, int *sum, char *negatives) {
    char *token = strtok((char *)input, delimiters); // Tokenize the input string
    int negative_found = 0;                          // Flag to track negative numbers

    init_negatives_buffer(negatives); // Initialize the negatives buffer

    while (token != NULL) {
        int num = atoi(token);                        // Convert the token to an integer
        if (num < 0) {                               // Check for negative numbers
            if (negative_found == 0) {
                strcpy(negatives, "negatives not allowed: "); // Initialize message
            } else {
                strcat(negatives, ", ");             // Append comma for additional negatives
            }
            strcat(negatives, token);                // Append the negative number to the message
            negative_found++;                         // Increment negative count
        } else if (num <= 1000) {                    // Check if the number is ≤ 1000
            *sum += num;                            // Add valid number to the sum
        }
        token = strtok(NULL, delimiters);            // Get the next token
    }

    return negative_found;                            // Return whether negatives were found
}

/**
 * @brief Initializes the negatives buffer.
 *
 * This function prepares the buffer to store negative numbers.
 *
 * @param negatives Buffer to store the negative numbers.
 */
void init_negatives_buffer(char *negatives) {
    negatives[0] = '\0';                             // Initialize the buffer as empty
}

#endif // STRING_CALCULATOR_H

