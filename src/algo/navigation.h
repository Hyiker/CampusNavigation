#ifndef CAMPUSNAVIGATION_NAVIGATION_H
#define CAMPUSNAVIGATION_NAVIGATION_H

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#define DBL_EPS 1e-6

struct Node {
    int id;
    double dis;

    bool operator<(const Node x) const {
        return x.dis < dis;
    }
};

struct Edge {
    int to;
    int next;
    double distance;
    bool bicycle_passable;
    double congestion_rate;
};

struct Path {
    int to;
    double distance;
};

class navigation {
   private:
    std::vector<Edge> edge;
    std::vector<int> head;
    int tot;
    int node_size;
    int edge_size;
    std::vector<bool> vis;
    std::vector<double> dis;
    std::vector<Path> route;
    std::string route_cache;
    bool under_navigating;

    void dijkstra(int start_node);
    template <typename Coefficient>
    void dijkstra(int start_node, Coefficient Coe);
    std::string route_output(int start_node, int end_node);

   public:
    navigation();
    explicit navigation(int node_size = 20, int edge_size = 200);
    void add_edge(int start_node, int end_node, double weight, bool bicycle_passable = 1, double congestion_rate = 1);
    std::string get_route();
    double navigate(int start_node, int end_node);
    /**
     *
     * @param strategy = 1: shortest path between start_node to end_node
     * @param strategy = 2: congestion is considered
     * @param strategy = 3: riding bicycle
     */
    double navigate(int start_node, int end_node, int strategy);

    // TODO: variadic template can only be defined inside class?
    // FIXME: seems that it only work approximately when the joints are in order, says, first get to A, then to B,
    // FIXME: then to C, finally to D, otherwise the route might be   A-->C-->B   B-->C   C-->D  , which is unreasonable
    template <typename First, typename Second>
    double navigate_multi_joint(First first, Second second) {
        static_assert(std::is_same<First, int>::value && std::is_same<Second, int>::value, "Integer needed!");
        double tmp = navigate(first, second);
        under_navigating = false;
        return tmp;
    }
    template <typename First, typename Second, typename... Pack>
    double navigate_multi_joint(First first, Second second, Pack... params) {
        if (!under_navigating) {
            route_cache = "";
        }
        under_navigating = true;
        static_assert(std::is_same<First, int>::value && std::is_same<Second, int>::value, "Integer needed!");
        return navigate(first, second) + navigate_multi_joint(second, params...);
    }

    std::vector<int> search(int start_node, double distance_range);
};

#endif  // CAMPUSNAVIGATION_NAVIGATION_H
