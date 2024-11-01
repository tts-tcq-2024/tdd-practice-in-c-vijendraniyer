#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int isDigit(char character) {
    if(character >= '0' && character <= '9') {
        return 1;
    }
    else {
        return 0;
    }
}

void throwNegativeException(int negatives[], int count) {
    printf("Exception: negatives not allowed ");
    for (int i = 0; i < count; i++) {
        printf("%d ", negatives[i]);
    }
    printf("\n");
    exit(1);
}

int shouldInsertComma(char currentChar, char nextChar) {
    return isDigit(currentChar) && isDigit(nextChar);
}

void insertCommasBetweenDigits(char *numbers) {
    char temp[512];
    int j = 0;

    for (int i = 0; numbers[i] != '\0'; i++) {
        temp[j++] = numbers[i];
        if (shouldInsertComma(numbers[i], numbers[i + 1])) {
            temp[j++] = ',';  
        }
    }
    temp[j] = '\0';
    strcpy(numbers, temp);  
}

int checkIfNextCharIsNotNumber(char nextChar) {
    if((nextChar == '\0' || !isDigit(nextChar))) {
        return 1;
    }
    else {
        return 0;
    }
}

int hasInvalidSequence(char currentChar, char nextChar, char character) {
    return currentChar == character && checkIfNextCharIsNotNumber(nextChar);
}

int isInputInvalid(int i, const char* numbers) {
    if(hasInvalidSequence(numbers[i], numbers[i + 1],',') || hasInvalidSequence(numbers[i], numbers[i + 1],'\n')) {
        return 1;
    }
    else {
        return 0;
    }
}

void validateInput(const char *numbers) {
    for (int i = 0; numbers[i] != '\0'; i++) {
        if (isInputInvalid(i,numbers)) {
            printf("Error: Invalid input sequence at position %d.\n", i);
            exit(1);
        }
    }
}

int checkNoOfDigits(const char* input, int i) {
    if (input[i] >= '0' && input[i] <= '9') {
        return 1;
    }
    else {
        return 0;
    }
}

int parseDigits(const char* input) {
    int containsDigit = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (checkNoOfDigits(input,i)) {
            containsDigit = 1;
        }
    }
    return !containsDigit;
}

int isNonNumericOrEmptyInput(const char *input) {
    if (strlen(input) == 0) {
        return 1;
    }
    return parseDigits(input);
}

int isCustomDelimiter(char *stringCopy) {
    return strncmp(stringCopy, "//", 2) == 0;
}

int parseInputIfValid(char *start, char *end) {
    if(end > start) {
        return 1;
    }
    return 0;
}

int parseInputIfNotNull(char *start, char *end) {
    if(start != NULL && end != NULL) {
        return 1;
    }
    return 0;
}

int parseInput(char *start, char *end) {
    if(parseInputIfNotNull(start,end) && parseInputIfValid(start,end)) {
        return 1;
    }
    else {
        return 0;
    }
}

void extractCustomDelimiter(char *stringCopy, char *delimiter) {
    char *start = strchr(stringCopy, '[');
    char *end = strchr(stringCopy, ']');
    if (parseInput(start,end)) {
        strncpy(delimiter, start + 1, end - start - 1);
        delimiter[end - start - 1] = '\0';
    }
}

void handleEmptyDelimiter(char *numbers, char *delimiter) {
    if (strcmp(delimiter, "") == 0) {
        insertCommasBetweenDigits(numbers);
        strcpy(delimiter, ",");
    }
}

int processNegativeNumber(int number, int negatives[], int *negativeCount) {
    if (number < 0) {
        negatives[(*negativeCount)++] = number;
        return 0;
    }
    return 1;
}

void addToSumIfValid(int number, int *sum) {
    if (number <= 1000) {
        *sum += number;
    }
}

void validateCustomDelimiter(char *stringCopy, char **numbers, char *delimiter) {
    if (isCustomDelimiter(stringCopy)) {
        extractCustomDelimiter(stringCopy, delimiter);
        *numbers = strchr(stringCopy, '\n') + 1;
        handleEmptyDelimiter(*numbers, delimiter);
    }
}

void HandleNegatives(int negativeCount, int negatives[]) {
    if (negativeCount > 0) {
        throwNegativeException(negatives, negativeCount);
    }
}

int processTokenToInteger(char *numbers, char delimiter[]) {
    char *token = strtok(numbers, delimiter);
    int sum = 0;
    int negatives[256];
    int negativeCount = 0;
    while (token != NULL) {
        int number = atoi(token);
        if (!processNegativeNumber(number, negatives, &negativeCount)) {
            token = strtok(NULL, delimiter);
            continue;
        }
        addToSumIfValid(number, &sum);
        token = strtok(NULL, delimiter);
    }
    HandleNegatives(negativeCount,negatives);
    return sum;
}

int add(const char *stringInput) {
    if (isNonNumericOrEmptyInput(stringInput)) {
        return 0;
    }
    char stringCopy[512];
    strcpy(stringCopy, stringInput);
    char delimiter[256] = ",\n;*";
    char *numbers = stringCopy;
    validateCustomDelimiter(stringCopy, &numbers, delimiter);
    validateInput(numbers);
    return processTokenToInteger(numbers,delimiter);
}
