#include <filesystem>
#include "utils.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(DirTest, DirRecursive) {
    const char* testPath = "/Users/rostislavmolcanov/Documents"
                           "/itmo/op/sem2/lab02/lab8/labwork8-camel-1984/DIR";
    std::vector<std::filesystem::path> results;
    bool recursive;
    for (auto& entry : Dir(testPath, recursive)) {
        results.push_back(entry);
    }
    ASSERT_THAT(results,testing::ElementsAre(
                std::filesystem::path("/Users/rostislavmolcanov/Documents/itmo/op/sem2/lab02/lab8/labwork8-camel-1984/DIR/subDIR"),
                std::filesystem::path("/Users/rostislavmolcanov/Documents/itmo/op/sem2/lab02/lab8/labwork8-camel-1984/DIR/subDIR/3.txt"),
                std::filesystem::path("/Users/rostislavmolcanov/Documents/itmo/op/sem2/lab02/lab8/labwork8-camel-1984/DIR/2.txt"),
                std::filesystem::path("/Users/rostislavmolcanov/Documents/itmo/op/sem2/lab02/lab8/labwork8-camel-1984/DIR/1.txt")
                )
    );
}

TEST(DirTest, DirBasic) {
    const char* testPath = "/Users/rostislavmolcanov/Documents"
                           "/itmo/op/sem2/lab02/lab8/labwork8-camel-1984/DIR";
    std::vector<std::filesystem::path> results;
    for (auto& entry : Dir(testPath)) {
        results.push_back(entry);
    }
    ASSERT_THAT(results,testing::ElementsAre(
                std::filesystem::path("/Users/rostislavmolcanov/Documents/itmo/op/sem2/lab02/lab8/labwork8-camel-1984/DIR/subDIR"),
                std::filesystem::path("/Users/rostislavmolcanov/Documents/itmo/op/sem2/lab02/lab8/labwork8-camel-1984/DIR/2.txt"),
                std::filesystem::path("/Users/rostislavmolcanov/Documents/itmo/op/sem2/lab02/lab8/labwork8-camel-1984/DIR/1.txt")
                )
    );
}
