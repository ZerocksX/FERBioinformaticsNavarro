//
// Created by pavao on 4.1.2020..
//
#include <fstream>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>
#include "GfaGraph.h"

char reverseBase(char c) {
    switch (c) {
        case 'A':
            return 'T';
        case 'T':
            return 'A';
        case 'G':
            return 'C';
        case 'C':
            return 'G';
        default:
            return (char) 0;
    }
}

GfaGraph *GfaGraph::loadFromFile(std::string fileName) {
    auto *gfaGraph = new GfaGraph();
    std::ifstream file{fileName};
    std::vector<std::string> sLines;
    std::vector<std::string> lLines;
    while (file.good()) {
        std::string line;
        std::getline(file, line);
        if (!file.good()) break;
        if (line.empty()) continue;
        if (line[0] != 'S' && line[0] != 'L') continue;
        if (line[0] == 'S') {
            sLines.push_back(line);
        }
        if (line[0] == 'L') {
            lLines.push_back(line);
        }
    }
    for (const std::string &line : sLines) {
        std::stringstream sstr{line};
        int id;
        std::string dummy;
        std::string seq;
        sstr >> dummy;
        sstr >> id;
        sstr >> seq;
        gfaGraph->vertices[id] = std::make_shared<Node>(id, seq);
    }
    for (const std::string &line : lLines) {
        std::stringstream sstr{line};
        int from;
        int to;
        std::string fromstart;
        std::string toend;
        std::string dummy;
        int overlap;
        sstr >> dummy;
        sstr >> from;
        sstr >> fromstart;
        sstr >> to;
        sstr >> toend;
        sstr >> overlap;
        Node *fromNode = gfaGraph->vertices[from].get();
        Node *toNode = gfaGraph->vertices[to].get();
        Edge *edge = new Edge(std::shared_ptr<Node>(fromNode), std::shared_ptr<Node>(toNode), fromstart[0], toend[0],
                              overlap);
        fromNode->addChild(edge);
    }
    return gfaGraph;
}


GfaGraph::GfaGraph() {}

std::vector<std::shared_ptr<NodePosition>> NodePosition::next() {
    std::vector<std::shared_ptr<NodePosition>> result;
    std::string seq = this->node->sequence;
    int n = this->node->sequence.size();
    int pos = this->position + 1;
    if (pos < n) {
        result.push_back(std::move(std::make_shared<NodePosition>(this->node, pos, this->edge, false, this->orientation)));
    }
    for (auto &it : this->node->outEdges) {
        if ((n - it->overlap) == pos) {
            result.push_back(std::move(std::make_shared<NodePosition>(it.get()->to, 0, std::shared_ptr<Edge>(it.get()), false, it->toOrientation)));
        }
    }
    return result;
}

std::vector<std::shared_ptr<NodePosition>> NodePosition::previous() {
    std::vector<std::shared_ptr<NodePosition>> result;
    std::string seq = this->node->sequence;
    int n = this->node->sequence.size();
    int pos = this->position + 1;
    if (pos < n) {
        result.push_back(std::move(std::make_shared<NodePosition>(this->node, pos, this->edge, true, this->orientation)));
    }
    for (auto &it : this->node->inEdges) {
        if ((n - it->overlap) == pos) {
            result.push_back(std::move(std::make_shared<NodePosition>(this->node, 0, std::shared_ptr<Edge>(it.get()), true, it->toOrientation)));
        }
    }
    return result;
}

char NodePosition::getCurrentChar() const {
    int n = this->node->sequence.size();
    int p = this->reverse ? n - this->position - 1 : this->position;
    char c;
    if (this->orientation == '-') {
        p = n - p - 1;
        c = reverseBase(this->node->sequence[p]);
    } else {
        c = this->node->sequence[p];
    }
    return c;
}

NodePosition::NodePosition(std::shared_ptr<Node> node, int position, std::shared_ptr<Edge> edge,
                           bool reverse, char orientation) : node(std::move(node)), position(position), edge(std::move(edge)),
                                           reverse(reverse), orientation(orientation) {}

NodePosition::NodePosition(std::shared_ptr<Node> node, std::shared_ptr<Edge> edge) {
    this->node = std::move(node);
    this->position = 0;
    this->edge = std::move(edge);
    this->reverse = false;
    this->orientation = this->edge->fromOrientation;
}

std::ostream &operator<<(std::ostream &os, const NodePosition &position) {
    os << "node: " << *position.node << " position: " << position.position << " reverse: " << position.reverse << " char: " << position.getCurrentChar();
    return os;
}
