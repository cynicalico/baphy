#ifndef BAPHY_APPLICATION_HPP
#define BAPHY_APPLICATION_HPP

#include <baphy/event/all.hpp>
#include <nexus/nexus.hpp>

namespace baphy {
class Runner;

class Application {
public:
  Runner &runner;

  explicit Application(Runner &runner);

  virtual ~Application();

  Application(const Application &) = delete;
  Application &operator=(const Application &) = delete;

  Application(Application &&other) noexcept = delete;
  Application &operator=(Application &&other) noexcept = delete;

  virtual void update(double dt);

  virtual void draw();

  virtual void keyboard_callback(const KeyboardEvent &e);
  virtual void mouse_button_callback(const MouseButtonEvent &e);
  virtual void mouse_motion_callback(const MouseMotionEvent &e);
  virtual void mouse_wheel_callback(const MouseWheelEvent &e);
  virtual void quit_callback(const QuitEvent &e);

protected:
  nexus::ID callback_id_{};

private:
  void subscribe_callbacks_();
};
} // namespace baphy

#endif // BAPHY_APPLICATION_HPP
