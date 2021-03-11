#include "logger/logger.cc"
#include <gtest/gtest.h>

TEST(can_output_test, result_true) {
    ASSERT_TRUE(can_output(LogLevel::INFO, LogLevel::ERROR));
    ASSERT_TRUE(can_output(LogLevel::INFO, LogLevel::DEBUG));
    ASSERT_TRUE(can_output(LogLevel::INFO, LogLevel::INFO));
    ASSERT_TRUE(can_output(LogLevel::DEBUG, LogLevel::DEBUG));
    ASSERT_TRUE(can_output(LogLevel::DEBUG, LogLevel::ERROR));
    ASSERT_TRUE(can_output(LogLevel::WARN, LogLevel::ERROR));
    ASSERT_TRUE(can_output(LogLevel::ERROR, LogLevel::ERROR));
}

TEST(can_output_test, result_false) {
    ASSERT_FALSE(can_output(LogLevel::DEBUG, LogLevel::INFO));
    ASSERT_FALSE(can_output(LogLevel::WARN, LogLevel::DEBUG));
    ASSERT_FALSE(can_output(LogLevel::ERROR, LogLevel::WARN));
    ASSERT_FALSE(can_output(LogLevel::ERROR, LogLevel::INFO));
}
