#pragma once

#include <memory>

template <typename Predicate>
class FilterAdapter {
public:
    std::shared_ptr<const Predicate> predicate_;

    explicit FilterAdapter(Predicate predicate)
        : predicate_(std::make_shared<const Predicate>(predicate)) {}
    explicit FilterAdapter(std::shared_ptr<const Predicate> predicate)
        : predicate_(predicate) {}

    template <Range R>
    class FilteredView {
    public:
        R range_;
        std::shared_ptr<const Predicate> predicate_;
        using value_type = typename R::value_type;

        FilteredView(R range, std::shared_ptr<const Predicate> predicate)
            : range_(range), predicate_(predicate) {}

        class iterator {
        public:
            using BaseIterator = typename R::iterator;
            BaseIterator current_;
            BaseIterator end_;
            std::shared_ptr<const Predicate> predicate_;
            void advance() {
                while (current_ != end_ && !(*predicate_)(*current_)) {
                    ++current_;
                }
            }
            iterator(BaseIterator begin, BaseIterator end,
                    std::shared_ptr<const Predicate> predicate)
                : current_(begin), end_(end), predicate_(predicate) {
                advance();
            }
            auto operator*() const { return *current_; }
            iterator& operator++() {
                ++current_;
                advance();
                return *this;
            }
            bool operator!=(const iterator& other) const {
                return current_ != other.current_;
            }
        };
        iterator begin() const {
            return iterator(std::begin(range_), std::end(range_), predicate_);
        }
        iterator end() const {
            return iterator(std::end(range_), std::end(range_), predicate_);
        }
    };
    template <Range R>
    auto Pipe(R&& range) {
        return FilteredView<R>(range, predicate_);
    }
};
template <typename F>
auto Filter(F&& predicate) {
    return FilterAdapter(predicate);
}
