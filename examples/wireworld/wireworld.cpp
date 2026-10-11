#include "wireworld.hpp"
#include <utility>

WireworldRYGB::State WireworldRYGB::get(const glm::ivec2 pos) const {
    const auto it = cells_.find(pos);
    if (it == cells_.end())
        return State::Empty;
    return it->second;
}

void WireworldRYGB::set(const glm::ivec2 pos, const State state) {
    if (state == State::Empty)
        cells_.erase(pos);
    else
        cells_[pos] = state;
}

void WireworldRYGB::clear() {
    cells_.clear();
}

void WireworldRYGB::tick() {
    std::unordered_map<glm::ivec2, State> next{};

    for (const auto &[pos, state]: cells_) {
        switch (state) {
        case State::Empty:
            std::unreachable();
            break;
        case State::RWire:
            if (const auto [r_count, y_count, g_count, b_count] = head_counts_(pos);
                ((r_count == 1 || r_count == 2) && y_count == 0 && b_count == 0) ||
                (b_count == 1 && r_count == 0 && y_count == 0 && g_count == 0) ||
                (y_count == 2 && r_count == 0 && g_count == 0 && b_count == 0)) {
                next[pos] = State::RHead;
            } else {
                next[pos] = State::RWire;
            }
            break;
        case State::YWire:
            if (const auto [r_count, y_count, g_count, b_count] = head_counts_(pos);
                ((y_count == 1 || y_count == 2) && r_count == 0 && g_count == 0) ||
                ((r_count == 1 || r_count == 2) && y_count == 0 && g_count == 0 && b_count == 0)) {
                next[pos] = State::YHead;
            } else {
                next[pos] = State::YWire;
            }
            break;
        case State::GWire:
            if (const auto [r_count, y_count, g_count, b_count] = head_counts_(pos);
                ((g_count == 1 || g_count == 2) && y_count == 0 && b_count == 0) ||
                (r_count == 1 && y_count == 0 && g_count == 0 && b_count == 0) ||
                (b_count == 2 && r_count == 0 && y_count == 0 && g_count == 0)) {
                next[pos] = State::GHead;
            } else {
                next[pos] = State::GWire;
            }
            break;
        case State::BWire:
            if (const auto [r_count, y_count, g_count, b_count] = head_counts_(pos);
                ((b_count == 1 || b_count == 2) && r_count == 0 && g_count == 0) ||
                ((g_count == 1 || g_count == 2) && r_count == 0 && y_count == 0 && b_count == 0)) {
                next[pos] = State::BHead;
            } else {
                next[pos] = State::BWire;
            }
            break;
        case State::RHead:
            next[pos] = State::RTail;
            break;
        case State::YHead:
            next[pos] = State::YTail;
            break;
        case State::GHead:
            next[pos] = State::GTail;
            break;
        case State::BHead:
            next[pos] = State::BTail;
            break;
        case State::RTail:
            next[pos] = State::RWire;
            break;
        case State::YTail:
            next[pos] = State::YWire;
            break;
        case State::GTail:
            next[pos] = State::GWire;
            break;
        case State::BTail:
            next[pos] = State::BWire;
            break;
        }
    }

    cells_ = std::move(next);
}

std::tuple<int, int, int, int> WireworldRYGB::head_counts_(glm::ivec2 pos) const {
    int r_count = 0;
    int y_count = 0;
    int g_count = 0;
    int b_count = 0;

    for (int xoff = -1; xoff <= 1; ++xoff) {
        for (int yoff = -1; yoff <= 1; ++yoff) {
            const auto off_pos = pos + glm::ivec2{xoff, yoff};
            if (off_pos == pos)
                continue;

            switch (get(off_pos)) {
            case State::RHead:
                ++r_count;
                break;
            case State::YHead:
                ++y_count;
                break;
            case State::GHead:
                ++g_count;
                break;
            case State::BHead:
                ++b_count;
                break;
            default:
                // do nothing
                break;
            }
        }
    }

    return std::make_tuple(r_count, y_count, g_count, b_count);
}
