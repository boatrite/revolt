#include "../camera.h"
#include "../util/shader.h"
#include "renderer.h"

#include <memory>

class ChunkRenderer : public Renderer {
  public:
    ChunkRenderer(std::shared_ptr<UIContext> m_ui_context_ptr, std::shared_ptr<Camera> camera_ptr);
    ~ChunkRenderer() override {};
    void render(double dt) override;

  private:
    std::shared_ptr<UIContext> m_ui_context_ptr;
    std::shared_ptr<Camera> m_camera_ptr;

    Shader m_shader { Shader("chunk.vert", "chunk.frag") };
};
