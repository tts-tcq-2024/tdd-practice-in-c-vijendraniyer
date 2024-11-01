#include <gtest/gtest.h>
#include "StringCalculator.h"

TEST(TC001, ExpectZeroForEmptyString) {
    int expectedresult = 0;
    const char* input = "";
    int result = add(input);
    ASSERT_EQ(result, expectedresult);
}

TEST(TC002, ExpectZeroForEmptyInput) {
    int expectedresult = 0;
    const char* input = "Hello, world!";
    int result = add(input);
    ASSERT_EQ(result, expectedresult);
}

TEST(TC003, ExpectZeroForSingleZero) {
    int expectedresult = 0;
    const char* input = "0";
    int result = add(input);
    ASSERT_EQ(result, expectedresult);
}

TEST(TC004, ExpectSumForNumbersSeparatedbyValidDelimiter) {
    int expectedresult = 3;
    int expectedresult2 = 23;
    const char*  input = "1,2";
    const char* input2 = "3;5;7;8";
    int result = add(input);
    int result2 = add(input2);
    ASSERT_EQ(result, expectedresult);
    ASSERT_EQ(result2, expectedresult2);
}

TEST(TC005, ExpectSumWithNewlineDelimiter) {
    int expectedresult = 6;
    const char*  input = "1\n2,3";
    int result =add(input);
    ASSERT_EQ(result, expectedresult);
}

TEST(TC006, IgnoreNumbersGreaterThan1000) {
    int expectedresult = 1;
    int expectedresult2 = 0;
    const char*  input = "1,1001";
    const char* input2 = "2001;9087";
    int result =add(input);
    int result2=add(input2);
    ASSERT_EQ(result, expectedresult);
    ASSERT_EQ(result2, expectedresult2);
}

TEST(TC007, ExpectSumWithCustomDelimiter) {
    int expectedresult = 3;
    const char*  input = "//;\n1;2";
    int result = add(input);
    ASSERT_EQ(result, expectedresult);
}

TEST(TC008, ExpectSumWithAnyLengthofDelimiters) {
    int expectedresult = 6;
    const char*  input = "//[***]\n1***2***3";
    int result = add(input);
    ASSERT_EQ(result, expectedresult);
}
