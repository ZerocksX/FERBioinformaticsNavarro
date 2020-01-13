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

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

GfaGraph *GfaGraph::loadFromFile(std::string fileName) {
    auto *gfaGraph = new GfaGraph();
    std::ifstream file{fileName};
    std::vector<std::string> sLines;
    std::vector<std::string> lLines;
    while (file.good()) {
        std::string line;
        std::getline(file, line);
        if(trim(line).empty()){
            continue;
        }
//        if (!file.good()) break;
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
    if (!this->nextNodes.empty()) {
        return this->nextNodes;
    }
    std::vector<NodePosition> result;
    std::string seq = this->node.sequence;
    int n = this->node.sequence.size();
    int pos = this->position + 1;
    if (pos < n && pos >= 0) {
        result.emplace_back(this->node, pos, this->orientation);
    }
    for (auto &it : this->node.outEdges) {
        if ((n - it.overlap) == pos) {
            result.emplace_back(
                    gfaGraph->vertices[it.to],
                    0,
                    it.toOrientation
            );
        }
    }
    this->nextNodes.insert(this->nextNodes.begin(), result.begin(), result.end());
    return result;
}

std::vector<NodePosition> NodePosition::previous(GfaGraph *gfaGraph) {
    if (!this->previousNodes.empty()) {
        return this->previousNodes;
    }
    std::vector<NodePosition> result;
    std::string seq = this->node.sequence;
    int n = this->node.sequence.size();
    int pos = this->position - 1;
    if (pos < n && pos >= 0) {
        result.emplace_back(this->node, pos, this->orientation);
    }
    for (auto &it : this->node.inEdges) {
        if (it.overlap - 1 == pos) {
            result.emplace_back(
                    gfaGraph->vertices[it.to],
                    gfaGraph->vertices[it.to].sequence.size() - 1,
                    it.toOrientation
            );
        }
    }
    this->previousNodes.insert(this->previousNodes.begin(), result.begin(), result.end());
    return result;
}

char NodePosition::getCurrentChar() const {
    int n = this->node.sequence.size();
    int p = this->position;
    char c;
    if (this->orientation == '-') {
        p = n - p - 1;
        c = reverseBase(this->node.sequence[p]);
    } else {
        c = this->node.sequence[p];
    }
    return c;
}


NodePosition::NodePosition(Node &node, Edge edge) : NodePosition(node, 0, edge.fromOrientation) {
}


NodePosition::NodePosition(Node &node) : NodePosition(node, 0, '+') {
}

std::ostream &operator<<(std::ostream &os, const NodePosition &position) {
    os << "node: " << position.node << " position: " << position.position
       << " char: " << position.getCurrentChar();
    return os;
}

bool NodePosition::operator==(const NodePosition &rhs) const {
    return node == rhs.node &&
           position == rhs.position &&
           orientation == rhs.orientation;
}

bool NodePosition::operator!=(const NodePosition &rhs) const {
    return !(rhs == *this);
}

NodePosition::NodePosition(Node node, int position, char orientation) : node(std::move(node)), position(position),
                                                                        orientation(orientation) {
}


NodePositionGraph::NodePositionGraph(const std::unordered_set<NodePosition> &nodes, GfaGraph *gfaGraph) {
    int i = 0;
    this->n = nodes.size();
    for (const auto &node: nodes) {
        this->nodes.push_back(node);
        this->positions[node] = i;
        this->nextNodes.emplace_back();
        this->previousNodes.emplace_back();
        i++;
    }
    for (int node = 0; node < n; ++node) {
        for (const auto &parent : this->nodes[node].previous(gfaGraph)) {
            this->previousNodes[node].push_back(this->positions[parent]);
        }
        for (const auto &child : this->nodes[node].next(gfaGraph)) {
            this->nextNodes[node].push_back(this->positions[child]);
        }
    }
}
