#include <gtest/gtest.h>
int add(int a, int b) {
    return a + b;
}

TEST(zero_add, add) {
    ASSERT_EQ(0, add(-1, 1));
}

TEST(positive_add, add) {
    ASSERT_EQ(5, add(3, 2));
}

TEST(negative_add, add) {
    EXPECT_EQ(-3, add(7, -10));
}