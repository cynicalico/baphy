#include "baphy/util/io.hpp"
#include <fstream>
#include "baphy/log.hpp"

std::string normalize_line_endings_to_lf(std::string s);

std::optional<std::string> baphy::slurp(const std::filesystem::path &path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file) {
    BAPHY_LOG_ERROR(
        "Failed to open file '{}': {}", path.string(), strerror(errno));
    return std::nullopt;
  }

  const auto size = file.tellg();
  if (size < 0) {
    BAPHY_LOG_ERROR(
        "Failed to get file size '{}': {}", path.string(), strerror(errno));
    return std::nullopt;
  }

  std::string res(size, '\0');

  file.seekg(0, std::ios::beg);
  if (!file) {
    BAPHY_LOG_ERROR("Failed to seek to beginning of file '{}': {}",
                    path.string(),
                    strerror(errno));
    return std::nullopt;
  }

  if (!res.empty()) {
    file.read(res.data(), size);
    if (!file) {
      BAPHY_LOG_ERROR(
          "Failed to read file '{}': {}", path.string(), strerror(errno));
      return std::nullopt;
    }
  }

  return res;
}

std::string normalize_line_endings_to_lf(std::string s) {
  std::string normalized;
  normalized.reserve(s.size());

  for (std::size_t i = 0; i < s.size(); ++i) {
    if (s[i] == '\r') {
      if (i + 1 < s.size() && s[i + 1] == '\n')
        ++i;
      normalized.push_back('\n');
    } else {
      normalized.push_back(s[i]);
    }
  }

  return normalized;
}
