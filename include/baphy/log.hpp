#ifndef BAPHY_LOG_HPP
#define BAPHY_LOG_HPP

#include <memory>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/spdlog.h>

namespace baphy {
std::shared_ptr<spdlog::sinks::dist_sink_mt> logger_sinks();
std::shared_ptr<spdlog::logger> logger();

template<typename... T>
void log_info(spdlog::format_string_t<T...> fmt, T &&...args) {
#ifndef NDEBUG
  logger()->info(fmt, std::forward<T &&>(args)...);
#elif BAPHY_LOG_LEVEL == SPDLOG_LEVEL_INFO
#endif
}
} // namespace baphy

#endif // BAPHY_LOG_HPP
