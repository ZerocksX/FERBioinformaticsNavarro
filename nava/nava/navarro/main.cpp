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
    int n = graph->vertices.size();

    //samo obicni nodeovi
    for (int i = 1; i <= n; ++i) {
        Node node = graph->vertices[i];
        std::cout << node.id << "(" << node.sequence << "): ";
        for (auto &it : node.outEdges) {
            Node child = graph->vertices[it.to];
            std::cout << child.id << "(" << child.sequence << "), ";
        }
        std::cout << std::endl;
    }

    // posebni nodeovi koji idu char po char
    std::unordered_set<NodePosition> front;
    for (auto &it: graph->vertices) {
        Node node = it.second;
        for (auto &e: node.inEdges) {
            front.insert(NodePosition(node, e, true));
        }
    }
    std::unordered_set<NodePosition> allPos;
    allPos.insert(front.begin(), front.end());
    bool addedNew;
    do {
        addedNew = false;
        std::unordered_set<NodePosition> frontNew;
        for (auto np : front) {
            for (auto &newNP : np.previous(graph.get())) {
                bool isIn = allPos.find(newNP) != allPos.end();
                if(!isIn){
                    addedNew = true;
                    frontNew.insert(newNP);
                }
            }
        }
        std::cout << "size: " << frontNew.size() << std::endl;
        for(auto p : frontNew){
            std::cout << p << std::endl;
        }
        front.clear();
        front.insert(frontNew.begin(), frontNew.end());
        allPos.insert(front.begin(), front.end());
    } while (addedNew);
    //allPos su svi node-ovi onda se na njima radi previous i sto vec treba
    //preporucam za onaj Cv radit novu unordered_map<NodePosition, int> pa imat Cv i Cv' mape za racunanje 'retka' i novog 'retka'
    std::cout << "All node(char): " << std::endl;
    for (auto &nodePos: allPos) {
        std::cout << nodePos << std::endl;
    }
    return 0;
}