#pragma once

#include <vector>

#include "config.h"
#include "core/app.h"
#include "render/window.h"
#include "graphic_node.h"
#include "camera.h"

class Lab3 : public Core::App {
    public:
        
        Lab3();
        ~Lab3();

        bool Open();
        void Run();

    private:
        Display::Window* window;
        std::vector<GraphicNode> scene;
        Camera* camera;

        void initScene();
};