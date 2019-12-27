#include <iostream>
#include <algorithm>
#include "GfaGraph.h"

int cmp(NodePos n1, NodePos n2) {
    return n1.id < n2.id;
}

int main() {
    std::string inputDir = "C:/Users/pavao/Documents/faks/bioinf/lab/GraphAligner/WabiExperimentSnake/tmp/";
    GfaGraph graph = GfaGraph::LoadFromFile(
            inputDir + "ref10000_onechar.gfa"
//            inputDir + "ref10000_tangle.gfa"
//            inputDir + "ref10000_snp.gfa"
//            inputDir + "ref10000_twopath.gfa"
    );
    int n = graph.nodes.size();
    std::vector<NodePos> nodes(n);
    int i = 0;
    for (auto &kv : graph.nodes) {
        // if end is false then there are no neighbours so graph.edges[node] returns null either way
        nodes[i] = NodePos(kv.first, true);
        ++i;
    }
    std::sort(nodes.begin(), nodes.end(), cmp); // ordered ascending instead of hash from unsorted map
    for (NodePos node : nodes) {
        std::cout << node.id << "(" << graph.nodes[node.id] << "): ";
        for (NodePos neighbour : graph.edges[node]) {
            std::cout << neighbour.id << "(" << graph.nodes[neighbour.id] << "), ";
        }
        std::cout << std::endl;
    }
    return 0;
}