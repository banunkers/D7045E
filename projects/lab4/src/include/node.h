#pragma once

#include <vector>
#include <glm/mat4x4.hpp>

/**
 * Scene graph node
 **/
class Node {
    private:
        glm::mat4 model;
        std::vector<Node*> children;

    public:
        Node(glm::mat4 model);

        void update();
        void add(Node* node);
};