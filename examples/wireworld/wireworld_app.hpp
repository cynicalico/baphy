#ifndef BAPHY_EXAMPLE_WIREWORLD_WIREWORLD_APP_HPP
#define BAPHY_EXAMPLE_WIREWORLD_WIREWORLD_APP_HPP

#include <baphy/baphy.hpp>
#include "wireworld.hpp"

class WireworldApp final : public baphy::Application {
public:
    explicit WireworldApp(baphy::Runner &runner);

    WireworldApp(const WireworldApp &other) = delete;
    WireworldApp &operator=(const WireworldApp &other) = delete;

    WireworldApp(WireworldApp &&other) noexcept = delete;
    WireworldApp &operator=(WireworldApp &&other) noexcept = delete;

    baphy::Color clear_color() override;

    void update(double dt) override;

    void draw() override;

private:
    baphy::Window &window_;
    baphy::Painter &painter_;
    baphy::InputMgr &input_;

    WireworldRYGB world_{};
    glm::vec2 viewport_pos_{0, 0};
    int zoom_{10};

    bool pen_down_{false};
    bool pen_erasing_{false};
    glm::ivec2 pen_pos_{0, 0};
    WireworldRYGB::State pen_content_{WireworldRYGB::State::RWire};

    bool panning_{false};
    glm::vec2 panning_pos_{0, 0};

    baphy::Ticker<>::duration tick_interval_{std::chrono::milliseconds(100)};
    baphy::Ticker<> tick_timer_;
    bool ticking_{false};

    enum class SelectionState {
        None,
        Selecting,
        Idle,
        Moving,
        Copying
    };
    std::optional<glm::ivec2> sel_start_{std::nullopt};
    std::optional<glm::ivec2> sel_end_{std::nullopt};
    SelectionState sel_state_{SelectionState::None};
    std::optional<glm::ivec2> sel_moving_pos_{std::nullopt};
    ankerl::unordered_dense::map<glm::ivec2, WireworldRYGB::State, IVec2Hash> sel_buf_{};

    void draw_ui_();
    void draw_world_();

    glm::vec2 world_mouse_pos_() const;
    glm::ivec2 cell_mouse_pos_() const;

    void reset_selection_();
    void tick_sel_state_machine_();
    [[nodiscard]] bool is_sel_blocking_input_() const;
    [[nodiscard]] bool is_cell_pos_in_sel_(glm::ivec2 pos) const;
    void fill_sel_buf_();
    [[nodiscard]] std::pair<glm::ivec2, glm::ivec2> sel_rect_() const;
    [[nodiscard]] glm::ivec2 sel_mid_() const;
};

#endif // BAPHY_EXAMPLE_WIREWORLD_WIREWORLD_APP_HPP
