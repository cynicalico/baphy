#ifndef BAPHY_UTIL_TIME_HPP
#define BAPHY_UTIL_TIME_HPP

#include <algorithm>
#include <chrono>
#include <deque>
#include <functional>
#include <ranges>
#include <string>
#include "baphy/util/averagers.hpp"

namespace baphy {
std::string timestamp();

template<typename Clock = std::chrono::steady_clock>
class Ticker {
public:
  using time_point = std::chrono::time_point<Clock>;
  using duration = Clock::duration;

  explicit Ticker(duration interval = duration::zero());

  void reset();

  std::uint64_t tick();

  duration dt() const { return dt_; }

  duration elapsed() const { return last_ - start_; }

private:
  time_point start_;
  time_point last_;
  duration interval_;
  duration dt_;
  duration acc_;
};

template<typename Clock>
Ticker<Clock>::Ticker(duration interval)
    : start_(Clock::now()),
      last_(start_),
      interval_(interval),
      dt_(0),
      acc_(0) {}

template<typename Clock>
void Ticker<Clock>::reset() {
  start_ = Clock::now();
  last_ = start_;
  dt_ = duration::zero();
  acc_ = duration::zero();
}

template<typename Clock>
std::uint64_t Ticker<Clock>::tick() {
  const auto now = Clock::now();
  dt_ = now - last_;
  last_ = now;

  std::uint64_t tick_count = 0;
  if (interval_ != duration::zero()) {
    acc_ += dt_;
    while (acc_ >= interval_) {
      acc_ -= interval_;
      tick_count++;
    }
  }

  return tick_count;
}

template<typename Clock = std::chrono::steady_clock>
class FrameCounter {
public:
  using time_point = Ticker<Clock>::time_point;
  using duration = Ticker<Clock>::duration;

  FrameCounter();
  explicit FrameCounter(duration d);

  void reset();

  void update();

  [[nodiscard]] double fps() const;

  [[nodiscard]] duration dt() const;
  [[nodiscard]] std::vector<double> dts() const;

private:
  std::deque<time_point> timestamps_{};
  Ticker<Clock> ticker_;
  EMA averager_;
};

template<typename Clock>
FrameCounter<Clock>::FrameCounter()
    : FrameCounter(std::chrono::seconds(1)) {}

template<typename Clock>
FrameCounter<Clock>::FrameCounter(duration d)
    : ticker_(d),
      averager_(1.0) {}

template<typename Clock>
void FrameCounter<Clock>::reset() {
  timestamps_.clear();
  ticker_.reset();
  averager_ = EMA(1.0);
}

template<typename Clock>
void FrameCounter<Clock>::update() {
  timestamps_.push_back(Clock::now());

  while (timestamps_.size() > 2 &&
         timestamps_.back() - timestamps_.front() > std::chrono::seconds(1)) {
    timestamps_.pop_front();
  }

  averager_.update(static_cast<double>(timestamps_.size()));
  if (ticker_.tick() > 0)
    averager_.alpha = 2.0 / (1.0 + static_cast<double>(timestamps_.size()));
}

template<typename Clock>
double FrameCounter<Clock>::fps() const {
  return averager_.value();
}

template<typename Clock>
FrameCounter<Clock>::duration FrameCounter<Clock>::dt() const {
  if (timestamps_.size() < 2)
    return duration::zero();
  return timestamps_.back() - timestamps_[timestamps_.size() - 2];
}

template<typename T>
double as_secs_dt(T d) {
  return static_cast<double>(
      std::chrono::duration_cast<std::chrono::nanoseconds>(d).count() / 1e9);
}

template<typename Clock>
std::vector<double> FrameCounter<Clock>::dts() const {
  std::vector<double> ret{};
  if (timestamps_.size() >= 2)
    for (const auto &[t0, t1]: timestamps_ | std::views::adjacent<2>)
      ret.push_back(as_secs_dt(t1 - t0));
  return ret;
}

template<typename T, typename Clock = std::chrono::steady_clock>
class MaxPeriod {
public:
  using time_point = Ticker<Clock>::time_point;
  using duration = Ticker<Clock>::duration;

  explicit MaxPeriod(duration period);

  void update(T v);

  T value() const;

private:
  std::deque<std::tuple<time_point, T>> samples_{};
  std::unordered_map<T, std::size_t> frequency_{};
  duration period_;

  void inc_frequency_(T &v);
  void dec_frequency_(T &v);
};

template<typename T, typename Clock>
MaxPeriod<T, Clock>::MaxPeriod(duration period)
    : period_(period) {}

template<typename T, typename Clock>
void MaxPeriod<T, Clock>::update(T v) {
  auto now = Clock::now();
  samples_.emplace_back(now, v);
  inc_frequency_(v);

  while (!samples_.empty() && now - std::get<0>(samples_.front()) > period_) {
    dec_frequency_(std::get<1>(samples_.front()));
    samples_.pop_front();
  }
}

template<typename T, typename Clock>
T MaxPeriod<T, Clock>::value() const {
  return std::ranges::max_element(frequency_,
                                  [](const auto &a, const auto &b) {
                                    return std::get<0>(a) < std::get<0>(b);
                                  })
      ->first;
}

template<typename T, typename Clock>
void MaxPeriod<T, Clock>::inc_frequency_(T &v) {
  auto it = frequency_.find(v);
  if (it == frequency_.end())
    it = frequency_.emplace_hint(it, v, 0);
  ++it->second;
}

template<typename T, typename Clock>
void MaxPeriod<T, Clock>::dec_frequency_(T &v) {
  auto it = frequency_.find(v);
  assert(it != frequency_.end());
  if (--it->second == 0)
    frequency_.erase(it);
}
} // namespace baphy

#endif // BAPHY_UTIL_TIME_HPP
