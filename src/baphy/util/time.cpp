#include "baphy/util/time.hpp"

#include <chrono>
#include <fmt/format.h>

std::string baphy::timestamp() {
  const auto now = std::chrono::system_clock::now();
  const auto local_now = std::chrono::current_zone()->to_local(now);
  const auto day_point = std::chrono::floor<std::chrono::days>(local_now);

  const auto ymd = std::chrono::year_month_day{day_point};
  const auto year = static_cast<int>(ymd.year());
  const auto month = static_cast<unsigned>(ymd.month());
  const auto day = static_cast<unsigned>(ymd.day());

  const auto hms = std::chrono::hh_mm_ss{local_now - day_point};
  const auto hour = hms.hours().count();
  const auto minute = hms.minutes().count();
  const auto second = hms.seconds().count();

  return fmt::format(
      "{:04d}-{:02d}-{:02d}_{:02d}-{:02d}-{:02d}",
      year,
      month,
      day,
      hour,
      minute,
      second);
}
