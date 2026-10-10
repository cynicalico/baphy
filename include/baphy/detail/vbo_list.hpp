#ifndef BAPHY_DETAIL_VBO_LIST_HPP
#define BAPHY_DETAIL_VBO_LIST_HPP

// TODO: Some kind of checking for unused batches to save on memory after spikes

#include <cassert>
#include <limits>
#include <memory>
#include <span>
#include <vector>
#include "baphy/glh/buffer.hpp"

namespace baphy::detail {
template<typename T>
class VBOList {
public:
    using index_t = GLushort;
    static constexpr GLenum index_gl_type = GL_UNSIGNED_SHORT;
    static constexpr std::size_t max_vertex_capacity = std::size_t{std::numeric_limits<index_t>::max()} + 1;

    using vbo_t = glh::VecBuffer<T>;
    using ebo_t = glh::VecBuffer<index_t>;

    struct Batch {
        vbo_t vertices;
        ebo_t indices;

        Batch(std::size_t vertex_capacity, std::size_t index_capacity, glh::FillDirection fill_direction)
            : vertices(vertex_capacity, fill_direction),
              indices(index_capacity, fill_direction) {}
    };

    using iterator = std::vector<std::unique_ptr<Batch>>::iterator;
    using const_iterator = std::vector<std::unique_ptr<Batch>>::const_iterator;

    VBOList(std::size_t vertex_capacity, std::size_t index_capacity, glh::FillDirection fill_direction);

    void clear();

    void advance();

    [[nodiscard]] GLuint vbo_id() const { return curr_().vertices.id; }
    [[nodiscard]] GLuint ebo_id() const { return curr_().indices.id; }
    [[nodiscard]] std::size_t index_front() const { return curr_().indices.front(); }
    [[nodiscard]] std::size_t index_back() const { return curr_().indices.back(); }

    [[nodiscard]] bool can_fit(std::size_t vertex_count, std::size_t index_count) const;

    // indices are relative to the first of the given vertices
    bool extend(std::span<const T> vertices, std::span<const index_t> indices);

    void sync();

    iterator begin() { return batches_.begin(); }
    iterator end() { return batches_.begin() + static_cast<std::ptrdiff_t>(curr_idx_ + 1); }

    const_iterator begin() const { return batches_.begin(); }
    const_iterator end() const { return batches_.begin() + static_cast<std::ptrdiff_t>(curr_idx_ + 1); }

private:
    std::vector<std::unique_ptr<Batch>> batches_{};
    std::size_t curr_idx_{0};

    std::size_t vertex_capacity_;
    std::size_t index_capacity_;
    glh::FillDirection fill_direction_;

    std::vector<index_t> index_scratch_{};

    Batch &curr_();
    const Batch &curr_() const;
};

template<typename T>
VBOList<T>::VBOList(const std::size_t vertex_capacity,
                    const std::size_t index_capacity,
                    const glh::FillDirection fill_direction)
    : vertex_capacity_(vertex_capacity),
      index_capacity_(index_capacity),
      fill_direction_(fill_direction) {
    assert(vertex_capacity_ <= max_vertex_capacity);
    batches_.emplace_back(std::make_unique<Batch>(vertex_capacity_, index_capacity_, fill_direction_));
}

template<typename T>
void VBOList<T>::advance() {
    curr_idx_++;
    if (curr_idx_ >= batches_.size())
        batches_.emplace_back(std::make_unique<Batch>(vertex_capacity_, index_capacity_, fill_direction_));
}

template<typename T>
void VBOList<T>::clear() {
    for (auto &batch: batches_) {
        batch->vertices.clear();
        batch->indices.clear();
    }
    curr_idx_ = 0;
}

template<typename T>
bool VBOList<T>::can_fit(const std::size_t vertex_count, const std::size_t index_count) const {
    return curr_().vertices.can_fit(vertex_count) && curr_().indices.can_fit(index_count);
}

template<typename T>
bool VBOList<T>::extend(std::span<const T> vertices, std::span<const index_t> indices) {
    auto &batch = curr_();
    if (!can_fit(vertices.size(), indices.size()))
        return false;

    const auto base = fill_direction_ == glh::FillDirection::Forward ? batch.vertices.back()
                                                                     : batch.vertices.front() - vertices.size();

    index_scratch_.clear();
    for (const auto i: indices)
        index_scratch_.push_back(static_cast<index_t>(base + i));

    batch.vertices.extend(vertices);
    batch.indices.extend(index_scratch_);

    return true;
}

template<typename T>
void VBOList<T>::sync() {
    for (const auto &batch: batches_) {
        batch->vertices.sync();
        batch->indices.sync();
    }
}

template<typename T>
VBOList<T>::Batch &VBOList<T>::curr_() {
    return *batches_[curr_idx_];
}

template<typename T>
const VBOList<T>::Batch &VBOList<T>::curr_() const {
    return *batches_[curr_idx_];
}
} // namespace baphy::detail

#endif // BAPHY_DETAIL_VBO_LIST_HPP
