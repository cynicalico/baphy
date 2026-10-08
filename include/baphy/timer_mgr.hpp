#ifndef BAPHY_TIMER_MGR_HPP
#define BAPHY_TIMER_MGR_HPP

#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace baphy {
namespace detail {
class Timer {
public:
    virtual ~Timer() = default;

    virtual bool update(double dt) = 0;
    virtual bool fire() = 0;
};

class AfterTimer final : public Timer {
public:
    using Callback = std::function<void()>;

    template<typename F>
        requires std::constructible_from<Callback, F>
    AfterTimer(double delay, F &&f);

    bool update(double dt) override;
    bool fire() override;

private:
    double delay_;
    Callback f_;
};

class EveryTimer final : public Timer {
public:
    using Callback = std::function<void()>;

    template<typename F>
        requires std::constructible_from<Callback, F>
    EveryTimer(double interval, F &&f);

    bool update(double dt) override;
    bool fire() override;

private:
    double interval_;
    Callback f_;

    double acc_{0.0};
};
} // namespace detail

class TimerMgr {
public:
    using ID = std::size_t;

    template<typename F>
        requires std::constructible_from<detail::AfterTimer::Callback, F>
    ID after(double delay, F &&f);

    template<typename F>
        requires std::constructible_from<detail::EveryTimer::Callback, F>
    ID every(double interval, F &&f);

    void update_(double dt);

private:
    std::unordered_map<ID, std::unique_ptr<detail::Timer>> timers_{};
    std::unordered_set<ID> reserved_ids_{};

    ID gen_id_() const;
};

template<typename F>
    requires std::constructible_from<detail::AfterTimer::Callback, F>
detail::AfterTimer::AfterTimer(double delay, F &&f)
    : delay_{delay},
      f_{std::forward<F>(f)} {}

template<typename F>
    requires std::constructible_from<std::function<void()>, F>
detail::EveryTimer::EveryTimer(double interval, F &&f)
    : interval_{interval},
      f_{std::forward<F>(f)} {}

template<typename F>
    requires std::constructible_from<detail::AfterTimer::Callback, F>
TimerMgr::ID TimerMgr::after(double delay, F &&f) {
    const auto id = gen_id_();
    timers_.emplace(id, std::make_unique<detail::AfterTimer>(delay, std::forward<F>(f)));
    return id;
}

template<typename F>
    requires std::constructible_from<std::function<void()>, F>
TimerMgr::ID TimerMgr::every(double interval, F &&f) {
    const auto id = gen_id_();
    timers_.emplace(id, std::make_unique<detail::EveryTimer>(interval, std::forward<F>(f)));
    return id;
}
} // namespace baphy

#endif // BAPHY_TIMER_MGR_HPP
