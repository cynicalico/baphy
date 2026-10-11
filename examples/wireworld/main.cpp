#include "wireworld_app.hpp"

int main(int, char *[]) {
    return baphy::run<WireworldApp>({.title = "wireworld", .size = {500, 500}, .resizable = true});
}
