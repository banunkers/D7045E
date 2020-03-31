#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "mesh.h"
#include "material.h"
#include "node.h"

class GraphicNode : public Node {
    private:
        Mesh* mesh;
        Material* material;

    public:
        GraphicNode(Mesh* mesh, Material* material, glm::mat4 model);

        void update() override;
        void draw(glm::mat4 view);
};