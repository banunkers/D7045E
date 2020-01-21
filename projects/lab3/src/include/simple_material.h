#pragma once

#include "material.h"
#include "types.h"

class SimpleMaterial : public Material {
    private:
        Color color;
        const GLchar* vertexShader;
        const GLchar* fragmentShader;

    public:
        SimpleMaterial(Color color);
        
        void applyMaterial();
};