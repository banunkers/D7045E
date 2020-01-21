#pragma once

#include <cstring>
#include <iostream>
#include <GL/glew.h>

class Shader {
    enum Type {
        VERTEX,
        FRAGMENT
    };
    
    private:
        GLuint shaderHandle;
        Shader::Type type;

    public:
        Shader(GLchar* source, Type type);
        ~Shader();

        GLuint getHandle();
        Type getType();
};