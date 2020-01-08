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


class NodePosition {
public:
    std::shared_ptr<Node> node;
    int position;
    std::shared_ptr<Edge> edge;
    bool reverse;
    char orientation;

    std::vector<std::shared_ptr<NodePosition>> next();

    std::vector<std::shared_ptr<NodePosition>> previous();

    char getCurrentChar() const;

    NodePosition(std::shared_ptr<Node> node, int position, std::shared_ptr<Edge> edge, bool reverse, char orientation);

    NodePosition(std::shared_ptr<Node> node, std::shared_ptr<Edge> edge);

    friend std::ostream &operator<<(std::ostream &os, const NodePosition &position);
};


namespace std {
    template<>
    struct hash<NodePosition> {
        size_t operator()(const NodePosition &x) const {
            return hash<Node>()(x.node) ^ hash<int>()(x.position) ^ hash<Edge>()(x.edge) ^ hash<bool>()(x.reverse);
        }
    };
}

class GfaGraph {
public:
    std::unordered_map<int, std::shared_ptr<Node>> vertices;

    static GfaGraph *loadFromFile(std::string fileName);

private:
    GfaGraph();
};


#endif //NAVARRO_GFAGRAPH_H
