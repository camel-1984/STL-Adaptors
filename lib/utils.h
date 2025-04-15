#pragma once

#include <fstream>
#include <utility>

#include "processing.h"

class OutAdaptor {
public:
    template <Range R>
    auto Pipe(R&& range) {
        bool first = true;
        for (auto it = std::begin(range); it != std::end(range); ++it) {
            if (!first) {
                std::cout << " ";
            }
            std::cout << *it;
            first = false;
        }
    }
};
inline auto Out(std::ostream&) {
    return OutAdaptor{};
}

class DirAdapter {
public:
    bool recursive;
    const char* path;
    using value_type = std::filesystem::path;

    DirAdapter(const char* path, bool recursive)
        : path(path), recursive(recursive) {}
    explicit DirAdapter(const char* path)
        : path(path) {}

    class DirViewRecursive {
    public:
        using value_type = std::filesystem::path;
        using iterator = std::filesystem::recursive_directory_iterator;
        iterator begin_;
        iterator end_;

        explicit DirViewRecursive(const char* path) {
            begin_ = iterator(path);
            end_ = iterator();
        }
        iterator begin() const {
            return begin_;
        }
        iterator end() const {
            return end_;
        }
    };

    class DirViewBasic {
    public:
        using value_type = std::filesystem::path;
        using iterator = std::filesystem::directory_iterator;
        iterator begin_;
        iterator end_;

        explicit DirViewBasic(const char* path) {
            begin_ = iterator(path);
            end_ = iterator();
        }
        iterator begin() const {
            return begin_;
        }
        iterator end() const {
            return end_;
        }
    };
    DirViewBasic create_view_basic() const {
        return DirViewBasic(path);
    }
    DirViewRecursive create_view_recursive() const {
        return DirViewRecursive(path);
    }
};
inline auto Dir(const char* path, bool recursive) {
    return DirAdapter(path, recursive).create_view_recursive();
}
inline auto Dir(const char* path) {
    return DirAdapter(path).create_view_basic();
}

class OpenFilesAdaptor {
public:
    template <typename PathIterator>
    class FileContentIterator {
    public:
        using value_type = std::string;
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        FileContentIterator(PathIterator path_it, PathIterator path_end)
            : path_it_(path_it), path_end_(path_end) {
            if (path_it_ != path_end_) {
                read_file();
            }
        }

        reference operator*() const { return current_; }
        pointer operator->() const { return &current_; }

        FileContentIterator& operator++() {
            ++path_it_;
            if (path_it_ != path_end_) {
                read_file();
            }
            return *this;
        }

        FileContentIterator operator++(int) {
            FileContentIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator!=(const FileContentIterator& other) const {
            return path_it_ != other.path_it_;
        }

        void read_file() {
            std::ifstream file((*path_it_).path());
            if (file.is_open()) {
                current_.assign(
                    std::istreambuf_iterator<char>(file),
                    std::istreambuf_iterator<char>()
                );
            } else {
                current_.clear();
            }
        }

        PathIterator path_it_;
        PathIterator path_end_;
        value_type current_;
    };

    template <typename PathRange>
    class FileContentRange {
    public:
        using iterator = FileContentIterator<decltype(std::begin(std::declval<PathRange>()))>;
        using const_iterator = FileContentIterator<decltype(std::cbegin(std::declval<PathRange>()))>;
        using value_type = typename iterator::value_type;

        explicit FileContentRange(PathRange&& range)
            : range_(std::forward<PathRange>(range)) {}

        iterator begin() const {
            return iterator(std::begin(range_), std::end(range_));
        }

        iterator end() const {
            return iterator(std::end(range_), std::end(range_));
        }

    private:
        PathRange range_;
    };

    template <Range R>
    auto Pipe(R&& range) {
        return FileContentRange<R>(std::forward<R>(range));
    }
};

inline auto OpenFiles() {
    return OpenFilesAdaptor{};
}
