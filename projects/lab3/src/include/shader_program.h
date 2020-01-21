#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>

#include "shader.h"

class ShaderProgram {
    private:
        std::vector<Shader*> shaders;
        GLuint program;

        void linkShaders();

    public:
        ShaderProgram(std::vector<Shader*> shaders);
        ~ShaderProgram();

        void activateProgram();
};