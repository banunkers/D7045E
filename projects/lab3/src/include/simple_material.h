#pragma once

#include "material.h"
#include "types.h"

class SimpleMaterial : public Material {
    private:
        Color color;
        static constexpr const GLchar* vertexShader = R"(
            #version 310 es
            precision mediump float;

            layout(location=0) in vec3 a_position;
            
            uniform mat4 u_model;
            uniform mat4 u_view;
            uniform mat4 u_projection;

            void main()
            {
                gl_Position = u_projection * u_view * u_model * vec4(a_position, 1);
            }
        )";
        static constexpr const GLchar* fragmentShader = R"(
            #version 310 es
            precision mediump float;
            
            uniform vec4 u_color;

            out vec4 Color;
            
            void main()
            {
               Color = u_color;
            }
        )";

    public:
        SimpleMaterial(Color color);
        
        void applyMaterial(glm::mat4 transform, glm::mat4 view);
};