#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/scalar_multiplication.hpp>

#include <functional>

#include "camera.h"
#include "common.h"

Camera::Camera(std::shared_ptr<UIContext> ui_context_ptr) : m_ui_context_ptr { ui_context_ptr } {
  m_ui_context_ptr->addCursorMovedHandler(
    this,
    std::bind(&Camera::onCursorMoved, this, std::placeholders::_1, std::placeholders::_2)
  );

  m_ui_context_ptr->addGameGuiFocusChangedHandler(
    this,
    std::bind(&Camera::focusCallback, this, std::placeholders::_1)
  );

  m_ui_context_ptr->addProcessInput(
    this,
    std::bind(&Camera::processInput, this, std::placeholders::_1)
  );
}

Camera::~Camera() {
  m_ui_context_ptr->removeHandlers(this);
}

void Camera::update() {
  cameraFront = computeCameraFront();
}

glm::vec3 Camera::computeCameraFront() {
  glm::vec3 direction {};
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  return glm::normalize(direction);
}

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(position, position + cameraFront, cameraUp);
}

glm::mat4 Camera::getProjectionMatrix() const {
  return glm::perspective(glm::radians(fov), static_cast<float>(m_ui_context_ptr->getWidth()) / m_ui_context_ptr->getHeight(), nearPlane, farPlane);
}

const glm::vec3& Camera::getPosition() const {
  return position;
}

const glm::vec3& Camera::getCameraFront() const {
  return cameraFront;
}

void Camera::processInput(float dt) {
  if (!m_ui_context_ptr->isFocusedInGame()) {
    return;
  }

  float cameraSpeed = speed * dt;
  if (m_ui_context_ptr->isKeyPressed(GLFW_KEY_W)) {
    position.x += cameraSpeed * cos(glm::radians(yaw));
    position.z += cameraSpeed * sin(glm::radians(yaw));
  }
  if (m_ui_context_ptr->isKeyPressed(GLFW_KEY_S)) {
    position.x -= cameraSpeed * cos(glm::radians(yaw));
    position.z -= cameraSpeed * sin(glm::radians(yaw));
  }
  if (m_ui_context_ptr->isKeyPressed(GLFW_KEY_A)) {
    position -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
  }
  if (m_ui_context_ptr->isKeyPressed(GLFW_KEY_D)) {
    position += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
  }
  if (m_ui_context_ptr->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
    position -= cameraSpeed * cameraUp;
  }
  if (m_ui_context_ptr->isKeyPressed(GLFW_KEY_SPACE)) {
    position += cameraSpeed * cameraUp;
  }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Camera::onCursorMoved(double xpos, double ypos) {
  if (m_ui_context_ptr->isFocusedInGame()) {
    if (!lastX) {
      lastX = xpos;
    }
    if (!lastY) {
      lastY = ypos;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) {
      pitch = 89.0f;
    } else if (pitch < -89.0f) {
      pitch = -89.0f;
    }

    cameraFront = computeCameraFront();
  }
}

void Camera::focusCallback(bool focusedInGame) {
  if (focusedInGame) {
    // Clear out lastX and lastY so that they are re-initialized to the new
    // initial xpos and ypos in cursorPosCallback when going from GUI focus
    // mode to game focus mode.
    lastX = 0;
    lastY = 0;
  }
}
#pragma GCC diagnostic pop

void Camera::imguiDebugControlPanel() {
  if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::Text("Position: (%.0f, %.0f, %.0f)", position.x, position.y, position.z);
    const auto& chunk_position { Chunk::chunkPosition(position) };
    ImGui::Text("Chunk Position: (%.0f, %.0f, %.0f)", chunk_position.x, chunk_position.y, chunk_position.z);
    ImGui::Text("Yaw: %.0f", yaw);
    ImGui::Text("Pitch: %.0f", pitch);
    ImGui::Text("Camera Front: (%.0f, %.0f, %.0f)", cameraFront.x, cameraFront.y, cameraFront.z);
    ImGui::SliderFloat("FOV", &fov, Camera::MIN_FOV, Camera::MAX_FOV);
    ImGui::SliderFloat("Near Plane", &nearPlane, Camera::MIN_NEAR_PLANE, Camera::MAX_NEAR_PLANE);
    ImGui::SliderFloat("Far Plane", &farPlane, Camera::MIN_FAR_PLANE, Camera::MAX_FAR_PLANE);
    ImGui::SliderFloat("Camera Speed", &speed, Camera::MIN_SPEED, Camera::MAX_SPEED);
    ImGui::SliderFloat("Mouse Sensitivity", &sensitivity, Camera::MIN_SENSITIVITY, Camera::MAX_SENSITIVITY);
  }
}
