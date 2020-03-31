#include "node.h"

Node::Node(glm::mat4x4 model) {
    this->children = std::vector<Node*>();
    this->model = model;
}

void Node::add(Node* node) {
    this->children.push_back(node);
}

void Node::translate(glm::vec3 matrix) {
    this->model = glm::translate(this->model, matrix);
}

void Node::rotate(float degrees, glm::vec3 rotAxis) {
    this->model = glm::rotate(this->model, glm::radians(degrees), rotAxis);
}