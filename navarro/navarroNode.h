//
// Created by pavao on 12.1.2020..
//

#ifndef NAVARRO_NAVARRONODE_H
#define NAVARRO_NAVARRONODE_H

#include <fastqloader.h>
#include <unordered_map>
#include <unordered_set>
#include "../GfaGraph.h"

int g(int node, int i, char c, NodePositionGraph& graph, std::vector<int> &cv);

int score(FastQ fastQ, NodePositionGraph &graph);

void
propagate(int u, int v, NodePositionGraph &graph, std::vector<int> &cv);

#endif //NAVARRO_NAVARRONODE_H
