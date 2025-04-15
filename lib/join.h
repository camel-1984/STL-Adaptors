#pragma once

template <typename Key, typename Value>
class KV {
public:
    using key_type = Key;
    using value_type = Value;
	Key key;
	Value value;
};

template <typename Left, typename Right>
class JoinResult {
public:
    Left left;
    std::optional<Right> right;

    bool operator==(const JoinResult& other) const {
        return left == other.left && right == other.right;
    }
};

template <typename RightRange>
class JoinAdapter {
public:
    RightRange right_range_;
    template <typename T>
    static decltype(auto) get_key(const T& item) {
        return item.key;
    }
    template <typename T>
    static decltype(auto) get_value(const T& item) {
        return item.value;
    }
    template <typename Range>
    static auto buildLookupMap(Range&& range) {
        using Value = std::decay_t<decltype(*std::begin(range))>;
        std::unordered_map<decltype(Value::key), decltype(Value::value)> map;
        for (const auto& item : range) {
            map.emplace(item.key, item.value);
        }
        return map;
    }
    explicit JoinAdapter(RightRange&& right_range)
        : right_range_(right_range) {}

    template <typename LeftRange>
    class JoinedView {
    public:
        LeftRange left_range_;
        decltype(buildLookupMap(std::declval<RightRange>())) right_map_;
        using LeftValue = decltype(get_value(*std::begin(left_range_)));
        using RightValue = decltype(get_value(*std::begin(right_range_)));
        using value_type = JoinResult<LeftValue, RightValue>;

        JoinedView(LeftRange left_range, RightRange right_range)
            : left_range_(left_range),
              right_map_(buildLookupMap(right_range)) {}

        class iterator {
        public:
            using LeftIter = decltype(std::begin(std::declval<LeftRange>()));
            using RightMap = decltype(buildLookupMap(std::declval<RightRange>()));
            using RightMapIter = typename RightMap::const_iterator;
            LeftIter left_current_;
            LeftIter left_end_;
            const RightMap* right_map_;
            std::pair<RightMapIter, RightMapIter> current_right_range_;
            RightMapIter right_current_;
            void findRightMatches() {
                if (left_current_ != left_end_) {
                    current_right_range_ = right_map_->equal_range(get_key(*left_current_));
                    right_current_ = current_right_range_.first;
                }
            }
            iterator(LeftIter left_begin, LeftIter left_end, const RightMap& right_map)
                : left_current_(left_begin), left_end_(left_end), right_map_(&right_map) {
                findRightMatches();
            }

            value_type operator*() const {
                if (right_current_ != current_right_range_.second) {
                    return {get_value(*left_current_), right_current_->second};
                }
                return {get_value(*left_current_), std::nullopt};
            }
            iterator& operator++() {
                if (right_current_ != current_right_range_.second) {
                    ++right_current_;
                    if (right_current_ == current_right_range_.second) {
                        ++left_current_;
                        findRightMatches();
                    }
                } else {
                    ++left_current_;
                    findRightMatches();
                }
                if (left_current_ == left_end_) {
                    right_current_ = RightMapIter{};
                }
                return *this;
            }
            bool operator!=(const iterator& other) const {
                if (left_current_ == left_end_ && other.left_current_ == other.left_end_) {
                    return false;
                }
                return left_current_ != other.left_current_;
            }
        };
        iterator begin() {
            return iterator(std::begin(left_range_), std::end(left_range_), right_map_);
        }
        iterator end() {
            return iterator(std::end(left_range_), std::end(left_range_), right_map_);
        }
    };
    template <typename LeftRange>
    auto Pipe(LeftRange&& left_range) {
        return JoinedView<LeftRange>(left_range, right_range_);
    }
};
template <typename RightRange>
auto Join(RightRange&& right_range) {
    return JoinAdapter<RightRange>(right_range);
}
