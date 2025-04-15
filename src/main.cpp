#include <iostream>
#include <processing.h>
#include <write.h>
#include <filter.h>
#include <split.h>
#include <transform.h>
#include <utils.h>
#include <aggregate.h>

#include <split_expected.h>
#include <dropnull.h>
#include <join.h>

bool recursive;

int main(int argc, char** argv) {

    Dir(argv[1], recursive)
    | Filter([](const std::filesystem::path& p){ return p.extension() == ".txt"; })
    | OpenFiles()
    | Split("\n")
    | Transform(
    [](std::string& token) {std::transform(
            token.begin(),
            token.end(),
            token.begin(),
            [](char c) { return std::tolower(c); }
        );
        return token;
    })
    | AggregateByKey(
        std::size_t{0},
        [](const std::string&, size_t& count) { ++count;},
        [](const std::string& token) { return token;}
      )
    | Transform([](const std::pair<std::string, size_t>& stat)
        { return std::format("{} - {}", stat.first, stat.second);})
    | Out(std::cout);

    return 0;
}