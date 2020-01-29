#pragma once

#include <vector>
#include <GL/glew.h>

#include "shader.h"

class ShaderProgram {
    private:
        std::vector<Shader> shaders;

        void linkShaders();

    public:
        GLuint program;
        
        ShaderProgram(const std::vector<Shader>& shaders);
        ~ShaderProgram();

        void activateProgram();
};