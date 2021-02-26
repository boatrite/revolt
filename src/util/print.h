# pragma once

#include <glm/glm.hpp>

#include <array>
#include <iterator>
#include <ostream>
#include <vector>

// https://stackoverflow.com/a/19152438
  template <class T, std::size_t N>
inline std::ostream& operator<<(std::ostream& o, const std::array<T, N>& arr)
{
  copy(arr.cbegin(), arr.cend(), std::ostream_iterator<T>(o, " "));
  return o;
}

  template <class T>
inline std::ostream& operator<<(std::ostream& o, const std::vector<T>& vec)
{
  copy(vec.cbegin(), vec.cend(), std::ostream_iterator<T>(o, " "));
  return o;
}

inline std::ostream& operator<<(std::ostream& o, const glm::vec3 vec)
{
  o << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
  return o;
}

inline std::ostream& operator<<(std::ostream& o, const glm::vec4 vec)
{
  o << "(" << vec.x << "," << vec.y << "," << vec.z << "," << vec.w << ")";
  return o;
}
