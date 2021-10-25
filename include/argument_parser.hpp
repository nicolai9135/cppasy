#ifndef ARGUMENT_PARSER
#define ARGUMENT_PARSER

#include <string>

/**
 * Struct to store the user's arguments.
 */
struct options
{
    /**
     * Maximal depth. See polytope::depth for details.
     */
    int max_depth;

    /**
     * File containing the variable names and initial intervals.
     */
    std::string variable_file;

    /**
     * File containing the formula to be evaluated.
     */
    std::string formula_file;
};

/**
 * This function parses the provided arguments using `boost::program_options`
 * and returns a fitting struct #options.
 * @param argc number of arguments
 * @param argv[] `char*` containig the input
 * @return #options containing all information provided
 */
options parse_arguments(int argc, char* argv[]);

#endif