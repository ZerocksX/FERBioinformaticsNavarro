//
// Created by pavao on 8.1.2020..
//

#ifndef NAVARRO_NODE_H
#define NAVARRO_NODE_H


#include <string>
#include <unordered_map>
#include <memory>
#include <unordered_set>
#include <ostream>

class Edge;

class EdgeHashFunction {
public:

    // Use sum of lengths of first and last names
    // as hash function.
    size_t operator()(const Edge &x) const;
};

class Node {
public:
    int id;
    std::string sequence;
    std::unordered_set<Edge, EdgeHashFunction> inEdges;
    std::unordered_set<Edge, EdgeHashFunction> outEdges;

    Node() : id{}, sequence{} {};

//    ~Node(){};
    Node(int pid, std::string &psequence) : id{pid}, sequence{psequence} {};

    bool operator==(const Node &rhs) const;

    bool operator!=(const Node &rhs) const;

    bool operator<(const Node &rhs) const;

    bool operator>(const Node &rhs) const;

    bool operator<=(const Node &rhs) const;

    bool operator>=(const Node &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const Node &node);


};


class Edge {
public:
    Node from;
    Node to;
    char fromOrientation;
    char toOrientation;
    int overlap;

    Edge();

    Edge(Node &from, Node &to, char fromOrientation, char toOrientation, int overlap);

    bool operator==(const Edge &rhs) const;

    bool operator!=(const Edge &rhs) const;

};

namespace std {
    template<>
    struct hash<Node> {
        size_t operator()(const Node &x) const {
            return hash<long>()(x.id);
        }
    };
}

namespace std {
    template<>
    struct hash<Edge> {
        size_t operator()(const Edge &x) const {
            return hash<Node>()(x.from) ^ hash<Node>()(x.to) ^ hash<char>()(x.fromOrientation) ^
                   hash<char>()(x.toOrientation);
        }
    };
}

size_t EdgeHashFunction::operator()(const Edge &x) const {
    return std::hash<Edge>()(x);
}


#endif //NAVARRO_NODE_H
