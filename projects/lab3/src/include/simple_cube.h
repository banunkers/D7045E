#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphic_node.h"
#include "simple_material.h"
#include "mesh.h"
#include "types.h"

GraphicNode createSimpleCube(Color color, glm::vec3 position) {
    std::vector<Vertex> vertices = {
        Vertex(-1.0f/4.0f, -1.0f/4.0f, -1.0f/4.0f), // 0
        Vertex(-1.0f/4.0f, -1.0f/4.0f, 1.0f/4.0f),  // 1
        Vertex(-1.0f/4.0f, 1.0f/4.0f, 1.0f/4.0f),   // 2
        Vertex(1.0f/4.0f, 1.0f/4.0f, -1.0f/4.0f),   // 3
        Vertex(-1.0f/4.0f, 1.0f/4.0f, -1.0f/4.0f),  // 4
        Vertex(1.0f/4.0f, -1.0f/4.0f, 1.0f/4.0f),   // 5
        Vertex(1.0f/4.0f, -1.0f/4.0f, -1.0f/4.0f),  // 6
        Vertex(1.0f/4.0f, 1.0f/4.0f, 1.0f/4.0f)     // 7
    };

    std::vector<GLuint> indices = {
        0, 1, 2, 
        3, 0, 4,
        5, 0, 6,
        3, 6, 0,
        0, 2, 4,
        5, 1, 0,
        2, 1, 5,
        7, 6, 3,
        6, 7, 5,
        7, 3, 4,
        7, 3, 2,
        7, 2, 5
    };

    SimpleMaterial* material = new SimpleMaterial(color);
    Mesh* mesh = new Mesh(vertices, indices);

    return GraphicNode(mesh, material, glm::translate(glm::mat4(1.0f), position));
}