#include "renderer.h"
#include "../util/shader.h"

class DemoRenderer : public Renderer {
  public:
    DemoRenderer();
    ~DemoRenderer() override;
    void render(double dt) override;

  private:
    unsigned int m_vao {};
    unsigned int m_vbo {};
    bool m_dirty { true };
    Shader m_shader { Shader("demo.vert", "demo.frag") };
};
