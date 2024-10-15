#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>


class Camera
{
public:
    Camera();
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    void moveForward(float delta);
    void moveRight(float delta);
    void moveUp(float delta);
    void rotateRight(float degrees);
    void rotateUp(float degrees);
    void rotateForward(float degrees);

private:
    glm::vec4 forward;
    glm::vec4 right;
    glm::vec4 up;
    glm::vec4 position;
    float fov;
    float nearClip;
    float farClip;
    float aspectRatio;
    void updateCameraVectors();

};

#endif // CAMERA_H
