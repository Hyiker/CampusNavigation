#include <cassert>
#include <iostream>
#include <vector>
#include "algo/navigation.h"

int main() {
    int n, m, k, s, e, mid_point1, mid_point2;
    double range;

    // 输入图
    // n：点数，m：边数（这里咱认为一条路包括两个方向，两个方向长度相同、自行车同样地可通行或不可通行、拥挤度一致
    // u：每条路的起点，v：该路的终点，w：路的长度，f：自行车可通行为 1、不可通行为 0，r：拥挤度，应为不小于 1
    // 的double型数
    std::cin >> n >> m;
    navigation nav(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        bool f;
        double w, r;
        std::cin >> u >> v >> w >> f >> r;
        nav.add_edge(u, v, w, f, r);
        nav.add_edge(v, u, w, f, r);
    }

    // 导航 - strategy 1：单纯的最短路
    // s：起点，e：终点
    // 路径中括号内的数，表示该段路的长度（用于模拟导航）
    std::cout << "\nstrategy 1\n";
    std::cin >> s >> e;
    std::cout << s << "-->" << e << ": "
              << "distance = " << nav.navigate(s, e) << ", route = " << nav.get_route() << std::endl;
    // 同样可行但不推荐
    std::cout << s << "-->" << e << ": "
              << "distance = " << nav.navigate(s, e, 1) << ", route = " << nav.get_route() << std::endl;

    // 导航 - strategy 2：考虑到道路拥挤度
    std::cout << "\nstrategy 2\n";
    std::cin >> s >> e;
    std::cout << s << "-->" << e << ": "
              << "distance = " << nav.navigate(s, e, 2) << ", route = " << nav.get_route() << std::endl;

    // 导航 - strategy 3：自行车通行
    std::cout << "\nstrategy 3\n";
    std::cin >> s >> e;
    std::cout << s << "-->" << e << ": "
              << "distance = " << nav.navigate(s, e, 3) << ", route = " << nav.get_route() << std::endl;
    // 这是自行车无法到达的一种情况，实际使用时，距离为 -1 则表示无法到达
    s = 6, e = 4;
    std::cout << s << "-->" << e << ": "
              << "distance = " << nav.navigate(s, e, 3) << ", route = " << nav.get_route() << std::endl;

    // 导航 - 要求途径某些中间节点
    std::cout << "\nmid-point strategy\n";
    std::cin >> s >> e >> mid_point1 >> mid_point2;
    std::cout << s << "-->" << mid_point1 << "-->" << mid_point2 << "-->" << e << ": "
              << "distance = " << nav.navigate_multi_joint(s, mid_point1, mid_point2, e)
              << ", route = " << nav.get_route() << std::endl;
    // 这是产生FIXME中提到的问题的一个样例
    std::cout << "\nFIXME case\n";
    s = 6, e = 4, mid_point1 = 1, mid_point2 = 2;
    std::cout << s << "-->" << mid_point1 << "-->" << mid_point2 << "-->" << e << ": "
              << "distance = " << nav.navigate_multi_joint(s, mid_point1, mid_point2, e)
              << ", route = " << nav.get_route() << std::endl;
    s = 6, e = 4, mid_point1 = 2, mid_point2 = 1;
    std::cout << s << "-->" << mid_point1 << "-->" << mid_point2 << "-->" << e << ": "
              << "distance = " << nav.navigate_multi_joint(s, mid_point1, mid_point2, e)
              << ", route = " << nav.get_route() << std::endl;

    // 在一定范围内查找所有符合要求的建筑
    // s：中心点，range：距离范围（判断时使用 < 判断，故不包括边缘的节点）
    std::cout << "\nsearch\n";
    std::cin >> s >> range;
    std::vector<int> res = nav.search(s, range);
    std::cout << "There is/are " << res.size() << " point(s) within " << range << " away from " << s << ": ";
    for (auto p : res) {
        std::cout << p << " ";
    }
    std::cout << std::endl;
    return 0;
}
/*
9 14
0 1 3.5 1 1.5
0 3 4.5 1 2.5
0 4 2.5 1 3.2
1 2 1 0 1
1 3 6.5 1 1.5
1 4 3 0 2
1 7 7 1 3
1 8 2 0 1.5
2 3 5 0 2
2 6 4 0 3
3 5 2 1 1
4 5 2.5 1 1.2
5 6 1.5 0 2
5 8 3 1 2
0 6
7 5
7 5
7 5 1 2
0 4.55
 */