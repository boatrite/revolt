#include "renderer.h"
#include "../util/shader.h"

class RootRenderer : public Renderer {
  public:
    RootRenderer();
    void render(double dt) override;

  private:
    unsigned int m_vao {};
    unsigned int m_vbo {};
    bool m_dirty { true };
    Shader m_shader { Shader("shader.vert", "shader.frag") };
};
