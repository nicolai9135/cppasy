#ifndef PARSE_BOUNDARIES
#define PARSE_BOUNDARIES

#include <exception>
#include <vector>
#include <string>
#include <tuple>

/**
 * Exception for the case in which boundaries for at least one variable were
 * specified twice.
 */
class boundary_duplicate : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Invalid boudaries-file. Boundaries for at least one variable specified twice.";
    }
};

/**
 * Takes boundaries file and returns vector containing the boundaries. Used to
 * initialize #options.initial_intervals .
 * @param boundaries_file file path to the boundaries file
 * @return vector containing the intervals
 * \throws #boundary_duplicate
 */
std::vector<std::tuple<std::string, std::string, std::string>> parse_boundaries(std::string boundaries_file);
#endif