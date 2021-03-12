#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "algo/navigation.h"

TEST(test_suite_name, test_name)
{
    freopen("../../test/algo/algo_test.in", "r", stdin);
    int n, m, s, e, mid_point1, mid_point2;
    double range;

    std::cin >> n >> m;
    navigation nav(n);
    for (int i = 0; i < m; ++i)
    {
        int u, v;
        bool f;
        double w, r;
        std::cin >> u >> v >> w >> f >> r;
        nav.add_edge(u, v, w, f, r);
        nav.add_edge(v, u, w, f, r);
    }

    std::cin >> s >> e;
    ASSERT_FLOAT_EQ(6.5, nav.navigate(s, e));
    ASSERT_EQ("\"0-->4(2.500000)-->5(2.500000)-->6(1.500000)\"", nav.get_route());
    ASSERT_FLOAT_EQ(6.5, nav.navigate(s, e, 1));
    ASSERT_EQ("\"0-->4(2.500000)-->5(2.500000)-->6(1.500000)\"", nav.get_route());
    std::cin >> s >> e;
    ASSERT_FLOAT_EQ(30, nav.navigate(s, e, 2));
    ASSERT_EQ("\"7-->1(21.000000)-->8(3.000000)-->5(6.000000)\"", nav.get_route());

    std::cin >> s >> e;
    ASSERT_FLOAT_EQ(15.5, nav.navigate(s, e, 3));
    ASSERT_EQ("\"7-->1(7.000000)-->0(3.500000)-->4(2.500000)-->5(2.500000)\"", nav.get_route());

    std::cin >> s >> e;
    ASSERT_FLOAT_EQ(-1, nav.navigate(s, e, 3));

    std::cin >> s >> e >> mid_point1 >> mid_point2;
    ASSERT_FLOAT_EQ(13.5, nav.navigate_multi_joint(s, mid_point1, mid_point2, e));
    ASSERT_EQ("\"7-->1(7.000000)\"\"1-->2(1.000000)\"\"2-->6(4.000000)-->5(1.500000)\"", nav.get_route())
                                << nav.get_route();

    std::cin >> s >> e >> mid_point1 >> mid_point2;
    ASSERT_FLOAT_EQ(10, nav.navigate_multi_joint(s, mid_point1, mid_point2, e));
    ASSERT_EQ("\"6-->2(4.000000)-->1(1.000000)\"\"1-->2(1.000000)\"\"2-->1(1.000000)-->4(3.000000)\"", nav.get_route());
    std::cin >> s >> e >> mid_point1 >> mid_point2;
    ASSERT_FLOAT_EQ(8, nav.navigate_multi_joint(s, mid_point1, mid_point2, e));
    ASSERT_EQ("\"6-->2(4.000000)\"\"2-->1(1.000000)\"\"1-->4(3.000000)\"", nav.get_route());

    std::cin >> s >> range;
    std::vector<int> res = nav.search(s, range);
    std::string tmp;
    for (auto p : res)
    {
        tmp += std::to_string(p) + " ";
    }
    tmp.pop_back();
    ASSERT_EQ("0 1 2 3 4", tmp);
}
