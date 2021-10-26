#ifndef PARAMETER_SYNTHESIS
#define PARAMETER_SYNTHESIS

#include "argument_parser.hpp"
#include "polytope.hpp"
#include "orthotope.hpp"

/**
 * Struct to store the output of a parameter synthesis.
 */
struct synthesis_queues
{
    std::deque<std::unique_ptr<polytope>> safe_areas;
    std::deque<std::unique_ptr<polytope>> unsafe_areas;
    std::deque<std::unique_ptr<polytope>> unknown_areas;
};

/**
 * Central function of this tool, performs parameter synthesis.
 * @param user_input result of #parse_arguments
 * @return struct with queues containing areas specified as safe/unsafe/unknown
 */
synthesis_queues parameter_synthesis(options user_input);

// only for developement purposes
void print_and_delete(std::deque<std::unique_ptr<polytope>> &my_deque);

#endif