#pragma once

#include <vector>
#include <GL/glew.h>

#include "types.h"

class Mesh {
    private:
        GLuint vertexArrayObject;
        GLuint vertexBufferObject;
        GLuint elementBufferObject;
        std::vector<Vertex>* vertices;
        std::vector<GLuint>* indices;

        void initMesh();

    public:
        Mesh(std::vector<Vertex>* vertices, std::vector<GLuint>* indices);
        ~Mesh();
        
        void bindVAO();
        void unbindVAO();
        unsigned int getNumIndices();
};