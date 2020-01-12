//
// Created by pavao on 12.1.2020..
//
#include <iostream>
#include "navarroNode.h"

int score(FastQ fastQ, NodePositionGraph &graph) {
    std::unordered_map<int, int> cv;

    for (int i = 0; i < graph.n; ++i) {
        cv[i] = 0;
    }

    int m = fastQ.sequence.size();
//    std::cout << cv.size() << std::endl;
    for (int i = 1; i <= m; i++) {
        char c = fastQ.sequence[i - 1];
        if (i % 100 == 0) {
            std::cout << "Round: " << i << " of " << m << std::endl;
        }
        std::unordered_map<int, int> cvTemp;
        for (int node = 0; node < graph.n; ++node) {
            cvTemp[node] = g(node, i, c, graph, cv);
        }
        cv = cvTemp;
        for (int node = 0; node < graph.n; ++node) {
            for (int child : graph.nextNodes[node]) {
                propagate(node, child, graph, cv);
            }
        }
    }
    int min = 1 << 30u;
    for (int k = 0; k < graph.n; ++k) {
//        std::cout << graph.nodes[k] << " -> " << cv[k] << std::endl;
        min = std::min(cv[k], min);
    }
    return min;
}

int g(int node, int i, char c, NodePositionGraph &graph, std::unordered_map<int, int> &cv) {
    if (c == graph.nodes[node].getCurrentChar()) {
        int min = i - 1;
        for (int parent : graph.previousNodes[node]) {
            min = std::min(cv[parent], min);
        }
        return min;
    } else {
        if (graph.previousNodes[node].empty()) {
            return 1 + cv[node];
        }
        int min = 1u << 30u;
        for (int parent : graph.previousNodes[node]) {
            min = std::min(cv[parent], min);
        }
        return 1 + std::min(min, cv[node]);
    }
}

void propagate(int u, int v, NodePositionGraph &graph, std::unordered_map<int, int> &cv) {
    if (cv[v] > 1 + cv[u]) {
        cv[v] = 1 + cv[u];
        for (int child : graph.nextNodes[v]) {
            propagate(v, child, graph, cv);
        }
    }
}