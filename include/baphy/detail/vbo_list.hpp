#ifndef BAPHY_DETAIL_VBO_LIST_HPP
#define BAPHY_DETAIL_VBO_LIST_HPP

// TODO: Some kind of checking for unused batches to save on memory after spikes

#include <memory>
#include <vector>
#include "baphy/glh/buffer.hpp"

namespace baphy::detail {
template<typename T>
class VBOList {
public:
    using vbo_t = glh::VecBuffer<T>;
    using iterator = std::vector<std::unique_ptr<vbo_t>>::iterator;
    using const_iterator = std::vector<std::unique_ptr<vbo_t>>::const_iterator;

    VBOList(std::size_t vbo_size, glh::FillDirection fill_direction);

    void clear();

    void advance();

    [[nodiscard]] GLuint id() const { return curr_().id; }
    [[nodiscard]] std::size_t front() const { return curr_().front(); }
    [[nodiscard]] std::size_t back() const { return curr_().back(); }
    [[nodiscard]] std::size_t size() const { return curr_().size(); }
    [[nodiscard]] std::size_t capacity() const { return curr_().capacity(); }

    [[nodiscard]] bool can_fit(std::size_t count) const { return curr_().can_fit(count); }

    bool push(const T &v) { return curr_().push(v); }

    template<std::ranges::contiguous_range R>
        requires std::same_as<T, std::ranges::range_value_t<R>>
    bool extend(const R &data) {
        return curr_().extend(data);
    }

    void sync();

    iterator begin() { return vbos_.begin(); }
    iterator end() { return vbos_.begin() + static_cast<std::ptrdiff_t>(curr_idx_ + 1); }

    const_iterator begin() const { return vbos_.begin(); }
    const_iterator end() const { return vbos_.begin() + static_cast<std::ptrdiff_t>(curr_idx_ + 1); }

private:
    std::vector<std::unique_ptr<vbo_t>> vbos_{};
    std::size_t curr_idx_{0};

    std::size_t vbo_size_;
    glh::FillDirection fill_direction_;

    vbo_t &curr_();
    const vbo_t &curr_() const;
};

template<typename T>
VBOList<T>::VBOList(const std::size_t vbo_size, const glh::FillDirection fill_direction)
    : vbo_size_(vbo_size),
      fill_direction_(fill_direction) {
    vbos_.emplace_back(std::make_unique<vbo_t>(vbo_size_, fill_direction_));
}

template<typename T>
void VBOList<T>::advance() {
    curr_idx_++;
    if (curr_idx_ >= vbos_.size())
        vbos_.emplace_back(std::make_unique<vbo_t>(vbo_size_, fill_direction_));
}

template<typename T>
void VBOList<T>::clear() {
    for (auto &vbo: vbos_)
        vbo->clear();
    curr_idx_ = 0;
}

template<typename T>
void VBOList<T>::sync() {
    for (const auto &vbo: vbos_)
        vbo->sync();
}

template<typename T>
VBOList<T>::vbo_t &VBOList<T>::curr_() {
    return *vbos_[curr_idx_];
}

template<typename T>
const VBOList<T>::vbo_t &VBOList<T>::curr_() const {
    return *vbos_[curr_idx_];
}
} // namespace baphy::detail

#endif // BAPHY_DETAIL_VBO_LIST_HPP
