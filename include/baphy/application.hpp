#ifndef BAPHY_APPLICATION_HPP
#define BAPHY_APPLICATION_HPP

#include <nexus/nexus.hpp>

namespace baphy {
class Runner;

class Application {
public:
  Runner &runner;

  Application();

  virtual ~Application();

  virtual void update(double dt);

  virtual void draw();

protected:
  nexus::ID callback_id_;
};
} // namespace baphy

#endif // BAPHY_APPLICATION_HPP
