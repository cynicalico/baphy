#include "wireworld_app.hpp"
#include <ranges>

static const baphy::RGBA &state_color(const WireworldRYGB::State s) {
    static std::unordered_map<WireworldRYGB::State, baphy::RGBA> m = {
            {WireworldRYGB::State::RWire, baphy::rgb(224, 0, 0)},
            {WireworldRYGB::State::RHead, baphy::rgb(255, 192, 192)},
            {WireworldRYGB::State::RTail, baphy::rgb(255, 128, 128)},
            {WireworldRYGB::State::YWire, baphy::rgb(176, 160, 0)},
            {WireworldRYGB::State::YHead, baphy::rgb(255, 255, 128)},
            {WireworldRYGB::State::YTail, baphy::rgb(240, 240, 0)},
            {WireworldRYGB::State::GWire, baphy::rgb(0, 128, 0)},
            {WireworldRYGB::State::GHead, baphy::rgb(160, 255, 160)},
            {WireworldRYGB::State::GTail, baphy::rgb(96, 192, 96)},
            {WireworldRYGB::State::BWire, baphy::rgb(32, 32, 255)},
            {WireworldRYGB::State::BHead, baphy::rgb(192, 192, 255)},
            {WireworldRYGB::State::BTail, baphy::rgb(128, 128, 255)},
    };
    return m.at(s);
}

static const char *state_name(const WireworldRYGB::State s) {
    static std::unordered_map<WireworldRYGB::State, const char *> m = {
            {WireworldRYGB::State::RWire, "Red wire"},
            {WireworldRYGB::State::RHead, "Red head"},
            {WireworldRYGB::State::RTail, "Red tail"},
            {WireworldRYGB::State::YWire, "Yellow wire"},
            {WireworldRYGB::State::YHead, "Yellow head"},
            {WireworldRYGB::State::YTail, "Yellow tail"},
            {WireworldRYGB::State::GWire, "Green wire"},
            {WireworldRYGB::State::GHead, "Green head"},
            {WireworldRYGB::State::GTail, "Green tail"},
            {WireworldRYGB::State::BWire, "Blue wire"},
            {WireworldRYGB::State::BHead, "Blue head"},
            {WireworldRYGB::State::BTail, "Blue tail"},
    };
    return m.at(s);
}

static std::vector<glm::ivec2> bresenham(glm::ivec2 start, glm::ivec2 end) {
    std::vector<glm::ivec2> points;

    const auto bresenham_low = [&points](const glm::ivec2 p0, const glm::ivec2 p1) {
        const int dx = p1.x - p0.x;
        int dy = p1.y - p0.y;
        int yi = 1;
        if (dy < 0) {
            yi = -1;
            dy = -dy;
        }
        int D = 2 * dy - dx;
        int y = p0.y;

        for (int x = p0.x; x <= p1.x; x++) {
            points.emplace_back(x, y);
            if (D > 0) {
                y += yi;
                D += 2 * (dy - dx);
            } else {
                D += 2 * dy;
            }
        }
    };

    const auto bresenham_high = [&points](const glm::ivec2 p0, const glm::ivec2 p1) {
        int dx = p1.x - p0.x;
        const int dy = p1.y - p0.y;
        int xi = 1;
        if (dx < 0) {
            xi = -1;
            dx = -dx;
        }
        int D = 2 * dx - dy;
        int x = p0.x;

        for (int y = p0.y; y <= p1.y; y++) {
            points.emplace_back(x, y);
            if (D > 0) {
                x += xi;
                D += 2 * (dx - dy);
            } else {
                D += 2 * dx;
            }
        }
    };

    if (std::abs(end.y - start.y) < std::abs(end.x - start.x)) {
        if (start.x > end.x)
            std::swap(start, end);
        bresenham_low(start, end);
    } else {
        if (start.y > end.y)
            std::swap(start, end);
        bresenham_high(start, end);
    }
    return points;
}

WireworldApp::WireworldApp(baphy::Runner &runner)
    : Application(runner),
      window_(*runner.window),
      painter_(*runner.painter),
      input_(*runner.input),
      tick_timer_(tick_interval_) {
    window_.set_swap_interval(baphy::SwapInterval::Adaptive);
}

baphy::Color WireworldApp::clear_color() {
    return baphy::rgb(16, 16, 16);
}

