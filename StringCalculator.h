#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int isDigitChar(char character) {
    return (character >= '0' && character <= '9');
}

void raiseNegativeValueException(int negatives[], int count) {
    printf("Exception: negatives not allowed ");
    for (int i = 0; i < count; i++) {
        printf("%d ", negatives[i]);
    }
    printf("\n");
    exit(1);
}

int shouldInsertCommaBetweenDigits(char currentChar, char nextChar) {
    return isDigitChar(currentChar) && isDigitChar(nextChar);
}

void insertCommasBetweenNumericCharacters(char *numbers) {
    char temp[512];
    int j = 0;

    for (int i = 0; numbers[i] != '\0'; i++) {
        temp[j++] = numbers[i];
        if (shouldInsertCommaBetweenDigits(numbers[i], numbers[i + 1])) {
            temp[j++] = ',';  
        }
    }
    temp[j] = '\0';
    strcpy(numbers, temp);  
}

int isNextCharacterNotNumeric(char nextChar) {
    return (nextChar == '\0' || !isDigitChar(nextChar));
}

int hasInvalidCharacterSequence(char currentChar, char nextChar, char character) {
    return currentChar == character && isNextCharacterNotNumeric(nextChar);
}

int isInputSequenceInvalid(int i, const char* numbers) {
    return (hasInvalidCharacterSequence(numbers[i], numbers[i + 1], ',') || 
            hasInvalidCharacterSequence(numbers[i], numbers[i + 1], '\n'));
}

void validateInputSequence(const char *numbers) {
    for (int i = 0; numbers[i] != '\0'; i++) {
        if (isInputSequenceInvalid(i, numbers)) {
            printf("Error: Invalid input sequence at position %d.\n", i);
            exit(1);
        }
    }
}

int isDigitPresentInInput(const char* input, int i) {
    return (input[i] >= '0' && input[i] <= '9');
}

int checkForAnyDigits(const char* input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (isDigitPresentInInput(input, i)) {
            return 1; // Contains digit
        }
    }
    return 0; // No digits found
}

int isInputEmptyOrNonNumeric(const char *input) {
    return (strlen(input) == 0 || checkForAnyDigits(input) == 0);
}

int isCustomDelimiterDefined(char *stringCopy) {
    return strncmp(stringCopy, "//", 2) == 0;
}

int isInputRangeValid(char *start, char *end) {
    return (end > start);
}

int areInputPointersValid(char *start, char *end) {
    return (start != NULL && end != NULL);
}

int isInputValid(char *start, char *end) {
    return (areInputPointersValid(start, end) && isInputRangeValid(start, end));
}

void extractCustomDelimiter(char *stringCopy, char *delimiter) {
    char *start = strchr(stringCopy, '[');
    char *end = strchr(stringCopy, ']');
    if (isInputValid(start, end)) {
        strncpy(delimiter, start + 1, end - start - 1);
        delimiter[end - start - 1] = '\0';
    }
}

void handleEmptyDelimiterInInput(char *numbers, char *delimiter) {
    if (strcmp(delimiter, "") == 0) {
        insertCommasBetweenNumericCharacters(numbers);
        strcpy(delimiter, ",");
    }
}

int checkAndStoreNegativeValue(int number, int negatives[], int *negativeCount) {
    if (number < 0) {
        negatives[(*negativeCount)++] = number;
        return 0; // Negative found
    }
    return 1; // No negatives
}

void addValidNumberToTotal(int number, int *total) {
    if (number <= 1000) {
        *total += number;
    }
}

void validateCustomDelimiterSetup(char *stringCopy, char **numbers, char *delimiter) {
    if (isCustomDelimiterDefined(stringCopy)) {
        extractCustomDelimiter(stringCopy, delimiter);
        *numbers = strchr(stringCopy, '\n') + 1;
        handleEmptyDelimiterInInput(*numbers, delimiter);
    }
}

void checkForNegativeValues(int negativeCount, int negatives[]) {
    if (negativeCount > 0) {
        raiseNegativeValueException(negatives, negativeCount);
    }
}

int convertTokenToIntegerAndSum(char *numbers, char delimiter[]) {
    char *token = strtok(numbers, delimiter);
    int total = 0;
    int negatives[256];
    int negativeCount = 0;
    while (token != NULL) {
        int number = atoi(token);
        if (!checkAndStoreNegativeValue(number, negatives, &negativeCount)) {
            token = strtok(NULL, delimiter);
            continue;
        }
        addValidNumberToTotal(number, &total);
        token = strtok(NULL, delimiter);
    }
    checkForNegativeValues(negativeCount, negatives);
    return total;
}

int add(const char *stringInput) {
    if (isInputEmptyOrNonNumeric(stringInput)) {
        return 0;
    }
    char stringCopy[512];
    strcpy(stringCopy, stringInput);
    char delimiter[256] = ",\n;*";
    char *numbers = stringCopy;
    validateCustomDelimiterSetup(stringCopy, &numbers, delimiter);
    validateInputSequence(numbers);
    return convertTokenToIntegerAndSum(numbers, delimiter);
}
