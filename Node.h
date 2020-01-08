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

class Node {
public:
    int id;
    std::string sequence;
    std::unordered_set<std::shared_ptr<Edge>> inEdges;
    std::unordered_set<std::shared_ptr<Edge>> outEdges;

    Node() : id{}, sequence{} {};

//    ~Node(){};
    Node(int pid, std::string &psequence) : id{pid}, sequence{psequence} {};

    void addChild(Edge *edge);

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
    std::shared_ptr<Node> from;
    std::shared_ptr<Node> to;
    char fromOrientation;
    char toOrientation;
    int overlap;

    Edge(std::shared_ptr<Node> from, std::shared_ptr<Node> to, char fromOrientation, char toOrientation,
         int overlap);

};


namespace std {
    template<>
    struct hash<Node> {
        size_t operator()(const Node &x) const {
            return hash<long>()(x.id);
        }

        size_t operator()(const std::shared_ptr<Node> &x) const {
            return hash<long>()(x.get()->id);
        }
    };
}


namespace std {
    template<>
    struct hash<Edge> {

        size_t operator()(const std::shared_ptr<Edge> &x) const {
            return hash<int>()(x->from->id) ^ hash<int>()(x->to->id) ^ hash<char>()(x->fromOrientation) ^
                   hash<char>()(x->toOrientation);
        }
    };
}

#endif //NAVARRO_NODE_H
