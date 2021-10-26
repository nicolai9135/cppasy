#ifndef ARGUMENT_PARSER
#define ARGUMENT_PARSER

#include <string>
#include <exception>

/**
 * Exception to handle `$ cppasy --help` option
 */
class help : public std::exception
{
};

/**
 * Exception for the case in which no input file was provided.
 */
class no_input_file : public std::exception
{
    virtual const char* what() const throw()
    {
        return "No input file provided";
    }
};

/**
 * Struct to store the user's arguments.
 */
struct options
{
    /**
     * File containing the formula to be evaluated.
     */
    std::string formula_file;

    /**
     * Maximal depth. See polytope::depth for details.
     */
    int max_depth;

    /**
     * File containing the variable names and initial intervals.
     */
    std::string boundaries_file;
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