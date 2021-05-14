#include "toki/toki.hpp"
#include <gtest/gtest.h>
using namespace std;
TEST(system_time, format_test) {
    SystemTime st;
    st.set_timeval(60);
    ASSERT_EQ(st.to_string(), "00:01:00");
    st.set_timeval(3601);
    ASSERT_EQ(st.to_string(), "01:00:01");
    st.set_timeval(3659);
    ASSERT_EQ(st.to_string(), "01:00:59");
}