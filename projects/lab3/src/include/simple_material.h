#pragma once

#include "material.h"
#include "types.h"

class SimpleMaterial : public Material {
    private:
        Color color;
        static constexpr const GLchar* vertexShader = 
        "#version 310 es\n"
        "precision mediump float;\n"
        "layout(location=0) in vec3 pos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(pos, 1);\n"
        "}\n";

        static constexpr const GLchar* fragmentShader = 
        "#version 310 es\n"
        "precision mediump float;\n"
        "out vec4 Color;\n"
        "uniform vec4 u_color;\n"
        "void main()\n"
        "{\n"
        "   Color = u_color;\n"
        "}\n";

    public:
        SimpleMaterial(Color color);
        
        void applyMaterial();
};