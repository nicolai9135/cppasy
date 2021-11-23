#include "parse_boundaries.hpp"

#include <set>
#include <fstream>


std::vector<std::tuple<std::string, std::string, std::string>> parse_boundaries(std::string boundaries_file)
{
    std::set<std::tuple<std::string, std::string, std::string>> boundaries_set = {};
    std::ifstream boundaries_file_stream(boundaries_file);    
    std::string name, lower, upper;

    while (boundaries_file_stream >> name >> lower >> upper)
    {
        boundaries_set.insert({name, lower, upper});
    }

    // check for duplicates
    std::string last = "";
    for (const auto &b : boundaries_set)
    {
        std::string current = std::get<0>(b);
        if (current == last)
        {
            throw boundary_duplicate();
        }
        last = current;
    }

    // transform set to vector
    std::vector<std::tuple<std::string, std::string, std::string>> res(boundaries_set.begin(), boundaries_set.end());

    return res;
}