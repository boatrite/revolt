#include <memory>

#include "../camera.h"
#include "../util/shader.h"
#include "renderer.h"

class DemoRenderer : public Renderer {
  public:
    DemoRenderer(std::shared_ptr<Camera> cameraPtr);
    ~DemoRenderer() override;
    void render(double dt) override;

  private:
    std::shared_ptr<Camera> m_cameraPtr;
    unsigned int m_vao {};
    unsigned int m_vbo {};
    Shader m_shader { Shader("demo.vert", "demo.frag") };
};
