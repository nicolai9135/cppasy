#ifndef PARAMETER_SYNTHESIS
#define PARAMETER_SYNTHESIS

#include "polytope.hpp"
#include "orthotope.hpp"
#include <string>
#include <set>
#include <tuple>
#include <exception>

/**
 * Exception in case the a given string is not convertible into a number
 */
class not_a_number : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Invalid boundary. The string you passed is not convertible into a real number. The string may be of the form [num]*[.[num]*][E[+|-][num]+] or, if it represents a rational, [num]* / [num]*.";
    }
};

/**
 * Exception in case lower bound > upper bound
 */
class not_an_interval : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Invalid interval. (lower bound) > (upper bound)";
    }
};

/**
 * Exception in case an interval was not fully specified
 */
class boundary_missing : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Boundary missing. You must bound all intervals";
    }
};

/**
 * Struct to store the user's arguments when using the GUI.
 */
struct options
{
    /**
     * Specify whether #formula_str contains the file path to the formula or
     * the formula itself
     */
    bool formula_as_file;

    /**
     * `std::string` containing the formula.
     */
    std::string formula_str;

    /**
     * Maximal depth. See polytope::depth for details.
     */
    unsigned int max_depth;

    /**
     * Variable to print on the x-axies
     */
    std::string x_name;

    /**
     * Variable to print on the y-axies
     */
    std::string y_name;

    /**
     * set containing the names of all variables used in the formula
     */
    std::set<std::string> variable_names;

    /**
     * 3-tuple (variable, begin, end) with initial intervals
     */
    std::vector<std::tuple<std::string, std::string, std::string>> initial_intervals;

    /**
     * Performs different checks on #initial_intervals.
     * \throws not_a_number
     * \throws not_an_interval
     * \throws boundary_missing
     */
    void sanity_check_intervals();
};

/**
 * struct containing safe, unsafe and unknown areas
 */
struct areas
{
    /**
     * Queue containing all areas wich are already identified as safe.
     */
    std::deque<std::unique_ptr<polytope>> safe_areas;

    /**
     * Queue containing all areas wich are already identified as unsafe.
     */
    std::deque<std::unique_ptr<polytope>> unsafe_areas;

    /**
     * Queue containing all areas wich are not idetified yet.
     */
    std::deque<std::unique_ptr<polytope>> unknown_areas;
};

/**
 * class to perform paramenter synthesis and store neccessary data
 */
class synthesis
{
private:
    /**
     * #areas for a specific synthesis
     */
    areas synthesis_areas;

    /**
     * Helper function for #print_all_areas calling #polytope::print() for every
     * element of the deque.
     */
    void print_deque(std::deque<std::unique_ptr<polytope>> &my_deque);

    /**
     * `z3::context` to store all formulas, variables, ...
     */
    z3::context ctx;

    /**
     * Formula to perform parameter synthesis on.
     */
    z3::expr formula;

    /**
     * Names of the variables. Necessary to refine boundaries.
     */
    z3::expr_vector variable_names;

    /**
     * Solver to check whether #formula has a solution within a given #polytope.
     */
    z3::solver solver_pos;

    /**
     * Solver to check whether ((not) #formula) has a solution within a given 
     * #polytope.
     */
    z3::solver solver_neg;

    /**
     * depth up to which the synthesis should be performed
     */
    unsigned int max_depth;

public:

    /**
     * Constructor. Initializes all private members according to #options.
     */
    synthesis(options o);

    /**
     * Central function of this tool, performs parameter synthesis.
     * @sideeffect repeatedly pops an element from #areas::unknown_areas and (1)
     * appends it to #areas::safe_areas OR (2) appends it to 
     * #areas::unsafe_areas OR (3) splits it and appends the new areas to 
     * #areas::unknown_areas again.
     */
    void execute();

    /**
     * 
     */
    void continue_synthesis(unsigned int increment);

    /**
     * Prints #areas::safe_areas, #areas::unsafe_areas and #areas::unknown_areas
     * into the terminal.
     */
    void print_all_areas();

    areas *get_synthesis_areas_ptr();
    z3::expr_vector get_variable_names();
};

#endif