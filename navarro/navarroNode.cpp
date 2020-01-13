//
// Created by pavao on 12.1.2020..
//
#include <iostream>
#include "navarroNode.h"

std::vector<int> score(FastQ fastQ, NodePositionGraph &graph, std::vector<std::vector<Matching>> &backtrack) {

    std::vector<int> cv(graph.n);

    for (int i = 0; i < graph.n; ++i) {
        cv[i] = 0;
    }

    int m = fastQ.sequence.size();

//    std::cout << cv.size() << std::endl;
    for (int i = 1; i <= m; i++) {
        char c = fastQ.sequence[i - 1];
//        if (i % 100 == 0) {
//            std::cout << "Round: " << i << " of " << m << std::endl;
//        }
        std::vector<int> cvTemp(graph.n);
        for (int node = 0; node < graph.n; ++node) {
            cvTemp[node] = g(node, i, c, graph, cv, backtrack);
        }
        cv = cvTemp;
        for (int node = 0; node < graph.n; ++node) {
            for (int child : graph.nextNodes[node]) {
                propagate(node, child, graph, cv, backtrack, i);
            }
        }
    }
    return cv;
}

int g(int node, int i, char c, NodePositionGraph &graph, std::vector<int> &cv,
      std::vector<std::vector<Matching>> &backtrack) {
    if (c == graph.nodes[node].getCurrentChar()) {
        int min = i - 1;
        backtrack[i][node] = Matching(-1, -1, 0);//beginning
        for (int parent : graph.previousNodes[node]) {
            if (cv[parent] < min) {
                min = cv[parent];
                backtrack[i][node] = Matching(i - 1, parent, 0);//match
            }
        }
        return min;
    } else {
        if (graph.previousNodes[node].empty()) {
            backtrack[i][node] = Matching(i - 1, node, 2); //mismatch
            return 1 + cv[node];
        }
        int min = 1u << 30u;
        for (int parent : graph.previousNodes[node]) {
            if (cv[parent] < min) {
                min = cv[parent];
                backtrack[i][node] = Matching(i - 1, parent, 2);//mismatch
            }
        }
        if (cv[node] < min) {
            backtrack[i][node] = Matching(i - 1, node, 2); //mismatch
            return 1 + cv[node];
        } else {
            return 1 + min;
        }
    }
}

void
propagate(int u, int v, NodePositionGraph &graph, std::vector<int> &cv, std::vector<std::vector<Matching>> &backtrack,
          int i) {
    if (cv[v] > 1 + cv[u]) {
        cv[v] = 1 + cv[u];
        backtrack[i][v] = Matching(i, u, 1); // indel;
        for (int child : graph.nextNodes[v]) {
            propagate(v, child, graph, cv, backtrack, i);
        }
    }
}

Matching::Matching(int parentRow, int parentColumn, int op) : parentRow(parentRow), parentColumn(parentColumn),
                                                              op(op) {}

Matching::Matching() : Matching(-1, -1, -1) {}
