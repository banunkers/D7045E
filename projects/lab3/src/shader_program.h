#pragma once

#include <iostream>
#include <vector>

#include "glew.h"
#include "shader.h"

class ShaderProgram {
    private:
        std::vector<Shader*> shaders;
        GLuint program;

        void linkShaders() {
            this->program = glCreateProgram();

            for (auto const& shader: shaders) {
                glAttachShader(this->program, shader->getHandle());
            }

            glLinkProgram(this->program);

            // error log
            GLint shaderLogSize;
            glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
            if (shaderLogSize > 0) {
				GLchar* buf = new GLchar[shaderLogSize];
				glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
				printf("[PROGRAM LINK ERROR]: %s", buf);
				delete[] buf;
			}
        }

    public:
        ShaderProgram(std::vector<Shader*> shaders) {
            this->shaders = shaders;
            linkShaders();
        }

        ~ShaderProgram() {
            glDeleteProgram(this->program);
        }

        void activateProgram() {
            glUseProgram(this->program);
        }
};