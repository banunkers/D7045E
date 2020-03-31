#pragma once

#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


/**
 * Scene graph node
 **/
class Node {
    private:
        std::vector<Node*> children;
    
    public:
        glm::mat4 model;

        Node(glm::mat4 model);

        virtual void update() {} // update does nothing if not redefined by subclass
        void add(Node* node);
        void translate(glm::vec3 matrix);
        void rotate(float degress, glm::vec3 rotAxis);
};