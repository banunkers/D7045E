#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "lab3.h"
#include "simple_cube.h"
#include "types.h"

using namespace Display;

Lab3::Lab3() {}

Lab3::~Lab3() {}

bool Lab3::Open() {
    App::Open();

    this->window = new Display::Window;
    this->window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods) {
        if (action == GLFW_PRESS) {
            switch (key) {
                case GLFW_KEY_ESCAPE:
                    this->window->Close();
                    break;
                default:
                    break;
            }
        }
    });
    this->window->SetTitle(std::string("Lab 3"));
    this->window->SetSize(1500, 1500);

    if (this->window->Open()) {
        glClearColor(1.0f, 1.0f, 0.6f, 1.0f);
        initScene();
        return true;
    }
    return false;
}

/**
 * Creates a scene consisting of graphic nodes
 **/
void Lab3::initScene() {
    this->scene = std::vector<GraphicNode> {
        createSimpleCube(Color(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
    };
}

void Lab3::Run() {
    while (this->window->IsOpen()) {
        glClear(GL_COLOR_BUFFER_BIT);
        this->window->Update();

        for (auto& object : this->scene) {
            object.draw();
        }

        this->window->SwapBuffers();
    }
}