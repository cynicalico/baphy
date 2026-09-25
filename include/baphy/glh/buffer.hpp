#ifndef BAPHY_GLH_BUFFER_HPP
#define BAPHY_GLH_BUFFER_HPP

#include <algorithm>
#include <ranges>
#include <utility>
#include <vector>
#include "baphy/gl.hpp"

namespace baphy::glh {
template<std::ranges::contiguous_range R>
GLuint create_buffer_from_data(const R &data);

enum class FillDirection {
  Forward,
  Reverse,
};

template<typename T>
class VecBuffer {
public:
  GLuint id{0};

  explicit VecBuffer(std::size_t capacity,
                     FillDirection fill_direction = FillDirection::Forward);
  ~VecBuffer();

  VecBuffer(const VecBuffer &) = delete;
  VecBuffer &operator=(const VecBuffer &) = delete;

  VecBuffer(VecBuffer &&other) noexcept;
  VecBuffer &operator=(VecBuffer &&other) noexcept;

  /// The first index of the buffer (see "first" argument in glDrawArrays)
  [[nodiscard]] std::size_t front() const;
  /// The back index of the buffer (back - front is the "count")
  [[nodiscard]] std::size_t back() const;
  /// Shortcut for back - front
  [[nodiscard]] std::size_t size() const;
  /// Total capacity of the buffer
  [[nodiscard]] std::size_t capacity() const;

  /// Clear the buffer
  void clear();

  /// Checks if there is room for count elements to be pushed
  [[nodiscard]] bool can_fit(std::size_t count) const;

  /// Push element into buffer, returns true if successful
  bool push(const T &v);

  /// Push range of elements into buffer, returns true if successful
  template<std::ranges::contiguous_range R>
    requires std::same_as<T, std::ranges::range_value_t<R>>
  bool extend(const R &data);

  /// Synchronize the buffer with the GPU
  void sync();

private:
  std::vector<T> data_;
  std::size_t capacity_;
  FillDirection fill_direction_;
  std::size_t front_{0};
  std::size_t back_{0};

  std::size_t gl_front_{0};
  std::size_t gl_back_{0};
};

template<std::ranges::contiguous_range R>
GLuint create_buffer_from_data(const R &data) {
  GLuint id;

  glCreateBuffers(1, &id);
  glNamedBufferStorage(
      id,
      std::ranges::size(data) * sizeof(std::ranges::range_value_t<R>),
      std::ranges::data(data),
      0);

  return id;
}

template<typename T>
VecBuffer<T>::VecBuffer(std::size_t capacity, FillDirection fill_direction)
    : capacity_(capacity),
      fill_direction_(fill_direction) {
  data_ = std::vector<T>(capacity);

  if (fill_direction_ == FillDirection::Reverse) {
    front_ = capacity_;
    back_ = capacity_;
    gl_front_ = capacity_;
    gl_back_ = capacity_;
  }

  glCreateBuffers(1, &id);
  glNamedBufferStorage(
      id, capacity * sizeof(T), nullptr, GL_DYNAMIC_STORAGE_BIT);
}

template<typename T>
VecBuffer<T>::~VecBuffer() {
  if (id != 0)
    glDeleteBuffers(1, &id);
}

template<typename T>
VecBuffer<T>::VecBuffer(VecBuffer &&other) noexcept
    : id(std::exchange(other.id, 0)),
      data_(std::move(other.data_)),
      capacity_(std::exchange(other.capacity_, 0)),
      fill_direction_(other.fill_direction_),
      front_(std::exchange(other.front_, 0)),
      back_(std::exchange(other.back_, 0)),
      gl_front_(std::exchange(other.gl_front_, 0)),
      gl_back_(std::exchange(other.gl_back_, 0)) {}

template<typename T>
VecBuffer<T> &VecBuffer<T>::operator=(VecBuffer &&other) noexcept {
  if (this == &other)
    return *this;

  if (id != 0)
    glDeleteBuffers(1, &id);

  id = std::exchange(other.id, 0);
  data_ = std::move(other.data_);
  capacity_ = std::exchange(other.capacity_, 0);
  fill_direction_ = other.fill_direction_;
  front_ = std::exchange(other.front_, 0);
  back_ = std::exchange(other.back_, 0);
  gl_front_ = std::exchange(other.gl_front_, 0);
  gl_back_ = std::exchange(other.gl_back_, 0);

  return *this;
}

template<typename T>
std::size_t VecBuffer<T>::front() const {
  return front_;
}

template<typename T>
std::size_t VecBuffer<T>::back() const {
  return back_;
}

template<typename T>
std::size_t VecBuffer<T>::size() const {
  return back_ - front_;
}

template<typename T>
std::size_t VecBuffer<T>::capacity() const {
  return capacity_;
}

template<typename T>
void VecBuffer<T>::clear() {
  if (fill_direction_ == FillDirection::Forward) {
    front_ = 0;
    back_ = 0;
    gl_front_ = 0;
    gl_back_ = 0;
  } else {
    front_ = capacity_;
    back_ = capacity_;
    gl_front_ = capacity_;
    gl_back_ = capacity_;
  }
}

template<typename T>
bool VecBuffer<T>::can_fit(std::size_t count) const {
  return count <= capacity_ - size();
}

template<typename T>
bool VecBuffer<T>::push(const T &v) {
  if (!can_fit(1))
    return false;

  if (fill_direction_ == FillDirection::Forward) {
    data_[back_] = v;
    ++back_;
  } else {
    --front_;
    data_[front_] = v;
  }

  return true;
}

template<typename T>
template<std::ranges::contiguous_range R>
  requires std::same_as<T, std::ranges::range_value_t<R>>
bool VecBuffer<T>::extend(const R &data) {
  const auto count = std::ranges::size(data);
  if (!can_fit(count))
    return false;

  if (fill_direction_ == FillDirection::Forward) {
    std::ranges::copy(data, data_.begin() + static_cast<std::ptrdiff_t>(back_));
    back_ += count;
  } else {
    front_ -= count;
    std::ranges::copy(data,
                      data_.begin() + static_cast<std::ptrdiff_t>(front_));
  }

  return true;
}

template<typename T>
void VecBuffer<T>::sync() {
  if (fill_direction_ == FillDirection::Forward) {
    if (gl_back_ == back_)
      return;

    glNamedBufferSubData(
        id,
        static_cast<GLintptr>(gl_back_ * sizeof(T)),
        static_cast<GLsizeiptr>((back_ - gl_back_) * sizeof(T)),
        data_.data() + gl_back_);

    gl_front_ = front_;
    gl_back_ = back_;
  } else {
    if (gl_front_ == front_)
      return;

    glNamedBufferSubData(
        id,
        static_cast<GLintptr>(front_ * sizeof(T)),
        static_cast<GLsizeiptr>((gl_front_ - front_) * sizeof(T)),
        data_.data() + front_);

    gl_front_ = front_;
    gl_back_ = back_;
  }
}
} // namespace baphy::glh

#endif // BAPHY_GLH_BUFFER_HPP
