#include <gtest/gtest.h>

// basic assertions
TEST(HelloTest, BasicAssertions) {
    // expect two strings to not be equal
    EXPECT_STRNE("hello", "world");
    // expect equality
    EXPECT_EQ(7 * 6, 42);
}

TEST(HelloTest, BasicFail) {
    // expect two strings to not be equal
    EXPECT_STREQ("hello", "world");
    // expect equality
    EXPECT_EQ(7 * 6, 42);
}