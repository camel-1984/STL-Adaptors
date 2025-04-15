#include <expected>

#include "processing.h"
#include "split_expected.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(SplitExpectedTest, SplitsExpectedValues) {
    std::vector<std::expected<int, std::string>> input;
    input.emplace_back(42);
    input.emplace_back(std::unexpected("err1"));
    input.emplace_back(100);
    input.emplace_back(std::unexpected("err2"));
    auto [errorFlow, successFlow] = AsDataFlow(input) | SplitExpected();
    ASSERT_EQ(std::distance(errorFlow.begin(), errorFlow.end()), 2);
    ASSERT_EQ(std::distance(successFlow.begin(), successFlow.end()), 2);
}
