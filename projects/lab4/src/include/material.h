#pragma once

#include "shader_program.h"

class Material {
    protected:
        ShaderProgram* shaderProgram;

    public:
        Material(ShaderProgram* shaderProgram) {
            this->shaderProgram = shaderProgram;
        }
        
        virtual void applyMaterial(glm::mat4 transform, glm::mat4 view) = 0;
};
