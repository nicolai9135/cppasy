#ifndef ARGUMENT_PARSER
#define ARGUMENT_PARSER

#include <string>
#include <exception>

#include "parameter_synthesis.hpp"

/** Exception to handle `$ cppasy --help` option */
class help : public std::exception
{
};

/** Exception for the case in which no input file was provided. */
class no_input_file : public std::exception
{
    virtual const char* what() const throw()
    {
        return "No formula file (.smt2) provided";
    }
};

/** Exception for the case in which the formula file is invalid.*/
class invalid_formula_file : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Formula file (.smt2) invalid.";
    }
};

/** Exception in case an interval is missing. */
class interval_missing : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Boundaries file invalid. It is missing an interval for at least one variable used by the formula file";
    }
};

/** Exception in case too many intervals were provided.*/
class too_many_intervals : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Boundaries file invalid. You have provided intervals for at least one unused variable";
    }
};

/** Exception in case the specified formula-file does not exist.*/
class invalid_formula_path : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Invalid file path. The specified formula-file is not a regular file.";
    }
};

/** Exception in case the specified formula-file does not exist */
class invalid_boundaries_path : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Invalid file path. The specified boundaries-file is not a regular file.";
    }
};

/** Exception in case the specified sampling heuristic does not exist */
class invalid_sampling_heuristic : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Invalid sampling heuristic. There is no sampling heuristic with the given name";
    }
};

/** Exception in case the specified splitting heuristic does not exist */
class invalid_splitting_heuristic : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Invalid splitting heuristic. There is no splitting heuristic with the given name";
    }
};

/**
 * This function parses the provided arguments using `boost::program_options`
 * and returns a fitting struct #cli_options.
 * @param argc number of arguments
 * @param argv[] `char*` containig the input
 * @return #cli_options containing all information provided
 */
options parse_arguments(int argc, char* argv[]);

#endif