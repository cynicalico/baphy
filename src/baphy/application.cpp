#include "baphy/application.hpp"
#include "baphy/runner.hpp"

baphy::Application::Application()
    : runner{Runner::instance()} {
  callback_id_ = runner.nexus->acquire_id();
}

baphy::Application::~Application() {
  runner.nexus->release_id(callback_id_);
}

void baphy::Application::update(double) {}

void baphy::Application::draw() {}
