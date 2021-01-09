#include <memory>

#include "../camera.h"
#include "renderer.h"

class OverlayRenderer : public Renderer {
  public:
    OverlayRenderer(std::shared_ptr<Camera> cameraPtr);
    void render(double dt) override;

  private:
    std::shared_ptr<Camera> m_cameraPtr;
    bool m_show_overlay { true };
};
