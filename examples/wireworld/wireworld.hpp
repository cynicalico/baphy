#ifndef BAPHY_EXAMPLE_WIREWORLD_WIREWORLD_HPP
#define BAPHY_EXAMPLE_WIREWORLD_WIREWORLD_HPP

#include <glm/glm.hpp>
#include <tuple>
#include <unordered_map>

template<>
struct std::hash<glm::ivec2> {
    std::size_t operator()(const glm::ivec2 &v) const noexcept {
        return std::hash<int>{}(v.x) ^ (std::hash<int>{}(v.y) << 1);
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
    std::unordered_map<glm::ivec2, State> cells_;

    std::tuple<int, int, int, int> head_counts_(glm::ivec2 pos) const;
};

#endif // BAPHY_EXAMPLE_WIREWORLD_WIREWORLD_HPP
