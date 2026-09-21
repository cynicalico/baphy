#ifndef BAPHY_UTIL_IO_HPP
#define BAPHY_UTIL_IO_HPP

#include <filesystem>
#include <optional>
#include <string>

namespace baphy {
std::optional<std::string> slurp(const std::filesystem::path &path);
}

#endif // BAPHY_UTIL_IO_HPP
