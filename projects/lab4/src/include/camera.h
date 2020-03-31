#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "node.h"

class Camera : public Node {
    private:
        glm::vec3 position;
        glm::vec3 target;
        glm::mat4 view;
        const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);   // points upwards in world space

    public:
        Camera(glm::vec3 position, glm::vec3 target);
        glm::mat4 getView();
};