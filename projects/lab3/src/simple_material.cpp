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
    this->shaderProgram->activateProgram();

    // set color uniform
    GLint colorUniformLoc = glGetUniformLocation(this->shaderProgram->program, "u_color");
    if (colorUniformLoc != -1) {
        glUniform4f(colorUniformLoc, this->color.r, this->color.g, this->color.b, this->color.a);
    } else {
        printf("Failed to locate color uniform for SimpleMaterial");
    }
}