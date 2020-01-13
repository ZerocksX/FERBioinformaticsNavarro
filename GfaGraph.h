//
// Created by pavao on 4.1.2020..
//
#ifndef NAVARRO_GFAGRAPH_H
#define NAVARRO_GFAGRAPH_H

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <ostream>
#include "Node.h"


class GfaGraph {
public:
    std::unordered_map<int, Node> vertices;

    static GfaGraph *loadFromFile(std::string fileName);

private:
    GfaGraph();
};


class NodePosition {
public:
    NodePosition(Node &node, Edge edge);

    Node node;
    int position;
    char orientation;
    std::vector<NodePosition> previousNodes;
    std::vector<NodePosition> nextNodes;

    std::vector<NodePosition> next(GfaGraph *gfaGraph);

    std::vector<NodePosition> previous(GfaGraph *gfaGraph);

    char getCurrentChar() const;

    NodePosition(Node node, int position, char orNode);

    explicit NodePosition(Node &node);

    friend std::ostream &operator<<(std::ostream &os, const NodePosition &position);

    bool operator==(const NodePosition &rhs) const;

    bool operator!=(const NodePosition &rhs) const;
};


namespace std {
    template<>
    struct hash<NodePosition> {
        size_t operator()(const NodePosition &x) const {
            return hash<Node>()(x.node) ^ hash<int>()(x.position) ^ /*hash<Edge>()(x.edge) ^*/
                   hash<char>()(x.orientation);
        }


        size_t operator()(const std::shared_ptr<NodePosition> &x) const {
            return hash<Node>()(x->node) ^ hash<int>()(x->position) ^ /*hash<Edge>()(x->edge) ^*/
                   hash<char>()(x->orientation);
        }
    };
}


class NodePositionGraph {
public:
    NodePositionGraph(const std::unordered_set<NodePosition> &nodes, GfaGraph *gfaGraph);
    int n;
    std::unordered_map<NodePosition, int> positions;
    std::vector<NodePosition> nodes;
    std::vector<std::vector<int>> nextNodes;
    std::vector<std::vector<int>> previousNodes;

};


#endif //NAVARRO_GFAGRAPH_H
