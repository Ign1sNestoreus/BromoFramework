#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    Camera(float fov, float aspect, float nearPlane, float farPlane);

    void UpdateViewMatrix();
    void UpdateProjectionMatrix();

    const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
    const glm::mat4& GetProjectionMatrix() const { return m_projMatrix; }
    const glm::vec3& GetPosition() const {return m_position;}

    // Обработка ввода
    void ProcessKeyboard(int direction, float deltaTime); // 0=forward,1=back,2=left,3=right
    void ProcessMouseMovement(float xoffset, float yoffset);

    void SetPosition(const glm::vec3& pos) { m_position = pos; UpdateViewMatrix(); }
    void SetAspect(float aspect) { m_aspect = aspect; UpdateProjectionMatrix(); }

private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    float m_yaw;
    float m_pitch;

    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;

    glm::mat4 m_viewMatrix;
    glm::mat4 m_projMatrix;

    const float m_movementSpeed = 5.0f;
    const float m_mouseSensitivity = 0.1f;
};
