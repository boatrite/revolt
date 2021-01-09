#include <memory>
#include <vector>

#include "renderer.h"
#include "demo_renderer.h"

class RootRenderer : public Renderer {
  public:
    RootRenderer();
    void render(double dt) override;

  private:
    std::vector<std::shared_ptr<Renderer>> m_renderers {};
    bool m_wireframe { false };
};
