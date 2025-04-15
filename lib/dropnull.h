#pragma once

class DropNulloptAdapter {
public:

    template <Range R>
    class DropNulloptView {
    public:
        R range_;
        using value_type = typename R::value_type;

        explicit DropNulloptView(R range)
            : range_(std::move(range)) {}

        class iterator {
        public:
            using BaseIterator = typename R::iterator;
            BaseIterator current_;
            BaseIterator end_;
            void advance() {
                while (current_ != end_ && *current_ == std::nullopt) {
                    ++current_;
                }
            }
            iterator(BaseIterator begin, BaseIterator end)
                : current_(begin), end_(end) {
                advance();
            }
            auto operator*() const {
                return *current_;
            }
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
            return iterator(std::begin(range_), std::end(range_));
        }
        iterator end() const {
            return iterator(std::end(range_), std::end(range_));
        }
    };
    template <Range R>
    auto Pipe(R&& range) {
        return DropNulloptView(range);
    }
};
inline auto DropNullopt() {
    return DropNulloptAdapter{};
}
