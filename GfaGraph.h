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
    Node node;
    int position;
    Edge edge;
    bool reverse;
    char orientation;

    std::vector<NodePosition> next(GfaGraph *gfaGraph);

    std::vector<NodePosition> previous(GfaGraph *gfaGraph);

    char getCurrentChar() const;

    NodePosition(const Node &node, int position, const Edge &edge, bool reverse, char orientation);

    NodePosition(Node &node, Edge edge, bool reverse);
    NodePosition(Node &node, bool reverse);

    friend std::ostream &operator<<(std::ostream &os, const NodePosition &position);

    bool operator==(const NodePosition &rhs) const;

    bool operator!=(const NodePosition &rhs) const;
};


namespace std {
    template<>
    struct hash<NodePosition> {
        size_t operator()(const NodePosition &x) const {
            return hash<Node>()(x.node) ^ hash<int>()(x.position) ^ /*hash<Edge>()(x.edge) ^*/ hash<bool>()(x.reverse) ^
                   hash<char>()(x.orientation);
        }


        size_t operator()(const std::shared_ptr<NodePosition> &x) const {
            return hash<Node>()(x->node) ^ hash<int>()(x->position) ^ /*hash<Edge>()(x->edge) ^*/ hash<bool>()(x->reverse) ^
                   hash<char>()(x->orientation);
        }
    };
}


#endif //NAVARRO_GFAGRAPH_H
