#ifndef BAPHY_EXAMPLE_WIREWORLD_WIREWORLD_HPP
#define BAPHY_EXAMPLE_WIREWORLD_WIREWORLD_HPP

// https://lodev.org/ca/wireworldrgb.html

#include <array>
#include <glm/glm.hpp>
#include <vector>
#include "ankerl/unordered_dense.h"

struct IVec2Hash {
    using is_avalanching = void;

    std::uint64_t operator()(glm::ivec2 v) const noexcept {
        const auto packed =
                (static_cast<std::uint64_t>(static_cast<std::uint32_t>(v.x)) << 32) | static_cast<std::uint32_t>(v.y);
        return ankerl::unordered_dense::hash<std::uint64_t>{}(packed);
    }
};

class WireworldRYGB {
public:
    enum class State {
        Empty,
        RWire,
        YWire,
        GWire,
        BWire,
        RHead,
        YHead,
        GHead,
        BHead,
        RTail,
        YTail,
        GTail,
        BTail,
    };

    void clear();

    State get(glm::ivec2 pos) const;
    void set(glm::ivec2 pos, State state);

    void tick();

private:
    ankerl::unordered_dense::map<glm::ivec2, State, IVec2Hash> cells_;

    std::vector<glm::ivec2> heads_;
    std::vector<glm::ivec2> tails_;
    bool dirty_{false};

    std::vector<glm::ivec2> next_heads_;
    ankerl::unordered_dense::map<glm::ivec2, std::array<int, 4>, IVec2Hash> head_counts_;

    void rebuild_active_();

    static bool should_become_head_(State wire, const std::array<int, 4> &counts);
};

#endif // BAPHY_EXAMPLE_WIREWORLD_WIREWORLD_HPP
