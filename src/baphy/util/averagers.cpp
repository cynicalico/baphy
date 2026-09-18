#include "baphy/util/averagers.hpp"

baphy::CMA::CMA(std::size_t sample_count)
    : value_(0),
      sample_count_(sample_count) {}

void baphy::CMA::update(double v) {
  value_ += (v - value_) / ++sample_count_;
}

double baphy::CMA::value() const {
  return value_;
}

std::size_t baphy::CMA::samples() const {
  return sample_count_;
}

baphy::EMA::EMA(double alpha)
    : alpha(alpha),
      value_(0) {}

void baphy::EMA::update(double v) {
  value_ = alpha * v + (1 - alpha) * value_;
}

double baphy::EMA::value() const {
  return value_;
}

baphy::SMA::SMA(std::size_t sample_count)
    : sample_count(sample_count),
      value_(0) {}

void baphy::SMA::update(double v) {
  samples_.push_back(v);
  if (samples_.size() <= sample_count) {
    value_ += (v - value_) / static_cast<double>(samples_.size());
  } else {
    value_ +=
        1.0 / static_cast<double>(samples_.size()) * (v - samples_.front());
    samples_.pop_back();
  }
}

double baphy::SMA::value() const {
  return value_;
}
