#pragma once

struct Position {
  float x {}, y {};

  template <typename Archive>
  void serialize(Archive& archive) {
    archive(x, y);
  }
};
