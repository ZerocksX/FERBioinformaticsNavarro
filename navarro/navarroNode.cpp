//
// Created by pavao on 12.1.2020..
//
#include <iostream>
#include "navarroNode.h"

int score(FastQ fastQ, std::unordered_set<NodePosition> &nodes, GfaGraph *gfaGraph) {
    std::unordered_map<NodePosition, int> cv;

    for (auto &node : nodes) {
        cv[node] = 0;
    }

    int m = fastQ.sequence.size();
    std::cout << cv.size() << std::endl;
    for (int i = 1; i <= m; i++) {
        char c = fastQ.sequence[i - 1];
        if (i % 100 == 0) {
            std::cout << "Round: " << i << " of " << m << std::endl;
        }
        std::unordered_map<NodePosition, int> cvTemp;
        for (auto &node : nodes) {
            cvTemp[node] = g(node, i, c, gfaGraph, cv);
        }
//        cv = cvTemp;
        for (auto &node : nodes) {
            cv[node] = cvTemp[node];
        }
        for (auto &node : nodes) {
            for (auto &child : node.next(gfaGraph)) {
                propagate(node, child, gfaGraph, cv);
            }
        }
    }
    int min = 1 << 30u;
    for (auto &nodeScore : cv) {
        std::cout << nodeScore.first << " -> " << nodeScore.second << std::endl;
        min = std::min(nodeScore.second, min);
    }
    return min;
}

int g(const NodePosition &node, int i, char c, GfaGraph *gfaGraph, std::unordered_map<NodePosition, int> &cv) {
    if (c == node.getCurrentChar()) {
        int min = i - 1;
        for (auto &parent : node.previous(gfaGraph)) {
            min = std::min(cv[parent], min);
        }
        return min;
    } else {
        auto parents = node.previous(gfaGraph);
        if (parents.empty()) {
            return 1 + cv[node];
        }
        int min = 1u << 30u;
        for (auto &parent : node.previous(gfaGraph)) {
            min = std::min(cv[parent], min);
        }
        return 1 + std::min(min, cv[node]);
    }
}

/*
 * Propagate (u,v)
if Cv>1 + Cu
Cv ← 1 + Cu
for all z=(v; z)∈E
Propagate (v,z)
 * */

void
propagate(const NodePosition &u, const NodePosition &v, GfaGraph *graph, std::unordered_map<NodePosition, int> &cv) {
    if (cv[v] >  1 + cv[u]) {
        cv[v] = 1 + cv[u];
        for (const auto &z : v.next(graph)) {
            propagate(v, z, graph, cv);
        }
    }
}