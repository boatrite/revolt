#include <memory>
#include <vector>

#include "../camera.h"
#include "renderer.h"
#include "demo_renderer.h"

class RootRenderer : public Renderer {
  public:
    RootRenderer();
    void processInput(GLFWwindow* window, float dt) override;
    void update(double dt) override;
    void render(double dt) override;

    void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) override;
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
    void windowSizeCallback(GLFWwindow* window, int width, int height) override;
    void focusCallback(bool focusedInGame) override;

  private:
    std::shared_ptr<Camera> m_cameraPtr { std::make_shared<Camera>() };
    std::vector<std::shared_ptr<Renderer>> m_rendererPtrs {};
    bool m_wireframe { false };
};
