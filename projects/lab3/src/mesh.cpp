#include "mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices) {
    this->vertices = vertices;
    this->indices = indices;
    initMesh();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &this->vertexArrayObject);
    glDeleteBuffers(1, &this->vertexBufferObject);
    glDeleteBuffers(1, &this->elementBufferObject);
}

void Mesh::initMesh() {
    // setup VAO
    glGenVertexArrays(1, &this->vertexArrayObject);
    glBindVertexArray(this->vertexArrayObject);

    // setup VBO
    glGenBuffers(1, &this->vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);

    // setup EBO
    glGenBuffers(1, &this->elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);

    // setup vertex attrib pointer
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // unbind to avoid accidental modification
    glBindVertexArray(0);
}

void Mesh::bindVAO() {
    glBindVertexArray(this->vertexArrayObject);
}

void Mesh::unbindVAO() {
    glBindVertexArray(0);
}

unsigned int Mesh::getNumIndices() {
    return this->indices.size();
}
