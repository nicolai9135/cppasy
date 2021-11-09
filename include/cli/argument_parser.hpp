#ifndef ARGUMENT_PARSER
#define ARGUMENT_PARSER

#include <string>
#include <exception>

#include "parameter_synthesis.hpp"

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
 * This function parses the provided arguments using `boost::program_options`
 * and returns a fitting struct #cli_options.
 * @param argc number of arguments
 * @param argv[] `char*` containig the input
 * @return #cli_options containing all information provided
 */
cli_options parse_arguments(int argc, char* argv[]);

#endif