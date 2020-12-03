#include "graphic_node.h"

GraphicNode::GraphicNode(Mesh* mesh, Material* material, glm::mat4 model) : Node(model) {
    this->mesh = mesh;
    this->material = material;
}

void GraphicNode::draw(glm::mat4 view) {
    this->mesh->bindVAO();
    this->material->applyMaterial(this->model, view);
    glDrawElements(GL_TRIANGLES, this->mesh->getNumIndices(), GL_UNSIGNED_INT, (void*)0);
    this->mesh->unbindVAO();
}

void GraphicNode::update() {
    
}

