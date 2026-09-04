#ifndef BAPHY_UTIL_ENUM_CLASS_BITOPS_HPP
#define BAPHY_UTIL_ENUM_CLASS_BITOPS_HPP

#include <type_traits>

template<typename Enum>
constexpr std::underlying_type_t<Enum> unwrap(Enum e) {
  return static_cast<std::underlying_type_t<Enum>>(e);
}

/* Enum class bitmask operations
 */

template<typename Enum>
struct BaphyEnableBitops {
  static constexpr bool enable = false;
};

template<typename Enum>
std::enable_if_t<BaphyEnableBitops<Enum>::enable, Enum>
operator|(Enum lhs, Enum rhs) {
  using underlying = std::underlying_type_t<Enum>;
  return static_cast<Enum>(
      static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template<typename Enum>
std::enable_if_t<BaphyEnableBitops<Enum>::enable, Enum> &
operator|=(Enum &lhs, Enum rhs) {
  using underlying = std::underlying_type_t<Enum>;
  return lhs = static_cast<Enum>(
             static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template<typename Enum>
std::enable_if_t<BaphyEnableBitops<Enum>::enable, Enum>
operator&(Enum lhs, Enum rhs) {
  using underlying = std::underlying_type_t<Enum>;
  return static_cast<Enum>(
      static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template<typename Enum>
std::enable_if_t<BaphyEnableBitops<Enum>::enable, Enum> &
operator&=(Enum &lhs, Enum rhs) {
  using underlying = std::underlying_type_t<Enum>;
  return lhs = static_cast<Enum>(
             static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template<typename Enum>
std::enable_if_t<BaphyEnableBitops<Enum>::enable, bool>
is_flag_set(Enum e, Enum mask) {
  return (e & mask) == mask;
}

#define ENUM_CLASS_ENABLE_BITOPS(x)                                            \
  template<>                                                                   \
  struct BaphyEnableBitops<x> {                                                \
    static constexpr bool enable = true;                                       \
  }

#endif // BAPHY_UTIL_ENUM_CLASS_BITOPS_HPP
