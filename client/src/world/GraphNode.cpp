//
// Created by Alexander Winter on 2023-04-29.
//

#include "world/GraphNode.h"

GraphNode::GraphNode(int id, sf::Vector2f position)
    : neighbors(), id(id), position(position) {

}

const std::vector<GraphNode*>& GraphNode::getNeighbors() const {
    return neighbors;
}

const sf::Vector2f& GraphNode::getPosition() const {
    return position;
}

void GraphNode::addNeighbor(GraphNode* node) {
    neighbors.push_back(node);
}

void GraphNode::setPosition(const sf::Vector2f& position) {
    GraphNode::position = position;
}

int GraphNode::getId() {
    return id;
}
