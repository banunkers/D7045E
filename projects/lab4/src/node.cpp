#include "node.h"

Node::Node(glm::mat4x4 model) {
    this->children = std::vector<Node*>();
    this->model = model;
}

void Node::add(Node* node) {
    this->children.push_back(node);
}