#pragma once

#include "material.h"
#include "types.h"

class SimpleMaterial : public Material {
    private:
        Color color;
        const GLchar* vertexShader = R"(
            #version 310 es
            precision mediump float;
            layout(location=0) in vec3 pos;
            void main()
            {
                gl_Position = vec4(pos, 1);
            }
        )";
        const GLchar* fragmentShader = R"(
            #version 310 es
            precision mediump float;
            uniform vec4 u_color;
            void main()
            {
                Color = u_color;
            }
        )";

    public:
        SimpleMaterial(Color color);
        
        void applyMaterial();
};