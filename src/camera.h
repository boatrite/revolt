#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "window.h"

class Camera {
  private:
    std::shared_ptr<UIContext> m_ui_context_ptr;
    float yaw { 270.0f };
    float pitch { -30.0f };
    glm::vec3 cameraFront { computeCameraFront() };
    glm::vec3 cameraUp { glm::vec3(0.0f, 1.0f,  0.0f) };
    int width;
    int height;
    double lastX {};
    double lastY {};

  public:
    Camera(std::shared_ptr<UIContext> ui_context_ptr);

    static constexpr float MIN_FOV { 22.5f };
    static constexpr float MAX_FOV { 67.5f };

    static constexpr float MIN_NEAR_PLANE { 0.01f };
    static constexpr float MAX_NEAR_PLANE { 10.0f };

    static constexpr float MIN_FAR_PLANE { 0.01f };
    static constexpr float MAX_FAR_PLANE { 1000.0f };

    static constexpr float MIN_SPEED { 1.0f };
    static constexpr float MAX_SPEED { 100.0f };

    static constexpr float MIN_SENSITIVITY { 0.01f };
    static constexpr float MAX_SENSITIVITY { 1.0f };

    float fov { 45.0f };
    float nearPlane { 0.1f };
    float farPlane { 400.0f };
    float speed { 4.0f };
    float sensitivity { 0.05f };

    glm::vec3 position { glm::vec3(0.0f, 0.0f, 3.0f) };

    glm::vec3 computeCameraFront();

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void imguiDebugControlPanel();
    void processInput(float dt);
    void onCursorMoved(double xpos, double ypos);
    void windowSizeCallback(GLFWwindow* window, int width, int height);
    void focusCallback(bool focusedInGame);
};
