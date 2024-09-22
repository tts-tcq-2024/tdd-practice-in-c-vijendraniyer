#include <gtest/gtest.h> // Include Google Test framework
#include "StringCalculator.h" // Include the String Calculator header

// Test case for an empty string
TEST(StringCalculatorTest, TestEmptyString_ReturnsZero) {
    EXPECT_EQ(add(""), 0); // Expect the output to be 0
}

// Test case for a single number
TEST(StringCalculatorTest, TestSingleNumber_ReturnsSameNumber) {
    EXPECT_EQ(add("1"), 1); // Expect the output to be the same single number
}

// Test case for two numbers
TEST(StringCalculatorTest, TestTwoNumbers_ReturnsSum) {
    EXPECT_EQ(add("1,2"), 3); // Expect the output to be the sum of 1 and 2
}

// Test case for multiple numbers
TEST(StringCalculatorTest, TestMultipleNumbers_ReturnsSum) {
    EXPECT_EQ(add("1,2,3,4,5"), 15); // Expect the output to be the sum of the numbers
}

// Test case for new line as delimiter
TEST(StringCalculatorTest, TestNewLineDelimiter_ReturnsSum) {
    EXPECT_EQ(add("1\n2,3"), 6); // Expect the output to sum numbers separated by newlines and commas
}

// Test case for custom delimiter
TEST(StringCalculatorTest, TestCustomDelimiter_ReturnsSum) {
    EXPECT_EQ(add("//;\n1;2"), 3); // Expect the output to be the sum using a custom delimiter
}

// Test case for custom multi-character delimiter
TEST(StringCalculatorTest, TestCustomMultiCharacterDelimiter) {
    EXPECT_EQ(add("//[***]\n1***2***3"), 6); // Expect the output to be the sum using a multi-character delimiter
}

// Test case for a negative number
TEST(StringCalculatorTest, TestNegativeNumber_ThrowsException) {
    char buffer[256]; // Buffer to store the expected output message
    snprintf(buffer, sizeof(buffer), "Exception: negatives not allowed: -2"); // Format the expected message
    testing::internal::CaptureStdout(); // Capture standard output
    ASSERT_EQ(add("1,-2,3"), -1); // Expect the function to return -1 due to a negative number
    std::string output = testing::internal::GetCapturedStdout(); // Get the captured output
    EXPECT_EQ(output, buffer); // Check if the output matches the expected message
}

// Test case for multiple negative numbers
TEST(StringCalculatorTest, TestMultipleNegatives_ThrowsException) {
    char buffer[256]; // Buffer for expected output
    snprintf(buffer, sizeof(buffer), "Exception: negatives not allowed: -2, -3"); // Expected message
    testing::internal::CaptureStdout(); // Capture output
    ASSERT_EQ(add("1,-2,-3"), -1); // Expect -1 due to multiple negative numbers
    std::string output = testing::internal::GetCapturedStdout(); // Get output
    EXPECT_EQ(output, buffer); // Check the output
}

// Test case for ignoring numbers greater than 1000
TEST(StringCalculatorTest, TestIgnoreLargeNumbers) {
    EXPECT_EQ(add("2,1001"), 2); // Expect the output to ignore 1001 and return 2
}

// Test case for mixed delimiters
TEST(StringCalculatorTest, TestMixedDelimiters_ReturnsSum) {
    EXPECT_EQ(add("//;\n1;1001\n2,3"), 6); // Expect the sum to ignore 1001 and add 1 and 2 and 3
}

// Test case for invalid input ending with a comma
TEST(StringCalculatorTest, TestInvalidEndingWithComma_ThrowsException) {
    testing::internal::CaptureStdout(); // Capture output for validation
    ASSERT_EQ(add("1,\n"), -1); // Expect -1 for invalid input
    std::string output = testing::internal::GetCapturedStdout(); // Retrieve captured output
    EXPECT_EQ(output, "Exception: Invalid input\n"); // Check if the output matches the expected message
}

// Main function to run all test cases
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); // Initialize Google Test
    return RUN_ALL_TESTS(); // Execute all tests
}
