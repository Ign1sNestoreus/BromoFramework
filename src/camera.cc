#include "camera.hh"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

Camera::Camera(float fov, float aspect, float nearPlane, float farPlane)
    : m_position(0.0f, 0.0f, 3.0f)
    , m_front(0.0f, 0.0f, -1.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_worldUp(0.0f, 1.0f, 0.0f)
    , m_yaw(-90.0f)
    , m_pitch(0.0f)
    , m_fov(fov)
    , m_aspect(aspect)
    , m_near(nearPlane)
    , m_far(farPlane)
{
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix() {
    m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::UpdateProjectionMatrix() {
    m_projMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    // Vulkan требует инвертированный Y
    m_projMatrix[1][1] *= -1;
}

void Camera::ProcessKeyboard(int direction, float deltaTime) {
    float velocity = m_movementSpeed * deltaTime;
    switch (direction) {
        case 0: m_position += m_front * velocity; break;
        case 1: m_position -= m_front * velocity; break;
        case 2: m_position -= m_right * velocity; break;
        case 3: m_position += m_right * velocity; break;
    }
    UpdateViewMatrix();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;
    m_pitch = std::max(-89.0f, std::min(89.0f, m_pitch));

    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));

    UpdateViewMatrix();
}
