#ifndef BAPHY_UTIL_IO_HPP
#define BAPHY_UTIL_IO_HPP

#include <filesystem>
#include <iterator>
#include <optional>
#include <ranges>
#include <string>

namespace baphy {
std::optional<std::string> slurp(const std::filesystem::path &path);

template<std::ranges::view V>
    requires std::ranges::forward_range<const V> && std::same_as<std::ranges::range_value_t<const V>, char>
class normalize_lf_view : public std::ranges::view_interface<normalize_lf_view<V>> {
public:
    class iterator {
    public:
        using iterator_concept = std::forward_iterator_tag;
        using iterator_category = std::input_iterator_tag;
        using value_type = char;
        using difference_type = std::ranges::range_difference_t<const V>;

        iterator() = default;
        iterator(std::ranges::iterator_t<const V> cur, std::ranges::sentinel_t<const V> end)
            : cur_(cur),
              end_(end) {}

        char operator*() const { return *cur_ == '\r' ? '\n' : *cur_; }

        iterator &operator++() {
            const bool cr = *cur_ == '\r';
            ++cur_;
            if (cr && cur_ != end_ && *cur_ == '\n')
                ++cur_;
            return *this;
        }

        iterator operator++(int) {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        friend bool operator==(const iterator &a, const iterator &b) { return a.cur_ == b.cur_; }
        friend bool operator==(const iterator &it, std::default_sentinel_t) { return it.cur_ == it.end_; }

        std::ranges::iterator_t<const V> base() const { return cur_; }

    private:
        std::ranges::iterator_t<const V> cur_{};
        std::ranges::sentinel_t<const V> end_{};
    };

    normalize_lf_view()
        requires std::default_initializable<V>
    = default;

    explicit normalize_lf_view(V base)
        : base_(std::move(base)) {}

    V base() const &
        requires std::copy_constructible<V>
    {
        return base_;
    }
    V base() && { return std::move(base_); }

    iterator begin() const { return {std::ranges::begin(base_), std::ranges::end(base_)}; }
    [[nodiscard]] std::default_sentinel_t end() const { return std::default_sentinel; }

private:
    V base_{};
};

template<class R>
normalize_lf_view(R &&) -> normalize_lf_view<std::views::all_t<R>>;

namespace views {
struct normalize_lf_fn : std::ranges::range_adaptor_closure<normalize_lf_fn> {
    template<std::ranges::viewable_range R>
    auto operator()(R &&r) const {
        return normalize_lf_view(std::views::all(std::forward<R>(r)));
    }
};

inline constexpr normalize_lf_fn normalize_lf{};
} // namespace views
} // namespace baphy

template<class V>
constexpr bool std::ranges::enable_borrowed_range<baphy::normalize_lf_view<V>> = std::ranges::enable_borrowed_range<V>;

#endif // BAPHY_UTIL_IO_HPP
