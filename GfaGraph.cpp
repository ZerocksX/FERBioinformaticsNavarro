//
// Created by pavao on 4.1.2020..
//
#include <fstream>
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
        gfaGraph->vertices[id] = Node(id, seq);
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
        Node fromNode = gfaGraph->vertices[from];
        Node toNode = gfaGraph->vertices[to];
        Edge edge = Edge(fromNode.id, toNode.id, fromstart[0], toend[0],
                         overlap);
        fromNode.outEdges.insert(edge);
        toNode.inEdges.emplace(
                edge.to,
                edge.from,
                edge.toOrientation,
                edge.fromOrientation,
                edge.overlap
        );
        gfaGraph->vertices[from] = fromNode;
        gfaGraph->vertices[to] = toNode;
    }
    return gfaGraph;
}


GfaGraph::GfaGraph() {}

std::vector<NodePosition> NodePosition::next(GfaGraph *gfaGraph) {
    std::vector<NodePosition> result;
    std::string seq = this->node.sequence;
    int n = this->node.sequence.size();
    int pos = this->position + 1;
    if (pos < n) {
        result.emplace_back(this->node, pos, this->edge, false, this->orientation);
    }
    for (auto &it : this->node.outEdges) {
        if ((n - it.overlap) == pos) {
            result.emplace_back(gfaGraph->vertices[it.to], 0, it, false,
                                it.toOrientation);
        }
    }
    return result;
}

std::vector<NodePosition> NodePosition::previous(GfaGraph *gfaGraph) {
    std::vector<NodePosition> result;
    std::string seq = this->node.sequence;
    int n = this->node.sequence.size();
    int pos = this->position + 1;
    if (pos < n) {
        result.emplace_back(this->node, pos, this->edge, true, this->orientation);
    }
    for (auto &it : this->node.inEdges) {
        if ((n - it.overlap) == pos) {
            result.emplace_back(gfaGraph->vertices[it.to], 0, it, true, it.toOrientation);
        }
    }
    return result;
}

char NodePosition::getCurrentChar() const {
    int n = this->node.sequence.size();
    int p = this->reverse ? n - this->position - 1 : this->position;
    char c;
    if (this->orientation == '-') {
        p = n - p - 1;
        c = reverseBase(this->node.sequence[p]);
    } else {
        c = this->node.sequence[p];
    }
    return c;
}


NodePosition::NodePosition(Node &node, Edge edge, bool reverse) {
    this->node = node;
    this->position = 0;
    this->edge = edge;
    this->reverse = reverse;
    this->orientation = this->edge.fromOrientation;
}


NodePosition::NodePosition(Node &node, bool reverse) {
    this->node = node;
    this->position = 0;
    this->reverse = reverse;
    this->orientation = '+';

}

std::ostream &operator<<(std::ostream &os, const NodePosition &position) {
    os << "node: " << position.node << " position: " << position.position << " reverse: " << position.reverse
       << " char: " << position.getCurrentChar();
    return os;
}

bool NodePosition::operator==(const NodePosition &rhs) const {
    return node == rhs.node &&
           position == rhs.position &&
//           edge == rhs.edge &&
           reverse == rhs.reverse &&
           orientation == rhs.orientation;
}

bool NodePosition::operator!=(const NodePosition &rhs) const {
    return !(rhs == *this);
}

NodePosition::NodePosition(const Node &node, int position, const Edge &edge, bool reverse, char orientation) : node(
        node), position(position), edge(edge), reverse(reverse), orientation(orientation) {}

