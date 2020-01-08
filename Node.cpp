//
// Created by pavao on 8.1.2020..
//

#include "Node.h"


bool Edge::operator==(const Edge &rhs) const {
    return from == rhs.from &&
           to == rhs.to &&
           fromOrientation == rhs.fromOrientation &&
           toOrientation == rhs.toOrientation;
}

bool Edge::operator!=(const Edge &rhs) const {
    return !(rhs == *this);
}

Edge::Edge() {}

Edge::Edge(int from, int to, char fromOrientation, char toOrientation, int overlap) : from(from), to(to),
                                                                                      fromOrientation(fromOrientation),
                                                                                      toOrientation(toOrientation),
                                                                                      overlap(overlap) {}


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
