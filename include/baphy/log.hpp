#ifndef BAPHY_LOG_HPP
#define BAPHY_LOG_HPP

#include <memory>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/spdlog.h>

namespace baphy {
std::shared_ptr<spdlog::sinks::dist_sink_mt> logger_sinks();
std::shared_ptr<spdlog::logger> logger();
} // namespace baphy

#ifndef NDEBUG
#define BAPHY_LOG_TRACE(fmt, ...)                                              \
  baphy::logger()->trace(fmt __VA_OPT__(, ) __VA_ARGS__)
#define BAPHY_LOG_DEBUG(fmt, ...)                                              \
  baphy::logger()->debug(fmt __VA_OPT__(, ) __VA_ARGS__)
#define BAPHY_LOG_INFO(fmt, ...)                                               \
  baphy::logger()->info(fmt __VA_OPT__(, ) __VA_ARGS__)
#define BAPHY_LOG_WARN(fmt, ...)                                               \
  baphy::logger()->warn(fmt __VA_OPT__(, ) __VA_ARGS__)
#define BAPHY_LOG_ERROR(fmt, ...)                                              \
  baphy::logger()->error(fmt __VA_OPT__(, ) __VA_ARGS__)
#define BAPHY_LOG_CRITICAL(fmt, ...)                                           \
  baphy::logger()->critical(fmt __VA_OPT__(, ) __VA_ARGS__)
#else
#define BAPHY_LOG_TRACE(fmt, ...)                                              \
  SPDLOG_LOGGER_TRACE(baphy::logger(), fmt __VA_OPT__(, ) __VA_ARGS__)
#define BAPHY_LOG_DEBUG(fmt, ...)                                              \
  SPDLOG_LOGGER_DEBUG(baphy::logger(), fmt __VA_OPT__(, ) __VA_ARGS__)
#define BAPHY_LOG_INFO(fmt, ...)                                               \
  SPDLOG_LOGGER_INFO(baphy::logger(), fmt __VA_OPT__(, ) __VA_ARGS__)
#define BAPHY_LOG_WARN(fmt, ...)                                               \
  SPDLOG_LOGGER_WARN(baphy::logger(), fmt __VA_OPT__(, ) __VA_ARGS__)
#define BAPHY_LOG_ERROR(fmt, ...)                                              \
  SPDLOG_LOGGER_ERROR(baphy::logger(), fmt __VA_OPT__(, ) __VA_ARGS__)
#define BAPHY_LOG_CRITICAL(fmt, ...)                                           \
  SPDLOG_LOGGER_CRITICAL(baphy::logger(), fmt __VA_OPT__(, ) __VA_ARGS__)
#endif

#endif // BAPHY_LOG_HPP
