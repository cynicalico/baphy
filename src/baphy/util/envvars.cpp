#include "baphy/util/envvars.hpp"
#include "baphy/util/platform.hpp"

#if defined(BAPHY_PLATFORM_WINDOWS)
#include <windows.h>
#else
#include <cstdlib>
#endif

#if !defined(BAPHY_PLATFORM_WINDOWS)
extern char **environ;
#endif

std::string baphy::envvar_get(std::string_view name) {
  const auto name_str = std::string{name};

#if defined(BAPHY_PLATFORM_WINDOWS)
  const auto size = GetEnvironmentVariableA(name_str.c_str(), nullptr, 0);
  if (size == 0)
    return {};

  auto value = std::string(size, '\0');
  const auto written =
      GetEnvironmentVariableA(name_str.c_str(), value.data(), size);
  if (written == 0)
    return {};

  value.resize(written);
#else
  const auto *value = std::getenv(name_str.c_str());
  if (value == nullptr)
    return {};
#endif

  return value;
}

void baphy::envvar_set(std::string_view name, std::string_view value) {
  const auto name_str = std::string{name};
  const auto value_str = std::string{value};

#if defined(BAPHY_PLATFORM_WINDOWS)
  SetEnvironmentVariableA(name_str.c_str(), value_str.c_str());
#else
  setenv(name_str.c_str(), value_str.c_str(), 1);
#endif
}

void baphy::envvar_unset(std::string_view name) {
  const auto name_str = std::string{name};

#if defined(BAPHY_PLATFORM_WINDOWS)
  SetEnvironmentVariableA(name_str.c_str(), nullptr);
#else
  unsetenv(name_str.c_str());
#endif
}

std::vector<std::string> baphy::envvar_get_all() {
  auto envvars = std::vector<std::string>();

#if defined(BAPHY_PLATFORM_WINDOWS)
  const auto env = GetEnvironmentStringsA();
  if (env == nullptr)
    return envvars;

  for (auto entry = env; *entry != '\0';
       entry += std::string_view(entry).size() + 1) {
    envvars.emplace_back(entry);
  }

  FreeEnvironmentStringsA(env);
#else
  if (environ == nullptr)
    return envvars;

  for (auto entry = environ; *entry != nullptr; ++entry)
    envvars.emplace_back(*entry);
#endif

  return envvars;
}
