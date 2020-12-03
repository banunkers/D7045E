
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target) : Node(this->model) {
    this->position = position;
    this->target = target;
    this->model = glm::lookAt(this->position, this->target, this->up);
}
