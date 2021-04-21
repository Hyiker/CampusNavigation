#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <vector>
#include "algo/navigation.h"
inline int read() {
    int ans = 0;
    bool flag = 1;
    char c = getchar();
    while (c > '9' || c < '0') {
        if (c == '-') {
            flag = -1;
        }
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        ans = (ans << 1) + (ans << 3) + (c ^ 48);
        c = getchar();
    }
    return flag ? ans : ~ans + 1;
}
#define read read()
/*

TEST(NavigationFunctionalTest, BetweenBuildingNavigation) {
    std::ifstream ifs("test/test_data/algo/algo_functional_test.in");
    int n, m, s, e, mid_point1, mid_point2;
    double range;

    ifs >> n >> m;
    ASSERT_EQ(9, n) << n;
    navigation nav(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        bool f;
        double w, r;
        ifs >> u >> v >> w >> f >> r;
        nav.add_edge(u, v, w, f, r);
        nav.add_edge(v, u, w, f, r);
    }

    ifs >> s >> e;
    ASSERT_FLOAT_EQ(6.5, nav.navigate(s, e));
    ASSERT_EQ("\"0-->4(2.500000)-->5(2.500000)-->6(1.500000)\"", nav.get_route());
    ASSERT_FLOAT_EQ(6.5, nav.navigate(s, e, 1));
    ASSERT_EQ("\"0-->4(2.500000)-->5(2.500000)-->6(1.500000)\"", nav.get_route());
    ifs >> s >> e;
    ASSERT_FLOAT_EQ(30, nav.navigate(s, e, 2));
    ASSERT_EQ("\"7-->1(21.000000)-->8(3.000000)-->5(6.000000)\"", nav.get_route());

    ifs >> s >> e;
    ASSERT_FLOAT_EQ(15.5, nav.navigate(s, e, 3));
    ASSERT_EQ("\"7-->1(7.000000)-->0(3.500000)-->4(2.500000)-->5(2.500000)\"", nav.get_route());

    ifs >> s >> e;
    ASSERT_FLOAT_EQ(-1, nav.navigate(s, e, 3));

    ifs >> s >> e >> mid_point1 >> mid_point2;
    ASSERT_FLOAT_EQ(13.5, nav.navigate_multi_joint(s, mid_point1, mid_point2, e));
    ASSERT_EQ("\"7-->1(7.000000)\"\"1-->2(1.000000)\"\"2-->6(4.000000)-->5(1.500000)\"", nav.get_route())
        << nav.get_route();

    ifs >> s >> e >> mid_point1 >> mid_point2;
    ASSERT_FLOAT_EQ(10, nav.navigate_multi_joint(s, mid_point1, mid_point2, e));
    ASSERT_EQ("\"6-->2(4.000000)-->1(1.000000)\"\"1-->2(1.000000)\"\"2-->1(1.000000)-->4(3.000000)\"", nav.get_route());
    ifs >> s >> e >> mid_point1 >> mid_point2;
    ASSERT_FLOAT_EQ(8, nav.navigate_multi_joint(s, mid_point1, mid_point2, e));
    ASSERT_EQ("\"6-->2(4.000000)\"\"2-->1(1.000000)\"\"1-->4(3.000000)\"", nav.get_route());

    ifs >> s >> range;
    std::vector<int> res = nav.search(s, range);
    std::string tmp;
    for (auto p : res) {
        tmp += std::to_string(p) + " ";
    }
    tmp.pop_back();
    ASSERT_EQ("0 1 2 3 4", tmp);
    ifs.close();
}

class LargeDataParamTest : public ::testing::TestWithParam<std::string> {};

INSTANTIATE_TEST_SUITE_P(Test, LargeDataParamTest, testing::Values("1", "2", "3", "4", "5"));

TEST_P(LargeDataParamTest, DijkstraTest) {
    std::string str = GetParam();
    const std::string prefix_in = "test/test_data/algo/algo_in_", prefix_out = "test/test_data/algo/algo_out_",
                      suffix = ".txt";

    freopen((prefix_in + str + suffix).c_str(), "r", stdin);
    int n, m, s;
    n = read, m = read, s = read;
    navigation nav(n, m);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        u = read, v = read, w = read;
        nav.add_edge(u - 1, v - 1, w);
    }

    freopen((prefix_out + str + suffix).c_str(), "r", stdin);
    std::vector<int> expect_res(n);
    int x;
    for (int i = 0; i < n; ++i) {
        x = read;
        expect_res[i] = x;
    }

    for (int i = 0; i < n; ++i) {
        if (i != s - 1) {
            ASSERT_FLOAT_EQ(expect_res[i], nav.navigate(s - 1, i)) << s - 1 << " " << i;
        }
    }
}
*/
