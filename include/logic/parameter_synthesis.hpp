#ifndef PARAMETER_SYNTHESIS
#define PARAMETER_SYNTHESIS

#include "polytope.hpp"
#include "orthotope.hpp"

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
 * class to perform paramenter synthesis and store neccessary data
 */
class synthesis
{
private:
    /**
     * queue containing all areas wich are already identified as safe
     */
    std::deque<std::unique_ptr<polytope>> safe_areas;

    /**
     * queue containing all areas wich are already identified as unsafe
     */
    std::deque<std::unique_ptr<polytope>> unsafe_areas;

    /**
     * queue containing all areas wich are not idetified yet
     */
    std::deque<std::unique_ptr<polytope>> unknown_areas;

    /**
     * Helper function for #print_all_areas calling #polytope::print() for every
     * element of the deque.
     */
    void print_deque(std::deque<std::unique_ptr<polytope>> &my_deque);

    /**
     * #options struct to store synthesis settings.
     */
    options synthesis_options;

    /**
     * `z3::context` to store all formulas, variables, ...
     */
    z3::context ctx;

public:
    /**
     * Constructor. Initializes #synthesis_options with o.
     */

    synthesis(options o);
    /**
     * Central function of this tool, performs parameter synthesis according to
     * #synthesis_options .
     * @sideeffect repeatedly pops an element from #unknown_areas and (1) 
     * appends it to #safe_areas OR (2) appends it to #unsafe_areas OR (3)
     * splits it and appends the new areas to #unknown_areas again.
     */
    void execute();

    /**
     * Prints #safe_areas, #unsafe_areas and #unknown_areas into the terminal.
     */
    void print_all_areas();
};

#endif