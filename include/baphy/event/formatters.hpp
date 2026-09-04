#ifndef BAPHY_EVENT_FORMATTERS_HPP
#define BAPHY_EVENT_FORMATTERS_HPP

#include <fmt/format.h>
#include <string>
#include <string_view>
#include "baphy/event/common.hpp"

namespace baphy {
constexpr std::string_view to_string(const Key key) {
  switch (key) {
  case Key::Space:
    return "Space";
  case Key::Apostrophe:
    return "Apostrophe";
  case Key::Comma:
    return "Comma";
  case Key::Minus:
    return "Minus";
  case Key::Period:
    return "Period";
  case Key::Slash:
    return "Slash";
  case Key::N0:
    return "N0";
  case Key::N1:
    return "N1";
  case Key::N2:
    return "N2";
  case Key::N3:
    return "N3";
  case Key::N4:
    return "N4";
  case Key::N5:
    return "N5";
  case Key::N6:
    return "N6";
  case Key::N7:
    return "N7";
  case Key::N8:
    return "N8";
  case Key::N9:
    return "N9";
  case Key::Semicolon:
    return "Semicolon";
  case Key::Equal:
    return "Equal";
  case Key::A:
    return "A";
  case Key::B:
    return "B";
  case Key::C:
    return "C";
  case Key::D:
    return "D";
  case Key::E:
    return "E";
  case Key::F:
    return "F";
  case Key::G:
    return "G";
  case Key::H:
    return "H";
  case Key::I:
    return "I";
  case Key::J:
    return "J";
  case Key::K:
    return "K";
  case Key::L:
    return "L";
  case Key::M:
    return "M";
  case Key::N:
    return "N";
  case Key::O:
    return "O";
  case Key::P:
    return "P";
  case Key::Q:
    return "Q";
  case Key::R:
    return "R";
  case Key::S:
    return "S";
  case Key::T:
    return "T";
  case Key::U:
    return "U";
  case Key::V:
    return "V";
  case Key::W:
    return "W";
  case Key::X:
    return "X";
  case Key::Y:
    return "Y";
  case Key::Z:
    return "Z";
  case Key::LeftBracket:
    return "LeftBracket";
  case Key::Backslash:
    return "Backslash";
  case Key::RightBracket:
    return "RightBracket";
  case Key::GraveAccent:
    return "GraveAccent";
  case Key::World1:
    return "World1";
  case Key::World2:
    return "World2";
  case Key::Escape:
    return "Escape";
  case Key::Enter:
    return "Enter";
  case Key::Tab:
    return "Tab";
  case Key::Backspace:
    return "Backspace";
  case Key::Insert:
    return "Insert";
  case Key::Delete:
    return "Delete";
  case Key::Right:
    return "Right";
  case Key::Left:
    return "Left";
  case Key::Down:
    return "Down";
  case Key::Up:
    return "Up";
  case Key::PageUp:
    return "PageUp";
  case Key::PageDown:
    return "PageDown";
  case Key::Home:
    return "Home";
  case Key::End:
    return "End";
  case Key::CapsLock:
    return "CapsLock";
  case Key::ScrollLock:
    return "ScrollLock";
  case Key::NumLock:
    return "NumLock";
  case Key::PrintScreen:
    return "PrintScreen";
  case Key::Pause:
    return "Pause";
  case Key::F1:
    return "F1";
  case Key::F2:
    return "F2";
  case Key::F3:
    return "F3";
  case Key::F4:
    return "F4";
  case Key::F5:
    return "F5";
  case Key::F6:
    return "F6";
  case Key::F7:
    return "F7";
  case Key::F8:
    return "F8";
  case Key::F9:
    return "F9";
  case Key::F10:
    return "F10";
  case Key::F11:
    return "F11";
  case Key::F12:
    return "F12";
  case Key::F13:
    return "F13";
  case Key::F14:
    return "F14";
  case Key::F15:
    return "F15";
  case Key::F16:
    return "F16";
  case Key::F17:
    return "F17";
  case Key::F18:
    return "F18";
  case Key::F19:
    return "F19";
  case Key::F20:
    return "F20";
  case Key::F21:
    return "F21";
  case Key::F22:
    return "F22";
  case Key::F23:
    return "F23";
  case Key::F24:
    return "F24";
  case Key::F25:
    return "F25";
  case Key::Kp0:
    return "Kp0";
  case Key::Kp1:
    return "Kp1";
  case Key::Kp2:
    return "Kp2";
  case Key::Kp3:
    return "Kp3";
  case Key::Kp4:
    return "Kp4";
  case Key::Kp5:
    return "Kp5";
  case Key::Kp6:
    return "Kp6";
  case Key::Kp7:
    return "Kp7";
  case Key::Kp8:
    return "Kp8";
  case Key::Kp9:
    return "Kp9";
  case Key::KpDecimal:
    return "KpDecimal";
  case Key::KpDivide:
    return "KpDivide";
  case Key::KpMultiply:
    return "KpMultiply";
  case Key::KpSubtract:
    return "KpSubtract";
  case Key::KpAdd:
    return "KpAdd";
  case Key::KpEnter:
    return "KpEnter";
  case Key::KpEqual:
    return "KpEqual";
  case Key::LeftShift:
    return "LeftShift";
  case Key::LeftControl:
    return "LeftControl";
  case Key::LeftAlt:
    return "LeftAlt";
  case Key::LeftSuper:
    return "LeftSuper";
  case Key::RightShift:
    return "RightShift";
  case Key::RightControl:
    return "RightControl";
  case Key::RightAlt:
    return "RightAlt";
  case Key::RightSuper:
    return "RightSuper";
  case Key::Menu:
    return "Menu";
  }

  return "Unknown";
}

constexpr std::string_view to_string(const Button button) {
  switch (button) {
  case Button::Left:
    return "Left";
  case Button::Right:
    return "Right";
  case Button::Middle:
    return "Middle";
  case Button::N4:
    return "4";
  case Button::N5:
    return "5";
  case Button::N6:
    return "6";
  case Button::N7:
    return "7";
  case Button::N8:
    return "8";
  }

  return "Unknown";
}

constexpr std::string_view to_string(const Action action) {
  switch (action) {
  case Action::Release:
    return "Release";
  case Action::Press:
    return "Press";
  case Action::Repeat:
    return "Repeat";
  }

  return "Unknown";
}

constexpr std::string to_string(const ModFlags mods) {
  if (mods == ModFlags::None)
    return "None";

  std::string result;

  auto append = [&](ModFlags mod, std::string_view name) {
    if ((mods & mod) != ModFlags::None) {
      if (!result.empty())
        result += '|';
      result += name;
    }
  };

  append(ModFlags::Shift, "Shift");
  append(ModFlags::Control, "Control");
  append(ModFlags::Alt, "Alt");
  append(ModFlags::Super, "Super");
  append(ModFlags::Caps, "Caps");
  append(ModFlags::NumLock, "NumLock");

  return result.empty() ? "Unknown" : result;
}
} // namespace baphy

template<>
struct fmt::formatter<baphy::Key> : formatter<std::string_view> {
  template<typename FormatContext>
  auto format(const baphy::Key key, FormatContext &ctx) const {
    return formatter<std::string_view>::format(baphy::to_string(key), ctx);
  }
};

template<>
struct fmt::formatter<baphy::Button> : formatter<std::string_view> {
  template<typename FormatContext>
  auto format(const baphy::Button button, FormatContext &ctx) const {
    return formatter<std::string_view>::format(baphy::to_string(button), ctx);
  }
};

template<>
struct fmt::formatter<baphy::Action> : formatter<std::string_view> {
  template<typename FormatContext>
  auto format(const baphy::Action action, FormatContext &ctx) const {
    return formatter<std::string_view>::format(baphy::to_string(action), ctx);
  }
};

template<>
struct fmt::formatter<baphy::ModFlags> : formatter<std::string_view> {
  template<typename FormatContext>
  auto format(const baphy::ModFlags mods, FormatContext &ctx) const {
    const auto value = baphy::to_string(mods);
    return formatter<std::string_view>::format(value, ctx);
  }
};

#endif // BAPHY_EVENT_FORMATTERS_HPP
