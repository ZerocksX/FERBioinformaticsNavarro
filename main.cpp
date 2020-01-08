#include <iostream>
#include <algorithm>
#include <memory>
#include "GfaGraph.h"

std::unique_ptr<GfaGraph> graph;

int main() {
    std::string inputDir = R"(C:\Users\pavao\Documents\faks\bioinf\lab\navarro\input\)";
    std::cout << inputDir << std::endl;
    graph = std::unique_ptr<GfaGraph>(GfaGraph::loadFromFile(
//            inputDir + "ref10000_onechar.gfa"
//            inputDir + "ref10000_tangle.gfa"
//            inputDir + "ref10000_snp.gfa"
//            inputDir + "ref10000_twopath.gfa"
            inputDir + "example.gfa"
    ));
    std::cout << 1 << std::endl;
    int n = graph->vertices.size();
//    for (int i = 1; i <= n; ++i) {
//        Node *node = graph->vertices[i].get();
//        std::cout << node->id << "(" << node->sequence << "): ";
//        for (auto &it : node->outEdges) {
//            Node *child = it->to.get();
//            std::cout << child->id << "(" << child->sequence << "), ";
//        }
//        std::cout << std::endl;
//    }
    std::vector<std::shared_ptr<NodePosition>> nextPos;
    for (auto &it: graph->vertices) {
        std::shared_ptr<Node> node = it.second;
        for (auto &e: node->inEdges) {
            std::shared_ptr<NodePosition> nextNode = std::make_shared<NodePosition>(node, e);
            nextNode->reverse = true;
            nextPos.push_back(nextNode);
        }
    }
    for (auto &nodePos: nextPos) {
        std::cout << *nodePos << std::endl << std::endl;
        for (auto &next: nodePos->next()) {
            std::cout << *next << std::endl;
        }
        std::cout << std::endl;
    }
    return 0;
}
