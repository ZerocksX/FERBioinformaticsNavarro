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
    char orientation;

    std::vector<NodePosition> next(GfaGraph *gfaGraph) const;

    std::vector<NodePosition> previous(GfaGraph *gfaGraph) const;

    char getCurrentChar() const;

    NodePosition(const Node &node, int position, const Edge &edge, char orientation);

    NodePosition(Node &node, Edge edge);
    NodePosition(Node &node);

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


#endif //NAVARRO_GFAGRAPH_H
