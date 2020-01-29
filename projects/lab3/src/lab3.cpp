#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "lab3.h"
#include "simple_cube.h"
#include "types.h"

using namespace Display;

// object settings
const GLfloat transDistance = 0.1f;
const GLfloat rotAngle = 10.0f;
unsigned int focusedObject = 0;

// camera settings
// const glm::vec3 cameraPos = glm::vec3(-3.0f, 0.0f, 0.0f);
const glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
const glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

Lab3::Lab3() {}
Lab3::~Lab3() {}

bool Lab3::Open() {
    App::Open();

    this->window = new Display::Window;
    this->window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            switch (key) {
                case GLFW_KEY_ESCAPE:
                    this->window->Close();
                    break;
                case GLFW_KEY_1: case GLFW_KEY_2: case GLFW_KEY_3:
                    focusedObject = key - GLFW_KEY_1;
                    printf("Object %u focused\n", focusedObject + 1);
                    break;
                case GLFW_KEY_W:
                    this->scene[focusedObject].translate(glm::vec3(0.0f, 0.0f, -transDistance));
                    break;
                case GLFW_KEY_S:
                    this->scene[focusedObject].translate(glm::vec3(0.0f, 0.0f, transDistance));
                    break;
                case GLFW_KEY_A:
                    this->scene[focusedObject].translate(glm::vec3(-transDistance, 0.0f, 0.0f));
                    break;
                case GLFW_KEY_D:
                    this->scene[focusedObject].translate(glm::vec3(transDistance, 0.0f, 0.0f));
                    break;
                case GLFW_KEY_LEFT:
                    this->scene[focusedObject].rotate(-rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));
                    break;
                case GLFW_KEY_RIGHT:
                    this->scene[focusedObject].rotate(rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));
                    break;
                case GLFW_KEY_UP:
                    this->scene[focusedObject].rotate(-rotAngle, glm::vec3(1.0f, 0.0f, 0.0f));
                    break;
                case GLFW_KEY_DOWN:
                    this->scene[focusedObject].rotate(rotAngle, glm::vec3(1.0f, 0.0f, 0.0f));
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
        glEnable(GL_DEPTH_TEST);
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
        createSimpleCube(Color(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(-0.85f, 0.0f, 0.0f)),
        createSimpleCube(Color(0.0f, 1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
        createSimpleCube(Color(0.0f, 0.0f, 1.0f, 1.0f), glm::vec3(0.85f, 0.0f, 0.0f))
    };
    this->camera = new Camera(cameraPos, cameraTarget);
}

void Lab3::Run() {
    while (this->window->IsOpen()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        this->window->Update();

        for (auto& object : this->scene) {
            object.draw(this->camera->getView());
        }

        this->window->SwapBuffers();
    }
}