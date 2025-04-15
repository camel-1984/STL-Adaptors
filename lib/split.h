#pragma once

struct SplitAdapter {
    std::string delimiter;

    explicit SplitAdapter(std::string delimiter) : delimiter(std::move(delimiter)) {}
    template <Range R>
    auto Pipe(R&& range) const {
        std::vector<std::string> result;
        for (auto&& item : range) {
            std::string str;
            if constexpr (std::is_same_v<std::decay_t<decltype(item)>, std::stringstream>) {
                str = item.str();
            } else {
                str = item;
            }
            size_t start = 0;
            size_t end = str.find(delimiter);
            while (end != std::string::npos) {
                result.push_back(str.substr(start, end - start));
                start = end + delimiter.length();
                end = str.find(delimiter, start);
            }
            result.push_back(str.substr(start));
        }
        return result;
    }
};
inline auto Split(std::string delimiter) {
    return SplitAdapter{std::move(delimiter)};
}
