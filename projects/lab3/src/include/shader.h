#pragma once

#include <cstring>
#include <iostream>
#include <GL/glew.h>

class Shader {
    public:
        enum Type {
            VERTEX,
            FRAGMENT
        };
        Shader(const GLchar* source, Type type);
        ~Shader();

        GLuint getHandle();
        Type getType();
    
    private:
        GLuint shaderHandle;
        Type type;
};