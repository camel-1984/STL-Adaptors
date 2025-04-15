#include <processing.h>
#include <join.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

struct Student {
    uint64_t group_id;
    std::string name;
    bool operator==(const Student& other) const = default;
};

struct Group {
    uint64_t id;
    std::string name;
    bool operator==(const Group& other) const = default;
};

TEST(SimpleTest, JoinKV) {
    std::vector<KV<int, std::string>> left = {{0, "a"}, {1, "b"}, {2, "c"}, {3, "d"}, {1, "e"}};
    std::vector<KV<int, std::string>> right = {{0, "f"}, {1, "g"}, {3, "i"}};
    auto left_flow = AsDataFlow(left);
    auto right_flow = AsDataFlow(right);
    auto result = left_flow | Join(right_flow) | AsVector();
    ASSERT_THAT(
        result,
        testing::ElementsAre(
            JoinResult<std::string, std::string>{"a", "f"},
            JoinResult<std::string, std::string>{"b", "g"},
            JoinResult<std::string, std::string>{"c", std::nullopt},
            JoinResult<std::string, std::string>{"d", "i"},
            JoinResult<std::string, std::string>{"e", "g"}
        )
    );
}
