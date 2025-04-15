#pragma once

template <typename T, typename AggregationFunc, typename KeyExtractor>
class AggregateByKeyAdapter {
public:
    T initial;
    AggregationFunc aggregation_func;
    KeyExtractor key_extractor;

    AggregateByKeyAdapter(T initial, AggregationFunc aggregation_func, KeyExtractor key_extractor)
        : initial(initial),
          aggregation_func(aggregation_func),
          key_extractor(key_extractor) {}
    template <typename Range>
    auto Pipe(Range&& input_range) {
        using ElementType = decltype(*std::begin(input_range));
        using KeyType = decltype(key_extractor(std::declval<ElementType>()));
        using AccumulatedType = T;
        std::vector<KeyType> key_order;
        std::unordered_map<KeyType, AccumulatedType> accumulated;
        for (const auto& element : input_range) {
            KeyType key = key_extractor(element);
            auto it = accumulated.find(key);
            if (it == accumulated.end()) {
                accumulated[key] = initial;
                key_order.push_back(key);
                it = accumulated.find(key);
            }
            aggregation_func(element, it->second);
        }
        std::vector<std::pair<KeyType, AccumulatedType>> result;
        result.reserve(key_order.size());
        for (const auto& key : key_order) {
            result.emplace_back(key, accumulated[key]);
        }
        return result;
    }
};
template <typename T, typename AggregationFunc, typename KeyExtractor>
auto AggregateByKey(T initial, AggregationFunc aggregation_func, KeyExtractor key_extractor) {
    return AggregateByKeyAdapter<T, AggregationFunc, KeyExtractor>(initial, aggregation_func, key_extractor);
}
