//
// Created by pavao on 12.1.2020..
//
#include <iostream>
#include "navarroNode.h"

int score(FastQ fastQ, std::unordered_set<NodePosition> &nodes, GfaGraph *gfaGraph) {
    std::unordered_map<NodePosition, int> cv;
    std::unordered_map<NodePosition, int> cvTemp;

    for (auto &node : nodes) {
        cv[node] = 0;
    }

    int m = fastQ.sequence.size();

    for (int i = 1; i <= m; i++) {
        char c = fastQ.sequence[i - 1];
        for (auto &node : nodes) {
            cvTemp[node] = g(node, i, c, gfaGraph, cv);
        }
        for (auto &node : nodes) {
            cv[node] = cvTemp[node];
        }
    }
    int min = 1 << 30u;
    for (auto &nodeScore : cv) {
        std::cout << nodeScore.first << " ->" << nodeScore.second << std::endl;
        min = std::min(nodeScore.second, min);
    }
    return min;
}

int g(NodePosition node, int i, char c, GfaGraph *gfaGraph, std::unordered_map<NodePosition, int> &cv) {
    if (c == node.getCurrentChar()) {
        int min = i - 1;
        for (auto &parent : node.previous(gfaGraph)) {
            min = std::min(cv[parent], min);
        }
        return min;
    } else {
        auto parents = node.previous(gfaGraph);
        if (parents.empty()) {
            return 1;
        }
        int min = 1u << 30u;
        for (auto &parent : node.previous(gfaGraph)) {
            min = std::min(cv[parent], min);
        }
        return 1 + min;
    }
}