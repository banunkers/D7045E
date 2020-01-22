#include "lab3.h"

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

        return true;
    }
    return false;
}

void Lab3::Run() {
    while (this->window->IsOpen()) {
        glClear(GL_COLOR_BUFFER_BIT);
        this->window->Update();
    }
}