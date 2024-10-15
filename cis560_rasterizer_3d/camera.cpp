#include "camera.h"

Camera::Camera():
    forward(glm::vec4(0, 0, -1, 0)),
    right(glm::vec4(1, 0, 0, 0)),
    up(glm::vec4(0, 1, 0, 0)),
    position(glm::vec4(0, 0, 10, 1)),
    fov(45.0f),
    nearClip(0.01f),
    farClip(100.0f),
    aspectRatio(1.0f){
    //up date camer infor
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    glm::vec3 pos(position);
    return glm::lookAt(pos, pos + glm::vec3(forward), glm::vec3(up));
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
}

void Camera::moveForward(float value) {
    position += forward * value;
}

void Camera::moveRight(float value) {
    position += right * value;
}

void Camera::moveUp(float value) {
    position += up * value;
}

void Camera::rotateRight(float degrees) {
    float radians = glm::radians(degrees);
    forward = glm::rotate(forward, radians, glm::vec3(up));
    right = glm::rotate(right, radians, glm::vec3(up));
    updateCameraVectors();
}

void Camera::rotateUp(float degrees) {
    float radians = glm::radians(degrees);
    forward = glm::rotate(forward, radians, glm::vec3(right));
    up = glm::rotate(up, radians, glm::vec3(right));
    updateCameraVectors();
}

void Camera::rotateForward(float degrees) {
    float radians = glm::radians(degrees);
    right = glm::rotate(right, radians, glm::vec3(forward));
    up = glm::rotate(up, radians, glm::vec3(forward));
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    // Extract the x, y, z components for cross product
    glm::vec3 forward3(forward.x, forward.y, forward.z);
    glm::vec3 up3(up.x, up.y, up.z);

    // Calculate the new right and up vectors using cross product
    glm::vec3 right3 = glm::normalize(glm::cross(forward3, up3));
    up3 = glm::normalize(glm::cross(right3, forward3));

    // Convert back to glm::vec4 if needed
    right = glm::vec4(right3, 0.0f);
    up = glm::vec4(up3, 0.0f);
}
