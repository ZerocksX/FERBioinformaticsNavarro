//
// Created by pavao on 12.1.2020..
//

#ifndef NAVARRO_NAVARRONODE_H
#define NAVARRO_NAVARRONODE_H

#include "../GraphAligner/fastqloader.h"
#include <unordered_map>
#include <unordered_set>
#include "../GfaGraph.h"

class Matching {
public:
    int parentRow;
    int parentColumn;
    int op;

    Matching(int parentRow, int parentColumn, int op);

    Matching();

};

int g(int node, int i, char c, NodePositionGraph &graph, std::vector<int> &cv,
      std::vector<std::vector<Matching>> &backtrack);

std::vector<int> score(FastQ fastQ, NodePositionGraph &graph, std::vector<std::vector<Matching>> &backtrack);

void
propagate(int u, int v, NodePositionGraph &graph, std::vector<int> &cv, std::vector<std::vector<Matching>> &backtrack,
          int i);

#endif //NAVARRO_NAVARRONODE_H
