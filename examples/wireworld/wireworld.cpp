#include "wireworld.hpp"
#include <utility>

static bool is_wire(const WireworldRYGB::State s) {
    switch (s) {
    case WireworldRYGB::State::RWire:
    case WireworldRYGB::State::YWire:
    case WireworldRYGB::State::GWire:
    case WireworldRYGB::State::BWire:
        return true;
    default:
        return false;
    }
}

static bool is_head(const WireworldRYGB::State s) {
    switch (s) {
    case WireworldRYGB::State::RHead:
    case WireworldRYGB::State::YHead:
    case WireworldRYGB::State::GHead:
    case WireworldRYGB::State::BHead:
        return true;
    default:
        return false;
    }
}

static bool is_tail(const WireworldRYGB::State s) {
    switch (s) {
    case WireworldRYGB::State::RTail:
    case WireworldRYGB::State::YTail:
    case WireworldRYGB::State::GTail:
    case WireworldRYGB::State::BTail:
        return true;
    default:
        return false;
    }
}

static int color_of(const WireworldRYGB::State s) {
    switch (s) {
    case WireworldRYGB::State::RWire:
    case WireworldRYGB::State::RHead:
    case WireworldRYGB::State::RTail:
        return 0;
    case WireworldRYGB::State::YWire:
    case WireworldRYGB::State::YHead:
    case WireworldRYGB::State::YTail:
        return 1;
    case WireworldRYGB::State::GWire:
    case WireworldRYGB::State::GHead:
    case WireworldRYGB::State::GTail:
        return 2;
    case WireworldRYGB::State::BWire:
    case WireworldRYGB::State::BHead:
    case WireworldRYGB::State::BTail:
        return 3;
    default:
        std::unreachable();
    }
}

static WireworldRYGB::State advance(const WireworldRYGB::State s) {
    if (is_tail(s))
        return static_cast<WireworldRYGB::State>(static_cast<int>(s) - 8); // tail -> wire
    return static_cast<WireworldRYGB::State>(static_cast<int>(s) + 4); // wire -> head or head -> tail
}

WireworldRYGB::State WireworldRYGB::get(const glm::ivec2 pos) const {
    const auto it = cells_.find(pos);
    if (it == cells_.end())
        return State::Empty;
    return it->second;
}

void WireworldRYGB::set(const glm::ivec2 pos, const State state) {
    if (const State old = get(pos); is_head(old) || is_tail(old) || is_head(state) || is_tail(state))
        dirty_ = true;

    if (state == State::Empty)
        cells_.erase(pos);
    else
        cells_[pos] = state;
}

void WireworldRYGB::clear() {
    cells_.clear();
    heads_.clear();
    tails_.clear();
    dirty_ = false;
}

void WireworldRYGB::tick() {
    if (dirty_)
        rebuild_active_();

    head_counts_.clear();
    for (const auto &pos: heads_) {
        const int color = color_of(get(pos));

        for (int xoff = -1; xoff <= 1; ++xoff) {
            for (int yoff = -1; yoff <= 1; ++yoff) {
                const auto off_pos = pos + glm::ivec2{xoff, yoff};
                if (off_pos == pos)
                    continue;

                if (is_wire(get(off_pos)))
                    ++head_counts_[off_pos][color];
            }
        }
    }

    next_heads_.clear();
    for (const auto &[pos, counts]: head_counts_)
        if (should_become_head_(get(pos), counts))
            next_heads_.push_back(pos);

    for (const auto &pos: tails_)
        cells_[pos] = advance(cells_[pos]);
    for (const auto &pos: heads_)
        cells_[pos] = advance(cells_[pos]);
    for (const auto &pos: next_heads_)
        cells_[pos] = advance(cells_[pos]);

    std::swap(tails_, heads_);
    std::swap(heads_, next_heads_);
}

void WireworldRYGB::rebuild_active_() {
    heads_.clear();
    tails_.clear();

    for (const auto &[pos, state]: cells_)
        if (is_head(state))
            heads_.push_back(pos);
        else if (is_tail(state))
            tails_.push_back(pos);

    dirty_ = false;
}

bool WireworldRYGB::should_become_head_(const State wire, const std::array<int, 4> &counts) {
    const auto [r_count, y_count, g_count, b_count] = counts;

    switch (wire) {
    case State::RWire:
        return ((r_count == 1 || r_count == 2) && y_count == 0 && b_count == 0) ||
               (b_count == 1 && r_count == 0 && y_count == 0 && g_count == 0) ||
               (y_count == 2 && r_count == 0 && g_count == 0 && b_count == 0);
    case State::YWire:
        return ((y_count == 1 || y_count == 2) && r_count == 0 && g_count == 0) ||
               ((r_count == 1 || r_count == 2) && y_count == 0 && g_count == 0 && b_count == 0);
    case State::GWire:
        return ((g_count == 1 || g_count == 2) && y_count == 0 && b_count == 0) ||
               (y_count == 1 && r_count == 0 && g_count == 0 && b_count == 0) ||
               (b_count == 2 && r_count == 0 && y_count == 0 && g_count == 0);
    case State::BWire:
        return ((b_count == 1 || b_count == 2) && r_count == 0 && g_count == 0) ||
               ((g_count == 1 || g_count == 2) && r_count == 0 && y_count == 0 && b_count == 0);
    default:
        std::unreachable();
    }
}
