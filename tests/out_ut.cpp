#include "utils.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(OutTest, OutTestEmptyInput) {
    testing::internal::CaptureStdout();
    std::vector<int> emptyInput;
    AsDataFlow(emptyInput) | Out(std::cout);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "");
}

TEST(OutTest, OutTestSingleElement) {
    testing::internal::CaptureStdout();
    std::vector<int> single = {42};
    AsDataFlow(single) | Out(std::cout);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "42");
}

TEST(OutTest, OutTestMultipleElements) {
    testing::internal::CaptureStdout();
    std::vector<int> testData = {10, 20, 30};
    AsDataFlow(testData) | Out(std::cout);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "10 20 30");
}
