#pragma once

#include <vector>

#include "config.h"
#include "core/app.h"
#include "render/window.h"
#include "graphic_node.h"
#include "camera.h"

class Lab4 : public Core::App {
    public:
        enum Mode {
            CAMERA,
            OBJECT
        };

        Lab4();
        ~Lab4();

        bool Open();
        void Run();
        void ControlObject(int32 action, int32 key);
        void ControlCamera(int32 action, int32 key);

    private:
        Display::Window* window;
        std::vector<GraphicNode> scene;
        Camera* camera;
        Mode controlMode = Mode::OBJECT;

        void initScene();
};