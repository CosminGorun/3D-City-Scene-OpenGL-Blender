#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace gps {

    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp)
        : cameraPosition(cameraPosition), cameraTarget(cameraTarget), cameraUpDirection(cameraUp) {

        cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUp));
        cameraUpDirection = glm::normalize(glm::cross(cameraRightDirection, cameraFrontDirection));
    }

    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraPosition + cameraFrontDirection, cameraUpDirection);
    }

    void Camera::move(MOVE_DIRECTION direction, float speed) {
        switch (direction) {
        case MOVE_FORWARD:
            cameraPosition += speed * cameraFrontDirection;
            break;
        case MOVE_BACKWARD:
            cameraPosition -= speed * cameraFrontDirection;
            break;
        case MOVE_LEFT:
            cameraPosition -= speed * cameraRightDirection;
            break;
        case MOVE_RIGHT:
            cameraPosition += speed * cameraRightDirection;
            break;
        }
    }

    void Camera::rotate(float pitch, float yaw) {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFrontDirection = glm::normalize(front);
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
        cameraUpDirection = glm::normalize(glm::cross(cameraRightDirection, cameraFrontDirection));
    }
	glm::vec3 Camera::getCameraPosition() {
		return cameraPosition;
	}
    void Camera::moveInDirection(const glm::vec3 direction, glm::vec3 lookTarget, float speed, float a) {
        cameraPosition += direction * speed;
        glm::vec3 lookDir = glm::normalize(lookTarget-cameraPosition);
        glm::vec3 interpolatedDirection = glm::normalize((1-a)*cameraFrontDirection+a* lookDir);
        cameraFrontDirection = interpolatedDirection;
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
        cameraUpDirection = glm::normalize(glm::cross(cameraRightDirection, cameraFrontDirection));
    }

}
