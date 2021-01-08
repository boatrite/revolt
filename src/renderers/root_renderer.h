#include "renderer.h"

class RootRenderer : public Renderer {
  public:
    RootRenderer();
    void render(double dt) override;
};
