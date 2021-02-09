#pragma once

// See
// https://github.com/simongeilfus/Cinder-Cereal/blob/master/include/CinderCereal.h
// for more examples of serializing glm classes.

#include <cereal/cereal.hpp>

#include <glm/glm.hpp>

namespace cereal {
  template<class Archive>
  void serialize(Archive& archive, glm::vec3& vec) { archive(cereal::make_nvp("x", vec.x), cereal::make_nvp("y", vec.y), cereal::make_nvp("z", vec.z)); }
  template<class Archive>
  void serialize(Archive& archive, glm::vec4& vec) { archive(cereal::make_nvp("x", vec.x), cereal::make_nvp("y", vec.y), cereal::make_nvp("z", vec.z), cereal::make_nvp("w", vec.w)); }
}
