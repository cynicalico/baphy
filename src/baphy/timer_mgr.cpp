#include "baphy/timer_mgr.hpp"
#include <clero/clero.hpp>

bool baphy::detail::AfterTimer::update(double dt) {
    delay_ -= dt;
    return delay_ <= 0.0;
}

bool baphy::detail::AfterTimer::fire() {
    f_();
    return false;
}

bool baphy::detail::EveryTimer::update(double dt) {
    acc_ += dt;
    if (acc_ >= interval_) {
        acc_ -= interval_;
        return true;
    }
    return false;
}

bool baphy::detail::EveryTimer::fire() {
    f_();
    return true;
}

void baphy::TimerMgr::update_(double dt) {
    auto active_timers = std::move(timers_);
    timers_.clear();

    reserved_ids_.clear();
    for (const auto id: active_timers | std::views::keys)
        reserved_ids_.insert(id);

    for (auto &[id, timer]: active_timers) {
        if (!timer->update(dt) || timer->fire())
            timers_.emplace(id, std::move(timer));

        reserved_ids_.erase(id);
    }
}

baphy::TimerMgr::ID baphy::TimerMgr::gen_id_() const {
    do {
        const auto id = clero::rng(clero::int_range<ID>{0, std::numeric_limits<ID>::max()});
        if (!timers_.contains(id) && !reserved_ids_.contains(id))
            return id;
    } while (true);
}
