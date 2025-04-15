#pragma once

#include "processing.h"

template <Range RangeOne, Range RangeTwo, Range RangeBase>
auto operator|(std::pair<RangeOne, RangeTwo>&& lhs, RangeBase&& rhs) {
    return std::make_pair(
        std::forward<RangeOne>(lhs.first) | std::forward<RangeBase>(rhs),
        std::forward<RangeTwo>(lhs.second) | std::forward<RangeBase>(rhs)
    );
}

class SplitExpectedOp {
public:
    SplitExpectedOp() = default;
    template <typename Flow>
    auto Pipe(Flow&& flow) const {
        using ValueType = typename std::decay_t<Flow>::value_type;
        using SuccessType = typename ValueType::value_type;
        using ErrorType = typename ValueType::error_type;
        std::vector<SuccessType> successes;
        std::vector<ErrorType> errors;
        for (auto&& item : flow) {
            if (item.has_value()) {
                successes.push_back(item.value());
            } else {
                errors.push_back(item.error());
            }
        }

        class ErrorFlow {
        public:
            std::vector<ErrorType> data_;
            explicit ErrorFlow(std::vector<ErrorType>&& d) : data_(std::move(d)) {}

            class Iterator {
            public:
                using iterator_category = std::forward_iterator_tag;
                using value_type = ErrorType;
                using difference_type = std::ptrdiff_t;
                using pointer = const ErrorType*;
                using reference = const ErrorType&;
                explicit Iterator(typename std::vector<ErrorType>::const_iterator it)
                    : it_(it) {}
                reference operator*() const { return *it_; }
                Iterator& operator++() { ++it_; return *this; }
                bool operator!=(const Iterator& other) const { return it_ != other.it_; }
                typename std::vector<ErrorType>::const_iterator it_;
            };
            Iterator begin() const { return Iterator(data_.begin()); }
            Iterator end() const { return Iterator(data_.end()); }
        };

        class SuccessFlow {
        public:
            std::vector<SuccessType> data_;
            explicit SuccessFlow(std::vector<SuccessType>&& d) : data_(std::move(d)) {}

            class Iterator {
            public:
                using iterator_category = std::forward_iterator_tag;
                using value_type = SuccessType;
                using difference_type = std::ptrdiff_t;
                using pointer = const SuccessType*;
                using reference = const SuccessType&;
                explicit Iterator(typename std::vector<SuccessType>::const_iterator it)
                    : it_(it) {}
                reference operator*() const { return *it_; }
                Iterator& operator++() { ++it_; return *this; }
                bool operator!=(const Iterator& other) const { return it_ != other.it_; }
                typename std::vector<SuccessType>::const_iterator it_;
            };
            Iterator begin() const { return Iterator(data_.begin()); }
            Iterator end() const { return Iterator(data_.end()); }
        };
        return std::make_pair(ErrorFlow(std::move(errors)), SuccessFlow(std::move(successes)));
    }
};
inline SplitExpectedOp SplitExpected() {
    return SplitExpectedOp{};
}
