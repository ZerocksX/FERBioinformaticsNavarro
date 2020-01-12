#include <iostream>
#include <algorithm>
#include <memory>
#include <set>
#include <fastqloader.h>
#include <chrono>
#include "GfaGraph.h"
#include "navarro/navarroNode.h"

std::unique_ptr<GfaGraph> graph;

int main() {
    std::string inputDir = R"(C:\Users\pavao\Documents\faks\bioinf\lab\navarro\input\)";
    graph = std::unique_ptr<GfaGraph>(GfaGraph::loadFromFile(
//            inputDir + "example/example.gfa"
//            inputDir + "data/tmp/ref10000_linear.gfa"
            inputDir + "data2/tmp/ref10000_linear.gfa"
//            inputDir + "data2/tmp/ref10000_tangle.gfa"
//            inputDir + "sego/ref10000_tangle.gfa"
//            inputDir + "sego2/ref10000_linear.gfa"
//    inputDir + "data/tmp/ref10000_tangle.gfa"
//    inputDir + "data/tmp/ref10000_tangle.gfa"
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
        for (auto np : front) {
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
        front.clear();
        front.insert(frontNew.begin(), frontNew.end());
        allPos.insert(front.begin(), front.end());
    } while (addedNew);
    //allPos su svi node-ovi onda se na njima radi previous i sto vec treba
    //preporucam za onaj Cv radit novu unordered_map<NodePosition, int> pa imat Cv i Cv' mape za racunanje 'retka' i novog 'retka'
//    std::cout << "All node(char): " << std::endl;
//    for (auto nodePos: allPos) {
//        std::cout << nodePos << std::endl;
//        for (auto &child : nodePos.previous(graph.get())) {
//            std::cout << "\t" << child << std::endl;
//        }
//        std::cout << "\t--------------------------------------" << std::endl;
//        for (auto &child : nodePos.next(graph.get())) {
//            std::cout << "\t" << child << std::endl;
//        }
//    }

    std::vector<FastQ> queries = loadFastqFromFile(
//                    R"(C:\Users\pavao\Documents\faks\bioinf\lab\navarro\input\jere\ref10000_simulatedreads.fastq)"
//            R"(C:\Users\pavao\Documents\faks\bioinf\lab\navarro\input\example\example.fastq)"
//            R"(C:\Users\pavao\Documents\faks\bioinf\lab\navarro\input\data\tmp\ref10000_simulatedreads.fastq)"
            R"(C:\Users\pavao\Documents\faks\bioinf\lab\navarro\input\data2\tmp\ref10000_simulatedreads.fastq)"
//            R"(C:\Users\pavao\Documents\faks\bioinf\lab\navarro\input\sego\ref10000_simulatedreads.fastq)"
//            R"(C:\Users\pavao\Documents\faks\bioinf\lab\navarro\input\sego2\ref10000_simulatedreads.fastq)"
    );

//    int s = score(queries[19], allPos, graph.get());
    NodePositionGraph npGraph(allPos, graph.get());
//    for (int i = 0; i < npGraph.n; ++i) {
//        auto nodePos = npGraph.nodes[i];
//        std::cout << nodePos << std::endl;
//        for (auto &parent : npGraph.previousNodes[i]) {
//            std::cout << "\t" << npGraph.nodes[parent] << std::endl;
//        }
//        std::cout << "\t--------------------------------------" << std::endl;
//        for (auto &child : npGraph.nextNodes[i]) {
//            std::cout << "\t" << npGraph.nodes[child] << std::endl;
//        }
//    }
    auto start = std::chrono::high_resolution_clock::now();
    auto scores = std::vector<int>();
    for (int k = 0; k < queries.size(); k++) {
        auto q = queries[k];
        if (k % 2 == 1) {
            std::string seq = "";
            for (int i = q.sequence.size() - 1; i >= 0; --i) {
                switch (q.sequence[i]) {
                    case 'A':
                        seq += 'T';
                        break;
                    case 'T':
                        seq += 'A';
                        break;
                    case 'C':
                        seq += 'G';
                        break;
                    case 'G':
                        seq += 'C';
                        break;
                }
            }
            q.sequence = seq;
        }

        int s = score(q, npGraph);
//    int s = score(queries[0], npGraph);
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
