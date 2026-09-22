#ifndef BAPHY_GLH_BUFFER_HPP
#define BAPHY_GLH_BUFFER_HPP

#include <ranges>
#include "baphy/gl.hpp"

namespace baphy::glh {
template<std::ranges::contiguous_range R>
GLuint create_buffer_from_data(const R &data);
}

template<std::ranges::contiguous_range R>
GLuint baphy::glh::create_buffer_from_data(const R &data) {
  GLuint id;

  glCreateBuffers(1, &id);
  glNamedBufferStorage(
      id,
      std::ranges::size(data) * sizeof(std::ranges::range_value_t<R>),
      std::ranges::data(data),
      0);

  return id;
}


#endif // BAPHY_GLH_BUFFER_HPP
