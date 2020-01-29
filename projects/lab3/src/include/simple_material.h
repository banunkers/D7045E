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

            layout(location=0) out float dist;

            void main()
            {
                vec4 position = u_projection * u_view * u_model * vec4(a_position, 1);
                dist = distance(u_view[3].xyzw, position);
                gl_Position = position;
            }
        )";
        // here a_distance is the distance between the vertex position and
        // the static cameras position (light source)
        static constexpr const GLchar* fragmentShader = R"(
            #version 310 es
            precision mediump float;
            
            layout(location=0) in float a_distance;

            uniform vec4 u_color;

            out vec4 Color;
            
            void main()
            {
                Color = vec4(u_color.r / a_distance, u_color.g / a_distance, u_color.b / a_distance, u_color.a);
            }
        )";

    public:
        SimpleMaterial(Color color);
        
        void applyMaterial(glm::mat4 transform, glm::mat4 view);
};