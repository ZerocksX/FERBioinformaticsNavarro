//
// Created by pavao on 13.1.2020..
//

#ifndef NAVARRO_MAIN_H
#define NAVARRO_MAIN_H

#include <unordered_set>
#include "GfaGraph.h"
#include "navarro/navarroNode.h"

std::unordered_set<NodePosition> getAllNodes(GfaGraph *pGraph);

void printGraph(std::unordered_set<NodePosition> set, GfaGraph *pGraph);

void printNPGraph(NodePositionGraph graph);

#endif //NAVARRO_MAIN_H
