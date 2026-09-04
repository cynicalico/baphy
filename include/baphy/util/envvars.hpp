#ifndef BAPHY_UTIL_ENVVARS_HPP
#define BAPHY_UTIL_ENVVARS_HPP

#include <string>
#include <vector>

namespace baphy {
std::string envvar_get(std::string_view name);
void envvar_set(std::string_view name, std::string_view value);
void envvar_unset(std::string_view name);
std::vector<std::string> envvar_get_all();
} // namespace baphy

#endif // BAPHY_UTIL_ENVVARS_HPP
