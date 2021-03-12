#include "navigation.h"
#include <algorithm>
#include <cfloat>
#include <iostream>
#include <queue>
#include <stack>

navigation::navigation() : navigation(20, 200) {
}

navigation::navigation(int node_size, int edge_size)
    : node_size(node_size), edge_size(edge_size << 1), tot(0), under_navigating{false} {
    head.resize(node_size);
    edge.resize(edge_size << 1);
    vis.resize(node_size);
    dis.resize(node_size);
    dis.assign(node_size, DBL_MAX);
    route.resize(node_size);
    route.assign(node_size, {-1, 0});
}

void navigation::add_edge(int start_node, int end_node, double weight, bool bicycle_passable, double congestion_rate) {
    edge[++tot].to = end_node;
    edge[tot].next = head[start_node];
    edge[tot].distance = weight;
    edge[tot].bicycle_passable = bicycle_passable;
    edge[tot].congestion_rate = congestion_rate;
    head[start_node] = tot;
}

std::string navigation::get_route() {
    return route_cache;
}

void navigation::dijkstra(int start_node) {
    for (int i = 0; i < node_size; ++i) {
        vis[i] = false;
        dis[i] = DBL_MAX;
        route[i] = {-1, 0};
    }
    dis[start_node] = 0;
    std::priority_queue<Node> q;
    q.push({start_node, 0});
    while (!q.empty()) {
        Node u = q.top();
        q.pop();
        if (vis[u.id]) {
            continue;
        }
        vis[u.id] = true;
        for (int i = head[u.id]; i; i = edge[i].next) {
            int v = edge[i].to;
            if (u.dis + edge[i].distance < dis[v]) {
                dis[v] = u.dis + edge[i].distance;
                route[v] = {u.id, edge[i].distance};
                if (!vis[v]) {
                    q.push({v, dis[v]});
                }
            }
        }
    }
}

template <typename Coefficient>
void navigation::dijkstra(int start_node, Coefficient Coe) {
    for (int i = 0; i < node_size; ++i) {
        vis[i] = false;
        dis[i] = DBL_MAX;
        route[i] = {-1, 0};
    }
    dis[start_node] = 0;
    std::priority_queue<Node> q;
    q.push({start_node, 0});
    while (!q.empty()) {
        Node u = q.top();
        q.pop();
        if (vis[u.id]) {
            continue;
        }
        vis[u.id] = true;
        for (int i = head[u.id]; i; i = edge[i].next) {
            int v = edge[i].to;
            if (u.dis + edge[i].distance * Coe(i) < dis[v]) {
                dis[v] = u.dis + edge[i].distance * Coe(i);
                route[v] = {u.id, edge[i].distance * Coe(i)};
                if (!vis[v]) {
                    q.push({v, dis[v]});
                }
            }
        }
    }
}

std::string navigation::route_output(int start_node, int end_node) {
    std::stack<int> sta;
    std::string res = "\"";
    int node = end_node;
    while (route[node].to != -1) {
        sta.push(node);
        node = route[node].to;
    }
    res += std::to_string(start_node);
    while (!sta.empty()) {
        res += "-->" + std::to_string(sta.top()) + "(" + std::to_string(route[sta.top()].distance) + ")";
        sta.pop();
    }
    return res + "\"";
}

double navigation::navigate(int start_node, int end_node) {
    assert(start_node != end_node);
    assert(start_node < node_size && end_node < node_size);
    if (std::abs(dis[start_node]) > DBL_EPS) {
        dijkstra(start_node);
    }
    if (!under_navigating) {
        route_cache = "";
    }
    route_cache += route_output(start_node, end_node);
    return dis[end_node];
}

double navigation::navigate(int start_node, int end_node, int strategy) {
    assert(start_node != end_node);
    assert(start_node < node_size && end_node < node_size);
    switch (strategy) {
        case 2:
            dijkstra(start_node, [&](int i) -> double { return edge[i].congestion_rate; });
            break;
        case 3:
            // need to be change when the distance can similar to 1e9
            dijkstra(start_node, [&](int i) -> double { return edge[i].bicycle_passable ? 1 : 1e9; });
            if (dis[end_node] > 1e9) {
                std::cout << "No through road between your starting point and ending point!" << std::endl;
                return -1;
            }
            break;
        default:
            return navigate(start_node, end_node);
    }
    if (!under_navigating) {
        route_cache = "";
    }
    route_cache += route_output(start_node, end_node);
    dis[start_node] = DBL_MAX;  // do not cache the distance data, for it doesn't reflect the real distance
    return dis[end_node];
}

std::vector<int> navigation::search(int start_node, double distance_range) {
    assert(start_node < node_size);
    if (std::abs(dis[start_node]) > DBL_EPS) {
        dijkstra(start_node);
    }
    std::vector<int> res;
    for (int i = 0; i < node_size; ++i) {
        if (dis[i] < distance_range) {
            res.emplace_back(i);
        }
    }
    res.shrink_to_fit();
    return res;
}