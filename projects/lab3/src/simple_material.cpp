#include <vector>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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

void SimpleMaterial::applyMaterial(glm::mat4 transform, glm::mat4 view) {
    this->shaderProgram->activateProgram();

    // model
    GLint modelUniformLoc = glGetUniformLocation(this->shaderProgram->program, "u_model");
    if (modelUniformLoc != -1) {
        glUniformMatrix4fv(modelUniformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    } else {
        printf("Failed to locate model uniform for SimpleMaterial\n");
    }

    // view
    GLint viewUniformLoc = glGetUniformLocation(this->shaderProgram->program, "u_view");
    if (viewUniformLoc != -1) {
        // glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)); // TODO: should be camera position
        glUniformMatrix4fv(viewUniformLoc, 1, GL_FALSE, glm::value_ptr(view));
    } else {
        printf("Failed to locate view uniform for SimpleMaterial\n");
    }

    // projection
    GLint projUniformLoc = glGetUniformLocation(this->shaderProgram->program, "u_projection");
    if (projUniformLoc != -1) {
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
        glUniformMatrix4fv(projUniformLoc, 1, GL_FALSE, glm::value_ptr(proj));
    } else {
        printf("Failed to locate projection uniform for SimpleMaterial\n");
    }

    // color
    GLint colorUniformLoc = glGetUniformLocation(this->shaderProgram->program, "u_color");
    if (colorUniformLoc != -1) {
        glUniform4f(colorUniformLoc, this->color.r, this->color.g, this->color.b, this->color.a);
    } else {
        printf("Failed to locate color uniform for SimpleMaterial\n");
    }
}