void WireworldApp::update(double) {
    // Panning
    if (input_.button_pressed(baphy::Button::Middle)) {
        panning_ = true;
        panning_pos_ = world_mouse_pos_();
    } else if (input_.button_released(baphy::Button::Middle)) {
        panning_ = false;
    }
    if (panning_)
        viewport_pos_ = panning_pos_ - input_.mouse_pos() / static_cast<float>(zoom_);

    // Zooming
    if (const int wheel = input_.mouse_wheel_int().y; wheel != 0) {
        const glm::vec2 anchor = world_mouse_pos_();
        zoom_ = std::clamp(zoom_ + wheel, 2, 20);
        viewport_pos_ = anchor - input_.mouse_pos() / static_cast<float>(zoom_);
    }

    const bool was_sel_blocking_input = is_sel_blocking_input_(); // delay freeing the input by 1 tick
    tick_sel_state_machine_();

    if (!is_sel_blocking_input_() && !was_sel_blocking_input && !panning_) {
        // Pen
        if (input_.button_pressed(baphy::Button::Left) || input_.button_pressed(baphy::Button::Right)) {
            pen_down_ = true;
            pen_erasing_ = input_.button_pressed(baphy::Button::Right);
            pen_pos_ = cell_mouse_pos_();
            world_.set(pen_pos_,
                       pen_erasing_ ? WireworldRYGB::State::Empty
                                    : pen_content_); // we always draw/erase at least one point
        } else if (input_.button_released(baphy::Button::Left) || input_.button_released(baphy::Button::Right)) {
            pen_down_ = false;
        }
        if (pen_down_) {
            if (const auto curr_cell_pos = cell_mouse_pos_(); curr_cell_pos != pen_pos_) {
                for (const auto &point: bresenham(pen_pos_, curr_cell_pos))
                    world_.set(point, pen_erasing_ ? WireworldRYGB::State::Empty : pen_content_);
                pen_pos_ = curr_cell_pos;
            }
        }
    }

    // Clear the world
    if (input_.key_pressed(baphy::Key::R))
        world_.clear();

    // Sim control
    if (input_.key_pressed(baphy::Key::S)) {
        ticking_ = !ticking_;
        if (ticking_)
            tick_timer_.reset();
    }
    if ((ticking_ && tick_timer_.tick()) || input_.key_pressed(baphy::Key::T))
        world_.tick();
}

void WireworldApp::draw() {
    draw_ui_();
    draw_world_();
}

void WireworldApp::draw_ui_() {
    ImGui::SetNextWindowPos(ImVec2{0, 0}, ImGuiCond_Always);
    if (ImGui::Begin("##interval", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize)) {
        int ms = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(tick_interval_).count());
        ImGui::SetNextItemWidth(100);
        if (ImGui::InputInt("interval (ms)", &ms, 10, 100)) {
            tick_interval_ = std::chrono::milliseconds(std::max(ms, 1));
            tick_timer_.set_interval(tick_interval_);
        }
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2{static_cast<float>(window_.w()), 0}, ImGuiCond_Always, ImVec2{1, 0});
    if (ImGui::Begin("##pen_settings", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize)) {
        static constexpr WireworldRYGB::State palette[3][4] = {
                {WireworldRYGB::State::RWire,
                 WireworldRYGB::State::YWire,
                 WireworldRYGB::State::GWire,
                 WireworldRYGB::State::BWire},
                {WireworldRYGB::State::RHead,
                 WireworldRYGB::State::YHead,
                 WireworldRYGB::State::GHead,
                 WireworldRYGB::State::BHead},
                {WireworldRYGB::State::RTail,
                 WireworldRYGB::State::YTail,
                 WireworldRYGB::State::GTail,
                 WireworldRYGB::State::BTail},
        };
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{3, 3});
        for (const auto &row: palette) {
            for (int i = 0; i < 4; ++i) {
                const WireworldRYGB::State s = row[i];
                if (i > 0)
                    ImGui::SameLine();

                const glm::vec4 c = static_cast<baphy::Color>(state_color(s)).value();
                ImGui::PushID(static_cast<int>(s));
                if (ImGui::ColorButton("##pen",
                                       ImVec4{c.r, c.g, c.b, c.a},
                                       ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder,
                                       ImVec2{12, 12})) {
                    pen_content_ = s;
                }
                ImGui::SetItemTooltip("%s", state_name(s));
                if (pen_content_ == s) {
                    const ImVec2 min = ImGui::GetItemRectMin();
                    const ImVec2 max = ImGui::GetItemRectMax();
                    ImGui::GetWindowDrawList()->AddRect(
                            ImVec2{min.x - 1, min.y - 1}, ImVec2{max.x + 1, max.y + 1}, IM_COL32_WHITE);
                }
                ImGui::PopID();
            }
        }
        ImGui::PopStyleVar();
    }
    ImGui::End();
}

