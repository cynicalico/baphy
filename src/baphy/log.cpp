#include "baphy/log.hpp"

#include <filesystem>
#include <functional>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "baphy/util/envvars.hpp"
#include "baphy/util/time.hpp"

std::shared_ptr<spdlog::sinks::dist_sink_mt> baphy::logger_sinks() {
  static auto logger_sinks = std::make_shared<spdlog::sinks::dist_sink_mt>();

  return logger_sinks;
}

std::shared_ptr<spdlog::logger> baphy::logger() {
  static auto logger = std::invoke([] {
    const auto sinks = logger_sinks();
    sinks->add_sink(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

    const auto log_path = std::filesystem::current_path() / ".log" /
                          fmt::format("{}.log", timestamp());
    sinks->add_sink(
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_path, true));

    const auto l = std::make_shared<spdlog::logger>("baphy", logger_sinks());

#ifndef NDEBUG
    if (const auto log_level = envvar_get("BAPHY_LOG_LEVEL");
        !log_level.empty())
      l->set_level(spdlog::level::from_str(log_level));
    else
      l->set_level(spdlog::level::info);
#else
    l->set_level(spdlog::level::trace);
#endif

    return l;
  });

  return logger;
}
