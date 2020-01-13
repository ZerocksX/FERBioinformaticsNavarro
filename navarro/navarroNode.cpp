//
// Created by pavao on 12.1.2020..
//
#include <iostream>
#include "navarroNode.h"

std::vector<int> score(FastQ fastQ, NodePositionGraph &graph, std::vector<std::vector<Matching>> &backtrack) {

    std::vector<int> cv(graph.n);
    std::vector<int> cvTemp(graph.n);

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
        for (int node = 0; node < graph.n; ++node) {
            cvTemp[node] = g(node, i, c, graph, cv, backtrack);
        }
        cv.swap(cvTemp);
        for (int node = 0; node < graph.n; ++node) {
            for (int child : graph.nextNodes[node]) {
                propagate(node, child, graph, cv, backtrack, i);
            }
        }
//        for (int k = 0; k < graph.n; ++k) {
//            std::cout << graph.nodes[k] << " " << cv[k] << std::endl;
//        }
    }
    return cv;
}

int g(int node, int i, char c, NodePositionGraph &graph, std::vector<int> &cv,
      std::vector<std::vector<Matching>> &backtrack) {
    if (c == graph.nodes[node].getCurrentChar()) {
        int min = i - 1;
        backtrack[i][node] = Matching(-1, -1, 0);//beginning
        int minI = -1;
        for (int parent : graph.previousNodes[node]) {
            if (cv[parent] < min) {
                min = cv[parent];
                minI = parent;
            }
        }
        if(minI != -1){
            backtrack[i][node] = Matching(i - 1, minI, 0);//match
        }
        return min;
    } else {
        if (graph.previousNodes[node].empty()) {
            backtrack[i][node] = Matching(i - 1, node, 2); //mismatch
            return 1 + cv[node];
        }
        int min = 1u << 30u;
        int minI = -1;
        for (int parent : graph.previousNodes[node]) {
            if (cv[parent] < min) {
                min = cv[parent];
                minI = parent;
            }
        }
        backtrack[i][node] = Matching(i - 1, minI, 2);//mismatch
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
