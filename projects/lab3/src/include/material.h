#pragma once

#include "shader_program.h"

class Material {
    private:
        ShaderProgram* shaderProgram;

    public:
        Material(ShaderProgram* shaderProgram) {
            this->shaderProgram = shaderProgram;
        }

        ShaderProgram* getShaderProgram() {
            return this->shaderProgram;
        }
        
        virtual void applyMaterial() = 0;
};
