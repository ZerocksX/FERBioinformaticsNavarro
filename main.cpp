#include <iostream>
#include <algorithm>
#include <memory>
#include <set>
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
    std::unordered_set<NodePosition> front;
    for (auto &it: graph->vertices) {
        Node node = it.second;
        for (auto &e: node.outEdges) {
            front.insert(NodePosition(node, e, false));
        }
    }
    std::unordered_set<NodePosition> allPos;
    allPos.insert(front.begin(), front.end());
    bool addedNew;
    do {
        addedNew = false;
        std::unordered_set<NodePosition> frontNew;
        for (auto np : front) {
            for (auto &newNP : np.next()) {
                bool test = false;
                for (const auto& p : allPos) {
                    test = p == newNP;
                    if (test) {
                        break;
                    }
                }
                if(!test){
                    addedNew = true;
                    frontNew.insert(newNP);
                }

            }
        }
        for (auto &nodePos: frontNew) {
            std::cout << nodePos << std::endl;
        }
        std::cout << std::endl;
        front.clear();
        front.insert(frontNew.begin(), frontNew.end());
    } while (addedNew);
    for (auto &nodePos: allPos) {
        std::cout << nodePos << std::endl;
    }
    return 0;
}
