#include "graphic_node.h"

GraphicNode::GraphicNode(Mesh* mesh, Material* material, glm::mat4 transform) {
    this->mesh = mesh;
    this->material = material;
    this->transform = transform;
}

void GraphicNode::translate(glm::vec3 matrix) {
    this->transform = glm::translate(this->transform, matrix);
}

void GraphicNode::rotate(float degrees, glm::vec3 rotAxis) {
    this->transform = glm::rotate(this->transform, glm::radians(degrees), rotAxis);
}

void GraphicNode::draw(glm::mat4 view) {
    this->mesh->bindVAO();
    this->material->applyMaterial(this->transform, view);
    glDrawElements(GL_TRIANGLES, this->mesh->getNumIndices(), GL_UNSIGNED_INT, (void*)0);
    this->mesh->unbindVAO();
}