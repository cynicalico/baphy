#include <fmt/format.h>
#include "baphy/baphy.hpp"

int main(int, char *[]) {
  fmt::println("baphy v{}", baphy::version());

  return 0;
}
