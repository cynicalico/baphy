#ifndef BAPHY_APPLICATION_HPP
#define BAPHY_APPLICATION_HPP

#include <baphy/event/common.hpp>
#include <nexus/nexus.hpp>

namespace baphy {
class Runner;

class Application {
public:
  Runner &runner;

  Application(Runner &runner);

  virtual ~Application();

  Application(const Application &) = delete;
  Application &operator=(const Application &) = delete;

  Application(Application &&other) noexcept = delete;
  Application &operator=(Application &&other) noexcept = delete;

  virtual void update(double dt);

  virtual void draw();

  virtual void char_callback(unsigned int codepoint);

  virtual void cursor_enter_callback(bool entered);

  virtual void cursor_pos_callback(double x, double y);

  virtual void drop_callback(const std::vector<std::string> &paths);

  virtual void
  mouse_button_callback(Button button, Action action, ModFlags mods);

  virtual void key_callback(Key key, Action action, ModFlags mods);

  virtual void scroll_callback(double dx, double dy);

protected:
  nexus::ID callback_id_{};

  void subscribe_callbacks_();
};
} // namespace baphy

#endif // BAPHY_APPLICATION_HPP
