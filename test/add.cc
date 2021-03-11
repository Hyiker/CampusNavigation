#include <gtest/gtest.h>
int add(int a, int b) {
    return a + b;
}

TEST(add_test, zero) {
    ASSERT_EQ(0, add(-1, 1));
}

TEST(add_test, pos) {
    ASSERT_EQ(5, add(3, 2));
}

TEST(add_test, neg) {
    EXPECT_EQ(-3, add(7, -10));
}