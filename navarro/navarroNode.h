//
// Created by pavao on 12.1.2020..
//

#ifndef NAVARRO_NAVARRONODE_H
#define NAVARRO_NAVARRONODE_H

#include <fastqloader.h>
#include <unordered_map>
#include <unordered_set>
#include "../GfaGraph.h"

int g(const NodePosition& node, int i, char c, GfaGraph *gfaGraph, std::unordered_map<NodePosition, int> &cv);
int score(FastQ fastQ, std::unordered_set<NodePosition> &nodes, GfaGraph *gfaGraph);
void propagate(const NodePosition& u, const NodePosition& v, GfaGraph *graph, std::unordered_map<NodePosition, int> &cv);

#endif //NAVARRO_NAVARRONODE_H
