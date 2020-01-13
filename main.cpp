#include <iostream>
#include <algorithm>
#include <memory>
#include <set>
#include <fastqloader.h>
#include <chrono>
#include "GfaGraph.h"
#include "navarro/navarroNode.h"
#include "main.h"

void printBacktrack(std::vector<std::vector<Matching>> &backtrack, int i, NodePositionGraph &graph, FastQ query);

int main(int argc, char **argv) {
    std::ios::sync_with_stdio(false);
    std::string gfaPath;
    if (argc != 3) {
        std::cout << "Need to input two arguments, gfaPath and fastq path";
        return -1;
    }
    gfaPath = argv[1];

    GfaGraph* graph = GfaGraph::loadFromFile(
            gfaPath
    );
    std::cout << gfaPath << std::endl;
    int n = graph->vertices.size();
    std::unordered_set<NodePosition> allPos = getAllNodes(graph);
//    printGraph(allPos, graph.get());

    std::string fastqPath = argv[2];
    std::vector<FastQ> queries = loadFastqFromFile(
            fastqPath
    );

    NodePositionGraph npGraph(allPos, graph);
//    printNPGraph(npGraph);
    auto start = std::chrono::high_resolution_clock::now();
    auto scores = std::vector<int>();
    std::cout << npGraph.n << std::endl;
    for (int k = 0; k < queries.size(); k++) {
        auto q = queries[k];
        if (k % 2 == 1) {
            q = q.reverseComplement();
        }
        std::vector<std::vector<Matching>> backtrack(q.sequence.size());
        std::vector<int> cv = score(q, npGraph, backtrack);
        int i = -1;
        int min = 1 << 30u;
        for (int k = 0; k < npGraph.n; ++k) {
//            std::cout << npGraph.nodes[k] << " " << cv[k] << std::endl;
            if (cv[k] < min) {
                min = cv[k];
                i = k;
            }
        }
        int s = min;

//        for (int k = 0; k < npGraph.n; ++k) {
//            if(s==cv[k]){
//                Matching m = backtrack[q.sequence.size()][i];
//                printBacktrack(backtrack, k, npGraph, q);
//            }
//        }
//        Matching m = backtrack[q.sequence.size()][i];
//        printBacktrack(backtrack, i, npGraph, q);
        std::cout << s << std::endl;
        scores.push_back(s);
    }
    for (int i = 0; i < scores.size(); ++i) {
        std::cout << i << ": " << scores[i] << std::endl;
    }
    std::cout << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << std::endl;
    delete graph;
    return 0;
}

void printBacktrack(std::vector<std::vector<Matching>> &backtrack, int i, NodePositionGraph &graph, FastQ query) {
    Matching match = backtrack[query.sequence.size()][i];
    std::vector<char> chars;
    std::vector<int> ops;
    chars.push_back(graph.nodes[i].getCurrentChar());
    ops.push_back(match.op);
    while (true) {
        if (match.parentColumn == -1 || match.parentRow == -1) {
            ops.push_back(match.op);
            break;
        }
        chars.push_back(graph.nodes[match.parentColumn].getCurrentChar());
        ops.push_back(match.op);
        match = backtrack[match.parentRow][match.parentColumn];
    }
//    chars.push_back(' ');
    std::reverse(chars.begin(), chars.end());
//    ops.push_back(-1);
    std::reverse(ops.begin(), ops.end());
    ops.pop_back();
    for (char c: chars) {
        std::cout << c;
    }
    std::cout << std::endl;
    for (int op: ops) {
        switch (op) {
            case -1:
                std::cout << "B";
                break;
            case 0:
                std::cout << "M";
                break;
            case 1:
                std::cout << "I";
                break;
            case 2:
                std::cout << "m";
                break;
        }
    }
    std::cout << std::endl;
    int k = 0;
    for (int op : ops) {
        if (op == 1) {
            std::cout << "-";
        } else {
            std::cout << query.sequence[k++];
        }
    }
    std::cout << std::endl;
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
