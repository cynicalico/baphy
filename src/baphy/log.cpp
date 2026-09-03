#include "baphy/log.hpp"

#include <functional>
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::sinks::dist_sink_mt> baphy::logger_sinks() {
  static auto logger_sinks = std::make_shared<spdlog::sinks::dist_sink_mt>();

  return logger_sinks;
}

std::shared_ptr<spdlog::logger> baphy::logger() {
  static auto logger = std::invoke([]() {
    auto sinks = logger_sinks();
    sinks->add_sink(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    return std::make_shared<spdlog::logger>("baphy", logger_sinks());
  });

  return logger;
}
