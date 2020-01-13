#include <iostream>
#include <algorithm>
#include <memory>
#include <set>
#include <fastqloader.h>
#include <chrono>
#include "GfaGraph.h"
#include "navarro/navarroNode.h"


std::unordered_set<NodePosition> getAllNodes(GfaGraph *pGraph);

void printGraph(std::unordered_set<NodePosition> set, GfaGraph *pGraph);

void printNPGraph(NodePositionGraph graph);

int main(int argc, char **argv) {
    std::string gfaPath;
    if (argc != 3) {
        std::cout << "Need to input two arguments, gfaPath and fastq path";
        return -1;
    }
    gfaPath = argv[1];

    std::unique_ptr<GfaGraph> graph = std::unique_ptr<GfaGraph>(GfaGraph::loadFromFile(
            gfaPath
    ));
    std::cout << gfaPath << std::endl;
    int n = graph->vertices.size();
    std::unordered_set<NodePosition> allPos = getAllNodes(graph.get());
//    printGraph(allNodes, graph.get());

    std::string fastqPath = argv[2];
    std::vector<FastQ> queries = loadFastqFromFile(
            fastqPath
    );

    NodePositionGraph npGraph(allPos, graph.get());
//    printNPGraph(npGraph);
    auto start = std::chrono::high_resolution_clock::now();
    auto scores = std::vector<int>();
    std::cout << npGraph.n << std::endl;
    for (int k = 0; k < queries.size(); k++) {
        auto q = queries[k];
        if (k % 2 == 1) {
            q = q.reverseComplement();
//            std::string seq = "";
//            for (int i = q.sequence.size() - 1; i >= 0; --i) {
//                switch (q.sequence[i]) {
//                    case 'A':
//                        seq += 'T';
//                        break;
//                    case 'T':
//                        seq += 'A';
//                        break;
//                    case 'C':
//                        seq += 'G';
//                        break;
//                    case 'G':
//                        seq += 'C';
//                        break;
//                }
//            }
//            q.sequence = seq;
        }
        int s = score(q, npGraph);
        std::cout << s << std::endl;
        scores.push_back(s);
    }
    for (int i = 0; i < scores.size(); ++i) {
        std::cout << i << ": " << scores[i] << std::endl;
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << std::endl;
    return 0;
}

void printNPGraph(NodePositionGraph npGraph) {

    for (int i = 0; i < npGraph.n; ++i) {
        auto nodePos = npGraph.nodes[i];
        std::cout << nodePos << std::endl;
        for (auto &parent : npGraph.previousNodes[i]) {
            std::cout << "\t" << npGraph.nodes[parent] << std::endl;
        }
        std::cout << "\t--------------------------------------" << std::endl;
        for (auto &child : npGraph.nextNodes[i]) {
            std::cout << "\t" << npGraph.nodes[child] << std::endl;
        }
    }
}

void printGraph(std::unordered_set<NodePosition> allPos, GfaGraph *pGraph) {

//    allPos su svi node-ovi onda se na njima radi previous i sto vec treba
//    preporucam za onaj Cv radit novu unordered_map<NodePosition, int> pa imat Cv i Cv' mape za racunanje 'retka' i novog 'retka'
    std::cout << "All node(char): " << std::endl;
    for (auto nodePos: allPos) {
        std::cout << nodePos << std::endl;
        for (auto &child : nodePos.previous(pGraph)) {
            std::cout << "\t" << child << std::endl;
        }
        std::cout << "\t--------------------------------------" << std::endl;
        for (auto &child : nodePos.next(pGraph)) {
            std::cout << "\t" << child << std::endl;
        }
    }
}

std::unordered_set<NodePosition> getAllNodes(GfaGraph *pGraph) {

    std::unordered_set<NodePosition> front;
    for (auto &it: pGraph->vertices) {
        Node node = it.second;
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
        for (auto np : front) {
            for (auto &newNP : np.previous(pGraph)) {
                bool isIn = allPos.find(newNP) != allPos.end();
                if (!isIn) {
                    addedNew = true;
                    frontNew.insert(newNP);
                }
            }

            for (auto &newNP : np.next(pGraph)) {
                bool isIn = allPos.find(newNP) != allPos.end();
                if (!isIn) {
                    addedNew = true;
                    frontNew.insert(newNP);
                }
            }
        }
        front.clear();
        front.insert(frontNew.begin(), frontNew.end());
        allPos.insert(front.begin(), front.end());
    } while (addedNew);
    return allPos;
}
