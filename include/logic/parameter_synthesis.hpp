#ifndef PARAMETER_SYNTHESIS
#define PARAMETER_SYNTHESIS

#include "polytope.hpp"
#include "orthotope.hpp"

/**
 * Struct to store the user's arguments.
 */
struct cli_options
{
    /**
     * File containing the formula to be evaluated.
     */
    std::string formula_file;

    /**
     * Maximal depth. See polytope::depth for details.
     */
    unsigned int max_depth;

    /**
     * File containing the variable names and initial intervals.
     */
    std::string boundaries_file;
};

/**
 * class to perform paramenter synthesis and store neccessary data
 */
class synthesis
{
private:
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
     * Constructor. Initializes all private members according to #cli_options .
     */
    synthesis(cli_options o);
    /**
     * Central function of this tool, performs parameter synthesis.
     * @sideeffect repeatedly pops an element from #unknown_areas and (1) 
     * appends it to #safe_areas OR (2) appends it to #unsafe_areas OR (3)
     * splits it and appends the new areas to #unknown_areas again.
     */
    void execute();

    /**
     * 
     */
    void continue_synthesis(unsigned int increment);

    /**
     * Prints #safe_areas, #unsafe_areas and #unknown_areas into the terminal.
     */
    void print_all_areas();
};

#endif