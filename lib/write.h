#pragma once

#include <sstream>

class WriteAdapter {
public:
    std::ostream* output_stream;
    char delimiter;

    WriteAdapter(std::ostream& os, char delim)
        : output_stream(&os), delimiter(delim) {}
    template <Range R>
    auto Pipe(R&& range) const {
        bool first = true;
        for (const auto& item : range) {
            if (!first) {
                *output_stream << delimiter;
            }
            *output_stream << item;
            first = false;
        }
        *output_stream << delimiter;
        return range;
    }
};
inline auto Write(std::ostream& os, char delimiter) {
    return WriteAdapter(os, delimiter);
}
