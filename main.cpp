#include <iostream>
#include <algorithm>
#include <memory>
#include <set>
#include <fastqloader.h>
#include "GfaGraph.h"
#include "navarro/navarroNode.h"

std::unique_ptr<GfaGraph> graph;

int main() {
    std::string inputDir = R"(C:\Users\pavao\Documents\faks\bioinf\lab\navarro\input\)";
    graph = std::unique_ptr<GfaGraph>(GfaGraph::loadFromFile(
//            inputDir + "ref10000_linear.gfa"
//            inputDir + "jere/ref10000_onechar.gfa"
//    inputDir + "jere/ref10000_snp.gfa"
    inputDir + "jere/ref10000_twopath.gfa"
//            inputDir + "ref10000_onechar.gfa"
//            inputDir + "ref10000_tangle.gfa"
//            inputDir + "ref10000_snp.gfa"
//            inputDir + "ref10000_twopath.gfa"
//            inputDir + "example.gfa"
    ));
    std::cout << inputDir << std::endl;
    int n = graph->vertices.size();

    //samo obicni nodeovi
//    for (int i = 1; i <= n; ++i) {
//        Node node = graph->vertices[i];
//        std::cout << node.id << "(" << node.sequence << "): ";
//        for (auto &it : node.outEdges) {
//            Node child = graph->vertices[it.to];
//            std::cout << child.id << "(" << child.sequence << "), ";
//        }
//        std::cout << std::endl;
//    }

    // posebni nodeovi koji idu char po char
    std::unordered_set<NodePosition> front;
    for (auto &it: graph->vertices) {
        Node node = it.second;
//        front.insert(NodePosition(node, false));
        for (auto &e: node.inEdges) {
            front.insert(NodePosition(node, e));
        }
        for (auto &e: node.outEdges) {
            front.insert(NodePosition(node, e));
        }
        if (node.inEdges.empty() && node.outEdges.empty()) {
            front.insert(NodePosition(node));
        }
    }
    std::unordered_set<NodePosition> allPos;
    allPos.insert(front.begin(), front.end());
    bool addedNew;
    do {
        addedNew = false;
        std::unordered_set<NodePosition> frontNew;
        for (auto &np : front) {
            for (auto &newNP : np.previous(graph.get())) {
                bool isIn = allPos.find(newNP) != allPos.end();
                if (!isIn) {
                    addedNew = true;
                    frontNew.insert(newNP);
                }
            }

            for (auto &newNP : np.next(graph.get())) {
                bool isIn = allPos.find(newNP) != allPos.end();
                if (!isIn) {
                    addedNew = true;
                    frontNew.insert(newNP);
                }
            }
        }
//        std::cout << "size: " << frontNew.size() << std::endl;
//        for(auto p : frontNew){
//            std::cout << p << std::endl;
//        }
        front.clear();
        front.insert(frontNew.begin(), frontNew.end());
        allPos.insert(front.begin(), front.end());
    } while (addedNew);
    //allPos su svi node-ovi onda se na njima radi previous i sto vec treba
    //preporucam za onaj Cv radit novu unordered_map<NodePosition, int> pa imat Cv i Cv' mape za racunanje 'retka' i novog 'retka'
    std::cout << "All node(char): " << std::endl;
    for (auto &nodePos: allPos) {
        std::cout << nodePos << std::endl;
        for (auto &child : nodePos.previous(graph.get())) {
            std::cout << "\t" << child << std::endl;
        }
        std::cout << "\t--------------------------------------" << std::endl;
        for (auto &child : nodePos.next(graph.get())) {
            std::cout << "\t" << child << std::endl;
        }
    }

    std::vector<FastQ> queries = loadFastqFromFile(
//            R"(C:\Users\pavao\Documents\faks\bioinf\lab\navarro\input\ref10000_simulatedreads.fastq)"
                    R"(C:\Users\pavao\Documents\faks\bioinf\lab\navarro\input\jere\ref10000_simulatedreads.fastq)"
//            R"(C:\Users\pavao\Documents\faks\bioinf\lab\navarro\input\example.fastq)"
    );

    int s = score(queries[0], allPos, graph.get());
//    int s = score(queries[0], allPos, graph.get());
    std::cout << s << std::endl;
    return 0;
}
