#include "wireworld_app.hpp"

int main(int, char *[]) {
    return baphy::run<WireworldApp>({.title = "wireworld", .size = {1920, 1080}, .resizable = true});
}
