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
        GraphicNode(Mesh* mesh, Material* material, glm::mat4 transform) {
            this->mesh = mesh;
            this->material = material;
            this->transform = transform;
        }

        ~GraphicNode() {}

        void translate(glm::vec3 matrix) {
            this->transform = glm::translate(this->transform, matrix);
        }

        void rotate(float degrees, glm::vec3 rotAxis) {
            this->transform = glm::rotate(this->transform, glm::radians(degrees), rotAxis);
        }

        void draw() {
            this->mesh->bindVAO();
            this->material->applyMaterial();
            glDrawElements(GL_TRIANGLES, this->mesh->getNumIndices(), GL_UNSIGNED_INT, (void*)0);
            this->mesh->unbindVAO();
        }
};