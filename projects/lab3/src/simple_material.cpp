#include <vector>
#include <iostream>

#include "simple_material.h"
#include "material.h"
#include "shader.h"
#include "shader_program.h"
#include "types.h"

SimpleMaterial::SimpleMaterial(Color color) : Material(
        new ShaderProgram(std::vector<Shader> {
            Shader(this->vertexShader, Shader::VERTEX),
            Shader(this->fragmentShader, Shader::FRAGMENT)
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