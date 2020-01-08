//
// Created by pavao on 8.1.2020..
//

#include "Node.h"

#include <utility>


Edge::Edge(std::shared_ptr<Node> from, std::shared_ptr<Node> to, char fromOrientation, char toOrientation,
           int overlap) : from(std::move(from)), to(std::move(to)), fromOrientation(fromOrientation),
                          toOrientation(toOrientation),
                          overlap(overlap) {}


void Node::addChild(Edge *edge) {
    Node *to = edge->to.get();
    this->outEdges.insert(std::move(std::shared_ptr<Edge>(edge)));
    to->inEdges.insert(std::move(std::make_shared<Edge>(
            edge->to,
            edge->from,
            edge->toOrientation,
            edge->fromOrientation,
            edge->overlap
    )));
}

bool Node::operator==(const Node &rhs) const {
    return id == rhs.id;
}

bool Node::operator!=(const Node &rhs) const {
    return !(rhs == *this);
}

bool Node::operator<(const Node &rhs) const {
    return id < rhs.id;
}

bool Node::operator>(const Node &rhs) const {
    return rhs < *this;
}

bool Node::operator<=(const Node &rhs) const {
    return !(rhs < *this);
}

bool Node::operator>=(const Node &rhs) const {
    return !(*this < rhs);
}

std::ostream &operator<<(std::ostream &os, const Node &node) {
    os << "id: " << node.id << " sequence: " << node.sequence;
    return os;
}


namespace std {
    template<>
    struct hash<std::pair<int, char>> {
        size_t operator()(const std::pair<int, char> &x) const {
            return hash<int>()(x.first) ^ hash<char>()(x.second);
        }
    };
}