void WireworldApp::draw_world_() {
    const auto zoom_f = static_cast<float>(zoom_);

    const auto view_size = glm::vec2(window_.w(), window_.h()) / zoom_f;
    const glm::ivec2 min = glm::floor(viewport_pos_);
    const glm::ivec2 max = glm::ceil(viewport_pos_ + view_size);

    const bool moving = sel_state_ == SelectionState::Moving;

    for (int y = min.y; y < max.y; ++y) {
        for (int x = min.x; x < max.x; ++x) {
            if (const auto state = world_.get({x, y}); state != WireworldRYGB::State::Empty) {
                const auto color =
                        moving && sel_buf_.contains({x, y}) ? state_color(state).darken(0.5f) : state_color(state);
                painter_.fill_square(glm::round((glm::vec2(x, y) - viewport_pos_) * zoom_f), zoom_f, color);
            }
        }
    }

    const glm::ivec2 offset = moving ? cell_mouse_pos_() - *sel_moving_pos_ : glm::ivec2{0, 0};

    if (moving) {
        for (const auto &[pos, state]: sel_buf_) {
            painter_.fill_square(
                    glm::round((glm::vec2(pos + offset) - viewport_pos_) * zoom_f), zoom_f, state_color(state));
        }
    }

    if (sel_state_ != SelectionState::None) {
        const glm::vec2 sel_min = glm::min(*sel_start_, *sel_end_) + offset;
        const glm::vec2 sel_max = glm::max(*sel_start_, *sel_end_) + 1 + offset;

        const auto top_left = glm::round((sel_min - viewport_pos_) * zoom_f);
        const auto bottom_right = glm::round((sel_max - viewport_pos_) * zoom_f);
        painter_.stroke_rect(top_left, bottom_right - top_left, 1.0, baphy::rgb(255, 255, 255));
    }
}

glm::vec2 WireworldApp::world_mouse_pos_() const {
    return viewport_pos_ + input_.mouse_pos() / static_cast<float>(zoom_);
}

glm::ivec2 WireworldApp::cell_mouse_pos_() const {
    return {glm::floor(world_mouse_pos_())};
}

void WireworldApp::reset_selection_() {
    sel_start_ = std::nullopt;
    sel_end_ = std::nullopt;
    sel_moving_pos_ = std::nullopt;
    sel_buf_.clear();
    sel_state_ = SelectionState::None;
}

void WireworldApp::tick_sel_state_machine_() {
    if (input_.key_pressed(baphy::Key::Escape)) {
        reset_selection_();
        return;
    }

    bool processing = true; // keep processing, we need more passes through the state machine
    do {
        switch (sel_state_) {
        case SelectionState::None:
            processing = false;
            if (input_.button_pressed(baphy::Button::Left) && input_.key_down(baphy::Key::LShift)) {
                sel_start_ = cell_mouse_pos_();
                sel_end_ = sel_start_;
                sel_state_ = SelectionState::Selecting;
            }
            break;
        case SelectionState::Selecting:
            processing = false;
            if (input_.button_released(baphy::Button::Left))
                sel_state_ = SelectionState::Idle;
            else
                sel_end_ = cell_mouse_pos_();
            break;
        case SelectionState::Idle:
            processing = false;
            if (((input_.button_pressed(baphy::Button::Left) && !input_.key_down(baphy::Key::LShift)) ||
                 input_.button_pressed(baphy::Button::Right)) &&
                !is_cell_pos_in_sel_(cell_mouse_pos_())) {
                reset_selection_();
            } else if (input_.button_pressed(baphy::Button::Left) && input_.key_down(baphy::Key::LShift)) {
                reset_selection_();
                processing = true;
            } else if (input_.button_pressed(baphy::Button::Left)) {
                sel_moving_pos_ = cell_mouse_pos_();
                sel_state_ = SelectionState::Moving;
                processing = true;
            }
            break;
        case SelectionState::Moving:
            processing = false;
            fill_sel_buf_();
            if (input_.button_released(baphy::Button::Left)) {
                const glm::ivec2 offset = cell_mouse_pos_() - *sel_moving_pos_;
                for (const auto &pos: sel_buf_ | std::views::keys)
                    world_.set(pos, WireworldRYGB::State::Empty);
                for (const auto &[pos, state]: sel_buf_)
                    world_.set(pos + offset, state);

                *sel_start_ += offset;
                *sel_end_ += offset;
                sel_moving_pos_ = std::nullopt;
                sel_buf_.clear();
                sel_state_ = SelectionState::Idle;
            }
            break;
        case SelectionState::Copying:
            // TODO
            break;
        }
    } while (processing);
}

bool WireworldApp::is_sel_blocking_input_() const {
    return sel_state_ != SelectionState::None;
}

bool WireworldApp::is_cell_pos_in_sel_(const glm::ivec2 pos) const {
    if (sel_state_ == SelectionState::None)
        return false;

    const glm::ivec2 sel_min = glm::min(*sel_start_, *sel_end_);
    const glm::ivec2 sel_max = glm::max(*sel_start_, *sel_end_) + 1;

    return pos.x >= sel_min.x && pos.x < sel_max.x && pos.y >= sel_min.y && pos.y < sel_max.y;
}

void WireworldApp::fill_sel_buf_() {
    sel_buf_.clear();

    const glm::ivec2 sel_min = glm::min(*sel_start_, *sel_end_);
    const glm::ivec2 sel_max = glm::max(*sel_start_, *sel_end_) + 1;

    for (int x = sel_min.x; x < sel_max.x; ++x) {
        for (int y = sel_min.y; y < sel_max.y; ++y) {
            const auto pos = glm::ivec2(x, y);
            if (const auto state = world_.get(pos); state != WireworldRYGB::State::Empty)
                sel_buf_[pos] = state;
        }
    }
}
