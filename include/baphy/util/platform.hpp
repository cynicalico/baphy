#ifndef BAPHY_UTIL_PLATFORM_HPP
#define BAPHY_UTIL_PLATFORM_HPP

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#define BAPHY_PLATFORM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1 || TARGET_OS_IPHONE == 1
#define BAPHY_PLATFORM_IOS
#else
#define BAPHY_PLATFORM_MACOS
#endif
#elif defined(__ANDROID__)
#define BAPHY_PLATFORM_ANDROID
#elif defined(__linux__)
#define BAPHY_PLATFORM_LINUX
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) ||   \
    defined(__bsdi__)
#define BAPHY_PLATFORM_BSD
#else
#define BAPHY_PLATFORM_UNKNOWN
#endif

#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__) ||            \
    defined(__arm64__) || defined(__aarch64__)
#define BAPHY_ARCH_64
#else
#define BAPHY_ARCH_32
#endif

#endif // BAPHY_UTIL_PLATFORM_HPP
