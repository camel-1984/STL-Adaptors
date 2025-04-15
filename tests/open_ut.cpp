#include <filesystem>
#include "utils.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(OpenTest, OpenBasic) {
    const char* testPath = "/Users/rostislavmolcanov/Documents"
                           "/itmo/op/sem2/lab02/lab8/labwork8-camel-1984/DIR";
    auto results = Dir(testPath) | OpenFiles();
    ASSERT_THAT(results,testing::ElementsAre(
                std::filesystem::path(""),
                std::filesystem::path("ONE\nTWO\nTHREE\nFOUR"),
                std::filesystem::path("ONE\nTWO\nTHREE")
                )
    );
}

TEST(OpenTest, OpenRecursive) {
    const char* testPath = "/Users/rostislavmolcanov/Documents"
                              "/itmo/op/sem2/lab02/lab8/labwork8-camel-1984/DIR";
    bool recursive;
    auto results = Dir(testPath, recursive) | OpenFiles();
    ASSERT_THAT(results,testing::ElementsAre(
                std::filesystem::path(""),
                std::filesystem::path("FIVE\nFIVE\nFIVE\nFIVE\nSEVEN SEVEN SEVEN"),
                std::filesystem::path("ONE\nTWO\nTHREE\nFOUR"),
                std::filesystem::path("ONE\nTWO\nTHREE")
                )
    );
}
