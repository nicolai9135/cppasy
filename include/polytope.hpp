#ifndef POLYTOPE
#define POLYTOPE

#include <vector>
#include <deque>
#include <memory>
#include <z3++.h>

using coordinate = std::vector<z3::expr>;
using interval = std::pair<z3::expr, z3::expr>;
using intervals = std::vector<interval>;
enum splitting_heuristic { bisect_all, bisect_single };
enum sampling_heuristic { corners };

/**
 * The purpose of this tool is to perform parameter synthesis in a user-provided
 * region. To do so, the initial region is splitted multiple times into smaller
 * regions. #polytope is the class to represent one of these regions, resp.
 */
class polytope
{
protected:
    /**
     * This number represents the progress of the algorithm and is used to abort 
     * calculation at some point. The higher #depth, the more steps have been 
     * taken to come to this point. What #depth is actually representing is 
     * dependent (1) the concrete #polytope used (2) the splitting heuristic. 
     */
    int depth; 

    /**
     * Vector of ::coordinate s within the #polytope which have already 
     * been identified as safe. If this list is not empty, calling 
     * ``solver_pos.check()`` on this #polytope is not necessary anymore.
     */
    std::vector<coordinate> safe_coordinates;

    /**
     * Vector of ::coordinate s within the #polytope which have already 
     * been identified as unsafe. If this list is not empty, calling 
     * ``solver_neg.check()`` on this #polytope is not necessary anymore.
     */
    std::vector<coordinate> unsafe_coordinates;

public:
    int get_depth();
    void set_depth(int d);
    std::vector<coordinate> get_safe_coordinates();
    std::vector<coordinate> get_unsafe_coordinates();

    /**
     * The shape of a #polytope is defined through the set its boundaries. The
     * representation of a #polytope's boudaries is dependent on its shape and
     * surfaces. This function takes the #polytope dependent boundary
     * representation and puts them into `z3` format. A `z3` representation is
     * necessary for the solver to ensure that a solver only consideres
     * coordinates within the boundaries of a #polytope.
     * @param ctx `z3::context` in which the expressions should be defined
     * @param variable_names list of names of the free variables
     * @return `z3` representation of the #polytope's boundaries
     */
    virtual z3::expr_vector get_boundaries_z3(z3::context &ctx, z3::expr_vector &variable_names) = 0;

    /**
     * Prints all attributes of a #polytope. Mainly for develepement purposes
     */
    virtual void print() = 0;

    /**
     * Splits `this` #polytope into multiple new polytopes depending on the
     * provided #splitting_heuristic.
     * @param splitting_h #splitting_heuristic to be used
     */
    virtual std::deque<std::unique_ptr<polytope>> split(splitting_heuristic splitting_h) = 0;

    /**
     * takes samples at different ::coordinate s within the #polytope and stores
     * them in #safe_coordinates for #unsafe_coordinates
     * @sideeffect extends #safe_coordinates and/or #unsafe_coordinates
     * @param sampling_h #sampling_heuristic to be used
     */
    virtual void sample(sampling_heuristic sampling_h) = 0;
};

#endif