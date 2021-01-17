#include <memory>
#include <vector>

#include "../camera.h"
#include "renderer.h"

class RootRenderer : public Renderer {
  private:
    std::shared_ptr<UIContext> m_ui_context_ptr;
    std::shared_ptr<Camera> m_camera_ptr { std::make_shared<Camera>(m_ui_context_ptr) };
    std::vector<std::shared_ptr<Renderer>> m_rendererPtrs {};
    bool m_wireframe { true };

  public:
    RootRenderer(std::shared_ptr<UIContext> ui_context_ptr);
    ~RootRenderer();
    void render(double dt) override;
};
