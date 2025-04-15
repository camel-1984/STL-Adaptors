#pragma once

template <typename Predicate>
class TransformAdapter {
public:
    std::shared_ptr<Predicate> predicate_;

    explicit TransformAdapter(Predicate predicate)
        : predicate_(std::make_shared<Predicate>(predicate)) {}

    template <Range R>
    class TransformedView {
    public:
        R range_;
        std::shared_ptr<Predicate> predicate_;
        using value_type = typename R::value_type;

        TransformedView(R range, std::shared_ptr<Predicate> predicate)
            : range_(range), predicate_(predicate) {}

        class iterator {
        public:
            using BaseIterator = typename R::iterator;
            BaseIterator current_;
            BaseIterator end_;
            std::shared_ptr<Predicate> predicate_;

            iterator(BaseIterator begin, BaseIterator end, std::shared_ptr<Predicate> predicate)
                : current_(begin), end_(end), predicate_(predicate) {}
            auto operator*() {
                return (*predicate_)(*current_);
            }
            iterator& operator++() {
                ++current_;
                return *this;
            }
            bool operator!=(const iterator& other) const {
                return current_ != other.current_;
            }
        };
        iterator begin() {
            return iterator(std::begin(range_), std::end(range_), predicate_);
        }
        iterator end() {
            return iterator(std::end(range_), std::end(range_), predicate_);
        }
    };
    template <Range R>
    auto Pipe(R&& range) {
        return TransformedView<R>(range, predicate_);
    }
};
template <typename F>
auto Transform(F&& predicate) {
    return TransformAdapter(predicate);
}
