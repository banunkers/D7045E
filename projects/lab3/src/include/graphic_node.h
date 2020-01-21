#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh.h"
#include "material.h"

class GraphicNode {
    private:
        Mesh* mesh;
        Material* material;
        glm::mat4 transform;

    public:
        GraphicNode(Mesh* mesh, Material* material, glm::mat4 transform);
        
        void translate(glm::vec3 matrix);
        void rotate(float degrees, glm::vec3 rotAxis);
        void draw();
};