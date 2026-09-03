#ifndef BAPHY_RUNNER_HPP
#define BAPHY_RUNNER_HPP

namespace baphy {
class Runner {
public:
  Runner(const Runner &) = delete;
  Runner &operator=(const Runner &) = delete;

  Runner(Runner &&) = delete;
  Runner &operator=(Runner &&) = delete;

  static Runner &instance();

private:
  Runner();
  ~Runner();
};
} // namespace baphy

#endif // BAPHY_RUNNER_HPP
