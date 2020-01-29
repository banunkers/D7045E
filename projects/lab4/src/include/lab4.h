#pragma once

#include <vector>

#include "config.h"
#include "core/app.h"
#include "render/window.h"
#include "graphic_node.h"
#include "camera.h"

class Lab4 : public Core::App {
    public:
        
        Lab4();
        ~Lab4();

        bool Open();
        void Run();

    private:
        Display::Window* window;
        std::vector<GraphicNode> scene;
        Camera* camera;

        void initScene();
};