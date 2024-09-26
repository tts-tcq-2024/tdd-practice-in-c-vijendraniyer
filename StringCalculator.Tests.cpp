#include "StringCalculator.h"
#include <gtest/gtest.h>

// Test for empty string
TEST(StringCalculatorTest, TestEmptyString_ReturnsZero) {
    EXPECT_EQ(add(""), 0);
}

// Test for a single number
TEST(StringCalculatorTest, TestSingleNumber_ReturnsSameNumber) {
    EXPECT_EQ(add("1"), 1);
}

// Test for two numbers
TEST(StringCalculatorTest, TestTwoNumbers_ReturnsSum) {
    EXPECT_EQ(add("1,2"), 3);
}

// Test for multiple numbers
TEST(StringCalculatorTest, TestMultipleNumbers_ReturnsSum) {
    EXPECT_EQ(add("1,2,3,4,5"), 15);
}

// Test for new line as a delimiter
TEST(StringCalculatorTest, TestNewLineDelimiter_ReturnsSum) {
    EXPECT_EQ(add("1\n2,3"), 6);
}

// Test for custom delimiter
TEST(StringCalculatorTest, TestCustomDelimiter_ReturnsSum) {
    EXPECT_EQ(add("//;\n1;2"), 3);
}

// Test for custom multi-character delimiter
TEST(StringCalculatorTest, TestCustomMultiCharacterDelimiter) {
    EXPECT_EQ(add("//[***]\n1***2***3"), 6);
}

// Test for a negative number
TEST(StringCalculatorTest, TestNegativeNumber_ThrowsException) {
    testing::internal::CaptureStderr();
    EXPECT_EQ(add("1,-2,3"), -1);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "Exception: negatives not allowed: -2\n");
}

// Test for multiple negative numbers
TEST(StringCalculatorTest, TestMultipleNegatives_ThrowsException) {
    testing::internal::CaptureStderr();
    EXPECT_EQ(add("1,-2,-3"), -1);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "Exception: negatives not allowed: -2, -3\n");
}

// Test for ignoring large numbers
TEST(StringCalculatorTest, TestIgnoreLargeNumbers) {
    EXPECT_EQ(add("2,1001"), 2);
}

// Test for mixed delimiters
TEST(StringCalculatorTest, TestMixedDelimiters_ReturnsSum) {
    EXPECT_EQ(add("//;\n1;1001\n2,3"), 6);
}

// Test for invalid input ending with a comma
TEST(StringCalculatorTest, TestInvalidEndingWithComma_ThrowsException) {
    testing::internal::CaptureStderr();
    EXPECT_EQ(add("1,\n"), -1);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "Exception: Invalid input\n");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
