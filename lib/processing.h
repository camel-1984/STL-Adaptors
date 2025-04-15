#pragma once

#include <functional>
#include <iostream>

template <typename T>
concept Range = requires(T& t) {
    std::begin(t);
    std::end(t);
};

template <Range R, typename Adapter>
auto operator|(R&& range, Adapter&& adapter) {
    return std::forward<Adapter>(adapter).Pipe(std::forward<R>(range));
}

class AsVectorAdapter {
public:
    template <Range R>
    auto Pipe(R&& range) const {
        using value_type = std::decay_t<decltype(*std::begin(range))>;
        std::vector<value_type> result;
        for (const auto& item : range) {
            result.push_back(item);
        }
        return result;
    }
};
inline auto AsVector() {
    return AsVectorAdapter{};
}

template <typename Iterator>
class AsDataFlowAdapter {
public:
    using iterator = Iterator;
    using const_iterator = Iterator;
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    iterator begin_;
    iterator end_;

    AsDataFlowAdapter(iterator b, iterator e) : begin_(b), end_(e) {}
    iterator begin() const { return begin_; }
    iterator end() const { return end_; }
};
template <typename Container>
auto AsDataFlow(Container& container) {
    return AsDataFlowAdapter(std::begin(container), std::end(container));
}
