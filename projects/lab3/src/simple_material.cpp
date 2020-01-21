#pragma once

#include "simple_material.h"

SimpleMaterial::SimpleMaterial(Color color)
    : Material(
        new ShaderProgram(std::vector<Shader*> {
            new Shader(this->vertexShader, Shader::VERTEX),
            new Shader(this->fragmentShader, Shader::FRAGMENT)
        })
    ) {
    this->color = color;
}

void SimpleMaterial::applyMaterial() {
    this->getShaderProgram()->activateProgram();
}
